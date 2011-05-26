#ifndef NWA_PARSER_HPP
#define NWA_PARSER_HPP

#include <iosfwd>
#include <string>
#include "NWA.hpp"

namespace wali {
  namespace nwa {
    extern NWARefPtr read_nwa(std::istream & is, std::string * name = NULL);
    extern void test_all();

    typedef std::map<std::string, NWARefPtr> ProcedureMap;
    extern ProcedureMap read_nwa_proc_set(std::istream & is);
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
