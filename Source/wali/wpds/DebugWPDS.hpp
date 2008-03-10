#ifndef wali_wpds_DEBUG_WPDS_GUARD
#define wali_wpds_DEBUG_WPDS_GUARD 1
/*!
 * @author Nicholas Kidd
 */

#include "wali/wpds/WPDS.hpp"

namespace wali
{
    namespace wpds
    {
        /*!
         *
         * @class DebugWPDS
         *
         * This class emits lots of debugging information when
         * running poststar or prestar. It also single steps poststar
         * and prestar
         */
        class DebugWPDS : public WPDS
        {
            public:
                DebugWPDS();
                DebugWPDS( Wrapper * wrapper );
                DebugWPDS( Worklist<wfa::Trans> * worklist );
                DebugWPDS( Wrapper * wrapper , Worklist<wfa::Trans> * worklist );

                virtual ~DebugWPDS();

            protected:
                virtual void prestarComputeFixpoint( ::wali::wfa::WFA& fa );

                virtual void poststarComputeFixpoint( ::wali::wfa::WFA& fa );

                virtual void post( ::wali::wfa::Trans* t , ::wali::wfa::WFA& fa );

        };

    }   // namespace wpds

}   // namespace wali
#endif  // wali_wpds_DEBUG_WPDS_GUARD
