#include "wali/nwa/NWA.hpp"

namespace wali
{
  namespace nwa
  {
    namespace construct
    {
      
    /**
     *
     * @brief constructs the NWA resulting from the union of the given NWAs 
     *
     * @param - first: the NWA to union with 'second'
     * @param - second: the NWA to union with 'first'
     *
     */
    void NWA::unionNWA( NWA const & first, NWA const & second )
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
      assert(! overlap(first,second) );

      //Clear all states(except the stuck state) and transitions from this machine.
      clear();

      //Copy all of the functionality of the two machines.  
      //States (Initial and final state information included.)
      states.addAll(first.states);   //Note: This includes copying clientInfo information over.  
      states.addAll(second.states);  //Note: This includes copying clientInfo information over.

      //Transitions (Call, internal, and return.)
      trans.addAllTrans(first.trans);
      trans.addAllTrans(second.trans);      
    }

    }
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

