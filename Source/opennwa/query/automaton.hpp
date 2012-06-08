#ifndef WALI_NWA_QUERY_automaton_HPP
#define WALI_NWA_QUERY_automaton_HPP

#include "opennwa/NwaFwd.hpp"


namespace opennwa {
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
    isDeterministic(Nwa const & nwa);


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
    statesOverlap(Nwa const & first, Nwa const & second);

    /// Returns the number of states of 'nwa' that appear in the call
    /// position of a call transition.
    size_t numCallSites(Nwa const & nwa);

    /// Returns the number of states of 'nwa' that appear in the entry
    /// position of a call transition.
    size_t numEntrySites(Nwa const & nwa);    

    /// Returns the number of states of 'nwa' that appear in the exit
    /// position of a return transition.
    size_t numExitSites(Nwa const & nwa);

    /// Returns the number of states of 'nwa' that appear in the return
    /// position of a return transition.
    size_t numReturnSites(Nwa const & nwa);

  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

