#ifndef PROCEDURE_NWAS_HPP
#define PROCEDURE_NWAS_HPP

#include "NWA.hpp"

#include <map>
#include <string>

namespace wali {
  namespace nwa {
    /// Maps name of procedure to the procedure NWA
    typedef std::map<std::string, NWARefPtr> ProcedureMap;

    /// Assembles all NWAs in 'procedures' into one large
    /// NWA. Replaces any transition labeled with a symbol
    /// '__call__<procname>' with a call transition from the source
    /// state to the initial state of the NWA with procedure name
    /// 'procname' and a return transition from the final state of the
    /// NWA with name 'procname' to the original transition's target
    /// state (with the original transition's source state as a
    /// predecessor).
    ///
    /// The initial state of the NWA named 'main' is the full NWA's
    /// start state, and the accepting state of 'main' is the full
    /// NWA's accepting state.
    NWARefPtr
    assemble_nwa(ProcedureMap const & procedures, Key call_key, Key return_key);

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
