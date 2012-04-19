/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/TotalOrderWorklist.hpp"
#include "wali/wfa/Trans.hpp"

namespace wali
{
  TotalOrderWorklist::TotalOrderWorklist() : PriorityWorklist<LessThan>()
  {
  }

  TotalOrderWorklist::~TotalOrderWorklist()
  {
  }
}

