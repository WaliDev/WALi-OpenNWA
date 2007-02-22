#ifndef wali_wpds_fwpds_FWPDS_GUARD
#define wali_wpds_fwpds_FWPDS_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/Wrapper.hpp"

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

        namespace fwpds {

            class FWPDS : public WPDS {
                public:
                    FWPDS();
                    FWPDS(Wrapper* wrapper);
                    FWPDS( Worklist<wfa::Trans> * worklist );
                    FWPDS( Wrapper * wrapper , Worklist<wfa::Trans> * worklist );
                    
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

