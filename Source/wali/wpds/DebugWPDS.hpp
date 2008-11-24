#ifndef wali_wpds_DEBUG_WPDS_GUARD
#define wali_wpds_DEBUG_WPDS_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/wpds/WPDS.hpp"

namespace wali
{
  namespace wpds
  {
    /**
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
        DebugWPDS( ref_ptr<Wrapper> wrapper );

        virtual ~DebugWPDS();

      protected:
        virtual void prestarComputeFixpoint( wfa::WFA& fa );

        virtual void poststarComputeFixpoint( wfa::WFA& fa );

        virtual void post( wfa::ITrans* t , wfa::WFA& fa );

    }; // class DebugWPDS

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_DEBUG_WPDS_GUARD

