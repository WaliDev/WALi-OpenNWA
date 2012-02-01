#include "opennwa/Nwa.hpp"
#include "opennwa/construct/intersect.hpp"

namespace opennwa
{
  namespace construct
  {
    void intersect(Nwa & out, Nwa const & first, Nwa const & second)
    {
      out._private_intersect_(first, second);
    }


    NwaRefPtr intersect( Nwa const & first, Nwa const & second )
    {
      NwaRefPtr nwa(new Nwa());
      intersect(*nwa, first, second);
      return nwa;
    }
      
  } // end 'namespace construct'!!!

    /**
     *
     * @brief constructs the NWA which is the intersection of the given NWAs
     *
     * @param - first: the NWA to intersect with 'second'
     * @param - second: the NWA to intersect with 'first'
     *	
     */
  void Nwa::_private_intersect_( Nwa const & first, Nwa const & second ) 
  {
    //TODO: ponder the following ...
    //Q: how do we prevent the stuck state from being the same as any of the states that we
    //      generate as a part of this process?
    //A: allow it to be the stuck state of one of the given NWAs or ???

    //Q: do we need to realize all implicit transitions for this to work?  ie
    //   do we want to make explicit transitions to the stuck state so that (m,a,m') and
    //    (n,a,stuck) could potentially produce ( (m,n),a,(m',stuck) )?
    //Note: once one machine is in the stuck state, it will never leave the stuck state,
    //      thus unless the machine in the stuck state has an accepting stuck state,
    //      from any state (m',stuck) there is no path to an accepting state
    //      However, if the stuck state is an accepting state, there could be an accepting
    //      state of the form (m',stuck).  Further, if the other machine's stuck state is
    //      not accepting, then the state (stuck,stuck) will not be accepting.  
    //A: The stuck state is never accepting in any machine, so we do not need to distinguish
    //      (m,stuck) from stuck.

    //Clear all states(except the stuck state) and transitions from this machine.
    clear();

    std::set<StatePair> visitedPairs; // All the pairs of states we have ever encountered.
    std::deque<StatePair> worklistPairs; // Pairs of states yet to be processed
    typedef std::map<StatePair, Key> PairStMap;
    PairStMap pairToStMap; // The corresponding state in the product 
    // for all intersectable pairs encountered

    //Start the worklist with all possible initial states of the intersection NWA.
    for( StateIterator fit = first.beginInitialStates(); fit != first.endInitialStates(); fit++ ) 
    {
      for( StateIterator sit = second.beginInitialStates(); sit != second.endInitialStates(); sit++ ) 
      {
        State newSt;
        StatePair sp(*fit,*sit);
        visitedPairs.insert(sp);
        ClientInfoRefPtr resCI;
        if( stateIntersect(first,*fit,second,*sit,newSt,resCI) ) 
        {
          addInitialState(newSt); 
          //an initial state could also be a final state.
          if(first.isFinalState(*fit) && second.isFinalState(*sit))
            addFinalState(newSt);

          //Attach client info to the newly created state.
          states.setClientInfo(newSt,resCI);

          worklistPairs.push_back(sp);
          pairToStMap[sp] = newSt;
        }

        //perform the epsilon closure of sp
        std::set<StatePair> newPairs;
        epsilonClosure(&newPairs,sp,first,second);
        //add all new pairs to the worklist
        for(  std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
        {
          State st;
          //We don't want to put this on the worklist again.
          visitedPairs.insert(*it); 
          ClientInfoRefPtr CI;
          //Check and make sure this intersection makes sense.
          if( stateIntersect(first,it->first,second,it->second,st,CI) ) 
          {
            addInitialState(st);
            //an initital state could also be a final state
            if( first.isFinalState(it->first) && second.isFinalState(it->second) )
              addFinalState(st);

            //Attach client info to the newly created state.
            states.setClientInfo(st,CI);

            worklistPairs.push_back(*it);
            pairToStMap[*it] = st;
          }
        } //epsilon closure complete
      }      
    }

    //Process the worklist one pair at a time.
    while(! worklistPairs.empty() ) 
    {
      StatePair currpair = worklistPairs.front();
      worklistPairs.pop_front();

      //Process outgoing call transitions
      Calls firstCalls = first.trans.getTransCall(currpair.first);
      Calls secondCalls = second.trans.getTransCall(currpair.second);
      for(  Calls::const_iterator fit = firstCalls.begin(); fit != firstCalls.end(); fit++ ) 
      {
        Symbol firstSym = Trans::getCallSym(*fit);
        State firstEntry = Trans::getEntry(*fit);
        for(  Calls::const_iterator sit = secondCalls.begin(); sit != secondCalls.end(); sit++ ) 
        {
          Symbol secondSym = Trans::getCallSym(*sit);
          // Are the symbols intersectable
          Symbol resSym;
          if(! transitionIntersect(first,firstSym,second,secondSym,resSym) ) 
            continue; // Symbols not intersectable, do nothing

          State secondEntry = Trans::getEntry(*sit);
          StatePair entryPair(firstEntry,secondEntry);
            
          // If we have already considered tgtPair and found them to be nonintersectable, continue 
          if( visitedPairs.count(entryPair) != 0 && pairToStMap.count(entryPair) == 0 )
            continue;
          visitedPairs.insert(entryPair);

          State resSt;
          bool newlyCreatedResSt = false;
          // Have we seen entryPair before?
          if( pairToStMap.count(entryPair) == 0 ) 
          { 
            //We have not seen this pair before
            // Are the entry nodes intersectable?
            ClientInfoRefPtr resCI;
            if(! stateIntersect(first,firstEntry,second,secondEntry,resSt,resCI) ) 
              continue;
            newlyCreatedResSt = true;
            // We have a new state in resSt!
            if( first.isFinalState(firstEntry) && second.isFinalState(secondEntry) )
              addFinalState(resSt);
            else
              addState(resSt);

            //Attach client info to the newly created state.
            states.setClientInfo(resSt,resCI);

            //Add this to the worklist.
            worklistPairs.push_back(entryPair);
            pairToStMap[entryPair] = resSt;           
          } 
          else 
          { 
            //We have seen this pair before.
            resSt = pairToStMap[entryPair];
          }

          //perform the epsilon closure of entryPair
          std::set<StatePair> newPairs;
          epsilonClosure(&newPairs,entryPair,first,second);
          //add all new pairs to the worklist
          for(  std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
          {
            bool newlycreated = false;
            State st;
            //If we have already considered this pair and found them nonintersectable, continue
            if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
              continue;
            visitedPairs.insert(*it);
            //Have we seen this pair before?
            if( pairToStMap.count(*it) == 0 )
            {
              //Check and make sure this intersection makes sense.
              ClientInfoRefPtr CI;
              if( stateIntersect(first,it->first,second,it->second,st,CI) )
              {
                newlycreated = true;
                if( first.isFinalState(it->first) && second.isFinalState(it->second) )
                  addFinalState(st);
                else
                  addState(st); 

                //Attach client info to the newly created state.
                states.setClientInfo(st,CI);
                        
                //Add this to the worklist.
                worklistPairs.push_back(*it);
                pairToStMap[*it] = st;
              }
            }
            else
            {
              //We have seen this pair before.
              st = pairToStMap[*it];
            }

            //Add an edge that is the current call trans with collapsed epsilon internal trans.
            intersectClientInfoCall(first,Trans::getCallSite(*fit),it->first,
                                    second,Trans::getCallSite(*sit),it->second,
                                    resSym,st);    //Intersect Call Trans client info.
            if( isTransitionPossible( pairToStMap[currpair],resSym,st) ) {
              addCallTrans(pairToStMap[currpair],resSym,st);  
            }
            else if(newlycreated) {
              // transition not possible and newly created state
              // roll back; revert; clean up....
              worklistPairs.pop_back();
              pairToStMap.erase(*it);
              visitedPairs.erase(*it);
              removeState(st);
            }
          }

            
          //Add an edge that traverses the current call transition.
          intersectClientInfoCall(first,Trans::getCallSite(*fit),Trans::getEntry(*fit),
                                  second,Trans::getCallSite(*sit),Trans::getEntry(*sit),
                                  resSym,resSt);   //Intersect Call Trans client info.
          if( isTransitionPossible( pairToStMap[currpair],resSym,resSt) ) {
            addCallTrans(pairToStMap[currpair],resSym,resSt);
          }
          else if(newlyCreatedResSt) {
            // transition not possible and newly created state
            // roll back; revert; clean up....
            worklistPairs.pop_back();
            pairToStMap.erase(entryPair);
            visitedPairs.erase(entryPair);
            removeState(resSt);
          }
        }
      }

      // Process outgoing internal transitions
      Internals firstInternals = first.trans.getTransFrom(currpair.first);
      Internals secondInternals = second.trans.getTransFrom(currpair.second);
      for(  Internals::const_iterator fit = firstInternals.begin(); fit != firstInternals.end(); fit++ ) 
      {
        Symbol firstSym = Trans::getInternalSym(*fit);
        State firstTgt = Trans::getTarget(*fit);
        for(  Internals::const_iterator sit = secondInternals.begin(); sit != secondInternals.end(); sit++ ) 
        {
          Symbol secondSym = Trans::getInternalSym(*sit);
          // Are the symbols intersectable
          Symbol resSym;
          if(! transitionIntersect(first,firstSym,second,secondSym,resSym) ) 
            continue; // Symbols not intersectable, do nothing

          State secondTgt = Trans::getTarget(*sit);
          StatePair tgtPair(firstTgt,secondTgt);

          // If we have already considered tgtPair and found its elements to be nonintersectable, continue 
          if( visitedPairs.count(tgtPair) != 0 && pairToStMap.count(tgtPair) == 0 )
            continue;
          visitedPairs.insert(tgtPair);

          State resSt;
          bool newlyCreatedResSt = false;
          // Have we seen tgtPair before?
          if( pairToStMap.count(tgtPair) == 0 ) 
          { 
            //We have not seen this pair before
            // Are the tgt nodes intersectable?
            ClientInfoRefPtr resCI;
            if(! stateIntersect(first,firstTgt,second,secondTgt,resSt,resCI) ) 
              continue;
            newlyCreatedResSt = true;
            // We have a new state in resSt!
            if( first.isFinalState(firstTgt) && second.isFinalState(secondTgt) )
              addFinalState(resSt);
            else
              addState(resSt);

            //Attach client info to the newly created state.
            states.setClientInfo(resSt,resCI);

            worklistPairs.push_back(tgtPair);
            pairToStMap[tgtPair] = resSt;              
          } 
          else 
          { 
            // we have already seen this pair before
            resSt = pairToStMap[tgtPair];
          }

          //perform the epsilon closure of tgtPair
          std::set<StatePair> newPairs;
          epsilonClosure(&newPairs,tgtPair,first,second);
          //add all new pairs to the worklist
          for(  std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
          {
            State st;
            bool newlyCreatedSt = false;
            //If we have already considered this pair and found them nonintersectable, continue
            if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
              continue;
            visitedPairs.insert(*it);
            //Have we seen this pair before?
            if( pairToStMap.count(*it) == 0 )
            {
              //Check and make sure this intersection makes sense.
              ClientInfoRefPtr CI;
              if( stateIntersect(first,it->first,second,it->second,st,CI) )
              {
                if( first.isFinalState(it->first) && second.isFinalState(it->second) )
                  addFinalState(st);
                else
                  addState(st); 
                newlyCreatedSt = true;
                //Attach client info to the newly created state.
                states.setClientInfo(st,CI);
                        
                worklistPairs.push_back(*it);
                pairToStMap[*it] = st;
              }
            }
            else
            {
              //We have seen this pair before.
              st = pairToStMap[*it];
            }
                
            //Add an edge that is the current internal trans with collapsed epsilon internal trans.
            intersectClientInfoInternal(first,Trans::getSource(*fit),it->first,
                                        second,Trans::getSource(*sit),it->second,
                                        resSym, st);    //Intersect Internal Trans client info.
            if( isTransitionPossible( pairToStMap[currpair],resSym,st) ) {
              addInternalTrans(pairToStMap[currpair],resSym,st);  
            } else if( newlyCreatedSt ) {
              // transition not possible and newly created state
              // roll back; revert; clean up....
              worklistPairs.pop_back();
              pairToStMap.erase(*it);
              visitedPairs.erase(*it);
              removeState(st);
            }
          }
            

          //Add an edge that is the current internal transition.
          intersectClientInfoInternal(first,Trans::getSource(*fit),Trans::getTarget(*fit),
                                      second,Trans::getSource(*sit),Trans::getTarget(*sit),
                                      resSym,resSt);   //Intersect Internal Trans client info.
          if( isTransitionPossible( pairToStMap[currpair],resSym,resSt) )  {
            addInternalTrans(pairToStMap[currpair],resSym,resSt);
          } else if( newlyCreatedResSt ) {
            // transition not possible and newly created state
            // roll back; revert; clean up....
            worklistPairs.pop_back();
            pairToStMap.erase(tgtPair);
            visitedPairs.erase(tgtPair);
            removeState(resSt);
          }
        }
      }
        
      // Process outgoing return transitions where currpair.first and currpair.second are
      // both exit components of the respective return transitions
      Returns firstReturns = first.trans.getTransExit(currpair.first);
      Returns secondReturns = second.trans.getTransExit(currpair.second);
      for(  Returns::const_iterator fit = firstReturns.begin(); fit != firstReturns.end(); fit++ ) 
      {
        State firstCall = Trans::getCallSite(*fit);
        Symbol firstSym = Trans::getReturnSym(*fit);
        State firstRet = Trans::getReturnSite(*fit);
        for(  Returns::const_iterator sit = secondReturns.begin(); sit != secondReturns.end(); sit++ ) 
        {
          Symbol secondSym = Trans::getReturnSym(*sit);
          // Are the symbols intersectable
          Symbol resSym;
          if(! transitionIntersect(first,firstSym,second,secondSym,resSym) ) 
            continue; // Symbols not intersectable, do nothing

          // Check intersectability and visited status of the respective call components
          State secondCall = Trans::getCallSite(*sit);
          StatePair callPair(firstCall,secondCall);  // Call components of the respective return transitions
          // Proceed only if the pair of call components has already been visited 
          if( visitedPairs.count(callPair) == 0 )
            continue;
          // If we have already considered callPair and found its elements to be nonintersectable, continue 
          if( visitedPairs.count(callPair) != 0 && pairToStMap.count(callPair) == 0 )
            continue;
          State callSt = pairToStMap[callPair];

          // Check intersectability and visited status of the respective return components
          State secondRet = Trans::getReturnSite(*sit);
          StatePair retPair(firstRet,secondRet);
          // If we have already considered retPair and found its elements to be nonintersectable, continue 
          if( visitedPairs.count(retPair) != 0 && pairToStMap.count(retPair) == 0 )
            continue;
          visitedPairs.insert(retPair);

          State retSt;
          bool newlyCreatedRetSt = false;
          // Are the return components intersectable?
          if( pairToStMap.count(retPair) == 0 ) 
          { // Don't know yet
            ClientInfoRefPtr retCI;
            if(! stateIntersect(first,firstRet,second,secondRet,retSt,retCI) ) 
              continue;
            // We have found a new state in retSt!
            newlyCreatedRetSt = true;
            if( first.isFinalState(firstRet) && second.isFinalState(secondRet) )
              addFinalState(retSt);
            else
              addState(retSt);

            //Attach client info to the newly created state.
            states.setClientInfo(retSt,retCI);

            worklistPairs.push_back(retPair);
            pairToStMap[retPair] = retSt;
          } 
          else 
          {  // We have already seen retPair before and its components are intersectable
            retSt = pairToStMap[retPair];
          }

          //perform the epsilon closure of retPair
          std::set<StatePair> newPairs;
          epsilonClosure(&newPairs,retPair,first,second);
          //add all new pairs to the worklist
          for(  std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
          {
            State st;
            bool newlyCreatedSt = false;
            //If we have already considered this pair and found them nonintersectable, continue
            if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
              continue;
            visitedPairs.insert(*it);
            //Have we seen this pair before?
            if( pairToStMap.count(*it) == 0 )
            {
              //Check and make sure this intersection makes sense.
              ClientInfoRefPtr CI;
              if( stateIntersect(first,it->first,second,it->second,st,CI) )
              {
                if( first.isFinalState(it->first) && second.isFinalState(it->second) )
                  addFinalState(st);
                else
                  addState(st);
                newlyCreatedSt = true;
                //Attach client info to the newly created state.
                states.setClientInfo(st,CI);
                        
                worklistPairs.push_back(*it);
                pairToStMap[*it] = st;
              }
            }
            else
            {
              //We have seen this pair before.
              st = pairToStMap[*it];
            }
                
            //Add an edge that is the current return trans with collapsed epsilon internal trans.
            intersectClientInfoReturn(first,Trans::getExit(*fit),Trans::getCallSite(*fit),it->first,
                                      second,Trans::getExit(*sit),Trans::getCallSite(*sit),it->second,
                                      resSym,st);    //Intersect Internal Trans client info.
            if( isTransitionPossible(pairToStMap[currpair],resSym,st) ) {
              addReturnTrans(pairToStMap[currpair],callSt,resSym,st);  
            } else if( newlyCreatedSt ) {
              removeState( st );
            }
          }
            
            
          //Add an edge that is the current return transition.
          intersectClientInfoReturn(first,Trans::getExit(*fit),Trans::getCallSite(*fit),Trans::getReturnSite(*fit),
                                    second,Trans::getExit(*sit),Trans::getCallSite(*sit),Trans::getReturnSite(*sit),
                                    resSym,retSt);   //Intersect Return Trans client info.
          if( isTransitionPossible(pairToStMap[currpair],resSym,retSt) ) {
            addReturnTrans(pairToStMap[currpair],callSt,resSym,retSt);
          } else if( newlyCreatedRetSt ) {
            // transition not possible and newly created state
            // roll back; revert; clean up....
            worklistPairs.pop_back();
            pairToStMap.erase(retPair);
            visitedPairs.erase(retPair);
            removeState(retSt);
          }
        }
      }

      // Process outgoing return transitions where currpair.first and currpair.second are
      // both call components of the respective return transitions 
      firstReturns = first.trans.getTransPred(currpair.first);
      secondReturns = second.trans.getTransPred(currpair.second);
      for(  Returns::const_iterator fit = firstReturns.begin(); fit != firstReturns.end(); fit++ ) 
      {
        State firstExit = Trans::getExit(*fit);
        Symbol firstSym = Trans::getReturnSym(*fit);
        State firstRet = Trans::getReturnSite(*fit);
        for(  Returns::const_iterator sit = secondReturns.begin(); sit != secondReturns.end(); sit++ ) 
        {
          Symbol secondSym = Trans::getReturnSym(*sit);
          // Are the symbols intersectable
          Symbol resSym;
          if(! transitionIntersect(first,firstSym,second,secondSym,resSym) ) 
            continue; // Symbols not intersectable, do nothing

          // Check intersectability and visited status of the respective exit components
          State secondExit = Trans::getExit(*sit);
          StatePair exitPair(firstExit, secondExit);  // Exit components of the respective return transitions
          // Proceed only if the pair of exit components has already been visited 
          if( visitedPairs.count(exitPair) == 0 )
            continue;
          // If we have already considered exitPair and found its elements to be nonintersectable, continue 
          if( visitedPairs.count(exitPair) != 0 && pairToStMap.count(exitPair) == 0 ) 
            continue;
          State exitSt = pairToStMap[exitPair];

          // Check intersectability and visited status of the respective return components
          State secondRet = Trans::getReturnSite(*sit);
          StatePair retPair(firstRet, secondRet);
          // If we have already considered retPair and found its elements to be nonintersectable, continue 
          if( visitedPairs.count(retPair) != 0 && pairToStMap.count(retPair) == 0 )
            continue;
          visitedPairs.insert(retPair);

          State retSt;
          bool newlyCreatedRetSt = false;
          //  Are the return components intersectable?
          if( pairToStMap.count(retPair) == 0 ) 
          { //Don't know yet
            ClientInfoRefPtr retCI;
            if(! stateIntersect(first,firstRet,second,secondRet,retSt,retCI) ) 
              continue;
            // We have a new state in retSt!
            newlyCreatedRetSt = true;
            if( first.isFinalState(firstRet) && second.isFinalState(secondRet) )
              addFinalState(retSt);
            else
              addState(retSt);

            //Attach client info to the newly created state.
            states.setClientInfo(retSt,retCI);

            worklistPairs.push_back(retPair);
            pairToStMap[retPair] = retSt;
          } 
          else 
          { //  We have already seen retPair before and its components are intersectable
            retSt = pairToStMap[retPair];
          }

          //perform the epsilon closure of retPair
          std::set<StatePair> newPairs;
          epsilonClosure(&newPairs,retPair,first,second);
          //add all new pairs to the worklist
          for(  std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
          {
            State st;
            bool newlyCreatedSt = false;
            //If we have already considered this pair and found them nonintersectable, continue
            if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
              continue;
            visitedPairs.insert(*it);
            //Have we seen this pair before?
            if( pairToStMap.count(*it) == 0 )
            {
              //Check and make sure this intersection makes sense.
              ClientInfoRefPtr CI;
              if( stateIntersect(first,it->first,second,it->second,st,CI) )
              {
                if( first.isFinalState(it->first) && second.isFinalState(it->second) )
                  addFinalState(st);
                else
                  addState(st); 
                newlyCreatedSt = true;
                //Attach client info to the newly created state.
                states.setClientInfo(st,CI);
                        
                worklistPairs.push_back(*it);
                pairToStMap[*it] = st;
              }
            }
            else
            {
              //We have seen this pair before.
              st = pairToStMap[*it];
            }
                
            //Add an edge that is the current return trans with collapsed epsilon internal trans.
            intersectClientInfoReturn(first,Trans::getExit(*fit),Trans::getCallSite(*fit),it->first,
                                      second,Trans::getExit(*sit),Trans::getCallSite(*sit),it->second,
                                      resSym,st);    //Intersect Internal Trans client info.
            if( isTransitionPossible(exitSt,resSym,st) ) {
              addReturnTrans(exitSt,pairToStMap[currpair],resSym,st);  
            } else if( newlyCreatedSt ) {
              // transition not possible and newly created state
              // roll back; revert; clean up....
              worklistPairs.pop_back();
              pairToStMap.erase(*it);
              visitedPairs.erase(*it);
              removeState(st);
            }
          }

            
          //Add an edge that is the current return transition.
          intersectClientInfoReturn(first,Trans::getExit(*fit),Trans::getCallSite(*fit),Trans::getReturnSite(*fit),
                                    second,Trans::getExit(*sit),Trans::getCallSite(*sit),Trans::getReturnSite(*sit),
                                    resSym,retSt);   //Intersect Return Trans client info.
          if( isTransitionPossible(exitSt,resSym,retSt) ) {
            addReturnTrans(exitSt,pairToStMap[currpair],resSym,retSt);
          } else if( newlyCreatedRetSt ) {
            // transition not possible and newly created state
            // roll back; revert; clean up....
            worklistPairs.pop_back();
            pairToStMap.erase(retPair);
            visitedPairs.erase(retPair);
            removeState(retSt);
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

