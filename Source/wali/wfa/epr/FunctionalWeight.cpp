#include "wali/wfa/epr/FunctionalWeight.hpp"

namespace wali {
  namespace wfa {
    namespace epr {

      FunctionalWeight::FunctionalWeight(sem_elem_t l,
                                         sem_elem_t r) : SemElem(),
                                                         left(l),
                                                         right(r) { 
        normalize();
      }

      FunctionalWeight::~FunctionalWeight() { }

      // if either weight is zero, make both zero
      void FunctionalWeight::normalize() {
        assert(left.is_valid());
        assert(right.is_valid());

        if(left->equal(left->zero())) {
          right = left->zero();
        } else if(right->equal(right->zero())) {
          left = right->zero();
        }
      }


      sem_elem_t FunctionalWeight::one() const {
        return new FunctionalWeight(left->one(), right->one());
      }

      sem_elem_t FunctionalWeight::zero() const {
        return new FunctionalWeight(left->zero(), right->zero());
      }

      // Extend is compose^{reversed}
      sem_elem_t FunctionalWeight::extend(SemElem *se) {
        FunctionalWeight *fse = dynamic_cast< FunctionalWeight * > (se);
        if(fse == NULL) {
          *waliErr << "Error: non-functional weight given to FunctionalWeight::extend\n";
          assert(0);
        }

        sem_elem_t l = fse->left->extend(left);
        sem_elem_t r = right->extend(fse->right);
        return new FunctionalWeight(l, r);
      }

      // combine should never be called
      sem_elem_t FunctionalWeight::combine(SemElem *se ATTR_UNUSED) {
        *waliErr << "Error: Combine called on FunctionalWeight\n";
        assert(0);
        return NULL;
      }

      bool FunctionalWeight::equal(SemElem *se) const {
        FunctionalWeight *fse = dynamic_cast< FunctionalWeight * > (se);
        if(fse == NULL) {
          *waliErr << "Error: non-functional weight given to FunctionalWeight::extend\n";
          assert(0);
        }

        return left->equal(fse->left) && right->equal(fse->right);
      }

      sem_elem_t FunctionalWeight::apply(sem_elem_t se) {
        return (left->extend(se))->extend(right);
      }

      std::ostream &FunctionalWeight::print(std::ostream &o) const {
        o << "FunctionalWeight[";
        left->print(o);
        o << " and ";
        right->print(o);
        o << "]\n";
        return o;
      }

    } // namespace epr
  } // namespace epr
} // namespace wali
