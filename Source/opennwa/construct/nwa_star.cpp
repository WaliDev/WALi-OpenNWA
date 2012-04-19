#include "opennwa/Nwa.hpp"
#include "opennwa/construct/star.hpp"

namespace opennwa
{

  namespace construct
  {

    void star(Nwa & out, Nwa const & source)
    {
      out._private_star_(source);
    }

    NwaRefPtr star( Nwa const & source )
    {
      NwaRefPtr nwa( new Nwa());
      star(*nwa, source);
      return nwa;
    }
      
  } // end of 'namespace construct' !!!
  
    
    /**
     *
     * @brief constructs the NWA resulting from performing Kleene-* on the given NWA
     *
     * @param - first: the NWA to perform Kleene-* on
     *
     */
    
  void Nwa::_private_star_( Nwa const & first )
  {
    State newStart = getKey("kleeneStarStart");
    assert(!first.isState(newStart));
      
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

     

    //The initial and final state of A* is newStart
    clearInitialStates();
    clearFinalStates();
    addInitialState(newStart);
    addFinalState(newStart);

    // Add newStart->q0 transitions
    for( StateIterator sit = first.beginInitialStates(); sit != first.endInitialStates(); sit++ ) {
      State target = getKey(*sit, prime);
      // INTERNAL (inference rule in TR)
      addInternalTrans(newStart, EPSILON, target);
    }
    for( StateIterator sit = first.beginFinalStates(); sit != first.endFinalStates(); sit++ ) {
      State f = *sit;
      State fp = getKey(*sit, prime);

      // RESTART (inference rule in TR)
      addInternalTrans(f, EPSILON, newStart);
      addInternalTrans(fp, EPSILON, newStart);
    }
      

    //Transitions of A*:

    //Internal: for each (q,a,p) in delta_i, A* gets (q,a,p) and (q',a,p')
    for( InternalIterator iit = first.beginInternalTrans();
         iit != first.endInternalTrans(); iit++ )
    {
      State q = Trans::getSource(*iit);
      Symbol a = Trans::getInternalSym(*iit);
      State p = Trans::getTarget(*iit);

      //(q,a,p)
      // INTERNAL (inference rule in TR)
      addInternalTrans(q,a,p);

      //(q',a,p')
      State qp = wali::getKey(q,prime);
      State pp = wali::getKey(p,prime);
      // INTERNAL (inference rule in TR)
      addInternalTrans(qp,a,pp);
    }

    //Call: for each(q,a,p) in delta_c, A* gets (q,a,p) and (q',a,p)
    for( CallIterator cit = first.beginCallTrans();
         cit != first.endCallTrans(); cit++ )
    {
      State q = Trans::getCallSite(*cit);
      Symbol a = Trans::getCallSym(*cit);
      State p = Trans::getEntry(*cit);

      //(q,a,p)
      // CALL (inference rule in TR)
      addCallTrans(q,a,p);

      //(q',a,p)
      State qp = wali::getKey(q,prime);
      // CALL (inference rule in TR)
      addCallTrans(qp,a,p);
    }

    //Return: for each (q,r,a,p) in delta_r, A* gets (q,r,a,p) and (q,r',a,p'), 
    //          For each (q,r,a,p) in delra_r with r in Q0, 
    //            A* gets (q',s,a,p') for each s in Q union Q' and (q',newStart,a,p')
    for( ReturnIterator rit = first.beginReturnTrans();
         rit != first.endReturnTrans(); rit++ )
    {
      State q = Trans::getExit(*rit);
      State r = Trans::getCallSite(*rit);
      Symbol a = Trans::getReturnSym(*rit);
      State p = Trans::getReturnSite(*rit);

      State qp = wali::getKey(q,prime);

      //(q,r,a,p)
      // RETURN (inference rule in TR)
      addReturnTrans(q,r,a,p);

      //(q,r',a,p')
      State rp = wali::getKey(r,prime);
      State pp = wali::getKey(p,prime);
      // RETURN (inference rule in TR)
      addReturnTrans(q,rp,a,pp);

      //if r in Q0
      if( first.isInitialState(r) )
      {
        // (q',newStart,a,p')
        // GLOBALLY-PENDING (inference rule in TR)
        addReturnTrans(qp,newStart,a,pp);
          
        //for each s in Q
        for( StateIterator sit = first.beginStates();
             sit != first.endStates(); sit++ )
        {
          State s = *sit;

          //Handle s
          //(q',s,a,p')
          // LOCALLY_PENDING (inference rule in TR)
          addReturnTrans(qp,s,a,pp);

          //Handle corresponding s'
          //(q',s',a,p')
          // LOCALLY_PENDING (inference rule in TR)
          State sp = wali::getKey(s,prime);
          addReturnTrans(qp,sp,a,pp);
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

