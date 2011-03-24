#include "wali/nwa/deprecate.h"
#include "wali/nwa/details/StateSet.hpp"

namespace wali
{
  namespace nwa
  {
    DEPRECATE("Try to avoid using this class. Talk to Evan for workarounds. Use wali::nwa::details::StateSet if you must.")
    typedef details::StateSet StateSet;
  }
}
