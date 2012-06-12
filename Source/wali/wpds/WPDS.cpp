/**
 * @author Nicholas Kidd
 * @author Akash Lal
 *
 * @version $Id$
 */

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include "wali/Worklist.hpp"
#include "wali/KeyPairSource.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/TransSet.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/Config.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Wrapper.hpp"
#include "wali/wpds/GenKeySource.hpp"
#include "wali/DefaultWorklist.hpp"
#include <iostream>
#include <cassert>

//
// TODO: 
//      look into having each trans w/ pointers to States
//      eps hash
//      how to perform lookup on (p,*,*)
//
namespace wali
{
  using wfa::Trans;
  using wfa::TransSet;
  using wfa::WFA;
  using wfa::State;

  namespace wpds
  {

    const std::string WPDS::XMLTag("WPDS");

    WPDS::WPDS() :
      wrapper(0),
      worklist( new DefaultWorklist<wfa::ITrans>() ),
      currentOutputWFA(0)
    {
    }

    WPDS::WPDS( ref_ptr<Wrapper> w ) :
      wrapper(w),
      worklist( new DefaultWorklist<wfa::ITrans>() ),
      currentOutputWFA(0)
    {
    }

    WPDS::WPDS( const WPDS& w ) :
      wali::Printable(),
      wali::wfa::ConstTransFunctor(),
      wrapper(w.wrapper),
      worklist( new DefaultWorklist<wfa::ITrans>() ),
      currentOutputWFA(0)
    {
      RuleCopier rc(*this,wrapper);
      w.for_each(rc);
    }

    WPDS::~WPDS()
    {
      //*waliErr << "~WPDS()" << std::endl;
      clear();
    }

    void WPDS::clear()
    {
      /* Delete allocated Config objects */
      chash_t::iterator cit = config_map().begin();
      chash_t::iterator citEND = config_map().end();

      for( ; cit != citEND ; cit++ )
      {
        Config* c = cit->second;
        assert(c);
        delete c;
        cit->second = 0;
      }

      /* clear everything */
      config_map().clear();
      //*waliErr << "  1. Cleared config_map()" << std::endl;

      rule_zeroes.clear();
      //*waliErr << "  2. Cleared rule_zeroes()" << std::endl;

      r2hash.clear();
      //*waliErr << "  3. Cleared r2hash()" << std::endl;

      worklist->clear();
      //*waliErr << "  4. Cleared worklist()" << std::endl;

      pds_states.clear();
      //*waliErr << "  5. Cleared pds_states()" << std::endl;
    }

    /**
     * Set the worklist used for pre and poststar queries.
     */
    void WPDS::setWorklist( ref_ptr< Worklist<wfa::ITrans> > wl )
    {
      if (wl == 0) {
        *waliErr << "[ERROR] Cannot set the worklist to NULL.\n";
        assert(0);
      }
      worklist = wl;
    }

    bool WPDS::add_rule(
        Key from_state,
        Key from_stack,
        Key to_state,
        sem_elem_t se )
    {
      return add_rule(from_state,from_stack,to_state,WALI_EPSILON,WALI_EPSILON,se );
    }

    bool WPDS::add_rule(
        Key from_state,
        Key from_stack,
        Key to_state,
        Key to_stack1,
        sem_elem_t se )
    {
      return add_rule(from_state,from_stack,to_state,to_stack1,WALI_EPSILON,se );
    }

    bool WPDS::add_rule(
        Key from_state,
        Key from_stack,
        Key to_state,
        Key to_stack1,
        Key to_stack2,
        sem_elem_t se )
    {
      rule_t r;
      bool rb = add_rule(from_state,from_stack,to_state,to_stack1,to_stack2,se, false, r );
      return rb;
    }

        bool WPDS::replace_rule(
        Key from_state,
        Key from_stack,
        Key to_state,
        sem_elem_t se )
    {
      return replace_rule(from_state,from_stack,to_state,WALI_EPSILON,WALI_EPSILON,se );
    }

    bool WPDS::replace_rule(
        Key from_state,
        Key from_stack,
        Key to_state,
        Key to_stack1,
        sem_elem_t se )
    {
      return replace_rule(from_state,from_stack,to_state,to_stack1,WALI_EPSILON,se );
    }

    bool WPDS::replace_rule(
        Key from_state,
        Key from_stack,
        Key to_state,
        Key to_stack1,
        Key to_stack2,
        sem_elem_t se )
    {
      rule_t r;
      bool rb = add_rule(from_state,from_stack,to_state,to_stack1,to_stack2,se, true, r );
      return rb;
    }

    WFA WPDS::prestar( WFA const & input )
    {
      WFA output;
      prestar(input,output);
      return output;
    }

    void WPDS::prestar( WFA const & input, WFA & fa )
    {
      prestarSetupFixpoint(input,fa);
      prestarComputeFixpoint( fa );
      unlinkOutput(fa);
      currentOutputWFA = 0;
    }

    void WPDS::setupOutput( ::wali::wfa::WFA const & input, ::wali::wfa::WFA& fa )
    {
      currentOutputWFA = &fa;
      Key init = input.getInitialState();
      std::set<Key> localF( input.getFinalStates() );
      size_t inputGeneration = input.getGeneration();
      // cannot clear if input == output
      if( &input == currentOutputWFA ) {
        WFA tmp(input);
        fa.clear();
        tmp.for_each(*this);
      }
      else {
        fa.clear();
        input.for_each(*this);
      }
      // Now copy over initial and final state information
      // that was stored before the clear() and adding
      // of transitions.
      fa.addState(init,this->theZero);
      fa.setInitialState( init );
      for (std::set<Key>::iterator cit = localF.begin();
          cit != localF.end() ; cit++)
      {
        Key f = *cit;
        fa.addState(f,this->theZero);
        fa.addFinalState(f);
      }
      currentOutputWFA->setGeneration(inputGeneration+1);
    }

    // Sets the config field to NULL
    class Unlinker : public wfa::TransFunctor 
    {
      public:
        virtual ~Unlinker() {}
        virtual void operator()(wfa::ITrans* t) 
        {
          t->setConfig(0);
        }
    };

    void WPDS::unlinkOutput( WFA& fa) 
    {
      Unlinker unlinker;
      fa.for_each(unlinker);
    }

    void WPDS::prestarSetupFixpoint( WFA const & input, WFA& fa )
    {
      setupOutput(input,fa);
      fa.setQuery(WFA::INORDER);

      //
      // do rules 0
      // rule_zeroes contains Configs of (p,WALI_EPSILON)
      //
      for( std::set< Config * >::iterator it = rule_zeroes.begin();
          it != rule_zeroes.end();
          it++ )
      {
        Config *cloc = *it;

        // For each rule that connects a Config * to the one
        // from the outer loop
        Config::reverse_iterator rit = cloc->rbegin();
        for( ; rit != cloc->rend(); rit++ )
        {
          rule_t r = *rit;

          { // BEGIN DEBUGGING
            //r->print( *waliErr << "Adding Rule0: " ) << std::endl;
          } // END DEBUGGING

          // Rule 0s generate a transition right away. Because
          // WPDS::update invokes WFA::insert we must make sure
          // that the new states are inserted into the WFA. 
          fa.addState( r->from_state(),r->weight()->zero() );
          fa.addState( r->to_state(),r->weight()->zero() );

          // add transition for rule
          update( r->from_state()
              , r->from_stack()
              , r->to_state()
              , r->weight()
              , r->from()
              );
        }
      }
    }

    void WPDS::prestarComputeFixpoint( WFA& fa )
    {

      wfa::ITrans * t;

      while( get_from_worklist( t ) )
        pre(t,fa);
    }

    void WPDS::pre( wfa::ITrans* t, WFA& fa )
    {
      //t->print(std::cout << "Popped: ") << "\n";

      // Get config
      Config * config = t->getConfig();

      { // BEGIN DEBUGGING
        assert( config );
      } // END DEBUGGING

      sem_elem_t dnew = t->getDelta();
      t->setDelta(dnew->zero());

      // For each backward rule of config
      Config::reverse_iterator bwit = config->rbegin();
      for( ; bwit != config->rend() ; bwit++ )
      {
        rule_t & r = *bwit;

        prestar_handle_trans( t,fa,r,dnew );

      }

      // check matching rule 2s 
      r2hash_t::iterator r2it = r2hash.find( t->stack() );

      // does a rule 2 exist with matching second symbol on rhs
      if( r2it != r2hash.end() )
      {
        // get reference
        std::list< rule_t > & ls = r2it->second;
        std::list< rule_t >::iterator lsit;

        // loop through list
        for( lsit = ls.begin() ; lsit != ls.end() ; lsit++ )
        {
          rule_t & r = *lsit;

          wfa::ITrans *tp = fa.find(r->to_state(),r->to_stack1(),t->from());
          if(tp != 0)
          {
            prestar_handle_call(tp, t, r, dnew);
          }
        }
      }
    }

    void WPDS::prestar_handle_call(
        wfa::ITrans * t1 ,
        wfa::ITrans * t2,
        rule_t  &r,
        sem_elem_t delta
        )
    {
      // f(r) * t1
      sem_elem_t wrtp = r->weight()->extend( t1->weight() );

      // f(r) * t2 * delta
      sem_elem_t wnew = wrtp->extend( delta );

      // update
      update( r->from()->state()
          , r->from()->stack()
          , t2->to()
          , wnew
          , r->from()
          );
    }


    void WPDS::prestar_handle_trans(
        wfa::ITrans* t ,
        WFA & fa   ,
        rule_t & r,
        sem_elem_t delta
        )
    {

      sem_elem_t wrule_trans = r->weight()->extend( delta );
      Key fstate = r->from()->state();
      Key fstack = r->from()->stack();

      if( r->is_rule2() )
      {
        KeyPair kp( t->to(),r->stack2() );
        { // BEGIN DEBUGGING
          //r->print( *waliErr << "Handling Rule 2: " ) << std::endl;
          //*waliErr << "\t(" << key2str(kp.first) << ", " << key2str(kp.second) << ",*)\n";
        } // END DEBUGGING
        WFA::kp_map_t::iterator kpit = fa.kpmap.find( kp );
        WFA::kp_map_t::iterator kpitEND = fa.kpmap.end();
        TransSet::iterator tsit;

        if( kpit != kpitEND )
        {
          TransSet& transSet = kpit->second;
          for( tsit = transSet.begin(); tsit != transSet.end(); tsit++ )
          {
            wfa::ITrans* tprime = *tsit;
            { // BEGIN DEBUGGING
              //*waliErr << "\tMatched: (" << key2str(tprime->from()) << ", ";
              //*waliErr << key2str(tprime->stack()) << ", ";
              //*waliErr << key2str(tprime->to()) << ")\n";
            } // END DEBUGGING
            sem_elem_t wtp = wrule_trans->extend( tprime->weight() );
            update( fstate
                , fstack
                , tprime->to()
                , wtp
                , r->from()
                );
          }
        }
      }
      else {
        update( fstate
            , fstack
            , t->to()
            , wrule_trans
            , r->from() 
            );
      }
    }

    WFA WPDS::poststar( WFA const & input )
    {
      WFA output(WFA::INORDER, input.progress);
      poststar(input,output);
      return output;
    }

    void WPDS::poststar( WFA const & input, WFA & fa )
    {
      poststarSetupFixpoint(input,fa);
      poststarComputeFixpoint(fa);
      unlinkOutput(fa);
      currentOutputWFA = 0;
    }

    void WPDS::poststarSetupFixpoint( WFA const & input, WFA& fa )
    {
      setupOutput(input,fa);
      fa.setQuery(WFA::REVERSE);

      // Generate midstates for each rule type two
      r2hash_t::iterator r2it = r2hash.begin();
      for( ; r2it != r2hash.end() ; r2it++ )
      {
        std::list< rule_t > & ls = r2it->second;
        std::list< rule_t >::iterator rlsit = ls.begin();
        for( ; rlsit != ls.end() ; rlsit++ )
        {
          rule_t & r = *rlsit;
          Key gstate = gen_state( r->to_state(),r->to_stack1() );
          fa.addState( gstate,r->weight()->zero() );
        }
        if( fa.progress.is_valid() )
            fa.progress->tick();
      }
    }

    void WPDS::poststarComputeFixpoint( WFA& fa )
    {
      wfa::ITrans* t;

      while( get_from_worklist( t ) ) 
      {
        post( t , fa );
        if( fa.progress.is_valid() )
            fa.progress->tick();
      }
    }

    void WPDS::post( wfa::ITrans* t, WFA& fa )
    {
      // Get config
      Config * config = t->getConfig();

      // Get delta
      sem_elem_t dnew = t->getDelta();

      // Reset delta of t to zero to signify completion
      // of work for that delta
      t->setDelta(theZero);

      // For each forward rule of config
      // Apply rule to create new transition
      if( WALI_EPSILON != t->stack() )
      {
        Config::iterator fwit = config->begin();
        for( ; fwit != config->end() ; fwit++ ) {
          rule_t & r = *fwit;
          poststar_handle_trans( t,fa,r,dnew );
        }
      }
      else {
        // (p,eps,q) + (q,y,q') => (p,y,q')
        State * state = fa.getState( t->to() );
        State::iterator it = state->begin();
        for(  ; it != state->end() ; it++ )
        {
          wfa::ITrans* tprime = *it;
          poststar_handle_eps_trans(t, tprime, dnew);
        }
      }
    }

    void WPDS::poststar_handle_eps_trans(wfa::ITrans *teps, wfa::ITrans*tprime, sem_elem_t delta)
    {
      sem_elem_t wght = tprime->poststar_eps_closure( delta );
      Config * config = make_config( teps->from(),tprime->stack() );
      update( teps->from()
          , tprime->stack()
          , tprime->to()
          , wght
          , config
          );
    }

    void WPDS::poststar_handle_trans(
        wfa::ITrans* t ,
        WFA & fa   ,
        rule_t & r,
        sem_elem_t delta
        )
    {
      Key rtstate = r->to_state();
      Key rtstack = r->to_stack1();
      sem_elem_t wrule_trans = delta->extend( r->weight() );

      if( r->to_stack2() == WALI_EPSILON ) {
        // t must be a rule 1 (pop rules handled by poststar_handle_eps_trans)
        update( rtstate, rtstack, t->to(), wrule_trans, r->to() );
      }
      else {

        // Is a rule 2 so we must generate a state
        // and create 2 new transitions
        Key gstate = gen_state( rtstate,rtstack );

        wfa::ITrans* tprime = 
          update_prime( gstate, t, r, delta, wrule_trans );

        State * state = fa.getState( gstate );

        sem_elem_t quasi = state->quasi->combine( wrule_trans->quasi_one() );
        state->quasi = quasi;

        update( rtstate, rtstack, gstate, quasi, r->to() );

        // Trans with generated from states do not go on the worklist
        // and there is no Config matching them so pass 0 (NULL) as the
        // Config * for update_prime
        if( tprime->modified() )
        {

          WFA::eps_map_t::iterator epsit = fa.eps_map.find( tprime->from() );
          if( epsit != fa.eps_map.end() )
          {
            // tprime stack key
            Key tpstk = tprime->stack();
            // tprime to key
            Key tpto = tprime->to();
            // get epsilon list ref
            TransSet& transSet = epsit->second;
            // iterate
            TransSet::iterator tsit = transSet.begin();
            for( ; tsit != transSet.end() ; tsit++ )
            {
              wfa::ITrans* teps = *tsit;
              Config * config = make_config( teps->from(),tpstk );
              sem_elem_t epsW = tprime->getDelta()->extend( teps->weight() );

              update( teps->from(),tpstk,tpto,
                  epsW, config );
            }
          }
        }
      }
    }

    /**
     * Generate's a key representing the entry point to a procedure
     */
    Key WPDS::gen_state( Key state, Key stack )
    {
      return getKey(
          new GenKeySource(
            currentOutputWFA->getGeneration(),
            getKey(state,stack)));
    }


    std::ostream & WPDS::print( std::ostream & o ) const
    {
      RulePrinter rp(o);
      for_each( rp );
      return o;
    }

    std::ostream & WPDS::marshall( std::ostream & o ) const
    {
      RuleMarshaller rm(o);
      o << "<" << XMLTag << ">\n";
      for_each( rm );
      o << "</" << XMLTag << ">\n";
      return o;
    }

    std::ostream & WPDS::print_dot( std::ostream & o, bool print_weights) const
    {
      o << "digraph \"WPDS@" << std::hex << (void*)this << std::dec << "\" {\n";

      RuleDotty rd(o, print_weights);
      for_each( rd );

      o << "}\n";
      return o;
    }

    int WPDS::count_rules() const {
      int cnt = 0;
      const_iterator it = config_map().begin();
      for( ; it != config_map().end() ; it++ ) {
        const Config *c = config_map().value( it );
        Config::const_iterator rit = c->begin();
        for( ; rit != c->end() ; rit++ )
          cnt++;
      }
      return cnt;
    }

    void WPDS::for_each( ConstRuleFunctor &func ) const
    {
      const_iterator it = config_map().begin();
      for( ; it != config_map().end() ; it++ ) {
        const Config *c = config_map().value( it );
        Config::const_iterator rit = c->begin();
        for( ; rit != c->end() ; rit++ )
          func( *rit );
      }
    }

    void WPDS::for_each( RuleFunctor &func )
    {
      iterator it = config_map().begin();
      for( ; it != config_map().end() ; it++ ) {
        Config *c = config_map().value( it );
        Config::iterator rit = c->begin();
        for( ; rit != c->end() ; rit++ )
          func( *rit );
      }
    }

    bool WPDS::is_pds_state(Key k) const {
      return pds_states.find(k) != pds_states.end();
    }

    Key WPDS::constructCFG(std::set<Key> &entries, std::map<Key, Key> &entryState, wfa::WFA &out) {

      if(pds_states.size() != 1) {
        *waliErr << "Error: constructCFG can only handle single-state PDSs\n";
        assert(0);
      }

      Key start_state = *pds_states.begin();

      // Get all WPDS symbols
      WpdsStackSymbols syms;
      for_each(syms);

      // Create an automaton Agrow with transitions (start_state, e, <start_state,e>)
      // for each entry point node e.
      wfa::WFA Agrow;
      std::set<Key>::iterator it; 

      for(it = entries.begin(); it != entries.end(); it++) {
        syms.entryPoints.insert(*it);
      }

      // Need to get the WFA::generation correct for the mid-states so that
      // new mid-states are not created while running poststar        
      Agrow.setGeneration(Agrow.getGeneration() + 1);
      currentOutputWFA = &Agrow; 

      for(it = syms.entryPoints.begin(); it != syms.entryPoints.end(); it++) {
        Key entry = *it;
        Key estate = gen_state(start_state, entry);
        Agrow.addTrans(start_state, entry, estate, theZero->one());
        entryState[entry] = estate;
      }
      Agrow.setInitialState(start_state);

      // Set the generation back to original value
      Agrow.setGeneration(Agrow.getGeneration() - 1);
      currentOutputWFA = 0;

      // Run poststar
      poststar(Agrow, out);

      return start_state;
    }

    /////////////////////////////////////////////////////////////////
    // Protected Methods
    /////////////////////////////////////////////////////////////////

// Actually adds the rule. Allows for 2 return values, namely
    // the bool and a rule_t which contains a pointer to the real Rule.
    // This allows for subclasses to perform postprocessing.
    // Replaces the weight of an existing rule if replace_weight is set.
    bool WPDS::add_rule(
        Key from_state,
        Key from_stack,
        Key to_state,
        Key to_stack1,
        Key to_stack2,
        sem_elem_t se,
        bool replace_weight,
        rule_t& r
        )
    {
      // Every rule must have these 3 pieces defined
      // TODO - raise an exception?
      assert( from_state != WALI_EPSILON );
      assert( from_stack != WALI_EPSILON );
      assert( to_state   != WALI_EPSILON );
      Config *from = make_config(from_state,from_stack);
      Config *to = make_config(to_state,to_stack1);

      // Mark the states as PDS states
      pds_states.insert(from_state);
      pds_states.insert(to_state);

      // make_rule will create links b/w Configs and the Rule
      r = new Rule(from, to, to_stack2, se);
      bool rb = make_rule(from,to,to_stack2,replace_weight,r);

      // if rb is false then the rule is new
      if( !rb ) {
        if( to_stack1 == WALI_EPSILON )
        {
          // Invariant assertion.
          assert( to_stack2 == WALI_EPSILON );
          rule_zeroes.insert( to );
        }
        else if( to_stack2 != WALI_EPSILON ) {
          r2hash_t::iterator r2it = r2hash.find( to_stack2 );
          if( r2it == r2hash.end() ) {
            std::list< rule_t > ls;
            r2it = r2hash.insert( to_stack2,ls ).first;
          }
          r2it->second.push_back( r );
        }
      }
      // Set up theZero weight
      if (!theZero.is_valid() && r->weight().is_valid())
        theZero = r->weight()->zero();
      return rb;
    }

    // Actually adds the rule. Allows for 2 return values, namely
    // the bool and a rule_t which contains a pointer to the real Rule.
    // This allows for subclasses to perform postprocessing.
    bool WPDS::add_rule(
        Key from_state,
        Key from_stack,
        Key to_state,
        Key to_stack1,
        Key to_stack2,
        sem_elem_t se,
        rule_t& r
        )
    {
      // Every rule must have these 3 pieces defined
      // TODO - raise an exception?
      assert( from_state != WALI_EPSILON );
      assert( from_stack != WALI_EPSILON );
      assert( to_state   != WALI_EPSILON );
      Config *from = make_config(from_state,from_stack);
      Config *to = make_config(to_state,to_stack1);

      // Mark the states as PDS states
      pds_states.insert(from_state);
      pds_states.insert(to_state);

      // make_rule will create links b/w Configs and the Rule
      r = new Rule(from, to, to_stack2, se);
      bool rb = make_rule(from,to,to_stack2,r);

      // if rb is false then the rule is new
      if( !rb ) {
        if( to_stack1 == WALI_EPSILON )
        {
          // Invariant assertion.
          assert( to_stack2 == WALI_EPSILON );
          rule_zeroes.insert( to );
        }
        else if( to_stack2 != WALI_EPSILON ) {
          r2hash_t::iterator r2it = r2hash.find( to_stack2 );
          if( r2it == r2hash.end() ) {
            std::list< rule_t > ls;
            r2it = r2hash.insert( to_stack2,ls ).first;
          }
          r2it->second.push_back( r );
        }
      }
      // Set up theZero weight
      if (!theZero.is_valid() && r->weight().is_valid())
        theZero = r->weight()->zero();
      return rb;
    }


    /**
     * Creates a Config if one does not already exist
     * with KeyPair (state,stack).
     *
     * @return Config(state,stack)
     */
    Config * WPDS::make_config( Key state, Key stack )
    {
      Config *cf = find_config( state,stack );
      if( 0 == cf ) {
        cf = new Config(state,stack);
        KeyPair kp(state,stack);
        config_map().insert( kp,cf );
      }
      return cf;
    }

/**
     * Creates a rule that links two configurations.
     * If rule exists then (combines the weight if replace_weight is false) or
     * (replace the weight if replace_weight is true)
     * and drops the rule passed in.
     *
     * @return true if Rule already existed
     *
     * @see Config
     * @see sem_elem_t
     * @see rule_t
     */
    bool WPDS::make_rule(
        Config *f,
        Config *t,
        Key stk2,
        bool replace_weight,
        rule_t& r )
    {
      bool exists = false;
      Config::iterator it = f->begin();
      Config::iterator itEND = f->end();

      for( ; it != itEND; it++ ) {
        rule_t tmp = *it;
        if( tmp->f == f && tmp->t == t && tmp->to_stack2() == stk2 )
        {
          exists = true;
          if( wrapper.is_valid() ) {
            // New behavior
            // Wrap the duplicate rule's weight, then combine since
            // wrapping produces a new semiring.
            r->setWeight( wrapper->wrap(*r) );
          }

          if(!replace_weight) {
            sem_elem_t x = tmp->weight()->combine(r->weight()); 
            r->setWeight(x);
          }
          // This copy operation also copies other things that might sit on the
          // rule (i.e., merge functions)
          tmp->copy(r);
          r = tmp;
          break;
        }
      }
      if( !exists ) {
        if( wrapper.is_valid() ) {
          r->setWeight( wrapper->wrap(*r) );
        }
        f->insert(r);
        t->rinsert(r);
      }
      return exists;
    }


     /**
     * Creates a rule that links two configurations.
     * If rule exists then combines the weight and drops
     * the rule passed in.
     *
     * @return true if Rule already existed
     *
     * @see Config
     * @see sem_elem_t
     * @see rule_t
     */
    bool WPDS::make_rule(
        Config *f,
        Config *t,
        Key stk2,
        rule_t& r )
    {
      bool exists = false;
      Config::iterator it = f->begin();
      Config::iterator itEND = f->end();

      for( ; it != itEND; it++ ) {
        rule_t tmp = *it;
        if( tmp->f == f && tmp->t == t && tmp->to_stack2() == stk2 )
        {
          exists = true;
          if( wrapper.is_valid() ) {
            // New behavior
            // Wrap the duplicate rule's weight, then combine since
            // wrapping produces a new semiring.
            r->setWeight( wrapper->wrap(*r) );
          }
          sem_elem_t x = tmp->weight()->combine(r->weight());
          tmp->setWeight(x);
          r = tmp;
          break;
        }
      }
      if( !exists ) {
        if( wrapper.is_valid() ) {
          r->setWeight( wrapper->wrap(*r) );
        }
        f->insert(r);
        t->rinsert(r);
      }
      return exists;
    }


    Config * WPDS::find_config( Key state, Key stack )
    {
      Config *cf = 0;
      KeyPair kp(state,stack);
      iterator it = config_map().find(kp);
      if( it != config_map().end() ) {
        cf = it->second;
      }
      return cf;
    }

    /////////////////////////////////////////////////////////////////
    // Protected WPDS methods
    /////////////////////////////////////////////////////////////////

    bool WPDS::get_from_worklist( wfa::ITrans* & t )
    {
      if( !worklist->empty() ) {
        t = worklist->get();
        return true;
      }
      else {
        // t is a reference to a pointer so
        // this NULLs out the pointer t.
        t = 0;
        return false;
      }
    }

    /**
     * @brief helper function to create and link a transition
     *
     */
    void WPDS::update(
        Key from,
        Key stack,
        Key to,
        sem_elem_t se,
        Config * cfg
        )
    {
      wfa::ITrans*t = currentOutputWFA->insert(new Trans(from,stack,to,se));
      t->setConfig(cfg);
      if (t->modified()) {
        //t->print(std::cout << "Adding transition: ") << "\n";
        worklist->put( t );
      }
    }

    /**
     * update_prime does not need to take a Config b/c no Config
     * will match a transition taht is created here. The from state
     * is not \in WFA.P. Therefore we do not need to add it to the
     * worklist.
     *
     * @return generated transition
     */
    wfa::ITrans* WPDS::update_prime(
        Key from, //<! Guaranteed to be a generated state
        wfa::ITrans* call, //<! The call transition
        rule_t r, //<! The push rule
        sem_elem_t delta ATTR_UNUSED, //<! Delta change on the call transition
        sem_elem_t wWithRule //<! delta \extends r->weight()
        )
    {
      wfa::ITrans* tmp = new Trans(from,r->to_stack2(),call->to(),wWithRule);
      wfa::ITrans* t = currentOutputWFA->insert(tmp);
      return t;
    }

    /////////////////////////////////////////////////////////////////
    // Implement TransFunctor.
    // This is used to copy Transitions from the input automaton
    // to the output. 
    // This sets up the links for running saturation.
    /////////////////////////////////////////////////////////////////
    void WPDS::operator()( wfa::ITrans const * orig )
    {
      if( wali::is_strict() && is_pds_state(orig->to())) {
        *waliErr << "[ERROR] WALi cannot have incoming transition to a PDS state\n";
        orig->print( *waliErr << "    ") << std::endl;
        //assert(0);
      }

      Config* c = make_config( orig->from(),orig->stack() );
      wfa::ITrans* t = orig->copy();
      t->setConfig(c);
      if (wrapper.is_valid()) {
        t->setWeight( wrapper->wrap(*orig) );
      }

      // fa.addTrans takes ownership of passed in pointer
      currentOutputWFA->addTrans( t );

      // add t to the worklist for saturation
      worklist->put( t );
    }


    void WPDS::printStatistics(std::ostream & os) const
    {
      WpdsStackSymbols stack;
      for_each(stack);

      WpdsRules rules;
      for_each(rules);
      
      os << "Statistics for WPDS " << this << ":\n"
         << "   control states: " << num_pds_states() << "\n"
         << "\n"
         << "   stack symbols: " << stack.gamma.size() << "\n"
         << "   call points:   " << stack.callPoints.size() << "\n"
         << "   entry points:  " << stack.entryPoints.size() << "\n"
         << "   return points: " << stack.returnPoints.size() << "\n"
         << "\n"
         << "   rules:  " << rules.pushRules.size() + rules.popRules.size() + rules.stepRules.size() << "\n"
         << "   pushes: " << rules.pushRules.size() << "\n"
         << "   steps:  " << rules.stepRules.size() << "\n"
         << "   pops:   " << rules.popRules.size() << "\n";
    }
  } // namespace wpds

} // namespace wali


