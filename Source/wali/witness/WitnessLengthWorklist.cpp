/*!
 * @author Rich Joiner
 */

#include "wali/Common.hpp"
#include "wali/witness/WitnessLengthWorklist.hpp"
#include "wali/wfa/Trans.hpp"

namespace wali
{
  namespace witness
  {
    WitnessLengthWorklist::WitnessLengthWorklist() : PriorityWorklist<ShorterThan>()
    {
    }

    WitnessLengthWorklist::~WitnessLengthWorklist()
    {
    }
  }
}

