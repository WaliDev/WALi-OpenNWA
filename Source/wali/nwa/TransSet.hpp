#include "wali/nwa/deprecate.h"
#include "wali/nwa/details/TransSet.hpp"

namespace wali
{
  namespace nwa
  {
    DEPRECATE("Try to avoid using this class. Talk to Evan for workarounds. Use wali::nwa::details::TransSet if you must.")
    typedef details::TransSet TransSet;
  }
}
