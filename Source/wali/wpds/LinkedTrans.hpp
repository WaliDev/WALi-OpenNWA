#ifndef wali_wpds_CTRANS_GUARD
#define wali_wpds_CTRANS_GUARD 1

// Disable
//   warning C4250: warning C4250: 'wali::wpds::LinkedTrans' : inherits 'wali::Markable::wali::Markable::mark' via dominance
// for MSVC
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable: 4250)
#endif

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/wfa/Trans.hpp"

namespace wali
{
  namespace wpds
  {
    class WPDS;
    class Config;

    class LinkedTrans : public wali::wfa::Trans
    {
      public:
        friend class WPDS;

      public:
        LinkedTrans(  wali_key_t from,
            wali_key_t stack,
            wali_key_t to,
            const sem_elem_t & se,
            Config *config
            );

        virtual ~LinkedTrans();

        Config *config;
    };

  } // namespace wpds

} // namespace wali


// Restore the warning state
#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif  // wali_wpds_CTRANS_GUARD

