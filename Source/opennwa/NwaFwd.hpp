#ifndef wali_nwa_NWAFWD_GUARD
#define wali_nwa_NWAFWD_GUARD 1

/// This file defines forward declarations for various useful NWA-related
/// types.

#include <set>

#include "opennwa/deprecate.h"

// ::wali
#include "wali/Key.hpp"
#include "wali/Common.hpp"
#include "wali/ref_ptr.hpp"

namespace opennwa
{
  using wali::ref_ptr;
  
  /// Globally (within a process) unique ID of a state
  ///
  typedef wali::Key State;
  /// Globally (within a process) unique ID of a symbol
  ///
  typedef wali::Key Symbol;

  const Symbol EPSILON = wali::WALI_EPSILON;
  const Symbol WILD = wali::WALI_WILD;

  using wali::getKey;


  // Forward decl
  class Nwa;

  /// A reference-counted smart pointer to an NWA
  ///
  typedef ref_ptr<Nwa> NwaRefPtr;

  // Forward decl
  class NestedWord;
  typedef ref_ptr<NestedWord> NestedWordRefPtr;

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

  // Forward decl
  class ClientInfo;

  typedef ref_ptr<ClientInfo> ClientInfoRefPtr;
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
