/*!
 * @author Nick Kidd
 */

/*
 * WFA::eraseState is currently broken. It does not remove the
 * incoming Transes to the State. TODO: FIX
 */

#include "wali/Common.hpp"
#include "wali/DefaultWorklist.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/WeightMaker.hpp"
#include <iostream>

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

        WFA::WFA( query_t q ) : init_state( WALI_EPSILON ),query(q)
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
            Key isold = init_state;
            // TODO : Add debug check to verify key exists
            init_state = key;

            return isold;
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
        // Get some weight from the WFA, if it is non-empty 
        // If the WFA is empty, return NULL
        //
        sem_elem_t WFA::getSomeWeight() {
          sem_elem_t ret;
          kp_map_t::iterator it = kpmap.begin();
          while(it != kpmap.end()) {
            TransSet &tset = it->second;
            TransSet::iterator it2 = tset.begin();
            if(it2 != tset.end()) {
              return (*it2)->weight();
            }
            it ++;
          }
          return ret;
        }

        //!
        // @brief add trans (p,g,q,se) to WFA
        //
        void WFA::addTrans(
                Key p,
                Key g,
                Key q,
                sem_elem_t se )
        {
            addTrans( new Trans(p,g,q,se) );
        }

        //!
        // @brief add Trans *t to WFA
        // Takes care of adding states and calling insert. This
        // method (actually insert) assumes ownership of the memory
        // pointed to by the Trans* t.
        //
        void WFA::addTrans( Trans * t )
        {
            //t->print( *waliErr << "\tInserting Trans" ) << std::endl;
            insert( t );
        }

        //
        // Erase a trans
        // Must remove from kpmap, eps_map, and State('from')
        //
        void WFA::erase(
                Key from,
                Key stack,
                Key to )
        {
            // Remove from kpmap
            Trans* tKp = eraseTransFromKpMap(from,stack,to);

            if( tKp != NULL )
            {
                Trans* tEps = eraseTransFromEpsMap(tKp);

                { // BEGIN DEBUGGING
                    if( tEps != NULL )
                        assert( tKp == tEps );
                } // END DEBUGGING

                State* state = state_map.find(from)->second;
                state->eraseTrans(tKp);
                delete tKp;
            }
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
            bool rc = false;
            Trans *tret = 0;
            KeyPair kp(p,g);
            kp_map_t::iterator it = kpmap.find(kp);
            if( it != kpmap.end() )
            {
                TransSet& transSet = it->second;
                TransSet::iterator tsit= transSet.find(p,g,q);
                if( tsit != transSet.end() ) {
                    tret = *tsit;
                }

                /*
                trans_list_t &ls = it->second;
                trans_list_t::iterator lit = ls.begin();
                for( ; lit != ls.end() ; lit ++ )
                {
                    Trans *ttmp = *lit;
                    // already matched (p,g,*) so just
                    // check q
                    if( q == ttmp->to() )
                    {
                        tret = ttmp;
                        break;
                    }
                }
                */
            }
            if( 0 != tret ) {
                rc = true;
                t = *tret;
            }
            return rc;
        }

        void WFA::for_each( ConstTransFunctor & tf ) const
        {
            /*
            kp_map_t::const_iterator it = kpmap.begin();
            kp_map_t::const_iterator itEND = kpmap.end();
            for( ; it != itEND ; it++ )
            {
                const TransSet & transSet = it->second;
                transSet.each(tf);
            }
            */
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
            /*
            kp_map_t::iterator it = kpmap.begin();
            kp_map_t::iterator itEND = kpmap.end();
            for( ; it != itEND ; it++ )
            {
                TransSet & transSet = it->second;
                transSet.each(tf);
            }
            */
            state_map_t::iterator it = state_map.begin();
            state_map_t::iterator itEND = state_map.end();
            for( ; it != itEND ; it++ )
            {
                State* st = it->second;
                TransSet & transSet = st->getTransSet();
                transSet.each(tf);
            }
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
                    Trans *t = *tsit;

                    // for each trans in (TransSet) stkit->second
                    TransSet::iterator stklsit = stkit->second.begin();
                    TransSet::iterator stklsitEND = stkit->second.end();
                    for( ; stklsit != stklsitEND ; stklsit++ )
                    {
                        Trans *t2 = *stklsit;
                        Key fromkey = getKey( t->from(),t2->from() );
                        Key tokey = getKey( t->to(),t2->to() );
                        sem_elem_t W = wmaker.make_weight(t->weight(),t2->weight());
                        Trans* newTrans = new Trans(fromkey,t->stack(),tokey,W); 
                        //newTrans->print( std::cout << "\tAdding Trans: " ) << std::endl;
                        dest.addTrans( newTrans );
                    }
                }
            }
        }

        /*
        void WFA::do_fixpoint( Worklist& wl, FixpointLogic& logic )
        {
            setupFixpoint(wl,logic);
            while( !wl.empty() )
            {
                State* q = wl.get();
                sem_elem_t the_delta = q->delta();
                q->delta() = the_delta->zero();

                logic.process(q,the_delta,wl);
            }
        }
        */

        //
        // Calls path_summary with default Worklist
        //
        void WFA::path_summary()
        {
            DefaultWorklist<State> wl;
            path_summary(wl);
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
                        Trans* t = *tit; // (q',_,q)

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
        // Removes all transitions in the (init_state,F) chop
        // TODO: broken
        //
        void WFA::prune()
        {

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
                    Trans* t = *it;
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
                    o  << ",color=light_blue,style=filled";
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
        Trans * WFA::insert( Trans * tnew )
        {
            ////
            // WFA::find code duplicated to keep
            // a handle on the kp_map_t::iterator
            ////
            Trans * told = 0;
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
                    combineTrans( told,tnew );
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
                //throw NoSuchStateException( name );
                return NULL;
            }
            else {
                const State * state = stit->second;
                //return *state;
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
        // This is a virtual method.
        //
        void WFA::combineTrans( Trans * told, Trans * tnew )
        {
            /*
            { // BEGIN DEBUGGING
                *waliErr << "[WFA::combineTrans(old,new)] ";
                told->print( *waliErr ) << "   .+.   ";
                tnew->print( *waliErr ) << std::endl;
            } // END DEBUGGING
            */
            told->combine_weight( tnew->weight() );
        }

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
                    Trans* t = *stit;
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
        Trans* WFA::eraseTransFromKpMap(
                Key from,
                Key stack,
                Key to )
        {
            // ignore weight on Trans
            Trans terase(from,stack,to,0);
            return eraseTransFromKpMap(&terase);
        }

        Trans* WFA::eraseTransFromKpMap( Trans* terase )
        {
            Trans* tret = NULL;
            kp_map_t::iterator kpit = kpmap.find( terase->keypair() );

            // remove from kpmap
            if( kpit != kpmap.end() ) {
                tret = kpit->second.erase(terase);
            }
            return tret;
        }

        Trans* WFA::eraseTransFromEpsMap( Trans* terase )
        {
            Trans* tret = NULL;
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
        // to and from state.
        //
        bool WFA::eraseState(
                State* state
                )
        {
            State::iterator it = state->begin();
            State::iterator itEND = state->end();
            for( ; it != itEND ; it++ )
            {
                Trans* stateTrans = *it;

                //Key from = stateTrans->from();
                //Key stack = stateTrans->stack();
                //Key to = stateTrans->to();
                //Trans* tKp = eraseTransFromKpMap( from,stack,to );

                Trans* tKp = eraseTransFromKpMap(stateTrans);

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

                Trans* teps = eraseTransFromEpsMap(tKp);

                { // BEGIN DEBUGGING
                    if( teps != NULL ) {
                        assert( tKp == teps );
                    }
                } // END DEBUGGING

                delete tKp;
            }

            // Remove from state map
            // TODO: Is this necessary?
            //state_map.erase( state->name() );
            //if( state->name() == getInitialState() ) {
            //    setInitialState( WALI_EPSILON );
            //}

            Q.erase(state->name());
            //F.erase(state->name());

            // Since we are not deleting the State, we need
            // to clear its TransLists
            state->clearTransSet();

            // delete the memory
            //delete state;

            // TODO: does not erase incoming Trans!
            return true;
        }

    } // namespace wfa

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
