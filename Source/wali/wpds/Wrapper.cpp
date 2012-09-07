/**
 * @author Nicholas Kidd
 * @version $Id: Wrapper.cpp 1182 2011-06-14 20:56:58Z driscoll $
 */

#include "wali/wpds/Wrapper.hpp"
#include "wali/wfa/ITrans.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/ewpds/ERule.hpp"

namespace wali
{
  namespace wpds
  {
    // default does nothing
    sem_elem_t Wrapper::wrap( wfa::ITrans const & t )
    {
      return t.weight();
    }

    // default does nothing
    sem_elem_t Wrapper::wrap( wpds::Rule const & r )
    {
      return r.weight();
    }

    // default does nothing
    merge_fn_t Wrapper::wrap( wpds::ewpds::ERule const & r ATTR_UNUSED, merge_fn_t user_merge )
    {
      (void) r;
      return user_merge;
    }

    sem_elem_t Wrapper::unwrap( sem_elem_t se )
    {
      return se;
    }

    merge_fn_t Wrapper::unwrap( merge_fn_t mf )
    {
      return mf;
    }

  } // namespace wpds

} // namespace wali

