/*!
 * @author Nick Kidd
 */

#include "wali/wpds/Wrapper.hpp"
#include "wali/wfa/ITrans.hpp"
#include "wali/wpds/Rule.hpp"

namespace wali
{

  namespace wpds
  {
    // default does nothing
    sem_elem_t Wrapper::wrap( wfa::ITrans& t )
    {
      return t.weight();
    }

    // default does nothing
    sem_elem_t Wrapper::wrap( wpds::Rule& r )
    {
      return r.weight();
    }

    // default does nothing
    merge_fn_t Wrapper::wrap( merge_fn_t user_merge )
    {
      return user_merge;
    }

    sem_elem_t Wrapper::unwrap( sem_elem_t se )
    {
      return se;
    }
  }

}

