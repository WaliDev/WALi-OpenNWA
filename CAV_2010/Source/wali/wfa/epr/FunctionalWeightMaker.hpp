#ifndef wali_wfa_FUNCTIONAL_WEIGHT_MAKER_GUARD
#define wali_wfa_FUNCTIONAL_WEIGHT_MAKER_GUARD 1

/*!
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include "wali/wfa/WeightMaker.hpp"

namespace wali {
  namespace wfa {
    namespace epr {

      /*!
       * @class FunctionalWeightMaker
       *
       * This class defines the interface for joining the weights
       * on WFA transitions to form Functional weights (to create an EPA)
       *
       * @see wali::wfa::epr::FunctionalWeight
       * @see wali::wfa::epr::EPA
       */

      class FunctionalWeightMaker : public WeightMaker {
      public:
        FunctionalWeightMaker() {}
        ~FunctionalWeightMaker() {}

        sem_elem_t make_weight( sem_elem_t lhs, sem_elem_t rhs );

        sem_elem_t make_weight( ITrans *lhs, ITrans *rhs);

      }; // FunctionalWeightMaker

    } // namespace epr
  } // namespace wfa
} // namespace wali

#endif // wali_FUNCTIONAL_WEIGHT_MAKER_GUARD
