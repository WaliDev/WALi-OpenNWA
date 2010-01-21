/*!
 * @author Nicholas Kidd
 */

#include "wali/wpds/LinkedTrans.hpp"
#include "wali/wpds/Config.hpp"

namespace wali
{
  namespace wpds
  {
    LinkedTrans::LinkedTrans(  wali_key_t from,
        wali_key_t stack,
        wali_key_t to,
        const sem_elem_t & se,
        Config *config_
        ) : Trans(from,stack,to,se),config(config_) {}

    LinkedTrans::~LinkedTrans() {}

  } // namespace wpds

} // namespace wali

