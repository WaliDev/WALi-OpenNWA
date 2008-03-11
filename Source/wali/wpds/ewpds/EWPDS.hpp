#ifndef wpds_EWPDS_GUARD
#define wpds_EWPDS_GUARD

/*!
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/SemElemPair.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/ewpds/MergeFunction.hpp"
#include <set>

namespace wali
{
    class SemElem;
    template< typename T > class Worklist;

    namespace wpds 
    {
        class Config;
        class rule_t;
        class RuleFunctor;
        class ConstRuleFunctor;
        class Wrapper;

        namespace ewpds
        {

            using ::wali::wfa::WFA;
            class ERule;
            typedef ref_ptr<ERule> erule_t;


            /*!
             * @class EWPDS
             */

            class EWPDS : public WPDS
            {
                public:
                    typedef HashMap< KeyTriple, rule_t > merge_rule_hash_t;

                public:
                    using WPDS::poststar;
                    using WPDS::prestar;
                    friend class TransCopyPairLinker;

                public:

                    EWPDS( Wrapper * wrapper = 0 );

                    virtual ~EWPDS();

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
                            sem_elem_t se);

                    /*! @brief create rule with two r.h.s. stack symbols
                     *
                     * @return true if rule existed
                     *
                     * @see sem_elem_t
                     * @see merge_fn_t
                     * @see wali::Key
                     */
                    virtual bool add_rule(
                            wali::Key from_state,
                            wali::Key from_stack,
                            wali::Key to_state,
                            wali::Key to_stack1,
                            wali::Key to_stack2,
                            sem_elem_t se,
                            merge_fn_t mf);


                    // Both these versions are provided for EWPDS via WPDS
                    // void prestar( WFA & input, WFA & output );
                    // void prestar( WFA & input);

                    /*!
                     * @brief Perform poststar reachability query
                     *
                     * @param input
                     * @param output
                     *
                     * @see WFA
                     */
                    virtual void poststar( WFA & input, WFA& output );

                    /*!
                     * This method writes the EWPDS to the passed in 
                     * std::ostream parameter. Implements Printable::print.
                     *
                     * @param o the std::ostream this is written to
                     * @return std::ostream this was written to
                     *
                     * @see Printable
                     */
                    virtual std::ostream & print( std::ostream & o ) const;

                    /*!
                     * This method marshalls the EWPDS into the passed
                     * in std::ostream parameter.  Marshalling simply
                     * writes the EWPDS in XML form.
                     *
                     * @return std::ostream the EWPDS was marshalled into
                     */
                    virtual std::ostream & marshall( std::ostream & o ) const;

                    /*!
                     * Override WPDS::operator()(Trans*) for linking b/c
                     * EWPDS uses paired weights
                     */
                    virtual void operator()( wali::wfa::Trans* t );

                    bool is_pds_state(wali::Key k) const;
                    rule_t lookup_rule(wali::Key to_state, wali::Key to_stack1, wali::Key to_stack2) const;
                protected:

                    /*!
                     * @brief helper method for prestar
                     */
                    virtual void prestar_handle_call(
                            wfa::Trans *t1,
                            wfa::Trans *t2,
                            rule_t &r,
                            sem_elem_t delta
                            );

                    /*!
                     * @brief helper method for prestar
                     */
                    virtual void prestar_handle_trans(
                            wfa::Trans * t,
                            WFA & ca  ,
                            rule_t & r,
                            sem_elem_t delta );

                    /*!
                     * @brief helper method for poststar
                     */
                    /*
                    virtual void poststar_handle_eps_trans(
                            wfa::Trans *teps, 
                            wfa::Trans *tprime,
                            sem_elem_t delta);
                            */

                    /*!
                     * @brief helper method for poststar
                     */
                    virtual void poststar_handle_trans(
                            wfa::Trans * t ,
                            WFA & ca   ,
                            rule_t & r,
                            sem_elem_t delta
                            );

                    //void copy_and_link_and_pair( const WFA & in, WFA & dest );

                    wfa::Trans* Eupdate_prime(
                        Key from, Key stack, Key to,
                        erule_t er, sem_elem_t wAtCall);

                private:
                    std::set<wali::Key> pds_states; // set of PDS states
                    merge_rule_hash_t merge_rule_hash; // FIXME: verify correct usage of HashMap
                    bool usePairsDuringCopy;

            }; // class EWPDS

        } // namespace ewpds

    } // namespace wpds

} // namespace wali

#endif  // wpds_EWPDS_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
