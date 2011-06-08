#include "wali/nwa/NWA.hpp"

namespace wali
{
  namespace nwa
  {
    namespace construct
    {

      NWARefPtr reverse( NWA const & source )
      {
        NWARefPtr nwa( new NWA());
        reverse(*nwa, source);
        return nwa;
      }

    /**
     * @brief constructs the NWA which is the reverse of the given NWA
     *
     * @param - first: the NWA to reverse
     * 
     */
    void reverse( NWA & out, NWA const & first )
    {
      //Q: How should clientInfos be generated for the reversed NWA?
      //A: The clientInfos from the component machines are copied and added to the reversed NWA.

      //Clear all states(except the stuck state) and transitions from this machine.
      out.clear();


      //The reverse machine has all the states of the original machine.
      // FIXME-RELEASE: copy clientinfo. Or figure out how to do this.
      for (NWA::stateIterator state = first.beginStates();
           state != first.endStates(); ++state)
      {
        out.addState(*state);
      }
          

      //Swap initial and final state functionality.
      for( NWA::stateIterator it = first.beginInitialStates(); 
           it != first.endInitialStates(); it++ )
      {
        out.addFinalState(*it);
      }
      for( NWA::stateIterator it = first.beginFinalStates(); 
           it != first.endFinalStates(); it++ )
      {
        out.addInitialState(*it);
      }

      
      //Duplicate internal transitions with source/target swapped.
      for( NWA::InternalIterator it = first.beginInternalTrans(); 
           it != first.endInternalTrans(); it++ )
      {
        out.addInternalTrans(NWA::Trans::getTarget(*it),
                             NWA::Trans::getInternalSym(*it),
                             NWA::Trans::getSource(*it));
      }

      //Duplicate return transitions as call transitions with (return,sym,exit).
      for( NWA::ReturnIterator it = first.beginReturnTrans(); 
           it != first.endReturnTrans(); it++ )
      {
        out.addCallTrans(NWA::Trans::getReturnSite(*it),
                         NWA::Trans::getReturnSym(*it),
                         NWA::Trans::getExit(*it));
      }

      //Duplicate call transitions with associated return transitions as 
      //return transitions with (entry,return,sym,call).
      for( NWA::CallIterator cit = first.beginCallTrans(); 
           cit != first.endCallTrans(); cit++ )
      {
        for( NWA::ReturnIterator rit = first.beginReturnTrans(); 
             rit != first.endReturnTrans(); rit++ )
        {
          if( NWA::Trans::getCallSite(*cit) == NWA::Trans::getCallSite(*rit) )
          {
            out.addReturnTrans(NWA::Trans::getEntry(*cit),
                               NWA::Trans::getReturnSite(*rit),
                               NWA::Trans::getCallSym(*cit),
                               NWA::Trans::getCallSite(*cit));
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

