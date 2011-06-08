#include "wali/nwa/NWA.hpp"

namespace wali
{
  namespace nwa
  {
    namespace construct
    {


    /**
     * 
     * @brief constructs the NWA that is the complement of the given NWA
     *
     * @param - first: the NWA to perform the complement of
     *
     */
    void NWA::complement( NWA const & first )
    {
      //Q: How should clientInfos be generated for the complemented NWA?
      //A: The clientInfos from the component machines are copied and added to the complemented NWA.

      //Clear all states(except the stuck state) and transitions from this machine.
      clear();


      //Start with a deterministic copy of the given NWA.
      // FIXME: keep information about whether a machine is deterministic
      if(false) //! first.isDeterministic() )
      {
        determinize(first);   //Note: determinize() will take care of clientInfo information.
      }
      else
      {
        states.addAll(first.states); //Note: This includes copying clientInfo information over.          
        addAllSymbols(first.symbols);
        trans.addAllTrans(first.trans);
      }
      
      //Q: If the stuck state becomes accepting do we need to make explicit all
      //    transitions to the stuck state?  
      //A: Yes, we need to make explicit all transitions to the stuck state. 
      //Realize all implicit transitions so that it makes sense for the stuck state to become 
      //a final state.
      //Q: Should this be done in addFinalState() instead?  What does this mean for setStuckState()? 
      //A: it should be done in addFinalState
      //A: setStuckState should not allow setting as a stuck state any state that is final
      //Q: Technically this only needs to be done if the stuck state becomes a final state.  Is this
      //    always the case, or is there some possible situation in which there is a stuck state that
      //    is currently a final state and thus will not be a final state in the complement machine?
      //A: this is always the case since the stuck state can never be a final state
      
      //FinalStates = AllStates - FinalStates
      StateSet oldFinalStates;
      oldFinalStates.insert(beginFinalStates(),endFinalStates());
 
      clearFinalStates();
      for( stateIterator sit = beginStates(); sit != endStates(); sit++ )
      {
        if( oldFinalStates.count(*sit) == 0 )
          addFinalState(*sit);
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

