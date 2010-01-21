#ifndef wali_util_WEIGHT_CHANGER_GUARD
#define wali_util_WEIGHT_CHANGER_GUARD 1

/**
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"

namespace wali
{

  namespace util
  {
    /**
     * @class WeightChanger
     *
     * Encapsulates a unary function for changing weights
     *
     * @see wali::sem_elem_t
     */
    class WeightChanger
    {
      public:
        virtual ~WeightChanger() {}
        virtual sem_elem_t change_weight( sem_elem_t wt ) = 0;

    }; // WeightChanger

  } // namespace util

} // namespace wali

#endif  // wali_util_WEIGHT_CHANGER_GUARD

