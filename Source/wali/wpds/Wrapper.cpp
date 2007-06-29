/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wpds/Wrapper.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wpds/Rule.hpp"

namespace wali
{

    namespace wpds
    {
        // default does nothing
        sem_elem_t Wrapper::wrap( wfa::Trans& t )
        {
            return t.weight();
        }

        // default does nothing
        sem_elem_t Wrapper::wrap( wpds::Rule& r )
        {
            return r.weight();
        }

        // default does nothing
        Wrapper::merge_fn_t Wrapper::wrap( merge_fn_t user_merge )
        {
            return user_merge;
        }

        sem_elem_t Wrapper::unwrap( sem_elem_t se )
        {
            return se;
        }
    }

}

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
