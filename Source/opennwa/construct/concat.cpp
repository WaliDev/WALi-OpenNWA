#include "opennwa/Nwa.hpp"
#include "opennwa/construct/concat.hpp"

#include "opennwa/query/automaton.hpp"

#include "opennwa/query/calls.hpp"

namespace opennwa
{
  namespace construct
  {
      
    NwaRefPtr concat( Nwa const & first, Nwa const & second )
    {
      NwaRefPtr nwa( new Nwa());
      concat(*nwa, first, second);
      return nwa;
    }

    /**
     *
     * @brief constructs the NWA resulting from the concatenation of the given NWAs
     *
     * @param - first: the NWA to which 'second' should be concatenated
     * @param - second: the NWA to concatenate onto the end of 'first'
     *
     */
    void concat( Nwa & out, Nwa const & first, Nwa const & second )
    {
      //Q: Do we need to guarantee that there is no overlap in states between machines?
      //A: YES!
      //    If there is overlap, then we could conceivably go through the first half of one
      //    machine and then skip to the other machine before completing the first machine.
      //Q: How should we do this?  
      //    2. Generate new keys by the pairing scheme (1,k) for keys in first and (2,k) for keys in second.
      //    3. Implement a check for overlapping state keys and assert that no such overlap occurs.
      //A: 3 is easiest

      //Q: How should clientInfos be generated for the concatenated NWA?
      //A: The clientInfos from the component machines are copied and added to the concatenated NWA.

      //Test state Key overlap of the two NWAs.
      assert(! query::statesOverlap(first, second) );

      //Clear all states(except the stuck state) and transitions from this machine.
      out.clear();

      //Duplicate all of the functionality of the first machine (except the final state property).
      out = first;

      //Duplicate all of the functionality of the second machine (except the initial state property).
      out.combineWith(second);

      out.clearInitialStates();
      out.clearFinalStates();

      for (Nwa::StateIterator initial = first.beginInitialStates();
           initial != first.endInitialStates(); ++initial)
      {
        out.addInitialState(*initial);
      }
        
      // Now we have both machines combined. We need to set the final states
      // of the combined machine to be the final states of the second
      // machine, then add epsilon transitions from the final states of the
      // first to the final states of the second.

      // First step: set final states.
      for (Nwa::StateIterator final = second.beginFinalStates();
           final != second.endFinalStates(); ++final)
      {
        out.addFinalState(*final);
      }
      
      // Second step: add transitions from the first machine to the second
      for( Nwa::StateIterator fit = first.beginFinalStates();
           fit != first.endFinalStates(); fit++ )
      {
        for( Nwa::StateIterator sit = second.beginInitialStates();
             sit != second.endInitialStates(); sit++ )
        {
          out.addInternalTrans(*fit, EPSILON, *sit);
        }
      }

      // Third step: add additional return transitions. Each return
      // transition of the form (_, q0, _, _) in the second half can be
      // taken by a pending return -- but now that pending return can match
      // something in the first half. So we need to add a new transition
      // (_, q', _, _) for each q' in the first NWA that appears as the
      // call site in a call transition.
      StateSet call_sites = query::getCallSites(first);
        
      for( Nwa::ReturnIterator rit = second.beginReturnTrans();
           rit != second.endReturnTrans(); ++rit)
      {
        Nwa::Return rtrans = *rit;
          
        if (second.isInitialState(rtrans.second)) {
          for( StateSet::const_iterator new_pred = call_sites.begin();
               new_pred != call_sites.end(); ++new_pred)
          {
            rtrans.second = *new_pred;
            out.addReturnTrans(rtrans);
          }
        }
      }
    }

      
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

