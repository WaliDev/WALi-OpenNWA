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
    typedef Key St;
    typedef Key Sym;

    class NWA;
    typedef ref_ptr<NWA> NWARefPtr;
  }
}
  
#endif
