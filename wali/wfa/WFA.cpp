/*!
 * $Id: WFA.cpp,v 1.10 2005/06/24 01:14:42 kidd Exp $
 *
 * @author Nick Kidd
 * @version $Revision: 1.10 $
 */

#include <iostream>
#include "wali/KeyFactory.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/WeightMaker.hpp"

namespace wali
{
    namespace wfa
    {

        WFA::WFA() : init_state( WALI_EPSILON ) {}

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

                // This will populate all maps
                TransCopier copier(*this);
                rhs.for_each( copier );
            }

            return *this;
        }

        WFA::~WFA()
        {
            {
                // TODO : R
                std::cerr << "~WFA()\n";
            }
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
        }

        /*!
         * @brief set initial state
         */
        wali_key_t WFA::set_initial_state( wali_key_t key )
        {
            wali_key_t isold = init_state;
            // TODO : Add debug check to verify key exists
            init_state = key;

            return isold;
        }

        /*!
         * Add parameter key to the set of final states
         */
        void WFA::add_final_state( wali_key_t key )
        {
            F.insert(key);
        }

        /*!
         * Return true if parameter key is a final state
         */
        bool WFA::is_final_state( wali_key_t key ) const
        {
            return (F.find(key) != F.end());
        }

        /*!
         * @brief Return the initial state
         */
        wali_key_t WFA::initial_state() const
        {
            return init_state;
        }

        /*!
         * Test if param key is the initial state.
         */
        bool WFA::is_initial_state( wali_key_t key ) const
        {
            return key == init_state;
        }

        /*!
         * @brief add trans (p,g,q,se) to WFA
         */
        void WFA::add_trans(
                wali_key_t p,
                wali_key_t g,
                wali_key_t q,
                sem_elem_t se )
        {
            add_trans( new Trans(p,g,q,se) );
        }

        /*!
         * @brief add Trans *t to WFA
         * Takes care of adding states and calling insert. This
         * method (actually insert) assumes ownership of the memory
         * pointed to by the Trans* t.
         */
        void WFA::add_trans( Trans * t )
        {
            //std::cerr << "\tAdding 'from' state\n";
            add_state( t->from(), t->weight()->zero() );
            //std::cerr << "\tAdding 'to' state\n";
            add_state( t->to(), t->weight()->zero() );
            //std::cerr << "\tInserting Trans\n";
            insert( t );
        }

        bool WFA::find( 
                wali_key_t p,
                wali_key_t g,
                wali_key_t q,
                Trans & t )
        {
            Trans *tret = lookup(p,g,q);
            bool rc = false;
            if( 0 != tret ) {
                rc = true;
                t = *tret;
            }
            return rc;
        }

        void WFA::for_each( ConstTransFunctor & tf ) const
        {
            kp_map_t::const_iterator it = kpmap.begin();
            kp_map_t::const_iterator itEND = kpmap.end();
            for( ; it != itEND ; it++ )
            {
                const trans_list_t & ls = it->second;
                trans_list_t::const_iterator lit = ls.begin();
                for( ; lit != ls.end() ; lit++ )
                {
                    tf(*lit);
                }
            }
        }

        void WFA::for_each( TransFunctor& tf )
        {
            kp_map_t::iterator it = kpmap.begin();
            kp_map_t::iterator itEND = kpmap.end();
            for( ; it != itEND ; it++ )
            {
                trans_list_t & ls = it->second;
                trans_list_t::iterator lit = ls.begin();
                for( ; lit != ls.end() ; lit++ )
                {
                    tf(*lit);
                }
            }
        }

        // TODO : move this class somewhere else
        class StackHasher : public TransFunctor
        {
            public:
                typedef wali::HashMap< wali_key_t , WFA::trans_list_t > stackmap_t;
                stackmap_t stackmap;

                virtual ~StackHasher() {}

                virtual void operator()( Trans * t )
                {
                    wali_key_t stack = t->stack();
                    stackmap_t::iterator it = stackmap.find( stack );
                    if( it == stackmap.end() )
                    {
                        WFA::trans_list_t ls;
                        it = stackmap.insert(stack,ls).first;
                    }
                    it->second.push_back(t);
                }
        };

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

        /* 
         * Intersect 2 WFAs
         */
        WFA WFA::intersect( WeightMaker& wmaker , WFA& fa )
        {
            WFA dest;
            // do init states
            dest.set_initial_state(
                    KeyFactory::get_key(initial_state(),fa.initial_state()) );

            // do final states
            // Pairwise cross of the sets
            std::set< wali_key_t >::iterator keyit = F.begin();
            std::set< wali_key_t >::iterator keyitEND = F.end();
            for( ; keyit != keyitEND ; keyit++ )
            {
                std::set< wali_key_t >::iterator fait = fa.F.begin();
                std::set< wali_key_t >::iterator faitEND = fa.F.end();
                for( ; fait != faitEND ; fait++ )
                {
                    dest.add_final_state( KeyFactory::get_key(*keyit,*fait) );
                }
            }

            // Hash transitions of fa on stack symbol. Then probe the hash
            // with this's transitions to add transitions to dest
            StackHasher hashfa;
            fa.for_each(hashfa);

            kp_map_t::iterator kpit = kpmap.begin();
            kp_map_t::iterator kpitEND = kpmap.end();
            for( ; kpit != kpitEND ; kpit++ )
            {
                trans_list_t & ls = kpit->second;
                if( ls.empty() )
                {
                    continue;
                }
                // Probe StackHasher outside of the inner
                // for loop b/c the kp_map_t hashes on (p,g)
                // so each trans in the list has stack symbol g
                Trans *tprobe = ls.front();
                StackHasher::stackmap_t::iterator stkit;
                stkit = hashfa.stackmap.find( tprobe->stack() );

                if( stkit == hashfa.stackmap.end() )
                {
                    continue;
                }
                trans_list_t::iterator lsit = ls.begin();
                for( ; lsit != ls.end() ; lsit++ )
                {
                    // Probe StackHasher::stackmap with each trans
                    Trans *t = *lsit;
                    // match t w/ all trans in the stkit->second
                    trans_list_t::iterator stklsit = stkit->second.begin();
                    trans_list_t::iterator stklsitEND = stkit->second.end();
                    for( ; stklsit != stklsitEND ; stklsit++ )
                    {
                        Trans *t2 = *stklsit;
                        wali_key_t fromkey = KeyFactory::get_key( t->from(),t2->from() );
                        wali_key_t tokey = KeyFactory::get_key( t->to(),t2->to() );
                        sem_elem_t W = wmaker.make_weight(t->weight(),t2->weight());
                        dest.add_trans(fromkey,t->stack(),tokey,W);
                    }
                }
            }

            return dest;
        }

        //
        // @brief print WFA to param o
        //
        std::ostream & WFA::print( std::ostream & o ) const
        {
            o << "WFA -\n";
            o << "\tInitial State : ";
            o << KeyFactory::key2str(init_state) << std::endl;
            o << "\tF: {";
            std::set< wali_key_t >::const_iterator cit = F.begin();
            std::set< wali_key_t >::const_iterator citEND = F.end();
            bool first = true;
            for( ; cit != citEND ; cit++,first=false )
            {
                if(!first)
                    o << ", ";
                o << KeyFactory::key2str( *cit );
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
                wali_key_t key = stit->first;
                o << "\t" << key << " [label=\"";
                o << KeyFactory::key2str(key);
                o << "\"";
                if( is_initial_state(key) ) {
                    o  << ",color=green,style=filled";
                }
                else if( is_final_state(key) ) {
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
            // TODO : add "forward" or "backward" attribute ... like
            // o << "<WFA direction="forward">\n";
            o << "<WFA>\n";
            o << "\t<initial_state>" << KeyFactory::key2str( initial_state() );
            o << "</initial_state>\n";
            std::set< wali_key_t >::const_iterator Fit = F.begin();
            std::set< wali_key_t >::const_iterator FitEND = F.end();
            for( ; Fit != FitEND ; Fit++ )
            {
                o << "\t<final_state>";
                o << KeyFactory::key2str( *Fit );
                o << "</final_state>\n";
            }
            TransMarshaller marsh(o);
            for_each(marsh);
            o << "</WFA>";
            return o;
        }

        ///////////////////////
        // Begin protected WFA
        ///////////////////////

        /*
         * The action performed by this method remains a mystery...
         */
        Trans * WFA::lookup(
                wali_key_t p,
                wali_key_t g,
                wali_key_t q )
        {
            Trans *tret = 0;
            KeyPair kp(p,g);
            kp_map_t::iterator it = kpmap.find(kp);
            if( it != kpmap.end() )
            {
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
            }
            return tret;
        }

        /*
         * Inserts tnew into the WFA. If a transition matching tnew
         * exists, tnew is deleted.
         */
        Trans * WFA::insert( Trans * tnew )
        {
            ////
            // lookup code repeated to avoid second HashMap lookup
            // when the iterator gets reused below
            ////
            Trans * told = 0;
            kp_map_t::iterator it = kpmap.find(tnew->keypair());
            if( it != kpmap.end() )
            {
                trans_list_t &ls = it->second;
                trans_list_t::iterator lit = ls.begin();
                for( ; lit != ls.end() ; lit++ )
                {
                    Trans *ttmp = *lit;
                    // already matched (p,g,*) so just
                    // check ->to() field 
                    if( tnew->to() == ttmp->to() )
                    {
                        told = ttmp;
                        break;
                    }
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
                if( it == kpmap.end() )
                {
                    trans_list_t ls;
                    it = kpmap.insert(tnew->keypair(),ls).first;
                }
                it->second.push_back(tnew);

                // Set told to tnew for the return statement at end of 
                // method
                told = tnew;

                // Add tnew to the State's trans list
                //Printable::get_wpds_err() << "state_map.find( " << tnew->from() << " )...";
                state_map_t::iterator stit = state_map.find( tnew->from() );
                //Printable::get_wpds_err() << "done.\n ";
                stit->second->add_trans( tnew );

                // if tnew is an eps transition add to eps_map
                if( tnew->stack() == WALI_EPSILON )
                {
                    wali_key_t tokey = tnew->to();
                    eps_map_t::iterator epsit = eps_map.find( tokey );
                    if( epsit == eps_map.end() ) {
                        trans_list_t ls;
                        epsit = eps_map.insert( tokey,ls ).first;
                    }
                    epsit->second.push_back( tnew );
                }
            }
            else {
                // combine new into old
                combine_trans( told,tnew );
                delete tnew;
            }
            return told;
        }

        //
        // This is a virtual method.
        //
        void WFA::combine_trans( Trans * told, Trans * tnew )
        {
            told->combine_weight( tnew->weight() );
        }

        //
        // Add a state to the state map
        //
        void WFA::add_state( wali_key_t key , sem_elem_t zero )
        {
            if( state_map.find( key ) == state_map.end() ) {
                state_map.insert( key , new State( key,zero ) );
            }
        }

        //
        // Return State * corresponding to the key
        //
        State * WFA::get_state( wali_key_t name )
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

    } // namespace wfa

} // namespace wali
/*
 * $Log: WFA.cpp,v $
 * Revision 1.10  2005/06/24 01:14:42  kidd
 * Fix XML output errors for WFA
 *
 * Revision 1.9  2005/06/23 17:11:01  kidd
 * Fix XML output
 *
 * Revision 1.8  2005/06/23 17:07:10  kidd
 * Add Trans::marshall, WFA::marshall and class TransMarshaller
 *
 * Revision 1.7  2005/06/23 16:37:38  kidd
 * Removed CVS $Id: WFA.cpp,v 1.10 2005/06/24 01:14:42 kidd Exp $ from files. Removed some tabs. Fixed header inclusion orders
 * to ensure Common.hpp is included first. Common.hpp now suppresses Visual
 * Studio warning 4786 and ensured RTTI is enabled.
 *
 * Revision 1.6  2005/06/21 23:51:19  kidd
 * Added const qualifier to WFA::is_final_state.
 * Added WFA::print_dot for printing WFA in dot format. Required adding class
 * TransDotty for cycling through the transitions.
 *
 * Revision 1.5  2005/06/17 14:01:43  kidd
 * Cleaned up includes.
 *
 * Revision 1.4  2005/06/16 19:14:25  kidd
 * Added explicit initialization of base class(es) to copy constructor.
 *
 * Revision 1.3  2005/06/07 18:56:43  kidd
 *
 * Initial WFA::intersect implementation. There are some TODOs throughout for
 * things that need touched up.
 *
 * Revision 1.2  2005/06/07 13:25:48  kidd
 *
 * Fixed printing to use KeyFactory. changed add_initial_state to be
 * set_initial_state as only 1 init state is supported.
 *
 * Revision 1.1.1.1  2005/05/31 19:04:01  kidd
 * initial WALi import
 *
 */
