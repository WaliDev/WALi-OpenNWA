#ifndef wali_wfa_epr_FUNCTIONAL_WEIGHT_GUARD
#define wali_wfa_epr_FUNCTIONAL_WEIGHT_GUARD 1

/*!
 * @author Akash Lal
 */

#include "wali/wfa/ITrans.hpp"
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
       * though the type system allows this). There are no
       * combine and extend operation.
       */

      class FunctionalWeight : public SemElem {
      public:
        //FunctionalWeight();
        FunctionalWeight(ITrans *l, ITrans *r);
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
        TaggedWeight apply(TaggedWeight tw);

      private:
        // normalization operation
        void normalize();

        // The functional weight is \lambda x. (left \extend x \extend right)
        ITrans *ltrans, *rtrans;
      };
    } // namespace epr
  } // namespace wfa
} // namespace wali

#endif // wali_wfa_epr_FUNCTIONAL_WEIGHT_GUARD
