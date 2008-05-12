/*!
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/SemElemPair.hpp"
#include "wali/wfa/epr/FunctionalWeightMaker.hpp"
#include "wali/wfa/epr/FunctionalWeight.hpp"
#include "wali/wfa/Trans.hpp"

namespace wali 
{
  namespace wfa 
  {
    namespace epr 
    {

      sem_elem_t FunctionalWeightMaker::make_weight( ITrans *lhs, ITrans *rhs) {

        return new FunctionalWeight(lhs, rhs);
      }

      sem_elem_t FunctionalWeightMaker::make_weight( sem_elem_t lhs, sem_elem_t rhs) {
        Trans *lt = new Trans(0, 0, 0, lhs);
        Trans *rt = new Trans(0, 0, 0, rhs);
        FunctionalWeight *fw = new FunctionalWeight(lt, rt);
        delete lt;
        delete rt;
        return fw;
      }

    }// namespace epr

  } // namespace wfa

} // namespace wali


