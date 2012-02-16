/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/KeyOrderWorklist.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/KeyContainer.hpp"

namespace wali
{
  KeyOrderWorklist::KeyOrderWorklist() : PriorityWorklist<KeyComparer>()
  {
  }

  KeyOrderWorklist::~KeyOrderWorklist()
  {
  }
}

