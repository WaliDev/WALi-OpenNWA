#ifndef wpds_EWPDS_GUARD
#define wpds_EWPDS_GUARD 1

/*!
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/SemElemPair.hpp"
#include "wali/MergeFn.hpp"
#include "wali/wpds/WPDS.hpp"
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
                    // void poststar( WFA & input, WFA & output );
                    // void poststar( WFA & input);

                    /*!
                     * @brief Perform prestar reachability query
                     *
                     * @param input
                     * @param output
                     *
                     * @see WFA
                     */
                    virtual void prestar( WFA & input, WFA& output );

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
                     * Override WPDS::operator()(ITrans*) for linking b/c
                     * EWPDS uses paired weights
                     */
                    virtual void operator()( wfa::ITrans* t );

                    rule_t lookup_rule(wali::Key to_state, wali::Key to_stack1, wali::Key to_stack2) const;
                protected:

                    /*!
                     * @brief helper method for prestar
                     */
                    virtual void prestar_handle_call(
                            wfa::ITrans *t1,
                            wfa::ITrans *t2,
                            rule_t &r,
                            sem_elem_t delta
                            );

                    /*!
                     * @brief helper method for prestar
                     */
                    virtual void prestar_handle_trans(
                            wfa::ITrans * t,
                            WFA & ca  ,
                            rule_t & r,
                            sem_elem_t delta );

                    /*!
                     * @brief helper method for poststar
                     */

                    /*!
                     * @brief helper method for poststar
                     */
                    virtual void poststar_handle_trans(
                            wfa::ITrans * t ,
                            WFA & ca   ,
                            rule_t & r,
                            sem_elem_t delta
                            );

                    virtual void update_etrans(
                                               Key from
                                               , Key stack
                                               , Key to
                                               , sem_elem_t se
                                               , Config * cfg
                                               );
              
                    virtual void update(
                                        Key from
                                        , Key stack
                                        , Key to
                                        , sem_elem_t se
                                        , Config * cfg
                                        );

                    virtual wfa::ITrans* update_prime(
                        Key from, //<! Guaranteed to be a generated state
                        wfa::ITrans* call, //<! The call transition
                        rule_t r, //<! The push rule
                        sem_elem_t delta, //<! Delta change on the call transition
                        sem_elem_t wWithRule //<! delta \extends r->weight()
                        );

                private:
                    merge_rule_hash_t merge_rule_hash; // FIXME: verify correct usage of HashMap
                protected:
                    bool addEtrans; // Used during update()
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
