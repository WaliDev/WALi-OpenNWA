#include "wali/wfa/epr/FunctionalWeight.hpp"

namespace wali {
  namespace wfa {
    namespace epr {

      FunctionalWeight::FunctionalWeight(ITrans *l, ITrans *r) : SemElem() {
        ltrans = l->copy();
        rtrans = r->copy();
        normalize();

      }

      FunctionalWeight::~FunctionalWeight() { 
        delete ltrans;
        delete rtrans;
      }

      // if either weight is zero, make both zero
      void FunctionalWeight::normalize() {
        assert(ltrans != 0);
        assert(rtrans != 0);

      }


      sem_elem_t FunctionalWeight::one() const {
        // TODO: Find a better way to do this
        return new FunctionalWeight(ltrans, rtrans);
      }

      sem_elem_t FunctionalWeight::zero() const {
        // TODO: Find a better way to do this
        return new FunctionalWeight(ltrans, rtrans);
      }

      // Extend is compose^{reversed}
      sem_elem_t FunctionalWeight::extend(SemElem *se ATTR_UNUSED) {
        (void) se;
        *waliErr << "Error: Extend called on FunctionalWeight\n";
        assert(0);
        return 0;
      }

      // combine should never be called
      sem_elem_t FunctionalWeight::combine(SemElem *se ATTR_UNUSED) {
        (void) se;
        *waliErr << "Error: Combine called on FunctionalWeight\n";
        assert(0);
        return NULL;
      }

      // This is called sometimes to check equality with zero
      bool FunctionalWeight::equal(SemElem *se ATTR_UNUSED) const {
        //*waliErr << "Error: Equal called on FunctionalWeight\n";
        //assert(0);
        (void) se;
        return false;
      }

      TaggedWeight FunctionalWeight::apply(TaggedWeight tw) {
        TaggedWeight tw1 = ltrans->apply_post(tw);
        return rtrans->apply_pre(tw1);
      }

      std::ostream &FunctionalWeight::print(std::ostream &o) const {
        o << "FunctionalWeight[";
        ltrans->print(o);
        o << " and ";
        rtrans->print(o);
        o << "]\n";
        return o;
      }

    } // namespace epr
  } // namespace epr
} // namespace wali
