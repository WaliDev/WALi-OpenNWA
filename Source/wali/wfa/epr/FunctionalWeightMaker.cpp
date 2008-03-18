/*!
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/SemElemPair.hpp"
#include "wali/wfa/epr/FunctionalWeightMaker.hpp"
#include "wali/wfa/epr/FunctionalWeight.hpp"

namespace wali {
  namespace wfa {
    namespace epr {

      sem_elem_t FunctionalWeightMaker::make_weight( sem_elem_t lhsBeforeCall, 
                                                     sem_elem_t lhsAfterCall ATTR_UNUSED,
                                                     wali::wpds::ewpds::merge_fn_t lhsmf,
                                                     sem_elem_t rhs) {
        
        return new FunctionalWeight(lhsBeforeCall, lhsmf, rhs);
      }
      
      sem_elem_t FunctionalWeightMaker::make_weight( sem_elem_t lhs, sem_elem_t rhs ) {
        
        return new FunctionalWeight(lhs, rhs);  
      }

    }// namespace epr
  } // namespace wfa
} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 2 ***
   ;;; End: ***
*/
