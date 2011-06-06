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
    /// Globally (within a process) unique ID of a state
    ///
    typedef Key State;
    /// Globally (within a process) unique ID of a symbol
    ///
    typedef Key Symbol;


    // Forward decl
    class NWA;

    /// A reference-counted smart pointer to an NWA
    ///
    typedef ref_ptr<NWA> NWARefPtr;
    
    struct Configuration;

    // Forward decl
    class NestedWord;

    /// A set of States.
    ///
    /// (Do not depend on the details of std::set; the type could
    /// change to an unordered_set or other type with a similar
    /// interface in future versions.)
    typedef std::set<State> StateSet;
    /// A set of Symbols
    ///
    /// (Do not depend on the details of std::set; the type could
    /// change to an unordered_set or other type with a similar
    /// interface in future versions.)
    typedef std::set<Symbol> SymbolSet;
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
