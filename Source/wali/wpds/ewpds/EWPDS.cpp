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
#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include "wali/Worklist.hpp"
#include "wali/KeySource.hpp"
#include "wali/KeyPairSource.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/TransSet.hpp"
#include "wali/wpds/Config.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/LinkedTrans.hpp"
#include "wali/wpds/Wrapper.hpp"
#include "wali/wpds/GenKeySource.hpp"
#include "wali/wpds/ewpds/ERule.hpp"
#include "wali/wpds/ewpds/EWPDS.hpp"

#include <iostream>
#include <cassert>

// sem_elem_t -> sem_elem_t
#define SEM_PAIR_GET_FIRST(se) (((SemElemPair *)(se.get_ptr()))->get_first())
#define SEM_PAIR_GET_SECOND(se) (((SemElemPair *)(se.get_ptr()))->get_second())
#define SEM_PAIR_COLLAPSE(se) (SEM_PAIR_GET_FIRST(se)->extend(SEM_PAIR_GET_SECOND(se)))

namespace wali
{
    namespace wpds 
    {  
        namespace ewpds
        {
            class TransPairCollapse : public wali::wfa::TransFunctor
            {
                public:
              TransPairCollapse() { }
                    virtual ~TransPairCollapse() { }
                    virtual void operator()( ::wali::wfa::Trans * orig )
                    {
                        orig->setWeight(SEM_PAIR_COLLAPSE(orig->weight()));
                    }

            };

        }
    }
}


namespace wali
{
    using wfa::Trans;
    using wfa::TransSet;
    using wfa::WFA;
    using wfa::State;

    namespace wpds 
    {
        using ::wali::wpds::Config;

        namespace ewpds
        {

            typedef ref_ptr<ERule> erule_t;

          EWPDS::EWPDS( Wrapper * wrapper, Worklist<wfa::Trans> * wl) : WPDS(wrapper, wl), usePairsDuringCopy(false)
            { }


            EWPDS::~EWPDS()
            {
                //*waliErr << "~EWPDS()" << std::endl;
                pds_states.clear();
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
                return rb;
            }

            bool EWPDS::is_pds_state(Key k) const {
                if(pds_states.find(k) == pds_states.end())
                    return false;
                return true;
            }

            rule_t EWPDS::lookup_rule(Key to_state, Key to_stack1, Key to_stack2) const {
                merge_rule_hash_t::const_iterator rhash_it = merge_rule_hash.find(KeyTriple(to_state,to_stack1,to_stack2));
                if(rhash_it == merge_rule_hash.end()) {
                    return NULL;
                } 
                return rhash_it->second;
            }

            void EWPDS::prestar_handle_call(
                    Trans *t1,
                    Trans *t2,
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
                    LinkedTrans * t ,
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
                            Trans * tprime = *tsit;
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

            void EWPDS::poststar_handle_eps_trans(Trans *teps, 
                                                  Trans *tprime, 
                                                  sem_elem_t delta)
            {
                // TODO Remove these checks once merge functions
                //      get moved to the transitions and out of the
                //      (Rule:MergeFn) map.
                KeySource *ks = getKeySource(teps->to());
                GenKeySource* gks = dynamic_cast<GenKeySource*>(ks);
                KeyPairSource *kps;
                if (gks != 0) { // check gks first b/c that is what poststar generates now
                    kps = dynamic_cast<KeyPairSource*>(getKeySource(gks->getKey()));
                }
                else {
                    // this probably should not occur, unless the user
                    // created teps himself.
                    kps = dynamic_cast<KeyPairSource*>(ks);
                }
                sem_elem_t wght;
                
                // tprime->print(std::cout) << "\n";
                // delta->print(std::cout) << "\n";

                if (0 != kps) { // apply merge function
                  // Find the rule first
                  rule_t r = lookup_rule(kps->get_key_pair().first, kps->get_key_pair().second, tprime->stack());
                  assert(r.get_ptr() != NULL);
                  erule_t er = (ERule *)(r.get_ptr());
                  sem_elem_t w = er->merge_fn()->apply_f(SEM_PAIR_GET_FIRST(tprime->weight()), SEM_PAIR_GET_FIRST(delta));
                  wght = new SemElemPair(w, w->one());
                } else {
                  wght = tprime->weight()->extend( delta );
                }
                Config * config = make_config( teps->from(),tprime->stack() );
                update( teps->from()
                        , tprime->stack()
                        , tprime->to()
                        , wght
                        , config
                        );
            }

            void EWPDS::poststar_handle_trans(
                    LinkedTrans * t ,
                    WFA & fa   ,
                    rule_t & r,
                    sem_elem_t delta
                    )
            {
                erule_t er = (ERule *)(r.get_ptr());
                Key rtstate = r->to_state();
                Key rtstack = r->to_stack1();

                sem_elem_t wrule_trans = delta->extend( er->extended_weight() );

                if( r->to_stack2() == WALI_EPSILON ) {
                    update( rtstate, rtstack, t->to(), wrule_trans, r->to() );
                }
                else {

                    // Is a rule 2 so we must generate a state
                    // and create 2 new transitions
                    Key gstate = gen_state( rtstate,rtstack );

                    Trans * tprime = update_prime( gstate, r->to_stack2(), t->to(), wrule_trans  );

                    // Setup the weight for taking quasione
                    // TODO: This does not use the diff operator because for pair we would
                    // need it to be (delta,second) + (first,delta). Otherwise taking
                    // an extend of the components doesn't work out
                    sem_elem_t called_weight = SEM_PAIR_COLLAPSE(tprime->weight());

                    //called_weight->print(std::cout << "called_weight = ") << "\n";

                    // Take quasione
                    State * state = fa.getState( gstate );
                    // This combine operation is redundant because called_weight is
                    // computed on the transition weight, not its delta
                    sem_elem_t quasi = state->quasi->combine( called_weight );
                    state->quasi = quasi;

                    sem_elem_t quasi_extended = new SemElemPair(quasi->quasi_one(), quasi->one());
                    update( rtstate, rtstack, gstate, quasi_extended, r->to() );

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
                                Trans * teps = *tsit;
                                // apply merge function
                                sem_elem_t w = er->merge_fn()->apply_f(
                                        SEM_PAIR_GET_FIRST(tprime->getDelta()), SEM_PAIR_GET_FIRST(teps->weight()));
                                sem_elem_t epsW = new SemElemPair(w, w->one());
                                Config * config = make_config( teps->from(),tpstk );

                                update( teps->from(),tpstk,tpto,
                                        epsW, config );
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

#if 0
            void EWPDS::fpostar( wali::WFA& fa_in, wali::WFA& fa_out )
            {
            }
#endif

            /////////////////////////////////////////////////////////////////
            // Protected Methods
            /////////////////////////////////////////////////////////////////

            void EWPDS::operator()( wali::wfa::Trans * orig )
            {
                Config *c = make_config( orig->from(),orig->stack() );
                sem_elem_t se;
                if( wrapper ) {
                    se = wrapper->wrap(*orig);
                }
                else {
                    se = orig->weight();
                }
                LinkedTrans *t;
                if(usePairsDuringCopy) {
                  t = new LinkedTrans( orig->from()
                        ,orig->stack()
                        ,orig->to()
                        , new SemElemPair(se,se->one())
                        ,c);
                } else {
                  t = new LinkedTrans( orig->from()
                        ,orig->stack()
                        ,orig->to()
                        ,se
                        ,c);
                }

                // fa.addTrans takes ownership of passed in pointer
                currentOutputWFA->addTrans( t );

                // add t to the worklist for saturation
                worklist->put( t );
            }


        } // namespace ewpds

    } // namespace wpds

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
