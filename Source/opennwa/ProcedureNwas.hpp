#ifndef PROCEDURE_NWAS_HPP
#define PROCEDURE_NWAS_HPP

#include "Nwa.hpp"

#include <map>
#include <string>

#include <boost/function.hpp>

namespace opennwa {
  ///
  /// Maps name of procedure to the procedure NWA
  typedef std::map<std::string, NwaRefPtr> ProcedureMap;

    
  struct CallReturnTransitionInserter {
    const Symbol call_symbol;
    const Symbol return_symbol;

    CallReturnTransitionInserter(Symbol _call_symbol,
                                 Symbol _return_symbol)
      : call_symbol(_call_symbol)
      , return_symbol(_return_symbol)
    {}

    void operator() (Nwa & nwa, State source, State target) {
      nwa.addCallTrans(source, call_symbol, target);
    }

    void operator() (Nwa & nwa, State source, State pred, State target) {
      nwa.addReturnTrans(source, pred, return_symbol, target);
    }
  };
    

  struct EpsilonTransitionInserter {
    void operator() (Nwa & nwa, State source, State target) {
      nwa.addInternalTrans(source, EPSILON, target);
    }

    void operator() (Nwa & nwa, State source, State pred, State target) {
      (void) pred;
      nwa.addInternalTrans(source, EPSILON, target);
    }
  };

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
  NwaRefPtr
  assemble_nwa(ProcedureMap const & procedures,
               boost::function<void (Nwa &, State, State)> call_inserter,
               boost::function<void (Nwa &, State, State, State)> return_inserter);


}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
