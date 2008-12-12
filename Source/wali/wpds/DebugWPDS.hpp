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
        static const std::string XMLTag;

      public:
        DebugWPDS();
        DebugWPDS( ref_ptr<Wrapper> wrapper );

        virtual ~DebugWPDS();

      protected:
        virtual void prestarComputeFixpoint( wfa::WFA& fa );

        virtual void poststarComputeFixpoint( wfa::WFA& fa );

        virtual void post( wfa::ITrans* t , wfa::WFA& fa );

        virtual void poststar_handle_trans(
            wfa::ITrans * t ,
            wfa::WFA & ca   ,
            rule_t & r,
            sem_elem_t delta);

        virtual void update(
            Key from, Key stack, Key to, 
            sem_elem_t se, Config * cfg );

        virtual wfa::ITrans* update_prime(
            Key from, //<! Guaranteed to be a generated state
            wfa::ITrans* call, //<! The call transition
            rule_t r, //<! The push rule
            sem_elem_t delta, //<! Delta change on the call transition
            sem_elem_t wWithRule //<! delta \extends r->weight()
            );

    }; // class DebugWPDS

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_DEBUG_WPDS_GUARD

