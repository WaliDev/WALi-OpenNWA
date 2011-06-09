#include "wali/nwa/NWA.hpp"
#include "wali/nwa/details/TransitionStorage.hpp"

using wali::nwa::details::TransitionStorage;

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
        for (NWA::StateIterator state = first.beginStates();
             state != first.endStates(); ++state)
        {
          out.addState(*state);
        }
          

        //Swap initial and final state functionality.
        for( NWA::StateIterator it = first.beginInitialStates(); 
             it != first.endInitialStates(); it++ )
        {
          out.addFinalState(*it);
        }
        for( NWA::StateIterator it = first.beginFinalStates(); 
             it != first.endFinalStates(); it++ )
        {
          out.addInitialState(*it);
        }

      
        //Duplicate internal transitions with source/target swapped.
        for( NWA::InternalIterator it = first.beginInternalTrans(); 
             it != first.endInternalTrans(); it++ )
        {
          out.addInternalTrans(TransitionStorage::getTarget(*it),
                               TransitionStorage::getInternalSym(*it),
                               TransitionStorage::getSource(*it));
        }

        //Duplicate return transitions as call transitions with (return,sym,exit).
        for( NWA::ReturnIterator it = first.beginReturnTrans(); 
             it != first.endReturnTrans(); it++ )
        {
          out.addCallTrans(TransitionStorage::getReturnSite(*it),
                           TransitionStorage::getReturnSym(*it),
                           TransitionStorage::getExit(*it));
        }

        //Duplicate call transitions with associated return transitions as 
        //return transitions with (entry,return,sym,call).
        for( NWA::CallIterator cit = first.beginCallTrans(); 
             cit != first.endCallTrans(); cit++ )
        {
          for( NWA::ReturnIterator rit = first.beginReturnTrans(); 
               rit != first.endReturnTrans(); rit++ )
          {
            if( TransitionStorage::getCallSite(*cit) == TransitionStorage::getCallSite(*rit) )
            {
              out.addReturnTrans(TransitionStorage::getEntry(*cit),
                                 TransitionStorage::getReturnSite(*rit),
                                 TransitionStorage::getCallSym(*cit),
                                 TransitionStorage::getCallSite(*cit));
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

