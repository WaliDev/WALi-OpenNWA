/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/DefaultWorklist.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/WeightMaker.hpp"
#include "wali/regex/AllRegex.hpp"
#include "wali/wpds/GenKeySource.hpp"

#include <iostream>
#include <vector>
#include <stack>

#define FOR_EACH_STATE(name)                                \
  State* name;                                            \
state_map_t::iterator name##it = state_map.begin();     \
state_map_t::iterator name##itEND = state_map.end();    \
for( ; name##it != name##itEND && (0 != (name = name##it->second)) ; name##it++ )

#define FOR_EACH_FINAL_STATE(name)                                  \
  State* name;                                                    \
std::set< Key >::iterator name##it = F.begin();                 \
std::set< Key >::iterator name##itEND = F.end();                \
for( ; name##it != name##itEND && (0 != (name = getState(*name##it))) ; name##it++ )

namespace wali
{
  namespace wfa
  {
    const std::string WFA::XMLTag("WFA");
    const std::string WFA::XMLQueryTag("query");
    const std::string WFA::XMLInorderTag("INORDER");
    const std::string WFA::XMLReverseTag("REVERSE");

    WFA::WFA( query_t q, progress_t prog )
        : init_state( WALI_EPSILON ),query(q),generation(0),progress(prog)
    {
      if( query == MAX ) {
        *waliErr << "[WARNING] Invalid WFA::query. Resetting to INORDER.\n";
        query = INORDER;
      }
    }

    WFA::WFA( const WFA & rhs ) : Printable()
    {
      operator=(rhs);
    }

    WFA& WFA::operator=( const WFA & rhs )
    {
      if( this != &rhs )
      {
        clear();

        // Copy important state information
        init_state = rhs.init_state;
        F = rhs.F;
        query = rhs.query;
        progress = rhs.progress;

        // Be sure to copy the state_map. Otherwise, 
        // some states that are in rhs but not actually apart
        // of a transition will be lost.
        state_map_t::const_iterator it = rhs.state_map.begin();
        for( ; it != rhs.state_map.end(); it++ ) {
          addState( it->first, it->second->weight()->zero() );
        }

        // This will populate all maps
        TransCopier copier(*this);
        rhs.for_each( copier );
        generation = rhs.generation;
      }
      return *this;
    }

    WFA::~WFA()
    {
      clear();
    }

    void WFA::clear()
    {
      /* Must manually delete all Trans objects. If reference
       * counting is used this code can be removed
       */
      TransDeleter td;
      for_each(td);


      /* Must manually delete all State objects. If reference
       * counting is used this code can be removed
       */
      state_map_t::iterator it = state_map.begin();
      state_map_t::iterator itEND = state_map.end();
      for( ; it != itEND ; it++ )
      {
        delete it->second;
        it->second = 0;
      }

      /* Clear all of the maps to release references to stale
       * objects */
      kpmap.clear();
      eps_map.clear();
      state_map.clear();
      F.clear();
      Q.clear();
      init_state = WALI_EPSILON;
    }

    //!
    // @brief set initial state
    //
    Key WFA::set_initial_state( Key key )
    {
      return setInitialState(key);
    }

    //!
    // @brief set initial state
    //
    Key WFA::setInitialState( Key key )
    {
      Key isold = init_state;
      // TODO : Add debug check to verify key exists
      init_state = key;

      return isold;
    }

    //!
    // Add parameter key to the set of final states
    //
    void WFA::add_final_state( Key key )
    {
      F.insert(key);
    }

    //!
    // Add parameter key to the set of final states
    //
    void WFA::addFinalState( Key key )
    {
      F.insert(key);
    }

    //!
    // Return true if parameter key is a final state
    //
    bool WFA::isFinalState( Key key ) const
    {
      return (F.find(key) != F.end());
    }

    //
    // @brief Return the initial state
    //
    Key WFA::initial_state() const
    {
      return init_state;
    }

    //
    //  @brief Return the initial state
    //
    Key WFA::getInitialState() const
    {
      return init_state;
    }

    //
    // Test if param key is the initial state.
    // 
    bool WFA::isInitialState( Key key ) const
    {
      return key == init_state;
    }

    //
    // Set the query
    //
    WFA::query_t WFA::setQuery( WFA::query_t newQuery )
    {
      query_t old = query;
      query = newQuery;
      return old;
    }

    //
    // get the query
    //
    WFA::query_t WFA::getQuery() const
    {
      return query;
    }

    //
    // get the current generation
    //
    size_t WFA::getGeneration() const
    {
      return generation;
    }

    //
    // Set the generation
    //
    void WFA::setGeneration(size_t g) 
    {
      generation = g;
    }

    //
    // Get some weight from the WFA, if it is non-empty 
    // If the WFA is empty, return NULL
    //
    sem_elem_t WFA::getSomeWeight() const{
      sem_elem_t ret;
      kp_map_t::const_iterator it = kpmap.begin();
      while(it != kpmap.end()) {
        const TransSet &tset = it->second;
        TransSet::const_iterator it2 = tset.begin();
        if(it2 != tset.end()) {
          return (*it2)->weight();
        }
        it ++;
      }
      return ret;
    }

    //!
    //! @brief add trans (p,g,q,se) to WFA
    //! Default creates a basic Trans object.
    //!
    void WFA::addTrans(
        Key p,
        Key g,
        Key q,
        sem_elem_t se )
    {
      addTrans( new Trans(p,g,q,se) );
    }

    //!
    //! @brief add ITrans*t to WFA
    //! Takes care of adding states and calling insert. This
    //! method (actually insert) assumes ownership of the memory
    //! pointed to by the ITrans* t.
    //!
    void WFA::addTrans( ITrans * t )
    {
      //t->print( *waliErr << "\tInserting Trans" ) << std::endl;
      insert( t );
    }

    //
    // Erase a trans from the WFA maps
    // Returns the erased trans
    //
    ITrans * WFA::eraseTransFromMaps(
        Key from,
        Key stack,
        Key to )
    {
      // Remove from kpmap
      ITrans* tKp = eraseTransFromKpMap(from,stack,to);

      if( tKp != NULL )
      {
        ITrans* tEps = eraseTransFromEpsMap(tKp);

        { // BEGIN DEBUGGING
          if( tEps != NULL )
            assert( tKp == tEps );
        } // END DEBUGGING

      }

      return tKp;
    }

    //
    // Erase a trans
    // Must remove from kpmap, eps_map, State('from')
    //
    void WFA::erase(
        Key from,
        Key stack,
        Key to )
    {
      // Remove from maps
      ITrans* t = eraseTransFromMaps(from,stack,to);

      State* state = state_map.find(from)->second;
      state->eraseTrans(t);

      delete t;
    }

    //
    // Removes State q from the WFA and any transitions leading
    // to and from q.
    //
    bool WFA::eraseState(
        Key q
        )
    {
      state_map_t::iterator it = state_map.find(q);
      if( it == state_map.end() ) {
        return false;
      }
      return eraseState( it->second );
    }

    //
    // Finds Trans(p,g,q). Invokes copy constructor on parameter t.
    // Returns true if find was successful.
    //
    bool WFA::find( 
        Key p,
        Key g,
        Key q,
        Trans & t )
    {
      ITrans *itrans = find(p,g,q);
      if(itrans != 0) {
        t = *itrans;
        return true;
      }
      return false;
    }

    //
    // Finds Trans(p,g,q) and returns a pointer to it
    // (null if not found)
    // This should only be used by friend classes because
    // it exposes a pointer to inside the WFA
    //
    ITrans* WFA::find( 
        Key p,
        Key g,
        Key q)
    {
      KeyPair kp(p,g);
      kp_map_t::iterator it = kpmap.find(kp);
      if( it != kpmap.end() )
      {
        TransSet& transSet = it->second;
        TransSet::iterator tsit= transSet.find(p,g,q);
        if( tsit != transSet.end() ) {
          ITrans* itrans = *tsit;
          return itrans;
        }
      }
      return 0;
    }


    void WFA::for_each( ConstTransFunctor & tf ) const
    {
      state_map_t::const_iterator it = state_map.begin();
      state_map_t::const_iterator itEND = state_map.end();
      for( ; it != itEND ; it++ )
      {
        const State* st = it->second;
        const TransSet & transSet = st->getTransSet();
        transSet.each(tf);
      }
    }

    void WFA::for_each( TransFunctor& tf )
    {
      state_map_t::iterator it = state_map.begin();
      state_map_t::iterator itEND = state_map.end();
      for( ; it != itEND ; it++ )
      {
        State* st = it->second;
        TransSet & transSet = st->getTransSet();
        transSet.each(tf);
      }
    }

    void WFA::duplicateStates(std::set<Key> &st, WFA &output) const {
      // Create a map from state to their renamed counterpart
      std::map< Key, Key > dup;
      std::set< Key >::const_iterator it;

      for(it = st.begin(); it != st.end(); it++) {
        Key s = *it;
        Key sprime = getKey(key_src_t(new wpds::GenKeySource(getGeneration(), s)));
        dup[s] = sprime;
      }

      // Use TransDuplicator to put in the right transitions
      TransDuplicator td(output, dup);
      for_each(td);

      // Set initial and final states in output to be the same as input
      output.setInitialState(getInitialState());

      for(it = F.begin(); it != F.end(); it++) {
        output.addFinalState(*it);
      }
      // Set generation
      output.setGeneration(getGeneration() + 1);
    }

    /*!
     * Intersect this with parameter fa. This is a wrapper
     * for intersect( WeightMaker&,WFA& ) that passes
     * the WeightMaker KeepBoth.
     *
     * @see WeightMaker
     */
    WFA WFA::intersect( WFA& fa )
    {
      KeepBoth wmaker;
      return intersect(wmaker,fa);
    }

    void WFA::intersect( WFA& fa, WFA& dest )
    {
      KeepBoth wmaker;
      intersect(wmaker,fa,dest);
    }

    /* 
     * Intersect this and fa, returning the result
     */
    WFA WFA::intersect( WeightMaker& wmaker , WFA& fa )
    {
      WFA dest;
      intersect(wmaker,fa,dest);
      return dest;
    }

    //
    // Intersect this and fa, storing the result in dest
    // TODO: Note: if this == dest there might be a problem
    //
    void WFA::intersect(
        WeightMaker& wmaker
        , WFA& fa
        , WFA& dest )
    {
      query_t lhsQuery = getQuery();
      // Hash transitions of fa on stack symbol. Then probe the hash
      // with this's transitions to add transitions to dest
      StackHasher hashfa;
      fa.for_each(hashfa);

      StackHasher hashThis;
      for_each( hashThis );

      // Store init state
      Key dest_init_state = getKey(initial_state(),fa.initial_state());

      // Store final states
      // Pairwise cross of the sets
      std::set< Key > dest_final_states;
      std::set< Key >::iterator keyit = F.begin();
      std::set< Key >::iterator keyitEND = F.end();
      for( ; keyit != keyitEND ; keyit++ )
      {
        std::set< Key >::iterator fait = fa.F.begin();
        std::set< Key >::iterator faitEND = fa.F.end();
        for( ; fait != faitEND ; fait++ )
        {
          dest_final_states.insert( getKey(*keyit,*fait) );
        }
      }

      // Do this here in case dest == this
      State *initThis = getState(getInitialState());
      State *initFa = fa.getState(fa.getInitialState());
      if( initThis == NULL || initFa == NULL )
      {
        *waliErr << "[ERROR - WFA::intersect] No initial state.\n";
        *waliErr << "\tIntersection must be empty.\n";
        return;
      }

      sem_elem_t stateWeight = 
        wmaker.make_weight( initThis->weight()->zero()
            , initFa->weight()->zero() ); 

      // Reset dest
      dest.clear();

      // Note: We need to make sure the state exists b/c
      // setInitialState cannot call addState because there is no
      // weight to call it with
      dest.addState( dest_init_state, stateWeight->zero() );
      // Set dest init state
      dest.setInitialState( dest_init_state );

      // Set dest final states
      keyit = dest_final_states.begin();
      keyitEND = dest_final_states.end();
      for( ; keyit != keyitEND ; keyit++ )
      {
        Key f = *keyit;
        dest.addState(f,stateWeight->zero());
        dest.addFinalState(f);
      }

      // Set result's query mode
      dest.setQuery( lhsQuery );

      // Perform intersection

      StackHasher::iterator stit = hashThis.begin();
      StackHasher::iterator stitEND = hashThis.end();
      for( ; stit != stitEND ; stit++ )
      {
        // Probe StackHasher outside of the inner
        StackHasher::stackmap_t::iterator stkit( hashfa.stackmap.find( stit->first ) );

        if( stkit == hashfa.stackmap.end() )
        {
          //std::cout << "\tSkipping '" << key2str(stit->first) << "'\n";
          continue;
        }
        // for each trans in (TransSet) stit->second 
        TransSet& tsThis = stit->second;
        TransSet::iterator tsit = tsThis.begin();
        TransSet::iterator tsitEND = tsThis.end();
        for( ; tsit != tsitEND ; tsit++ )
        {
          ITrans *t = *tsit;

          // for each trans in (TransSet) stkit->second
          TransSet::iterator stklsit = stkit->second.begin();
          TransSet::iterator stklsitEND = stkit->second.end();
          for( ; stklsit != stklsitEND ; stklsit++ )
          {
            ITrans*t2 = *stklsit;
            Key fromkey = getKey( t->from(),t2->from() );
            Key tokey = getKey( t->to(),t2->to() );
            sem_elem_t W = wmaker.make_weight(t ,t2);
            ITrans* newTrans = new Trans(fromkey,t->stack(),tokey,W); 
            //newTrans->print( std::cout << "\tAdding Trans: " ) << std::endl;
            dest.addTrans( newTrans );
          }
        }
      }
    }

    //
    // Calls path_summary with default Worklist
    //
    void WFA::path_summary()
    {
      DefaultWorklist<State> wl;
      path_summary(wl);
    }

    // 
    // Simply delegates to the Tarjan algorithm
    //
    regex::regex_t WFA::toRegex() {
      return TarjanBasicRegex();
    }

    //
    // Calls path_summary with default Worklist
    //
    void WFA::path_summary(sem_elem_t wt)
    {
      DefaultWorklist<State> wl;
      path_summary(wl, wt);
    }

    //
    // Computes path_summary 
    //
    void WFA::path_summary(Worklist<State> &wl)
    {
      sem_elem_t nullwt; // treated as ONE
      path_summary(wl, nullwt);
    }

    //
    // Computes path_summary
    //
    void WFA::path_summary( Worklist<State>& wl, sem_elem_t wt )
    {
      // BEGIN DEBUGGING
      int numPops = 0;
      // END DEBUGGING
      PredHash_t preds;
      setupFixpoint( wl,preds, wt );
      while( !wl.empty() )
      {
        State* q = wl.get();
        sem_elem_t the_delta = q->delta();
        q->delta() = the_delta->zero();

        { // BEGIN DEBUGGING
          numPops++;
          //q->print( *waliErr << "  Popped: " ) << std::endl;
        } // END DEBUGGING

        // Get a handle on ZERO b/c we use it alot
        sem_elem_t ZERO = q->weight()->zero();

        // Find predecessor set
        PredHash_t::iterator predHashIt = preds.find(q->name());

        // Some states may have no predecessors, like
        // the initial state
        if(  predHashIt == preds.end() )
        {
          continue;
        }

        // Tell predecessors we have changed
        StateSet_t& stateSet = predHashIt->second;
        StateSet_t::iterator stateit = stateSet.begin();
        for( ; stateit != stateSet.end() ; stateit++ )
        {
          State* qprime = *stateit;

          // For each Trans (q',x,q)
          State::iterator tit = qprime->begin();
          State::iterator titEND = qprime->end();
          // the new W(q')
          sem_elem_t newW = qprime->weight()->zero();
          // For each (q',_,q)
          for( ; tit != titEND ; tit++ )
          {
            ITrans* t = *tit; // (q',_,q)

            { // BEGIN DEBUGGING
              //t->print( *waliErr << "\t++ Popped " ) << std::endl;
            } // END DEBUGGING

            if( t->to() == q->name() ) {

              sem_elem_t extended;
              if( query == INORDER )
                extended = t->weight()->extend( the_delta );
              else
                extended = the_delta->extend( t->weight() );
              newW = newW->combine(extended);
            }

          }
          // delta => (w+se,w-se)
          // Use extended->delta b/c we want the diff b/w the new
          // weight (extended) and what was there before
          std::pair< sem_elem_t,sem_elem_t > p =
            newW->delta( qprime->weight() );

          { // BEGIN DEBUGGING
            //qprime->weight()->print( *waliErr << "   oldW " << key2str(qprime->name()) ) << std::endl;
            //newW->print( *waliErr << "   newW " << key2str(qprime->name()) ) << std::endl;
            //p.first->print( *waliErr << "\t++ p.first " ) << std::endl;
            //p.second->print( *waliErr << "\t++ p.second " ) << std::endl;
          } // END DEBUGGING

          // Sets qprime's new weight
          // p.first == (l(t) X the_delta) + W(qprime)
          qprime->weight() = p.first;

          // on the worklist?
          if( qprime->marked() ) {
            qprime->delta() = qprime->delta()->combine(p.second);
          }
          else {
            // not on the worklist means its delta is zero
            qprime->delta() = p.second;

            // add to worklist if not zero
            if( !qprime->delta()->equal(ZERO) ) {
              wl.put(qprime);
            }
          }
        }
        if( progress.is_valid() )
            progress->tick();
      }
      { // BEGIN DEBUGGING
        //*waliErr << "\n --- WFA::path_summary needed " << numPops << " pops\n";
        //*waliErr << "WFA state labels:\n";
        //FOR_EACH_STATE( st ) {
        //    *waliErr << "\t" << key2str(st->name()) << ": ";
        //    st->weight()->print( *waliErr ) << std::endl;
        //}
      } // END DEBUGGING
    }

    //
    // Removes all transitions <b>not</b> in the (init_state,F) chop
    //
    void WFA::prune()
    {

      // First, remove all transitions with zero weight
      TransZeroWeight tzw;
      for_each(tzw);
      TransSet::iterator transit = tzw.zeroWeightTrans.begin();
      for(; transit != tzw.zeroWeightTrans.end(); transit++) {
        ITrans *t = *transit;
        erase(t->from(), t->stack(), t->to());
      }
      tzw.zeroWeightTrans.clear();

      // Now, do the (init_state, F) chop
      DefaultWorklist<State> wl;
      PredHash_t preds;
      setupFixpoint( wl,preds );
      FOR_EACH_STATE(resetState) {
        resetState->tag = 0;
      }
      FOR_EACH_FINAL_STATE(finalState) {
        finalState->tag = 1;
      }

      // first backwards prune
      while( !wl.empty() )
      {
        State* q = wl.get();
        PredHash_t::iterator predHashIt = preds.find(q->name());
        if( predHashIt == preds.end() ) {
          continue;
        }
        StateSet_t& stateSet = predHashIt->second;
        StateSet_t::iterator stateIt = stateSet.begin();
        StateSet_t::iterator stateItEND = stateSet.end();
        //sem_elem_t ONE = q->weight()->one();
        //sem_elem_t ZERO = q->weight()->zero();

        for( ; stateIt != stateItEND ; stateIt++ )
        {
          State* p = *stateIt;
          if( p->tag == 0 ) {
            p->tag = 1;
            wl.put(p);
          }
        }
      }

      State* init = getState( getInitialState() );
      { // BEGIN DEBUGGING
        assert( init != 0 );
      } // END DEBUGGING

      // go from 1->2 or 0->1
      init->tag++;

      wl.put(init);
      while( ! wl.empty() ) {
        State* p = wl.get();
        TransSet& tSet = p->getTransSet();
        TransSet::iterator it = tSet.begin();
        TransSet::iterator itEND = tSet.end();
        TransSet::iterator eraseIt;
        // for each (p,_,q)
        // mark q reached
        while( it != itEND ) {
          ITrans* t = *it;
          eraseIt = it;
          it++;
          State* q = getState(t->to());
          // A state that was backwards reachable from a final state
          // will have a tag of 1. Set tag to 2 to signify it is
          // also forwards reachable
          if( q->tag == 1 ) {
            q->tag = 2;
            wl.put(q);
          }
          else if( q->tag == 0 ) {
            // A tag of 0 means the State is not backwards
            // reachable. However, since State p was on the
            // worklist it is forwards reachable and will not be
            // removed. Therefore, we must manually remove these
            // transitions.
            //
            // Simple example:
            //    (p,a) -> acc
            //    (p,b) -> bad
            //
            // If we do not remove (p,b) -> bad here, it will be
            // leftover
            eraseTransFromKpMap(t);
            eraseTransFromEpsMap(t);
            tSet.erase(eraseIt);
            delete t;
          }
        }
      }

      //
      // States that have a tag of 2 are forwards and backwards
      // reachable. Erase all other states.
      //
      FOR_EACH_STATE( eraseMe ) {
        if( eraseMe->tag != 2 ) {
          { // BEGIN DEBUGGING
            //*waliErr << "Erasing State '" << key2str(eraseMe->name()) << "'\n";
          } // END DEBUGGING
          eraseState(eraseMe);
        }
      }
    }

    //
    // Intersect (in place) with (stk \Gamma^*)
    //
    void WFA::filter(Key stk) {
      std::set<Key> stkset;
      stkset.insert(stk);
      filter(stkset);
    }

    //
    // Intersect (in place) with (stk \Gamma^*)
    //
    void WFA::filter(std::set<Key> &stkset) {
      if(kpmap.size() == 0) return;

      // Remove outgoing transitions from the init state that do not
      // have stack in stkset
      State *init = getState(getInitialState());
      assert(init != 0);

      State::iterator tit = init->begin();
      State::iterator titEND = init->end();

      // List of transitions to be deleted
      std::list<ITrans *> to_delete;
      for( ; tit != titEND ; tit++ ) {
        ITrans* t = *tit; 
        if(stkset.find(t->stack()) == stkset.end()) {
          to_delete.push_back(t);
        }
      }
      std::list<ITrans *>::iterator lit;
      for(lit = to_delete.begin(); lit != to_delete.end(); lit++) {
        ITrans* t = *lit;
        erase(t->from(), t->stack(), t->to());
      }

      // prune
      prune();
    }

    //
    // @brief print WFA to param o
    //
    std::ostream & WFA::print( std::ostream & o ) const
    {
      o << "WFA -\n";
      o << "  Initial State : ";
      o << key2str(init_state) << std::endl;

      // Q
      o << "  Q: {";
      std::set< Key >::const_iterator cit = Q.begin();
      std::set< Key >::const_iterator citEND = Q.end();
      for( bool first=true; cit != citEND ; cit++,first=false )
      {
        if(!first)
          o << ", ";
        o << key2str( *cit );
      }
      o << "}\n";

      // F
      o << "  F: {";
      cit = F.begin();
      citEND = F.end();
      for( bool first=true; cit != citEND ; cit++,first=false )
      {
        if(!first)
          o << ", ";
        o << key2str( *cit );
      }
      o << "}\n";

      TransPrinter printer( o );
      for_each( printer );

      o << "\nWeights on states: \n";
      state_map_t::const_iterator sit = state_map.begin();
      state_map_t::const_iterator sitEND = state_map.end();
      for(; sit != sitEND; sit++)
      {
        o << key2str( sit->first ) << ": \n";
        sit->second->weight()->print( o );
        o << "\n";
      }

      return o;
    }

    //
    // @brief Print WFA in dot format
    //
    std::ostream& WFA::print_dot(
        std::ostream& o,
        bool print_weights ) const
    {
      o << "digraph \"WFA@" << std::hex << (void*)this << std::dec << "\" {\n";
      TransDotty dotter( o, print_weights );
      for_each(dotter);
      state_map_t::const_iterator stit = state_map.begin();
      state_map_t::const_iterator stitEND = state_map.end();
      for( ; stit != stitEND ; stit++ )
      {
        Key key = stit->first;
        o << "\t" << key << " [label=\"";
        o << key2str(key);
        o << "\"";
        if( isInitialState(key) ) {
          o  << ",color=green,style=filled";
        }
        else if( isFinalState(key) ) {
          o  << ",color=lightblue,style=filled";
        }
        o << "];\n";
      }

      o << "}\n";
      return o;

    }

    //
    // @brief marshall WFA in XML 
    //
    std::ostream& WFA::marshall( std::ostream& o ) const
    {
      o << "<" << XMLTag;
      switch( query ) {
        case REVERSE:
          o << " " << XMLQueryTag << "='REVERSE'>\n";
          break;
        case INORDER:
        case MAX:
          o << " " << XMLQueryTag << "='INORDER'>\n";
          break;
      }

      // do initial state
      marshallState(o,init_state);

      // do final states
      std::set< Key >::const_iterator Fit = F.begin();
      std::set< Key >::const_iterator FitEND = F.end();
      for( ; Fit != FitEND ; Fit++ )
      {
        marshallState(o,*Fit);
      }
      TransMarshaller marsh(o);
      for_each(marsh);
      o << "</" << XMLTag << ">\n";
      return o;
    }

    std::ostream& WFA::marshallState( std::ostream& o,Key key ) const
    {
      // <State
      o << "\t<" << State::XMLTag;
      // _Name='<name>'
      o << " " << State::XMLNameTag << "='" << key2str( key ) << "'";
      if( isInitialState(key) ) {
        // _initial='TRUE'
        o << " " << State::XMLInitialTag << "='TRUE'";
      }
      if( isFinalState(key) ) {
        // if is final, then _final='TRUE'
        o << " " << State::XMLFinalTag << "='TRUE'";
      }
      getState(key)->weight()->marshallWeight(o << ">");
      o << "</" << State::XMLTag << ">\n";
      return o;
    }

    //
    // Inserts tnew into the WFA. If a transition matching tnew
    // exists, tnew is deleted.
    //
    ITrans* WFA::insert( ITrans* tnew )
    {
      ////
      // WFA::find code duplicated to keep
      // a handle on the kp_map_t::iterator
      ////
      ITrans* told = 0;
      kp_map_t::iterator it = kpmap.find(tnew->keypair());
      if( it != kpmap.end() )
      {
        TransSet& transSet = it->second;
        TransSet::iterator tsit= transSet.find(tnew);
        if( tsit != transSet.end() ) {
          told = *tsit;
        }
      }
      ////
      // Cases
      //  if 0 == told (told does not exist)
      //      if it == kpmap.end
      //          (p,g,*) does not exist
      //      else
      //          just add (p,g,q) to it.second
      //  else (0 != told)
      //      combine new val w/ old
      //
      ////
      if( 0 == told )
      {
        sem_elem_t ZERO( tnew->weight()->zero() );
        //*waliErr << "\tAdding 'from' state'" << key2str(t->from()) << "'\n";
        addState( tnew->from(), ZERO );
        //*waliErr << "\tAdding 'to' state '" << key2str(t->to()) << "'\n";
        addState( tnew->to(), ZERO );

        if( it == kpmap.end() )
        {
          TransSet transSet;
          it = kpmap.insert(tnew->keypair(),transSet).first;
        }
        it->second.insert(tnew);

        // Set told to tnew for the return statement at end of 
        // method
        told = tnew;

        // Add tnew to the 'to' State's reverse trans list
        state_map_t::iterator from_stit = state_map.find( tnew->from() );

        { // BEGIN DEBUGGING
          if( from_stit == state_map.end() ) {
            tnew->print( *waliErr << "\n\n+++ WTF +++\n" ) << std::endl;
            assert( Q.find(tnew->from()) != Q.end() );
            assert( from_stit != state_map.end() );
          }
        } // END DEBUGGING

        from_stit->second->addTrans( tnew );

        // if tnew is an eps transition add to eps_map
        if( tnew->stack() == WALI_EPSILON )
        {
          eps_map_t::iterator epsit = eps_map.find( tnew->to() );
          if( epsit == eps_map.end() ) {
            TransSet transSet;
            epsit = eps_map.insert( tnew->to(),transSet ).first;
          }
          epsit->second.insert( tnew );
        }
      }
      else {
        // Safety check. If told == tnew then the combine
        // is a waste of effort and the deletion is just plain
        // wrong.
        if( told != tnew ) {
          // combine new into old

          told->combineTrans( tnew );
          delete tnew;
        }
        else {
          *waliErr << "[WARNING - WFA::insert]\n";
          *waliErr << "\tAttempt to insert 'Trans' already owned by WFA.\n";
        }
      }
      return told;
    }

    TransSet WFA::match( Key p, Key y ) {
      KeyPair kp(p,y);
      kp_map_t::iterator it = kpmap.find(kp);
      return (it != kpmap.end()) ? it->second : TransSet();
    }

    //
    // Add a state to the state map
    //
    void WFA::addState( Key key , sem_elem_t zero )
    {
      if( state_map.find( key ) == state_map.end() ) {
        State* state = new State(key,zero);
        state_map.insert( key , state );
        Q.insert(key);
      }
    }

    const State* WFA::getState( Key name ) const
    {
      state_map_t::const_iterator stit = state_map.find( name );
      if( stit == state_map.end() ) {
        return NULL;
      }
      else {
        const State * state = stit->second;
        return state;
      }
    }

    const std::set< Key >& WFA::getStates() const
    {
      return Q;
    }

    const std::set< Key >& WFA::getFinalStates() const
    {
      return F;
    }

    ///////////////////////
    // Begin protected WFA
    ///////////////////////

    //
    // Return State * corresponding to the key
    //
    State * WFA::getState( Key name )
    {
      state_map_t::iterator stit = state_map.find( name );
      if( stit == state_map.end() ) {
        //throw NoSuchStateException( name );
        return NULL;
      }
      else {
        State * state = stit->second;
        //return *state;
        return state;
      }
    }

    //
    // Place WFA in state ready for fixpoint
    // Initialize weight on final states to be st (if it is NULL, then ONE) 
    //
    void WFA::setupFixpoint( Worklist<State>& wl, PredHash_t& preds) {
      sem_elem_t nullwt; // treated as ONE
      setupFixpoint(wl, preds, nullwt);
    }

    //
    // Place WFA in state ready for fixpoint
    // Initialize weight on final states to be st (if it is NULL, then ONE) 
    //
    void WFA::setupFixpoint( Worklist<State>& wl, PredHash_t& preds, sem_elem_t wtFinal )
    {
      state_map_t::iterator it = state_map.begin();
      state_map_t::iterator itEND = state_map.end();
      bool first = true;
      sem_elem_t ZERO,ONE;
      for( ; it != itEND ; it++ )
      {
        // State p
        State* st = it->second;
        // Get a handle on ONE and ZERO.
        // Do it here b/c we do not have a way to
        // get the ONE and ZERO weights w/out first
        // having a weight, i.e. st->weight()
        if( first ) {
          ONE = st->weight()->one();
          ZERO = st->weight()->zero();
          if(wtFinal.get_ptr() == NULL) {
            wtFinal = ONE;
          }
          first = false;
        }
        st->unmark();
        if( isFinalState( st->name() ) ) {
          st->weight() = wtFinal;
          st->delta() = wtFinal;
          wl.put( st );
        }
        else {
          st->weight() = ZERO;
          st->delta() = ZERO;
        }

        State::iterator stit = st->begin();
        State::iterator stEnd = st->end();

        for( ; stit != stEnd; stit++ ) {
          // (p,_,q)
          ITrans* t = *stit;
          Key toKey = t->to();
          PredHash_t::iterator predIt = preds.find(toKey);
          if( predIt == preds.end() ) {
            StateSet_t stateSet;
            predIt = preds.insert(toKey,stateSet).first;
          }
          //*waliErr << "Adding '" << key2str(st->name()) << "' to pred of '";
          //*waliErr << key2str(toKey) << "'\n";
          predIt->second.insert( st );
        }
      }
    }

    //
    // Removes Trans(from,stack,to) from the kpmap. Returns true
    // if the Trans was erased, i.e., the Trans existed.
    //
    ITrans* WFA::eraseTransFromKpMap(
        Key from,
        Key stack,
        Key to )
    {
      // ignore weight on Trans
      Trans terase(from,stack,to,0);
      return eraseTransFromKpMap(&terase);
    }

    ITrans* WFA::eraseTransFromKpMap( ITrans* terase )
    {
      ITrans* tret = NULL;
      kp_map_t::iterator kpit = kpmap.find( terase->keypair() );

      // remove from kpmap
      if( kpit != kpmap.end() ) {
        tret = kpit->second.erase(terase);
      }
      return tret;
    }

    ITrans* WFA::eraseTransFromEpsMap( ITrans* terase )
    {
      ITrans* tret = NULL;
      if( terase->stack() == WALI_EPSILON )
      {
        // remove from epsmap
        // This loop could be moved to its own method
        eps_map_t::iterator epit = eps_map.find( terase->to() );
        if( epit != eps_map.end() ) {
          tret = epit->second.erase(terase);
        }
      }
      return tret;
    }

    //
    // Removes State state from the WFA and any transitions leading
    // from the state.
    // Note: This function does not remove incoming transitions to the state -- That 
    // has to be done by the client
    //
    bool WFA::eraseState( State* state )
    {
      State::iterator it = state->begin();
      State::iterator itEND = state->end();
      for( ; it != itEND ; it++ )
      {
        ITrans* stateTrans = *it;

        //Key from = stateTrans->from();
        //Key stack = stateTrans->stack();
        //Key to = stateTrans->to();
        //Trans* tKp = eraseTransFromKpMap( from,stack,to );

        ITrans* tKp = eraseTransFromMaps(stateTrans->from(), stateTrans->stack(), stateTrans->to());

        { // BEGIN DEBUGGING
          if( tKp != NULL && tKp != stateTrans ) {
            *waliErr << "[ERROR] WFA::eraseState\n";
            tKp->print( *waliErr << "\tKpMap Trans: " ) << "\n";
            stateTrans->print( *waliErr << "\tState Trans: " ) << "\n";
            // Make sure we fail 
            assert( tKp != NULL && stateTrans == tKp );
          }
          else {
            // Fail if tKp == NULL
            assert(tKp);
          }
        } // END DEBUGGING

        delete tKp;
      }

      Q.erase(state->name());
      //F.erase(state->name());

      // Since we are not deleting the State, we need
      // to clear its TransLists
      state->clearTransSet();

      // Do <b>not</b> delete the memory
      // States are left in the state_map of the WFA and
      // reclaimed by ~WFA().
      //delete state;

      return true;
    }

    /*!
     * TODO - future uses 
     * One can imagine that a regular expression
     * for states other than the initial state is
     * desirable. To do so, either this method
     * should take the desired key or it should
     * return the resultant vector.
     *
     * @see Tarjan's paper on path expressions 
     */
    wali::regex::regex_t WFA::TarjanBasicRegex()
    {
      using namespace wali::regex;

      typedef wali::HashMap<wali::Key,int> index_map_t;
      std::vector<wali::Key> order(Q.size());
      std::vector<regex_t> nodes(Q.size()); // holds answer
      index_map_t index_map;

      {
        std::set<wali::Key>::iterator it = Q.begin();
        for( int i=0; it != Q.end() ; i++, it++ ) {
          order[i] = *it;
          index_map.insert(*it,i);
        }
      }

      const size_t n = order.size();
      regex_t **P = new regex_t*[n];
      // initialize
      for( size_t i=0 ; i < n ; i++ ) {
        P[i] = new regex_t[n];
        for( size_t j=0 ; j < n ; j++ ) {
          P[i][j] = Regex::NIL();
        }
      }
      // for e \in E, P(h(e),t(e)) := [P(h(e),t(e)) \cup e]
      for( size_t i=0; i < n ; i++ ) {
        wali::Key hd = order[i];
        State* state = getState(hd);
        TransSet::iterator it = state->begin();
        for( ; it != state->end() ; it++ ) {
          ITrans* t = *it;
          index_map_t::iterator lkup = index_map.find(t->to());
          assert( lkup != index_map.end() );
          const int tl = lkup->second;
          { // DEBUGGING
            //P[i][tl]->print( *eout << "\tOLD(" << i << "," << tl << ") : " ) << std::endl;
          }
          P[i][tl] = Regex::COMBINE(P[i][tl],Root::make(t->stack(),t->weight()));
          { // DEBUGGING
            //P[i][tl]->print( *eout << "\tP(" << i << "," << tl << ") : " ) << std::endl;
          }
        }
      }
      // loop for v = 1 until n do
      // construct the path sequence
      for( size_t v = 0; v < n ; v++ ) {
        P[v][v] = Regex::STAR(P[v][v]);
        for( size_t u = v+1; u < n ; u++ ) {

          // 0 \extend 0 == 0
          if( P[u][v]->isZero() ) continue;

          //P[u][v] = Regex::EXTEND(P[u][v],P[v][v]);
          P[u][v] = this->EXTEND(P[u][v],P[v][v]);

          for( size_t w = v+1; w < n ; w++ ) {

            // 0 \extend 0 == 0
            if( P[v][w]->isZero() ) continue;

            //P[u][w] = Regex::COMBINE(P[u][w],Regex::EXTEND(P[u][v],P[v][w]));
            P[u][w] = Regex::COMBINE(P[u][w],this->EXTEND(P[u][v],P[v][w]));
            { // DEBUGGING
              //P[u][w]->print( *eout << "\tP(" << u << "," << w << ") : " ) << std::endl;
            }
          }
        }
      }

      index_map_t::iterator lkup = index_map.find(getInitialState());
      // If there are no transitions starting from the initial state,
      // return Regex::NIL().
      //assert(lkup != index_map.end());
      if (lkup == index_map.end())
      {
        *waliErr << "[WARNING] No transitions from initial state." << std::endl;
        for( size_t i=0; i < n ; i++ ) {
          { // DEBUGGING
            //nodes[i]->print( *eout << "(" << i << ") " ) << std::endl;
          }
          delete[] P[i];
        }
        delete[] P;
        return Regex::NIL();
      }
      const int init_idx = lkup->second;

      for( size_t i=0; i < n; i++) {
        nodes[i] = Regex::NIL();
      }

      nodes[init_idx] = Regex::ID();

      for( size_t u=0; u < n ; u++) {
        for( size_t w =u; w < n ; w++) {
          if( w == u && P[u][u]->isOne() ) continue;
          if( P[u][w]->isZero() ) continue;
          if( u == w ) {
            //nodes[u] = Regex::EXTEND(nodes[u],P[u][w]);
            nodes[u] = this->EXTEND(nodes[u],P[u][w]);
          }
          else {
            //nodes[w] = Regex::COMBINE(nodes[w], Regex::EXTEND(nodes[u],P[u][w]));
            nodes[w] = Regex::COMBINE(nodes[w], this->EXTEND(nodes[u],P[u][w]));
          }
        }
      }
      for( long u = static_cast<long>(n)-1; u >= 0; u-- ) {
        for( long w = u-1; w >=0 ; w-- ) {
          if( P[u][w]->isZero() ) continue;
          if( u == w ) {
            //nodes[u] = Regex::EXTEND(nodes[u],P[u][w]);
            nodes[u] = this->EXTEND(nodes[u],P[u][w]);
          }
          else {
            //nodes[w] = Regex::COMBINE(nodes[w],Regex::EXTEND(nodes[u],P[u][w]));
            nodes[w] = Regex::COMBINE(nodes[w],this->EXTEND(nodes[u],P[u][w]));
          }
        }
      }
      // now return nodes[INDEX(getInitialState())]
      std::set< wali::Key >::iterator fit = F.begin();
      regex_t answer = Regex::NIL();
      for( ; fit != F.end() ; fit++ ) {
        wali::Key f = *fit;
        index_map_t::iterator mit = index_map.find(f);
        if( mit != index_map.end() ) {
          answer = Regex::COMBINE(answer,nodes[mit->second]);
        }
      }
      for( size_t i=0; i < n ; i++ ) {
        { // DEBUGGING
          //nodes[i]->print( *eout << "(" << i << ") " ) << std::endl;
        }
        delete[] P[i];
      }
      delete[] P;
      return answer;
    }

    regex::regex_t WFA::EXTEND(regex::regex_t a , regex::regex_t b) {
      return (query == INORDER) ?
        regex::Regex::EXTEND(a,b) : regex::Regex::EXTEND(b,a);
    }





    //////////////
    // Helper functions

    /// Adds to 'accessible' the fact that it is possible to reach the
    /// destination of the transition 'trans' with the weight of 'trans'. If
    /// that state was already known to be reachable, joins the new weight
    /// with the old one.
    void add_trans_to_accessible_states(WFA::AccessibleStateMap & accessible,
                                        Key dest, sem_elem_t weight)
    {
      if (accessible.find(dest) != accessible.end()) {
        weight = weight->combine(accessible[dest]);
      }

      accessible[dest] = weight;
    }

    /// Like dest += src. For any key in src not in dest, add it with the
    /// weight in src. For keys in both, dest's weight is set to the combine.
    void merge_state_maps(WFA::AccessibleStateMap & dest,
                          WFA::AccessibleStateMap const & src)
    {
      for (WFA::AccessibleStateMap::const_iterator state = src.begin();
           state != src.end(); ++state)
      {
        add_trans_to_accessible_states(dest, state->first, state->second);
      }
    }
    

    WFA::AccessibleStateMap
    WFA::epsilonClose(Key start) const
    {
      eps_map_t forward_eps_map;
      for (eps_map_t::const_iterator eps_it = eps_map.begin();
           eps_it != eps_map.end(); ++eps_it)
      {
        TransSet const & eps_dests = eps_it->second;
        for (TransSet::const_iterator trans_it = eps_dests.begin();
             trans_it != eps_dests.end(); ++trans_it)
        {
          forward_eps_map[(*trans_it)->from()].insert(*trans_it);
        }
      }
      
      
      AccessibleStateMap result;
      std::stack<std::pair<Key, sem_elem_t> > worklist;
      std::set<Key> visited;
      
      worklist.push(std::make_pair(start, getSomeWeight()->one()));
      visited.insert(start);

      while (!worklist.empty()) {
        Key source = worklist.top().first;
        sem_elem_t weight = worklist.top().second;
        worklist.pop();

        if (forward_eps_map.find(source) != forward_eps_map.end()) {
          TransSet const & eps_dests = forward_eps_map.find(source)->second;
          for (TransSet::const_iterator trans_it = eps_dests.begin();
               trans_it != eps_dests.end(); ++trans_it)
          {
            sem_elem_t dest_weight = weight->extend((*trans_it)->weight());
            Key dest = (*trans_it)->to();
            add_trans_to_accessible_states(result, dest, dest_weight);

            // Add the destination state to the worklist (maybe)
            if (visited.find(dest) == visited.end()) {
              visited.insert(dest);
              worklist.push(std::make_pair(dest, dest_weight));
            }
            else {
              // FIXME: update entry in worklist. (Needed?)
            }
          }
        }
      }

      return result;
    }

    
    WFA::AccessibleStateMap
    WFA::simulate(AccessibleStateMap const & start,
                  Word const & word) const
    {
      AccessibleStateMap before = start;
      AccessibleStateMap after;

      for (AccessibleStateMap::const_iterator start_it = start.begin();
           start_it != start.end(); ++start_it)
      {
        AccessibleStateMap eclose = epsilonClose(start_it->first);
        merge_state_maps(before, eclose);
      }

      for (Word::const_iterator pos = word.begin(); pos != word.end(); ++pos) {
        // Figure out where the machine will be in the next step from each of
        // the current positions.
        after.clear();
        Key letter = *pos;

        for (AccessibleStateMap::const_iterator start_config = before.begin();
             start_config != before.end(); ++start_config)
        {
          Key source = start_config->first;
          sem_elem_t source_weight = start_config->second;
          
          // Where can we go from this position?
          if (kpmap.find(KeyPair(source, letter)) != kpmap.end()) {
            TransSet const & transitions = kpmap.find(KeyPair(source, letter))->second;

            for (TransSet::const_iterator trans_it = transitions.begin();
                 trans_it != transitions.end(); ++trans_it)
            {
              // Well, it might be the target of the transition itself...
              add_trans_to_accessible_states(after, (*trans_it)->to(), (*trans_it)->weight());
              
              AccessibleStateMap eclose = epsilonClose((*trans_it)->to());
              for (AccessibleStateMap::const_iterator dest = eclose.begin();
                   dest != eclose.end(); ++dest)
              {
                add_trans_to_accessible_states(after, dest->first, dest->second);
              }
            } // For each outgoing transition
            
          }  // if there are outgoing transitions...
        } // For each possible starting configuration
        
        // Now after holds the list of starting positions. After this, before
        // will.
        swap(after, before);
        
      } // For each letter in 'word'

      return before;
    }


    bool
    WFA::isAcceptedWithNonzeroWeight(Word const & word) const
    {
      AccessibleStateMap start;
      start[getInitialState()] = NULL;

      AccessibleStateMap finish = simulate(start, word);

      std::set<Key> const & finals = getFinalStates();
      for(std::set<Key>::const_iterator final = finals.begin();
          final != finals.end(); ++final)
      {
        if (finish.find(*final) != finish.end()) {
          //  FIXME: check weight
          return true;
        }
      }

      return false;
    }


  } // namespace wfa

} // namespace wali

// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
