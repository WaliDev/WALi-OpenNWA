/*!
 * @author Nick Kidd
 * $Id$
 */

#include "wali/Common.hpp"

#include "wali/wfa/Trans.hpp"

#include "wali/wpds/ewpds/ETrans.hpp"

namespace wali {
  namespace wpds {
    namespace ewpds {

      ETrans::ETrans(
          Key f, Key s, Key t,
          sem_elem_t wAtCall,
          sem_elem_t wAfterCall,
          erule_t er)
        : DecoratorTrans(new wfa::Trans(f,s,t,wAfterCall)),wAtCall(wAtCall),erule(er)
      {
      }

      ETrans::ETrans(
          ITrans* d,
          sem_elem_t wAtCall,
          erule_t er)
        : DecoratorTrans(d),wAtCall(wAtCall),erule(er)
      {
      }

      ETrans::~ETrans()
      {
      }

      wfa::ITrans* ETrans::copy() const {
        return new ETrans(getDelegate()->copy(),wAtCall,erule);
      }

      merge_fn_t ETrans::getMergeFn() const {
        return erule->merge_fn();
      }

      erule_t ETrans::getERule() const {
        return erule;
      }

      sem_elem_t ETrans::getWeightAtCall() const {
        return wAtCall;
      }

      void ETrans::setWeightAtCall(sem_elem_t wt) {
        wAtCall = wt;
      }

      sem_elem_t ETrans::poststar_eps_closure( sem_elem_t se ) {
        return getMergeFn()->apply_f(wAtCall,se);
      }

      sem_elem_t ETrans::make_weight( wali::wfa::WeightMaker &wmaker, sem_elem_t se ) {
        return wmaker.make_weight(wAtCall, weight(), getMergeFn(), se);
      }

      void ETrans::combineTrans( ITrans* tp )
      {
        // TODO - change dynamic_cast to static_cast 
        //        When happy with impl.
        ETrans* e = dynamic_cast<ETrans*>(tp);
        if( e != 0) {
          // Need to update wAtCall
          wAtCall = wAtCall->combine(e->wAtCall);
        }
        getDelegate()->combineTrans(tp);
      }

    } // namespace ewpds
  } // namespace wpds
} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 2 ***
   ;;; End: ***
*/

