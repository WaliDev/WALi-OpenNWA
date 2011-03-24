#include "wali/nwa/deprecate.h"
#include "wali/nwa/details/TransInfo.hpp"

namespace wali
{
  namespace nwa
  {
    DEPRECATE("Try to avoid using this class. Talk to Evan for workarounds. Use wali::nwa::details::TransInfo if you must.")
    typedef details::TransInfo TransInfo;
  }
}
