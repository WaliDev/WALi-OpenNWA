#ifndef wali_FUNCTIONAL_WEIGHT_GUARD
#define wali_FUNCTIONAL_WEIGHT_GUARD 1

/*!
 * @author Akash Lal
 */

#include "wali/SemElem.hpp"

namespace wali {
  namespace wfa {
    namespace epr {

      /*! 
       * @class FunctionalWeight
       * @brief Functional weights for representing error-projection automaton
       *
       * This class provides functional weights that are used in the
       * automaton for representing the error projection operation.
       * This class is not a semiring, and cannot be used freely as one (even
       * though the type system allows this). In particular, there is no
       * combine operation.
       */

      class FunctionalWeight : public SemElem {
      public:
        //FunctionalWeight();
        FunctionalWeight(sem_elem_t l, sem_elem_t r);
        virtual ~FunctionalWeight();

        //////////////////////
        // Semiring operations
        //////////////////////

        sem_elem_t one() const;
        sem_elem_t zero() const;
        sem_elem_t extend(SemElem *se);
        sem_elem_t combine(SemElem *se);
        bool equal(SemElem *se) const;

        std::ostream &print(std::ostream &o) const;

        //////////////////////
        // Apply functional
        //////////////////////

        // return left \extend se \extend right
        sem_elem_t apply(sem_elem_t se);

      private:
        // normalization operation
        void normalize();

        // The functional weight is \lambda x. (left \extend x \extend right)
        sem_elem_t left, right;
      };
    } // namespace epr
  } // namespace wfa
} // namespace wali

#endif // wali_FUNCTIONAL_WEIGHT_GUARD
