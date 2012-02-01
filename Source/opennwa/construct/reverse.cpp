#include "opennwa/Nwa.hpp"
#include "opennwa/construct/reverse.hpp"
#include "opennwa/details/TransitionStorage.hpp"

using opennwa::details::TransitionStorage;

namespace opennwa
{
  namespace construct
  {

    NwaRefPtr reverse( Nwa const & source )
    {
      NwaRefPtr nwa( new Nwa());
      reverse(*nwa, source);
      return nwa;
    }

    /**
     * @brief constructs the NWA which is the reverse of the given NWA
     *
     * @param - first: the NWA to reverse
     * 
     */
    void reverse( Nwa & out, Nwa const & first )
    {
      //Q: How should clientInfos be generated for the reversed NWA?
      //A: The clientInfos from the component machines are copied and added to the reversed NWA.

      //Clear all states(except the stuck state) and transitions from this machine.
      out.clear();

      State prime = wali::getKey("prime");

      //The reverse machine has all the states of the original machine.
      // FIXME-RELEASE: copy clientinfo. Or figure out how to do this.
      for (Nwa::StateIterator sit = first.beginStates();
           sit != first.endStates(); ++sit)
      {
        State state = *sit;
        State primed = getKey(state, prime);
          
        out.addState(state);
        out.addState(primed);
      }
          

      //Swap initial and final state functionality.
      for( Nwa::StateIterator it = first.beginInitialStates(); 
           it != first.endInitialStates(); it++ )
      {
        State state = *it;
        State primed = getKey(state, prime);
          
        out.addFinalState(state);
        out.addFinalState(primed);
      }
      for( Nwa::StateIterator it = first.beginFinalStates(); 
           it != first.endFinalStates(); it++ )
      {
        out.addInitialState(getKey(*it, prime));
      }

      
      //Duplicate internal transitions with source/target swapped.
      for( Nwa::InternalIterator it = first.beginInternalTrans(); 
           it != first.endInternalTrans(); it++ )
      {
        State source = it->first;
        Symbol symbol = it->second;
        State target = it->third;

        State source_p = getKey(source, prime);
        State target_p = getKey(target, prime);
          
        out.addInternalTrans(target, symbol, source);
        out.addInternalTrans(target_p, symbol, source_p);
      }


      //Duplicate call transitions with associated return transitions as 
      //return transitions with (entry,return,sym,call).
      for( Nwa::CallIterator cit = first.beginCallTrans(); 
           cit != first.endCallTrans(); cit++ )
      {
        State call_site = cit->first;
        Symbol call_sym = cit->second;
        State entry_site = cit->third;

        State call_site_p = getKey(call_site, prime);
        State entry_site_p = getKey(entry_site, prime);
          
        for( Nwa::ReturnIterator rit = first.beginReturnTrans(); 
             rit != first.endReturnTrans(); rit++ )
        {
          if( TransitionStorage::getCallSite(*cit) == TransitionStorage::getCallSite(*rit) )
          {
            State exit_site = rit->first;
            //State call_pred = rit->second; same as call_site
            State return_sym = rit->third;
            State return_site = rit->fourth;

            //State exit_site_p = getKey(exit_site, prime);
            State return_site_p = getKey(return_site, prime);

            // Add to delta_c:
            //    (r, ret_sym, x)
            //    (r', ret_sym, x)
            //
            // Add to delta_r:
            //    (e, r, call_sym, c)
            //    (e, r', call_sym, c')
            //    (e', q_0, call_sym, c') for each q_0 (in reverse)
              
            out.addCallTrans(return_site, return_sym, exit_site);
            out.addCallTrans(return_site_p, return_sym, exit_site);

            out.addReturnTrans(entry_site, return_site, call_sym, call_site);
            out.addReturnTrans(entry_site, return_site_p, call_sym, call_site_p);
            for (Nwa::StateIterator q0it = out.beginInitialStates();
                 q0it != out.endInitialStates(); ++q0it)
            {
              out.addReturnTrans(entry_site_p, *q0it, call_sym, call_site_p);
            }
              
          } // end if (binding call_site=call_pred)
        } // for return trans
      } // for call trans

    }

      
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

