#include "TransitionStorage.hpp"

using wali::printKey;

namespace opennwa
{
  namespace details
  {
    
    //
    // Methods
    //

    TransitionStorage & TransitionStorage::operator=( const TransitionStorage & other )
    {
      if (this == &other)     
        return *this;

      clear();
      T_info.clearMaps();
        
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      
      T_info = other.T_info;
      return *this;
    }
   
    //Transition Accessors

    /**
     *  
     * @brief finds the symbol on any transition whose source is 'from' and
     *        whose target is 'to'
     *
     * @param - from: the source of the transition
     * @param - to: the target of the transition
     * @return true if there exists some transition with the given source and 
     *         target, false otherwise
     *
     */
    bool TransitionStorage::getSymbol( State fromSt, State toSt, Symbol & sym ) const
    {
      //Check internal transitions.
      const Info::Internals & from = T_info.fromTrans(fromSt);
      for(Info::InternalIterator it = from.begin(); it != from.end(); it++ )
      {
        if( toSt == getTarget(*it) )
        {
          sym = getInternalSym(*it);
          return true;
        }
      }
      //Check call transitions.
      const Info::Calls & call = T_info.callTrans(fromSt);
      for(Info::CallIterator it = call.begin(); it != call.end(); it++ )
      {
        if( toSt == getEntry(*it) )
        {
          sym = getCallSym(*it);
          return true;
        }
      }
      //Check return transitions.      
      const Info::Returns & exit = T_info.exitTrans(fromSt);
      for(Info::ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( toSt == getReturnSite(*it) )
        {
          sym = getReturnSym(*it);
          return true;
        }
      }
      //Q: does this count as a symbol we would like to have?
      /*const Info::Returns pred = T_info.predTrans(fromSt);
        for( Info::ReturnIterator it = pred.begin(); it != pred.end(); it++ )
        {
        if( toSt == getReturnSite(*it) )
        {
        sym = getReturnSym(*it);
        return true;
        }
        }*/
      
      return false;
    }

    /**
     * 
     * @brief tests whether there exists a transition whose source is 'from',
     *        whose symbol is 'sym', and whose target is 'to'
     *
     * @param - from: the source of the transition
     * @param - sym: the symbol that labels the transition
     * @param - to: the target of the transition
     * @return true if such a transition exists, false otherwise
     * 
     */
    bool TransitionStorage::findTrans( State fromSt, Symbol sym, State toSt ) const
    {
      //Check internal transitions.
      const Info::Internals & from = T_info.fromTrans(fromSt);
      for( Info::InternalIterator it = from.begin(); it != from.end(); it++ )
      {
        if( toSt == getTarget(*it) && ( sym == getInternalSym(*it) || sym == WILD ) )
          return true;
      }
      //Check call transitions.
      const Info::Calls & call = T_info.callTrans(fromSt);
      for( Info::CallIterator it = call.begin(); it != call.end(); it++ )
      {
        if( toSt == getEntry(*it) && ( sym == getCallSym(*it) || sym == WILD ) )
        {
          return true;
        }
      }
      //Check return transitions.      
      const Info::Returns & exit = T_info.exitTrans(fromSt);
      for( Info::ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( toSt == getReturnSite(*it) && ( sym == getReturnSym(*it) || sym == WILD ) )
        {
          return true;
        }
      }
      //Q: does this count as a symbol we would like to have?
      /*const Info::Returns pred = T_info.predTrans(fromSt);
        for( Info::ReturnIterator it = pred.begin(); it != pred.end(); it++ )
        {
        if( toSt == getReturnSite(*it) && sym == getReturnSym(*it) )
        {
        return true;
        }
        }*/
      
      return false;
    }

    /**
     * 
     * @brief returns all return sites that correspond with the given call site
     *
     * @param - callSite: the call site whose return sites to look for
     * @return the set of all return sites that correspond with the given call site
     *
     */
    const TransitionStorage::States TransitionStorage::getReturnSites( State callSite ) const
    {
      States returns;
      const Info::Returns & pred = T_info.predTrans(callSite);
      for( Info::ReturnIterator it = pred.begin(); it != pred.end(); it++ )
      {
        returns.insert(getReturnSite(*it));
      }
      return returns;
    }

    /**
     * 
     * @brief returns all return sites that correspond with the given exit and call site
     *
     * @param - exit: the exit whose return sites to look for
     * @param - callSite: the call site whose return sites to look for
     * @return the set of all return sites that correspond with the given call site
     *
     */
    TransitionStorage::States TransitionStorage::getReturnSites( State exit, State callSite ) const
    {
      States returns;
      const Info::Returns & pred = T_info.predTrans(callSite);
      for( Info::ReturnIterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( getExit(*it) == exit )
          returns.insert(getReturnSite(*it));
      }
      return returns;
    }

    /**
     * 
     * @brief returns all call sites that correspond with the given exit - return site pair
     *
     * @param - exitSite: the exit of the pair whose call sites to look for
     * @param = returnSite: the return site of the pair whose call sites to look for
     * @return the set of all call sites that correspond with the exit - return site pair
     *
     */
    const TransitionStorage::States TransitionStorage::getCallSites( State exitSite, State returnSite ) const
    {
      States calls;
      const Info::Returns & exit = T_info.exitTrans(exitSite);
      for( Info::ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( getReturnSite(*it) == returnSite )
          calls.insert(getCallSite(*it));  
      }
      return calls;
    }

    /**
     * 
     * @brief returns all entry sites that correspond with the given call site
     *
     * @param - callSite: the call site whose entry sites to look for
     * @return the set of all entry sites that correspond with the given call site
     *
     */
    const TransitionStorage::States TransitionStorage::getEntries( State callSite ) const
    {
      States entries;
      const Info::Calls & cll = T_info.callTrans(callSite);
      for( Info::CallIterator it = cll.begin(); it != cll.end(); it++ )
      {
        entries.insert(getEntry(*it));
      }
      return entries;
    }

    /**
     * 
     * @brief returns all targets that correspond with the given source 
     *
     * @param - source: the source whose targets to look for
     * @return the set of all targets that correspond with the given source 
     *
     */
    const TransitionStorage::States TransitionStorage::getTargets( State source ) const
    {
      States targets;
      const Info::Internals & src = T_info.fromTrans(source);
      for( Info::InternalIterator it = src.begin(); it != src.end(); it++ )
      {
        targets.insert(getTarget(*it));
      }
      return targets;
    }

    /**
     *   
     * @brief creates transitions for 'dup' mirroring 'orig' outgoing transitions
     *
     * @param - orig: the state that is being duplicated
     * @param - dup: the state that is duplicating 'orig'
     *  
     */
    void TransitionStorage::dupTransOutgoing( State orig, State dup )
    { 
      // Duplicate outgoing internal transitions.
      const Info::Internals & from = T_info.fromTrans(orig);
      for( Info::InternalIterator it = from.begin(); it != from.end(); it++ )
      {
        Internal iTrans(dup,getInternalSym(*it),getTarget(*it));
        addInternal(iTrans);
      }

      // Duplicate call site call transitions.
      const Info::Calls & call = T_info.callTrans(orig);
      for( Info::CallIterator it = call.begin(); it != call.end(); it++ )
      {
        Call cTrans(dup,getCallSym(*it),getEntry(*it));
        addCall(cTrans);

      }

      // Duplicate exit point return transitions.
      const Info::Returns & exit = T_info.exitTrans(orig);
      for( Info::ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        Return rTrans(dup,getCallSite(*it),getReturnSym(*it),getReturnSite(*it));
        addReturn(rTrans);
      }

      // Q: Do we want to do these? Seems inconsistent. -Evan
      // A: Aditya says 'yes', and thinks we need it. So we definitely don't
      //    want to change it without looking at uses and compensating.
      // Duplicate call predecessor return transitions.
      const Info::Returns & pred = T_info.predTrans(orig);
      for( Info::ReturnIterator it = pred.begin(); it != pred.end(); it++ )
      {
        Return rTrans(getExit(*it),dup,getReturnSym(*it),getReturnSite(*it));
        addReturn(rTrans);
      }
    }

    /**
     *  
     * @brief creates transitions for 'dup' mirroring 'orig' transitions
     *
     * @param - orig: the state that is being duplicated
     * @param - dup: the state that is duplicating 'orig'
     *  
     */
    void TransitionStorage::dupTrans( State orig, State dup )
    { 
      //Duplicate outgoing internal transitions.
      const Info::Internals & from = T_info.fromTrans(orig);
      for( Info::InternalIterator it = from.begin(); it != from.end(); it++ )
      {
        Internal iTrans(dup,getInternalSym(*it),getTarget(*it));
        addInternal(iTrans);

        // Q: This is also inconsistent with dupTransOutgoing, which didn't do
        //    anything special with self loops. -Evan
        // A: Aditya says yes, but this is again what McVeto(?) needs.
        //    dupTrans is used in 'duplicateState', but dupTransOutgoing is
        //    different.
        if( orig == getTarget(*it) )   //Handle self-loops.
        {
          Internal loop(dup,getInternalSym(*it),dup);
          addInternal(loop);
        }
      }

      //Duplicate incoming internal transitions.
      const Info::Internals & to = T_info.toTrans(orig);
      for( Info::InternalIterator it = to.begin(); it != to.end(); it++ )
      {
        Internal iTrans(getSource(*it),getInternalSym(*it),dup);
        addInternal(iTrans);
      }

      //Duplicate call site call transitions.
      const Info::Calls & call = T_info.callTrans(orig);
      for( Info::CallIterator it = call.begin(); it != call.end(); it++ )
      {
        Call cTrans(dup,getCallSym(*it),getEntry(*it));
        addCall(cTrans);
        if( orig == getEntry(*it) )   //Handle self-loops.
        {
          Call loop(dup,getCallSym(*it),dup);
          addCall(loop);
        }
      }

      //Duplicate entry point call transitions.
      const Info::Calls & entry = T_info.entryTrans(orig);
      for( Info::CallIterator it = entry.begin(); it != entry.end(); it++ )
      {
        Call cTrans(getCallSite(*it),getCallSym(*it),dup);
        addCall(cTrans);
      }

      //Duplicate exit point return transitions.
      const Info::Returns & exit = T_info.exitTrans(orig);
      for( Info::ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        Return rTrans(dup,getCallSite(*it),getReturnSym(*it),getReturnSite(*it));
        addReturn(rTrans);
        if( orig == getCallSite(*it) )   //Handle self-loops.
        {
          Return loop(dup,dup,getReturnSym(*it),getReturnSite(*it));
          addReturn(loop);
        }
        if( orig == getReturnSite(*it) )   //Handle self-loops.
        {
          Return loop(dup,getCallSite(*it),getReturnSym(*it),dup);
          addReturn(loop);
        }
        if( orig == getCallSite(*it) && orig == getReturnSite(*it) )   //Handle self-loops.
        {
          Return loop(dup,dup,getReturnSym(*it),dup);
          addReturn(loop);
        }
      }

      //Duplicate call predecessor return transitions.
      const Info::Returns & pred = T_info.predTrans(orig);
      for( Info::ReturnIterator it = pred.begin(); it != pred.end(); it++ )
      {
        Return rTrans(getExit(*it),dup,getReturnSym(*it),getReturnSite(*it));
        addReturn(rTrans);
        if( orig == getReturnSite(*it) )   //Handle self-loops.
        {
          Return loop(getExit(*it),dup,getReturnSym(*it),dup);
          addReturn(loop);
        }
      }

      //Duplicate return site return transitions.
      const Info::Returns & ret = T_info.retTrans(orig);
      for( Info::ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        Return rTrans(getExit(*it),getCallSite(*it),getReturnSym(*it),dup);
        addReturn(rTrans);
      }
    }

    /**
     *
     * @brief removes all transitions from this collection of transitions
     *
     * This method removes all transitions from this collection of
     * transitions. 
     *
     */
    void TransitionStorage::clear( )
    {
      callTrans.clear();
      internalTrans.clear();
      returnTrans.clear();
      
      T_info.clearMaps();
    }
    
    /**
     *
     * @brief get all call transitions in this collection of transitions 
     *
     * @return all call transitions in this collection of transitions
     *
     */
    const TransitionStorage::Calls & TransitionStorage::getCalls() const
    {
      return callTrans;
    }
    
    /**
     *
     * @brief get all internal transitions in this collection of transitions 
     *
     * @return all internal transitions in the collection of transitions 
     *
     */
    const TransitionStorage::Internals & TransitionStorage::getInternals() const
    {
      return internalTrans;
    }
    
    /**
     *
     * @brief get all return transitions in the collection of transitions 
     *
     * @return all return transitions in the collection of transitions 
     *
     */
    const TransitionStorage::Returns & TransitionStorage::getReturns() const
    {
      return returnTrans;
    }

    /**
     *
     * @brief add a call transition to the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the call transition already exists in the NWA
     *
     */
    bool TransitionStorage::addCall( State from, Symbol sym, State to )
    {
      Call ct = Call(from,sym,to);
      return addCall(ct);
    }
      
    /**
     *
     * @brief add the given call transition to the collection of transitions
     *
     * @param - addTrans: the call transition to add to the collection of transitions
     * @return false if the call transition already exists in the collection
     *
     */ 
    bool TransitionStorage::addCall( const Call & addTrans )
    {     
      bool added = callTrans.insert(addTrans).second;

      if (added) {
        T_info.addCall(addTrans);
      }

      return added;
    }

    /**
     *
     * @brief add an internal transition to the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the internal transition already exists in the NWA
     *
     */
    bool TransitionStorage::addInternal( State from, Symbol sym, State to )
    {
      Internal it(from,sym,to);
      return addInternal(it);
    }
    
    /**
     *
     * @brief add the given internal transition to the collection of transitions
     *
     * @param - addTrans: the internal transition to add to the collection of transitions
     * @return false if the internal transition already exists in the collection
     *
     */
    bool TransitionStorage::addInternal( const Internal & addTrans )
    {
      bool added = internalTrans.insert(addTrans).second;

      if(added) {
        T_info.addIntra(addTrans);
      }

      return added;
    }
    
    /**
     *
     * @brief add a return transition to the NWA
     *
     * @param - from: the state the edge departs from
     * @param - pred: the state from which the call was initiated  
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the return transition already exists in the NWA
     *
     */
    bool TransitionStorage::addReturn( State from, State pred, Symbol sym, State to )
    {
      Return rt(from,pred,sym,to);
      return addReturn(rt);
    }

    /**
     *
     * @brief add the given return transition to the collection of transitions
     *
     * @param - addTrans: the return transition to add to the collection of transitions
     * @return false if the return transition already exists in the collection
     *
     */
    bool TransitionStorage::addReturn( const Return & addTrans )
    {
      bool added = returnTrans.insert(addTrans).second;

      if (added) {
        T_info.addRet(addTrans);
      }

      return added;
    }
      
    /**
     *
     * @brief add all transitions in the given collection of transitions to this
     *        collection of transitions
     *
     * @param - addTransitionStorage: the collection of transitions to add to this 
     *          collection of transitions
     *
     */
    void TransitionStorage::addAllTrans( TransitionStorage addTransSet )
    {   
      //Add call transitions.
      for(CallIterator it = addTransSet.beginCall(); 
          it != addTransSet.endCall(); it ++ )
      {
        addCall(*it);
      }

      //Add internal transitions.
      for(InternalIterator it = addTransSet.beginInternal(); 
          it != addTransSet.endInternal(); it ++ )
      {
        addInternal(*it);
      }

      //Add return transitions.
      for(ReturnIterator it = addTransSet.beginReturn(); 
          it != addTransSet.endReturn(); it ++ )
      {
        addReturn(*it);
      }
    }

    /**
     *
     * @brief remove a call transition from the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the call transition does not exist in the NWA
     *
     */
    bool TransitionStorage::removeCall( State from, Symbol sym, State to )
    {
      Call ct(from,sym,to);
      return removeCall(ct);
    }
      
    /**
     *
     * @brief remove the given call transition from this collection of 
     *        transitions
     *
     * @param - removeTrans: the call transition to remove from the collection
     * @return false if the given call transition does not exist in the 
     *          collection, true otherwise.
     *
     */
    bool TransitionStorage::removeCall( const Call & removeTrans )
    {
      size_t erased = callTrans.erase(removeTrans);
      if (erased > 0) {
        T_info.removeCall(removeTrans);
      }

      return erased > 0;
    }

    /**
     *
     * @brief remove an internal transition from the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the internal transition does not exist in the NWA
     *
     */
    bool TransitionStorage::removeInternal( State from, Symbol sym, State to )
    {
      Internal it(from,sym,to);
      return removeInternal(it);
    }
    
    /**
     *
     * @brief remove the given internal transition from the collection of 
     *        transitions
     *
     * @param - removeTrans: the internal transition to remove from the collection
     * @return false if the given internal transition does not exist in the 
     *          collection, true otherwise.
     *
     */
    bool TransitionStorage::removeInternal( const Internal & removeTrans )
    {     
      size_t erased = internalTrans.erase(removeTrans);
      if (erased > 0) {
        T_info.removeIntra(removeTrans);
      }

      return erased > 0;
    }

    /**
     *
     * @brief remove a return transition from the NWA
     *
     * @param - from: the state the edge departs from
     * @param - pred: the state from which the call was initiated  	   
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to 
     * @return false if the return transition does not exist in the NWA
     *
     */
    bool TransitionStorage::removeReturn( State from, State pred, Symbol sym, State to )
    {
      Return rt(from,pred,sym,to);
      return removeReturn(rt);
    }
    
    /**
     *
     * @brief remove the given return transition from the collection of 
     *        transitions
     *
     * @param - removeTrans: the return transition to remove from the collection
     * @return false if the given return transition does not exist in the 
     *          collection, true otherwise.
     *
     */
    bool TransitionStorage::removeReturn( const Return & removeTrans )
    {
      size_t erased = returnTrans.erase(removeTrans);
      if (erased > 0) {
        T_info.removeRet(removeTrans);
      }

      return erased > 0;
    }
     
    /**
     * 
     * @brief test whether the given call transition is in this collection of 
     *        transitions 
     *
     * @param - from: the state the edge departs from  
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return true if the given call transition is in this collection of 
     *          transitions
     *
     */
    bool TransitionStorage::isCall( State from, Symbol sym, State to ) const
    {
      Call ct(from,sym,to);
      return isCall(ct);
    }

    /**
     *
     * @brief test whether the given call transition is in this collection of 
     *        transitions 
     *
     * @param - trans: the call transition to check
     * @return true if the given call transition is in this collection of 
     *          transitions
     *
     */
    bool TransitionStorage::isCall( const Call & trans ) const
    {
      Calls const & outgoing = T_info.callTrans(trans.first);
      return outgoing.count(trans) > 0;
    }

    /**
     *
     * @brief test whether the given internal transition is in this collection of 
     *        transitions 
     *
     * @param - from: the state the edge departs from  
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to 
     * @return true if the given internal transition is in this collection of 
     *          transitions associated with the NWA
     *
     */
    bool TransitionStorage::isInternal( State from, Symbol sym, State to ) const
    {
      Internal it(from,sym,to);
      return isInternal(it);
    }
    
    /**
     *
     * @brief test whether the given internal transition is in this collection of 
     *        transitions 
     *
     * @param - trans: the internal transition to check
     * @return true if the given internal transition is in this collection of 
     *          transitions associated with the NWA
     *
     */
    bool TransitionStorage::isInternal( const Internal & trans ) const
    {
      Internals const & outgoing = T_info.fromTrans(trans.first);
      return (outgoing.count(trans) > 0);
    }

    /**
     *
     * @brief test whether the given return transition is in this collection of 
     *        transitions 
     *
     * @param - from: the state the edge departs from
     * @param - pred: the state from which the call was initiated  	   
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return true if the given return transition is in this collection of 
     *          transitions
     *
     */
    bool TransitionStorage::isReturn( State from, State pred, Symbol sym, State to ) const
    {
      Return rt(from,pred,sym,to);
      return isReturn(rt);
    }
    
    /**
     *
     * @brief test whether the given return transition is in this collection of 
     *        transitions 
     *
     * @param - trans: the return transition to check
     * @return true if the given return transition is in this collection of 
     *          transitions
     *
     */
    bool TransitionStorage::isReturn( const Return & trans ) const
    {
      Returns const & outgoing = T_info.exitTrans(trans.first);
      return (outgoing.count(trans) > 0);
    } 
         
    //Utilities	

    /**
     *
     * @brief print the collection of transitions
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    std::ostream & TransitionStorage::print( std::ostream & o ) const
    {    
      //Print call transitions.
      o << "Delta_c: {\n  ";
      bool first = true;
      for( CallIterator cit = callTrans.begin(); cit != callTrans.end(); cit++, first=false )
      {
        if( !first )
          o << ", \n  ";
        o << "(";
        printKey(o,getCallSite(*cit));
        o << " (=" << getCallSite(*cit) << ") ";
        o << ", ";
        printKey(o,getCallSym(*cit));
        o << ", "; 
        printKey(o,getEntry(*cit));
        o << " (=" << getEntry(*cit) << ") ";
        o << ")";
      }
      o << "\n}\n";

      //Print internal transitions.
      o << "Delta_i:  {\n  ";
      first = true;
      for(InternalIterator iit = internalTrans.begin();
          iit != internalTrans.end(); iit++, first=false )
      {
        if( !first )
          o << ",\n  ";
        o << "(";
        printKey(o,getSource(*iit));
        o << " (=" << getSource(*iit) << ") ";
        o << ", ";
        printKey(o,getInternalSym(*iit));
        o << ", ";
        printKey(o,getTarget(*iit));
        o << " (=" << getTarget(*iit) << ") ";
        o << ")";
      }
      o << "\n}\n";

      //Print return transitions.
      o << "Delta_r: {\n  ";
      first = true;
      for(ReturnIterator rit = returnTrans.begin();
          rit != returnTrans.end(); rit++, first = false )
      {
        if( !first )
          o << ",\n  ";
        o << "(";
        printKey(o,getExit(*rit));
        o << " (=" << getExit(*rit) << ") ";
        o << ", ";
        printKey(o,getCallSite(*rit));
        o << " (=" << getCallSite(*rit) << ") ";
        o << ", "; 
        printKey(o,getReturnSym(*rit));
        o << ", ";
        printKey(o,getReturnSite(*rit));
        o << " (=" << getReturnSite(*rit) << ") ";
        o << ")";
      }
      o << "\n}\n";
      
      return o;
    }

    /*
     * 
     * @brief print the collection of transitions in dot format
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    std::ostream & TransitionStorage::print_dot( std::ostream & o) const
    {
#if 0
      // TODO: make this work. Or not. -Evan 3/4/11
      // it won't compile. Was never instantiated in templated version
      //Print call transitions.
      o << "//Delta_c: \n";
      for (CallIterator cit = callTrans.begin(); cit != callTrans.end(); cit++)
      {
        cit->first.print(o << "\"") << "\"";
        o << "->";
        cit->third.print(o << "\"") << "\"";
        o << "[";
        o << " label=\"";
        cit->second.print(o);
        o << "\"";
        o << " color=green";
        o << "];\n";
      }
      o << " \n";

      //Print internal transitions.
      o << "// Delta_i:\n" ;

      for (InternalIterator iit = callTrans.begin(); iit != callTrans.end(); iit++)
      {
        iit->first.print(o << "\"") << "\"";
        o << "->";
        iit->third.print(o << "\"") << "\"";
        o << "[";
        o << " label=\"";
        iit->second.print(o);
        o << "\"";
        o << "];\n";
      }
      o << " \n";

      //Print return transitions.
      o << "// Delta_r:\n";
      for (ReturnIterator rit = returnTrans.begin(); rit != returnTrans.end(); rit++)
      {
        //dummy
        o << "\"" << std::dec << rit->second << std::dec << ", " << rit->first << "\"";
        o <<"[ shape=box ];\n";

        // exit to dummy
        rit->first.print(o << "\"") << "\"";
        o << "->";
        o << "\"" << rit->second <<", " << rit->first << "\"";
        o << "[";
        o << " label=\"";
        rit->third.print(o);
        o << "\"";
        o << " color=red";
        o << "];\n";

        // call to dummy
        rit->second.print(o << "\"") << "\"";
        o << "->";
        o << "\"" << rit->second <<", " << rit->first << "\"";
        o << "[";
        o << " color=blue";
        o << "];\n";

        //dummy to ret
        o << "\"" << rit->second <<", " << rit->first << "\"";
        o << "->";
        rit->fourth.print(o << "\"") << "\"";
        o << "[ style=dotted ];\n";

      }
      o << "\n";
#endif
      return o;
      
    }

    /**
     *
     * @brief tests whether this collection of transitions is equivalent 
     *        to the collection of transitions 'other'
     *
     * @param - other: the TransitionStorage to compare this TransitionStorage to
     * @return true if this TransitionStorage is equivalent to the TransitionStorage 'other'
     *
     */
    bool TransitionStorage::operator==( const TransitionStorage & other ) const
    {
      return ( (callTrans == other.callTrans) &&
               (internalTrans == other.internalTrans) &&
               (returnTrans == other.returnTrans) );
    }

    /**
     *
     * @brief provides access to the call transitions in the collection 
     *        through an iterator
     *
     * @return the starting point of an iterator through the call transitions
     *          in the collection of transitions
     *
     */  
    TransitionStorage::CallIterator TransitionStorage::beginCall() const
    {
      return callTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     *        through an iterator
     *
     * @return the starting point of an iterator through the internal transitions
     *          in the collection of transitions
     *
     */
    TransitionStorage::InternalIterator TransitionStorage::beginInternal() const
    {
      return internalTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     *        through an iterator
     *
     * @return the starting point of an iterator through the return transitions
     *          in the collection of transitions
     *
     */
    TransitionStorage::ReturnIterator TransitionStorage::beginReturn() const
    {
      return returnTrans.begin();
    }
      
    /**
     *
     * @brief provides access to the call transitions in the collection 
     *        through an iterator
     *
     * @return the exit point of an iterator through the call transitions in
     *          the collection of transitions
     *
     */
    TransitionStorage::CallIterator TransitionStorage::endCall() const
    {
      return callTrans.end();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     *        through an iterator
     *
     * @return the exit point of an iterator through the internal transitions in
     *          the collection of transitions
     *
     */
    TransitionStorage::InternalIterator TransitionStorage::endInternal() const
    {
      return internalTrans.end();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     *        through an iterator
     *
     * @return the exit point of an iterator through the return transitions in
     *          the collection of transitions
     *
     */
    TransitionStorage::ReturnIterator TransitionStorage::endReturn() const
    {
      return returnTrans.end();
    }
    
    /**
     *
     * @brief returns the number of call transitions in this collection of
     *        transitions
     *
     * @return the number of call transitions in this collection of transitions
     *
     */
    size_t TransitionStorage::sizeCall( ) const
    {
      return callTrans.size();
    }
        
    /**
     *
     * @brief returns the number of internal transitions in this collection of
     *        transitions 
     *
     * @return the number of internal transitions in this collection of transitions
     *
     */
    size_t TransitionStorage::sizeInternal( ) const
    {
      return internalTrans.size();
    }
        
    /**
     *
     * @brief returns the number of return transitions in this collection of
     *        transitions
     *
     * @return the number of return transitions in this collection of transitions
     *
     */
    size_t TransitionStorage::sizeReturn( )  const
    {
      return returnTrans.size();
    }
        
    /**
     *
     * @brief returns the total number of transitions (call, internal, and return) in 
     * this collection of transitions
     *
     * @return the total number of transitions in this collection of transitions
     *
     */
    size_t TransitionStorage::size( ) const
    {
      return (sizeCall() + sizeInternal() + sizeReturn());
    }
    
    /**
     * 
     * @brief returns all outgoing internal transitions for the given state
     *
     * @param - state: the source state
     * @return the set of all outgoing internal transitions for the given state
     *
     */
    const TransitionStorage::Internals & TransitionStorage::getTransFrom( State state ) const
    {
      return T_info.fromTrans( state );
    }
    
    /**
     * 
     * @brief retuans all incoming internal transitions for the given state
     *
     * @param - state: the target state
     * @return the set of all incoming internal transitions for the given state
     *
     */
    const TransitionStorage::Internals & TransitionStorage::getTransTo( State state ) const
    {
      return T_info.toTrans( state );
    }
    
    /**
     * 
     * @brief returns all call transitions for the given call site
     *
     * @param - state: the call site
     * @return the set of all call transitions for the given call site
     *
     */
    const TransitionStorage::Calls & TransitionStorage::getTransCall( State state ) const
    {
      return T_info.callTrans( state );
    }
    
    /**
     * 
     * @brief returns all call transitions for the given entry point
     *
     * @param - state: the entry point
     * @return the set of all call transitions for the given entry point
     *
     */
    const TransitionStorage::Calls & TransitionStorage::getTransEntry( State state ) const
    {
      return T_info.entryTrans( state );
    }
    
    /**
     * 
     * @brief returns all return transitions for the given exit point
     *
     * @param - state: the exit point
     * @return the set of all return transitions for the given exit point
     *
     */
    const TransitionStorage::Returns & TransitionStorage::getTransExit( State state ) const
    {
      return T_info.exitTrans( state );
    }
    
    /**
     * 
     * @brief returns all return transitions for the given call predecessor
     *
     * @param - state: the call predecessor
     * @return the set of all return transitions for the given call predecessor
     *
     */
    const TransitionStorage::Returns & TransitionStorage::getTransPred( State state ) const
    {
      return T_info.predTrans( state );
    }
    
    /**
     * 
     * @brief returns all return transitions for the given return site
     *
     * @param - state: the return site
     * @return the set of all return transitions for the given return site
     *
     */
    const TransitionStorage::Returns & TransitionStorage::getTransRet( State state ) const
    {
      return T_info.retTrans( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the source of any internal 
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the source of some internal 
     *          transition, false otherwise
     *
     */
    bool TransitionStorage::isFrom( State state ) const
    {
      return T_info.isFrom( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the target of any internal
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the target of some internal
     *          transition, false otherwise
     *
     */
    bool TransitionStorage::isTo( State state ) const
    { 
      return T_info.isTo( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the call site of any call 
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the call site of some call
     *          transition, false otherwise
     *
     */
    bool TransitionStorage::isCall( State state ) const
    {
      return T_info.isCall( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the entry point of any call
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the entry point of some call 
     *          transition, false otherwise
     *
     */
    bool TransitionStorage::isEntry( State state ) const
    {
      return T_info.isEntry( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the exit point of any return
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the exit point of some return
     *          transition, false otherwise
     *
     */
    bool TransitionStorage::isExit( State state ) const
    {
      return T_info.isExit( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the call predecessor for any
     *        return transition
     *
     * @param - state: the state to test
     * @return true if the given state is the call predecessor of some return
     *          transition, false otherwise
     *
     */
    bool TransitionStorage::isPred( State state ) const
    {
      return T_info.isPred( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the return site of any return
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the return site of some return 
     *          transition, false otherwise
     *
     */
    bool TransitionStorage::isRet( State state ) const
    {
      return T_info.isRet( state );
    }

    /** 
     *   
     * @brief removes all transitions involving the given state  
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    bool TransitionStorage::removeTransWith( State state )
    {
      bool foundCall = removeCallTransWith(state);
      bool foundInt = removeInternalTransWith(state);
      bool foundRet = removeReturnTransWith(state);

      return foundCall || foundInt || foundRet;
    }
      
    /** 
     *
     * @brief removes all call transitions to or from the given state  
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    bool TransitionStorage::removeCallTransWith( State state )
    {
      Calls outgoing = T_info.callTrans(state);
      Calls incoming = T_info.entryTrans(state);

      //Remove the transitions.
      for( CallIterator rit = outgoing.begin(); rit != outgoing.end(); rit++ )
      {
        removeCall(*rit);
      }

      //Remove the transitions.
      for( CallIterator rit = incoming.begin(); rit != incoming.end(); rit++ )
      {
        removeCall(*rit);
      }
      
      return (outgoing.size() > 0) || (incoming.size() > 0);
    }
  
    /** 
     *
     * @brief removes all internal transitions to or from the given state
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    bool TransitionStorage::removeInternalTransWith( State state )
    {
      Internals outgoing = T_info.fromTrans(state);
      Internals incoming = T_info.toTrans(state);

      //Remove the transitions.
      for( InternalIterator rit = outgoing.begin(); rit != outgoing.end(); rit++ )
      {
        removeInternal(*rit);
      }

      //Remove the transitions.
      for( InternalIterator rit = incoming.begin(); rit != incoming.end(); rit++ )
      {
        removeInternal(*rit);
      }
      
      return (outgoing.size() > 0) || (incoming.size() > 0);
    }
  
    /** 
     *
     * @brief removes all return transitions to or from the given state
     *        as well as return transitions corresponding to calls from
     *        that state
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    bool TransitionStorage::removeReturnTransWith( State state )
    {
      Returns outgoing = T_info.exitTrans(state);
      Returns incoming = T_info.retTrans(state);
      Returns predgoing = T_info.predTrans(state);

      //Remove the transitions.
      for( ReturnIterator rit = outgoing.begin(); rit != outgoing.end(); rit++ )
      {
        removeReturn(*rit);
      }

      //Remove the transitions.
      for( ReturnIterator rit = incoming.begin(); rit != incoming.end(); rit++ )
      {
        removeReturn(*rit);
      }

      for( ReturnIterator rit = predgoing.begin(); rit != predgoing.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return (outgoing.size() > 0) || (incoming.size() > 0) || (predgoing.size() > 0);
    }

    /** 
     *
     * @brief removes all transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    bool TransitionStorage::removeTransSym( Symbol sym )
    {
      bool foundCall = removeCallTransSym(sym);
      bool foundInternal = removeInternalTransSym(sym);
      bool foundRet = removeReturnTransSym(sym);
      return foundCall || foundInternal || foundRet;
    }
    
    /** 
     *
     * @brief removes all call transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    bool TransitionStorage::removeCallTransSym( Symbol sym )
    {
      Calls removeTrans;

      //Find transitions to remove.
      for( CallIterator cit = callTrans.begin(); cit != callTrans.end(); cit++ )
      {
        if( getCallSym(*cit) == sym )
          removeTrans.insert(*cit);
      }

      //Remove transitions.
      for( CallIterator rit = removeTrans.begin(); rit != removeTrans.end(); rit++ )
      {
        removeCall(*rit);
      }
      
      return removeTrans.size() > 0;  
    }
    
    /** 
     *
     * @brief removes all internal transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    bool TransitionStorage::removeInternalTransSym( Symbol sym )
    {
      Internals removeTrans;

      //Find transitions to remove.
      for( InternalIterator iit = internalTrans.begin(); iit != internalTrans.end(); iit++ )
      {
        if( getInternalSym(*iit) == sym )
          removeTrans.insert(*iit);
      }

      //Remove transitions.
      for( InternalIterator rit = removeTrans.begin(); rit != removeTrans.end(); rit++ )
      {
        removeInternal(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /** 
     *
     * @brief removes all return transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    bool TransitionStorage::removeReturnTransSym( Symbol sym )
    {
      Returns removeTrans;

      //Find transitions to remove.
      for( ReturnIterator rit = returnTrans.begin(); rit != returnTrans.end(); rit++ )
      {
        if( getReturnSym(*rit) == sym )
          removeTrans.insert(*rit);
      }

      //Remove transitions.
      for( ReturnIterator rit = removeTrans.begin(); rit != removeTrans.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /**
     * @brief test if there exists a call transition with the given from state 
     *        and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the call transition
     * @param - sym: the desired symbol for the call transition
     * @return true if there exists a call transition with the given from state and
     *          symbol in this collection of transitions 
     *
     */
    bool TransitionStorage::callExists( State from, Symbol sym ) const
    {
      Calls const & outgoing = T_info.callTrans(from);

      for( CallIterator cit = outgoing.begin(); cit != outgoing.end(); cit++ )
      {
        if( getCallSym(*cit) == sym )
          return true;
      }  
      return false;    
    }
    
    /**
     * Q: how can this be optimized?
     * @brief provides access to all call transitions with the given from state
     *        and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the call transitions
     * @param - sym: the desired symbol for the call transitions
     * @return the set of call transitions with the given from state and symbol
     *
     */
    const TransitionStorage::Calls TransitionStorage::getCalls( State from, Symbol sym ) const 
    {
      Calls result;
      Calls const & outgoing = T_info.callTrans(from);

      for( CallIterator cit = outgoing.begin(); cit != outgoing.end(); cit++ )
      {
        if( getCallSym(*cit) == sym )
          result.insert(*cit);
      } 
      return result;
    }
    
    /**
     * Q: how can this be optimized?
     * @brief test if there exists an internal transition with the given from state 
     *        and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the internal transition
     * @param - sym: the desired symbol for the internal transition
     * @return true if there exists an internal transition with the given from state and
     *          symbol in this collection of transitions
     *
     */
    bool TransitionStorage::internalExists( State from, Symbol sym ) const
    {
      Internals const & outgoing = T_info.fromTrans(from);

      for( InternalIterator iit = outgoing.begin(); iit != outgoing.end(); iit++ )
      {
        if( getInternalSym(*iit) == sym )
          return true;    
      }     
      return false;
    }
    
    /**
     * Q: how can this be optimized?
     * @brief provides access to all internal transitions with the given from 
     *        state and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the internal transitions
     * @param - sym: the desired symbol for the internal transitions
     * @return the set of internal transitions with the given from state and symbol
     *
     */
    const TransitionStorage::Internals TransitionStorage::getInternals( State from, Symbol sym ) const
    {
      Internals result;
      Internals const & outgoing = T_info.fromTrans(from);

      for( InternalIterator iit = outgoing.begin(); iit != outgoing.end(); iit++ )
      {
        if( getInternalSym(*iit) == sym )
          result.insert(*iit);
      } 
      return result;
    }
    
    /**
     * TODO this is redundant
     * @brief provides access to all internal transitions with the given 
     *        from state in this collection of transitions
     *
     * @param - from: the desired from state for the internal transitions
     * @return the set of internal transitions with the given from state
     *
     */
    const TransitionStorage::Internals TransitionStorage::getInternalsFrom( State from ) const
    {
      return T_info.fromTrans(from);
    }


    /**
     * Q: how can this be optimized?
     * @brief test if there exists a return transition with the given from state, 
     *        predecessor state, and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the return transition
     * @param - pred: the desired predecessor state for the return transition
     * @param - sym: the desired symbol for the return transition
     * @return true if there exists a return transition with the given from state and
     *          symbol in this collection of transitions
     *
     */
    bool TransitionStorage::returnExists( State from, State pred, Symbol sym ) const
    {
      Returns const & outgoing = T_info.exitTrans(from);

      for( ReturnIterator rit = outgoing.begin(); rit != outgoing.end(); rit++ )
      {
        if( (getCallSite(*rit) == pred) && (getReturnSym(*rit) == sym) )
          return true;      
      }     
      return false;
    }   
    
    /**
     * Q: how can this be optimized?
     * @brief provides access to all return transitions with the given from
     *        state and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the return transitions
     * @param - sym: the desired symbol for the return transitions
     * @return the set of return transitions with the given from state and symbol
     *
     */
    const TransitionStorage::Returns TransitionStorage::getReturns( State from, Symbol sym ) const
    {
      Returns result;
      Returns const & outgoing = T_info.exitTrans(from);

      for( ReturnIterator rit = outgoing.begin(); rit != outgoing.end(); rit++ )
      {
        if( (getExit(*rit) == from) && (getReturnSym(*rit) == sym) )
          result.insert(*rit);
      } 
      return result;
    }


  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

