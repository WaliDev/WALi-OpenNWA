/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/nwa/NWA.hpp"

namespace wali
{	
  namespace nwa
  {
    //
    // Methods
    //

    //Constructors and Destructor
    
    NWA::NWA( )
    {
      trans = new Trans();
    }
    
    NWA::NWA( NWA & other )
    {
      operator=(other);
    }
    
    NWA & NWA::operator=( NWA & other )
    {
      trans = new Trans();
      
      trans->addAllSymbols(other.trans->getSymbols());
      trans->addAllStates(*other.trans);
      trans->addAllInitialStates(*other.trans);
      trans->addAllFinalStates(*other.trans);
      trans->addAllTrans(*other.trans);
      
      return *this;
    }
   
    NWA::~NWA( )
    {         
      trans->~Trans();    
    }
    
  
    //State Accessors

    //All States
    /**
     *
     * @brief get all states in the state set of the NWA
     *
     * @return set of Key values for all states associated with the NWA
     *
     */
    const StateSet<NWA::States> & NWA::getStates( )
    {
      return trans->getStates();
    }
   
    /**
     *
     * @brief test if a state with the given name is a state of the NWA
     *
     * @param name of the state to check
     * @return true if the state with the given name is a state of the NWA
     *
     */
    bool NWA::isState( Key name )
    {
      return trans->isState(name);
    }
   
    /**
     *
     * @brief add a state with the given name to the NWA
     *
     * @param name of the state to add
     * @return false if the state already exists in the NWA
     *
     */
    bool NWA::addState( Key name )
    {
      return trans->addState(name);
    }
   
    /**
     *
     * @brief remove the state with the given name from the NWA
     *
     * @param name of the state to remove
     * @return false if the state does not exist in the NWA
     *
     */
    bool NWA::removeState( Key name )
    {
      return trans->removeState(name);
    }
   
    /**
     *
     * @brief remove all states from the NWA
     *
     */
    void NWA::clearStates( )
    {
      trans->clearStates();
    }
  
  
    //Initial States
    /**
     *
     * @brief get all states in the initial state set
     *
     * @return set of Key values for all inital states associated with 
     * the NWA
     *
     */
    const StateSet<NWA::States> & NWA::getInitialStates( )
    {
      return trans->getInitialStates();
    }
  
    /**
     *
     * @brief test if a state with the given name is an initial state
     *
     * @param name of the state to check
     * @return true if the state with the given name is an initial state
     *
     */
    bool NWA::isInitialState( Key name )
    {
      return trans->isInitialState(name); 
    }
  
    /**
     *
     * @brief make the state with the given name an initial state
     *
     * @param name of the state to add to initial state set
     * @return false if the state already exists in the initial state set
     * of the NWA
     *
     */
    bool NWA::addInitialState( Key name )
    {
      return trans->addInitialState(name);
    }
  
    /**
     *
     * @brief remove the state with the given name from the initial state 
     * set of the NWA
     *
     * @param name of the state to remove from the initial state set
     * @return false if the state does not exist in the initial state set 
     * of the NWA
     *
     */
    bool NWA::removeInitialState( Key name )
    {
      return trans->removeInitialState(name);
    }
  
    /**
     *
     * @brief remove all states from the initial state set of the NWA
     *
     */
    void NWA::clearInitialStates( )
    {
      trans->clearInitialStates();
    }
  
  
    //Final States
    /**
     *
     * @brief get all final states
     *
     * @return set of Key values for all final states associated with 
     * the NWA
     *
     */
    const StateSet<NWA::States> & NWA::getFinalStates( )
    {
      return trans->getFinalStates();
    }
  
    /**
     *
     * @brief test if a state with the given name is a final state
     *
     * @param name of the state to check
     * @return true if the state with the given name is a final state
     *
     */
    bool NWA::isFinalState( Key name )
    {
      return trans->isFinalState(name);
    }
  
    /**
     *
     * @brief make the state with the given name a final state
     *
     * @param name of the state to add to final state set
     * @return false if the state already exists in the final state set
     * of the NWA
     *
     */
    bool NWA::addFinalState( Key name )
    {
      return trans->addFinalState(name);
    }
  
    /**
     *
     * @brief remove the state with the given name from the final state 
     * set of the NWA
     *
     * @param name of the state to remove from the final state set
     * @return false if the state does not exist in the final state set 
     * of the NWA
     *
     */
    bool NWA::removeFinalState( Key name )
    {
      return trans->removeFinalState(name);
    }
   
    /**
     *
     * @brief remove all states from the final state set of the NWA
     *
     */
    void NWA::clearFinalStates( )
    {
      trans->clearFinalStates();
    }
    
    
    
    //Symbol Accessors

    /**
     *
     * @brief get all symbols in the symbol set of the NWA
     *
     * @return set of Key values for all symbols associated with the NWA
     *
     */
    const NWA::Symbols & NWA::getSymbols( )
    {
        return trans->getSymbols();
    }
  
    /**
     *
     * @brief test if the given symbol is a symbol of the NWA
     *
     * @param the symbol to check
     * @return true if the given symbol is a symbol of the NWA
     *
     */
    bool NWA::isSymbol( Key sym )
    {
      return trans->isSymbol(sym);
    }
   
    /**
     *
     * @brief add the given symbol to the NWA
     *
     * @param symbol to add
     * @return false if the symbol is already associated with the NWA
     *
     */
    bool NWA::addSymbol( Key sym )
    {
      return trans->addSymbol(sym);
    }
   
    /**
     *
     * @brief remove the given symbol from the NWA
     *
     * @param the symbol to remove
     * @return false if the symbol is no associated with the NWA
     *
     */
    bool NWA::removeSymbol( Key sym )
    {
      return trans->removeSymbol(sym);
    }
   
    /**
     *
     * @brief remove all symbols associated with the NWA
     *
     */
    void NWA::clearSymbols( )
    {
      trans->clearSymbols();
    }
    
  
  
    //Transition Accessors

    /**
     *
     * @brief remove all transitions from the NWA
     *
     */
    void NWA::clearTrans( )
    {
      trans->clear();
    }
  
  
    //Call Transitions
    /**
     *
     * @brief get all call transitions in the transition set of the NWA.
     *
     * @return set of call transitions associated with the NWA
     * 
     */
    const NWA::Calls & NWA::getCallTrans( )
    {
      return trans->getCalls();
    }
 
    /**
     *
     * @brief add a call transition to the NWA
     *
     * @param from: name of the State the edge departs from
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the State the edge arrives to  
     * @return false if the call transition already exists in the NWA
     *
     */
    bool NWA::addCallTrans( Key from, Key sym, Key to )
    {
      KeyTriple ct = KeyTriple(from,sym,to);
      return trans->addCall(ct);
    }
  
    /**
     *
     * @brief add a call transition to the NWA
     *
     * @param the call transition to add
     * @return false if the call transition already exists in the NWA
     *
     */
    bool NWA::addCallTrans( KeyTriple * ct )
    {
      return trans->addCall(*ct);
    }
  
    /**
     *
     * @brief remove the call transition with the given edge and label 
     * information from the NWA
     *
     * @param from: name of the State the edge departs from
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the State the edge arrives to  
     * @return false if the call transition does not exist in the NWA
     *
     */
    bool NWA::removeCallTrans( Key from, Key sym, Key to )
    {
      KeyTriple ct = KeyTriple(from,sym,to);
      return trans->removeCall(ct);
    }
  
    /**
     *
     * @brief remove a call transition from the NWA
     *
     * @param the call transition to remove
     * @return false if the call transition does not exist in the NWA
     *
     */
    bool NWA::removeCallTrans( KeyTriple * ct )
    {
      return trans->removeCall(*ct);
    }
  
    /**
     *
     * @brief get all internal transitions in the transition set of the 
     * NWA.
     *
     * @return set of internal transitions associated with the NWA
     *
     */
    const NWA::Internals & NWA::getInternalTrans( )
    {
      return trans->getInternals();
    }
  
    /**
     *
     * @brief add an internal transition to the NWA
     *
     * @param from: name of the state the edge departs from
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the state the edge arrives to  
     * @return false if the internal transition already exists in the NWA
     *
     */
    bool NWA::addInternalTrans( Key from, Key sym, Key to )
    {
      KeyTriple it = KeyTriple(from,sym,to);
      return trans->addInternal(it);
    }
 
    /**
     *
     * @brief add an internal transition to the NWA
     *
     * @param internal transition to add to the NWA
     * @return false if the internal transition already exists in the NWA
     *
     */
    bool NWA::addInternalTrans( KeyTriple * it )
    {
      return trans->addInternal(*it);
    }
  
    /**
     *
     * @brief remove the internal transition with the given edge and 
     * label information from the NWA
     * 
     * @param from: name of the state the edge departs from
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the state the edge arrives to  
     * @return false if the internal transition does not exist in the NWA
     *
     */
    bool NWA::removeInternalTrans( Key from, Key sym, Key to )
    {
      KeyTriple it = KeyTriple(from,sym,to);
      return trans->removeInternal(it);
    }
  
    /**
     *
     * @brief remove an internal transition from the NWA
     *
     * @param the internal transition to remove
     * @return false if the internal transition does not exist in the NWA
     *
     */
    bool NWA::removeInternalTrans( KeyTriple * it )
    {
      return trans->removeInternal(*it);
    }
  
    /**
     *
     * @brief get all return transitions in the transition set of the NWA.
     *
     * @return set of return transitions associated with the NWA
     *
     */
    const NWA::Returns & NWA::getReturnTrans( )
    {
      return trans->getReturns();
    }
   
    /**
     *
     * @brief add a return transition to the NWA
     *
     * @param from: name of the state the edge departs from
     * @param pred: name of the state from which the call was initiated  
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the state the edge arrives to  
     * @return false if the return transition already exists in the NWA
     *
     */
    bool NWA::addReturnTrans( Key from, Key pred, Key sym, Key to )
    {
      KeyQuad rt = KeyQuad(from,pred,sym,to);
      return trans->addReturn(rt);
    }
   
    /**
     *
     * @brief add a return transition to the NWA
     *
     * @param return transition to add to the NWA
     * @return false if the return transition already exists in the NWA
     *
     */
    bool NWA::addReturnTrans( KeyQuad * rt )
    {
      return trans->addReturn(*rt);
    }
  
    /**
     *
     * @brief remove the return transition with the given edge and label 
     * information from the NWA
     *
     * @param from: name of the state the edge departs from
     * @param pred: name of the state from which the call was initiated  	   
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the state the edge arrives to  
     * @return false if the return transition does not exist in the NWA
     *
     */
    bool NWA::removeReturnTrans( Key from, Key pred, Key sym, Key to )
    {
      KeyQuad rt = KeyQuad(from,pred,sym,to);
      return trans->removeReturn(rt);
    }
   
    /**
     *
     * @brief remove a return transition from the NWA
     *
     * @param the return transition to remove
     * @return false if the return transition does not exist in the NWA
     *
     */
    bool NWA::removeReturnTrans( KeyQuad * rt )
    {
      return trans->removeReturn(*rt);
    }
  
    //Building NWAs

    /**
     *
     * @brief constructs the NWA resulting from the union of this NWA and
     * the NWA 'other'
     *
     * @param the NWA with which to union this NWA
     * @return the NWA which is the union of this NWA and the NWA 'other'
     *
     */
    NWA * NWA::unionNWA( NWA other )
    {
      NWA * newNWA = new NWA();
      
      //Generate new states, adding them to the state set as well as
      //to the initial state set and final state set as needed.
      Key newKey;
      for( States::iterator fit = this->trans->beginStates();
            fit != this->trans->endStates(); fit++ )
      {
        for( States::iterator sit = other.trans->beginStates();
              sit != other.trans->endStates(); sit++ )
        {
          if( (*fit == WALI_BAD_KEY) && (*sit == WALI_BAD_KEY) )
            newKey = WALI_BAD_KEY;         
          else
            newKey = wali::getKey(*fit,*sit);
          
          newNWA->addState(newKey);
          
          if( this->isInitialState(*fit) && other.isInitialState(*sit) )
            newNWA->addInitialState(newKey);
          if( this->isFinalState(*fit) || other.isFinalState(*sit) )
            newNWA->addFinalState(newKey);      
        }
      }
      
      //Generate call transitions.
      for( Trans::callIterator fcit = this->trans->beginCall();
            fcit != this->trans->endCall(); fcit++ )
      {
        for( Trans::callIterator scit = other.trans->beginCall();
            scit != other.trans->endCall(); scit++ )
        {
          if( fcit->second == scit->second )
            newNWA->addCallTrans( wali::getKey(fcit->first,scit->first),
                                  fcit->second, 
                                  wali::getKey(fcit->third,scit->third) );
        }
      }
      //Generate internal transitions.
      for( Trans::internalIterator fiit = this->trans->beginInternal();
            fiit != this->trans->endInternal(); fiit++ )
      {
        for( Trans::internalIterator siit = other.trans->beginInternal();
              siit != other.trans->endInternal(); siit++ )
        {
          if( fiit->second == siit->second )
            newNWA->addInternalTrans( wali::getKey(fiit->first,siit->first),
                                      fiit->second, 
                                      wali::getKey(fiit->third,siit->third) );
        }
      }
      //Generate return transitions.  
      for( Trans::returnIterator frit = this->trans->beginReturn();
            frit != this->trans->endReturn(); frit++ )
      {
        for( Trans::returnIterator srit = other.trans->beginReturn();
              srit != other.trans->endReturn(); srit++ )
        {
          if( frit->third == srit->third )
            newNWA->addReturnTrans( wali::getKey(frit->first,srit->first),
                                    wali::getKey(frit->second,srit->second), 
                                    frit->third,
                                    wali::getKey(frit->fourth,srit->fourth) );
        }
      }   
      
      return newNWA;
    }
  
    /**
     *
     * @brief constructs the NWA resulting from the concatenation of this
     * NWA and the NWA 'other'
     *
     * @param the NWA with which to concatenate this NWA
     * @return the NWA which is the concatenation of this NWA and the NWA
     * 'other'
     * 
     */
    NWA * NWA::concat( NWA other )
    {
      NWA * newNWA = new NWA();
      
      //Join the state sets
      newNWA->trans->addAllStates(*this->trans);
      newNWA->trans->addAllStates(*other.trans);
      
      //Initial states of the new NWA are the initial states of this NWA
      newNWA->trans->addAllInitialStates(*this->trans);
      
      //Idea:
      //To speed this up we could create a new state, connect all final
      //states to this state, and connect this state to all initial states.
      //Then it will take time O(m+n) instead of O(m*n) where m and n are
      //the sizes of the initial and final state sets.
      
      //Add internal epsilon transitions from the final states of the first 
      //NWA to the initial states of the second NWA.
      for( Trans::stateIterator fit = trans->beginFinalStates();
        fit != trans->endFinalStates(); fit++ )
      {
        for( Trans::stateIterator iit = other.trans->beginInitialStates();
          iit != other.trans->endInitialStates(); iit++)
        {
          KeyTriple newTrans = KeyTriple(*fit,WALI_EPSILON,*iit);
          newNWA->addInternalTrans( &newTrans );
        }
      }
      
      //Final states of the new NWA are the final states of the NWA 'other'
      newNWA->trans->addAllFinalStates(*other.trans);

      //Join the transition sets
      newNWA->trans->addAllTrans(*this->trans);
      newNWA->trans->addAllTrans(*other.trans);

      return newNWA;
    }
  
    /**
     *
     * @brief constructs the NWA resulting from performing Kleene-* on 
     * this NWA
     *
     * @return the NWA which is the result of performing Kleene-* on 
     * this NWA
     *
     */
    //TODO: check this with respect to calls/returns
    NWA * NWA::star( )
    {
      NWA * newNWA = new NWA();
      
      //Add states.
      newNWA->trans->addAllStates(*this->trans);
      newNWA->trans->addAllInitialStates(*this->trans);
      newNWA->trans->addAllFinalStates(*this->trans);
      
      //Add trans.
      newNWA->trans->addAllTrans(*this->trans);
      
      //Idea:
      //The same possible speedup as in concat applies here.
      
      //Add internal epsilon transitions from final states to initial states.
      for( Trans::stateIterator fit = trans->beginFinalStates();
        fit != trans->endFinalStates(); fit++ )
      {
        for( Trans::stateIterator iit = trans->beginInitialStates();
          iit != trans->endInitialStates(); iit++ )
        {
          KeyTriple newTrans = KeyTriple(*fit,WALI_EPSILON,*iit);
          newNWA->addInternalTrans( &newTrans );
        }
      }
  
      return newNWA;
    }
 
    /**
     *
     * @brief constructs the NWA that is the complement of this NWA
     *
     * @return the NWA which is the complement of this NWA
     *
     */
    NWA * NWA::complement( )
    {
      NWA * newNWA = new NWA();
    
      //Start with a deterministic NWA.
      if(! this->isDeterministic() )
        determinize();
      
      //The new NWA will have all the same states and initial states.
      newNWA->trans->addAllStates(*this->trans);
      newNWA->trans->addAllInitialStates(*this->trans);
      
      //The new NWA will have all the same transitions.
      newNWA->trans->addAllTrans(*this->trans);    
    
      //All states that are final states of this NWA
      //are not final states of the new NWA and all states 
      //not in the final state set of this NWA are final
      //states of the new NWA.
      StateSet<States> oldFinalStates = StateSet<States>();
      for( Trans::stateIterator fit = trans->beginFinalStates();
            fit != trans->endFinalStates(); fit++ )
        oldFinalStates.add(*fit);
      
      for( Trans::stateIterator sit = trans->beginStates();
            sit != trans->endStates(); sit++ )
      {
        if( !oldFinalStates.contains(*sit) )
          newNWA->addFinalState(*sit);
      }
          
      return newNWA;
    }
  
    /**
     * @brief constructs the NWA which is the reverse of this NWA
     *
     * This method constructs the NWA which is the reverse of this NWA.
     *
     * @return the NWA which is the reverse of this NWA
     * 
     */
    NWA * NWA::reverse()
    {
      NWA * newNWA = new NWA();
          
      //The states remain the same.
      newNWA->trans->addAllStates(*this->trans);
    
      //Swap the initial state set with the final state set.
      for( Trans::stateIterator it = this->trans->beginFinalStates();
            it != this->trans->endFinalStates(); it++ )
        newNWA->addInitialState(*it);
      for( Trans::stateIterator it = this->trans->beginInitialStates();
            it != this->trans->endInitialStates(); it++ )
        newNWA->addFinalState(*it);
      
      //Reverse all transitions.
      for( Trans::internalIterator iit = this->trans->beginInternal();
            iit != this->trans->endInternal(); iit++ )
      {
        newNWA->addInternalTrans(iit->third,iit->second,iit->first);
      }         
      
      for( Trans::returnIterator rit = this->trans->beginReturn();
            rit != this->trans->endReturn(); rit++ )
      {
        newNWA->addCallTrans(rit->third,rit->second,rit->first);
      }
      for( Trans::callIterator cit = this->trans->beginCall();
            cit != this->trans->endCall(); cit++ )
      {
        for( Trans::returnIterator rit = this->trans->beginReturn();
              rit != this->trans->endReturn(); rit++ )
        {
          if( cit->first == rit->second )
          {
            newNWA->addReturnTrans(cit->third,rit->first,
                                    cit->second,cit->first);
            break;
          } 
        }
      }
      
      return newNWA;
    }
  
    /**
     *
     * @brief constructs the NWA which is the intersection of this NWA
     * and the NWA 'other'
     *
     * @param the NWA with which to perform the intersection with this
     * NWA
     * @return the NWA which is the intersection of this NWA and the NWA
     * 'other'
     *
     */
    NWA * NWA::intersect( NWA other )
    {
      NWA * newNWA = new NWA();
      
      //Generate new states, adding them to the state set as well as
      //to the initial state set and final state set as needed.
      Key newKey;
      for( States::iterator fit = this->trans->beginStates();
            fit != this->trans->endStates(); fit++ )
      {
        for( States::iterator sit = other.trans->beginStates();
              sit != other.trans->endStates(); sit++ )
        {
          if( (*fit == WALI_BAD_KEY) && (*sit == WALI_BAD_KEY) )
            newKey = WALI_BAD_KEY;         
          else
            newKey = wali::getKey(*fit,*sit);
          
          newNWA->addState(newKey);
          
          if( this->isInitialState(*fit) && other.isInitialState(*sit) )
            newNWA->addInitialState(newKey);
          if( this->isFinalState(*fit) && other.isFinalState(*sit) )
            newNWA->addFinalState(newKey);      
        }
      }
      
      //Generate call transitions.
      for( Trans::callIterator fcit = this->trans->beginCall();
            fcit != this->trans->endCall(); fcit++ )
      {
        for( Trans::callIterator scit = other.trans->beginCall();
            scit != other.trans->endCall(); scit++ )
        {
          if( fcit->second == scit->second )
            newNWA->addCallTrans( wali::getKey(fcit->first,scit->first),
                                  fcit->second, 
                                  wali::getKey(fcit->third,scit->third) );
        }
      }
      //Generate internal transitions.
      for( Trans::internalIterator fiit = this->trans->beginInternal();
            fiit != this->trans->endInternal(); fiit++ )
      {
        for( Trans::internalIterator siit = other.trans->beginInternal();
              siit != other.trans->endInternal(); siit++ )
        {
          if( fiit->second == siit->second )
            newNWA->addInternalTrans( wali::getKey(fiit->first,siit->first),
                                      fiit->second, 
                                      wali::getKey(fiit->third,siit->third) );
        }
      }
      //Generate return transitions.  
      for( Trans::returnIterator frit = this->trans->beginReturn();
            frit != this->trans->endReturn(); frit++ )
      {
        for( Trans::returnIterator srit = other.trans->beginReturn();
              srit != other.trans->endReturn(); srit++ )
        {
          if( frit->third == srit->third )
            newNWA->addReturnTrans( wali::getKey(frit->first,srit->first),
                                    wali::getKey(frit->second,srit->second), 
                                    frit->third,
                                    wali::getKey(frit->fourth,srit->fourth) );
        }
      }   
      
      return newNWA;
    }
  
    //Using NWAs
    
    /**
     * 
     * @brief constructs the WPDS which is the result of the explicit NWA 
     * plus WPDS construction from Advanced Querying for Property Checking
     *
     * @param the WPDS that forms the basis for the constructed WPDS
     * @return the WPDS which can be used to perform property checking 
     * using PDS reachability
     * 
     */
    wpds::WPDS NWA::plusWPDS( wpds::WPDS base )
    {
      wpds::WPDS result = wpds::WPDS();
      wpds::WpdsRules rules = wpds::WpdsRules();
      
      base.for_each(rules);
      
      //Step Rules:
      for( std::set< wpds::Rule >::iterator it = rules.stepRules.begin();
            it != rules.stepRules.end(); it++ )
      {
        //<p,n_1> -w-> <p',n_2> in delta_1, (q,n_1,q') in delta_i
        Key sym = it->from_stack();
        //look for internal transitions in the NWA with symbol sym
        for( std::set< KeyTriple >::iterator iit = trans->beginInternal();
            iit != trans->endInternal(); iit++ )
        {
          if( iit->second == sym )
          {
            //<(p,q),n_1> -w-> <(p',q'),n_2>
            Key from = getKey(it->from_state(),iit->first);
            Key to = getKey(it->to_state(),iit->third);
            result.add_rule(from,sym,to,it->to_stack1(),it->weight());
          }
        }
      }
      
      //Call Rules:
      for( std::set< wpds::Rule >::iterator it = rules.pushRules.begin();
            it != rules.pushRules.end(); it++ )
      {
        //<p,n_c> -w-> <p',e r_c> in delta_2, (q_c,n_c,q) in delta_c
        Key sym = it->from_stack();
        //look for call transitions in the NWA with symbol sym
        for( std::set< KeyTriple >::iterator cit = trans->beginCall();
              cit != trans->endCall(); cit++ )
        {
          if( cit->second == sym )
          {
            //<(p,q_c),n_c> -w-> <(p',q),e (r_c,q_c)>
            Key from = getKey(it->from_state(),cit->first);
            Key to = getKey(it->to_state(),cit->third);
            Key stack2 = getKey(it->to_stack2(),cit->first);
            result.add_rule(from,sym,to,it->to_stack1(),stack2,it->weight());
          }
        }
      }
      
      //Return Rules:
      for( std::set< wpds::Rule >::iterator it = rules.popRules.begin();
            it != rules.popRules.end(); it++ )
      {
        //<p,x> -w-> <p',*> in delta_0, (q_r,q_c,x,q) in delta_r
        Key sym = it->from_stack();
        //look for return transitions in the NWA with symbol sym
        for( std::set< KeyQuad >::iterator rit = trans->beginReturn();
              rit != trans->endReturn(); rit++ )
        {
          if( rit->third == sym )
          {
            //<(p,q_r),x> -w-> <(p',q_r^x),*>
            Key from = getKey(it->from_state(),rit->first);
            Key to = getKey( it->to_state(),getKey(rit->first,sym) );
            result.add_rule(from,sym,to,it->weight());
            //For all r_c in delta_2
            for( std::set< wpds::Rule >::iterator dit = rules.pushRules.begin();
                  dit != rules.pushRules.end(); dit++ )
            {
              //<(p',q_r^x),(r_c,q_c)> -1-> <(p',q),r_c>
              from = getKey( it->to_state(),getKey(rit->first,sym) );
              sym = getKey(dit->to_stack2(),rit->second);
              to = getKey(it->to_state(),rit->fourth);
              result.add_rule(from,sym,to,dit->to_stack2(),it->weight()->one());
            }
          }
        }
      }      
      
      return result;
    }
    
    /**
     *
     * @brief constructs the NWA equivalent to the given PDS
     *
     * @return the NWA equivalent to the given PDS
     *
     */
    nwa::NWA NWA::PDStoNWA(wpds::WPDS pds)
    {
      nwa::NWA result = nwa::NWA();
      
      std::map<Key,Key> call_return;
      
      wpds::WpdsRules rules = wpds::WpdsRules();
      
      pds.for_each(rules);
      
      //Step Rules:
      for( std::set< wpds::Rule >::iterator it = rules.stepRules.begin();
            it != rules.stepRules.end(); it++ )
      {
        //<p,n_1> -w-> <p',n_2> in delta_1 goes to ((p,n_1),n_1,(p',n_2)) in delta_i
        result.addInternalTrans(getKey( it->from_state(),it->from_stack() ),  //from
                                it->from_stack(),                             //sym
                                getKey( it->to_state(),it->to_stack1() ));    //to
      }
      
      //Call Rules:
      for( std::set< wpds::Rule >::iterator it = rules.pushRules.begin();
            it != rules.pushRules.end(); it++ )
      {
        //<p,n_c> -w-> <p',e r_c> in delta_2, ((p,n_c),c,(p',e)) in delta_c
        result.addCallTrans(getKey( it->from_state(),it->from_stack() ),      //from
                            it->from_stack(),                                 //sym
                            getKey( it->to_state(),it->to_stack1() ));        //to
        //add (p,n_c) => r_c to the call-return map
        call_return.insert(std::pair<Key,Key>(getKey( it->from_state(),it->from_stack() ),  //call
                                              it->to_stack2()));                            //ret
      }
      
      //Return Rules:
      for( std::set< wpds::Rule >::iterator it = rules.popRules.begin();
            it != rules.popRules.end(); it++ )
      {
        //<p,x> -w-> <p',*> in delta_0, ((p,x),(p,n_c),x,(p',r)) in delta_r
        for(std::map<Key,Key>::iterator cr_it = call_return.begin(); 
            cr_it != call_return.end(); cr_it++  )
          result.addReturnTrans(getKey( it->from_state(),it->from_stack() ),  //from
                                cr_it->first,                                 //pred
                                it->from_stack(),                             //sym
                                getKey( it->to_state(),cr_it->second ));      //to
      }
      
      return result;    
    }
    
    /**
     *
     * @brief constructs the PDS equivalent to this NWA
     *
     * @return the PDS equivalent to this NWA
     *
     */
    wpds::WPDS NWA::NWAtoPDS(NWA nwa,witness::Witness wgt)
    {
      wpds::WPDS result = wpds::WPDS();
      std::map< Key,Key > calls;
      
      Key program = wali::getKey("program"); 
      
      //Internal Transitions
      for( std::set< KeyTriple >::iterator iit = nwa.trans->beginInternal();
            iit != nwa.trans->endInternal(); iit++ )
      {
        result.add_rule(program,          //from_state
                        iit->first,       //from_stack
                        program,          //to_state
                        iit->third,       //to_stack1
                        wgt.one());       //weight      
      }
      
      //Call Transitions
      for( std::set< KeyTriple >::iterator cit = nwa.trans->beginCall();
            cit != nwa.trans->endCall(); cit++ )
      {
        for( std::set< KeyQuad >::iterator rit = nwa.trans->beginReturn();
              rit != nwa.trans->endReturn(); rit++ )
          if( cit->first == rit->second )
          {
            //for each return site with cit->first as call site ...
            Key ret = getKey(rit->fourth,rit->fourth); // (r,r)
            calls.insert(std::pair<Key,Key>(cit->first,ret));
            
            result.add_rule(program,          //from_state
                            cit->first,       //from_stack
                            program,          //to_state
                            cit->third,       //to_stack1
                            ret,              //to_stack2
                            wgt.one());       //weight  
          }  
      }
      
      //Return Transitions
      for( std::set< KeyQuad >::iterator rit = nwa.trans->beginReturn();
            rit != nwa.trans->endReturn(); rit++ )
      {
        Key rstate = getKey(program,rit->first); // (p,x_i)
        result.add_rule(program,          //from_state
                        rit->first,       //from_stack
                        rstate,           //to_state
                        wgt.one());       //weight 
        
        std::map<Key,Key>::iterator ret = calls.find(rit->second);
        if( ret != calls.end() )          
          result.add_rule(rstate,         //from_state
                          ret->second,    //from_stack
                          program,        //to_state
                          rit->fourth,    //to_stack
                          wgt.one());     //weight    
        
      }
      
      return result;
    }

    /**
     *
     * @brief constructs a deterministic NWA that is equivalent to this 
     * NWA.
     *
     * @return the NWA which is a deterministed equivalent to this NWA
     *
     */
    NWA & NWA::determinize( )
    {
      std::pair<Trans,StateMap> tmp = std::pair<Trans,StateMap>();
            
      //New initial state is {(q,q) | q is an element of Q (possibly Q_in?)}
      Key newInitialState;
      std::set<Key> initialStateSet;
      StatePairSet initialState = StatePairSet(); 
      bool first = true;
      for( Trans::stateIterator sit = trans->beginStates();
            sit != trans->endStates(); sit++ )
      {
        initialState.insert( StatePair(*sit,*sit) );
        if( first )
        {
          initialStateSet.insert(wali::getKey(*sit,*sit));
          first = false;
        }
        else
          initialStateSet.insert(wali::getKey(*sit,*sit));
      }
      
      newInitialState = wali::getKey(initialStateSet);
      //Add the initial state to the deterministic NWA.
      tmp.first.addInitialState(newInitialState);      
      //Add to the map the initial state set and the initial state.
      tmp.second.insert(std::pair<StatePairSet,Key>(initialState,newInitialState));
      
      tmp = det(tmp.first,tmp.second,initialState,std::stack<StatePairSet>());
          
      //A state is final if it contains a pair of the form (q,q') with
      //q an element of Q_in and q' an element of Q_f. 
      for( StateMap::iterator it = tmp.second.begin();
            it != tmp.second.end(); it++ )
      {
        for( StatePairSet::const_iterator sit = it->first.begin();
              sit != it->first.end(); sit++ )
        {
          if( trans->isInitialState(sit->first) && 
              trans->isFinalState(sit->second) )
          {
              tmp.first.addFinalState(it->second);
              break;
          }
        }          
      }
      
      trans = &tmp.first;    
      return *this;
    }
    
    /**
     *
     * @brief tests whether the NWA is deterministic 
     *
     * @return true if the NWA is deterministic, false otherwise
     *
     */
    bool NWA::isDeterministic( )
    {
      //TODO: optimize!!!
      //An NWA is not deterministic if there is not exactly one initial state
      if( trans->sizeInitialStates() != 1)
        return false;

      //An NWA is not deterministic if there are epsilon transitions to states
      //other than the error state or if there are multiple transitions of some
      //type that are identical except for the to state.
      for( Trans::symbolIterator it = trans->beginSymbols();
            it != trans->endSymbols(); it++ )
      {
        for( Trans::stateIterator sit = trans->beginStates();
              sit != trans->endStates(); sit++ )
        {
          //Check call transitions.
          int count = 0;
          for( Trans::callIterator cit = trans->beginCall();
                cit != trans->endCall(); cit++ )
          {
            //Epsilon transition to a state other than the error state.
            if( *it == wali::WALI_EPSILON )
              if( (cit->second == *it) && (cit->third != wali::WALI_BAD_KEY) )
                return false;
            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (cit->first == *sit) && (cit->second == *it) )
              count++;
          }
          if( count > 1 )
            return false;
          
          //Check internal transitions.
          count = 0;
          for( Trans::internalIterator iit = trans->beginInternal();
                iit != trans->endInternal(); iit++ )
          {
            //Epsilon transition to a state other than the error state.
            if( *it == wali::WALI_EPSILON )
              if( (iit->second == *it) && (iit->third != wali::WALI_BAD_KEY) )
                return false;
            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (iit->first == *sit) && (iit->second == *it) )
              count++;
          }
          if( count > 1 )
            return false;
          
          for( Trans::stateIterator pit = trans->beginStates();
                pit != trans->endStates(); pit++ )
          {
            //Check return transitions.
            count = 0;
            for( Trans::returnIterator rit = trans->beginReturn();
                  rit != trans->endReturn(); rit++ )
            {
              //Epsilon transition to a state other than the error state.
              if( *it == wali::WALI_EPSILON )
                if( (rit->third == *it) && (rit->fourth != wali::WALI_BAD_KEY) )
                  return false;
              //Keep a count of multiple transitions with the same from
              //state and symbol(that is not epsilon).
              else if( (rit->first == *sit) &&
                  (rit->second == *pit) &&
                  (rit->third == *it) )
                count++;
            }
            if( count > 1 )
              return false;
          }
        }
      }
            
      return true;
    }
  
    /**
     *
     * @brief tests whether the language accepted by this NWA is empty
     *
     * @return true if the language accepted by this NWA is empty
     *
     */
    //TODO: determinize prior to checking paths?
    bool NWA::isEmpty( )
    {
      //An automaton with no initial states must accept only the empty
      //language.
      if( trans->sizeInitialStates() == 0 )
        return true;
        
      //An automaton with no final states must accept only the empty 
      //language.
      if( trans->sizeFinalStates() == 0 )
        return true;
        
      //An automaton with no path from an initial state to a final state
      //must accept only the empty language.       
      for( Trans::stateIterator it = trans->beginInitialStates();
            it != trans->endInitialStates(); it++ )
      {
        //If an initial state is also a final state, then
        //the empty string is accepted.  Therefore the
        //accepted language is not the empty language.
        if( trans->isFinalState(*it) )
          return false;
        std::set< Key > visited = std::set< Key >();
        visited.insert(*it);
        if( hasPath(*it, std::stack< Key >(), visited) )
          return false;
        visited.erase(*it);
      }
      return true;
    }
  
    /**
     *
     * @brief tests whether the given nested word is a member of the 
     * language accepted by this NWA
     *
     * @param the nested word to test
     * @return true if the given nested word is a member of the language
     * accepted by this NWA
     *
     */
    //TODO: determinize prior to doing this simulation?
    bool NWA::isMember( nws::NWS word )
    {
      //Check if the NWS is just a nested word suffix
      //rather than a nested word.
      if( word.stackSize() > 0 )
        return false;
        
      //Simulate the nested word 'word' on the NWA.
      Trans::stateIterator iit = trans->beginInitialStates();
      while( iit != trans->endInitialStates() )
      {
        //Follow the word from here and try to match it.
        if( matchWord(*iit, word, std::stack< Key >()) )
          return true; 
      }
      return false;
    }
 
    //Utilities	

    /**
     *
     * @brief print the NWA
     *
     * @param the output stream to print to
     * @return the output stream that was printed to
     *
     */
    std::ostream & NWA::print( std::ostream & o) const
    {
      trans->print(o);
      return o;
    }
 
    /**
     *
     * @brief tests whether this NWA is equivalent to the NWA 'other'
     *
     * @param the NWA to compare this NWA to
     * @return true if this NWA is equivalent to the NWA 'other'
     *
     */
    bool NWA::operator==( NWA & other )
    {
      return ( (trans == other.trans) );
    }
  
    /**
     *
     * @brief returns the number of states associated with this NWA
     *
     * @return the number of states associated with this NWA
     *
     */
    size_t NWA::numStates( )
    {
      return trans->sizeStates();
    }
  
    /**
     *
     * @brief returns the number of transitions (of all kinds) 
     * associated with this NWA
     *
     * @return the number of transitions (of all kinds) associated
     * with this NWA
     *
     */
    size_t NWA::numTrans( )
    {
      return trans->size();
    }
    
    /**
     *
     * @brief tests whether the NWA has a path from the current state to
     * a final state given the call stack and the set of states already visited
     *
     * @param currState: the state from which to start searching for a path
     * @param calls: the stack of calls that have been made thus far
     * @param visited: the states which have already been visited
     * @return true if there is a path through the NWA from the current state 
     * to a final state, false otherwise
     *
     */
    bool NWA::hasPath( Key currState, std::stack< Key > calls, std::set< Key > visited )
    {
      //Try each possible transition out of this state to see if a final state
      //can be reached.
      
      //First try call transitions.
      calls.push(currState);
      for( Trans::callIterator cit = trans->beginCall();
            cit != trans->endCall(); cit++ )
      {
        //If the call starts from the current state and goes to a state not
        //yet visited, follow the path.
        if( (cit->first == currState) && (visited.count(cit->third) == 0) )
        {
          visited.insert(cit->third);
          if( hasPath(cit->third,calls,visited) )
              return true;
          visited.erase(cit->third);
        }    
      }
      calls.pop();
      
      //If no call transition led to a path, try internal transitions.
      for( Trans::internalIterator iit = trans->beginInternal();
            iit != trans->endInternal(); iit++ )
      {
        //If the call starts from the current state and goes to a state not
        //yet visited, follow the path.
        if( (iit->first == currState) && (visited.count(iit->third) == 0) )
        {
          visited.insert(iit->third);
          if( hasPath(iit->third,calls,visited) )
              return true;
          visited.erase(iit->third);
        }    
      }
      
      //If no internal transition led to a path, try return transitions.
      Key callState = calls.top();
      calls.pop();
      for( Trans::returnIterator rit = trans->beginReturn();
            rit != trans->endReturn(); rit++ )
      {
         if( (rit->first == currState) &&
             (rit->second == callState) &&
             (visited.count(rit->fourth) == 0) )
          {
            visited.insert(rit->fourth);
            if( hasPath(rit->fourth,calls,visited) )
              return true;
            visited.erase(rit->fourth);
          }    
        }
        calls.push(callState);      
      
      return false; 
    }
    
    /** 
     *
     * @brief tests whether the nested word suffix is accepted by 
     * this nested word automaton starting at the state with the
     * given name
     *
     * @parm currState: the state in the nested word automaton at 
     * which to start the simulation
     * @parm word: the nested word suffix to match
     * @return true if the simulation ends in an accepting state
     *
     */
    bool NWA::matchWord( Key currState, nws::NWS word, std::stack< Key > calls )
    {
      //Base case: the word suffix has been completely processed 
      // and acceptance is determined by whether the current
      // state is a final state or not.
      if( word.isEmpty() )
      {
        return isFinalState(currState);        
      }
      else if( word.nextNode() == NULL )
      {
        return false;
      }
      
      //Induction: the next node in the word suffix must be 
      // processed and the current state of the automaton
      // advanced properly.      
      nws::NWSNode * currNode = word.nextNode();
      word.removeNode();      
      
      //Check for transitions that match.
      
      //Nodes with an exit node match call transitions
      //Nodes with no exit node match
      //  return transitions if the node is the node at the top of the nesting
      //  internal transitions otherwise
      if( currNode->isCall() )
      {
        calls.push(currState);
        for( Trans::callIterator cit = trans->beginCall();
              cit != trans->endCall(); cit++ )
        {
          //If this symbol in the word can be matched, try matching
          //the rest of the word.
          if( (cit->first == currState) &&
              (cit->second == currNode->getSymbol()) )
          {
            if( matchWord(cit->third,word,calls) )
              return true;
          }    
        }
        calls.pop();
        //If no call transition led to a matched word, there is no
        //accepting path through this state for the given word suffix.
        return false;
      }
      else if( currNode == word.stackTop() )
      {
        Key callState = calls.top();
        calls.pop();
        for( Trans::returnIterator rit = trans->beginReturn();
              rit != trans->endReturn(); rit++ )
        {
          //If this symbol in the word can be matched, try matching
          //the rest of the word.
          if( (rit->first == currState) &&
              (rit->second == callState) &&
              (rit->third == currNode->getSymbol()) )
          {
            if( matchWord(rit->fourth,word,calls) )
              return true;
          }    
        }
        calls.push(callState);
        //If no return transition led to a matched word, there is no
        //accepting path through this state for the given word suffix.
        return false;
      }
      else
      {
        for( Trans::internalIterator iit = trans->beginInternal();
              iit != trans->endInternal(); iit++ )
        {
          //If this symbol in the word can be matched, try matching
          //the rest of the word.
          if( (iit->first == currState) &&
              (iit->second == currNode->getSymbol()) )
          {
            if( matchWord(iit->third,word,calls) )
              return true;
          }    
        }
        //If no internal transition led to a matched word, there is no
        //accepting path through this state for the given word suffix.
        return false;
      }
    }
    
    /**
     *
     * @brief constructs the transition table and state map for the deterministic 
     * NWA that is equivalent to this NWA.
     *
     * @return the transition table and state map for the deterministic NWA 
     * that is equivalent to this NWA
     *
     */
    std::pair<NWA::Trans,NWA::StateMap> NWA::det( Trans trans, StateMap stateMap, 
                        StatePairSet currState, std::stack<StatePairSet> callPred )
    {
      //Internal Transition
      for( Trans::symbolIterator it = trans.beginSymbols();
            it != trans.endSymbols(); it++ )
      {        
        StatePairSet internalTrans;
        //Find all internal transitions that use this symbol.
        for( Trans::internalIterator iit = trans.beginInternal();
              iit != trans.endInternal(); iit++ )
        {
          //If the symbols match, make a new state pair set.
          if( *it == iit->second )
          {
            internalTrans.insert( StatePair(iit->first,iit->second) );
          }
        }
        //At an internal position labeled a, the automaton replaces each
        //pair (q,q') in the current state by pairs of the form (q,q'') 
        //such that(q',a,q'') is an element of Delta_i.
        Key tmpInternalKey;
        StatePairSet tmpInternal;
        bool first;
        for( StatePairSet::iterator sit = currState.begin();
              sit != currState.end(); sit++ )
        {
          bool replaced = false;
          for( StatePairSet::iterator iit = internalTrans.begin();
                iit != internalTrans.end(); iit++ )
          {
            if( sit->second == iit->first )
            {
              tmpInternal.insert(StatePair(sit->first,iit->second));
              if(first)
              {
                tmpInternalKey = wali::getKey(sit->first,iit->second);
                first = false;
              }
              else
                tmpInternalKey = wali::getKey( wali::getKey(sit->first,iit->second), tmpInternalKey); 
              replaced = true;
            }
          }
          if(!replaced)
          {
            tmpInternal.insert(*sit);
            if(first)
            {
              tmpInternalKey = wali::getKey(sit->first,sit->second);
              first = false;
            }
            else
              tmpInternalKey = wali::getKey( wali::getKey(sit->first,sit->second), tmpInternalKey);
          }
        }
              
        //We only want to add states that don't already exist.        
        bool recurse = false;
        if( stateMap.count(tmpInternal) == 0 )
        {
          //Add the new key to the map.
          stateMap.insert(std::pair<StatePairSet,Key>(tmpInternal,tmpInternalKey));
          trans.addState(tmpInternalKey);
          recurse = true;
        }
        else
          tmpInternalKey = (stateMap.find(tmpInternal))->second;
            
        //Add a transition.
        KeyTriple newInternalTrans = 
              KeyTriple(stateMap.find(currState)->second,*it,tmpInternalKey);
        trans.addInternal(newInternalTrans);
        
        if(recurse)
        {
          std::pair<Trans,StateMap> result = det(trans,stateMap,tmpInternal,callPred);
          trans = result.first;
          stateMap = result.second;
        }
      }    
           
        //Call Transition  
        //??Does this only need to be done once??      
        for( Trans::symbolIterator it = trans.beginSymbols();
              it != trans.endSymbols(); it++ )
        {        
          //At a call position labeled a, the summary gets reinitialized: 
          //the new state contains pairs of the form (q,q'), where (q,a,q')
          // is an element of Delta_c.
          Key tmpCallKey;
          StatePairSet tmpCall;
          bool first = true;
          for( Trans::callIterator cit = trans.beginCall();
                cit != trans.endCall(); cit++ )
          {
            //If the symbols match, make a new state pair set.
            if( *it == cit->second )
            {
              tmpCall.insert(StatePair(cit->first,cit->third));
              if(first)
              {
                tmpCallKey = wali::getKey(cit->first,cit->third);
                first = false;
              }
              else
                tmpCallKey = wali::getKey( wali::getKey(cit->first,cit->third), tmpCallKey);
            }
          }
          //We only want to add states that don't already exist.          
          bool recurse = false;
          if( stateMap.count(tmpCall) == 0 )
          {
            //Add the new key to the map.
            stateMap.insert(std::pair<StatePairSet,Key>(tmpCall,tmpCallKey));
            trans.addState(tmpCallKey);
            recurse = true;
          }
          else
            tmpCallKey = (stateMap.find(tmpCall))->second;
            
          //Add a transition.
          KeyTriple newCallTrans = 
                KeyTriple(stateMap.find(currState)->second,*it,tmpCallKey);
          trans.addCall(newCallTrans);
          
          if(recurse)
          {
            callPred.push(currState);
            std::pair<Trans,StateMap> result = det(trans,stateMap,tmpCall,callPred);
            trans = result.first;
            stateMap = result.second;
          }
        }
        
        //Return Transition
        for( Trans::symbolIterator it = trans.beginSymbols();
              it != trans.endSymbols(); it++ )
        { 
          //Consider a return position labeled a, and suppose S denotes the
          //current state and S' denotes the state just before the call-predecessor.
          //Then (q,q') belongs to the new state, provided there exist states 
          //q_1,q_2 such that (q,q_1) is an element of S' and (q_1,q_2) is an 
          //element of S and (q_2,q_1,a,q') is an element of Delta_r.
          Key tmpReturnKey;
          StatePairSet tmpReturn;
          for( Trans::returnIterator rit = trans.beginReturn();
                rit != trans.endReturn(); rit++ )
          {
            //If the symbols match, make a new state pair set.
            if( *it == rit->third )
            {
              //S' is callPred.top(), S is currState
              
              //Want to find (q_1,q_2) in S
              bool found = false;
              for( StatePairSet::iterator sit = currState.begin();
                    sit != currState.end(); sit++ )
              {
                if( (sit->first == rit->second) && (sit->second == rit->first) )
                  found = true;
              }
              
              if( found )
              {
                //Want to find (q,q_1) in S'
                bool first = true;
                for( StatePairSet::iterator sit = callPred.top().begin();
                      sit != callPred.top().end(); sit++ )
                {
                  if( sit->second == rit->second )
                  {
                    tmpReturn.insert(StatePair(sit->first,rit->fourth));
                    if(first)
                    {
                      tmpReturnKey = wali::getKey( sit->first,rit->fourth );
                      first = false;
                    }
                    else
                      tmpReturnKey = wali::getKey( wali::getKey( sit->first,rit->fourth ), tmpReturnKey );
                  }
                }
              }
            }
          }          
          //We only want to add states that don't already exist.
          bool recurse = false;
          if( stateMap.count(tmpReturn) == 0 )
          {
            //Add the new key to the map.
            stateMap.insert(std::pair<StatePairSet,Key>(tmpReturn,tmpReturnKey));
            trans.addState(tmpReturnKey);
            recurse = true;
          }
          else
            tmpReturnKey = (stateMap.find(tmpReturn))->second;
          
          Key tmpCallKey = (stateMap.find(callPred.top()))->second;
            
          //Add a transition.
          KeyQuad newReturnTrans = 
                KeyQuad(stateMap.find(currState)->second,tmpCallKey,*it,tmpReturnKey);
          trans.addReturn(newReturnTrans);
          
          if(recurse)
          {
            callPred.pop();
            std::pair<Trans,StateMap> result = det(trans,stateMap,tmpReturn,callPred);
            trans = result.first;
            stateMap = result.second;
          }
        }
            
      return std::pair<Trans,StateMap>(trans,stateMap);
    }
  };
}