#ifndef NWA_PARSER_HPP
#define NWA_PARSER_HPP

#include <iosfwd>
#include <string>
#include "Nwa.hpp"

namespace opennwa {

  /// Reads a single NWA from 'is'. If 'name' is nonnull and the
  /// stream specifies a name, store the NWA's name at the location
  /// pointed to by 'name'.
  extern NwaRefPtr read_nwa(std::istream & is, std::string * name = NULL);

  /// A collection of named NWAs.
  ///
  typedef std::map<std::string, NwaRefPtr> ProcedureMap;

  /// Reads a stream full of NWA descriptions, returning the set of
  /// named NWAs. (The NWAs in the stream better all have unique
  /// names, or only the last one of each name will be returned. No
  /// attempt is made to verify this fact.)
  extern ProcedureMap read_nwa_proc_set(std::istream & is);

  /// Run NWA Parser unit tests
  ///
  extern void parser_test_all();

}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
