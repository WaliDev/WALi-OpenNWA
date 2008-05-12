#ifndef wali_wpds_CTRANS_GUARD
#define wali_wpds_CTRANS_GUARD 1

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

#endif  // wali_wpds_CTRANS_GUARD

