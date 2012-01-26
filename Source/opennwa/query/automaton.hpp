#ifndef WALI_NWA_QUERY_automaton_HPP
#define WALI_NWA_QUERY_automaton_HPP

#include "opennwa/NWAFwd.hpp"


namespace wali {
  namespace nwa {
    namespace query {

      
      /**
       *
       * @brief tests whether this NWA is deterministic 
       *
       * This method tests whether this NWA is deterministic or nondeterministic. If this 
       * NWA is deterministic, true is returned.  Otherwise, false is returned.
       *
       * @return true if this NWA is deterministic, false otherwise
       *
       */
      bool
      isDeterministic(NWA const & nwa);


      /**
       * 
       * @brief determines whether there is any overlap in the states of the given NWAs
       *
       * This method tests whether there is any overlap in the states of the given NWAs.
       *
       *   first->states()  intersect  second->states()  = {]
       *
       * @param - first: one of the NWAs whose states to compare
       * @param - second: one of the NWAs whose states to compare
       * @return true if there is some overlap in the states of the given NWAs, false otherwise
       *
       */
      bool
      statesOverlap(NWA const & first, NWA const & second);

    }
  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

