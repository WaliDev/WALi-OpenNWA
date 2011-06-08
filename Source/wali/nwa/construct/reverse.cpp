#include "wali/nwa/NWA.hpp"

namespace wali
{
  namespace nwa
  {
    namespace construct
    {
      
    /**
     * @brief constructs the NWA which is the reverse of the given NWA
     *
     * @param - first: the NWA to reverse
     * 
     */
    void NWA::reverse( NWA const & first )
    {
      //Note: (implicit) transitions to the stuck state in the original machine 
      //       cause problems in the reverse machine if the stuck state is a final state 
      //Q: how do I implement this?
      //A: realize all transitions of first before computing the reverse if stuck is final
      //Note: the stuck state can now never be final, so we do not need to worry about this any more

      //Q: How should clientInfos be generated for the reversed NWA?
      //A: The clientInfos from the component machines are copied and added to the reversed NWA.

      //Clear all states(except the stuck state) and transitions from this machine.
      clear();

      //The reverse machine has all the states of the original machine.
      states.addAllStates(first.states); //Note: This includes copying clientInfo information over. 

      //Swap initial and final state functionality.
      //Note: we know that this does not cause the stuck state to become a final state
      //      because the stuck state was not an initial state of the original machine 
      //      (as it was not even a state in the original machine).
      for( stateIterator it = first.beginInitialStates(); 
           it != first.endInitialStates(); it++ )
      {
        addFinalState(*it);
      }
      for( stateIterator it = first.beginFinalStates(); 
           it != first.endFinalStates(); it++ )
      {
        addInitialState(*it);
      }

      //Duplicate internal transitions with source/target swapped.
      for( InternalIterator it = first.beginInternalTrans(); 
           it != first.endInternalTrans(); it++ )
      {
        addInternalTrans(Trans::getTarget(*it),
                         Trans::getInternalSym(*it),
                         Trans::getSource(*it));
      }

      //Duplicate return transitions as call transitions with (return,sym,exit).
      for( ReturnIterator it = first.beginReturnTrans(); 
           it != first.endReturnTrans(); it++ )
      {
        addCallTrans(Trans::getReturnSite(*it),
                     Trans::getReturnSym(*it),
                     Trans::getExit(*it));
      }

      //Duplicate call transitions with associated return transitions as 
      //return transitions with (entry,return,sym,call).
      for( CallIterator cit = first.beginCallTrans(); 
           cit != first.endCallTrans(); cit++ )
      {
        for( ReturnIterator rit = first.beginReturnTrans(); 
             rit != first.endReturnTrans(); rit++ )
        {
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            addReturnTrans(Trans::getEntry(*cit),
                           Trans::getReturnSite(*rit),
                           Trans::getCallSym(*cit),
                           Trans::getCallSite(*cit));
          }
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

