#ifndef wali_wpds_fwpds_FWPDS_GUARD
#define wali_wpds_fwpds_FWPDS_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/Wrapper.hpp"

#include "wali/wpds/ewpds/MergeFunction.hpp"

#include "wali/graph/GraphCommon.hpp"

namespace wali {

    namespace wfa {
        class WFA;
    }
    namespace graph {
        class InterGraph;
    }

    namespace wpds {

        class Wrapper;
        class rule_t;

        namespace fwpds {

            class FWPDS : public WPDS {
                public:
                    typedef ewpds::merge_fn_t mfun_t;
                    typedef HashMap< KeyTriple, mfun_t > merge_rule_hash_t;

                public:
                    FWPDS();
                    FWPDS(Wrapper* wrapper);
                    FWPDS( Worklist<wfa::Trans> * worklist );
                    FWPDS( Wrapper * wrapper , Worklist<wfa::Trans> * worklist );

                    ////////////
                    // add rules
                    ////////////
                    //! @brief create rule with no r.h.s. stack symbols
                    //! @return true if rule existed
                    //! @see sem_elem_t
                    //! @see wali::Key
                    virtual bool add_rule(
                            wali::Key from_state,
                            wali::Key from_stack,
                            wali::Key to_state,
                            sem_elem_t se );

                    //! @brief create rule with one r.h.s. stack symbol
                    //! @return true if rule existed
                    virtual bool add_rule(
                            wali::Key from_state,
                            wali::Key from_stack,
                            wali::Key to_state,
                            wali::Key to_stack1,
                            sem_elem_t se );

                    //! @brief create rule with two r.h.s. stack symbols
                    //! @return true if rule existed
                    virtual bool add_rule(
                            wali::Key from_state,
                            wali::Key from_stack,
                            wali::Key to_state,
                            wali::Key to_stack1,
                            wali::Key to_stack2,
                            sem_elem_t se);

                    //! @brief create rule with two r.h.s. stack symbols
                    //! @return true if rule existed
                    virtual bool add_rule(
                            wali::Key from_state,
                            wali::Key from_stack,
                            wali::Key to_state,
                            wali::Key to_stack1,
                            wali::Key to_stack2,
                            sem_elem_t se,
                            mfun_t mf);


                    ///////////
                    // pre*
                    ///////////
                    virtual void prestar( ::wali::wfa::WFA & input, ::wali::wfa::WFA & output );

                    virtual wali::graph::InterGraph* prestarComputeInterGraph( wfa::WFA& input, wfa::WFA& output );

                    virtual void pre(LinkedTrans* t, ::wali::wfa::WFA& fa, wali::graph::InterGraph& gr );

                    //! Should never be called!
                    virtual void prestarComputeFixpoint( wfa::WFA& fa );

                    ///////////
                    // post*
                    ///////////
                    virtual void poststar( ::wali::wfa::WFA & input, ::wali::wfa::WFA & output );

                    virtual wali::graph::InterGraph* poststarComputeInterGraph( wfa::WFA& input, wfa::WFA& output );

                    virtual void post(LinkedTrans* t, ::wali::wfa::WFA& fa, wali::graph::InterGraph& gr );

                    //! Should never be called!
                    virtual void poststarComputeFixpoint( wfa::WFA& fa );

                    ///////////
                    // helpers
                    ///////////
                    bool checkResults( wfa::WFA& input, wali::graph::InterGraph* gr,bool poststar );

                    bool is_pds_state( Key k ) const;

                protected:
                    mfun_t lookup_merge( rule_t r ) const;
                    mfun_t lookup_merge( KeyTriple trip ) const;

                protected:
                    std::set< wali::Key > pds_states; //!< set of PDS states
                    merge_rule_hash_t merge_rule_hash; //!< map from (a,b,c) to merge_fn_t
                    bool is_ewpds;

            }; // class FWPDS

        } // namespace fwpds

    } // namespace wpds

} // namespace wali

#endif  // wali_wpds_fwpds_FWPDS_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

