/*!
 * @author Akash Lal
 */

/*
 * NAK: Witnesses (and wrapping) do not work w/ EWPDS.
 *      The problem lies w/ MergeFn::apply_f(a,b)
 *      wpds::Wrapper deals w/ weights and not MergeFns
 * TODO:
 *      Should be able to create a mergewrapper.
 */
// ::wali
#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include "wali/Worklist.hpp"
#include "wali/KeySource.hpp"
#include "wali/KeyPairSource.hpp"

// ::wali::wfa
#include "wali/wfa/ITrans.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/TransSet.hpp"

// ::wali::wpds
#include "wali/wpds/Config.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Wrapper.hpp"
#include "wali/wpds/GenKeySource.hpp"

// ::wali::wpds::ewpds
#include "wali/wpds/ewpds/ERule.hpp"
#include "wali/wpds/ewpds/EWPDS.hpp"
#include "wali/wpds/ewpds/ETrans.hpp"

#include <iostream>
#include <cassert>

namespace wali {
  namespace wpds {  
    namespace ewpds {
      class TransPairCollapse : public wali::wfa::TransFunctor
      {
        public:
          TransPairCollapse() { }
          virtual ~TransPairCollapse() { }
          virtual void operator()( ::wali::wfa::ITrans * orig )
          {
            orig->setWeight(orig->weight());
          }

      };
    }
  }
}


namespace wali
{
    using wfa::TransSet;
    using wfa::WFA;
    using wfa::State;

    namespace wpds 
    {
        using ::wali::wpds::Config;

        namespace ewpds
        {

            EWPDS::EWPDS( Wrapper * wrapper ) : WPDS(wrapper), usePairsDuringCopy(false)
            { }


            EWPDS::~EWPDS()
            {
                //*waliErr << "~EWPDS()" << std::endl;
                merge_rule_hash.clear();
            }

            bool EWPDS::add_rule(
                    Key from_state,
                    Key from_stack,
                    Key to_state,
                    sem_elem_t se )
            {
                return add_rule(from_state,from_stack,to_state,WALI_EPSILON,WALI_EPSILON,se,(merge_fn_t)(NULL) );
            }

            bool EWPDS::add_rule(
                    Key from_state,
                    Key from_stack,
                    Key to_state,
                    Key to_stack1,
                    sem_elem_t se )
            {
                return add_rule(from_state,from_stack,to_state,to_stack1,WALI_EPSILON,se,(merge_fn_t)(NULL) );
            }

            bool EWPDS::add_rule(
                    Key from_state,
                    Key from_stack,
                    Key to_state,
                    Key to_stack1,
                    Key to_stack2,
                    sem_elem_t se )
            {
                return add_rule(from_state,from_stack,to_state,to_stack1,to_stack2,se,(merge_fn_t)(NULL) );
            }

            bool EWPDS::add_rule(
                    Key from_state,
                    Key from_stack,
                    Key to_state,
                    Key to_stack1,
                    Key to_stack2,
                    sem_elem_t se,
                    merge_fn_t mf )
            {
                // Every rule must have these 3 pieces defined
                assert( from_state != WALI_EPSILON );
                assert( from_stack != WALI_EPSILON );
                assert( to_state   != WALI_EPSILON );
                wpds::Config *from = make_config(from_state,from_stack);
                wpds::Config *to = make_config(to_state,to_stack1);

                pds_states.insert(from_state);
                pds_states.insert(to_state);

                rule_t r = new ERule(from,to,to_stack2,se,mf);
                // Rule::gen_rule will create links b/w Configs and the Rule
                bool rb = make_rule(from,to,to_stack2,se,r);

                if( !rb && to_stack1 == WALI_EPSILON )
                {
                    assert( to_stack2 == WALI_EPSILON );
                    rule_zeroes.insert( to );
                }

                // if rb is false then the rule is new
                if( to_stack2 != WALI_EPSILON && !rb ) {
                    r2hash_t::iterator r2it = r2hash.find( to_stack2 );
                    if( r2it == r2hash.end() ) {
                        std::list< rule_t > ls;
                        r2it = r2hash.insert( to_stack2,ls ).first;
                    }
                    r2it->second.push_back( r );

                    merge_rule_hash_t::iterator rhash_it = merge_rule_hash.find(KeyTriple(to_state,to_stack1,to_stack2));
                    if(rhash_it == merge_rule_hash.end()) {
                        merge_rule_hash.insert(KeyTriple(to_state,to_stack1,to_stack2), r);
                    } else {
                        // FIXME: raise exception
                        *waliErr << "EWPDS: Cannot give two RULE2s with same RHS\n";
                    }
                }
                // Set up theZero weight
                if (!theZero.is_valid() && r->weight().is_valid())
                  theZero = r->weight()->zero();
                
                return rb;
            }

            rule_t EWPDS::lookup_rule(Key to_state, Key to_stack1, Key to_stack2) const {
                merge_rule_hash_t::const_iterator rhash_it = merge_rule_hash.find(KeyTriple(to_state,to_stack1,to_stack2));
                if(rhash_it == merge_rule_hash.end()) {
                    return NULL;
                } 
                return rhash_it->second;
            }

            void EWPDS::prestar_handle_call(
                    wfa::ITrans* t1,
                    wfa::ITrans* t2,
                    rule_t &r,
                    sem_elem_t delta
                    )
            {
                //std::cout << "Prestar_handle_call:\n";
                //t1->print(std::cout << "t1: ") << "\n";
                //t2->print(std::cout << "t2: ") << "\n";
                //r->print(std::cout << "r: ") << "\n";

                if(!is_pds_state(t2->from())) {
                  // f(r) * t1
                  sem_elem_t wrtp = r->weight()->extend( t1->weight() );
                  
                  // f(r) * t2 * delta
                  sem_elem_t wnew = wrtp->extend( delta );
                  
                  // update
                  update( r->from()->state(), r->from()->stack(), t2->to(),
                          wnew,r->from() );
                } else { // apply merge function
                  erule_t er = (ERule *)(r.get_ptr());
                  sem_elem_t w1 = er->merge_fn()->apply_f( t1->weight()->one(), t1->weight());
                  sem_elem_t wnew = w1->extend(delta);
                  // update
                  update( r->from()->state(), r->from()->stack(), t2->to(),
                          wnew,r->from() );
                }
            }

            void EWPDS::prestar_handle_trans(
                    wfa::ITrans * t ,
                    WFA & fa   ,
                    rule_t & r,
                    sem_elem_t delta
                    )
            {
                //std::cout << "Prestar_handle_trans:\n";
                //t->print(std::cout << "t: ") << "\n";
                //r->print(std::cout << "r: ") << "\n";

                sem_elem_t wrule_trans;
                if(r->stack2() == WALI_EPSILON || !is_pds_state(t->to())) {
                    wrule_trans = r->weight()->extend( delta );
                } else { // apply merge function
                    erule_t er = (ERule *)(r.get_ptr());
                    wrule_trans = er->merge_fn()->apply_f(delta->one(), delta);
                }
                Key fstate = r->from()->state();
                Key fstack = r->from()->stack();

                if( r->is_rule2() ) {
                    KeyPair kp( t->to(),r->stack2() );
                    WFA::kp_map_t::iterator kpit = fa.kpmap.find( kp );
                    WFA::kp_map_t::iterator kpitEND = fa.kpmap.end();
                    TransSet::iterator tsit;

                    if(kpit != kpitEND)
                    {
                        TransSet & transSet = kpit->second;
                        for( tsit = transSet.begin(); tsit != transSet.end(); tsit++ )
                        {
                            wfa::ITrans* tprime = *tsit;
                            sem_elem_t wtp = wrule_trans->extend( tprime->weight() );
                            update( fstate, fstack, tprime->to(), wtp, r->from() );
                        }
                    }
                }
                else {
                    update( fstate, fstack, t->to(), wrule_trans, r->from() );
                }
            }

            void EWPDS::poststar( WFA & input, WFA& fa )
            {
                usePairsDuringCopy = true;

                poststarSetupFixpoint(input, fa);
                poststarComputeFixpoint(fa);

                // convert back from <se,se> to se
                TransPairCollapse tpc;
                fa.for_each( tpc );

                currentOutputWFA = 0;
                usePairsDuringCopy = false;
            }

            void EWPDS::poststar_handle_trans(
                    wfa::ITrans * t ,
                    WFA & fa   ,
                    rule_t & r,
                    sem_elem_t delta
                    )
            {
              Key rtstate = r->to_state();
              Key rtstack = r->to_stack1();
              sem_elem_t wrule_trans = delta->extend(r->weight());

              //sem_elem_t wrule_trans = delta->extend( er->extended_weight() );

              if( r->to_stack2() == WALI_EPSILON ) {
                //update( rtstate, rtstack, t->to(), wrule_trans, r->to() );
                update( r->to_state(), r->to_stack1(), t->to(), wrule_trans, r->to() );
              }
              else {

                // Is a rule 2 so we must generate a state
                // and create 2 new transitions
                Key gstate = gen_state( rtstate,rtstack );

                wfa::ITrans* tprime = update_prime( gstate, t, r, delta, wrule_trans);

                // Setup the weight for taking quasione
                // TODO: This does not use the diff operator because for pair we would
                // need it to be (delta,second) + (first,delta). Otherwise taking
                // an extend of the components doesn't work out
                //sem_elem_t called_weight = SEM_PAIR_COLLAPSE(tprime->weight());

                //called_weight->print(std::cout << "called_weight = ") << "\n";

                // Take quasione
                State * state = fa.getState( gstate );

                // This combine operation is redundant because called_weight is
                // computed on the transition weight, not its delta
                //sem_elem_t quasi = state->quasi->combine( called_weight );
                //state->quasi = quasi;
                state->quasi = state->quasi->combine( wrule_trans );

                //sem_elem_t quasi_extended = new SemElemPair(quasi->quasi_one(), quasi->one());
                //update( rtstate, rtstack, gstate, quasi_extended, r->to() );
                update( rtstate, rtstack, gstate, state->quasi->quasi_one(), r->to() );

                // Trans with generated from states do not go on the worklist
                // and there is no Config matching them so pass 0 (NULL) as the
                // Config * for update_prime
                if( tprime->modified() )
                {
                  //std::cout <<
                  // "[EWPDS::poststar] tprime modified...searching for eps trans\n";

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

                      Config* config = make_config( teps->from(),tpstk );
                      sem_elem_t epsW = tprime->poststar_eps_closure( teps->weight() );

                      update( teps->from(),tpstk,tpto, epsW, config );
                    }
                  }
                }
              }
            }

            std::ostream & EWPDS::print( std::ostream & o ) const
            {
              return WPDS::print(o);
            }

            std::ostream & EWPDS::marshall( std::ostream & o ) const
            {
                RuleMarshaller rm(o);
                o << "<EWPDS>\n";
                WPDS::for_each( rm );
                o << "</EWPDS>" << std::endl;
                return o;
            }

            /////////////////////////////////////////////////////////////////
            // Protected Methods
            /////////////////////////////////////////////////////////////////

            void EWPDS::operator()( wfa::ITrans * orig )
            {
                if( is_pds_state(orig->to())) {
                  *waliErr << "WALi Error: cannot have incoming transition to a PDS state\n";
                  assert(0);
                }

                Config *c = make_config( orig->from(),orig->stack() );
                sem_elem_t se = (wrapper == 0) ? orig->weight() : wrapper->wrap(*orig);

                wfa::ITrans *t = orig->copy();

                t->setConfig(c);
                t->setWeight(se);

                // fa.addTrans takes ownership of passed in pointer
                currentOutputWFA->addTrans( t );

                // add t to the worklist for saturation
                worklist->put( t );
            }

            wfa::ITrans* EWPDS::update_prime(
                Key from, //<! Guaranteed to be a generated state
                wfa::ITrans* call, //<! The call transition
                rule_t r, //<! The push rule
                sem_elem_t delta, //<! Delta change on the call transition
                sem_elem_t wWithRule //<! delta \extends r->weight()
                )
            {
              //
              // !!NOTE!!
              // This code is copied in FWPDS::update_prime.
              // Changes here should be reflected there.
              //
              ERule* er = (ERule*)r.get_ptr();
              wfa::ITrans* tmp = 
                new ETrans(
                    from, r->to_stack2(), call->to(),
                    delta, wWithRule, er);
              wfa::ITrans* t = currentOutputWFA->insert(tmp);
              return t;
            }

        } // namespace ewpds

    } // namespace wpds

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
