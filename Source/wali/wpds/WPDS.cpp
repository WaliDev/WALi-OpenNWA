/*!
 * @author Nick Kidd
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
#include "wali/wpds/LinkedTrans.hpp"
#include "wali/wpds/Wrapper.hpp"
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
            wrapper(0)
                ,worklist( new DefaultWorklist<wfa::Trans>() )
                ,currentOutputWFA(0)
        {
        }

        WPDS::WPDS( Wrapper * w ) :
            wrapper(w)
                ,worklist( new DefaultWorklist<wfa::Trans>() )
                ,currentOutputWFA(0)
        {
        }

        WPDS::WPDS( Worklist<wfa::Trans> * wl ) :
            wrapper(0), worklist(wl) ,currentOutputWFA(0)
        {
            if( 0 == worklist )
                worklist = new DefaultWorklist<wfa::Trans>();
            else
                worklist->clear();
        }

        WPDS::WPDS( Wrapper* w, Worklist<wfa::Trans> * wl) :
            wrapper(w) , worklist(wl) ,currentOutputWFA(0)
        {
            if( 0 == worklist )
                worklist = new DefaultWorklist<wfa::Trans>();
            else
                worklist->clear();
        }

        WPDS::~WPDS()
        {
            //std::cerr << "~WPDS()\n";
            assert( worklist );
            clear();
            delete worklist;
        }

        void WPDS::clear()
        {
            /* Delete allocated Config objects */
            chash_t::iterator cit = config_map().begin();
            chash_t::iterator citEND = config_map().end();
            for( ; cit != citEND ; cit++ )
            {
                Config* c = cit->second;
                delete c;
                cit->second = 0;
            }
            /* clear everything */
            config_map().clear();
            rule_zeroes.clear();
            r2hash.clear();
            worklist->clear();
        }

        bool WPDS::add_rule(
                wali_key_t from_state,
                wali_key_t from_stack,
                wali_key_t to_state,
                sem_elem_t se )
        {
            return add_rule(from_state,from_stack,to_state,WALI_EPSILON,WALI_EPSILON,se );
        }

        bool WPDS::add_rule(
                wali_key_t from_state,
                wali_key_t from_stack,
                wali_key_t to_state,
                wali_key_t to_stack1,
                sem_elem_t se )
        {
            return add_rule(from_state,from_stack,to_state,to_stack1,WALI_EPSILON,se );
        }

        bool WPDS::add_rule(
                wali_key_t from_state,
                wali_key_t from_stack,
                wali_key_t to_state,
                wali_key_t to_stack1,
                wali_key_t to_stack2,
                sem_elem_t se )
        {
            rule_t r;
            bool rb = add_rule(from_state,from_stack,to_state,to_stack1,to_stack2,se,r );
            return rb;
        }

        WFA WPDS::prestar( WFA & input )
        {
            WFA output;
            prestar(input,output);
            return output;
        }

        void WPDS::prestar( WFA & input, WFA & fa )
        {
            prestarSetupFixpoint(input,fa);
            prestarComputeFixpoint( fa );
            currentOutputWFA = 0;
        }

        void WPDS::setupOutput( ::wali::wfa::WFA& input, ::wali::wfa::WFA& fa )
        {
            currentOutputWFA = &fa;
            // cannot clear if input == output
            if( &input == currentOutputWFA ) {
                WFA tmp(input);
                fa.clear();
                fa.set_initial_state( tmp.initial_state() );
                fa.F = tmp.F;
                tmp.for_each(*this);
            }
            else {
                fa.clear();
                fa.set_initial_state( input.initial_state() );
                fa.F = input.F;
                input.for_each(*this);
            }
        }

        void WPDS::prestarSetupFixpoint( WFA& input, WFA& fa )
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
                for( std::list< rule_t >::iterator rit = cloc->bwrules.begin();
                        rit != cloc->bwrules.end();
                        rit++ )
                {
                    rule_t r = *rit;

                    { // BEGIN DEBUGGING
                        //r->print( std::cerr << "Adding Rule0: " ) << std::endl;
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

            LinkedTrans * t;

            while( get_from_worklist( t ) )
            {

                //t->print( std::cerr << "$$$ Popped t ==> " ) << std::endl;

                // Get config
                Config * config = t->config;

                { // BEGIN DEBUGGING
                    assert( config );
                } // END DEBUGGING

                sem_elem_t dnew = t->delta;
                t->delta = dnew->zero();

                // For each backward rule of config
                Config::iterator bwit = config->bwrules.begin();
                for( ; bwit != config->bwrules.end() ; bwit++ )
                {
                    rule_t & r = *bwit;

                    //std::cerr << "\tCalling prestar_handle_trans  :  ";
                    //r->print( std::cout << "\tr == " ) << std::endl;

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

                        Trans tp;
                        if( fa.find(r->to_state(),r->to_stack1(),t->from(),tp) )
                        {
                            // f(r) * t'
                            sem_elem_t wrtp = r->weight()->extend( tp.se );

                            // f(r) * t' * delta
                            sem_elem_t wnew = wrtp->extend( dnew );

                            // update
                            update( r->from()->state()
                                    , r->from()->stack()
                                    , t->to()
                                    , wnew
                                    , r->from()
                                  );

                        }
                    }
                }
            }
        }

        void WPDS::prestar_handle_trans(
                LinkedTrans * t ,
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
                    //r->print( std::cerr << "Handling Rule 2: " ) << std::endl;
                    //std::cerr << "\t(" << key2str(kp.first) << ", " << key2str(kp.second) << ",*)\n";
                } // END DEBUGGING
                WFA::kp_map_t::iterator kpit = fa.kpmap.find( kp );
                WFA::kp_map_t::iterator kpitEND = fa.kpmap.end();
                TransSet::iterator tsit;

                if( kpit != kpitEND )
                {
                    TransSet& transSet = kpit->second;
                    for( tsit = transSet.begin(); tsit != transSet.end(); tsit++ )
                    {
                        Trans * tprime = *tsit;
                        { // BEGIN DEBUGGING
                            //std::cerr << "\tMatched: (" << key2str(tprime->from()) << ", ";
                            //std::cerr << key2str(tprime->stack()) << ", ";
                            //std::cerr << key2str(tprime->to()) << ")\n";
                        } // END DEBUGGING
                        sem_elem_t wtp = wrule_trans->extend( tprime->se );
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

        WFA WPDS::poststar( WFA & input )
        {
            WFA output;
            poststar(input,output);
            return output;
        }

        void WPDS::poststar( WFA & input, WFA & fa )
        {
            poststarSetupFixpoint(input,fa);
            poststarComputeFixpoint(fa);
            currentOutputWFA = 0;
        }

        void WPDS::poststarSetupFixpoint( WFA& input, WFA& fa )
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
                    wali_key_t gstate = gen_state( r->to_state(),r->to_stack1() );
                    fa.addState( gstate,r->weight()->zero() );
                }

            }
        }

        void WPDS::poststarComputeFixpoint( WFA& fa )
        {
            LinkedTrans * t;

            while( get_from_worklist( t ) ) {

                //t->print( std::cerr << "$$$ Popped t ==> " ) << std::endl;

                // Get config
                Config * config = t->config;
                
                // TODO : make debug stmt
                assert( config );

                sem_elem_t dnew = t->delta;
                t->delta = dnew->zero();

                // For each forward rule of config
                // Apply rule to create new transition
                if( WALI_EPSILON != t->stack() )
                {
                    Config::iterator fwit = config->fwrules.begin();
                    for( ; fwit != config->fwrules.end() ; fwit++ ) {
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
                        Trans * tprime = *it;
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
        }

        void WPDS::poststar_handle_trans(
                LinkedTrans * t ,
                WFA & fa   ,
                rule_t & r,
                sem_elem_t delta
                )
        {
            wali_key_t rtstate = r->to_state();
            wali_key_t rtstack = r->to_stack1();

            sem_elem_t wrule_trans = delta->extend( r->se );

            if( r->to_stack2() == WALI_EPSILON ) {
                update( rtstate, rtstack, t->to(), wrule_trans, r->to() );
            }
            else {

                // Is a rule 2 so we must generate a state
                // and create 2 new transitions
                // TODO: implement gen_state
                wali_key_t gstate = gen_state( rtstate,rtstack );

                Trans * tprime = update_prime( gstate, r->to_stack2(), t->to(), wrule_trans );

                State * state = fa.getState( gstate );
                sem_elem_t quasi = state->quasi->combine( wrule_trans );
                state->quasi = quasi;

                update( rtstate, rtstack, gstate, quasi->quasi_one(), r->to() );

                // Trans with generated from states do not go on the worklist
                // and there is no Config matching them so pass 0 (NULL) as the
                // Config * for update_prime
                if( tprime->modified() )
                {
                    // BEGIN DEBUGGING
                    {
                        std::cerr << "[WPDS::poststar]";
                        std::cerr << "t' modified. Searching for eps trans\n";
                    }
                    // END DEBUGGING

                    WFA::eps_map_t::iterator epsit = fa.eps_map.find( tprime->to() );
                    if( epsit != fa.eps_map.end() )
                    {
                        // tprime stack key
                        wali_key_t tpstk = tprime->stack();
                        // tprime to key
                        wali_key_t tpto = tprime->to();
                        // get epsilon list ref
                        TransSet& transSet = epsit->second;
                        // iterate
                        TransSet::iterator tsit = transSet.begin();
                        for( ; tsit != transSet.end() ; tsit++ )
                        {
                            Trans * teps = *tsit;
                            Config * config = make_config( teps->from(),tpstk );
                            sem_elem_t epsW = tprime->delta->extend( teps->se );
                            update( teps->from(),tpstk,tpto,
                                    epsW, config );
                        }
                    }
                }
            }
        }

        /*!
         * Generate's a key representing the entry point to a procedure
         */
        wali_key_t WPDS::gen_state( wali_key_t state, wali_key_t stack )
        {
            return getKey( state,stack );
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
            o << "</" << XMLTag << ">";
            return o;
        }

        void WPDS::for_each( ConstRuleFunctor &func ) const
        {
            const_iterator it = config_map().begin();
            for( ; it != config_map().end() ; it++ ) {
                const Config *c = config_map().value( it );
                std::list< rule_t >::const_iterator rit = c->forward().begin();
                for( ; rit != c->forward().end() ; rit++ )
                    func( *rit );
            }
        }

        void WPDS::for_each( RuleFunctor &func )
        {
            iterator it = config_map().begin();
            for( ; it != config_map().end() ; it++ ) {
                Config *c = config_map().value( it );
                std::list< rule_t >::iterator rit = c->fwrules.begin();
                for( ; rit != c->fwrules.end() ; rit++ )
                    func( *rit );
            }
        }


        /////////////////////////////////////////////////////////////////
        // Protected Methods
        /////////////////////////////////////////////////////////////////

        // Actually adds the rule. Allows for 2 return values, namely
        // the bool and a rule_t which contains a pointer to the real Rule.
        // This allows for subclasses to perform postprocessing.
        bool WPDS::add_rule(
                wali_key_t from_state,
                wali_key_t from_stack,
                wali_key_t to_state,
                wali_key_t to_stack1,
                wali_key_t to_stack2,
                sem_elem_t se,
                rule_t& r
                )
        {
            // Every rule must have these 3 pieces defined
            assert( from_state != WALI_EPSILON );
            assert( from_stack != WALI_EPSILON );
            assert( to_state   != WALI_EPSILON );
            Config *from = make_config(from_state,from_stack);
            Config *to = make_config(to_state,to_stack1);

            // make_rule will create links b/w Configs and the Rule
            bool rb = make_rule(from,to,to_stack2,se,r);

            // if rb is false then the rule is new
            if( !rb ) {
                if( to_stack1 == WALI_EPSILON )
                {
                    // TODO : make debug check
                    assert( to_stack2 == WALI_EPSILON );
                    rule_zeroes.insert( to );
                }
                if( to_stack2 != WALI_EPSILON ) {
                    r2hash_t::iterator r2it = r2hash.find( to_stack2 );
                    if( r2it == r2hash.end() ) {
                        std::list< rule_t > ls;
                        r2it = r2hash.insert( to_stack2,ls ).first;
                    }
                    r2it->second.push_back( r );
                }
            }
            return rb;
        }

        //
        // link input WFA transitions to Configs
        //
        //void WPDS::copy_and_link( WFA & in, WFA & dest )
        //{
        //   in.for_each( *this );
        //}

        Config * WPDS::make_config( wali_key_t state, wali_key_t stack )
        {
            Config *cf = find_config( state,stack );
            if( 0 == cf ) {
                cf = new Config(state,stack);
                config_map().insert( KeyPair(state,stack),cf );
            }
            return cf;
        }

        /*!
         * Creates a rule that links two configurations.
         * If rule exists then combines the weight
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
                wali_key_t stk2,
                sem_elem_t se,
                rule_t& r )
        {
            bool rb = false;
            std::list< rule_t >::iterator it = f->forward().begin();
            std::list< rule_t >::iterator itEND = f->forward().end();

            for( ; it != itEND; it++ )
            {
                rule_t &tmp = *it;
                if( tmp->f == f && tmp->t == t && tmp->to_stack2() == stk2 )
                {
                    rb = true;
                    if( wrapper ) {
                        // FIXME: Should we combine user weights at bottom
                        // of stack?
                        sem_elem_t combinedWeight = se->combine(wrapper->unwrap(tmp->se));
                        tmp->se = combinedWeight;
                        tmp->weight( wrapper->wrap(*tmp) );
                        //rule_t wrapTmp =  new Rule(f,t,stk2,se);
                        //tmp->se = tmp->se->combine(wrapper->wrap(*wrapTmp));
                    }
                    else {
                        tmp->se = tmp->se->combine(se);
                    }
                    r = tmp;
                    break;
                }
            }
            if( !rb ) {
                r =  new Rule(f,t,stk2,se);
                if( wrapper ) {
                    r->weight( wrapper->wrap(*r) );
                }
                f->forward().push_back(r);
                t->backward().push_back(r);
            }
            return rb;
        }

        Config * WPDS::find_config( wali_key_t state, wali_key_t stack )
        {
            Config *cf = 0;
            KeyPair kp(state,stack);
            iterator it = config_map().find(kp);
            if( it != config_map().end() )
                cf = it->second;
            return cf;
        }

        /////////////////////////////////////////////////////////////////
        // Protected WPDS methods
        /////////////////////////////////////////////////////////////////

        bool WPDS::get_from_worklist( LinkedTrans * & t )
        {
            if( !worklist->empty() ) {
                //t = (LinkedTrans *)worklist->get();
                //TODO: make this a debugging statement
                LinkedTrans *lt = dynamic_cast<LinkedTrans *>(worklist->get());
                assert(lt);
                t = lt;
                return true;
            }
            else {
                t = 0;
                return false;
            }
        }

        /*!
         * @brief helper function to create and link a transition
         *
         */
        void WPDS::update(
                wali_key_t from,
                wali_key_t stack,
                wali_key_t to,
                sem_elem_t se,
                Config * cfg )
        {
            LinkedTrans * lt = new LinkedTrans(from,stack,to,se,cfg);
            Trans *t = currentOutputWFA->insert(lt);
            if( t->modified() ) {
                worklist->put( t );
            }
        }

        /*!
         * update_prime does not need to take a Config b/c no Config
         * will match a transition taht is created here. The from state
         * is not \in WFA.P. Therefore we do not need to add it to the
         * worklist.
         *
         * @return generated transition
         */
        LinkedTrans * WPDS::update_prime(
                wali_key_t from,
                wali_key_t stack,
                wali_key_t to,
                sem_elem_t se )
        {
            LinkedTrans * t = new LinkedTrans(from,stack,to,se,0);
            Trans * tmp = currentOutputWFA->insert(t);
            // TODO: Make this a debugging stmt
            t = dynamic_cast<LinkedTrans*>(tmp);
            return t;
        }

        /////////////////////////////////////////////////////////////////
        // Implement TransFunctor.
        // This is used to copy Transitions from the input automaton
        // to the output. We create a LinkedTrans for fast (hopefully) indexing
        // of rules.
        /////////////////////////////////////////////////////////////////

        void WPDS::operator()( Trans * orig )
        {
            Config *c = make_config( orig->from(),orig->stack() );
            LinkedTrans *t;
            if( wrapper ) {
                t = new LinkedTrans(orig->from(),orig->stack(),orig->to(),
                        wrapper->wrap(*orig),c);
            }
            else {
                t = new LinkedTrans(orig->from(),orig->stack(),orig->to(),
                        orig->weight(),c);
            }

            // fa.addTrans takes ownership of passed in pointer
            currentOutputWFA->addTrans( t );

            // add t to the worklist for saturation
            worklist->put( t );
        }

    } // namespace wpds

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/
