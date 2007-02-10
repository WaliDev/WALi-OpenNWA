#ifndef wali_wpds_fwpds_FWPDS_GUARD
#define wali_wpds_fwpds_FWPDS_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/Wrapper.hpp"
#include "wali/wpds/fwpds/FwpdsCommon.hpp"
namespace wali {

    namespace wfa {
        class WFA;
    }
    namespace wpds {

        class Wrapper;

        namespace fwpds {
            class InterGraph;

            class FWPDS : public WPDS {
                public:
                    FWPDS();
                    FWPDS(Wrapper* wrapper);
                    FWPDS( Worklist<wfa::Trans> * worklist );
                    FWPDS( Wrapper * wrapper , Worklist<wfa::Trans> * worklist );

                    virtual void poststar( ::wali::wfa::WFA& input, ::wali::wfa::WFA& fa );

                    virtual void poststarComputeFixpoint( ::wali::wfa::WFA& fa );

                    virtual void post(LinkedTrans* t, ::wali::wfa::WFA& fa, InterGraph& gr );
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

