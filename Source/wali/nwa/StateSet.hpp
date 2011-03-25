#include "wali/nwa/deprecate.h"
#include "wali/nwa/details/StateStorage.hpp"

namespace wali
{
  namespace nwa
  {
    DEPRECATE("Try to avoid using this class. Talk to Evan for workarounds. Use wali::nwa::details::StateStorage if you must.")
    typedef details::StateStorage StateSet;
  }
}
