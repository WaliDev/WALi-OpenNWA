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
#include "wali/wfa/DeterminizeWeightGen.hpp"

#include <algorithm>
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
          // FIXME: why is this ->zero()? --EED 5/11/2012
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
      sem_elem_t ret = getState(getInitialState())->weight();
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
        Trans & t ) const
    {
      ITrans const * itrans = find(p,g,q);
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
      WFA const * const_this = this;
      ITrans const * trans = const_this->find(p,g,q);
      return const_cast<ITrans*>(trans);
    }

    ITrans const * WFA::find( 
        Key p,
        Key g,
        Key q) const
    {
      KeyPair kp(p,g);
      kp_map_t::const_iterator it = kpmap.find(kp);
      if( it != kpmap.end() )
      {
        TransSet const & transSet = it->second;
        TransSet::const_iterator tsit= transSet.find(p,g,q);
        if( tsit != transSet.end() ) {
          ITrans const * itrans = *tsit;
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
      IncomingTransMap_t preds;
      setupFixpoint( wl, &preds, NULL, wt );
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
        IncomingTransMap_t::iterator incomingTransIt = preds.find(q->name());

        // Some states may have no predecessors, like
        // the initial state
        if(  incomingTransIt == preds.end() )
        {
          continue;
        }

        // Tell predecessors we have changed
        std::vector<ITrans*> & incoming = incomingTransIt->second;

        std::vector<ITrans*>::iterator transit = incoming.begin();
        for( ; transit != incoming.end() ; ++transit )
        {
          ITrans* t = *transit;
          
          // We are looking at a transition (q', _, q)
          State* qprime = state_map[t->from()];

          sem_elem_t newW = qprime->weight()->zero();

          { // BEGIN DEBUGGING
            //t->print( *waliErr << "\t++ Popped " ) << std::endl;
          } // END DEBUGGING

          assert(t->to() == q->name());

          sem_elem_t extended;
          if( query == INORDER )
            extended = t->weight()->extend( the_delta );
          else
            extended = the_delta->extend( t->weight() );
          newW = newW->combine(extended);

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
      setupFixpoint( wl,NULL,&preds );
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
        bool print_weights,
        DotAttributePrinter * attribute_printer ) const
    {
      o << "digraph \"WFA@" << std::hex << (void*)this << std::dec << "\" {\n";
      TransDotty dotter( o, print_weights, attribute_printer );
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
        if (attribute_printer) {
          attribute_printer->print_extra_attributes(stit->second, o);
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
    void WFA::setupFixpoint( Worklist<State>& wl, IncomingTransMap_t* trans, PredHash_t* preds) {
      sem_elem_t nullwt; // treated as ONE
      setupFixpoint(wl, trans, preds, nullwt);
    }

    //
    // Place WFA in state ready for fixpoint
    // Initialize weight on final states to be st (if it is NULL, then ONE) 
    //
    void WFA::setupFixpoint( Worklist<State>& wl, IncomingTransMap_t* trans, PredHash_t* preds, sem_elem_t wtFinal )
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

          if (preds != NULL) {
            PredHash_t::iterator predIt = preds->find(toKey);
            if( predIt == preds->end() ) {
              StateSet_t stateSet;
              predIt = preds->insert(toKey,stateSet).first;
            }
            //*waliErr << "Adding '" << key2str(st->name()) << "' to pred of '";
            //*waliErr << key2str(toKey) << "'\n";
            predIt->second.insert( st );
          }

          if (trans != NULL) {
            IncomingTransMap_t::iterator transIt = trans->find(toKey);
            if( transIt == trans->end() ) {
              std::vector<ITrans*> transitions;
              transIt = trans->insert(toKey, transitions).first;
            }
            //*waliErr << "Adding transition from '" << key2str(st->name()) << "' to pred of '";
            //*waliErr << key2str(toKey) << "'\n";
            transIt->second.push_back(t);
          }
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
    bool add_trans_to_accessible_states(WFA::AccessibleStateMap & accessible,
                                        Key dest, sem_elem_t weight)
    {
      if (accessible.find(dest) != accessible.end()) {
        sem_elem_t old = accessible[dest];
        weight = weight->combine(old);

        if (weight->equal(old.get_ptr())) {
          // No change
          return false;
        }
      }

      // Either this is a new accessible state or the weight changed
      accessible[dest] = weight;
      return true;
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
      AccessibleStateMap result;
      std::set<Key> worklist;
      std::set<Key> visited;

      result.insert(std::make_pair(start, getSomeWeight()->one()));
      
      worklist.insert(start);
      visited.insert(start);

      while (!worklist.empty()) {
        Key source = *worklist.begin();
        sem_elem_t weight = result[source];
        worklist.erase(worklist.begin());

        if (kpmap.find(KeyPair(source, WALI_EPSILON)) != kpmap.end()) {
          TransSet const & eps_dests = kpmap.find(KeyPair(source, WALI_EPSILON))->second;
          for (TransSet::const_iterator trans_it = eps_dests.begin();
               trans_it != eps_dests.end(); ++trans_it)
          {
            sem_elem_t dest_weight = weight->extend((*trans_it)->weight());
            Key dest = (*trans_it)->to();
            bool change = add_trans_to_accessible_states(result, dest, dest_weight);

            // Add the destination state to the worklist (maybe). We need to
            // do this if it wasn't there before or if the weight has changed.
            if (visited.find(dest) == visited.end() || change)
            {
              visited.insert(dest);
              worklist.insert(dest);
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


    typedef std::set<Key> KeySet;

    static
    bool
    any_final(WFA const & wfa, KeySet const & states)
    {
      for(KeySet::const_iterator state = states.begin();
          state != states.end(); ++state)
      {
        if (wfa.isFinalState(*state)) {
          return true;
        }
      }
      return false;
    }

    std::map<Key, KeySet>
    WFA::next_states(WFA const & wfa, KeySet const & froms)
    {
      // symbol -> keyset
      std::map<Key, KeySet> nexts;

      for(KeySet::const_iterator from = froms.begin();
          from != froms.end(); ++from)
      {
        for (kp_map_t::const_iterator kpmap_iter = wfa.kpmap.begin();
             kpmap_iter != wfa.kpmap.end(); ++kpmap_iter)
        {
          Key source = kpmap_iter->first.first;
          Key symbol = kpmap_iter->first.second;
          
          if (source == *from
              && symbol != WALI_EPSILON)
          {
            TransSet const & outgoing = kpmap_iter->second;
            for (TransSet::const_iterator trans = outgoing.begin();
                 trans != outgoing.end(); ++trans)
            {
              AccessibleStateMap eclose = wfa.epsilonClose((*trans)->to());

              for (AccessibleStateMap::const_iterator target = eclose.begin();
                   target != eclose.end(); ++target)
              {
                nexts[symbol].insert(target->first);
              }
            } // for "each" outgoing transition [part 1]
          } // for "each" outgoing transition [part 2]
        } // for "each" outgoing transition [part 3]
        
      } // for each nondeterministic possibility

      return nexts;
    }


    WFA
    WFA::semideterminize(DeterminizeWeightGen const & wg) const
    {
      std::stack<KeySet> worklist;
      std::set<Key> visited;

      WFA result;
      sem_elem_t zero = getSomeWeight()->zero();
      sem_elem_t one = getSomeWeight()->one();

      {
        // Set up initial states
        KeySet det_initial;
        
        AccessibleStateMap initials = epsilonClose(this->getInitialState());
        for (AccessibleStateMap::const_iterator initial = initials.begin();
             initial != initials.end(); ++initial)
        {
          det_initial.insert(initial->first);
        }

        Key initial_key = getKey(det_initial);
        
        result.addState(initial_key, zero);
        result.setInitialState(initial_key);
        worklist.push(det_initial);
        visited.insert(initial_key);
      }

      while (!worklist.empty())
      {
        KeySet sources = worklist.top();
        worklist.pop();

        Key sources_key = getKey(sources);
        result.addState(sources_key, zero);

        if (any_final(*this, sources)) {
          result.addFinalState(sources_key);
        }

        // symbol -> next state
        std::map<Key, KeySet> nexts = next_states(*this, sources);

        for (std::map<Key, KeySet>::const_iterator next = nexts.begin();
             next != nexts.end(); ++next)
        {
          Key symbol = next->first;
          Key target_key = getKey(next->second);

          if (next->second.size() > 0) {
            // Only add a transition if it doesn't go to {}. Why? This is
            // borne out of the difference between determinize and
            // semideterminize. Determinize "can't" produce a total
            // automaton. However, without this check, it still inserts the
            // initial transitions to {}. From the point of view of producing
            // an incomplete automaton, these transitions are dumb. So we get
            // rid of them.
            sem_elem_t weight = wg.getWeight(*this, result, sources, symbol, next->second);
            result.addTrans(sources_key, symbol, target_key, weight);
          
            std::pair<KeySet::iterator, bool> p = visited.insert(target_key);
            if (p.second) {
              // It wasn't already there
              worklist.push(next->second);
            }
          }
        }
      }
      

      return result;
    }

    WFA
    WFA::semideterminize() const
    {
      AlwaysReturnOneWeightGen wg(getSomeWeight());
      return semideterminize(wg);
    }
    
    WFA
    WFA::determinize(DeterminizeWeightGen const & wg) const
    {
      WFA det = semideterminize(wg);
      det.complete();
      return det;
    }

    WFA
    WFA::determinize() const
    {
      AlwaysReturnOneWeightGen wg(getSomeWeight());
      return determinize(wg);
    }
    
    static
    size_t
    fact(size_t n) {
      if (n == 1) {
        return 1;
      }
      else {
        return n * fact(n-1);
      }
    }


    template<typename MapType>
    typename MapType::mapped_type
    get(MapType const & m, typename MapType::key_type key)
    {
      typename MapType::const_iterator place = m.find(key);
      if (place == m.end()) {
        throw 7;
      }
      return place->second;
    }

    static
    bool
    transitions_match(TransSet const & left_trans_set,
                      TransSet const & right_trans_set,
                      std::map<Key, Key> const & left_to_right,
                      bool check_weights)
    {
      for (TransSet::const_iterator trans_it = left_trans_set.begin();
           trans_it != left_trans_set.end(); ++trans_it)
      {
        ITrans const * left_trans = *trans_it;

        Key right_from_needed = get(left_to_right, left_trans->from());
        Key right_to_needed = get(left_to_right, left_trans->to());

        TransSet::const_iterator r_place = right_trans_set.find(right_from_needed,
                                                                left_trans->stack(),
                                                                right_to_needed);
        if (r_place == right_trans_set.end()) {
          // There was no such transition
          return false;
        }

        ITrans const * right_trans = *r_place;

        if (!left_trans->weight()->equal(right_trans->weight().get_ptr())
            && check_weights)
        {
          return false;
        }
      }

      return true;
    }
    

    bool
    WFA::is_isomorphism(WFA const & left, std::vector<Key> const & left_states,
                        WFA const & right, std::vector<Key> const & right_states,
                        bool check_weights)
    {
      assert(left_states.size() == right_states.size());
      (void) check_weights;

      std::map<Key, Key> left_to_right;
      for (size_t state_index = 0; state_index < left_states.size(); ++state_index) {
        left_to_right[left_states[state_index]]
          = right_states[state_index];
      }

      for (size_t state_index = 0; state_index < left_states.size(); ++state_index) {
        Key left_state = left_states[state_index];
        Key right_state = right_states[state_index];

        if (!left.getState(left_state)->weight()->equal(
              right.getState(right_state)->weight().get_ptr())
            && check_weights)
        {
          return false;
        }

        if (left.isInitialState(left_state) != right.isInitialState(right_state)) {
          // One is the start state and the other isn't
          return false;
        }

        if (left.isFinalState(left_state) != right.isFinalState(right_state)) {
          // One accepts and the other rejects
          return false;
        }

        for (kp_map_t::const_iterator kpmap_iter = left.kpmap.begin();
             kpmap_iter != left.kpmap.end(); ++kpmap_iter)
        {
          Key left_source = kpmap_iter->first.first;
          if (left_source == left_state) {
            // We're looking at outgoing transitions from left_state
            Key right_source = left_to_right[left_source];
            Key symbol = kpmap_iter->first.second;

            kp_map_t::const_iterator r_place = right.kpmap.find(KeyPair(right_source, symbol));
            if (r_place == right.kpmap.end()) {
              // There is no outgoing transition on (source, symbol)
              return false;
            }

            TransSet const & left_trans_set = kpmap_iter->second;
            TransSet const & right_trans_set = r_place->second;

            if (!transitions_match(left_trans_set, right_trans_set,
                                   left_to_right, check_weights))
            {
              return false;
            }

            // All transitions from left/right under symbol match
          }
        }

        // Everything about left/right states themselves and outgoing
        // transitions match. Time to check the next pair.
      }

      return true;
    }

    
    bool
    WFA::isIsomorphicTo(WFA const & other) const
    {
      return isIsomorphicTo(other, true);
    }

    
    bool
    WFA::isIsomorphicTo(WFA const & other, bool check_weights) const
    {
      std::vector<Key> left_states(getStates().begin(),
                                   getStates().end());
      std::vector<Key> right_states(other.getStates().begin(),
                                    other.getStates().end());

      if (left_states.size() != right_states.size()) {
        return false;
      }

      size_t count = 0; // Sanity checking

      do {
        if (is_isomorphism(*this, left_states, other, right_states, check_weights)) {
          return true;
        }
        ++count;
      } while(std::next_permutation(right_states.begin(), right_states.end()));

      assert(count == fact(getStates().size()));
      
      return false;
    }
    

    void WFA::complete(std::set<Key> const & symbols, Key sink_state)
    {
      sem_elem_t one = getSomeWeight()->one();

      // We only add {} on-demand. If it's not reachable, then we don't want
      // it there.
      bool added_sink_state = false;

      // Maps source state to set of outgoing letters. (We don't care where
      // they go, just whether there is any.)
      std::map<Key, KeySet> outgoing;
      
      for (kp_map_t::const_iterator kpmap_iter = kpmap.begin();
           kpmap_iter != kpmap.end(); ++kpmap_iter)
      {
        if (kpmap_iter->second.size() > 0) {
          outgoing[kpmap_iter->first.first].insert(kpmap_iter->first.second);
        }
      }

      KeySet const & states = getStates();
      for (KeySet::const_iterator state = states.begin();
           state != states.end(); ++state)
      {
        for (KeySet::const_iterator symbol = symbols.begin();
             symbol != symbols.end(); ++symbol)
        {
          if (outgoing[*state].find(*symbol) == outgoing[*state].end()) {
            added_sink_state = true;
            addState(sink_state, one->zero());
            addTrans(*state, *symbol, sink_state, one);
          }
        }
      }

      if (added_sink_state) {
        for (kp_map_t::const_iterator kp_iter = kpmap.begin();
             kp_iter != kpmap.end(); ++kp_iter)
        {
          addTrans(sink_state, kp_iter->first.second, sink_state, one);
        }
      }
    }

    void WFA::complete(std::set<Key> const & symbols)
    {
      std::set<Key> empty;
      complete(symbols, getKey(empty));
    }

    void WFA::complete(Key sink_state)
    {
      std::set<Key> symbols;
      for (kp_map_t::const_iterator kpmap_iter = kpmap.begin();
           kpmap_iter != kpmap.end(); ++kpmap_iter)
      {
        symbols.insert(kpmap_iter->first.second);
      }
      complete(symbols, sink_state);
    }
    
    void WFA::complete()
    {
      std::set<Key> empty;
      complete(getKey(empty));
    }


    WFA WFA::removeEpsilons() const
    {
      WFA result(*this);

      std::stack<ITrans const *> eps_transitions;

      // Step 1:
      // Make a list of all epsilon transitions
      for (eps_map_t::const_iterator eps_map_iter=result.eps_map.begin();
           eps_map_iter != result.eps_map.end(); ++eps_map_iter)
      {
        TransSet const & transitions = eps_map_iter->second;
        for (TransSet::const_iterator trans_iter = transitions.begin();
             trans_iter != transitions.end(); ++trans_iter)
        {
          ITrans const * trans = *trans_iter;
          eps_transitions.push(trans);

          if (trans->from() == trans->to()) {
            // Not sure if this is true, but I think it is...
            std::cerr << "[WARNING] epsilon self loop found in removeEpsilons(); this may go into an infinite loop now\n";
          }
        }
      }

      //std::cout << "removeEpsilons():\n    step 1 found " << eps_transitions.size() << " transitions\n";

      // Step 2:
      // Copy transitions around epsilon transitons.
      while (eps_transitions.size() > 0) {
        ITrans const * trans = eps_transitions.top();
        eps_transitions.pop();

        Key eps_source = trans->from();
        Key eps_dest = trans->to();
        sem_elem_t eps_weight = trans->weight();

        //std::cout << "    Looking at " << key2str(eps_source) << " --> " << key2str(eps_dest) << "\n";

        // OK. kp_map_t maps from (source * stack) -> {trans}. Furthermore,
        // this is a hash map so is not in any particular order, which means
        // we have to loop over the whole dang thing. (If we had an explicit
        // list of symbols we could do (for every symbol) but we can't.)
        for(kp_map_t::const_iterator group = kpmap.begin();
            group != kpmap.end(); ++group)
        {
          if (group->first.first == eps_dest) {
            // If we get in here, we're looking at outgoing transitions from
            // (source) on *some* symbol. So loop over all the transitions
            // from that state, "copy" them to eps_source. If we find another
            // epsilon transition, be sure to add it so we do this again in
            // the future.
            TransSet const & nexts = group->second;
            for (TransSet::const_iterator next = nexts.begin(); next != nexts.end(); ++next) {
              //std::cout << "        adding " << key2str(eps_source) << " to " << key2str((*next)->to()) << " on " << key2str((*next)->stack()) << "\n";
              result.addTrans(eps_source,
                              (*next)->stack(),
                              (*next)->to(),
                              eps_weight->extend((*next)->weight()));
              if ((*next)->stack() == WALI_EPSILON) {
                eps_transitions.push(result.find(eps_source,
                                                 (*next)->stack(),
                                                 (*next)->to()));
              }
            }
          }
        }
      }
      
      // Step 3:
      // Make another list of all epsilon transitions. (We destroyed the last
      // one, and even if we hadn't, we would have needed to keep it
      // up-to-date.)
      for (eps_map_t::const_iterator eps_map_iter=result.eps_map.begin();
           eps_map_iter != result.eps_map.end(); ++eps_map_iter)
      {
        TransSet const & transitions = eps_map_iter->second;
        for (TransSet::const_iterator trans_iter = transitions.begin();
             trans_iter != transitions.end(); ++trans_iter)
        {
          ITrans const * trans = *trans_iter;
          eps_transitions.push(trans);
          
          if (trans->from() == trans->to()) {
            // Not sure if this is true, but I think it is...
            std::cerr << "[WARNING] epsilon self loop found in removeEpsilons(); this may go into an infinite loop now\n";
          }
        }
      }

      // Step 4:
      // Remove all epsilon transitions.
      while (eps_transitions.size() > 0) {
        ITrans const * trans = eps_transitions.top();
        eps_transitions.pop();
        assert(trans->stack() == WALI_EPSILON);
        if (result.isFinalState(trans->to())) {
          result.addFinalState(trans->from());
        }
        result.erase(trans->from(), WALI_EPSILON, trans->to());
        // TODO: delete trans? Who holds ownership?
      }

      // Step 5:
      // Sanity check
      for (eps_map_t::const_iterator eps_map_iter=result.eps_map.begin();
           eps_map_iter != result.eps_map.end(); ++eps_map_iter)
      {
        TransSet const & transitions = eps_map_iter->second;
        assert(transitions.size() == 0);
      }

      return result;
    }
    

    //// Prints to 'os' statistics about this WFA. 
    void WFA::printStatistics(std::ostream & os) const
    {
      TransCounter counter;
      for_each(counter);

      std::set<Key> symbols;
      for(kp_map_t::const_iterator it = kpmap.begin();
          it != kpmap.end(); ++it)
      {
        symbols.insert(it->first.second);
      }

      os << "Statistics for WFA " << this << ":\n"
         << "              states: " << numStates() << "\n"
         << "    accepting states: " << getFinalStates().size() << "\n"
         << "             symbols: " << symbols.size() << "\n"
         << "         transitions: " << counter.getNumTrans() << "\n";
    }
      
    
  } // namespace wfa

} // namespace wali

// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
