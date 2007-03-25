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
                    TransPairCollapse( ) { }
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

            EWPDS::EWPDS( Wrapper * wrapper, Worklist<wfa::Trans> * wl) : WPDS(wrapper, wl)
            { }


            EWPDS::~EWPDS()
            {
                *waliErr << "~EWPDS()" << std::endl;
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

                if( to_stack1 == WALI_EPSILON )
                {
                    assert( to_stack2 == WALI_EPSILON );
                    rule_zeroes.insert( to );
                }
                rule_t r;

                // Rule::gen_rule will create links b/w Configs and the Rule
                bool rb = make_rule(from,to,to_stack2,se,mf,r);

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

            bool EWPDS::make_rule(
                    Config * f,
                    Config * t,
                    Key stk2,
                    sem_elem_t se,
                    merge_fn_t mf,
                    rule_t & r )
            {
                bool rb = false;
                Config::iterator it = f->begin();
                for( ; it != f->end(); it++ ) {
                    rule_t &tmp = *it;
                    if( tmp->from() == f && tmp->to() == t && tmp->to_stack2() == stk2 ) {
                        rb = true;
                        if( wrapper ) {
                            // FIXME: Should we combine user weights at bottom
                            // of stack?
                            sem_elem_t combinedWeight = se->combine(wrapper->unwrap(tmp->weight()));
                            tmp->se = combinedWeight;
                            tmp->weight( wrapper->wrap(*tmp) );
                            //rule_t wrapTmp =  new ERule(f,t,stk2,se,mf);
                            //tmp->se = tmp->se->combine(wrapper->wrap(*wrapTmp));
                        }
                        else {
                            tmp->weight(tmp->weight()->combine(se));
                        }
                        r = tmp;
                        break;
                    }
                }
                if( !rb ) {
                    ERule *er = new ERule(f,t,stk2,se,mf);
                    r = er;
                    if( wrapper ) {
                        sem_elem_t wrapped = wrapper->wrap(*r);
                        r->weight( wrapped );
                        sem_elem_t npair;
                        // NAK: Duplicated from ERule.cpp constructor
                        if( stk2 == WALI_EPSILON ) {
                            npair = new SemElemPair(wrapped, wrapped->one());
                        }
                        else {
                            npair = new SemElemPair(wrapped->one(), wrapped);
                        }
                        er->extended_se = npair;
                    }
                    f->insert(r);
                    t->rinsert(r);
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

            WFA EWPDS::prestar( WFA & input )
            {
                WFA fa;
                currentOutputWFA = &fa;
                input.for_each(*this);

                //
                // do rules 0
                // rule_zeroes contains Configs of (p,WALI_EPSILON)
                //
                for( std::set< Config * >::iterator it = WPDS::rule_zeroes.begin();
                        it != WPDS::rule_zeroes.end();
                        it++ )
                {
                    Config *cloc = *it;

                    // For each rule that connects a Config * to the one
                    // from the outer loop
                    Config::reverse_iterator rit = cloc->rbegin();
                    for( ; rit != cloc->rend(); rit++ )
                    {
                        rule_t r = *rit;

                        // add transition for rule
                        update( r->from_state()
                                , r->from_stack()
                                , r->to_state()
                                , r->weight()
                                , r->from()
                              );
                    }
                }

                LinkedTrans * t;

                while( WPDS::get_from_worklist( t ) )
                {

                    t->print( *waliErr << "$$$ Popped t ==> " ) << std::endl;

                    // Get config
                    Config * config = t->config;

                    // TODO : make debug stmt
                    assert( config );

                    sem_elem_t dnew = t->getDelta();
                    t->setDelta( dnew->zero() );

                    // For each backward rule of config
                    Config::reverse_iterator bwit = config->rbegin();
                    for( ; bwit != config->rend() ; bwit++ )
                    {
                        rule_t & r = *bwit;

                        *waliErr << "\tCalling prestar_handle_trans  :  ";
                        r->print( std::cout << "\tr == " ) << std::endl;

                        prestar_handle_trans( t,fa,r,dnew );

                    }

                    // check matching rule 2s 
                    r2hash_t::iterator r2it = WPDS::r2hash.find( t->stack() );

                    // does a rule 2 exist with matching second symbol on rhs
                    if( r2it != WPDS::r2hash.end() )
                    {
                        // get reference
                        std::list< rule_t > & ls = r2it->second;
                        std::list< rule_t >::iterator lsit;

                        // loop through list
                        for( lsit = ls.begin() ; lsit != ls.end() ; lsit++ )
                        {
                            rule_t & r = *lsit;

                            //Trans * tp = fa.lookup( r->to_state(),r->to_stack1(),t->from() );
                            Trans tp;
                            if( fa.find(r->to_state(),r->to_stack1(),t->from(),tp) )
                            {

                                if(!is_pds_state(t->from())) {
                                    // f(r) * t'
                                    sem_elem_t wrtp = r->weight()->extend( tp.weight() );

                                    // f(r) * t' * delta
                                    sem_elem_t wnew = wrtp->extend( dnew );

                                    // update
                                    update( r->from()->state(), r->from()->stack(), t->to(),
                                            wnew,r->from() );
                                } else { // apply merge function
                                    erule_t er = (ERule *)(r.get_ptr());
                                    sem_elem_t w1 = er->merge_fn()->apply_f( tp.weight()->one(), tp.weight());
                                    sem_elem_t wnew = w1->extend(dnew);
                                    // update
                                    update( r->from()->state(), r->from()->stack(), t->to(),
                                            wnew,r->from() );
                                }
                            }
                        }
                    }
                }

                currentOutputWFA = 0;
                return fa;
            }

            void EWPDS::prestar_handle_trans(
                    LinkedTrans * t ,
                    WFA & fa   ,
                    rule_t & r,
                    sem_elem_t delta
                    )
            {

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

                    for( ; kpit != kpitEND ; kpit++ )
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
                setupOutput(input,fa);

                // Generate midstates for each rule type two
                r2hash_t::iterator r2it = WPDS::r2hash.begin();
                for( ; r2it != r2hash.end() ; r2it++ )
                {
                    std::list< rule_t > & ls = r2it->second;
                    std::list< rule_t >::iterator rlsit = ls.begin();
                    for( ; rlsit != ls.end() ; rlsit++ )
                    {
                        erule_t r = (ERule *)((*rlsit).get_ptr());
                        Key gstate = WPDS::gen_state( r->to_state(),r->to_stack1() );
                        fa.addState( gstate,r->extended_weight()->zero() );
                    }
                }

                LinkedTrans * t;
                while( get_from_worklist( t ) ) {

                    t->print( *waliErr << "$$$ Popped t ==> " ) << std::endl;

                    // Get config
                    Config * config = t->config;

                    // TODO : make debug stmt
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
                            r->print( *waliErr << "\tMatched: " ) << std::endl;
                            poststar_handle_trans( t,fa,r,dnew );
                        }
                    }
                    else {
                        // (p,eps,q) + (q,y,q') => (p,y,q')
                        State * state = fa.getState( t->to() );
                        State::iterator it = state->begin();
                        for(  ; it != state->end() ; it++ )
                        {
                            Trans * tprime = *it;
                            KeySource *ks = getKeySource(t->to());
                            KeyPairSource *kps;
                            sem_elem_t wght;
                            tprime->print(*waliErr) << "\n";
                            dnew->print(*waliErr) << "\n";
                            if(0 != (kps = dynamic_cast<KeyPairSource *>(ks))) { // apply merge function
                                // Find the rule first
                                rule_t r = lookup_rule(kps->get_key_pair().first, kps->get_key_pair().second, tprime->stack());
                                assert(r.get_ptr() != NULL);
                                erule_t er = (ERule *)(r.get_ptr());
                                sem_elem_t w = er->merge_fn()->apply_f(SEM_PAIR_GET_FIRST(tprime->weight()), SEM_PAIR_GET_FIRST(dnew));
                                wght = new SemElemPair(w, w->one());
                            } else {
                                wght = tprime->weight()->extend( dnew );
                            }
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
                // convert back from <se,se> to se
                TransPairCollapse tpc;
                fa.for_each( tpc );
                currentOutputWFA = 0;
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
                    // TODO: implement gen_state
                    Key gstate = gen_state( rtstate,rtstack );

                    Trans * tprime = update_prime( gstate, r->to_stack2(), t->to(), wrule_trans  );

                    State * state = fa.getState( gstate );
                    sem_elem_t quasi = state->quasi->combine( wrule_trans );
                    state->quasi = quasi;

                    update( rtstate, rtstack, gstate, quasi->quasi_one(), r->to() );

                    // Trans with generated from states do not go on the worklist
                    // and there is no Config matching them so pass 0 (NULL) as the
                    // Config * for update_prime
                    if( tprime->modified() )
                    {
                        *waliErr <<
                            "[EWPDS::poststar] tprime modified...searching for eps trans\n";

                        WFA::eps_map_t::iterator epsit = fa.eps_map.find( tprime->to() );
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
                LinkedTrans *t = new LinkedTrans( orig->from()
                        ,orig->stack()
                        ,orig->to()
                        , new SemElemPair(se,se->one())
                        ,c);

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
