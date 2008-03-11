/*!
 * @author Nick Kidd
 * $Id$
 */

#include "wali/Common.hpp"
#include "wali/wpds/ewpds/ETrans.hpp"

namespace wali {
  namespace wpds {
    namespace ewpds {

      ETrans::ETrans(
          Key f, Key s, Key t,
          sem_elem_t wAtCall,
          sem_elem_t wAfterCall,
          merge_fn_t merge)
        : Trans(f,s,t,wAfterCall),wAtCall(wAtCall),mf(merge)
      {
      }

      ETrans::~ETrans()
      {
      }

      wfa::Trans* ETrans::copy() const {
        return new ETrans(from(),stack(),to(),wAtCall,weight(),mf);
      }

      sem_elem_t ETrans::poststar_eps_closure( sem_elem_t se ) {
        return mf->apply_f(wAtCall,se);
      }

      void ETrans::combineTrans( Trans* tp )
      {
        // TODO - change dynamic_cast to static_cast 
        //        When happy with impl.
        ETrans* e = dynamic_cast<ETrans*>(tp);
        if( e != 0) {
          // Need to update wAtCall
          wAtCall = wAtCall->combine(e->wAtCall);
        }
        Trans::combineTrans(tp);
      }

    } // namespace ewpds
  } // namespace wpds
} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 2 ***
   ;;; End: ***
*/

