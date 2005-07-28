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
    class Worklist;

    namespace wpds 
    {
        class Config;
        class rule_t;
        class LinkedTrans;
        class RuleFunctor;
        class ConstRuleFunctor;
        class TransCopyLinker;

        namespace ewpds
        {

            using ::wali::wfa::WFA;

            /*!
             * @class EWPDS
             */

            class EWPDS : public WPDS
            {
                public:
                    typedef HashMap< KeyTriple, rule_t > merge_rule_hash_t;

                public:
                    friend class TransCopyPairLinker;

                public:

                    EWPDS( Worklist * worklist = 0 );

                    virtual ~EWPDS();

                    /*! @brief create rule with no r.h.s. stack symbols
                     *
                     * @return true if rule existed
                     *
                     * @see sem_elem_t
                     * @see wali_key_t
                     */
                    virtual bool add_rule(
                            wali_key_t from_state,
                            wali_key_t from_stack,
                            wali_key_t to_state,
                            sem_elem_t se );

                    /*! @brief create rule with one r.h.s. stack symbol
                     *
                     * @return true if rule existed
                     *
                     * @see sem_elem_t
                     * @see wali_key_t
                     */
                    virtual bool add_rule(
                            wali_key_t from_state,
                            wali_key_t from_stack,
                            wali_key_t to_state,
                            wali_key_t to_stack1,
                            sem_elem_t se );

                    /*! @brief create rule with two r.h.s. stack symbols
                     *
                     * @return true if rule existed
                     *
                     * @see sem_elem_t
                     * @see wali_key_t
                     */
                    virtual bool add_rule(
                            wali_key_t from_state,
                            wali_key_t from_stack,
                            wali_key_t to_state,
                            wali_key_t to_stack1,
                            wali_key_t to_stack2,
                            sem_elem_t se);

                    /*! @brief create rule with two r.h.s. stack symbols
                     *
                     * @return true if rule existed
                     *
                     * @see sem_elem_t
                     * @see merge_fn_t
                     * @see wali_key_t
                     */
                    virtual bool add_rule(
                            wali_key_t from_state,
                            wali_key_t from_stack,
                            wali_key_t to_state,
                            wali_key_t to_stack1,
                            wali_key_t to_stack2,
                            sem_elem_t se,
                            merge_fn_t mf);


                    /*!
                     * @brief Perform prestar reachability query
                     *
                     * @return WFA
                     *
                     * @see WFA
                     */
                    virtual WFA prestar( const WFA & input );

                    /*!
                     * @brief Perform poststar reachability query
                     *
                     * @return WFA
                     *
                     * @see WFA
                     */
                    virtual WFA poststar( const WFA & input );

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

                    bool is_pds_state(wali_key_t k) const;
                    rule_t lookup_rule(wali_key_t to_state, wali_key_t to_stack1, wali_key_t to_stack2) const;
                protected:

                    /*!
                     * @brief helper method for prestar
                     */
                    void prestar_handle_trans(
                            LinkedTrans * t,
                            WFA & ca  ,
                            rule_t & r,
                            sem_elem_t delta );

                    /*!
                     * @brief helper method for poststar
                     */
                    void poststar_handle_trans(
                            LinkedTrans * t ,
                            WFA & ca   ,
                            rule_t & r,
                            sem_elem_t delta
                            );

                    void EWPDS::copy_and_link_and_pair( const WFA & in, WFA & dest );

                    virtual bool make_rule(
                            Config * f,
                            Config * t,
                            wali_key_t stk2,
                            sem_elem_t se,
                            merge_fn_t mf,
                            rule_t & r );

                private:
                    std::set<wali_key_t> pds_states; // set of PDS states
                    merge_rule_hash_t merge_rule_hash; // FIXME: verify correct usage of HashMap

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
