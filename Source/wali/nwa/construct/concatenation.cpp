#include "wali/nwa/NWA.hpp"

namespace wali
{
  namespace nwa
  {
    namespace construct
    {
      

    /**
     *
     * @brief constructs the NWA resulting from the concatenation of the given NWAs
     *
     * @param - first: the NWA to which 'second' should be concatenated
     * @param - second: the NWA to concatenate onto the end of 'first'
     *
     */
    void NWA::concat( NWA const & first, NWA const & second )
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
      assert(! overlap(first, second) );

      //Clear all states(except the stuck state) and transitions from this machine.
      clear();

      //Duplicate all of the functionality of the first machine (except the final state property).
      states.addAllStates(first.states);   //Note: This includes copying clientInfo information over.
      states.addAllInitialStates(first.states);
      trans.addAllTrans(first.trans);

      //Duplicate all of the functionality of the second machine (except the initial state property).
      states.addAllStates(second.states);  //Note: This includes copying clientInfo information over.
      states.addAllFinalStates(second.states);
      trans.addAllTrans(second.trans);

      //Add epsilon transitions from the final states of the first machine to the initial
      //states of the second machine.
      for( stateIterator fit = first.beginFinalStates(); fit != first.endFinalStates(); fit++ )
      {
        for( stateIterator sit = second.beginInitialStates(); sit != second.endInitialStates(); sit++ )
        {
          addInternalTrans(*fit,WALI_EPSILON,*sit);
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

