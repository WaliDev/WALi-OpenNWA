#include "wali/wpds/DebugWPDS.hpp"
#include "wali/wpds/Config.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/Trans.hpp"

namespace wali
{

  using wfa::Trans;
  using wfa::TransSet;
  using wfa::WFA;
  using wfa::State;

  namespace wpds
  {
    DebugWPDS::DebugWPDS() : WPDS()
    {
    }

    DebugWPDS::DebugWPDS( ref_ptr<Wrapper> wrapper ) : WPDS( wrapper )
    {
    }

    DebugWPDS::~DebugWPDS()
    {
    }


    void DebugWPDS::prestarComputeFixpoint( wfa::WFA& fa )
    {

      wfa::ITrans* t;

      while( get_from_worklist( t ) ) 
      {
        t->print( *waliErr << "$$$ Popped t ==> " ) << std::endl;
        // TODO insert debugger code
        pre( t , fa );
      }
    }

    void DebugWPDS::poststarComputeFixpoint( wfa::WFA& fa )
    {

      wfa::ITrans* t;

      while( get_from_worklist( t ) ) 
      {
        t->print( *waliErr << "$$$ Popped t ==> " ) << std::endl;
        // TODO insert debugger code
        post( t , fa );
      }
    }

    void DebugWPDS::post( wfa::ITrans * t, wfa::WFA& fa )
    {

      // Get config
      Config * config = t->getConfig();

      assert( config );

      sem_elem_t dnew = t->getDelta();
      t->setDelta( dnew->zero() );

      // For each forward rule of config
      // Apply rule to create new transition
      if( WALI_EPSILON != t->stack() )
      {
        Config::iterator fwit = config->begin();
        for( ; fwit != config->end() ; fwit++ ) {
          rule_t & r = *fwit;

          { // BEGIN DEBUGGING
            r->print( std::cout << "\tMatched - " ) << std::endl;
          } // END DEBUGGING

          poststar_handle_trans( t,fa,r,dnew );
        }
      }
      else {
        // (p,eps,q) + (q,y,q') => (p,y,q')
        wfa::State * state = fa.getState( t->to() );
        wfa::State::iterator it = state->begin();
        for(  ; it != state->end() ; it++ )
        {
          wfa::ITrans* tprime = *it;

          { // BEGIN DEBUGGING
            tprime->print( std::cout << "\tMatched - " ) << std::endl;
          } // END DEBUGGING

          sem_elem_t wght = tprime->weight()->extend( dnew );
          Config * config = make_config( t->from(),tprime->stack() );
          update( t->from()
              , tprime->stack()
              , tprime->to()
              , wght
              , config
              );
        }
      }
    }

    void DebugWPDS::poststar_handle_trans(
        wfa::ITrans * t,
        wfa::WFA & fa,
        rule_t & r,
        sem_elem_t delta
        )
    {
      Key rtstate = r->to_state();
      Key rtstack = r->to_stack1();
      *waliErr << "[DebugWPDS::poststar_handle_trans] Before extend." << std::endl;
      sem_elem_t wrule_trans = delta->extend( r->weight() );
      *waliErr << "[DebugWPDS::poststar_handle_trans] After extend." << std::endl;

      if( r->to_stack2() == WALI_EPSILON ) {
        update( rtstate, rtstack, t->to(), wrule_trans, r->to() );
      }
      else {

        // Is a rule 2 so we must generate a state
        // and create 2 new transitions
        Key gstate = gen_state( rtstate,rtstack );

        wfa::ITrans* tprime = 
          update_prime( gstate, t, r, delta, wrule_trans );

        State * state = fa.getState( gstate );

        sem_elem_t quasi = state->quasi->combine( wrule_trans );
        state->quasi = quasi;

        update( rtstate, rtstack, gstate, quasi->quasi_one(), r->to() );

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

  }   // namespace wpds

}   // namespace wali
