
#include "opennwa/Nwa.hpp"
#include "opennwa/query/automaton.hpp"

namespace opennwa {
  namespace query {

      

    bool
    isDeterministic(Nwa const & nwa)
    {
      return nwa._private_isDeterministic_();
    }



    bool
    statesOverlap(Nwa const & first, Nwa const & second)
    {
      StateSet intersection;
      // The following line does 'intersection = first->states() intersect second->states()'
      std::set_intersection(first.beginStates(), first.endStates(),
                            second.beginStates(), second.endStates(),
                            std::inserter(intersection, intersection.begin()));

      if (intersection.size() == 0) {
        // No states are shared
        return false;
      }
      else {
        return true;
      }
    }

  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
