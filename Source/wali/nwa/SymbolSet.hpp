#include "wali/nwa/deprecate.h"
#include "wali/nwa/details/SymbolStorage.hpp"

namespace wali
{
  namespace nwa
  {
    DEPRECATE("Try to avoid using this class. Talk to Evan for workarounds. Use wali::nwa::details::SymbolStorage if you must.")
    typedef details::SymbolSet SymbolSet;
  }
}
