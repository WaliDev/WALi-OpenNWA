#include "wali/nwa/NWA.hpp"

namespace wali
{
  namespace nwa
  {
    namespace construct
    {

      void star(NWA & out, NWA const & source)
      {
        out._private_star_(source);
      }

      NWARefPtr star( NWA const & source )
      {
        NWARefPtr nwa( new NWA());
        star(*nwa, source);
        return nwa;
      }
      
    }
    
    /**
     *
     * @brief constructs the NWA resulting from performing Kleene-* on the given NWA
     *
     * @param - first: the NWA to perform Kleene-* on
     *
     */
    
    void NWA::_private_star_( NWA const & first )
    {
      //TODO: ponder the following ... 
      //Q: how do we prevent the stuck state from being the same as any of the states that we
      //      generate as a part of this process?
      //A: check against all possible overlaps?  this might be expensive, but it would work

      //Q: How should clientInfos be generated for the star NWA?
      //A: The clientInfos from the component machine are copied to both the direct copies of
      //    states and the primed copies of states and added to the star NWA.

      //Clear all states(except the stuck state) and transitions from this machine.
      clear();

      //Somehow mark unmatched nesting edges as if they are pending (tag its state so that 
      //at a return the states labeling the incident nesting edges are ignored).
      State prime = wali::getKey("prime");

      //The state-space of A* is Q + Q'.
      states.addAll(first.states); //Note: This includes copying clientInfo information over.    
      for( StateIterator sit = first.beginStates(); sit != first.endStates(); sit++ )
      {
        State sp = wali::getKey(*sit,prime);
        states.addState(sp);

        //Note: The clientInfos for the states in Q' are copies of the clientInfos for the 
        //      corresponding states from Q.
        //Set the clientInfo of this state.
        ClientInfoRefPtr ci = first.getClientInfo(*sit);
        if (ci.is_valid()) {
          states.setClientInfo(sp, ci->cloneRp());
        }
      }

      clearInitialStates();
      clearFinalStates();

      //The initial and final states of A* are Q0'. 
      for( StateIterator sit = first.beginInitialStates(); 
           sit != first.endInitialStates(); sit++ )
      {
        State sp = wali::getKey(*sit,prime);
        addInitialState(sp);
        addFinalState(sp);
      }

      //Transitions of A*:

      //Internal: for each (q,a,p) in delta_i, A* gets (q,a,p) and (q',a,p') and if
      //          p in Qf, then (q,a,r') and (q',a,r') for each r in Q0
      for( InternalIterator iit = first.beginInternalTrans();
           iit != first.endInternalTrans(); iit++ )
      {
        State q = Trans::getSource(*iit);
        Symbol a = Trans::getInternalSym(*iit);
        State p = Trans::getTarget(*iit);

        //(q,a,p)
        addInternalTrans(q,a,p);

        //(q',a,p')
        State qp = wali::getKey(q,prime);
        State pp = wali::getKey(p,prime);
        addInternalTrans(qp,a,pp);

        //if p in Qf
        if( first.isFinalState(p) )
        {
          //for each r in Q0
          for( StateIterator sit = first.beginInitialStates(); 
               sit != first.endInitialStates(); sit++ )
          {
            State rp = wali::getKey(*sit,prime);

            //(q,a,r')
            addInternalTrans(q,a,rp);

            //(q',a,r')
            addInternalTrans(qp,a,rp);
          }
        }
      }

      //Call: for each(q,a,p) in delta_c, A* gets (q,a,p) and (q',a,p), 
      //      and if p in Qf then (q,a,r') and (q',a,r') for each r in Q0
      for( CallIterator cit = first.beginCallTrans();
           cit != first.endCallTrans(); cit++ )
      {
        State q = Trans::getCallSite(*cit);
        Symbol a = Trans::getCallSym(*cit);
        State p = Trans::getEntry(*cit);

        //(q,a,p)
        addCallTrans(q,a,p);

        //(q',a,p)
        State qp = wali::getKey(q,prime);
        addCallTrans(qp,a,p);

        //if p in Qf
        if( first.isFinalState(p) )
        {
          //for each r in Q0
          for( StateIterator sit = first.beginInitialStates();
               sit != first.endInitialStates(); sit++ )
          {
            State rp = wali::getKey(*sit,prime);

            //(q,a,r')
            addCallTrans(q,a,rp);

            //(q',a,r')
            addCallTrans(qp,a,rp);
          }
        }
      }

      //Return: for each (q,r,a,p) in delta_r, A* gets (q,r,a,p) and (q,r',a,p'), 
      //          and if p in Qf, then (q,r,a,s') and (q,r',a,s') for each s in Q0
      //          For each (q,r,a,p) in delra_r with r in Q0, 
      //            A* gets (q',s,a,p') for each s in Q union Q' 
      //          and if p in Qf, then (q',s,a,t') for each s in Q union Q' and t in Q0.
      for( ReturnIterator rit = first.beginReturnTrans();
           rit != first.endReturnTrans(); rit++ )
      {
        State q = Trans::getExit(*rit);
        State r = Trans::getCallSite(*rit);
        Symbol a = Trans::getReturnSym(*rit);
        State p = Trans::getReturnSite(*rit);

        //(q,r,a,p)
        addReturnTrans(q,r,a,p);

        //(q,r',a,p')
        State rp = wali::getKey(r,prime);
        State pp = wali::getKey(p,prime);
        addReturnTrans(q,rp,a,pp);

        //if p in Qf
        if( first.isFinalState(p) )
        {
          //for each s in Q0
          for( StateIterator sit = first.beginInitialStates();
               sit != first.endInitialStates(); sit++ )
          { 
            State sp = wali::getKey(*sit,prime);

            //(q,r,a,s')
            addReturnTrans(q,r,a,sp);

            //(q,r',a,s')
            addReturnTrans(q,rp,a,sp);
          } 
        }

        //if r in Q0
        if( first.isInitialState(r) )
        {
          //for each s in Q
          for( StateIterator sit = first.beginStates();
               sit != first.endStates(); sit++ )
          {
            State s = *sit;
            State qp = wali::getKey(q,prime);

            //Handle s
            //(q',s,a,p')
            addReturnTrans(qp,s,a,pp);

            //Handle corresponding s'
            //(q',s',a,p')
            State sp = wali::getKey(s,prime);
            addReturnTrans(qp,sp,a,pp);

            //if p in Qf
            if( first.isFinalState(p) )
            {
              //for each t in Q0
              for( StateIterator it = first.beginInitialStates();
                   it != first.endInitialStates(); it++ )
              {                
                State tp = wali::getKey(*it,prime);

                //Handle s
                //(q',s,a,t')
                addReturnTrans(qp,s,a,tp);

                //Handle corresponding s'
                //(q',s',a,t')
                addReturnTrans(qp,sp,a,tp);
              }
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

