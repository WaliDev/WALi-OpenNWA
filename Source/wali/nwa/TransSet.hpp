#include "wali/nwa/deprecate.h"
#include "wali/nwa/details/TransitionStorage.hpp"

namespace wali
{
  namespace nwa
  {
    DEPRECATE("Try to avoid using this class. Talk to Evan for workarounds. Use wali::nwa::details::TransitionStorage if you must.")
    typedef details::TransitionStorage TransSet;
  }
}
