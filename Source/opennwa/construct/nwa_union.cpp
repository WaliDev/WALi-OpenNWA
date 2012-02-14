#include "opennwa/Nwa.hpp"
#include "opennwa/construct/union.hpp"

#include "opennwa/query/automaton.hpp"

namespace opennwa
{
  namespace construct
  {

    NwaRefPtr unionNwa( Nwa const & first, Nwa const & second )
    {
      NwaRefPtr nwa( new Nwa());
      unionNwa(*nwa, first, second);
      return nwa;
    }

      
    /**
     *
     * @brief constructs the NWA resulting from the union of the given NWAs 
     *
     * @param - first: the NWA to union with 'second'
     * @param - second: the NWA to union with 'first'
     *
     */
    void unionNwa( Nwa & out, Nwa const & first, Nwa const & second )
    {
      //Q: Do we need to guarantee that there is no overlap in states between machines? 
      //A: YES
      //    If there is overlap, then we could conceivably go through the first half of one
      //    machine and then skip to the other machine, thereby not completing either machine.
      //Q: How should we do this?  
      //    1. Use the same base algorithm as intersection.  
      //    2. Generate new keys by the pairing scheme (1,k) for keys in first and (2,k) for keys in second.
      //    3. Implement a check for overlapping state keys and assert that no such overlap occurs.
      //A: 3 is easiest

      //Q: How should clientInfos be generated for the union NWA?
      //A: The clientInfos from the component machines are copied and added to the union NWA.

      //Test state Key overlap of the two NWAs.
      assert(! query::statesOverlap(first,second) );

      //Clear all states(except the stuck state) and transitions from this machine.
      out.clear();

      //Copy all of the functionality of the two machines.  
      out = first;
      out.combineWith(second);

      // Now we have something with the combined states, symbols, and
      // transitions from the two machines. It has the initial and final
      // states of the 'first' machine, but lacks the initial and final
      // states of the 'second' machine. So we need to add those.
      for (Nwa::StateIterator initial = second.beginInitialStates();
           initial != second.endInitialStates(); ++initial)
      {
        out.addInitialState(*initial);
      }

      for (Nwa::StateIterator final = second.beginFinalStates();
           final != second.endFinalStates(); ++final)
      {
        out.addFinalState(*final);
      }
    }

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

