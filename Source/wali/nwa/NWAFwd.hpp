#ifndef wali_nwa_NWAFWD_GUARD
#define wali_nwa_NWAFWD_GUARD 1

/// This file defines forward declarations for various useful NWA-related
/// types.

#include "wali/nwa/deprecate.h"

// ::wali
#include "wali/Key.hpp"
#include "wali/ref_ptr.hpp"

namespace wali
{
  namespace nwa
  {
    DEPRECATE("Use wali::nwa::State instead")
    typedef Key St;

    DEPRECATE("Use wail::nwa::Symbol instead")
    typedef Key Sym;

    typedef Key State;
    typedef Key Symbol;

    class NWA;
    typedef ref_ptr<NWA> NWARefPtr;
  }
}
  
#endif
