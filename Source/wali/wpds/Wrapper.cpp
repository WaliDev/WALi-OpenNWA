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
        sem_elem_t Wrapper::wrap( wfa::Trans& t )
        {
            return t.weight();
        }

        sem_elem_t Wrapper::wrap( wpds::Rule& r )
        {
            return r.weight();
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
