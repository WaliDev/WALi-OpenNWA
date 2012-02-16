#include <sstream>
#include <cstring>

#include "opennwa/Nwa.hpp"
#include "opennwa/details/Configuration.hpp"
#include "opennwa/NestedWord.hpp"
#include "opennwa/query/transitions.hpp"
#include "opennwa/query/calls.hpp"
#include "opennwa/query/internals.hpp"
#include "opennwa/nwa_pds/conversions.hpp"

namespace opennwa
{
  using wali::wpds::WPDS;
  using wali::printKey;
  using wali::key2str;
    
  const std::string Nwa::XMLTag = "XML";
      
  //
  // Methods
  //

  //Constructors and Destructor
  Nwa::Nwa( )
  { }

    
  Nwa::Nwa( const Nwa & other )
    : Printable(other)
    , Countable(other)
    , states(other.states), symbols(other.symbols), trans(other.trans)
  {   }

    
  Nwa & Nwa::operator=( const Nwa & other )
  {
    if( this == &other )
      return *this;

    //Erase any pre-existing data.
    clear();

    //Copy data over from 'other'
    states = other.states;
    symbols = other.symbols;
    trans = other.trans;

    return *this;
  }
    
  /**
   *
   * @brief erases all data from the NWA
   *
   * This method restore's the NWA to its default state:
   * the stuck state with no symbols or transitions.
   *
   */
    
  void Nwa::clear( )
  {
    clearStates();
    clearSymbols();
    //clearTrans() called from clearStates()
  }

  //State Accessors

  /**
   * 
   * @brief access the client information associated with the given state
   *
   * @param - state: the state whose client information to retrieve
   * @return the client information associated with the given state
   *
   */
    
  Nwa::ClientInfoRefPtr Nwa::getClientInfo( State state ) const
  {
    assert(state < wali::WALI_BAD_KEY);
    return states.getClientInfo(state);
  }

  /**
   * 
   * @brief set the client information associated with the given state
   *
   * @param - state: the  state whose client information to set
   * @param - c: the desired client information for the given state
   *
   */
    
  void Nwa::setClientInfo( State state, const ClientInfoRefPtr c )
  {
    assert(state < wali::WALI_BAD_KEY);
    states.setClientInfo(state,c);
  }
     
  //All States

  /**
   *   
   * @brief provides access to all states in the NWA
   *
   * @return a set of all states 
   *
   */
    
  const StateSet & Nwa::getStates( ) const
  {
    return states.getStates();
  }

  /**
   * 
   * @brief test if a given state is a state of this NWA
   *
   * @param - state: the state to check
   * @return true if the given state is a state of this NWA
   *
   */
    
  bool Nwa::isState( State state ) const
  {
    assert(state < wali::WALI_BAD_KEY);
    return states.isState(state);
  }

  /**
   *  
   * @brief add the given state to this NWA
   *
   * @param - state: the state to add
   * @return false if the state already exists in the NWA, true otherwise
   *
   */
    
  bool Nwa::addState( State state )
  {
    assert(state < wali::WALI_BAD_KEY);
    return states.addState(state);
  }

  /**
   *
   * @brief returns the number of states associated with this NWA
   *
   * @return the number of states associated with this NWA
   *
   */
    
  size_t Nwa::sizeStates( ) const
  {
    return states.sizeStates();
  }
  /**
   *
   * @brief returns the number of states associated with this NWA
   *
   * Note: This method simply calls sizeStates( ).  It is here to make the transition
   *        from using PDS to using NWA easier.
   *
   * @return the number of states associated with this NWA
   *
   */
    
  /**
   *  
   * @brief remove the given state from this NWA
   *
   * @param - state: the state to remove
   * @return false if the state does not exist in the NWA, true otherwise
   *
   */
    
  bool Nwa::removeState( State state )
  {
    assert(state < wali::WALI_BAD_KEY);

    bool removed = states.removeState(state);
    //Remove transitions associated with the state that was removed.
    if( removed )
    {
      trans.removeTransWith(state);
    }

    return removed;
  }

  /**
   *
   * @brief remove all states from the NWA
   *
   */
    
  void Nwa::clearStates( )
  {
    states.clearStates();

    //Since all states are being removed, all transitions are removed as well.
    clearTrans();
  }

  //Initial States

  /**
   *  
   * @brief obtain the states in the initial state set
   *
   * @return set of inital states associated with the NWA
   *
   */
    
    
  const StateSet & Nwa::getInitialStates( ) const
  {
    return states.getInitialStates();
  }

  /**
   * 
   * @brief test if the given state is an initial state of this NWA
   *
   * @param - state: the state to check
   * @return true if the given state is an initial state, false otherwise
   *
   */
    
    
  bool Nwa::isInitialState( State state ) const
  {
    assert(state < wali::WALI_BAD_KEY);
    return states.isInitialState(state);
  }

  /**
   * 
   * @brief make the given state an initial state in this NWA
   *
   * @param - state: the state to add to initial state set
   * @return false if the state already exists in the initial state set of the NWA
   *
   */
    
    
  bool Nwa::addInitialState( State state )
  {
    assert(state < wali::WALI_BAD_KEY);
    return states.addInitialState(state);
  }

  /**
   *
   * @brief returns the number of initial states associated with this NWA
   *
   * @return the number of initial states associated with this NWA
   *
   */
    
    
  size_t Nwa::sizeInitialStates( ) const
  {
    return states.sizeInitialStates();
  }

  /**
   * 
   * @brief remove the given state from the initial state set of this NWA
   *
   * @param - state: the state to remove from the initial state set
   * @return false if the state does not exist in the initial state set of this NWA
   *
   */
    
    
  bool Nwa::removeInitialState( State state )
  {
    assert(state < wali::WALI_BAD_KEY);
    return states.removeInitialState(state);
  }

  /**
   *
   * @brief remove all states from the initial state set of the NWA
   *
   */
    
    
  void Nwa::clearInitialStates( )
  {
    states.clearInitialStates();
  }

  //Final States

  /**
   *
   * @brief obtain the final states in this NWA
   *
   * @return set of all final states associated with this NWA
   *
   */
    
    
  const StateSet & Nwa::getFinalStates( ) const
  {
    return states.getFinalStates();
  }

  /**
   *
   * @brief test if a state with the given name is a final state
   *
   * @param - state: the state to check
   * @return true if the given state is a final state
   *
   */
    
    
  bool Nwa::isFinalState( State state ) const
  {
    assert(state < wali::WALI_BAD_KEY);
    return states.isFinalState(state);
  }

  /**
   *
   * @brief make the given state a final state
   *
   * @param - state: the state to add to final state set
   * @return false if the state already exists in the final state set of the NWA
   *
   */
    
    
  bool Nwa::addFinalState( State state )
  {
    assert(state < wali::WALI_BAD_KEY);
    return states.addFinalState(state);
  }

  /**
   * 
   * @brief returns the number of final states associated with this NWA
   *
   * @return the number of final states associated with this NWA
   *
   */
    
    
  size_t Nwa::sizeFinalStates( ) const
  {
    return states.sizeFinalStates();
  }

  /**
   *
   * @brief remove the given state from the final state set of the NWA
   *
   * @param - state: the state to remove from the final state set
   * @return false if the state does not exist in the final state set of the NWA
   *
   */
    
    
  bool Nwa::removeFinalState( State state )
  {
    assert(state < wali::WALI_BAD_KEY);
    return states.removeFinalState(state);
  }

  /**
   *
   * @brief remove all states from the final state set of the NWA
   *
   */
    
    
  void Nwa::clearFinalStates( )
  {
    states.clearFinalStates();
  }

  //Symbol Accessors

  /**
   *
   * @brief obtain all symbols in this NWA
   *
   * @return set of all symbols associated with this NWA
   *
   */
    
    
  const std::set< Symbol> & Nwa::getSymbols( ) const
  {
    return symbols.getSymbols();
  }

  /**
   *
   * @brief test if the given symbol is associated with the NWA
   *
   * @param - sym: the symbol to check
   * @return true if the given symbol is associated with the NWA
   *
   */
    
    
  bool Nwa::isSymbol( Symbol sym ) const
  {
    assert(sym < wali::WALI_BAD_KEY);
    return symbols.isSymbol(sym);
  }

  /**
   *
   * @brief add the given symbol to the NWA
   *
   * @param - sym: the symbol to add
   * @return false if the symbol is already associated with the NWA
   *
   */
    
    
  bool Nwa::addSymbol( Symbol sym )
  {
    assert(sym < wali::WALI_BAD_KEY);
    return symbols.addSymbol(sym);
  }

  /**
   *
   * @brief returns the number of symbols associated with this NWA
   *
   * Note: The epsilon symbol is included in this count.
   *
   * @return the number of symbols associated with this NWA
   *
   */
    
    
  size_t Nwa::sizeSymbols( ) const
  {
    return symbols.sizeSymbols();
  }

  /**
   *
   * @brief remove the given symbol from the NWA
   *
   * @param - sym: the symbol to remove
   * @return false if the symbols is not associated with the NWA
   *
   */
    
  bool Nwa::removeSymbol( Symbol sym )
  {
    assert(sym < wali::WALI_BAD_KEY);

    if( sym == WILD || sym == EPSILON )
      return false;

    bool removed = symbols.removeSymbol(sym);
    if( removed )
    {
      trans.removeTransSym(sym);
    }
    return removed;
  }

  /**
   *
   * @brief remove all symbols associated with the NWA
   *
   */
    
  void Nwa::clearSymbols( )
  {
    symbols.clearSymbols();

    //Since all symbols are being removed, all transitions are removed as well.
    clearTrans();
  }


    
  //Transition Accessors

  /**
   *  
   * @brief duplicates the original state, but only duplicates outgoing transitions
   *
   * @param - orig: the name of the original state, i.e. the state to duplicate
   * @param - dup: the name of the duplicate state
   *
   */
    
  void Nwa::duplicateStateOutgoing( State orig, State dup )
  {
    assert(orig < wali::WALI_BAD_KEY);
    assert(dup < wali::WALI_BAD_KEY);
    assert(isState(orig));

    states.addState(dup);
    states.dupState(orig,dup);  //Duplicate state characteristics(initial/final).

    trans.dupTransOutgoing(orig,dup);  //Duplicate outgoing transitions.
  }

  /**
   *  
   * @brief duplicates the original state
   *
   * @param - orig: the name of the original state, i.e. the state to duplicate
   * @param - dup: the name of the duplicate state
   *
   */
    
  void Nwa::duplicateState( State orig, State dup )
  {
    assert(orig < wali::WALI_BAD_KEY);
    assert(dup < wali::WALI_BAD_KEY);
    assert(isState(orig));

    states.addState(dup);
    states.dupState(orig,dup);  //Duplicate state characteristics(initial/final).

    trans.dupTrans(orig,dup);  //Duplicate all transitions.
  }

  /**
   *  
   * @brief realizes all implicit transitions in the NWA
   *
   */
    
  void Nwa::realizeImplicitTrans(State stuck)
  {
    std::set<wali::Triple<State, Symbol, State> > returns;
    for( ReturnIterator ret = beginReturnTrans(); ret != endReturnTrans(); ++ret) 
    {
      returns.insert(wali::Triple<State, Symbol, State>(ret->first, ret->second, ret->third));
    }

    // For every state
    //    For every symbol
    //       For every kind of transition
    //          If *no* outgoing transition exists of that type,
    //          add a transition to the stuck state.
    //
    // - For return transitions, have to check for a return transition
    //   from every other state and add any that don't exist
    // - Also in the implementation, we don't have a "for every kind of
    //   transition" loop; the three types are done one after the other
    for( StateIterator stit = beginStates(); stit != endStates(); ++stit )
    {
      State state = *stit;

      for( SymbolIterator symit = beginSymbols(); symit != endSymbols(); ++symit )
      {
        Symbol symbol = *symit;

        // Skip over epsilons. (A) they are handled in other places and
        // (B) there's no implicit epsilon transitions to stuck anyway.
        if( symbol == EPSILON )
        {
          continue;
        }

        //Skip over wilds.
        if( symbol == WILD )
        {
          continue;
        }

        if( !trans.callExists(state, symbol) && !trans.callExists(state, WILD) )
        {
          addCallTrans(state, symbol, stuck);
        }

        if( !trans.internalExists(state, symbol) && !trans.internalExists(state, WILD) )
        {
          addInternalTrans(state, symbol, stuck);
        }

        for( StateIterator pred = beginStates(); pred != endStates(); ++pred )
        {
          if( returns.find(wali::Triple<State,Symbol,State>(state, *pred, symbol)) == returns.end() 
              && returns.find(wali::Triple<State,Symbol,State>(state, *pred, WILD)) == returns.end() )
          {
            addReturnTrans(state, *pred, symbol, stuck);
          }
        }
      } // for each symbol
    } // for each state
  }

  /**
   *
   * @brief returns the number of transitions associated with this NWA
   *
   * @return the number of transitions associated with this NWA
   *
   */
    
    
  size_t Nwa::sizeTrans( ) const
  {
    return trans.size();
  }

  /**
   *
   * @brief remove all transitions from the NWA
   *
   */
  void Nwa::clearTrans( )
  {
    trans.clear();
  }

  //Call Transitions


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
    
  bool Nwa::addCallTrans( State from, Symbol sym, State to )
  {
    assert(from < wali::WALI_BAD_KEY);
    assert(sym < wali::WALI_BAD_KEY);
    assert(to < wali::WALI_BAD_KEY);

    assert(sym != EPSILON ); //An Epsilon symbol on a call doesn't make sense.

    //Add the states and symbol of this transition to the appropriate sets.
    addState(from);
    addSymbol(sym);
    addState(to);

    return trans.addCall(from,sym,to);
  }

  /**
   *
   * @brief add a call transition to the NWA
   *
   * @param - ct: the call transition to add
   * @return false if the call transition already exists in the NWA
   *
   */
    
  bool Nwa::addCallTrans( Call & ct )
  {
    assert(Trans::getCallSite(ct) < wali::WALI_BAD_KEY);
    assert(Trans::getCallSym(ct) < wali::WALI_BAD_KEY);
    assert(Trans::getEntry(ct) < wali::WALI_BAD_KEY);

    assert( Trans::getCallSym(ct) != EPSILON ); //An Epsilon symbol on a call doesn't make sense.

    //Add the states and symbol of this transition to the appropriate sets.
    addState(Trans::getCallSite(ct));
    addSymbol(Trans::getCallSym(ct));
    addState(Trans::getEntry(ct));

    return trans.addCall(ct);
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
    
  bool Nwa::removeCallTrans( State from, Symbol sym, State to )
  {
    assert(from < wali::WALI_BAD_KEY);
    assert(sym < wali::WALI_BAD_KEY);
    assert(to < wali::WALI_BAD_KEY);

    if(! isState(from) || ! isSymbol(sym) || ! isState(to) || sym == EPSILON )
      return false;

    return trans.removeCall(from,sym,to);
  }

  /**
   *
   * @brief remove a call transition from the NWA
   *
   * @param - ct: the call transition to remove
   * @return false if the call transition does not exist in the NWA
   *
   */
    
  bool Nwa::removeCallTrans( const Call & ct )
  {
    assert(Trans::getCallSite(ct) < wali::WALI_BAD_KEY);
    assert(Trans::getCallSym(ct) < wali::WALI_BAD_KEY);
    assert(Trans::getEntry(ct) < wali::WALI_BAD_KEY);

    if(! isState(Trans::getCallSite(ct))
       || ! isSymbol(Trans::getCallSym(ct))
       || ! isState(Trans::getEntry(ct)) 
       || Trans::getCallSym(ct) == EPSILON )
      return false;

    return trans.removeCall(ct);
  }

  /**
   *
   * @brief returns the number of call transitions associated with this NWA
   *
   * @return the number of call transitions associated with this NWA
   *
   */
    
    
  size_t Nwa::sizeCallTrans( ) const
  {
    return trans.sizeCall();
  }

  //Internal Transitions


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
    
  bool Nwa::addInternalTrans( State from, Symbol sym, State to )
  {
    assert(from < wali::WALI_BAD_KEY);
    assert(sym < wali::WALI_BAD_KEY);
    assert(to < wali::WALI_BAD_KEY);

    //Add the states and symbol of this transition to the appropriate sets.
    addState(from);
    addSymbol(sym);
    addState(to);

    return trans.addInternal(from,sym,to);
  }

  /**
   *
   * @brief add an internal transition to the NWA
   *
   * @param - it: internal transition to add to the NWA
   * @return false if the internal transition already exists in the NWA
   *
   */
    
  bool Nwa::addInternalTrans( Internal & it )
  {
    assert(Trans::getSource(it) < wali::WALI_BAD_KEY);
    assert(Trans::getInternalSym(it) < wali::WALI_BAD_KEY);
    assert(Trans::getTarget(it) < wali::WALI_BAD_KEY);

    //Add the states and symbol of this transition to the appropriate sets.
    addState(Trans::getSource(it));
    addSymbol(Trans::getInternalSym(it));
    addState(Trans::getTarget(it));

    return trans.addInternal(it);
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
    
  bool Nwa::removeInternalTrans( State from, Symbol sym, State to )
  {
    assert(from < wali::WALI_BAD_KEY);
    assert(sym < wali::WALI_BAD_KEY);
    assert(to < wali::WALI_BAD_KEY);

    if(! isState(from)
       || (! isSymbol(sym) && sym != EPSILON && sym != WILD)
       || ! isState(to) )
      return false;

    return trans.removeInternal(from,sym,to);
  }

  /**
   *
   * @brief remove an internal transition from the NWA
   *
   * @param - it: the internal transition to remove
   * @return false if the internal transition does not exist in the NWA
   *
   */
    
  bool Nwa::removeInternalTrans( const Internal & it )
  {
    assert(Trans::getSource(it) < wali::WALI_BAD_KEY);
    assert(Trans::getInternalSym(it) < wali::WALI_BAD_KEY);
    assert(Trans::getTarget(it) < wali::WALI_BAD_KEY);

    if(! isState(Trans::getSource(it))
       || ! isSymbol(Trans::getInternalSym(it))
       || ! isState(Trans::getTarget(it)) )
      return false;

    return trans.removeInternal(it);
  }

  /**
   *
   * @brief returns the number of internal transitions associated with this NWA
   *
   * @return the number of internal transitions associated with this NWA
   *
   */
    
    
  size_t Nwa::sizeInternalTrans( ) const
  {
    return trans.sizeInternal();
  }

  //Return Transitions

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
    
  bool Nwa::addReturnTrans( State from, State pred, Symbol sym, State to )
  {
    assert(from < wali::WALI_BAD_KEY);
    assert(pred < wali::WALI_BAD_KEY);
    assert(sym < wali::WALI_BAD_KEY);
    assert(to < wali::WALI_BAD_KEY);

    assert( sym != EPSILON ); //An Epsilon symbol on a return doesn't make sense.

    //Add the states and symbol of this transition to the approprite stes.
    addState(from);
    addState(pred);
    addSymbol(sym);
    addState(to);

    return trans.addReturn(from,pred,sym,to);
  }

  /**
   *
   * @brief add a return transition to the NWA
   *
   * @param - rt: return transition to add to the NWA
   * @return false if the return transition already exists in the NWA
   *
   */
    
  bool Nwa::addReturnTrans( Return & rt )
  {
    assert(Trans::getExit(rt) < wali::WALI_BAD_KEY);
    assert(Trans::getCallSite(rt) < wali::WALI_BAD_KEY);
    assert(Trans::getReturnSym(rt) < wali::WALI_BAD_KEY);
    assert(Trans::getReturnSite(rt) < wali::WALI_BAD_KEY);

    assert( Trans::getReturnSym(rt) != EPSILON ); //An Epsilon symbol on a return doesn't make sense.

    //Add the states and symbol of this transition to the appropriate sets.
    addState(Trans::getExit(rt));
    addState(Trans::getCallSite(rt));
    addSymbol(Trans::getReturnSym(rt));
    addState(Trans::getReturnSite(rt));

    return trans.addReturn(rt);
  }

  /**
   *
   * @brief remove a return transition from the NWA
   *
   * @param - from: of the state the edge departs from
   * @param - sym: the symbol labeling the edge
   * @param - to: the state the edge arrives to  
   * @return false if the return transition does not exist in the NWA
   *
   */
    
  bool Nwa::removeReturnTrans( State from, Symbol sym, State to )
  {
    assert(from < wali::WALI_BAD_KEY);
    assert(sym < wali::WALI_BAD_KEY);
    assert(to < wali::WALI_BAD_KEY);

    if(! isState(from) || ! isSymbol(sym) || ! isState(to) ||  sym == EPSILON )
      return false;
     
    bool removed = false;
    const Trans::States preds = trans.getCallSites(from,to);
    for( Trans::StateIterator it = preds.begin(); it != preds.end(); it++ )
    {
      if(! isState(*it) )
        return false;
      removed = removed || trans.removeReturn(from,*it,sym,to);
    }
    return removed;
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
    
  bool Nwa::removeReturnTrans( State from, State pred, Symbol sym, State to )
  {
    assert(from < wali::WALI_BAD_KEY);
    assert(pred < wali::WALI_BAD_KEY);
    assert(sym < wali::WALI_BAD_KEY);
    assert(to < wali::WALI_BAD_KEY);

    if(! isState(from) || ! isState(pred) || ! isSymbol(sym) || ! isState(to) 
       || sym == EPSILON )
      return false;

    return trans.removeReturn(from,pred,sym,to);
  }

  /**
   *
   * @brief remove a return transition from the NWA
   *
   * @param - rt: the return transition to remove
   * @return false if the return transition does not exist in the NWA
   *
   */
    
  bool Nwa::removeReturnTrans( const Return & rt )
  {
    assert(Trans::getExit(rt) < wali::WALI_BAD_KEY);
    assert(Trans::getCallSite(rt) < wali::WALI_BAD_KEY);
    assert(Trans::getReturnSym(rt) < wali::WALI_BAD_KEY);
    assert(Trans::getReturnSite(rt) < wali::WALI_BAD_KEY);

    if(! isState(Trans::getExit(rt))
       || ! isState(Trans::getCallSite(rt))
       || ! isSymbol(Trans::getReturnSym(rt))
       || ! isState(Trans::getReturnSite(rt)) 
       || Trans::getReturnSym(rt) == EPSILON )
      return false;

    return trans.removeReturn(rt);
  }

  /**
   *
   * @brief returns the number of return transitions associated with this NWA
   *
   * @return the number of return transitions associated with this NWA
   *
   */
    
    
  size_t Nwa::sizeReturnTrans( ) const
  {
    return trans.sizeReturn();
  }

  //Building NWAs


  /***
   * @brief constructs an NWA which is the projection of the given NWA to the states
   * provided
   */
    
  void Nwa::projectStates(const Nwa & other, const StateSet &prjStates)
  {
    //copy data from other
    states = other.states;
    symbols = other.symbols;
    trans = other.trans;

    //for( std::set<St>::const_iterator it = prjStates.begin(); it!=prjStates.end(); it++) {
    //  bool b = removeState(*it);
    //  if( !b ) {
    //    std::cout << "State not found " << *it << "\n";
    //  }
    //  assert(b && "State not found in projectStates");
    //}
    StateSet project_out;
    for(StateIterator it = beginStates(); it!=endStates(); it++) {
      if( prjStates.count(*it) == 0 ) {
        project_out.insert(*it);
      }
    }

    for( StateSet::const_iterator it = project_out.begin();
         it!=project_out.end(); it++ ) {
      bool b = removeState(*it);
      if( !b ) {
        std::cout << "State not found " << *it << "\n";
      }
      assert(b && "State not found in projectStates");
    }
    return;
  }


    
  bool Nwa::isTransitionPossible( const State &src, const Symbol &sym, const State &tgt) 
  {
    (void) src;
    (void) sym;
    (void) tgt;
    return true;
  }


  void Nwa::removeImplicitTransitions()
  {
    // This proceeds in two steps: detects stuck states, then removes
    // them and transitions to them.

    // First we detect stuck states. We first assume that every state
    // is stuck. Then proceed through all transitions in the graph
    // (first calls, then internals, then returns); if we see a transition
    // p --> q where p!=q, then we know that p isn't a stuck state.
    // Any state left has the property that every transition out of the
    // state goes to the "real" stuck state or back to itself.
    // (However, we can only remove a state if it accepts iff the real
    // stuck state accepts.)
    StateSet stuckStates(beginStates(), endStates());

    for( CallIterator call = trans.beginCall(); call != trans.endCall(); ++call)
    {
      if( Trans::getCallSite(*call) != Trans::getEntry(*call) )
      {
        stuckStates.erase(Trans::getCallSite(*call));
      }
    }

    for( InternalIterator internal = trans.beginInternal();
         internal != trans.endInternal(); ++internal)
    {
      if( Trans::getSource(*internal) != Trans::getTarget(*internal) )
      {
        stuckStates.erase(Trans::getSource(*internal));
      }
    }

    for( ReturnIterator ret = trans.beginReturn(); ret != trans.endReturn(); ++ret)
    {
      if( Trans::getExit(*ret) != Trans::getReturnSite(*ret) )
      {
        stuckStates.erase(Trans::getExit(*ret));
      }
    }

    // Now we have a list of stuck states. Go through and remove each,
    // but only if the stuck state is not final.
    for(  StateSet::iterator stuck = stuckStates.begin();
          stuck != stuckStates.end(); ++stuck)
    {
      if( ! isFinalState(*stuck) )
      {
        removeState(*stuck);
      }
    }
  }

    
  void Nwa::pruneUnreachableForward(const StateSet & sources)
  {
    std::deque<State> worklist(sources.begin(), sources.end());  
    StateSet unreachables = getStates();
    StateSet visited;
    for( StateIterator it = sources.begin(); it!=sources.end(); it++) {
      visited.insert(*it);
    }   

    while( ! worklist.empty() ) {
      State src = worklist.front();
      worklist.pop_front();
      // book keeping
      unreachables.erase(src);
      // for each target
      std::set<std::pair<Symbol,State> > succs = query::getTargets(*this, src);
      for( std::set<std::pair<Symbol,State> >::const_iterator it = succs.begin(); it!=succs.end(); it++) {
        // if not visited
        if(visited.count(it->second) == 0 ) {
          worklist.push_back(it->second);
          visited.insert(it->second);
        }
      }

      // for each entry
      succs = query::getEntries(*this, src);
      for( std::set<std::pair<Symbol,State> >::const_iterator it = succs.begin(); it!=succs.end(); it++) {
        // if not visited
        if(visited.count(it->second) == 0 ) {
          worklist.push_back(it->second);
          visited.insert(it->second);
        }
      }
      // for each return transition
      const Returns & returns = trans.getReturns();
      for( ReturnIterator it = returns.begin(); it != returns.end(); it++ )
      {
        // if src is the exit or the call
        if( src == Trans::getExit(*it) || src == Trans::getCallSite(*it) )
        {
          // and if the exit and call are already visited
          if( visited.count( Trans::getExit(*it) ) > 0 && visited.count( Trans::getCallSite(*it) ) > 0  
              // but the return site is not visited
              && visited.count(Trans::getReturnSite(*it) ) == 0 ) {

            worklist.push_back(Trans::getReturnSite(*it) );
            visited.insert(Trans::getReturnSite(*it));
          }
        }
      }

    }

    for(StateIterator it = unreachables.begin(); it!=unreachables.end(); it++) {
      removeState(*it);
    }
    return;
  }

       
  void Nwa::pruneUnreachableBackward(const StateSet & targets)
  {      
    std::deque<State> worklist(targets.begin(), targets.end());  
    StateSet unreachables = getStates();
    StateSet visited;
    for( StateIterator it = targets.begin(); it!=targets.end(); it++) {
      visited.insert(*it);
    }   

    while( ! worklist.empty() ) {
      State src = worklist.front();
      worklist.pop_front();
      // book keeping
      unreachables.erase(src);
      // for each source
      std::set<std::pair<State,Symbol> > succs = query::getSources(*this, src);
      for( std::set<std::pair<Symbol,State> >::const_iterator it = succs.begin(); it!=succs.end(); it++) {
        // if not visited
        if(visited.count(it->first) == 0 ) {
          worklist.push_back(it->first);
          visited.insert(it->first);
        }
      }

      // for each call successor state
      succs = query::getCallSites(*this, src);
      for( std::set<std::pair<State,Symbol> >::const_iterator it = succs.begin(); it!=succs.end(); it++) {
        // if not visited
        if(visited.count(it->first) == 0 ) {
          worklist.push_back(it->first);
          visited.insert(it->first);
        }
      }
      // for each return transition
      const Returns & returns = trans.getReturns();
      for( ReturnIterator it = returns.begin(); it != returns.end(); it++ )
      {
        // if src is the return
        if( src == Trans::getReturnSite(*it) )
        {
          // and if the exit not visited
          if( visited.count( Trans::getExit(*it) ) == 0 ) {

            worklist.push_back(Trans::getExit(*it) );
            visited.insert(Trans::getExit(*it));
          }

          // if the call not visited
          if( visited.count( Trans::getCallSite(*it) ) == 0 ) {

            worklist.push_back(Trans::getCallSite(*it) );
            visited.insert(Trans::getCallSite(*it));
          }
        }
      }
    }

    for(StateIterator it = unreachables.begin(); it!=unreachables.end(); it++) {
      removeState(*it);
    }
    return;

  }

    
  void Nwa::pruneUnreachableInitial() {
    pruneUnreachableForward( getInitialStates() );
  }

    
  void Nwa::pruneUnreachableFinal() {
    pruneUnreachableBackward( getFinalStates() );
  }

    
  void Nwa::chop()
  {
    pruneUnreachableInitial();
    pruneUnreachableFinal();

  }






  /**
   *
   * @brief tests whether this NWA is deterministic 
   *
   * @return true if this NWA is deterministic, false otherwise
   *
   */
    
  bool Nwa::_private_isDeterministic_( ) const
  {
    //An NWA is not deterministic if there is not exactly one initial state
    if( sizeInitialStates() > 1 ) 
      return false;
      
    //An NWA is not deterministic if there are epsilon transitions. 
    for( InternalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
    {
      if( Trans::getInternalSym(*iit) == EPSILON ) {
        //std::cout << "There are epsilon transitions";
        return false;
      }
    }
      
    //TODO: optimize this, it can be very expensive
    //An NWA is not deterministic if there are multiple transitions of some
    //type that are identical except for the to state.
    //For each state/symbol pair check that <= 1 transition leaves the state with that symbol.
    for( SymbolIterator it = symbols.beginSymbols(); it != symbols.endSymbols(); it++ )
    {
      for( StateIterator sit = beginStates(); sit != endStates(); sit++ )
      {
        //Check call transitions.
        int count_found = 0;
        bool wild = false;
        for( CallIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
        {
          if( wild && cit->first == *sit && Trans::getCallSym(*cit) == WILD ) {
            // Two wild transitions from the same site
            return false;
          }
            
          //Wild symbol 
          if( cit->first == *sit && Trans::getCallSym(*cit) == WILD )
            wild = true;  

          //Keep a count of multiple transitions with the same from
          //state and symbol(that is not epsilon).
          else if( (Trans::getCallSite(*cit) == (*sit)) && (Trans::getCallSym(*cit) == *it) )
            count_found++;
        }
        if( count_found > 1 ) {
          //std::cout << "isDeterministic returning on line " << (__LINE__+1) << "\n";
          return false;
        }
        else if( wild && count_found > 0 ) {
          //std::cout << "isDeterministic returning on line " << (__LINE__+1) << "\n";
          return false;
        }

        //Check internal transitions.
        count_found = 0;
        wild = false;
        //std::cout << "About to look for internals\n";
        for( InternalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
        {
          //Wild symbol
          if( wild && iit->first == *sit && Trans::getInternalSym(*iit) == WILD ) {
            // Two wild transitions from the same state
            return false;
          }
            
          if( iit->first == *sit && Trans::getInternalSym(*iit) == WILD ) {
            //std::cout << "Found internal from " << key2str(iit->first) << " to "
            //          << key2str(iit->third) << " on " << key2str(iit->second) << "\n";
            wild = true;
          }

          //Keep a count of multiple transitions with the same from
          //state and symbol(that is not epsilon).
          else if( (Trans::getSource(*iit) == *sit) && (Trans::getInternalSym(*iit) == *it) ) {
            //std::cout << "Found internal from " << key2str(iit->first) << " to "
            //          << key2str(iit->third) << " on " << key2str(iit->second) << "\n";
            count_found++;
          }
        }
        if( count_found > 1 ) {
          //std::cout << "isDeterministic returning on line " << (__LINE__+1) << "\n";
          return false;
        }
        else if( wild && count_found > 0 ) {
          //std::cout << "isDeterministic returning on line " << (__LINE__+1) << "\n";
          return false;  
        }
        for( StateIterator pit = beginStates(); pit != endStates(); pit++ )
        {
          //Check return transitions.
          count_found = 0;
          wild = false;
          for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
          {
            //Wild symbol

            if( wild
                && rit->first == *sit
                && rit->second == *pit
                && Trans::getReturnSym(*rit) == WILD )
            {
              return false;
            }
              
            if( rit->first == *sit
                && rit->second == *pit
                && Trans::getReturnSym(*rit) == WILD )
            {
              wild = true;
            }

            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (Trans::getExit(*rit) == *sit) 
                     && (Trans::getCallSite(*rit) == *pit) 
                     && (Trans::getReturnSym(*rit) == *it) )
              count_found++;
          }
          if( count_found > 1 ) {
            //std::cout << "isDeterministic returning on line " << (__LINE__+1) << "\n";
            return false;
          }
          else if( wild && count_found > 0 ) {
            //std::cout << "isDeterministic returning on line " << (__LINE__+1) << "\n";
            return false;
          }
        }
      }
    }

    return true;
  }
   
  /**
   * 
   * @brief intersects client information 
   *
   * @param - first: the NWA in which to look up the client information for 
   *                'call1' and 'entry1'
   * @param - call1: the call site associated with the first entry whose client 
   *                  information to intersect
   * @param - entry1: the first entry point whose client information to intersect
   * @param - second: the NWA in which to look up the client information for
   *                  'call2' and 'entry2'
   * @param - call2: the call site associated with the second entry whose client
   *                  information to intersect
   * @param - entry2: the second entry point whose client information to intersect
   * @param - resSym: the symbol associated with the transition that is being created
   * @param - resSt: the state which will receive the computed client information
   *
   */
    
  void Nwa::intersectClientInfoCall( Nwa const & first, State call1, State entry1, 
                                     Nwa const & second, State call2, State entry2, 
                                     Symbol resSym, State resSt )
  {
    (void) first, (void) call1, (void) entry1, (void) second;
    (void) call2, (void) entry2, (void) resSym, (void) resSt;
    //Note: When overriding this method your metric must combine any client information associated
    // with the target states of the two transitions and set the client information of result
    // to that value.
  }

  /**
   * 
   * @brief intersects client information 
   *
   * @param - first: the NWA in which to look up the client information for 
   *                'src1' and 'tgt1'
   * @param - src1: the source associated with the first target whose client 
   *                  information to intersect
   * @param - tgt1: the first target whose client information to intersect
   * @param - second: the NWA in which to look up the client information for
   *                'src2' and 'tgt2'
   * @param - src2: the source associated with the second target whose client
   *                  information to intersect
   * @param - tgt2: the second target whose client information to intersect
   * @param - resSym: the symbol associated with the transition that is being created
   * @param - resSt: the state which will receive the computed client information
   * @param - result: the client information resulting from the intersection of
   *                'tgt1' and 'tgt2'
   *
   */
    
  void Nwa::intersectClientInfoInternal( Nwa const & first, State src1, State tgt1, 
                                         Nwa const & second, State src2, State tgt2, 
                                         Symbol resSym, State resSt )
  {
    (void) first, (void) src1, (void) tgt1, (void) second, (void) src2;
    (void) tgt2, (void) resSym, (void) resSt;
    //Note: When overriding this method your metric must combine any client information associated
    // with the target states of the two transitions and set the client information of result
    // to that value.
  }

  /**
   * 
   * @brief intersects client information 
   *
   * @param - first: the NWA in which to look up the client information for 
   *                'exit1', 'call1', and 'ret1'
   * @param - exit1: the exit point associated with the first return site whose client 
   *                  information to intersect
   * @param - call1: the call site associated with the first return site whose client
   *                  information to intersect
   * @param - ret1: the first return site whose client information to intersect
   * @param - second: the NWA in which to look up the client information for
   *                'exit2', 'call2', and 'ret2'
   * @param - exit2: the exit point associated with the second return site whose client
   *                  information to intersect
   * @param - call2: the call site associated with the second return site whose client
   *                  information to intersect
   * @param - ret2: the second return site whose client information to intersect
   * @param - resSym: the symbol associated with the transition that is being created
   * @param - resSt: the state which will receive the computed client information
   * @param - result: the client information resulting from the intersection of
   *                'ret1' and 'ret2'
   *
   */
    
  void Nwa::intersectClientInfoReturn( Nwa const & first, State exit1, State call1, State ret1,
                                       Nwa const & second, State exit2, State call2, State ret2,
                                       Symbol resSym, State resSt )
  {
    (void) first, (void) exit1, (void) call1, (void) ret1, (void) second;
    (void) exit2, (void) call2, (void) ret2, (void) resSym, (void) resSt;
    //Note: When overriding this method your metric must combine any client information associated
    // with the target states of the two transitions and set the client information of result
    // to that value.
  }

  /**
   * 
   * @brief intersect states
   * 
   * @param - first: the NWA in which to look up information about 'state1'
   * @param - state1: the first state to intersect
   * @param - second: the NWA in which to look up information about 'state2'
   * @param - state2: the second state to intersect
   * @param - resSt: the state that results from performing the intersection
   *
   */
    
  bool Nwa::stateIntersect( Nwa const & first, State state1, Nwa const & second, State state2, 
                            State & resSt, ClientInfoRefPtr & resCI ) 
  {
    (void) first, (void) second, (void) resCI;
      
    //Note: When overriding this method your metric must determine whether the
    //      given states are compatible and set resSt to the appropriate state.

    std::stringstream ss;
    ss << "(key#"  << state1 << "," << state2 << ")";
    resSt = getKey(ss.str());

    //resSt = getKey(state1,state2);

    return true;
  }

  /**
   * 
   * @brief intersect symbols
   * 
   * @param - first: the NWA in which to look up information about 'sym1'
   * @param - sym1: the first symbol to intersect
   * @param - second: the NWA in which to look up information about 'sym2'
   * @param - sym2: the second symbol to intersect
   * @param - resSym: the symbol that results from performing the intersection
   *
   */
    
  bool Nwa::transitionIntersect( Nwa const & first, Symbol sym1, Nwa const & second, Symbol sym2, Symbol & resSym )
  {
    (void) first, (void) second;
      
    //Note: When overriding this method your metric must determine whether the
    //      given symbols are compatible and set result to the appropriate symbol.

    //Epsilons are treated separately in the algorithms, so epsilons match nothing.
    if( sym1 == EPSILON || sym2 == EPSILON )
      return false;
    //Wild symbols match everything.
    if( sym1 == WILD )
    {
      resSym = sym2;
      return true;
    }
    if( sym2 == WILD )
    {
      resSym = sym1;
      return true;
    }
    if( sym1 == sym2 ) //Symbols only match the exact same symbol (except for epsilons and wilds).
    {
      resSym = sym1;
      return true;
    }
    else
      return false;
  }

  /**
   * 
   * @brief merge states for determinize
   * 
   * @param - nwa: the NWA in which to look up information about the states
   * @param - binRel: the states to merge
   * @param - resCI: the client info that results from performing the merge
   *
   */
    
  void Nwa::mergeClientInfo( Nwa const & first, 
                             wali::relations::RelationTypedefs<State>::BinaryRelation const & binRel, 
                             State resSt, ClientInfoRefPtr & resCI )
  {
    (void) first, (void) binRel, (void) resSt, (void) resCI;
    //Note: When overriding this method your metric must combine any client information associated
    //       with the states of the binary relation and set the client information of resSt to 
    //       that value.
  }

  /**
   * 
   * @brief merges clientInfo for determinize
   * 
   * @param - nwa: the NWA in which to look up information about the states
   * @param - binRelCall: the states that compose the call site for this call transition
   * @param - binRelEntry: the states to merge that compose the entry point for this call transition
   * @param - callSt: the call site of the transition that is being created
   * @param - resSym: the symbol associated with the transition that is being created
   * @param - resSt: the state resulting from the merge
   * @param - resCI: the client info that results from performing the merge
   *
   */
    
  void Nwa::mergeClientInfoCall( Nwa const & nwa, 
                                 wali::relations::RelationTypedefs<State>::BinaryRelation const & binRelCall, 
                                 wali::relations::RelationTypedefs<State>::BinaryRelation const & binRelEntry,
                                 State callSt, Symbol resSym, State resSt, ClientInfoRefPtr & resCI )
  {
    (void) nwa, (void) binRelCall, (void) binRelEntry, (void) callSt;
    (void) resSym, (void) resSt, (void) resCI;
    //Note: When overriding this method your metric must combine any client information associated
    //       with the states of the binary relation and set the client information of resSt to 
    //       that value.
  }

  /**
   * 
   * @brief merges clientInfo for determinize
   * 
   * @param - nwa: the NWA in which to look up information about the states
   * @param - binRelSource: the states that compose the source for this internal transition
   * @param - binRelTarget: the states to merge that compose the target for this internal transition
   * @param - sourceSt: the source of the transition that is being created
   * @param - resSym: the symbol associated with the transition that is being created
   * @param - resSt: the state resulting from the merge
   * @param - resCI: the client info that results from performing the merge
   *
   */
    
  void Nwa::mergeClientInfoInternal( Nwa const & nwa, 
                                     wali::relations::RelationTypedefs<State>::BinaryRelation const & binRelSource, 
                                     wali::relations::RelationTypedefs<State>::BinaryRelation const & binRelTarget,
                                     State sourceSt, Symbol resSym, State resSt, ClientInfoRefPtr & resCI )
  {
    (void) nwa, (void) binRelSource, (void) binRelTarget, (void) sourceSt;
    (void) resSym, (void) resSt, (void) resCI;
    //Note: When overriding this method your metric must combine any client information associated
    //       with the states of the binary relation and set the client information of resSt to 
    //       that value.
  }

  /**
   * 
   * @brief merges clientInfo for determinize
   * 
   * @param - nwa: the NWA in which to look up information about the states
   * @param - binRelExit: the states that compose the exit point for this return transition
   * @param - binRelCall: the states that compose the call site for this return transition
   * @param - binRelReturn: the states to merge that compose the return site for this return transition
   * @param - exitSt: the exit point of the transition that is being created
   * @param - callSt: the call site of the transition that is being created
   * @param - resSym: the symbol associated with the transition that is being created
   * @param - resSt: the state resulting from the merge
   * @param - resCI: the client info that results from performing the merge
   *
   */
    
  void Nwa::mergeClientInfoReturn( Nwa const & nwa, 
                                   wali::relations::RelationTypedefs<State>::BinaryRelation const & binRelExit,
                                   wali::relations::RelationTypedefs<State>::BinaryRelation const & binRelCall, 
                                   wali::relations::RelationTypedefs<State>::BinaryRelation const & binRelReturn,
                                   State exitSt, State callSt, Symbol resSym, State resSt, ClientInfoRefPtr & resCI )
  {
    (void) nwa, (void) binRelExit, (void) binRelCall, (void) binRelReturn;
    (void) exitSt, (void) callSt, (void) resSym, (void) resSt, (void) resCI;
    //Note: When overriding this method your metric must combine any client information associated
    //       with the states of the binary relation and set the client information of resSt to 
    //       that value.
  }

  //Using NWAs

    

      
  /**
   *
   * @brief tests whether the language accepted by this NWA is empty
   *
   * @return true if the language accepted by this NWA is empty
   *
   */
    
  bool Nwa::_private_isEmpty_( ) const
  {
    //An automaton with no initial states must accept only the empty language.
    if( sizeInitialStates() == 0 )
      return true;

    //An automaton with no final states must accept only the empty language.
    if( sizeFinalStates() == 0 )
      return true;

    //An automaton with an initial state that is a final state is definitely not empty.
    for( StateIterator it = beginInitialStates(); it != endInitialStates(); it++ )
    {
      if( isFinalState(*it) )
        return false;
    }

    //Any final state reachable from an initial state shows this NWA is not empty.

    Reach wgts = Reach(true);

    //Make a WFA with transitions from initial state to final state for each 
    //initial state of the NWA.
    wali::wfa::WFA initials;
    //Set up initial state.
    wali::Key init = nwa_pds::getProgramControlLocation();
    initials.addState(init,wgts.zero());
    initials.setInitialState(init);
    //Set up final state.
    wali::Key fin = getKey("fin");
    initials.addState(fin,wgts.zero());
    initials.addFinalState(fin);
    //Add transitions.
    for( StateIterator it = beginInitialStates(); it != endInitialStates(); it++ )
    {
      initials.addTrans(init,*it,fin,wgts.one());
    }

#if 1 // Allow pending returns
    for( StateIterator it = beginInitialStates(); it != endInitialStates(); it++ )
    {
      initials.addTrans(fin,*it,fin,wgts.one());
    }
#endif

    //std::cout << "Initials:\n";
    //initials.print_dot(std::cout);
    //std::cout << "\n";

    //Make a WFA with transitions from initial state to final state for each
    //final state of the NWA.
    wali::wfa::WFA finals;
    //Set up initial state.
    finals.addState(init,wgts.zero());
    finals.setInitialState(init);
    //Set up final state.
    finals.addState(fin,wgts.zero());
    finals.addFinalState(fin);
    //Add transitions.
    for( StateIterator it = beginFinalStates(); it != endFinalStates(); it++ )
    {
      finals.addTrans(init,*it,fin,wgts.one());
    }

#if 1 // Allow pending calls
    for( StateIterator it = beginStates(); it != endStates(); it++ )
    {
      finals.addTrans(fin,*it,fin,wgts.one());
    }
#endif

    //std::cout << "Finals:\n";
    //finals.print_dot(std::cout);
    //std::cout << "\n";

#if 1
    ReachGen reach;
    //Perform poststar to determine reachability.
    //std::cerr << "Before poststar\n";
    wali::wfa::WFA postInitials = poststar(initials,reach);
    //std::cerr << "After poststar!\n";

    //std::cout << "poststar(initials):\n";
    //postInitials.print_dot(std::cout);
    //std::cout << "\n";

    //intersect poststar(initials) and finals
    //std::cerr << "Before intersect\n";
    wali::wfa::WFA reachable = postInitials.intersect(finals);
    //std::cerr << "After intersect!\n";

    //std::cout << "intersection:\n";
    //reachable.print_dot(std::cout);
    //std::cout << "\n";
#else
    //Perform poststar to determine reachability.
    ReachGen<Client> reach;
    //std::cerr << "Before prestar\n";
    wali::wfa::WFA preFinals = prestar(finals,reach);
    //std::cerr << "After prestar!\n";

    //intersect poststar(initials) and finals
    //std::cerr << "Before WFA intersect\n";
    wali::wfa::WFA reachable = preFinals.intersect(initials);
    //std::cerr << "After WFA intersect!\n";
#endif

    reachable.prune();
    //reachable.print_dot(std::cout << "Reachable after prune: \n") << "\n";

    //If there are no transitions in this intersection, then the NWA is empty.
    wali::wfa::TransCounter tc = wali::wfa::TransCounter();
    reachable.for_each( tc );
      
    if( tc.getNumTrans() == 0 )
      return true;  //There was no path from an initial state to a final state in the NWA.
    else
      return false; //There was a path from an initial state to a final state in the NWA.
  }

  /**
   *
   * @brief perform the prestar reachability query defined by the given WFA
   *
   * @param - input: the starting point of the reachability query
   * @param - wg: the functions to use in generating weights
   * @return the WFA resulting from performing the prestar reachability query 
   *
   */
    
  wali::wfa::WFA Nwa::prestar( wali::wfa::WFA const & input,
                               WeightGen const & wg,
                               ref_ptr< wali::Worklist<wali::wfa::ITrans> > worklist) const
  {
    WPDS wpds = nwa_pds::NwaToWpdsCalls(*this, wg);
    if (worklist != NULL) {
      wpds.setWorklist(worklist);
    }
    return wpds.prestar(input);
  }

  /**
   *
   * @brief perform the prestar reachability query defined by the given WFA
   *
   * Note: Any transitions in output before the query will be there after the query but
   * will have no effect on the reachability query.
   *
   * @param - input: the starting point of the reachability query
   * @param - ouput: the result of performing the reachability query
   * @param - wg: the functions to use in generating weights
   *
   */
    
  void Nwa::prestar( wali::wfa::WFA const & input,
                     wali::wfa::WFA & output,
                     WeightGen const & wg,
                     ref_ptr< wali::Worklist<wali::wfa::ITrans> > worklist) const
  {
    WPDS wpds = nwa_pds::NwaToWpdsCalls(*this, wg);
    if (worklist != NULL) {
      wpds.setWorklist(worklist);
    }
    wpds.prestar(input, output);
  }

  /**
   *
   * @brief perform the poststar reachability query defined by the given WFA
   *
   * @param - input: the starting point of the reachability query
   * @param - wg: the functions to use in generating weights
   * @return the WFA resulting from performing the poststar reachability query
   *
   */
    
  wali::wfa::WFA Nwa::poststar( wali::wfa::WFA const & input,
                                WeightGen const & wg,
                                ref_ptr< wali::Worklist<wali::wfa::ITrans> > worklist) const
  {
    WPDS wpds = nwa_pds::NwaToWpdsCalls(*this, wg);
    if (worklist != NULL) {
      wpds.setWorklist(worklist);
    }
    return wpds.poststar(input);
  }

  /**
   *
   * @brief perform the poststar reachability query defined by the given WFA
   * 
   * Note: Any transitions in output before the query will be there after the query but
   * will have no effect on the reachability query.
   *
   * @param - input: the starting point of the reachability query
   * @param - output: the result of performing the reachability query
   * @param - wg: the functions to use in generating weights
   *
   */
    
  void Nwa::poststar( wali::wfa::WFA const & input,
                      wali::wfa::WFA & output,
                      WeightGen const & wg,
                      ref_ptr< wali::Worklist<wali::wfa::ITrans> > worklist) const
  {
    WPDS wpds = nwa_pds::NwaToWpdsCalls(*this, wg);
    if (worklist != NULL) {
      wpds.setWorklist(worklist);
    }
    wpds.poststar(input, output);
  }

  //Utilities	

  /**
   *
   * @brief print the NWA
   *
   * @param - o: the output stream to print to
   * @return the output stream that was printed to
   *
   */
    
  std::ostream & Nwa::print( std::ostream & o ) const
  {
    //TODO: flag for explicit stuck state transition printing?
    states.print(o);
    symbols.print(o);
    trans.print(o);
    return o;
  }

  /**
   *
   * @brief print the NWA in dot format
   *
   * @param - o: the output stream to print to
   * @return the output stream that was printed to
   *
   */
    
  std::ostream & Nwa::print_dot( std::ostream & o, std::string title, bool abbrev ) const
  {
    //TODO: flag for explicit stuck state transition printing?
    o << "digraph \"NWA\" { \n";
#if 0
    o << "    label = \"" << title <<"\" ;\n";
    o << "    subgraph cluster_key {\n";
    o << "        label = \"Key\";\n";
    o << "        style=filled;\n";
    o << "        color=white;\n";
    o << "        dummy [shape=box];";
    o << "        call -> entry [color=green];\n";
    o << "        call -> dummy [color=blue];\n";
    o << "        exit -> dummy [color=red];\n";
    o << "        dummy -> return [style=dashed];\n";
    o << "    }\n";
#else
    (void) title;
#endif

    //initial state
    StateSet initials = getInitialStates();
    for(  StateSet::const_iterator it = initials.begin(); it != initials.end(); it++ )
    {
      printKey(o << "\"",*it,abbrev)<<"__precursor__\" [ label=\"\",style=\"invis\" ]";
      printKey(o << "\"",*it,abbrev)<<"__precursor__\"";
      o << "->";
      printKey(o << "\"",*it,abbrev)<<"\"";
    }

    //final states
    StateSet finals = getFinalStates();
    for(  StateSet::const_iterator it = finals.begin(); it != finals.end(); it++ ) 
    {
      printKey(o << "\"",*it,abbrev) <<"\" [ peripheries=2 ]";
    }

    //Print call transitions.
    o << "//Delta_c: \n";
    CallIterator cit = beginCallTrans();
    CallIterator citEND = endCallTrans();
    for(; cit != citEND; cit++ )
    {
      printKey(o << "\"",Trans::getCallSite(*cit),abbrev) << "\"";
      o << "->";
      printKey(o << "\"",Trans::getEntry(*cit),abbrev) << "\"";
      o << "[";
      o << " label=\"";
      printKey(o,Trans::getCallSym(*cit),abbrev);
      o << "\"";
      o << " color=green";
      o << "];\n";        
    }
    o << " \n";
    //Print internal transitions.
    o << "// Delta_i:\n" ;
    InternalIterator iit = beginInternalTrans();
    InternalIterator iitEND = endInternalTrans();
    for(; iit != iitEND; iit++ )
    {
      printKey(o << "\"",Trans::getSource(*iit),abbrev) << "\"";
      o << "->";
      printKey(o << "\"",Trans::getTarget(*iit),abbrev) << "\"";
      o << "[";
      o << " label=\"";
      printKey(o,Trans::getInternalSym(*iit),abbrev);
      o << "\"";
      o << "];\n";
    }
    o << " \n";


    //Print return transitions. (This is so dumbly done)
    o << "// Delta_r:\n";
    for(StateIterator exitit = beginStates(); exitit != endStates(); ++exitit)
    {
      for(StateIterator returnit = beginStates(); returnit != endStates(); ++returnit)
      {
        // For each Sym, holds the set of call predecessors for which
        // (exit, pred, sym, return) is in delta_r
        std::map<Symbol, StateSet > returns;

        // Populate it:
        for(ReturnIterator cur_trans = beginReturnTrans(); cur_trans != endReturnTrans(); ++cur_trans)
        {
          if(*exitit == Trans::getExit(*cur_trans)
             && *returnit == Trans::getReturnSite(*cur_trans))
          {
            returns[Trans::getReturnSym(*cur_trans)].insert(Trans::getCallSite(*cur_trans));
          }
        }

        // Now add an edge for each return symbol
        for( std::map<Symbol, StateSet >::const_iterator cur_trans = returns.begin();
             cur_trans != returns.end(); ++cur_trans)
        {
          std::stringstream ss;
          printKey(ss, cur_trans->first,abbrev);
          ss << ": ";
          for( StateSet::const_iterator prediter = cur_trans->second.begin();
               prediter != cur_trans->second.end(); ++prediter)
          {
            printKey(ss, *prediter, abbrev) << ", ";
          }

          // Now ss holds the label for the edge exit->return
          printKey(o << "\"", *exitit, abbrev) << "\" ->";
          printKey(o << "\"", *returnit, abbrev) << "\"";
          o << "[ label=\"" << ss.str() << "\" color=red];\n";
        }
      }
    }

    o << "\n";

    o << "}\n";
    o.flush();
    return o;
  }

  /**
   *
   * @brief tests whether this NWA is equivalent to the NWA 'other'
   *
   * @param - other: the NWA to compare this NWA to
   * @return true if this NWA is equivalent to the NWA 'other'
   *
   */
    
  bool Nwa::operator==( const Nwa & other ) const
  {
    return ( (states == other.states) 
             && (symbols == other.symbols) 
             && (trans == other.trans) );
  }

  //TODO: add methods like ...
  //virtual void for_each(ConstRuleFunctor &func) const;
  //virtual void for_each(RuleFunctor &func) const;
  //virtual void operator()(wali::wfa::ITrans *t);

  /**
   *
   * @brief add all the states in the given StateSet to the NWA
   *
   * @param - addStateSet: the StateSet that contains the states to add
   *
   */
    
    
  void Nwa::addAllStates( StateStorage addStateSet )
  {
    states.addAllStates(addStateSet);
  }

  /**
   *
   * @brief provide access to the beginning of the state set
   *
   * @return an iterator pointing to the beginning of the state set
   *
   */
    
    
  Nwa::StateIterator Nwa::beginStates( ) const
  {
    return states.beginStates();
  }

  /**
   * 
   * @brief provide access to the end of the state set
   *
   * @return an iterator pointing just past the end of the state set
   *
   */
    
    
  Nwa::StateIterator Nwa::endStates( ) const
  {
    return states.endStates();
  }

  /**
   *
   * @brief adds all of the states in the given StateSet to the initial state set 
   *        associated with this NWA
   *
   * @param - addStateSet: the state set whose initial states to add to this NWA's
   *                        initial state set
   *
   */
    
    
  void Nwa::addAllInitialStates( StateStorage addStateSet )
  {
    states.addAllInitialStates(addStateSet);
  }

  /**
   *
   * @brief provide access to the beginning of the initial state set
   *
   * @return an iterator pointing to the beginning of the initial state set
   *
   */ 
    
    
  Nwa::StateIterator Nwa::beginInitialStates( ) const
  {
    return states.beginInitialStates();
  }

  /**
   * 
   * @brief provide access to the end of the initial state set
   *
   * @return an iterator pointing just past the end of the initial state set
   *
   */
    
    
  Nwa::StateIterator Nwa::endInitialStates( ) const
  {
    return states.endInitialStates();
  }

  /**
   *
   * @brief adds all of the final states in the given StateSet to the final state set
   *        associated with this NWA
   *
   * @param - addStateSet: the StateSet whose final states to add to this NWA's
   *                        final state set
   *
   */
    
    
  void Nwa::addAllFinalStates( StateStorage addStateSet )
  {
    states.addAllFinalStates(addStateSet);
  }

  /**
   *
   * @brief provide access to the beginning of the final state set
   *
   * @return an iterator pointing to the beginning of the final state set
   *
   */
    
    
  Nwa::StateIterator Nwa::beginFinalStates( ) const
  {
    return states.beginFinalStates();
  }

  /**
   * 
   * @brief provide access to the end of the final state set
   *
   * @return an iterator pointing just past the end of the final state set
   *
   */
    
    
  Nwa::StateIterator Nwa::endFinalStates( ) const
  {
    return states.endFinalStates();
  }

  /**
   *
   * @brief add the given symbols to the NWA
   *
   * @param - addSymbolSet: the symbols to add
   *
   */
    
    
  void Nwa::addAllSymbols( SymbolStorage addSymbolSet )
  {
    symbols.addAllSymbols(addSymbolSet);
  }

  /**
   *
   * @brief provide access to the beginning of the symbol set
   *
   * @return an iterator pointing to the beginning of the symbol set
   *
   */
    
    
  Nwa::SymbolIterator Nwa::beginSymbols( ) const
  {
    return symbols.beginSymbols();
  }

  /**
   * 
   * @brief provide access to the end of the symbol set
   *
   * @return an iterator pointing just past the end of the symbol set
   *
   */
    
    
  Nwa::SymbolIterator Nwa::endSymbols( ) const
  {
    return symbols.endSymbols();
  }

  /**
   *
   * @brief provide access to the beginning of the call transition set
   *
   * @return an iterator pointing to the beginning of the call transition set
   *
   */
    
    
  Nwa::CallIterator Nwa::beginCallTrans( ) const
  {
    return trans.beginCall();
  }

  /**
   * 
   * @brief provide access to the end of the call transition set
   *
   * @return an iterator pointing just past the end of the call transition set
   *
   */
    
    
  Nwa::CallIterator Nwa::endCallTrans( ) const
  {
    return trans.endCall();
  }

  /**
   *
   * @brief provide access to the beginning of the internal transition set
   *
   * @return an iterator pointing to the beginning of the internal transition set
   *
   */
    
    
  Nwa::InternalIterator Nwa::beginInternalTrans( ) const
  {
    return trans.beginInternal();
  }

  /**
   * 
   * @brief provide access to the end of the internal transition set
   *
   * @return an iterator pointing just past the end of the internal transition set
   *
   */
    
    
  Nwa::InternalIterator Nwa::endInternalTrans( ) const
  {
    return trans.endInternal();
  }

  /**
   *
   * @brief provide access to the beginning of the return transition set
   *
   * @return an iterator pointing to the beginning of the return transition set
   *
   */
    
    
  Nwa::ReturnIterator Nwa::beginReturnTrans( ) const
  {
    return trans.beginReturn();
  }

  /**
   * 
   * @brief provide access to the end of the return transition set
   *
   * @return an iterator pointing just past the end of the return transition set
   *
   */
    
    
  Nwa::ReturnIterator Nwa::endReturnTrans( ) const
  {
    return trans.endReturn();
  }

  /**
   *
   * @brief computes the epsilon closure of the given states in their respective NWAs
   *
   * @param - sp: the starting point of the closure
   * @param - first: the NWA that determines the transitions available to the first 
   *                  component of the state pair
   * @param - second: the NWA that determines the transitions available to the second
   *                  component of the state pair
   * @return the set of state pairs reachable from the initial state pair by traversing
   *          epsilon transitions
   *
   */
    
  void Nwa::epsilonClosure( StateSet * newPairs, State st ) const
  {
    //compute the states reachable from st via epsilon transitions
    Internals reachable = trans.getInternals(st,EPSILON);
    for(  Internals::iterator it = reachable.begin(); it != reachable.end(); it++ )
    {
      State newSt = Trans::getTarget(*it);
      //Check that we haven't seen this pair already.
      if( newPairs->count(newSt) == 0 )
      {
        //Add a new pair.
        newPairs->insert(newSt);
        //Recursively explore.
        epsilonClosure(newPairs,newSt); 
      }
    }
  }

  /**
   *
   * @brief computes the epsilon closure of the given states in their respective NWAs
   *
   * @param - sp: the starting point of the closure
   * @param - first: the NWA that determines the transitions available to the first 
   *                  component of the state pair
   * @param - second: the NWA that determines the transitions available to the second
   *                  component of the state pair
   * @return the set of state pairs reachable from the initial state pair by traversing
   *          epsilon transitions
   *
   */
    
  void Nwa::epsilonClosure( std::set<StatePair> * newPairs, StatePair sp, Nwa const & first, Nwa const & second ) const
  {
    //Compute the cross produce of all states reachable from sp via epsilon transitions.

    //Explore epsilon transitions reachable from the first component of sp.
    Internals reachable = first.trans.getInternals(sp.first, EPSILON);
    for(  Internals::iterator it = reachable.begin(); it != reachable.end(); it++ )
    {
      StatePair newSP = StatePair(Trans::getTarget(*it),sp.second);
      //Check that we haven't seen this pair already.
      if( newPairs->count(newSP) == 0 )
      {
        //Add a new pair.
        newPairs->insert(newSP);
        //Recursively explore.
        epsilonClosure(newPairs,newSP,first,second); 
      }
    }

    //Explore epsilon transitions reachable from the second component of sp.
    reachable = second.trans.getInternals(sp.second, EPSILON);
    for(  Internals::iterator it = reachable.begin(); it != reachable.end(); it++ )
    {
      StatePair newSP = StatePair(sp.first,Trans::getTarget(*it));
      //Check that we haven't seen this pair already.
      if( newPairs->count(newSP) == 0 )
      {
        //Add a new pair.
        newPairs->insert(newSP);
        //Recursively explore.
        epsilonClosure(newPairs,newSP,first,second);
      }
    }
  }

    
  std::ostream& Nwa::marshall( std::ostream& os ) const
  {
    os << "<" << XMLTag << ">\n";

    for(SymbolIterator sym = beginSymbols(); sym != endSymbols(); ++sym)
    {
      os << "    <" << SymbolStorage::XMLSymbolTag() << " " << SymbolStorage::XMLNameAttr() << "=\"" << key2str(*sym) << "\"/>\n";
    }

    for(StateIterator state = beginStates(); state != endStates(); ++state)
    {
      os << "    <" << StateStorage::XMLStateTag() << " ";
      os << StateStorage::XMLNameAttr() << "=\"" << key2str(*state) << "\"";
      if(isInitialState(*state)) os << " " << StateStorage::XMLInitialAttr() << "=\"TRUE\"";
      if(isFinalState(*state)) os << " " << StateStorage::XMLFinalAttr() << "=\"TRUE\"";
      os << "/>\n";
    }

    for(CallIterator call = trans.beginCall(); call != trans.endCall(); ++call)
    {
      os << "    <" << Trans::CallXMLTag() << " ";
      os << Trans::XMLFromAttr() <<"=\"" << key2str(call->first) << "\" ";
      os << Trans::XMLSymbolAttr() << "=\"" << key2str(call->second) << "\" ";
      os << Trans::XMLToAttr() << "=\"" << key2str(call->third) << "\"/>\n";
    }
    for(InternalIterator internal = trans.beginInternal(); internal != trans.endInternal(); ++internal)
    {
      os << "    <" << Trans::InternalXMLTag() << " ";
      os << Trans::XMLFromAttr() <<"=\"" << key2str(internal->first) << "\" ";
      os << Trans::XMLSymbolAttr() << "=\"" << key2str(internal->second) << "\" ";
      os << Trans::XMLToAttr() << "=\"" << key2str(internal->third) << "\"/>\n";
    }
    for(ReturnIterator return_ = trans.beginReturn(); return_ != trans.endReturn(); ++return_)
    {
      os << "    <" << Trans::ReturnXMLTag() << " ";
      os << Trans::XMLFromAttr() <<"=\"" << key2str(return_->first) << "\" ";
      os << Trans::XMLPredAttr() <<"=\"" << key2str(return_->second) << "\" ";
      os << Trans::XMLSymbolAttr() << "=\"" << key2str(return_->third) << "\" ";
      os << Trans::XMLToAttr() << "=\"" << key2str(return_->fourth) << "\"/>\n";
    }


    os << "</" << XMLTag << ">\n";

    return os;
  }

  const char *
  Nwa::toStringGdb() const
  {
    std::stringstream ss;
    print(ss);

#ifdef _MSC_VER // Avoid MSVC warning
    return _strdup(ss.str().c_str());
#else
    return strdup(ss.str().c_str());
#endif
  }

  void
  Nwa::combineWith(Nwa const & rhs)
  {
    this->states.addAll(rhs.states);
    this->addAllSymbols(rhs.symbols);
    this->trans.addAllTrans(rhs.trans);
  }

  bool
  Nwa::isMemberNondet( NestedWord const & word ) const
  {
    typedef details::Configuration Configuration;
      
    std::set<Configuration> nextConfigs;
    for(StateIterator iter = beginInitialStates(); iter!=endInitialStates(); ++iter) {
      nextConfigs.insert(Configuration(*iter));
    }
            
    for( NestedWord::const_iterator curpos = word.begin();
         curpos != word.end(); ++curpos)
    {
      // When we start this loop, 'nextConfigs' holds the
      // *non-epsilon-closed* list of configurations we will use *this
      // iteration*. We first compute the epsilon closure of these
      // configurations, *then* update the variable currConfigs with the
      // union of 'nextConfigs' and the closure. Then we can proceed with
      // the simulation.
        
      // First, we take the epsilon closure of the current configuration set.
      std::set<Configuration> currConfigs;
      for( std::set<Configuration>::const_iterator config = nextConfigs.begin();
           config != nextConfigs.end(); ++config)
      {
        StateSet closure;
        epsilonClosure(&closure, config->state);
          
        for(StateSet::const_iterator other = closure.begin();
            other != closure.end(); ++other)
        {
          Configuration c(*config);
          c.state = *other;
          currConfigs.insert(c);
        }
          
        // TODO: Remove this once e-close includes the starting state
        Configuration c(*config);
        currConfigs.insert(c);
      }
        
      // Third, we clear out nextConfigs
      nextConfigs.clear();
        
      // Do something different depending on whether the current position is
      // a call, return, or internal symbol. But in all cases, put the possible
      // next configurations in here:
      if( curpos->type == NestedWord::Position::ReturnType )  {   //Denotes a return transition.
        // Determine the possible next configurations for each current config.
        // We need to look at the current state as well as the top of the 
        // call stack in each case.
        for( std::set<Configuration>::const_iterator config = currConfigs.begin();
             config != currConfigs.end(); ++config)
        {
          // Use trans.getReturns to get the matching return transitions out
          // of the "current" state, then separately check to see whether the
          // call predecessor matches.
          Returns rets = trans.getReturns(config->state, curpos->symbol);
          for( ReturnIterator rit = rets.begin(); rit != rets.end(); rit++ ) {
            // We can take a return transition under two conditions. One is
            // that there are things in the call predecessors stack and we
            // match. The other is there is nothing in the call predecessor
            // stack and the call predecessor on the return transition is
            // an initial state.

            if (config->callPredecessors.empty()) {
              if (isInitialState(rit->second)) {
                Configuration c(*config);
                c.state = Trans::getReturnSite(*rit);
                nextConfigs.insert(c);
              }
            }
            else {
              if (Trans::getCallSite(*rit) == config->callPredecessors.back()) {
                Configuration c(*config);
                c.callPredecessors.pop_back();
                c.state = Trans::getReturnSite(*rit);
                nextConfigs.insert(c);
              }
            }
            // Ok, we've checked that return transition
          }
        }
      }
      else if( curpos->type == NestedWord::Position::CallType ) {  //Denotes a call transition.
        // Determine the possible next configurations for each current config.
        // Now we just need to look at outgoing transitions.
        for( std::set<Configuration>::const_iterator config = currConfigs.begin();
             config != currConfigs.end(); ++config)
        {
          Calls calls = trans.getCalls(config->state, curpos->symbol);
          for( CallIterator cit = calls.begin(); cit != calls.end(); cit++ ) {
            // Construct a new configuration that's the same as the old
            // configuration, except with a pushed stack and new state
            Configuration c(*config);
            c.callPredecessors.push_back(config->state);
            c.state = Trans::getEntry(*cit);
            nextConfigs.insert(c);
          }
        }
      }
      else {   //Must be an internal transition.
        // Determine the possible next configurations for each current config.
        // Now we just need to look at outgoing transitions.
        for( std::set<Configuration>::const_iterator config = currConfigs.begin();
             config != currConfigs.end(); ++config)
        {
          Internals ints = trans.getInternals(config->state, curpos->symbol);
          for( InternalIterator iit = ints.begin(); iit != ints.end(); iit++ ) {
            // Construct a new configuration that's the same as the old
            // configuration, except with a new state
            Configuration c(*config);
            c.state = Trans::getTarget(*iit);
            nextConfigs.insert(c);
          }
        }
      }
    } 
            
    // Just like when we started this loop, 'nextConfigs' holds the non-epsilon-closed
    // list of configurations found after the last symbol was read. Before we check
    // whether we wound up in an accepting state, we have to close again.
            
    // First, we take the epsilon closure of the current configuration set.
    std::set<Configuration> currConfigs;
    for( std::set<Configuration>::const_iterator config = nextConfigs.begin();
         config != nextConfigs.end(); ++config)
    {
      StateSet closure;
      epsilonClosure(&closure, config->state);
                
      for( StateSet::const_iterator other = closure.begin();
           other != closure.end(); ++other)
      {
        Configuration c(*config);
        c.state = *other;
        currConfigs.insert(c);
      }

      // TODO: Remove this once e-close includes the starting state
      Configuration c(*config);
      currConfigs.insert(c);
    }
            
    //At the end of the word, if we are in a final state,
    //then return true
    for( std::set<Configuration>::const_iterator config = currConfigs.begin();
         config != currConfigs.end(); ++config)
    {
      if (isFinalState(config->state)) {
        //if (config->callPredecessors.size() != 0) {
        //  std::cerr << "Alert! In SimulateWordNondet, we are ending in a final state with nonempty stack!\n";
        //  exit(20);
        //}
        return true;
      }
    }
            
    return false;
  }

}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

