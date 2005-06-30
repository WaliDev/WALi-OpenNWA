#ifndef wali_wfa_WFA_GUARD
#define wali_wfa_WFA_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/SemElem.hpp"
#include "wali/HashMap.hpp"
#include "wali/KeyContainer.hpp"
#include <iostream>
#include <list>
#include <set>

namespace wali
{
    class Worklist;

    namespace wpds
    {
        class WPDS;
        class TransCopyLinker;
        namespace ewpds
        {
            class EWPDS;
        }
    }

    namespace wfa
    {
        class State;
        class Trans;
        class TransFunctor;
        class ConstTransFunctor;
        class WeightMaker;

        /*! @class WFA
         *
         * TODO:
         *  - Should state_map_t have type HashMap< wali_key_t , ref_ptr<State> >
         *      This allows for automatic collection of States via HashMap
         */

        class WFA : public Printable
        {
            //
            // Types
            //
            public:
                /*!
                 * @enum query_t
                 * query_t determines how the weights are extended
                 * during path_summary. INORDER is the normal each state's
                 * weight is the combine of all outgoing transitions's weights
                 * extended by the "to" state. REVERSE reverses this. INORDER
                 * corresponds to a prestar query while REVERSE corresponds to
                 * a poststar query when used with WPDS
                 */
                enum query_t { INORDER,REVERSE,MAX };

                typedef std::list< Trans * > trans_list_t;
                typedef wali::HashMap< KeyPair, trans_list_t > kp_map_t;
                typedef wali::HashMap< wali_key_t , State * > state_map_t;
                typedef wali::HashMap< wali_key_t , trans_list_t > eps_map_t;

                friend class ::wali::wpds::WPDS;
                friend class ::wali::wpds::ewpds::EWPDS;

            protected:
            private:

                //
                // Methods
                //
            public:
                WFA( query_t q = INORDER );
                WFA( const WFA & rhs );
                WFA & operator=( const WFA & rhs );

                virtual ~WFA();

                /*!
                 * Remove all transitions from the WFA
                 */
                virtual void clear();

                /*!
                 * @brief set initial state
                 *
                 * A WFA has 1 initial state. This method will
                 * set it to the key parameter
                 *
                 * @param key the key for the desired initial state
                 * @return wali_key_t that is the old initial stat
                 */
                wali_key_t set_initial_state( wali_key_t key );

                /*!
                 * @brief Return the initial state
                 *
                 * @return wali_key_t for the initial state
                 */
                wali_key_t initial_state() const;

                /*!
                 * Test if param key is the initial state.
                 *
                 * @return true if key == WFA::initial_state()
                 */
                bool is_initial_state( wali_key_t key ) const;

                /*!
                 * Add parameter key to the set of final states
                 */
                void add_final_state( wali_key_t key );

                /*!
                 * Return true if parameter key is a final state
                 */
                bool is_final_state( wali_key_t key ) const;

                /*! @brief Add transition (p,g,q) to the WFA
                 *
                 * @see wali_key_t
                 * @see sem_elem_t
                 */
                virtual void add_trans(
                        wali_key_t p,
                        wali_key_t g,
                        wali_key_t q,
                        sem_elem_t se );

                /*!
                 * @brief Add Trans t to the WFA
                 *
                 * @see Trans
                 */
                virtual void add_trans( Trans * t );

                /*!
                 * @brief return true if transition matching (p,g,q) exists
                 * and copy that transition into the ref parameter t
                 *
                 * If a Trans matching (p,g,q) exists then it is copied into the
                 * passed in reference "Trans & t". This way the user is not given
                 * and handle to the WFA's internal Trans * pointer and deleting
                 * all transitions associated with this WFA when it is deleted is
                 * safe. However, because the weight of this transition is a
                 * sem_elem_t (ref_ptr< SemElem >) modifying the weight from
                 * the Trans & t will result in modification of the weight on
                 * the interal Trans * object.
                 *
                 * @return bool true if transition matching (p,g,q) exists
                 * @see Trans
                 * @see SemElem
                 * @see sem_elem_t
                 */
                virtual bool find( 
                        wali_key_t p,
                        wali_key_t g,
                        wali_key_t q,
                        Trans & t );

                /*!
                 * @brief invoke TransFunctor tf on each Trans
                 */
                virtual void for_each( TransFunctor& tf );

                /*!
                 * @brief invoke ConstTransFunctor on each Trans
                 */
                virtual void for_each( ConstTransFunctor& tf ) const;

                /*!
                 * Intersect this with parameter fa. This is a wrapper
                 * for intersect( WeightMaker&,WFA& ) that passes
                 * the WeightMaker KeepBoth.
                 *
                 * @see wali::wfa::WeightMaker
                 * @see wali::wfa::KeepBoth
                 */
                virtual WFA intersect( WFA& fa );

                /*!
                 * Intersect this with parameter fa and return the result
                 * by value. The parameter WeightMaker determines how
                 * intersection should join the weights on matching
                 * transitions.
                 *
                 * @see wali::wfa::WeightMaker
                 */
                virtual WFA intersect( WeightMaker& wmaker , WFA& fa );

                /*!
                 * Performs path summary. Simply calls the path_summary with
                 * the default Worklist provided by WALi.
                 */
                void path_summary();

                /*!
                 * Performs path summary with the specified Worklist
                 */
                virtual void path_summary( Worklist& wl );

                /*!
                 * Write the WFA to the std::ostream parameter, implements
                 * Printable::print
                 *
                 * @param o the std::ostream this is written to
                 * @return std::ostream WFA was written to
                 *
                 * @see Printable
                 */
                virtual std::ostream & print( std::ostream & ) const;

                /*!
                 * @brief Print WFA in dot format
                 */
                virtual std::ostream& print_dot(
                        std::ostream& o,
                        bool print_weights=true ) const;

                /*!
                 * @brief marshall WFA in XML 
                 */
                virtual std::ostream& marshall( std::ostream& o ) const;

            protected:

                /*!
                 * @brief lookup returns a pointer to a transition matching (p,g,q)
                 *
                 * @return pointer to Trans or NULL(0) if it does not exist
                 */
                Trans * lookup(
                        wali_key_t p,
                        wali_key_t g,
                        wali_key_t q );

                /*!
                 * @brief inserts tnew into the WFA
                 *
                 * This method inserts Trans * tnew into the WFA.  If a transition
                 * told matching (p,g,q) of tnew already exists, then tnew is
                 * combined into told.
                 *
                 * @warning: Note that insert assumes ownership of the passed
                 *      in parameter tnew
                 *
                 * @return pointer to real transition
                 */
                Trans * insert( Trans * tnew );

                /*! @brief fold tnew into told
                 */
                virtual void combine_trans( Trans * told, Trans * tnew );

                /*! 
                 * Create a State * for the key wali_key_t
                 */
                void add_state( wali_key_t key , sem_elem_t zero );

                /*!
                 * WFA::get_state returns a reference to the State named
                 * by the parameter name. If not such State exists a NULL
                 * pointer is returned.
                 *
                 * @param name the key for this State
                 * @return pointer to the State named by param name
                 *
                 */

                State * get_state( wali_key_t name );

                /*!
                 * setup_fixpoint clears each states markable flag and sets
                 * the states weight to zero
                 */
                void setup_fixpoint( Worklist& wl );

            private:

                //
                // Variables
                //
            public:
            protected:
                kp_map_t kpmap;          //! < map from KeyPair to list of trans
                state_map_t state_map;   //! < map from key to State
                eps_map_t eps_map;       //! < map from to state to list of eps trans
                wali_key_t init_state;   //! < initial state of WFA
                std::set< wali_key_t > F;//! < set of final states
                query_t query;           //! < determine the extend order for path_summary

            private:
        };

    } // namespace wfa

} // namespace wali

#endif  // wali_wfa_WFA_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
