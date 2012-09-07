/*!
 * @author Nicholas Kidd
 * $Id: ETrans.cpp 1099 2011-06-07 22:22:04Z driscoll $
 */

#include "wali/Common.hpp"

#include "wali/wfa/Trans.hpp"

#include "wali/wpds/ewpds/ETrans.hpp"

namespace wali {
  namespace wpds {
    namespace ewpds {

      ETrans::ETrans(
          Key f, Key s, Key t,
          sem_elem_t weightAtCall,
          sem_elem_t wAfterCall,
          erule_t er)
        : DecoratorTrans(new wfa::Trans(f,s,t,wAfterCall)),wAtCall(weightAtCall),erule(er)
      {
      }

      ETrans::ETrans(
          ITrans* d,
          sem_elem_t weightAtCall,
          erule_t er)
        : DecoratorTrans(d),wAtCall(weightAtCall),erule(er)
      {
      }

      ETrans::~ETrans()
      {
      }

      std::ostream& ETrans::print( std::ostream& o ) const {
        o << "[ETRANS] ";
        return getDelegate()->print(o);
      }



      wfa::ITrans* ETrans::copy() const {
        return new ETrans(getDelegate()->copy(),wAtCall,erule);
      }

      wfa::ITrans* ETrans::copy(Key f, Key s, Key t) const {
        return new ETrans(getDelegate()->copy(f,s,t),wAtCall,erule);
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

      TaggedWeight ETrans::apply_post( TaggedWeight tw) const {
        if(tw.isRet()) {
          sem_elem_t wt = getMergeFn()->apply_f(wAtCall, tw.getWeight());
          return TaggedWeight(wt, walienum::NONE);
        }
        return TaggedWeight(weight()->extend(tw.getWeight()), walienum::CALL);
      }

      TaggedWeight ETrans::apply_pre( TaggedWeight tw) const {
        assert(!tw.isCall());
        return TaggedWeight(tw.getWeight()->extend(weight()), walienum::RETURN);
      }

      void ETrans::applyWeightChanger( util::WeightChanger &wc) {
        setWeight(wc.change_weight(weight()));
        setWeightAtCall(wc.change_weight(getWeightAtCall()));
      }

      void ETrans::combineTrans( ITrans* tp )
      {
        // TODO - change dynamic_cast to static_cast 
        //        When happy with impl.
        ETrans* e = dynamic_cast<ETrans*>(tp);
        if( e != 0 && wAtCall.is_valid()) {
          // Need to update wAtCall
          wAtCall = wAtCall->combine(e->wAtCall);
        }
        getDelegate()->combineTrans(tp);
      }

    } // namespace ewpds

  } // namespace wpds

} // namespace wali

