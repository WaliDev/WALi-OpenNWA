#ifndef wali_nwa_NWAFWD_GUARD
#define wali_nwa_NWAFWD_GUARD 1

/// This file defines forward declarations for various useful NWA-related
/// types.

#include <set>

#include "wali/nwa/deprecate.h"

// ::wali
#include "wali/Key.hpp"
#include "wali/ref_ptr.hpp"

namespace wali
{
  namespace nwa
  {
    typedef Key State;
    typedef Key Symbol;

    class NWA;
    typedef ref_ptr<NWA> NWARefPtr;

    struct Configuration;
    class NestedWord;

    typedef std::set<State> StateSet;
    typedef std::set<Symbol> SymbolSet;
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
