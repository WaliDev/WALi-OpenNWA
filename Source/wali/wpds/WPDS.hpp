#ifndef wali_wpds_WPDS_GUARD
#define wali_wpds_WPDS_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/HashMap.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/SemElem.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include <iostream>
#include <set>

namespace wali
{
    template< typename T > class Worklist;

    namespace wfa
    {
        class Trans;
    }

    namespace wpds
    {

        class Config;
        class rule_t;
        class LinkedTrans;
        class RuleFunctor;
        class ConstRuleFunctor;
        class Wrapper;

        /*!
         * @class WPDS
         */

        class WPDS : public Printable, public wali::wfa::TransFunctor
        {

            public:
                static const std::string XMLTag;

            protected:
                typedef HashMap< KeyPair,Config * > chash_t;
                typedef chash_t::iterator iterator;
                typedef chash_t::const_iterator const_iterator;

                /*!
                 * r2hash_t is a map from key to list of rules. The key
                 * is the second RHS stack symbol of a rule. For example, 
                 * the rule R1 = < s1,a > -> < s2, b c > will add R1 to 
                 * the list that is mapped to by the stack symbol c.
                 */
                typedef HashMap< wali::Key, std::list< rule_t > > r2hash_t;

            private:

            public:

                WPDS();
                WPDS( Wrapper * wrapper );
                WPDS( Worklist<wfa::Trans> * worklist );
                WPDS( Wrapper * wrapper , Worklist<wfa::Trans> * worklist );

                virtual ~WPDS();

                /*!
                 * Clears all rules from the WPDS
                 */
                virtual void clear();

                /*! @brief create rule with no r.h.s. stack symbols
                 *
                 * @return true if rule existed
                 *
                 * @see sem_elem_t
                 * @see wali::Key
                 */
                virtual bool add_rule(
                        wali::Key from_state,
                        wali::Key from_stack,
                        wali::Key to_state,
                        sem_elem_t se );

                /*! @brief create rule with one r.h.s. stack symbol
                 *
                 * @return true if rule existed
                 *
                 * @see sem_elem_t
                 * @see wali::Key
                 */
                virtual bool add_rule(
                        wali::Key from_state,
                        wali::Key from_stack,
                        wali::Key to_state,
                        wali::Key to_stack1,
                        sem_elem_t se );

                /*! @brief create rule with two r.h.s. stack symbols
                 *
                 * @return true if rule existed
                 *
                 * @see sem_elem_t
                 * @see wali::Key
                 */
                virtual bool add_rule(
                        wali::Key from_state,
                        wali::Key from_stack,
                        wali::Key to_state,
                        wali::Key to_stack1,
                        wali::Key to_stack2,
                        sem_elem_t se );

                /*!
                 * @brief Perform prestar reachability query
                 *
                 * @return ::wali::wfa::WFA
                 *
                 * @see ::wali::wfa::WFA
                 */
                virtual ::wali::wfa::WFA prestar( ::wali::wfa::WFA & input );

                /*!
                 * @brief Perform prestar reachability query
                 * The result of the query is stored in
                 * the parameter WFA& output. Any transitions
                 * in output before the query will be there
                 * after the query but will have no effect
                 * on the reachability query.
                 *
                 * @return void
                 *
                 * @see ::wali::wfa::WFA
                 */
                virtual void prestar( ::wali::wfa::WFA & input, ::wali::wfa::WFA & output );

                /*!
                 * @brief Perform poststar reachability query
                 *
                 * @return WFA
                 *
                 * @see ::wali::wfa::WFA
                 */
                virtual ::wali::wfa::WFA poststar( ::wali::wfa::WFA & input );

                /*!
                 * @brief Perform poststar reachability query.
                 * The result of the query is stored in
                 * the parameter WFA& output. Any transitions
                 * in output before the query will be there
                 * after the query but will have no effect
                 * on the reachability query.
                 *
                 * @return ::wali::wfa::WFA
                 *
                 * @see ::wali::wfa::WFA
                 */
                virtual void poststar( ::wali::wfa::WFA & input, ::wali::wfa::WFA & output );

                /*!
                 * This method writes the WPDS to the passed in 
                 * std::ostream parameter. Implements Printable::print.
                 *
                 * @param o the std::ostream this is written to
                 * @return std::ostream this was written to
                 *
                 * @see Printable
                 */
                virtual std::ostream & print( std::ostream & o ) const;

                /*!
                 * This method marshalls the WPDS into the passed
                 * in std::ostream parameter.  Marshalling simply
                 * writes the WPDS in XML form.
                 *
                 * @return std::ostream the WPDS was marshalled into
                 */
                virtual std::ostream & marshall( std::ostream & o ) const;

                /*!
                 * @brief apply ConstRuleFunctor to each rule in WPDS
                 *
                 * @see ConstRuleFunctor
                 * @see Rule
                 * @see rule_t
                 */
                virtual void for_each( ConstRuleFunctor &func ) const;

                /*!
                 * @brief apply RuleFunctor to each rule in WPDS
                 *
                 * @see RuleFunctor
                 * @see Rule
                 * @see rule_t
                 */
                virtual void for_each( RuleFunctor &func );

                /*!
                 * Implementation of TransFunctor
                 */
                virtual void operator()( wali::wfa::Trans* t );

            protected:

                /*! @brief Actually creates the rule, hanldes the mappings,
                 * etc.
                 *
                 * @return true if rule existed
                 *
                 * @see sem_elem_t
                 * @see wali::Key
                 */
                virtual bool add_rule(
                        wali::Key from_state,
                        wali::Key from_stack,
                        wali::Key to_state,
                        wali::Key to_stack1,
                        wali::Key to_stack2,
                        sem_elem_t se,
                        rule_t& r );

                /*!
                 * @brief copy relevant material from input WFA to output WFA
                 */
                virtual void setupOutput( ::wali::wfa::WFA& input, ::wali::wfa::WFA& fa );

                /*!
                 * @brief Gets WPDS ready for fixpoint
                 */
                virtual void prestarSetupFixpoint( ::wali::wfa::WFA& input, ::wali::wfa::WFA& fa );

                /*!
                 * @brief Performs the fixpoint computation
                 */
                virtual void prestarComputeFixpoint( ::wali::wfa::WFA& fa );

                /*!
                 * @brief Performs pre for 1 Trans
                 */
                virtual void pre( LinkedTrans * t, ::wali::wfa::WFA& fa );

                /*!
                 * @brief helper method for prestar
                 */
                virtual void prestar_handle_trans(
                        LinkedTrans * t,
                        ::wali::wfa::WFA & ca  ,
                        rule_t & r,
                        sem_elem_t delta );

                /*!
                 * @brief Gets WPDS ready for fixpoint
                 */
                virtual void poststarSetupFixpoint( ::wali::wfa::WFA& input, ::wali::wfa::WFA& fa );

                /*!
                 * @brief Performs the fixpoint computation
                 */
                virtual void poststarComputeFixpoint( ::wali::wfa::WFA& fa );

                /*!
                 * @brief Performs post for 1 Trans
                 */
                virtual void post( LinkedTrans * t, ::wali::wfa::WFA& fa );

                /*!
                 * @brief helper method for poststar
                 */
                virtual void poststar_handle_trans(
                        LinkedTrans * t ,
                        ::wali::wfa::WFA & ca   ,
                        rule_t & r,
                        sem_elem_t delta
                        );

                /*!
                 * @brief create a new temp state from two existing states
                 *
                 * gen_state is only used by poststar
                 *
                 * @return wali::Key for new state
                 */
                virtual wali::Key gen_state( wali::Key state, wali::Key stack );

                /*!
                 * @brief link input WFA transitions to Configs
                 *
                 * @see Config
                 * @see Trans
                 * 
                 */
                //virtual void copy_and_link( ::wali::wfa::WFA & in, ::wali::wfa::WFA & out );

                /*!
                 * Create the Config for the state and stack KeyPair.
                 * If the Config already exists just return that.
                 *
                 * @return Config pointer
                 */
                virtual Config * make_config( wali::Key state, wali::Key stack );

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
                virtual bool make_rule(
                        Config *f,
                        Config *t,
                        wali::Key stk2,
                        sem_elem_t se,
                        rule_t& r );

                /*!
                 * Find config with KeyPair(state,stack)
                 *
                 * @return the Config * or 0 if it doesn't exist
                 *
                 * @see Config
                 * @see KeyPair
                 */
                virtual Config * find_config( wali::Key state, wali::Key stack );

                /*! @brief helper method for fixpoint loop
                 *
                 * return true if LinkedTrans was retrieved from
                 * worklist, false if worklist is empty
                 */
                virtual bool get_from_worklist( LinkedTrans * & );

                /*!
                 * @brief helper function to create and link a transition
                 *
                 */
                virtual void update(
                        wali::Key from
                        , wali::Key stack
                        , wali::Key to
                        , sem_elem_t se
                        , Config * cfg
                        );

                /*!
                 * update_prime does not need to take a Config b/c no Config
                 * will match a transition taht is created here. The from state
                 * is not \in WFA.P. Therefore we do not need to add it to the
                 * worklist.
                 *
                 * @return generated transition
                 */
                virtual LinkedTrans * update_prime(
                        wali::Key from
                        , wali::Key stack
                        , wali::Key to
                        , sem_elem_t se
                        );

                /*!
                 * @return const chash_t reference
                 */
                const chash_t & config_map() const {
                    return configs;
                }

                /*!
                 * @return chash_t reference
                 */
                chash_t & config_map() {
                    return configs;
                }

            private: // methods

            protected: // data members
                Wrapper * wrapper;
                Worklist<wfa::Trans> * worklist;
                chash_t configs;
                std::set< Config * > rule_zeroes;
                r2hash_t r2hash;
                wali::wfa::WFA* currentOutputWFA;

            private:

        };

    } // namespace wpds

} // namespace wali

#endif  // wali_wpds_WPDS_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
