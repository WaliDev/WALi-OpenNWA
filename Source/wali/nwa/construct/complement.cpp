#include "wali/nwa/NWA.hpp"
#include "wali/nwa/construct/determinize.hpp"

namespace wali
{
  namespace nwa
  {
    namespace construct
    {

      NWARefPtr complement( NWA const & source )
      {
        NWARefPtr nwa( new NWA());
        complement(*nwa, source);
        return nwa;
      }
      
      /**
       * 
       * @brief constructs the NWA that is the complement of the given NWA
       *
       * @param - first: the NWA to perform the complement of
       *
       */
      void complement( NWA & out, NWA const & first )
      {
        //Q: How should clientInfos be generated for the complemented NWA?
        //A: The clientInfos from the component machines are copied and added to the complemented NWA.

        //Clear all states(except the stuck state) and transitions from this machine.
        out.clear();


        //Start with a deterministic copy of the given NWA.
        // FIXME: keep information about whether a machine is deterministic
        if(false) //! first.isDeterministic() )
        {
          determinize(out, first);   //Note: determinize() will take care of clientInfo information.
        }
        else
        {
          out = first;
        }
      
      
        //FinalStates = AllStates - FinalStates
        StateSet oldFinalStates(out.beginFinalStates(), out.endFinalStates());
        out.clearFinalStates();
        for( NWA::stateIterator sit = out.beginStates(); sit != out.endStates(); sit++ )
        {
          if( oldFinalStates.count(*sit) == 0 )
            out.addFinalState(*sit);
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

