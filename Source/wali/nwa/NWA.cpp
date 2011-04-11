#include "wali/nwa/NWA.hpp"
#include "wali/nwa/details/Configuration.hpp"
#include "wali/nwa/NestedWord.hpp"

namespace wali
{
  namespace nwa
  {
    const std::string NWA::XMLTag = "XML";
      
    //
    // Methods
    //

    //Constructors and Destructor
    NWA::NWA( )
      : stuck(false)
    { }

    
    NWA::NWA( State stuckSt )
      : stuck(false)
    {
      setStuckState(stuckSt); 
    }

    
    NWA::NWA( const NWA & other )
      : Printable(other)
      , Countable(other)
      , stuck( other.stuck), states(other.states), symbols(other.symbols), trans(other.trans)
    {   }

    
    NWA & NWA::operator=( const NWA & other )
    {
      if( this == &other )
        return *this;

      //Erase any pre-existing data.
      clear();

      //Copy data over from 'other'
      stuck = other.stuck;
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
    
    void NWA::clear( )
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
    
     NWA::ClientInfoRefPtr NWA::getClientInfo( State state ) const
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
    
    void NWA::setClientInfo( State state, const ClientInfoRefPtr c )
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
    
    const NWA::StateSet & NWA::getStates( ) const
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
    
    bool NWA::isState( State state ) const
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
    
    bool NWA::addState( State state )
    {
      assert(state < wali::WALI_BAD_KEY);
      assert(stuck);
      return states.addState(state);
    }

    /**
     *
     * @brief returns the number of states associated with this NWA
     *
     * @return the number of states associated with this NWA
     *
     */
    
    size_t NWA::sizeStates( ) const
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
    
    bool NWA::removeState( State state )
    {
      assert(state < wali::WALI_BAD_KEY);
      assert(stuck);
      if( isStuckState(state) )   //Can't remove the stuck state.
        return false;

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
    
    void NWA::clearStates( )
    {
      states.clearStates();

      //Since all states are being removed, all transitions are removed as well.
      clearTrans();

      stuck = false;    //The stuck state has been removed too.
    }

    //Initial States

    /**
     *  
     * @brief obtain the states in the initial state set
     *
     * @return set of inital states associated with the NWA
     *
     */
    
    
    const NWA::StateSet & NWA::getInitialStates( ) const
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
    
    
    bool NWA::isInitialState( State state ) const
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
    
    
    bool NWA::addInitialState( State state )
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
    
    
    size_t NWA::sizeInitialStates( ) const
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
    
    
    bool NWA::removeInitialState( State state )
    {
      assert(state < wali::WALI_BAD_KEY);
      return states.removeInitialState(state);
    }

    /**
     *
     * @brief remove all states from the initial state set of the NWA
     *
     */
    
    
    void NWA::clearInitialStates( )
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
    
    
    const NWA::StateSet & NWA::getFinalStates( ) const
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
    
    
    bool NWA::isFinalState( State state ) const
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
    
    
    bool NWA::addFinalState( State state )
    {
      assert(state < wali::WALI_BAD_KEY);

      //Q: should we allow the stuck state to be made final? if so, do we need to do anything special?
      //A: No.  Yes, realize implicit transitions(removing the stuck state in the process) and allow 
      //           the state to be made final.

      //If we are trying to add the stuck state as a final state, all implicit transitions
      //must be realized and this state can no longer be the stuck state.
      if( isStuckState(state) )
      {
        realizeImplicitTrans();
      }

      return states.addFinalState(state);
    }

    /**
     * 
     * @brief returns the number of final states associated with this NWA
     *
     * @return the number of final states associated with this NWA
     *
     */
    
    
    size_t NWA::sizeFinalStates( ) const
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
    
    
    bool NWA::removeFinalState( State state )
    {
      assert(state < wali::WALI_BAD_KEY);
      return states.removeFinalState(state);
    }

    /**
     *
     * @brief remove all states from the final state set of the NWA
     *
     */
    
    
    void NWA::clearFinalStates( )
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
    
    
    const std::set< Symbol> & NWA::getSymbols( ) const
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
    
    
    bool NWA::isSymbol( Symbol sym ) const
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
    
    
    bool NWA::addSymbol( Symbol sym )
    {
      assert(sym < wali::WALI_BAD_KEY);
      assert(stuck);
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
    
    
    size_t NWA::sizeSymbols( ) const
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
    
    bool NWA::removeSymbol( Symbol sym )
    {
      assert(sym < wali::WALI_BAD_KEY);

      if( sym == WALI_WILD || sym == WALI_EPSILON )
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
    
    void NWA::clearSymbols( )
    {
      //assert(stuck);
      symbols.clearSymbols();

      //Since all symbols are being removed, all transitions are removed as well.
      clearTrans();
    }

    //Transition Accessors

    /**
     * 
     * @brief finds a symbol which occurs in a transition with the given endpoints
     *
     * @param - from: the source of the desired transition
     * @param - to: the target of the desired transition
     * @param - sym: the location in which to place any symbol labeling such a transition
     * @return true if some such transition is found, false otherwise
     *
     */
    
    
    bool NWA::getSymbol( State from, State to, Symbol & sym ) const
    {
      assert(from < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);
      return trans.getSymbol(from,to,sym);
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
    
    bool NWA::findTrans( State from, Symbol sym, State to) const
    {
      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);
      return trans.findTrans(from,sym,to);
    }

    /**
     * 
	 * @brief obtains all the symbols that label transitions from 'source' to 'target'
	 *
	 * @param - source: the source of the desired transitions
	 * @param - target: the target of the desired transitions
	 * @return the set of symbols that label transitions from 'source' to 'target'
	 *
     */
    
    const std::set< Symbol> NWA::getSymbols( State source, State target ) const
    {
      assert(source < wali::WALI_BAD_KEY);
      assert(target < wali::WALI_BAD_KEY);

       std::set<Symbol> syms;

       std::set<Call> const & calls = trans.getTransEntry(target);
      for( CallIterator cit = calls.begin(); cit != calls.end(); cit++ )
      {
        if( Trans::getCallSite(*cit) == source )
          syms.insert(Trans::getCallSym(*cit));
      }

       std::set<Internal> const & internals = trans.getTransTo(target);
      for( InternalIterator iit = internals.begin(); iit != internals.end(); iit++ )
      {
        if( Trans::getSource(*iit) == source )
          syms.insert(Trans::getInternalSym(*iit));
      }

       std::set<Return> const & returns = trans.getTransRet(target);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
      {
        if( Trans::getExit(*rit) == source )
          syms.insert(Trans::getReturnSym(*rit));
      }

      return syms;
    }

    /**
     * 
	 * @brief obtains all the symbols that label transitions from 'source'
	 *
	 * @param - source: the source of the desired transitions
	 * @return the set of symbols that label transitions from 'source'
	 *
     */
    
    const std::set< Symbol> NWA::getSymbolsFrom( State source ) const
    {
      assert(source < wali::WALI_BAD_KEY);

       std::set<Symbol> syms;

       std::set<Call> const & calls = trans.getTransCall(source);
      for( CallIterator cit = calls.begin(); cit != calls.end(); cit++ )
      {
          syms.insert(Trans::getCallSym(*cit));
      }

       std::set<Internal> const & internals = trans.getTransFrom(source);
      for( InternalIterator iit = internals.begin(); iit != internals.end(); iit++ )
      {
          syms.insert(Trans::getInternalSym(*iit));
      }

       std::set<Return> const & returns = trans.getTransExit(source);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
      {
          syms.insert(Trans::getReturnSym(*rit));
      }

      return syms;
    }

    /**
     * 
	 * @brief obtains all the symbols that label transitions to 'target'
	 *
	 * @param - target: the target of the desired transitions
	 * @return the set of symbols that label transitions to 'target'
	 *
     */
    
    const std::set< Symbol> NWA::getSymbolsTo( State target ) const
    {
      assert(target < wali::WALI_BAD_KEY);

       std::set<Symbol> syms;

       std::set<Call> const & calls = trans.getTransEntry(target);
      for( CallIterator cit = calls.begin(); cit != calls.end(); cit++ )
      {
          syms.insert(Trans::getCallSym(*cit));
      }

       std::set<Internal> const & internals = trans.getTransTo(target);
      for( InternalIterator iit = internals.begin(); iit != internals.end(); iit++ )
      {
          syms.insert(Trans::getInternalSym(*iit));
      }

       std::set<Return> const & returns = trans.getTransRet(target);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
      {
          syms.insert(Trans::getReturnSym(*rit));
      }

      return syms;
    }

    /**
     * 
     * @brief obtains all the states that are predecessors of the given state
     *
     * @param - state: the state of the state whose predecessors to look up
     * @return the set of all states that are predecessors of the given state
     *
     */ 
    
    void NWA::getPredecessors( State state,  StateSet & preds ) const
    {
      assert(state < wali::WALI_BAD_KEY);
       
       std::set<Call> const & calls = trans.getTransEntry(state);
      for( CallIterator cit = calls.begin(); cit != calls.end(); cit++ )
        preds.insert(Trans::getCallSite(*cit));

       std::set<Internal> const & internals = trans.getTransTo(state);
      for( InternalIterator iit = internals.begin(); iit != internals.end(); iit++ )
        preds.insert(Trans::getSource(*iit));

       std::set<Return> const & returns = trans.getTransRet(state);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
        preds.insert(Trans::getExit(*rit));
    }
	  /**
     * 
	   * @brief obtains all the states that are predecessors of the given state
	   *
	   * @param - state: the state whose predecessors to look up
	   * @return the set of all states that are predecessors of the given state
	   *
     */
	  
    const NWA::StateSet NWA::getPredecessors( State state ) const
    {
	    StateSet preds;
      getPredecessors(state, preds);
	    return preds;
    }

    /**
     * 
     * @brief obtains all the states that are predecessors of the given state with 
     *        respect to the given symbol
     *
     * @param - symbol: the symbol that should label a transition from each predecessor 
     *                  to the given state
     * @param - state: the state whose predecessors to look up
     * @param - preds: the set of all states that are predecessors of the given state
     *                  with respect to the given symbol
     *
     */
    
    void NWA::getPredecessors( Symbol symbol, State state,  StateSet & preds ) const
    {
      assert(state < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

       std::set<Call> const & calls = trans.getTransEntry(state);
      for( CallIterator cit = calls.begin(); cit != calls.end(); cit++ )
        if( symbol == Trans::getCallSym(*cit) )
          preds.insert(Trans::getCallSite(*cit));

       std::set<Internal> const & internals = trans.getTransTo(state);
      for( InternalIterator iit = internals.begin(); iit != internals.end(); iit++ )
        if( symbol == Trans::getInternalSym(*iit) )
          preds.insert(Trans::getSource(*iit));

       std::set<Return> const & returns = trans.getTransRet(state);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
        if( symbol == Trans::getReturnSym(*rit) )
          preds.insert(Trans::getExit(*rit));
    }
	  /**
     * 
	   * @brief obtains all the states that are predecessors of the given state with
	   *		respect to the given symbol
	   *
	   * @param - symbol: the symbol that should label a transition from each predecessor
	   *					to the given state
	   * @param - state: the state whose predecessors to look up
	   * @return the set of all states that are predecessors of the given state with 
	   *			respect to the given symbol
	   *
     */
	  
    const std::set< State> NWA::getPredecessors( Symbol symbol, State state ) const
    {
  	   StateSet preds;
      getPredecessors(symbol, state, preds);
	    return preds;
    }

    /**
     * 
     * @brief obtains all the states that are successors of the given state
     *
     * @param - state: the state whose successors to lookup
     * @param - succs: the set of all states that are successors of the given state
     *
     */
    
    void NWA::getSuccessors( State state,  StateSet & succs ) const
    {
      assert(state < wali::WALI_BAD_KEY);
       
       std::set<Call> const & calls = trans.getTransCall(state);
      for( CallIterator cit = calls.begin(); cit != calls.end(); cit++ )
        succs.insert(Trans::getEntry(*cit));

       std::set<Internal> const & internals = trans.getTransFrom(state);
      for( InternalIterator iit = internals.begin(); iit != internals.end(); iit++ )
        succs.insert(Trans::getTarget(*iit));

       std::set<Return> const & returns = trans.getTransExit(state);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
        succs.insert(Trans::getReturnSite(*rit));
    }
	  /**
     * 
	   * @brief obtains all the states that are successors of the given state
	   *
	   * @param - state: the state whose successors to look up
	   * @return the set of all states that are successors of the given state
	   *
     */
	  
    const std::set< State> NWA::getSuccessors( State state ) const
    {
  	   StateSet succs;
      getSuccessors(state, succs);
	    return succs;
    }

    /**
     * 
     * @brief obtains all the states that are successors of the given state with
     *        respect to the given symbol
     *
     * @param - symbol: the symbol that should label a transition from the given 
     *                  state to each successor
     * @param - state: the state whose successors to look up
     * @param - succs: the set of all states that are successors of the given state
     *                  with respect to the given symbol
     *
     */
    
    void NWA::getSuccessors( State state, Symbol symbol,  StateSet & succs ) const
    {
      assert(state < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

       std::set<Call> const & calls = trans.getTransCall(state);
      for( CallIterator cit = calls.begin(); cit != calls.end(); cit++ )
        if( symbol == Trans::getCallSym(*cit) )
          succs.insert(Trans::getEntry(*cit));

       std::set<Internal> const & internals = trans.getTransFrom(state);
      for( InternalIterator iit = internals.begin(); iit != internals.end(); iit++ )
        if( symbol == Trans::getInternalSym(*iit) )
          succs.insert(Trans::getTarget(*iit));

       std::set<Return> const & returns = trans.getTransExit(state);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
        if( symbol == Trans::getReturnSym(*rit) )
          succs.insert(Trans::getReturnSite(*rit));
    }
	  /**
     * 
     * @brief obtains all the states that are successors of the given state with
	   *		respect to the given symbol
	   *
     * @param - symbol: the symbol that should label a transition from the given
     *					state to each successor
     * @param - state: the state whose successors to look up
     * @return the set of all states that are successors of the given state
     *			with respect to the given symbol
     *
     */
    
    const std::set< State> NWA::getSuccessors( State state, Symbol symbol ) const
    {
       StateSet succs;
      getSuccessors(state, symbol, succs);
      return succs;
    }

	/**
     * 
	 * @brief obtains all the symbols that label return transitions whose 
	 *		call-predecessor is 'call' and whose return site is 'ret'
	 *
	 * @param - call: the call-predecessor of the desired return transitions
	 * @param - ret: the return site of the desired return transitions
	 * @return the set of symbols that label return transitions whose
	 *			call-predecessor is 'call' and whose return site is 'ret'
	 *
     */
    
    const std::set< Symbol> NWA::getCallRetSymbols( State call, State ret ) const
    {
      assert(call < wali::WALI_BAD_KEY);
      assert(ret < wali::WALI_BAD_KEY);

       std::set<Symbol> syms;

       std::set<Return> const & returns = trans.getTransRet(ret);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
      {
        if( Trans::getCallSite(*rit) == call )
          syms.insert(Trans::getReturnSym(*rit));
      }

      return syms;
    }

	/**
     * 
	 * @brief obtains all the symbols that label return transitions whose
	 *		call-predecessor is 'call'
	 *
	 * @param - call: the call-predecessor of the desired return transitions
	 * @return the set of symbols that label return transitions whose
	 *			call-predecessor is 'call'
	 *
     */
    
    const std::set< Symbol> NWA::getCallRetSymbolsFrom( State call ) const
    {
      assert(call < wali::WALI_BAD_KEY);

       std::set<Symbol> syms;

       std::set<Return> const & returns = trans.getTransPred(call);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
      {
          syms.insert(Trans::getReturnSym(*rit));
      }

      return syms;
    }

	/**
     * 
	 * @brief obtains all the symbols that label return transitions whose
	 *		return site is 'ret'
	 *
	 * @param - ret: the return site of the desired return transitions
	 * @return the set of symbols that label return transitions whose
	 *			return site is 'ret'
	 *
     */
    
    const std::set< Symbol> NWA::getCallRetSymbolsTo( State ret ) const
    {
      assert(ret < wali::WALI_BAD_KEY);

       std::set<Symbol> syms;

       std::set<Return> const & returns = trans.getTransRet(ret);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
      {
          syms.insert(Trans::getReturnSym(*rit));
      }

      return syms;
    }

    /**
     *  
     * @brief obtains all the states that are call-predecessors of the given state
     *
     * @param - state: the state whose call-predecessors to look up
     * @param - preds: the set of all states that are call-predecessors of the given state
     *
     */ 
    
    void NWA::getCallPredecessors( State state,  StateSet & c_preds ) const
    {
      assert(state < wali::WALI_BAD_KEY);
      
       std::set<Return> const & returns = trans.getTransRet(state);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
        c_preds.insert(Trans::getCallSite(*rit));
    }
	/**
     * 
	 * @brief obtains all the states that are call-predecessors of the given state
	 *
	 * @param - state: the state whose call-predecessors to look up
	 * @return the set of all states that are call-predecessors of the given state
	 *
     */
	
  const std::set< State> NWA::getCallPredecessors( State state ) const
    {
      assert(state < wali::WALI_BAD_KEY);
      
	     StateSet c_preds;
      getCallPredecessors(state, c_preds);
	    return c_preds;
    }
      
    /**
     * 
     * @brief obtains all the states that are call-predecessors of the given state with 
     *        respect to the given symbol
     *
     * @param - symbol: the symbol that should label a transition with each call-predecessor 
     *                  and the given state
     * @param - state: the state whose call-predecessors to look up
     * @param - preds: the set of all states that are call-predecessors of the given state
     *                  with respect to the given symbol
     *
     */
    
    void NWA::getCallPredecessors( Symbol symbol, State state,  StateSet & c_preds ) const
    {
      assert(state < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

       std::set<Return> const & returns = trans.getTransRet(state);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
        if( symbol == Trans::getReturnSym(*rit) )
          c_preds.insert(Trans::getCallSite(*rit));
    }
	/**
     * 
	 * @brief obtains all the states that are call-predecessors of the given state with
	 *		respect to the given symbol
	 *
	 * @param - symbol: the symbol that should label a transition with each call-predecessor
	 *					and the given state
	 * @param - state: the state whose call-predecessors to look up
	 * @return the set of all states that are call-predecessors of the given state
	 *			with respect to the given symbol
	 *
     */
	
  const std::set< State> NWA::getCallPredecessors( Symbol symbol, State state ) const
    {
       StateSet c_preds;
      getCallPredecessors(symbol, state, c_preds);
	    return c_preds;
    }

    /**
     * 
     * @brief obtains all the states that are call-successors of the given state
     *
     * @param - state: the state whose call-successors to lookup
     * @param - succs: the set of all states that are call-successors of the given state
     *
     */
    
    void NWA::getCallSuccessors( State state,  StateSet & c_succs ) const
    {
      assert(state < wali::WALI_BAD_KEY);
      
       std::set<Return> const & returns = trans.getTransPred(state);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
        c_succs.insert(Trans::getReturnSite(*rit));
    }
	/**
     * 
	 * @brief obtains all the states that are call-successors of the given state
	 *
	 * @param - state: the state whose call-successors to look up
	 * @return the set of all states that are call-successors of the given state
	 *
     */
	
  const std::set< State> NWA::getCallSuccessors( State state ) const
    {
       StateSet c_succs;
      getCallSuccessors(state, c_succs);
	    return c_succs;
    }

    /**
     * 
     * @brief obtains all the states that are call-successors of the given state with
     *        respect to the given symbol
     *
     * @param - symbol: the symbol that should label a return transition with each call-
     *                  successor and the given state
     * @param - state: the state whose call-successors to look up
     * @param - succs: the set of all states that are call-successors of the given state
     *                  with respect to the given symbol
     *
     */
    
    void NWA::getCallSuccessors( State state, Symbol symbol,  StateSet & c_succs ) const
    {
      assert(state < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);
      
       std::set<Return> const & returns = trans.getTransPred(state);
      for( ReturnIterator rit = returns.begin(); rit != returns.end(); rit++ )
        if( symbol == Trans::getReturnSym(*rit) )
          c_succs.insert(Trans::getReturnSite(*rit));
    }
	/**
     * 
	 * @brief obtains all the states that are call-successors of the given state with 
	 *		respect to the given symbol
	 *
	 * @param - symbol: the symbol that should label a return transition with each call-
	 *					successor and the given state
	 * @param - state: the state whose call-successors to look up
	 * @return the set of all states that are call-successors of the given state with
	 *			respect to the given symbol
	 *
     */
	
  const std::set< State> NWA::getCallSuccessors( State state, Symbol symbol ) const
    {
       StateSet c_succs;
      getCallSuccessors(state, symbol, c_succs);
	    return c_succs;
    }

    /**
     *  
     * @brief duplicates the original state, but only duplicates outgoing transitions
     *
     * @param - orig: the name of the original state, i.e. the state to duplicate
     * @param - dup: the name of the duplicate state
     *
     */
    
    void NWA::duplicateStateOutgoing( State orig, State dup )
    {
      assert(orig < wali::WALI_BAD_KEY);
      assert(dup < wali::WALI_BAD_KEY);
      assert(stuck);
      assert(!isStuckState(orig));  //Duplicating the stuck state doesn't make sense.
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
    
    void NWA::duplicateState( State orig, State dup )
    {
      assert(orig < wali::WALI_BAD_KEY);
      assert(dup < wali::WALI_BAD_KEY);
      assert(stuck);
      assert(!isStuckState(orig));  //Duplicating the stuck state doesn't make sense.
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
    
    void NWA::realizeImplicitTrans()
    {
       std::set<Triple<State, Symbol, State> > returns;
      for( ReturnIterator ret = beginReturnTrans(); ret != endReturnTrans(); ++ret) 
      {
        returns.insert(Triple<State, Symbol, State>(ret->first, ret->second, ret->third));
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
      for( stateIterator stit = beginStates(); stit != endStates(); ++stit )
      {
        State state = *stit;

        for( symbolIterator symit = beginSymbols(); symit != endSymbols(); ++symit )
        {
          Symbol symbol = *symit;

          // Skip over epsilons. (A) they are handled in other places and
          // (B) there's no implicit epsilon transitions to stuck anyway.
          if( symbol == WALI_EPSILON )
          {
            continue;
          }

          //Skip over wilds.
          if( symbol == WALI_WILD )
          {
            continue;
          }

          if( !trans.callExists(state, symbol) && !trans.callExists(state, WALI_WILD) )
          {
            addCallTrans(state, symbol, getStuckState());
          }

          if( !trans.internalExists(state, symbol) && !trans.internalExists(state, WALI_WILD) )
          {
            addInternalTrans(state, symbol, getStuckState());
          }

          for( stateIterator pred = beginStates(); pred != endStates(); ++pred )
          {
            if( returns.find(Triple<State,Symbol,State>(state, *pred, symbol)) == returns.end() 
              && returns.find(Triple<State,Symbol,State>(state, *pred, WALI_WILD)) == returns.end() )
            {
              addReturnTrans(state, *pred, symbol, getStuckState());
            }
          }
        } // for each symbol
      } // for each state

      stuck = false;  //Since all transitions are explicit, the stuck state is reset.
    }

    /**
     *
     * @brief returns the number of transitions associated with this NWA
     *
     * @return the number of transitions associated with this NWA
     *
     */
    
    
    size_t NWA::sizeTrans( ) const
    {
      return trans.size();
    }

    /**
     *
     * @brief remove all transitions from the NWA
     *
     */
    void NWA::clearTrans( )
    {
      //assert(stuck);
      trans.clear();
    }

    //Call Transitions

    /** 
     * 
     * @brief returns the call site states associated with the given symbol
     *
     * @param - symbol: the symbol whose call site states to obtain
     * @return the set of call site states associated with the given symbol
     *
     */
    
    const std::set< State> NWA::getCallSites_Sym( Symbol symbol ) const
    {
      assert(symbol < wali::WALI_BAD_KEY);

      const Calls & call = trans.getCalls();
       StateSet calls;
      for( CallIterator it = call.begin(); it != call.end(); it++ )
      {
        if( symbol == Trans::getCallSym(*it) )
          calls.insert( Trans::getCallSite(*it) );
      }
      return calls;
    }

    /**
     * 
     * @brief returns the call site states associated with the given symbol
     *        and entry point
     *
     * @param - symbol: the symbol whose call site states to obtain
     * @param - entryPoint: the entry point state whose call site states to
     *                    obtain
     * @return the set of call site states associated with the given symbol
     *          and entry point
     *
     */
    
    const std::set< State> NWA::getCallSites( Symbol symbol, State entryPoint ) const
    {
      assert(entryPoint < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Calls & call = trans.getTransEntry(entryPoint);
       StateSet calls;
      for( CallIterator it = call.begin(); it != call.end(); it++ )
      {
        if( symbol == Trans::getCallSym(*it) )
          calls.insert( Trans::getCallSite(*it) );
      }
      return calls;
    }
      
    /**
     * 
     * @brief returns the call site/symbol pairs associated with the given 
     *        entry point
     *
     * @param - entryPoint: the entry point state whose call site/symbol 
     *                      pairs to obtain
     * @return the set of call site/symbol pairs associated with the given 
     *          entry point
     *
     */
    
    const std::set<std::pair< State, Symbol> > NWA::getCallSites( State entryPoint ) const
    {
      assert(entryPoint < wali::WALI_BAD_KEY);

      const Calls & call = trans.getTransEntry(entryPoint);
       std::set<std::pair<Symbol,State> > calls;
      for( CallIterator it = call.begin(); it != call.end(); it++ )
      {
        calls.insert( std::pair<Symbol,State>(Trans::getCallSite(*it), Trans::getCallSym(*it) ) );
      }
      return calls;
    }

	/**
     * 
	 * @brief returns the call sites of all call transitions in the NWA
	 *
	 * @return the set of call sites of all call transitions in the NWA
	 *
     */
	
    const std::set< State> NWA::getCallSites( ) const
    {
      const Calls & call = trans.getCalls();
       StateSet calls;
      for( CallIterator it = call.begin(); it != call.end(); it++ )
      {
        calls.insert( Trans::getCallSite(*it) );
      }
      return calls;
    }

    /**
     * 
     * @brief returns the symbols that label call transitions in the NWA
     *
     * @return the set of symbols that label call transitions in the NWA
     *
     */
    
    const std::set< Symbol> NWA::getCallSym(  ) const
    {
      const Calls & calls = trans.getCalls();
       std::set<Symbol> syms;
      for( CallIterator it = calls.begin(); it != calls.end(); it++ )
      {
        syms.insert( Trans::getCallSym(*it) );
      }
      return syms;
    }

    /**
     * 
     * @brief returns the symbols that label the call transitions from 
     *        'callSite' to 'entryPoint' in the NWA
     *
     * @param - callSite: the call site of the desired transitions
     * @param - entryPoint: the entry point of the desired transitions
     * @return the set of symbols that label the call transitions from
     *          'callSite' to 'entryPoint' in the NWA
     *
     */
    
    const std::set< Symbol> NWA::getCallSym( State callSite, State entryPoint ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(entryPoint < wali::WALI_BAD_KEY);

      const Calls & calls = trans.getTransCall(callSite);
       std::set<Symbol> syms;
      for( CallIterator it = calls.begin(); it != calls.end(); it++ )
      {
        if( entryPoint == Trans::getEntry(*it) )
          syms.insert( Trans::getCallSym(*it) );
      }
      return syms;
    }

    /**
     * 
     * @brief returns the symbols that label the call transitions from 
     *        'callSite' in the NWA
     *
     * @param - callSite: the call site of the desired transitions
     * @return the set of symbols that label the call transitions from
     *          'callSite' in the NWA
     *
     */
    
    const std::set< Symbol> NWA::getCallSym_Call( State callSite ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);

      const Calls & calls = trans.getTransCall(callSite);
       std::set<Symbol> syms;
      for( CallIterator it = calls.begin(); it != calls.end(); it++ )
      {
          syms.insert( Trans::getCallSym(*it) );
      }
      return syms;
    }

    /**
     * 
     * @brief returns the symbols that label the call transitions to
     *        'entryPoint' in the NWA
     *
     * @param - entryPoint: the entry point of the desired transitions
     * @return the set of symbols that label the call transitions to
     *          'entryPoint' in the NWA
     *
     */
    
    const std::set< Symbol> NWA::getCallSym_Entry( State entryPoint ) const
    {
      assert(entryPoint < wali::WALI_BAD_KEY);

      const Calls & calls = trans.getTransEntry(entryPoint);
       std::set<Symbol> syms;
      for( CallIterator it = calls.begin(); it != calls.end(); it++ )
      {
          syms.insert( Trans::getCallSym(*it) );
      }
      return syms;
    }

    /**
     * 
     * @brief returns the entry point states associated with the given symbol
     *
     * @param - symbol: the symbol whose entry point states to obtain
     * @return the set of entry point states associated with the given symbol
     *
     */
    
    const std::set< State> NWA::getEntries_Sym( Symbol symbol ) const
    {
      assert(symbol < wali::WALI_BAD_KEY);

      const Calls & ent = trans.getCalls();
       StateSet entries;
      for( CallIterator it = ent.begin(); it != ent.end(); it++ )
      {
        if( symbol == Trans::getCallSym(*it) )
          entries.insert( Trans::getEntry(*it) );
      }
      return entries;
    }
      
    /**
     * 
     * @brief returns the entry point states associated with the given call
     *        site and symbol
     *
     * @param - callSite: the call site whose entry point states to obtain
     * @param - symbol: the symbol whose entry point states to obtain
     * @return the set of entry point states associated with the given call
     *          site and symbol
     *
     */
    
    const std::set< State> NWA::getEntries( State callSite, Symbol symbol ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Calls & ent = trans.getTransCall(callSite);
       StateSet entries;
      for( CallIterator it = ent.begin(); it != ent.end(); it++ )
      {
        if( symbol == Trans::getCallSym(*it) )
          entries.insert( Trans::getEntry(*it) );
      }
      return entries;
    }

    /**
     * 
     * @brief returns the symbol/entry point pairs associated with the given call site
     *
     * @param - callSite: the call site whose symbol/entry point pairs to obtain
     * @return the set of symbol/entry point pairs associated with the given call site
     *
     */
    
    const std::set<std::pair< Symbol, State> > NWA::getEntries( State callSite ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);
      const Calls & ent = trans.getTransCall(callSite);
       std::set<std::pair<Symbol,State> > entries;
      for( CallIterator it = ent.begin(); it != ent.end(); it++ )
      {
        entries.insert( std::pair<Symbol,State>(Trans::getCallSym(*it),Trans::getEntry(*it)) );
      }
      return entries;
    }

	/**
     * 
	 * @brief returns the entry points of all call transitions in the NWA
	 *
	 * @return the set of entry points of all call transitions in the NWA
	 *
     */
	
    const std::set< State> NWA::getEntries( ) const
    {
      const Calls & call = trans.getCalls();
       StateSet entries;
      for( CallIterator it = call.begin(); it != call.end(); it++ )
      {
        entries.insert( Trans::getEntry(*it) );
      }
      return entries;
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
    
    bool NWA::addCallTrans( State from, Symbol sym, State to )
    {
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      assert(sym != WALI_EPSILON ); //An Epsilon symbol on a call doesn't make sense.

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if( isStuckState(from) )
      {
        assert( isStuckState(to) );
      }

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
    
    bool NWA::addCallTrans( Call & ct )
    {
      assert(stuck);

      assert(Trans::getCallSite(ct) < wali::WALI_BAD_KEY);
      assert(Trans::getCallSym(ct) < wali::WALI_BAD_KEY);
      assert(Trans::getEntry(ct) < wali::WALI_BAD_KEY);

      assert( Trans::getCallSym(ct) != WALI_EPSILON ); //An Epsilon symbol on a call doesn't make sense.

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if( isStuckState(Trans::getCallSite(ct)) )
      {
        assert( isStuckState(Trans::getEntry(ct)) );
      }

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
    
    bool NWA::removeCallTrans( State from, Symbol sym, State to )
    {
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      if(! isState(from) || ! isSymbol(sym) || ! isState(to) || sym == WALI_EPSILON )
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
    
    bool NWA::removeCallTrans( const Call & ct )
    {
      assert(stuck);

      assert(Trans::getCallSite(ct) < wali::WALI_BAD_KEY);
      assert(Trans::getCallSym(ct) < wali::WALI_BAD_KEY);
      assert(Trans::getEntry(ct) < wali::WALI_BAD_KEY);

      if(! isState(Trans::getCallSite(ct))
        || ! isSymbol(Trans::getCallSym(ct))
        || ! isState(Trans::getEntry(ct)) 
        || Trans::getCallSym(ct) == WALI_EPSILON )
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
    
    
    size_t NWA::sizeCallTrans( ) const
    {
      return trans.sizeCall();
    }

    //Internal Transitions

    /**
     *
     * @brief returns the source states associated with the given symbol
     *
     * @param - symbol: the symbol whose source states to obtain
     * @return the set of source states associated with the given symbol
     *
     */
    
    const std::set< State> NWA::getSources_Sym( Symbol symbol ) const
    {
      assert(symbol < wali::WALI_BAD_KEY);

      const Internals & src = trans.getInternals();
       StateSet sources;
      for( InternalIterator it = src.begin(); it != src.end(); it++ )
      {
        if( symbol == Trans::getInternalSym(*it) )
          sources.insert( Trans::getSource(*it) );
      }
      return sources;
    }

    /**
     * 
     * @brief returns the source states associated with the given symbol 
     *        and target state
     *
     * @param - symbol: the symbol whose source states to obtain
     * @param - target: the target state whose source states to obtain
     * @return the set of source states assoicated with the given
     *          symbol and target state
     *
     */
    
    const std::set< State> NWA::getSources( Symbol symbol, State target ) const
    {
      assert(target < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Internals & src = trans.getTransTo(target);
       StateSet sources;
      for( InternalIterator it = src.begin(); it != src.end(); it++ )
      {
        if( symbol == Trans::getInternalSym(*it) )
          sources.insert( Trans::getSource(*it) );
      }
      return sources;
    }

    /**
     * 
     * @brief returns the source/symbol pairs associated with the 
     *        given target state
     *
     * @param - target: the target state whose source/symbol pairs to 
     *                  obtain
     * @return the set of source/symbol pairs associated with the given
     *          target state
     *
     */
    
    const std::set<std::pair< State, Symbol> > NWA::getSources( State target ) const
    {
      assert(target < wali::WALI_BAD_KEY);

      const Internals & src = trans.getTransTo(target);
       std::set<std::pair<Symbol,State> > sources;
      for( InternalIterator it = src.begin(); it != src.end(); it++ )
      {
        sources.insert( std::pair<Symbol,State>(Trans::getSource(*it), Trans::getInternalSym(*it)) );
      }
      return sources;
    }

	/**
     * 
	 * @brief returns the sources of all internal transitions in the NWA
	 *
	 * @return the set of sources of all internal transitions in the NWA
	 *
     */
	
    const std::set< State> NWA::getSources(  ) const
    {
      const Internals & src = trans.getInternals();
       StateSet sources;
      for( InternalIterator it = src.begin(); it != src.end(); it++ )
      {
        sources.insert( Trans::getSource(*it) );
      }
      return sources;
    }

    /**
     * 
     * @brief returns the symbols that label internal transitions in the NWA
     *
     * @return the set of symbols that label internal transitions in the NWA
     *
     */
    
    const std::set< Symbol> NWA::getInternalSym(  ) const
    {
      const Internals & ints = trans.getInternals();
       std::set<Symbol> syms;
      for( InternalIterator it = ints.begin(); it != ints.end(); it++ )
      {
        syms.insert( Trans::getInternalSym(*it) );
      }
      return syms;
    }

    /**
     * 
     * @brief returns the symbols that label internal transitions from 'source'
     *        to 'target' in the NWA
     *
     * @param - source: the source of the desired transitions
     * @param - target: the target of the desired transitions
     * @return the set of symbols that label internal transitions from 'source'
     *          to 'target' in the NWA
     *
     */
    
    const std::set< Symbol> NWA::getInternalSym( State source, State target ) const
    {
      const Internals & ints = trans.getTransFrom(source);
       std::set<Symbol> syms;
      for( InternalIterator it = ints.begin(); it != ints.end(); it++ )
      {
        if( target == Trans::getTarget(*it) )
          syms.insert( Trans::getInternalSym(*it) );
      }
      return syms;
    }

    /**
     * 
     * @brief returns the symbols that label internal transitions from 'source'
     *        in the NWA
     *
     * @param - source: the source of the desired transitions
     * @return the set of symbols that label internal transitions from 'source'
     *          in the NWA
     *
     */
    
    const std::set< Symbol> NWA::getInternalSym_Source( State source ) const
    {
      const Internals & ints = trans.getTransFrom(source);
       std::set<Symbol> syms;
      for( InternalIterator it = ints.begin(); it != ints.end(); it++ )
      {
        syms.insert( Trans::getInternalSym(*it) );
      }
      return syms;
    }

    /**
     * 
     * @brief returns the symbols that label internal transitions 
     *        to 'target' in the NWA
     *
     * @param - target: the target of the desired transitions
     * @return the set of symbols that label internal transitions 
     *          to 'target' in the NWA
     *
     */
    
    const std::set< Symbol> NWA::getInternalSym_Target( State target ) const
    {
      const Internals & ints = trans.getTransTo(target);
       std::set<Symbol> syms;
      for( InternalIterator it = ints.begin(); it != ints.end(); it++ )
      {
        syms.insert( Trans::getInternalSym(*it) );
      }
      return syms;
    }

    /**
     * 
     * @brief returns the target states associated with the given symbol
     *
     * @param - symbol: the symbol whose target states to obtain
     * @return the set of target states associated with the given symbol
     *
     */
    
    const std::set< State> NWA::getTargets_Sym( Symbol symbol ) const
    {
      assert(symbol < wali::WALI_BAD_KEY);

      const Internals & tgt = trans.getInternals();
       StateSet targets;
      for( InternalIterator it = tgt.begin(); it != tgt.end(); it++ )
      {
        if( symbol == Trans::getInternalSym(*it) )
          targets.insert( Trans::getTarget(*it) );
      }
      return targets;
    }

    /**
     * 
     * @brief returns the target states associated with the given source
     *        state and symbol
     *
     *
     * @param - source: the source state whose target states to obtain
     * @param - symbol: the symbol whose target states to obtain
     * @return the set of target states associated with the given source
     *          state and symbol
     *
     */
    
    const std::set< State> NWA::getTargets( State source, Symbol symbol ) const
    {
      assert(source < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Internals & tgt = trans.getTransFrom(source);
       StateSet targets;
      for( InternalIterator it = tgt.begin(); it != tgt.end(); it++ )
      {
        if( symbol == Trans::getInternalSym(*it) )
          targets.insert( Trans::getTarget(*it) );
      }
      return targets;
    }

    /**
     * 
     * @brief returns the symbol/target pairs associated with the given source
     *
     * @param - source: the source whose symbol/target pairs to obtain
     * @return the set of symbol/target pairs associated with the given source
     *
     */
    
    const std::set<std::pair< Symbol, State> > NWA::getTargets( State source ) const
    {
      assert(source < wali::WALI_BAD_KEY);
      const Internals & tgt = trans.getTransFrom(source);
       std::set<std::pair<Symbol,State> > targets;
      for( InternalIterator it = tgt.begin(); it != tgt.end(); it++ )
      {
        targets.insert( std::pair<Symbol,State>(Trans::getInternalSym(*it),Trans::getTarget(*it)) );
      }
      return targets;
    }

	/**
     * 
	 * @brief returns the targets of all internal transitions in the NWA
	 *
	 * @return the set of targets of all internal transitions in the NWA
	 *
     */
    
    const std::set< State> NWA::getTargets(  ) const
    {
      const Internals & tgt = trans.getInternals();
       StateSet targets;
      for( InternalIterator it = tgt.begin(); it != tgt.end(); it++ )
      {
        targets.insert( Trans::getTarget(*it) );
      }
      return targets;
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
    
    bool NWA::addInternalTrans( State from, Symbol sym, State to )
    {
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if( isStuckState(from) )
      {
        assert(isStuckState(to));
      }

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
    
    bool NWA::addInternalTrans( Internal & it )
    {
      assert(stuck);

      assert(Trans::getSource(it) < wali::WALI_BAD_KEY);
      assert(Trans::getInternalSym(it) < wali::WALI_BAD_KEY);
      assert(Trans::getTarget(it) < wali::WALI_BAD_KEY);

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if( isStuckState(Trans::getSource(it)) )
      {
        assert( isStuckState(Trans::getTarget(it)) );
      }

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
    
    bool NWA::removeInternalTrans( State from, Symbol sym, State to )
    {
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      if(! isState(from) || ! isSymbol(sym) || ! isState(to) )
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
    
    bool NWA::removeInternalTrans( const Internal & it )
    {
      assert(stuck);

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
    
    
    size_t NWA::sizeInternalTrans( ) const
    {
      return trans.sizeInternal();
    }

    //Return Transitions

    /**
     * 
     * @brief returns the exit states associated with the given symbol
     *
     * @param - symbol: the symbol whose exit states to obtain
     * @return the set of exit states associated with the given symbol
     *
     */
    
    const std::set< State> NWA::getExits_Sym( Symbol symbol ) const
    {
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns & exit = trans.getReturns();
       StateSet exits;
      for( ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( symbol == Trans::getReturnSym(*it) )
        {
          exits.insert( Trans::getExit(*it) );
        }
      }
      return exits;
    }

    /**
     * 
     * @brief returns the exit states associated with the given call site,
     *        symbol, and return site
     *
     * @param - callSite: the call site state whose exit states to obtain
     * @param - symbol: the symbol whose exit states to obtain
     * @param - returnSite: the return site state whose exit states to obtain
     * @return the set of exit states associated with the given call site,
     *          symbol, and return site
     *
     */
    
    const std::set< State> NWA::getExits( State callSite, Symbol symbol, State returnSite ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns & exit = trans.getTransPred(callSite);
       StateSet exits;
      for( ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( (Trans::getReturnSite(*it) == returnSite) && (symbol == Trans::getReturnSym(*it)) )
        {
          exits.insert( Trans::getExit(*it) );
        }
      }
      return exits;
    }

    /**
     * 
     * @brief returns the exit point/symbol pairs associated with the given 
     *        call site and return site
     *
     * @param - callSite: the call site state whose exit point/symbol pairs to obtain
     * @param - returnSite: the return site state whose exit point/symbol pairs to obtain
     * @return the set of exit point/symbol pairs associated with the given call site 
     *          and return site
     *
     */
    
    const std::set<std::pair< State, Symbol> > NWA::getExits( State callSite, State returnSite ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);

      const Returns & exit = trans.getTransPred(callSite);
       std::set<std::pair<State,Symbol> > exits;
      for( ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( Trans::getReturnSite(*it) == returnSite )
        {
          exits.insert( std::pair<State,Symbol>(Trans::getExit(*it),Trans::getReturnSym(*it)) );
        }
      }
      return exits;
    }

	/**
     * 
	 * @brief returns the exit points of all return transitions in the NWA
	 *
	 * @return the set of exit points of all return transitions in the NWA
	 *
     */
	
    const std::set< State> NWA::getExits( ) const
    {
      const Returns & exit = trans.getReturns();
       StateSet exits;
      for( ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
          exits.insert( Trans::getExit(*it) );
      }
      return exits;
    }

    /**
     * 
     * @brief returns the exit states associated with the given call site 
     *        and symbol
     * 
     * @param - callSite: the call site state whose exit states to obtain
     * @param - symbol: the symbol whose exit states to obtain
     * @return the set of exit states associated with the given call state
     *          and symbol
     *
     */
    
    const std::set< State> NWA::getExits_Call( State callSite, Symbol symbol ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns & exit = trans.getTransPred(callSite);
       StateSet exits;
      for( ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( symbol == Trans::getReturnSym(*it) )
          exits.insert( Trans::getExit(*it) );
      }
      return exits;
    }

    /**
     * 
     * @brief returns the exit point/symbol pairs associated with the
     *        given call site
     *
     * @param - callSite: the call site state whose exit point/symbol pairs
     *                    to obtain
     * @return the set of exit point/symbol pairs associated with the given
     *          call site
     *
     */
    
    const std::set<std::pair< State, Symbol> > NWA::getExits_Call( State callSite ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);

      const Returns & exit = trans.getTransPred(callSite);
       std::set<std::pair<State,Symbol> > exits;
      for( ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        exits.insert( std::pair<State,Symbol>(Trans::getExit(*it),Trans::getReturnSym(*it)) );
      }
      return exits;
    }

    /**
     * 
     * @brief returns the exit states associated with the given symbol and
     *        return site
     *
     * @param - symbol: the symbol whose exit states to obtain
     * @param - returnSite: the return site state whose exit states to obtain
     * @return the set of exit states associated with the given symbol and
     *          return site
     *
     */
    
    const std::set< State> NWA::getExits_Ret( Symbol symbol, State returnSite ) const
    {
      assert(returnSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns & exit = trans.getTransRet(returnSite);
       StateSet exits;
      for( ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( symbol == Trans::getReturnSym(*it) )
          exits.insert( Trans::getExit(*it) );
      }
      return exits;
    }

    /**
     * 
     * @brief returns the exit point/symbol pairs associated with the
     *        given return site
     *
     * @param - returnSite: the return site state whose exit point/symbol
     *                      pairs to obtain
     * @return the set of exit point/symbol pairs associated with the given
     *          return site
     *
     */
    
    const std::set<std::pair< State, Symbol> > NWA::getExits_Ret( State returnSite ) const
    {
      assert(returnSite < wali::WALI_BAD_KEY);

      const Returns & exit = trans.getTransRet(returnSite);
       std::set<std::pair<State,Symbol> > exits;
      for( ReturnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        exits.insert( std::pair<State,Symbol>(Trans::getExit(*it),Trans::getReturnSym(*it)) );
      }
      return exits;
    }

    /**
     * 
     * @brief returns the call site states associated with the given symbol
     *
     * @param - symbol: the symbol whose call site states to obtain
     * @return the set of call site states associated with the given symbol
     *
     */
    
    const std::set< State> NWA::getCalls_Sym( Symbol symbol ) const
    {
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns & call = trans.getReturns();
       StateSet calls;
      for( ReturnIterator it = call.begin(); it != call.end(); it++ )
      {
        if( symbol == Trans::getReturnSym(*it) )
        {
          calls.insert( Trans::getCallSite(*it) );
        }
      }
      return calls;
    }

    /**
     * 
     * @brief returns the call site states associated with the given exit 
     *        point, symbol, and return site
     *
     * @param - exitPoint: the exit point whose call site states to obtain
     * @param - symbol: the symbol whose call site states to obtain
     * @param - returnSite: the return site whose call site states to obtain
     * @return the set of call site states assocaited with the given exit
     *          point, symbol, and return site
     * 
     */
    
    const std::set< State> NWA::getCalls( State exitPoint, Symbol symbol, State returnSite ) const
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns call = trans.getTransExit(exitPoint);
       StateSet calls;
      for( ReturnIterator it = call.begin(); it != call.end(); it++ )
      {
        if( (Trans::getReturnSite(*it) == returnSite) &&  (symbol == Trans::getReturnSym(*it)) )
        {
          calls.insert( Trans::getCallSite(*it) );
        }
      }
      return calls;
    }

    /**
     * 
     * @brief returns the call site/symbol pairs associated with the given exit 
     *        point and return site states
     *
     * @param - exitPoint: the exit point whose call site/symbol pairs to obtain
     * @param - returnSite: the return site whose call site/symbol pairs to obtain
     * @return the set of call site/symbol pairs associated with the given exit 
     *          point and return site states
     *
     */
    
    const std::set<std::pair< State, Symbol> > NWA::getCalls( State exitPoint, State returnSite ) const
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);

      const Returns call = trans.getTransExit(exitPoint);
       std::set<std::pair<State,Symbol> > calls;
      for( ReturnIterator it = call.begin(); it != call.end(); it++ )
      {
        if( Trans::getReturnSite(*it) == returnSite )
        {
          calls.insert( std::pair<State,Symbol>(Trans::getCallSite(*it),Trans::getReturnSym(*it)) );
        }
      }
      return calls;
    }

	/**
     * 
	 * @brief returns the call sites of all return transitions in the NWA
	 *
	 * @return the set of call sites of all return transitions in the NWA
	 *
     */
	
    const std::set< State> NWA::getCalls( ) const
    {
      const Returns call = trans.getReturns();
       StateSet calls;
      for( ReturnIterator it = call.begin(); it != call.end(); it++ )
      {
          calls.insert( Trans::getCallSite(*it) );
      }
      return calls;
    }

    /**
     * 
     * @brief returns the call site states associated with the given exit point 
     *        and symbol
     * 
     * @param - exitPoint: the exit point whose call site states to obtain
     * @param - symbol: the symbol whose call site states to obtain
     * @return the set of all call sites associated with the given exit point
     *          and symbol
     *
     */
    
    const std::set< State> NWA::getCalls_Exit( State exitPoint, Symbol symbol ) const
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns call = trans.getTransExit(exitPoint);
       StateSet calls;
      for( ReturnIterator it = call.begin(); it != call.end(); it++ )
      {
        if( symbol == Trans::getReturnSym(*it) )
          calls.insert( Trans::getCallSite(*it) );
      }
      return calls;
    }

    /**
     * 
     * @brief returns the call site/symbol pairs associated with the given
     *        exit point
     *
     * @param - exitPoint: the exit point whose call site/symbol pairs to obtain
     * @return the set of call site/symbol pairs associated with the given
     *         exit point
     *
     */
    
    const std::set<std::pair< State, Symbol> > NWA::getCalls_Exit( State exitPoint ) const
    {
      assert(exitPoint < wali::WALI_BAD_KEY);

      const Returns call = trans.getTransExit(exitPoint);
       std::set<std::pair<State,Symbol> > calls;
      for( ReturnIterator it = call.begin(); it != call.end(); it++ )
      {
        calls.insert( std::pair<State,Symbol>(Trans::getCallSite(*it),Trans::getReturnSym(*it)) );
      }
      return calls;
    }

    /**
     * 
     * @brief returns the call site states associated with the given symbol
     *        and return site
     *
     * @param - symbol: the symbol whose call site states to obtain
     * @param - returnSite: the return site whose call site states to obtain
     * @return the set of call site states associated with the given symbol
     *          and return site
     *
     */
    
    const  std::set< State> NWA::getCalls_Ret( Symbol symbol, State returnSite ) const
    {
      assert(returnSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns call = trans.getTransRet(returnSite);
       StateSet calls;
      for( ReturnIterator it = call.begin(); it != call.end(); it++ )
      {
        if( symbol == Trans::getReturnSym(*it) )
          calls.insert( Trans::getCallSite(*it) );
      }
      return calls;
    }

    /**
     * 
     * @brief returns the call site/symbol pairs associated with the given
     *        return site
     *
     * @param - returnSite: the return site whose call site/symbol pairs to
     *                      obtain
     * @return the set of all call site/symbol pairs associated with the 
     *          given return site
     *
     */
    
    const std::set<std::pair< State, Symbol> > NWA::getCalls_Ret( State returnSite ) const
    {
      assert(returnSite < wali::WALI_BAD_KEY);

      const Returns call = trans.getTransRet(returnSite);
       std::set<std::pair<State,Symbol> > calls;
      for( ReturnIterator it = call.begin(); it != call.end(); it++ )
      {
        calls.insert( std::pair<State,Symbol>(Trans::getCallSite(*it),Trans::getReturnSym(*it)) );
      }
      return calls;
    }

    /**
     * 
     * @brief returns the symbols that label return transitions in the NWA
     *
     * @return the set of symbols that label return transitions in the NWA
     *
     */
    
    const std::set< Symbol> NWA::getReturnSym(  ) const
    {
      const Returns rets = trans.getReturns();
       std::set<Symbol> syms;
      for( ReturnIterator it = rets.begin(); it != rets.end(); it++ )
      {
        syms.insert( Trans::getReturnSym(*it) );
      }
      return syms;
    }

    /**
     * 
     * @brief returns the symbols that label return transitions from 'exitPoint'
     *        to 'returnSite' with call-predecessor 'callSite' in the NWA
     *
     * @param - exitPoint: the exit point of the desired transition
     * @param - callSite: the call site of the desired transition
     * @param - returnSite: the return site of the desired transition
     * @return the set of symbols that label return transitions from 'exitPoint'
     *          to 'returnSite' with call-predecessor 'callSite' in the NWA
     *
     */
    
    const std::set< Symbol> NWA::getReturnSym( State exitPoint, State callSite, State returnSite  ) const
      {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(callSite < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransExit(exitPoint);
       std::set<Symbol> syms;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        if( (Trans::getCallSite(*it) == callSite) && (returnSite == Trans::getReturnSite(*it)) )
        {
          syms.insert( Trans::getReturnSym(*it) );
        }
      }
      return syms;
    }

    /**
       * 
       * @brief returns the symbols that label return transitions from 'exitPoint'
       *        n the NWA
       *
       * @param - exitPoint: the exit point of the desired transition
       * @return the set of symbols that label return transitions from 'exitPoint'
       *          in the NWA
       *
       */
    
    const std::set< Symbol> NWA::getReturnSym_Exit( State exitPoint ) const
      {
      assert(exitPoint < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransExit(exitPoint);
       std::set<Symbol> syms;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        syms.insert( Trans::getReturnSym(*it) );
      }
      return syms;
    }

    /**
       * 
       * @brief returns the symbols that label return transitions 
       *        with call-predecessor 'callSite' in the NWA
       *
       * @param - callSite: the call site of the desired transition
       * @return the set of symbols that label return transitions 
       *          with call-predecessor 'callSite' in the NWA
       *
       */
    
    const std::set< Symbol> NWA::getReturnSym_Call( State callSite ) const
      {
      assert(callSite < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransPred(callSite);
       std::set<Symbol> syms;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        syms.insert( Trans::getReturnSym(*it) );
      }
      return syms;
    }

    /**
       * 
       * @brief returns the symbols that label return transitions 
       *        to 'returnSite' in the NWA
       *
       * @param - returnSite: the return site of the desired transition
       * @return the set of symbols that label return transitions 
       *          to 'returnSite' in the NWA
       *
       */
    
    const std::set< Symbol> NWA::getReturnSym_Ret( State returnSite ) const
      {
      assert(returnSite < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransRet(returnSite);
       std::set<Symbol> syms;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        syms.insert( Trans::getReturnSym(*it) );
      }
      return syms;
    }

    /**
       * 
       * @brief returns the symbols that label return transitions from 'exitPoint'
       *        with call-predecessor 'callSite' in the NWA
       *
       * @param - exitPoint: the exit point of the desired transition
       * @param - callSite: the call site of the desired transition
       * @return the set of symbols that label return transitions from 'exitPoint'
       *          with call-predecessor 'callSite' in the NWA
       *
       */

    const std::set< Symbol> NWA::getReturnSym_ExitCall( State exitPoint, State callSite  ) const
      {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(callSite < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransExit(exitPoint);
       std::set<Symbol> syms;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        if( (Trans::getCallSite(*it) == callSite) )
        {
          syms.insert( Trans::getReturnSym(*it) );
        }
      }
      return syms;
    }

    /**
       * 
       * @brief returns the symbols that label return transitions from 'exitPoint'
       *        to 'returnSite' in the NWA
       *
       * @param - exitPoint: the exit point of the desired transition
       * @param - returnSite: the return site of the desired transition
       * @return the set of symbols that label return transitions from 'exitPoint'
       *          to 'returnSite' in the NWA
       *
       */
    
    const std::set< Symbol> NWA::getReturnSym_ExitRet( State exitPoint, State returnSite  ) const
      {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransExit(exitPoint);
       std::set<Symbol> syms;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        if( (returnSite == Trans::getReturnSite(*it)) )
        {
          syms.insert( Trans::getReturnSym(*it) );
        }
      }
      return syms;
    }

    /**
       * 
       * @brief returns the symbols that label return transitions 
       *        to 'returnSite' with call-predecessor 'callSite' in the NWA
       *
       * @param - callSite: the call site of the desired transition
       * @param - returnSite: the return site of the desired transition
       * @return the set of symbols that label return transitions 
       *          to 'returnSite' with call-predecessor 'callSite' in the NWA
       *
       */
    
    const std::set< Symbol> NWA::getReturnSym_CallRet( State callSite, State returnSite  ) const
      {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransPred(callSite);
       std::set<Symbol> syms;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        if( (returnSite == Trans::getReturnSite(*it)) )
        {
          syms.insert( Trans::getReturnSym(*it) );
        }
      }
      return syms;
    }



    /**
     * 
     * @brief returns the return site states associated with the given symbol
     *
     * @param - symbol: the symbol whose reutrn site states to obtain
     * @return the set of return site states associated with the given symbol
     *
     */
    
    const std::set< State> NWA::getReturns_Sym( Symbol symbol ) const
    {
      assert(symbol < wali::WALI_BAD_KEY);
      
      const Returns ret = trans.getReturns();
       StateSet returns;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        if( symbol == Trans::getReturnSym(*it) )
        {
          returns.insert( Trans::getReturnSite(*it) );
        }
      }
      return returns;
    }

    /**
     * 
     * @brief returns the return site states associated with the given exit 
     *        point, call site, and symbol
     *
     * @param - exitPoint: the exit point whose return site states to obtain
     * @param - callSite: the call site whose return site states to obtain
     * @param - symbol: the symbol whose return site states to obtain
     * @return the set of return site states associated with the given exit
     *          point, call site, and symbol
     *
     */
    
    const std::set< State> NWA::getReturns( State exitPoint, State callSite, Symbol symbol ) const
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(callSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransExit(exitPoint);
       StateSet returns;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        if( (Trans::getCallSite(*it) == callSite) && (symbol == Trans::getReturnSym(*it)) )
        {
          returns.insert( Trans::getReturnSite(*it) );
        }
      }
      return returns;
    }

    /**
     * 
     * @brief returns the symbol/return site pairs associated with the given exit point 
     *        and call site
     *
     * @param - exit: the exit point whose symbol/return site pairs to obtain
     * @param - callSite: the call site whose symbol/return site pairs to obtain
     * @return the set of symbol/return site pairs associated with the given exit point 
     *          and call site
     *
     */
    
    const std::set<std::pair< Symbol, State> > NWA::getReturns( State exit, State callSite ) const
    {
      assert(exit < wali::WALI_BAD_KEY);
      assert(callSite < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransExit(exit);
       std::set<std::pair<Symbol,State> > returns;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        if( Trans::getCallSite(*it) == callSite )
        {
          returns.insert( std::pair<Symbol,State>(Trans::getReturnSym(*it),Trans::getReturnSite(*it)) );
        }
      }
      return returns;
    }

	/**
     * 
	 * @brief returns the return sites of all return transitions in the NWA
	 *
	 * @return the set of return sites of all return transitions in the NWA
	 *
     */
	
    const std::set< State> NWA::getReturns( ) const
    {
      const Returns ret = trans.getReturns();
       StateSet returns;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
          returns.insert( Trans::getReturnSite(*it) );
      }
      return returns;
    }

    /**
     * 
     * @brief returns the return site states associated with the given exit 
     *        point and symbol
     *
     * @param - exitPoint: the exit point whose return site states to obtain
     * @param - symbol: the symbol whose return site states to obtain
     * @return the set of return site states associated with the given exit
     *         point and symbol
     *
     */
    
    const std::set< State> NWA::getReturns_Exit( State exitPoint, Symbol symbol ) const
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransExit(exitPoint);
       StateSet returns;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        if( symbol == Trans::getReturnSym(*it) )
          returns.insert( Trans::getReturnSite(*it) );
      }
      return returns;
    }

    /**
     * 
     * @brief returns the symbol/return site pairs associated with the given
     *        exit point
     *
     * @param - exitPoint: the exit point whose symbol/return site pairs to obtain
     * @return the set of symbol/return site pairs associated with the given 
     *          exit point
     *
     */
    
    const std::set<std::pair< Symbol, State> > NWA::getReturns_Exit( State exitPoint ) const
    {
      assert(exitPoint < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransExit(exitPoint);
       std::set<std::pair<Symbol,State> > returns;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        returns.insert( std::pair<Symbol,State>(Trans::getReturnSym(*it),Trans::getReturnSite(*it)) );
      }
      return returns;
    }

    /**
     * 
     * @brief returns the return site states associated with the given call 
     *        site and symbol
     *
     * @param - callSite: the call site whose return site states to obtain
     * @param - symbol: the symbol whose return site states to obtain
     * @return the set of return site states associated with the given call
     *          site and symbol
     *
     */
    
    const std::set< State> NWA::getReturns_Call( State callSite, Symbol symbol ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransPred(callSite);
       StateSet returns;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        if( symbol == Trans::getReturnSym(*it) )
          returns.insert( Trans::getReturnSite(*it) );
      }
      return returns;
    }

    /**
     * 
     * @brief returns the symbol/return site pairs associated with the given
     *        call site
     *
     * @param - callSite: the call site whose symbol/return site pairs to obtain
     * @return the set of symbol/return site pairs associated with the given
     *          call site
     *
     */
    
    const std::set<std::pair< Symbol, State> > NWA::getReturns_Call( State callSite ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransPred(callSite);
       std::set<std::pair<Symbol,State> > returns;
      for( ReturnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        returns.insert( std::pair<Symbol,State>(Trans::getReturnSym(*it),Trans::getReturnSite(*it)) );
      }
      return returns;
    }

    /**
     * 
     * @brief returns the return sites associated with the given call site
     *
     * @param - callSite: the call site whose return sites to obtain
     * @return the set of return sites associated with the given call site
     *
     */
    
    
    const std::set< State> NWA::getReturnSites( State callSite ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);
      return trans.getReturnSites(callSite);
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
    
    bool NWA::addReturnTrans( State from, State pred, Symbol sym, State to )
    {
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(pred < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      assert( sym != WALI_EPSILON ); //An Epsilon symbol on a return doesn't make sense.

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if( isStuckState(from) )
      {
        assert( isStuckState(to) );
      }
      else if(  isStuckState(pred) )
      {
        assert( isStuckState(to) );
      }

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
    
    bool NWA::addReturnTrans( Return & rt )
    {
      assert(stuck);

      assert(Trans::getExit(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getCallSite(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getReturnSym(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getReturnSite(rt) < wali::WALI_BAD_KEY);

      assert( Trans::getReturnSym(rt) != WALI_EPSILON ); //An Epsilon symbol on a return doesn't make sense.

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if(  isStuckState(Trans::getExit(rt)) )
      {
        assert( isStuckState(Trans::getReturnSite(rt)));
      }
      else if( isStuckState(Trans::getCallSite(rt)) )
      {
        assert( isStuckState(Trans::getReturnSite(rt)) );
      }

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
    
    bool NWA::removeReturnTrans( State from, Symbol sym, State to )
    {
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      if(! isState(from) || ! isSymbol(sym) || ! isState(to) ||  sym == WALI_EPSILON )
        return false;
     
      bool removed = false;
      const Trans::States preds = trans.getCallSites(from,to);
      for( Trans::stateIterator it = preds.begin(); it != preds.end(); it++ )
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
    
    bool NWA::removeReturnTrans( State from, State pred, Symbol sym, State to )
    {
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(pred < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      if(! isState(from) || ! isState(pred) || ! isSymbol(sym) || ! isState(to) 
        || sym == WALI_EPSILON )
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
    
    bool NWA::removeReturnTrans( const Return & rt )
    {
      assert(stuck);

      assert(Trans::getExit(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getCallSite(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getReturnSym(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getReturnSite(rt) < wali::WALI_BAD_KEY);

      if(! isState(Trans::getExit(rt))
        || ! isState(Trans::getCallSite(rt))
        || ! isSymbol(Trans::getReturnSym(rt))
        || ! isState(Trans::getReturnSite(rt)) 
        || Trans::getReturnSym(rt) == WALI_EPSILON )
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
    
    
    size_t NWA::sizeReturnTrans( ) const
    {
      return trans.sizeReturn();
    }

    //Building NWAs


    /***
    * @brief constructs an NWA which is the projection of the given NWA to the states
    * provided
    */
    
    void NWA::projectStates(const NWA & other, const StateSet &prjStates)
    {
      //copy data from other
      stuck = other.stuck;
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
      for(stateIterator it = beginStates(); it!=endStates(); it++) {
        if( prjStates.count(*it) == 0 && !isStuckState(*it) ) {
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
    /**
     *
     * @brief constructs the NWA resulting from the union of the given NWAs 
     *
     * @param - first: the NWA to union with 'second'
     * @param - second: the NWA to union with 'first'
     *
     */
    
    void NWA::unionNWA( NWA const & first, NWA const & second )
    {
      //Q: Do we need to guarantee that there is no overlap in states between machines? 
      //A: YES
      //    If there is overlap, then we could conceivably go through the first half of one
      //    machine and then skip to the other machine, thereby not completing either machine.
      //Q: How should we do this?  
      //    1. Use the same base algorithm as intersection.  
      //    2. Generate new keys by the pairing scheme (1,k) for keys in first and (2,k) for keys in second.
      //    3. Implement a check for overlapping state keys and assert that no such overlap occurs.
      //A: 3 is easiest

      //Q: How should clientInfos be generated for the union NWA?
      //A: The clientInfos from the component machines are copied and added to the union NWA.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      //Test state Key overlap of the two NWAs.
      assert(! overlap(first,second) );

      //Check that the stuck state of this NWA does not exist as a state in either component machine 
      //('first' and 'second') unless it is the stuck state of that machine.
      if(! first.isStuckState(getStuckState()) )
        assert(! first.isState(getStuckState()) );
      if(! second.isStuckState(getStuckState()) )
        assert(! second.isState(getStuckState()) );

	    //Clear all states(except the stuck state) and transitions from this machine.
      State stuckSt = getStuckState();
      ClientInfoRefPtr stuckStInfo = getClientInfo( stuckSt );
      clear();
      setStuckState(stuckSt);
      setClientInfo(stuckSt, stuckStInfo ); // set the client info associated with the stuck state


      //Copy all of the functionality of the two machines.  
      //States (Initial and final state information included.)
      states.addAll(first.states);   //Note: This includes copying clientInfo information over.  
      states.addAll(second.states);  //Note: This includes copying clientInfo information over.

      //Transitions (Call, internal, and return.)
      trans.addAllTrans(first.trans);
      trans.addAllTrans(second.trans);      
    }

    bool NWA::isTransitionPossible( const State &src, const Symbol &sym, const State &tgt) 
    {
      (void) src;
      (void) sym;
      (void) tgt;
      return true;
    }

    /**
     *
     * @brief constructs the NWA which is the intersection of the given NWAs
     *
     * @param - first: the NWA to intersect with 'second'
     * @param - second: the NWA to intersect with 'first'
     *	
     */
    
    void NWA::intersect( NWA const & first, NWA const & second ) 
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

      //Make sure we can modify transitions in this machine.
      assert(stuck);

	  //Clear all states(except the stuck state) and transitions from this machine.
      State stuckSt = getStuckState();
      ClientInfoRefPtr stuckStInfo = getClientInfo( stuckSt );
      clear();
      setStuckState(stuckSt);
      setClientInfo(stuckSt, stuckStInfo ); // set the client info associated with the stuck state

       std::set<StatePair> visitedPairs; // All the pairs of states we have ever encountered.
      std::deque<StatePair> worklistPairs; // Pairs of states yet to be processed
      typedef std::map<StatePair, Key> PairStMap;
      PairStMap pairToStMap; // The corresponding state in the product 
                             // for all intersectable pairs encountered

      //Start the worklist with all possible initial states of the intersection NWA.
      for( stateIterator fit = first.beginInitialStates(); fit != first.endInitialStates(); fit++ ) 
      {
        for( stateIterator sit = second.beginInitialStates(); sit != second.endInitialStates(); sit++ ) 
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


      //#if 0 // Commented out because I don't know how this interacts with the new stuck state behavior
    
    void NWA::removeImplicitTransitions()
    {
      stuck = true;
        
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
        if( Trans::getCallSite(*call) != Trans::getEntry(*call)
            && Trans::getEntry(*call) != states.getStuckState())
        {
          stuckStates.erase(Trans::getCallSite(*call));
        }
      }

      for( InternalIterator internal = trans.beginInternal();
           internal != trans.endInternal(); ++internal)
      {
        if( Trans::getSource(*internal) != Trans::getTarget(*internal) 
            && Trans::getTarget(*internal) != states.getStuckState())
        {
          stuckStates.erase(Trans::getSource(*internal));
        }
      }

      for( ReturnIterator ret = trans.beginReturn(); ret != trans.endReturn(); ++ret)
      {
        if( Trans::getExit(*ret) != Trans::getReturnSite(*ret) 
            && Trans::getReturnSite(*ret) != states.getStuckState())
        {
          stuckStates.erase(Trans::getExit(*ret));
        }
      }

      // Now we have a list of stuck states. Go through and remove each,
      // but only if the acceptingness is the same as the true stuck state.
      for(  StateSet::iterator stuck = stuckStates.begin();
           stuck != stuckStates.end(); ++stuck)
      {
        if( isFinalState(*stuck) == isFinalState(states.getStuckState()) )
        {
          if(states.isStuckState(*stuck)) {
            removeState(*stuck);
          }
          else {
            // Remove all the explicit transitions to the stuck state at least
            trans.removeTransWith(*stuck);
          }
        }
      }
    }
      //#endif

    
    void NWA::pruneUnreachableForward(const StateSet & sources)
    {
      std::deque<State> worklist(sources.begin(), sources.end());  
      StateSet unreachables = getStates();
      StateSet visited;
      for( stateIterator it = sources.begin(); it!=sources.end(); it++) {
        visited.insert(*it);
      }   

      while( ! worklist.empty() ) {
        State src = worklist.front();
        worklist.pop_front();
        // book keeping
        unreachables.erase(src);
        // for each target
        std::set<std::pair<Symbol,State> > succs = getTargets(src);
        for( std::set<std::pair<Symbol,State> >::const_iterator it = succs.begin(); it!=succs.end(); it++) {
          // if not visited
          if(visited.count(it->second) == 0 ) {
            worklist.push_back(it->second);
            visited.insert(it->second);
          }
        }

        // for each entry
        succs = getEntries(src);
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

      for(stateIterator it = unreachables.begin(); it!=unreachables.end(); it++) {
        removeState(*it);
      }
      return;
    }

       
    void NWA::pruneUnreachableBackward(const StateSet & targets)
    {      
      std::deque<State> worklist(targets.begin(), targets.end());  
      StateSet unreachables = getStates();
      StateSet visited;
      for( stateIterator it = targets.begin(); it!=targets.end(); it++) {
        visited.insert(*it);
      }   

      while( ! worklist.empty() ) {
        State src = worklist.front();
        worklist.pop_front();
        // book keeping
        unreachables.erase(src);
        // for each source
        std::set<std::pair<State,Symbol> > succs = getSources(src);
        for( std::set<std::pair<Symbol,State> >::const_iterator it = succs.begin(); it!=succs.end(); it++) {
          // if not visited
          if(visited.count(it->first) == 0 ) {
            worklist.push_back(it->first);
            visited.insert(it->first);
          }
        }

        // for each call successor state
        succs = getCallSites(src);
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

      for(stateIterator it = unreachables.begin(); it!=unreachables.end(); it++) {
        removeState(*it);
      }
      return;

    }

    
    void NWA::pruneUnreachableInitial() {
      pruneUnreachableForward( getInitialStates() );
    }

    
    void NWA::pruneUnreachableFinal() {
      pruneUnreachableBackward( getFinalStates() );
    }

    
    void NWA::chop()
    {
      pruneUnreachableInitial();
      pruneUnreachableFinal();

    }

    /**
     *
     * @brief constructs the NWA resulting from the concatenation of the given NWAs
     *
     * @param - first: the NWA to which 'second' should be concatenated
     * @param - second: the NWA to concatenate onto the end of 'first'
     *
     */
    
    void NWA::concat( NWARefPtr first, NWARefPtr second )
    {
      //Q: Do we need to guarantee that there is no overlap in states between machines?
      //A: YES!
      //    If there is overlap, then we could conceivably go through the first half of one
      //    machine and then skip to the other machine before completing the first machine.
      //Q: How should we do this?  
      //    1. Use the same base algorithm as intersection.  
      //    2. Generate new keys by the pairing scheme (1,k) for keys in first and (2,k) for keys in second.
      //    3. Implement a check for overlapping state keys and assert that no such overlap occurs.
      //A: 3 is easiest

      //Q: How should clientInfos be generated for the concatenated NWA?
      //A: The clientInfos from the component machines are copied and added to the concatenated NWA.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      //Test state Key overlap of the two NWAs.
      assert(! overlap(*first,* second) );

      //Check that the stuck state of this NWA does not exist as a state in either component machine 
      //('first' and 'second') unless it is the stuck state of that machine.
      if(! first->isStuckState(getStuckState()) )
        assert(! first->isState(getStuckState()) );
      if(! second->isStuckState(getStuckState()) )
        assert(! second->isState(getStuckState()) );

	  //Clear all states(except the stuck state) and transitions from this machine.
      State stuckSt = getStuckState();
      ClientInfoRefPtr stuckStInfo = getClientInfo( stuckSt );
      clear();
      setStuckState(stuckSt);
      setClientInfo(stuckSt, stuckStInfo ); // set the client info associated with the stuck state

      //Duplicate all of the functionality of the first machine (except the final state property).
      states.addAllStates(first->states);   //Note: This includes copying clientInfo information over.
      states.addAllInitialStates(first->states);
      trans.addAllTrans(first->trans);

      //Duplicate all of the functionality of the second machine (except the initial state property).
      states.addAllStates(second->states);  //Note: This includes copying clientInfo information over.
      states.addAllFinalStates(second->states);
      trans.addAllTrans(second->trans);

      //Add epsilon transitions from the final states of the first machine to the initial
      //states of the second machine.
      for( stateIterator fit = first->beginFinalStates(); fit != first->endFinalStates(); fit++ )
      {
        for( stateIterator sit = second->beginInitialStates(); sit != second->endInitialStates(); sit++ )
        {
          addInternalTrans(*fit,WALI_EPSILON,*sit);
        }
      }
    }

    /**
     * @brief constructs the NWA which is the reverse of the given NWA
     *
     * @param - first: the NWA to reverse
     * 
     */
    
    void NWA::reverse( NWARefPtr first )
    {
      //Note: (implicit) transitions to the stuck state in the original machine 
      //       cause problems in the reverse machine if the stuck state is a final state 
      //Q: how do I implement this?
      //A: realize all transitions of first before computing the reverse if stuck is final
      //Note: the stuck state can now never be final, so we do not need to worry about this any more

      //Q: How should clientInfos be generated for the reversed NWA?
      //A: The clientInfos from the component machines are copied and added to the reversed NWA.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      //Check that the stuck state of this NWA does not exist as a state in 'first' 
      //Note: it cannot be the stuck state of 'first' because then it would have an outgoing transition.
      assert(! first->isState(getStuckState()) );

	  //Clear all states(except the stuck state) and transitions from this machine.
      State stuckSt = getStuckState();
      ClientInfoRefPtr stuckStInfo = getClientInfo( stuckSt );
      clear();
      setStuckState(stuckSt);
      setClientInfo(stuckSt, stuckStInfo ); // set the client info associated with the stuck state

      //The reverse machine has all the states of the original machine.
      states.addAllStates(first->states); //Note: This includes copying clientInfo information over. 

      //Swap initial and final state functionality.
      //Note: we know that this does not cause the stuck state to become a final state
      //      because the stuck state was not an initial state of the original machine 
      //      (as it was not even a state in the original machine).
      for( stateIterator it = first->beginInitialStates(); 
        it != first->endInitialStates(); it++ )
      {
        addFinalState(*it);
      }
      for( stateIterator it = first->beginFinalStates(); 
        it != first->endFinalStates(); it++ )
      {
        addInitialState(*it);
      }

      //Duplicate internal transitions with source/target swapped.
      for( InternalIterator it = first->beginInternalTrans(); 
        it != first->endInternalTrans(); it++ )
      {
        addInternalTrans(Trans::getTarget(*it),
                          Trans::getInternalSym(*it),
                          Trans::getSource(*it));
      }

      //Duplicate return transitions as call transitions with (return,sym,exit).
      for( ReturnIterator it = first->beginReturnTrans(); 
        it != first->endReturnTrans(); it++ )
      {
        addCallTrans(Trans::getReturnSite(*it),
                      Trans::getReturnSym(*it),
                      Trans::getExit(*it));
      }

      //Duplicate call transitions with associated return transitions as 
      //return transitions with (entry,return,sym,call).
      for( CallIterator cit = first->beginCallTrans(); 
        cit != first->endCallTrans(); cit++ )
      {
        for( ReturnIterator rit = first->beginReturnTrans(); 
          rit != first->endReturnTrans(); rit++ )
        {
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            addReturnTrans(Trans::getEntry(*cit),
                            Trans::getReturnSite(*rit),
                            Trans::getCallSym(*cit),
                            Trans::getCallSite(*cit));
          }
        }
      }

    }

    /**
     *
     * @brief constructs the NWA resulting from performing Kleene-* on the given NWA
     *
     * @param - first: the NWA to perform Kleene-* on
     *
     */
    
    void NWA::star( NWARefPtr first )
    {
      //TODO: ponder the following ... 
      //Q: how do we prevent the stuck state from being the same as any of the states that we
      //      generate as a part of this process?
      //A: check against all possible overlaps?  this might be expensive, but it would work

      //Q: How should clientInfos be generated for the star NWA?
      //A: The clientInfos from the component machine are copied to both the direct copies of
      //    states and the primed copies of states and added to the star NWA.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

	  //Clear all states(except the stuck state) and transitions from this machine.
      State stuckSt = getStuckState();
      ClientInfoRefPtr stuckStInfo = getClientInfo( stuckSt );
      clear();
      setStuckState(stuckSt);
      setClientInfo(stuckSt, stuckStInfo ); // set the client info associated with the stuck state

      //Somehow mark unmatched nesting edges as if they are pending (tag its state so that 
      //at a return the states labeling the incident nesting edges are ignored).
      State prime = wali::getKey("prime");

      //The state-space of A* is Q + Q'.
      states.addAll(first->states); //Note: This includes copying clientInfo information over.    
      for( stateIterator sit = first->beginStates(); sit != first->endStates(); sit++ )
      {
        State sp = wali::getKey(*sit,prime);
        states.addState(sp);

        //Note: The clientInfos for the states in Q' are copies of the clientInfos for the 
        //      corresponding states from Q.
        //Set the clientInfo of this state.
        ClientInfo info = *(first->getClientInfo(*sit));
        ClientInfoRefPtr ci = ClientInfoRefPtr(new ClientInfo(info));
        states.setClientInfo(sp,ci);
      }

      //The initial and final states of A* are Q0'. 
      for( stateIterator sit = first->beginInitialStates(); 
            sit != first->endInitialStates(); sit++ )
      {
        State sp = wali::getKey(*sit,prime);
        addInitialState(sp);
        addFinalState(sp);
      }

      //Transitions of A*:

      //Internal: for each (q,a,p) in delta_i, A* gets (q,a,p) and (q',a,p') and if
      //          p in Qf, then (q,a,r') and (q',a,r') for each r in Q0
      for( InternalIterator iit = first->beginInternalTrans();
            iit != first->endInternalTrans(); iit++ )
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
        if( first->isFinalState(p) )
        {
          //for each r in Q0
          for( stateIterator sit = first->beginInitialStates(); 
                sit != first->endInitialStates(); sit++ )
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
      for( CallIterator cit = first->beginCallTrans();
            cit != first->endCallTrans(); cit++ )
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
        if( first->isFinalState(p) )
        {
          //for each r in Q0
          for( stateIterator sit = first->beginInitialStates();
                sit != first->endInitialStates(); sit++ )
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
      for( ReturnIterator rit = first->beginReturnTrans();
            rit != first->endReturnTrans(); rit++ )
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
        if( first->isFinalState(p) )
        {
          //for each s in Q0
          for( stateIterator sit = first->beginInitialStates();
                sit != first->endInitialStates(); sit++ )
          { 
            State sp = wali::getKey(*sit,prime);

            //(q,r,a,s')
            addReturnTrans(q,r,a,sp);

            //(q,r',a,s')
            addReturnTrans(q,rp,a,sp);
          } 
        }

        //if r in Q0
        if( first->isInitialState(r) )
        {
          //for each s in Q
          for( stateIterator sit = first->beginStates();
                sit != first->endStates(); sit++ )
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
            if( first->isFinalState(p) )
            {
              //for each t in Q0
              for( stateIterator it = first->beginInitialStates();
                    it != first->endInitialStates(); it++ )
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

    /**
     * 
     * @brief constructs the NWA that is the complement of the given NWA
     *
     * @param - first: the NWA to perform the complement of
     *
     */
    
    void NWA::complement( NWARefPtr first )
    {
      //Q: How should clientInfos be generated for the complemented NWA?
      //A: The clientInfos from the component machines are copied and added to the complemented NWA.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      //Check that the stuck state of this NWA does not exist as a state in 'first' 
      //Note: it cannot be the stuck state of 'first' because it is going to become a final state
      //      prior to the completion of adding transitions to this machine.
      assert(! first->isState(getStuckState()) );

	  //Clear all states(except the stuck state) and transitions from this machine.
      State stuckSt = getStuckState();
      ClientInfoRefPtr stuckStInfo = getClientInfo( stuckSt );
      clear();
      setStuckState(stuckSt);
      setClientInfo(stuckSt, stuckStInfo ); // set the client info associated with the stuck state


      //Start with a deterministic copy of the given NWA.
      // FIXME: keep information about whether a machine is deterministic
      if(false) //! first->isDeterministic() )
      {
        determinize(first);   //Note: determinize() will take care of clientInfo information.
      }
      else
      {
        states.addAll(first->states); //Note: This includes copying clientInfo information over.          
        addAllSymbols(first->symbols);
        trans.addAllTrans(first->trans);
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


#if 0
    template< T>
    struct terrible_set_hasher
    {
      size_t operator() (T const & set) const {
        return set.size();
      }
    };

    template< T>
    struct terrible_set_comparer
    {
      bool operator() (T const & lhs, T const & rhs) const {
        if (lhs.size() != rhs.size()) return false;
          
        for ( T::const_iterator li = lhs.begin() ; li != lhs.end() ; ++li) {
          if (rhs.find(*li) == rhs.end()) return false;
        }

        return true;
      }
    };
#endif
      

    /**
     *
     * @brief constructs a deterministic NWA that is equivalent to the given NWA.
     *
     * @param - nondet: the NWA to determinize
     *
     */
    
    void NWA::determinize( NWARefPtr nondet )
    {
#ifdef USE_BUDDY
      wali::relations::buddyInit();
#endif

#ifdef USE_BUDDY
#  define DECLARE(type, name)  type name(nondet->largestState())
#else
#  define DECLARE(type, name)  type name
#endif
        
      //TODO: ponder the following ...
      //Q: how do we guarantee that the stuck state is not any of the states generated here?
      
      //Q: what should be done with clientInfos here?
      //A: use helper methods similar to the treatment of clientInfos for intersection

      assert(stuck);

      //Clear all states(except the stuck state) and transitions from this machine.
      State stuckSt = getStuckState();
      ClientInfoRefPtr stuckStInfo = getClientInfo( stuckSt );
      clear();
      setStuckState(stuckSt);
      setClientInfo(stuckSt, stuckStInfo ); // set the client info associated with the stuck state


      using namespace wali::relations;

      typedef std::set<std::pair<State, State> > SetBinaryRelation;
      typedef  RelationTypedefs<State>::BinaryRelation BinaryRelation;
      typedef  RelationTypedefs<State>::TernaryRelation TernaryRelation;

#if 0
      typedef  std::tr1::unordered_set<BinaryRelation
          , terrible_set_hasher<BinaryRelation>
          //, terrible_set_comparer<BinaryRelation>
          >
          RelationSet;
#endif

#ifdef USE_BUDDY
      typedef VectorSet<BinaryRelation> RelationSet;
#else
      typedef std::set<BinaryRelation> RelationSet;
#endif

      NWARefPtr nondet_copy(new NWA(*nondet));
      nondet = nondet_copy;

      {
        //BlockTimer timer("Reify consumer transitions (inside determinize)");
        nondet->realizeImplicitTrans();
      }

      // Construct Id
      DECLARE(BinaryRelation, Id);
      for( stateIterator sit = nondet->beginStates(); sit != nondet->endStates(); sit++ )
      {
        Id.insert(std::pair<State,State>(*sit,*sit));
      }
      
      // Construct Id0
      DECLARE(BinaryRelation, Id0);
      for( stateIterator sit = nondet->beginInitialStates(); sit != nondet->endInitialStates(); sit++ )
      {
          Id0.insert(std::pair<State,State>(*sit,*sit));
      }
      
      //Construct the epsilon closure relation for the states in nondet.
      SetBinaryRelation pre_close; //Collapse epsilon transitions.
      SetBinaryRelation Ie;   //Internal transitions with epsilon.
      project_symbol_3<SetBinaryRelation>(Ie,nondet->trans.getInternals(), WALI_EPSILON);
#ifdef USE_BUDDY
      transitive_closure(pre_close,Ie);
#else
      transitive_closure<State>(pre_close,Ie);
#endif

      DECLARE(BinaryRelation, bdd_pre_close);
      for( SetBinaryRelation::const_iterator iter = pre_close.begin(); iter != pre_close.end(); ++iter) {
        bdd_pre_close.insert(*iter);
      }

      DECLARE(BinaryRelation, close);
      union_(close, bdd_pre_close, Id);

      // R0 is used later; to avoid recomputation we do it now
      // Epsilon Closure( {(q,q) | q is an element of Q_in in nondeterministic NWA } )
      DECLARE(BinaryRelation, R0);
#ifdef USE_BUDDY
      compose/*<St>*/(R0,Id0,close);
#else
      compose<State>(R0,Id0,close);
#endif

      //Make a key for this state.
      State r0 = makeKey(R0);
      
      //Add the initial state to the deterministic NWA.
      addInitialState(r0);

      //Set the client info for this new state.
      ClientInfoRefPtr CI;
      mergeClientInfo(nondet,R0,r0,CI);
      states.setClientInfo(r0,CI);

      //Put the initial state on the worklist.
      RelationSet wl;
      wl.insert(R0); // or close

      //Keep track of all visited states.
      RelationSet visited;

      // Pre-compute some projections and stuff
      std::map<wali::Key, BinaryRelation> callTransPerSymbol;
      std::map<wali::Key, BinaryRelation> internalTransPerSymbol;
      std::map<wali::Key, TernaryRelation> returnTransPerSymbol;

      for( symbolIterator it = nondet->beginSymbols(); it != nondet->endSymbols(); it++ ) {
        if (*it == WALI_EPSILON) continue;    //Epsilon is handled with closure.
        if (*it == WALI_WILD) continue;

#ifdef USE_BUDDY
        internalTransPerSymbol[*it] = BinaryRelation(nondet->largestState());
        callTransPerSymbol[*it] = BinaryRelation(nondet->largestState());
        returnTransPerSymbol[*it] = TernaryRelation(nondet->largestState());
#endif
        
        project_symbol_3<BinaryRelation>(internalTransPerSymbol[*it], nondet->trans.getInternals(), *it);
        project_symbol_3<BinaryRelation>(internalTransPerSymbol[*it], nondet->trans.getInternals(), WALI_WILD);

        project_symbol_3<BinaryRelation>(callTransPerSymbol[*it], nondet->trans.getCalls(), *it);
        project_symbol_3<BinaryRelation>(callTransPerSymbol[*it], nondet->trans.getCalls(), WALI_WILD);   //Every symbol also matches wild.

        project_symbol_4(returnTransPerSymbol[*it], nondet->trans.getReturns(), *it);
        project_symbol_4(returnTransPerSymbol[*it], nondet->trans.getReturns(),WALI_WILD);   //Every symbol also matches wild.
      }

      
      //Process the states on the worklist.
      while(! wl.empty() )
      {
        //Take a state off of the worklist.
        BinaryRelation R = *wl.begin();
        wl.erase(wl.begin());
  
        //Mark this state as visited.
        visited.insert(R);

        //Make a key for this state.
        State r = makeKey(R);


        // Note: The clientInfo for this state was set before it was put on
        // the worklist
        if( isStuckState(r) ) continue;    
        
        //Check each symbol individually.
        for( symbolIterator it = nondet->beginSymbols(); it != nondet->endSymbols(); it++ )
        {
          if (*it == WALI_EPSILON) continue;    //Epsilon is handled with closure.
          if (*it == WALI_WILD) continue;       //Wild is matched to every symbol as we go.

          //Process internal transitions.
          //Compute the relation.
          DECLARE(BinaryRelation, Ri);
          DECLARE(BinaryRelation, Rtmpi);
          BinaryRelation const & Ii = internalTransPerSymbol[*it];

#if 0
          DECLARE(BinaryRelation, IiOrig);
          project_symbol_3(IiOrig,nondet->trans.getInternals(),*it);   
          project_symbol_3(IiOrig,nondet->trans.getInternals(),WALI_WILD);   //Every symbol also matches wild.
          
          if (Ii == IiOrig) {
            std::cout << "Ii == IiOrig holds!\n";
          }
          else {
            std::cout << "***** Ii == IiOrig VIOLATED\n";
            std::cout << "      Ii's size is " << Ii.size() << "\n";
            std::cout << "      Orig's size is " << IiOrig.size() << "\n";
          }
#endif

#ifdef USE_BUDDY
          compose/*<St>*/(Rtmpi,R,Ii);
          compose/*<St>*/(Ri,Rtmpi,close);
#else
          compose<State>(Rtmpi,R,Ii);
          compose<State>(Ri,Rtmpi,close);
#endif
          //Make a key for this state.
          State ri = makeKey(Ri);

          //Add the state to the deterministic NWA.
          addState(ri);
          //Add the transition to the deterministic NWA.
          addInternalTrans(r,*it,ri);

          //Adjust the client info for this state.
          ClientInfoRefPtr riCI;
          mergeClientInfoInternal(nondet,R,Ri,r,*it,ri,riCI);
          states.setClientInfo(ri,riCI);

          //Determine whether to add this state to the worklist.
          if( visited.find(Ri) == visited.end() )
          {
            wl.insert(Ri);
          }

          //Process call transitions.
          //Compute the relation.
          //BinaryRelation IdClose_Delta2;
          DECLARE(BinaryRelation, Rc);
          BinaryRelation const & Ic = callTransPerSymbol[*it];

#if 0
          DECLARE(BinaryRelation, IcOrig);
          project_symbol_3(IcOrig,nondet->trans.getCalls(),*it);  
          project_symbol_3(IcOrig,nondet->trans.getCalls(),WALI_WILD);   //Every symbol also matches wild.
          
          if (Ic == IcOrig) {
            std::cout << "Ic == IcOrig holds!\n";
          }
          else {
            std::cout << "***** Ic == IcOrig VIOLATED\n";
          }
#endif

          //compose/*<St>*/(IdClose_Delta2, R0, Ic);
          //compose/*<St>*/(Rc,IdClose_Delta2,close);
#ifdef USE_BUDDY
          compose/*<St>*/(Rc,Ic,close);
#else
          compose<State>(Rc,Ic,close);
#endif
          
          //Make a key for this state.
          State rc = makeKey(Rc);
          //Add the state to the deterministic NWA.
          addState(rc);
          //Add the transition to the deterministic NWA.
          addCallTrans(r,*it,rc);

          //Adjust the client info for this state.
          ClientInfoRefPtr rcCI;
          mergeClientInfoCall(nondet,R,Rc,r,*it,rc,rcCI);
          states.setClientInfo(rc,rcCI);

          //Determine whether to add this state to the worklist.
          if( visited.find(Rc) == visited.end() )
          {
            wl.insert(Rc);
          }

          //Process return transitions.
          TernaryRelation const & Ir = returnTransPerSymbol[*it];

#if 0
          TernaryRelation IrOrig;
          project_symbol_4(IrOrig,nondet->trans.getReturns(),*it);    
          project_symbol_4(IrOrig,nondet->trans.getReturns(),WALI_WILD);   //Every symbol also matches wild.

          if (Ir == IrOrig) {
            std::cout << "Ir == IrOrig holds!\n";
          }
          else {
            std::cout << "***** Ir == IrOrig VIOLATED\n";
          }
#endif

          //For each possible call predecessor:
          for(  RelationSet::iterator rit = visited.begin();
                rit != visited.end(); rit++ )
          {
            //Compute the relation.
              DECLARE(BinaryRelation, Rr);
              DECLARE(BinaryRelation, Rtmpr);
#ifdef USE_BUDDY
            merge/*<St>*/(Rtmpr,R,*rit,Ir);
            compose/*<St>*/(Rr,Rtmpr,close);
#else
            merge<State>(Rtmpr,R,*rit,Ir);
            compose<State>(Rr,Rtmpr,close);
#endif
            //Make a key for this state and the call predecessor state.
            State rr = makeKey(Rr);
            State rc = makeKey(*rit);
            //Add the state to the deterministic NWA.
            addState(rr);
            //Add the transition to the deterministic NWA.
            addReturnTrans(r,rc,*it,rr);

            //Adjust the client info for this state.
            ClientInfoRefPtr rrCI;
            mergeClientInfoReturn(nondet,R,*rit,Rr,r,rc,*it,rr,rrCI);
            states.setClientInfo(rr,rrCI);

            //Determine whether to add this state to the worklist.
            if( visited.find(Rr) == visited.end() )
            {
              wl.insert(Rr);
            }
          }
          //For each possible exit point:
          for(  RelationSet::iterator rit = visited.begin();
                rit != visited.end(); rit++ )
          {
            //Compute the relation.
              DECLARE(BinaryRelation, Rr);
              DECLARE(BinaryRelation, Rtmpr);
#ifdef USE_BUDDY
            merge/*<St>*/(Rtmpr,*rit,R,Ir);
#else
            merge<State>(Rtmpr,*rit,R,Ir);
#endif
#ifdef USE_BUDDY
            compose/*<St>*/(Rr,Rtmpr,close);
#else
            compose<State>(Rr,Rtmpr,close);
#endif
            //Make a key for this state and the exit point state.
            State rr = makeKey(Rr);
            State re = makeKey(*rit);
            //Add the state to the deterministic NWA.
            addState(rr);
            //Add the transition to the deterministic NWA.
            addReturnTrans(re,r,*it,rr);
            
            //Adjust the client info for this state.
            ClientInfoRefPtr rrCI;
            mergeClientInfo(nondet,Rr,rr,rrCI);
            states.setClientInfo(rr,rrCI);

            //Determine whether to add this state to the worklist.
            if( visited.find(Rr) == visited.end() )
            {
              wl.insert(Rr);
            }
          }
        }

        //Deterministic final states.
        //Necessary components for a final state, i.e., 
        //any final state must contain one of the pairs in 
        //{(init,fin) | init is an initial state and fin is a final state}
        DECLARE(BinaryRelation, Rf);
        for( stateIterator iit = nondet->beginInitialStates();
             iit != nondet->endInitialStates(); iit++ )
        {
          for( stateIterator fit = nondet->beginFinalStates();
               fit != nondet->endFinalStates(); fit++ )
          {
            Rf.insert(std::pair<State,State>(*iit,*fit));
          }
        }
        //For each state in the deterministic maching, check whether it is a final state.
        for(  RelationSet::iterator sit = visited.begin();
              sit != visited.end(); sit++ )
        {
            DECLARE(BinaryRelation, Rtmpf);
          relations::intersect(Rtmpf,Rf,*sit);
          if(! Rtmpf.empty() )
          {
            State f = makeKey(*sit);
            addFinalState(f);
          }
        }
      }
#undef DECLARE
    }

#ifdef USE_BUDDY
    
    State NWA::makeKey(
       relations::RelationTypedefs<State>::BinaryRelation const & R ) const
    {
      std::stringstream ss;
      ss << R.getBdd();
      return getKey(ss.str());
    }
#else
    /**
     *
     * @brief returns the state corresponding to the given binary relation
     *
     * @param - R: the binary relation whose state to access
     * @return the state corresponding to the given binary relation
     *
     */
    
     State NWA::makeKey(
       relations::RelationTypedefs<State>::BinaryRelation const & R ) const
    {
      // C++ really needs C++0x's 'auto' keyword :-)
      typedef  relations::RelationTypedefs<State>::BinaryRelation::const_iterator Iterator;

      std::stringstream ss;
      for( Iterator mit = R.begin(); mit != R.end(); mit++ )
      {
#define key2str(a) a
        ss << "<" << key2str(mit->first) << "," << key2str(mit->second) << ">"; // 
#undef key2str
      }

      //std::cerr << "makeKey -> " << ss.str().size() << " characters\n";

      return getKey(ss.str());
    }
#endif

    /**
     *
     * @brief tests whether this NWA is deterministic 
     *
     * @return true if this NWA is deterministic, false otherwise
     *
     */
    
    bool NWA::isDeterministic( )
    {
      //An NWA is not deterministic if there is not exactly one initial state
      if( sizeInitialStates() != 1 ) 
        return false;
      
      //An NWA is not deterministic if there are epsilon transitions. 
      for( InternalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
      {
        if( Trans::getInternalSym(*iit) == WALI_EPSILON )
          return false;
      }
      
      //TODO: optimize this, it can be very expensive
      //An NWA is not deterministic if there are multiple transitions of some
      //type that are identical except for the to state.
      //For each state/symbol pair check that <= 1 transition leaves the state with that symbol.
      for( symbolIterator it = symbols.beginSymbols(); it != symbols.endSymbols(); it++ )
      {
        for( stateIterator sit = beginStates(); sit != endStates(); sit++ )
        {
          //Check call transitions.
          int count = 0;
          bool wild = false;
          for( CallIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
          {
            //Wild symbol 
            if( Trans::getCallSym(*cit) == WALI_WILD )
              wild = true;  

            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (Trans::getCallSite(*cit) == (*sit)) && (Trans::getCallSym(*cit) == *it) )
              count++;
          }
          if( count > 1 )
            return false;
          else if( wild && count > 0 )
            return false;

          //Check internal transitions.
          count = 0;
          wild = false;
          for( InternalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
          {
            //Wild symbol 
            if( Trans::getInternalSym(*iit) == WALI_WILD )
              wild = true;  

            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (Trans::getSource(*iit) == *sit) && (Trans::getInternalSym(*iit) == *it) )
              count++;
          }
          if( count > 1 )
            return false;
          else if( wild && count > 0 )
            return false;  

          for( stateIterator pit = beginStates(); pit != endStates(); pit++ )
          {
            //Check return transitions.
            count = 0;
            wild = false;
            for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
            {
              //Wild symbol 
              if( Trans::getReturnSym(*rit) == WALI_WILD )
                wild = true; 

              //Keep a count of multiple transitions with the same from
              //state and symbol(that is not epsilon).
              else if( (Trans::getExit(*rit) == *sit) 
                       && (Trans::getCallSite(*rit) == *pit) 
                       && (Trans::getReturnSym(*rit) == *it) )
                count++;
            }
            if( count > 1 )
              return false;
            else if( wild && count > 0 )
              return false;  
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
    
    void NWA::intersectClientInfoCall( NWA const & first, State call1, State entry1, 
                                               NWA const & second, State call2, State entry2, 
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
    
    void NWA::intersectClientInfoInternal( NWA const & first, State src1, State tgt1, 
                                                   NWA const & second, State src2, State tgt2, 
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
    
    void NWA::intersectClientInfoReturn( NWA const & first, State exit1, State call1, State ret1,
                                                 NWA const & second, State exit2, State call2, State ret2,
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
    
    bool NWA::stateIntersect( NWA const & first, State state1, NWA const & second, State state2, 
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
    
    bool NWA::transitionIntersect( NWA const & first, Symbol sym1, NWA const & second, Symbol sym2, Symbol & resSym )
    {
      (void) first, (void) second;
      
      //Note: When overriding this method your metric must determine whether the
      //      given symbols are compatible and set result to the appropriate symbol.

      //Epsilons are treated separately in the algorithms, so epsilons match nothing.
      if( sym1 == WALI_EPSILON || sym2 == WALI_EPSILON )
        return false;
      //Wild symbols match everything.
      if( sym1 == WALI_WILD )
      {
        resSym = sym2;
        return true;
      }
      if( sym2 == WALI_WILD )
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
    
    void NWA::mergeClientInfo( NWARefPtr first, 
                   relations::RelationTypedefs<State>::BinaryRelation const & binRel, 
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
    
    void NWA::mergeClientInfoCall( NWARefPtr nwa, 
                   relations::RelationTypedefs<State>::BinaryRelation const & binRelCall, 
                   relations::RelationTypedefs<State>::BinaryRelation const & binRelEntry,
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
    
    void NWA::mergeClientInfoInternal( NWARefPtr nwa, 
                   relations::RelationTypedefs<State>::BinaryRelation const & binRelSource, 
                   relations::RelationTypedefs<State>::BinaryRelation const & binRelTarget,
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
    
    void NWA::mergeClientInfoReturn( NWARefPtr nwa, 
                   relations::RelationTypedefs<State>::BinaryRelation const & binRelExit,
                   relations::RelationTypedefs<State>::BinaryRelation const & binRelCall, 
                   relations::RelationTypedefs<State>::BinaryRelation const & binRelReturn,
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
     * @brief constructs the WPDS which is the result of the explicit NWA plus WPDS 
     *        construction from Advanced Querying for Property Checking
     *
     * Note: For now we are assuming that implicit transitions to the stuck state need to be 
     *       realized for this construction to work.  Therefore, this method can only be called
     *       on an NWA of type 1 and afterwards it is an NWA of type 2.
     *
     * @param - base: the WPDS that forms the basis for the constructed WPDS
     * @return the WPDS which can be used to perform property checking using PDS 
     *          reachability
     * 
     */
    
    wpds::WPDS NWA::plusWPDS( const wpds::WPDS & base )
    {
      //Q: do implicit transitions to the stuck state affect this result?
      //A: it changes the states that are reachable

      realizeImplicitTrans();

      wpds::WPDS result;

      wpds::WpdsRules rules = wpds::WpdsRules();

      base.for_each(rules);

      //Step Rules:
      for(  std::set<wpds::Rule>::iterator it = rules.stepRules.begin();
            it != rules.stepRules.end(); it++ )
      {
        //<p,n_1> -w-> <p',n_2> in delta_1, (q,n_1,q') in delta_i
        Key sym = it->from_stack();
        //look for internal transitions in the NWA with symbol 'sym'
        for( InternalIterator iit = beginInternalTrans();
              iit != endInternalTrans(); iit++ )
        {
          if( sym == Trans::getInternalSym(*iit) )
          {
            //<(p,q),n_1> -w-> <(p',q'),n_2>
            Key from = getKey(it->from_state(),Trans::getSource(*iit));
            Key to = getKey(it->to_state(),Trans::getTarget(*iit));
            result.add_rule(from,sym,to,it->to_stack1(),it->weight());
          }
        }
      }

      //Call Rules:
      for(  std::set<wpds::Rule>::iterator it = rules.pushRules.begin();
            it != rules.pushRules.end(); it++ )
      {
        //<p,n_c> -w-> <p',e r_c> in delta_3, (q_c,n_c,q) in delta_c
        Key sym = it->from_stack();
        //look for call transitions in the NWA with symbol 'sym'
        for( CallIterator cit = beginCallTrans();
              cit != endCallTrans(); cit++ )
        {
          if( sym == Trans::getCallSym(*cit) )
          {
            //<(p,q_c),n_c> -w-> <(p',q),e (r_c,q_c)>
            Key from = getKey(it->from_state(),Trans::getCallSite(*cit));
            Key to = getKey(it->to_state(),Trans::getEntry(*cit));
            Key stack2 = getKey(it->to_stack2(),Trans::getCallSite(*cit));
            result.add_rule(from,sym,to,it->to_stack1(),stack2,it->weight());
          }
        }
      }

      //Return Rules:
      for( std::set<wpds::Rule>::iterator it = rules.popRules.begin();
            it != rules.popRules.end(); it++ )
      {
        //<p,x> -w-> <p',*> in delta_0, (q_r,q_c,x,q) in delta_r
        Key sym = it->from_stack();
        //look for return transitions in the NWA with symbol 'sym'
        for( ReturnIterator rit = beginReturnTrans();
              rit != endReturnTrans(); rit++ )
        {
          if( sym == Trans::getReturnSym(*rit) )
          {
            //<(p,q_r),x> -w-> <(p',q_r^x),*>
            Key from = getKey(it->from_state(),Trans::getExit(*rit));
            Key to = getKey(it->to_state(),getKey(Trans::getExit(*rit),sym));
            result.add_rule(from,sym,to,it->weight());
            //For all r_c in delta_2
            for( std::set<wpds::Rule>::iterator dit = rules.pushRules.begin();
                  dit != rules.pushRules.end(); dit++ )
            {
              //<(p',q_r^x),(r_c,q_c)> -1-> <(p',q),r_c>
              from = getKey(it->to_state(),getKey(Trans::getExit(*rit),sym));
              sym = getKey(dit->to_stack2(),Trans::getCallSite(*rit));
              to = getKey(it->to_state(),Trans::getReturnSite(*rit));
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
     * This method constructs the NWA that is equivalent to the given PDS.
     *
     * @param - pds: the pds to convert 
     * @return the NWA equivalent to the given PDS
     *
     */
    
    void NWA::PDStoNWA( const wpds::WPDS & pds )
    {
      //TODO: check this!

      //Make sure we can modify transitions in this machine.
      assert(stuck);

	  //Clear all states(except the stuck state) and transitions from this machine.
      State stuckSt = getStuckState();
      ClientInfoRefPtr stuckStInfo = getClientInfo( stuckSt );
      clear();
      setStuckState(stuckSt);
      setClientInfo(stuckSt, stuckStInfo ); // set the client info associated with the stuck state


      std::map<State,State> call_return;

      wpds::WpdsRules rules = wpds::WpdsRules();
      pds.for_each(rules);

      //Step Rules:
      for( std::set<wpds::Rule>::iterator it = rules.stepRules.begin();
            it != rules.stepRules.end(); it++ )
      {
        //<p,n_1> -w-> <p',n_2> in delta_1 goes to ((p,n_1),n_1,(p',n_2)) in delta_i
        addInternalTrans(getKey(it->from_state(),it->from_stack()), //from
                         it->from_stack(),                          //sym
                         getKey(it->to_state(),it->to_stack1()));   //to
      }

      //Call Rules:
      for( std::set<wpds::Rule>::iterator it = rules.pushRules.begin();
            it != rules.pushRules.end(); it++ )
      {
        //<p,n_c> -w-> <p',e r_c> in delta_2, ((p,n_c),c,(p',e)) in delta_c
        addCallTrans(getKey(it->from_state(),it->from_stack()),   //from
                      it->from_stack(),                           //sym
                      getKey(it->to_state(),it->to_stack1()));    //to
        //add (p,n_c) => r_c to the call-return map
        call_return.insert(std::pair<State,State>(getKey(it->from_state(),it->from_stack()),  //call
                                            it->to_stack2()));                          //ret
      }

      //Return Rules:
      for( std::set<wpds::Rule>::iterator it = rules.popRules.begin();
            it != rules.popRules.end(); it++ )
      {
        //<p,x> -w-> <p',*> in delta_0, ((p,x),(p,n_c),x,(p',r)) in delta_r
        for( std::map<State,State>::iterator cr_it = call_return.begin();
              cr_it != call_return.end(); cr_it++ )
        {
          addReturnTrans(getKey(it->from_state(),it->from_stack()),   //from
                          cr_it->first,                               //pred
                          it->from_stack(),                           //sym
                          getKey(it->to_state(),cr_it->second));      //to
        }
      }
    }

    /**
     *
     * @brief constructs the PDS equivalent to this NWA
     *
     * Note: This version keeps returns on the stack.
     *
     * @param - wg: the functions to use in generating weights
     * @return the PDS equivalent to this NWA
     *
     */ 
    
    wpds::WPDS NWA::NWAtoPDSreturns( WeightGen & wg ) const
    {
      //TODO: beware the implicit transitions
      //Q: do we need to make all transitions explicit in order to make this correct?

      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  //= wali::getKey("program");

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( InternalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
      {      
        //(q,sigma,q') in delta_i goes to <p,q> -w-> <p,q'> in delta_1
        //where the weight w depends on sigma

	    State src = Trans::getSource(*iit);
		  State tgt = Trans::getTarget(*iit);
      
      if( Trans::getInternalSym(*iit) == WALI_WILD )
		    wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
      else
        wgt = wg.getWeight(src, getClientInfo(src),
                            Trans::getInternalSym(*iit),
                            WeightGen::INTRA,
                            tgt, getClientInfo(tgt));  //w

        result.add_rule(program,                       //from_state (p)
                        src,									         //from_stack (q)
                        program,                       //to_state (p)
                        tgt,									         //to_stack1 (q')
                        wgt);                          //weight  (w)      
      }

      //Call Transitions 
      for( CallIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
      {
        State src = Trans::getCallSite(*cit);
		    State tgt = Trans::getEntry(*cit);
        for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
        {
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            //(q_c,sigma,q_e) in delta_c and (q_x,q_c,*,q_r) in delta_r goes to
            // <p,q_c> -w-> <p,q_e q_r> in delta_2 and w depends on sigma

            if( Trans::getCallSym(*cit) == WALI_WILD )
              wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); // w
            else
              wgt = wg.getWeight(src, getClientInfo(src),
                                  Trans::getCallSym(*cit),
                                  WeightGen::CALL_TO_ENTRY,  
                                  tgt, getClientInfo(tgt)); // w   

            result.add_rule(program,                      //from_state (p)
                            src,     //from_stack (q_c)
                            program,                      //to_state (p)
                            tgt,        //to_stack1 (q_e)
                            Trans::getReturnSite(*rit),   //to_stack2 (q_r)
                            wgt);                         //weight  (w)    
          }
        }
      }

      //Return Transitions
      for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
      {
        //(q_x,q_c,*,q_r) in delta_r goes to
        // <p,q_x> -w-> <p_q_xcr,epsilon> in delta_0
        // and <p_q_xcr,q_r> -1-> <p,q_r> in delta_1
        // where p_q_xcr = (p,q_x,q_c,q_r) and w depends on sigma

        State src = Trans::getExit(*rit);
        State tgt = Trans::getReturnSite(*rit);
        
        if( Trans::getReturnSym(*rit) == WALI_WILD )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w  
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getReturnSym(*rit),
                              WeightGen::EXIT_TO_RET,
                              tgt, getClientInfo(tgt));  // w            

        Key rstate = getControlLocation(src,Trans::getCallSite(*rit),tgt);    //p_q_xcr

        result.add_rule(program,                    //from_state (p)
                        src,                        //from_stack (q_x)
                        rstate,                     //to_state (p_q_xcr == (p,q_x,q_c,q_c))
                        wgt);                       //weight  (w)

        wgt = wg.getOne();                          // 1

        result.add_rule(rstate,                     //from_state (p_q_xcr == (p,q_x,q_c,q_r))
                        tgt,                        //from_stack (q_r)
                        program,                    //to_state (p)
                        tgt,                        //to_stack (q_r)
                        wgt);                       //weight  (1)   
      }  

      return result;
    }

    /**
     *
     * @brief constructs the backwards PDS equivalent to this NWA
     *
     * Note: This version keeps returns on the stack.
     *
     * @param - wg: the functions to use in generating weights
     * @return the backwards PDS equivalent to this NWA
     *
     */
    
    wpds::WPDS NWA::NWAtoBackwardsPDSreturns( WeightGen & wg ) const
    {
      //TODO: beware the implicit transitions
      //Q: do we need to make all transitions explicit in order to make this correct?

      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  //= wali::getKey("program");

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( InternalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
      {      
        //(q,sigma,q') in delta_i goes to <p,q'> -w-> <p,q> in delta_1
        //where the weight w depends on sigma

        State src = Trans::getSource(*iit);
        State tgt = Trans::getTarget(*iit);
        
        if( Trans::getInternalSym(*iit) == WALI_WILD )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getInternalSym(*iit),
                              WeightGen::INTRA,
                              tgt, getClientInfo(tgt));          // w

        result.add_rule(program,                                //from_state (p)
                        tgt,                                    //from_stack (q')
                        program,                                //to_state (p)
                        src,                                    //to_stack1 (q)
                        wgt);                                   //weight (w)      
      }

      //Call Transitions 
      for( CallIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
      {
        State src = Trans::getCallSite(*cit);
        State tgt = Trans::getEntry(*cit);
        for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            //(q_c,sigma,q_e) in delta_c and (q_x,q_c,*,q_r) in delta_r goes to
            // <p,q_e> -w-> <p_q_erc,epsilon> in delta_0
            // and <p_q_erc,q_c> -1-> <p,q_c> in delta_1
            // where p_q_erc = (p,q_e,q_r,q_c) and w depends on sigma

            if( Trans::getCallSym(*cit) == WALI_WILD )
              wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); // w
            else
              wgt = wg.getWeight(src, getClientInfo(src), 
                                  Trans::getCallSym(*cit),
                                  WeightGen::CALL_TO_ENTRY,  
                                  tgt, getClientInfo(tgt)); // w    

            Key cstate = getControlLocation(tgt,Trans::getReturnSite(*rit),src);    //p_q_erc

            result.add_rule(program,                    //from_state (p)
                            Trans::getEntry(*cit),      //from_stack (q_e)
                            cstate,                     //to_state (p_q_erc == (p,q_e,q_r,q_c))
                            wgt);                       //weight (w)

            wgt = wg.getOne();                          // 1
            
            result.add_rule(cstate,                     //from_state (p_q_erc == (p,q_e,q_r,q_c))
                            src,                        //from_stack (q_c)
                            program,                    //to_state (p)
                            src,                        //to_stack (q_c)
                            wgt);                       //weight (1)    
          }  
      }

      //Return Transitions
      for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
      {
        //(q_x,q_c,*,q_r) in delta_r goes to
        // <p,q_r> -w-> <p,q_x q_c> in delta_2 
        // and w depends on sigma

        State src = Trans::getExit(*rit);
        State tgt = Trans::getReturnSite(*rit);
        
        if( Trans::getReturnSym(*rit) == WALI_WILD )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getReturnSym(*rit),
                              WeightGen::EXIT_TO_RET,
                              tgt, getClientInfo(tgt));  // w

        result.add_rule(program,                      //from_state (p)
                        tgt,                          //from_stack (q_r)
                        program,                      //to_state (p)
                        src,                          //to_stack1 (q_x)
                        Trans::getCallSite(*rit),     //to_stack2 (q_c)
                        wgt);                         //weight (w)      
      }

      return result;
    }

    /**
     *
     * @brief constructs the PDS equivalent to this NWA
     *
     * Note: This version keeps calls on the stack.
     *
     * @param - wg: the functions to use in generating weights
     * @return the PDS equivalent to this NWA
     *
     */ 
    
    wpds::WPDS NWA::NWAtoPDScalls( WeightGen & wg ) const
    {
      //TODO: beware the implicit transitions
      //Q: do we need to make all transitions explicit in order to make this correct?

      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  // = wali::getKey("program");

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( InternalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
      {  
        // (q,sigma,q') in delta_i goes to <p,q> -w-> <p,q'> in delta_1
        // where the weight w depends on sigma

        State src = Trans::getSource(*iit);
        State tgt = Trans::getTarget(*iit);

        if( Trans::getInternalSym(*iit) == WALI_WILD )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getInternalSym(*iit),
                              WeightGen::INTRA,
                              tgt, getClientInfo(tgt));           // w

          result.add_rule(program,                                //from_state (p)
                          src,                                    //from_stack (q)
                          program,                                //to_state (p)
                          tgt,                                    //to_stack1 (q')
                          wgt);                                   //weight (w)      
      }

      //Call Transitions
      for( CallIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
      {           
        // (q_c,sigma,q_e) in delta_c goes to
        // <p,q_c> -w-> <p,q_e q_c> in delta_2 
        // and the weight w depends on sigma

        State src = Trans::getCallSite(*cit);
        State tgt = Trans::getEntry(*cit);

        if( Trans::getCallSym(*cit) == WALI_WILD )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); // w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getCallSym(*cit),
                              WeightGen::CALL_TO_ENTRY,
                              tgt, getClientInfo(tgt));          // w

        result.add_rule(program,                                //from_state (p)
                        src,                                    //from_stack (q_c)
                        program,                                //to_state (p)
                        Trans::getEntry(*cit),                  //to_stack1 (q_e)
                        src,                                    //to_stack2 (q_c)
                        wgt);                                   //weight (w)  
      } 

      //Return Transitions
      int r_count = 0;
      for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
      {
        ++r_count;
          
        //std::cerr << "Return transition #" << r_count << " of " << trans.size() << "\n";
        // (q_x,q_c,sigma,q_r) in delta_r goes to 
        // <p,q_x> -w-> <p_q_xcr,epsilon> in delta_0
        // and <p_q_xcr,q_c> -1-> <p,q_r> in delta_1
        // where p_q_xcr = (p,q_x,q_c,q_r), and w depends on sigma

        State src = Trans::getExit(*rit);
        State tgt = Trans::getReturnSite(*rit);

        if( Trans::getReturnSym(*rit) == WALI_WILD )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w 
        else
          wgt = wg.getWeight(src, getClientInfo(src), 
                              Trans::getReturnSym(*rit),
                              WeightGen::EXIT_TO_RET,  
                              tgt, getClientInfo(tgt));    // w     

        //Note: if you change this, make sure you modify the code in NWPForest.createCA()
        Key rstate = getControlLocation(src,Trans::getCallSite(*rit),tgt);  //p_q_xcr

        result.add_rule(program,                              //from_state (p)
                        src,                                  //from_stack (q_x)
                        rstate,                               //to_state (p_q_xcr == (p,q_x,q_c,q_r))
                        wgt);                                 //weight (w)

        
        wgt = wg.getOne();                                    // 1                      
         
        result.add_rule(rstate,                               //from_state (p_q_xcr == (p,q_x,q_c,q_r))
                        Trans::getCallSite(*rit),             //from_stack (q_c)
                        program,                              //to_state (p)
                        tgt,                                  //to_stack (q_r)
                        wgt);                                 //weight (1)
      }

      return result;
    }

    /**
     *
     * @brief constructs the backwards PDS equivalent to this NWA
     *
     * Note: This version keeps calls on the stack.
     *
     * @param - wg: the functions to use in generating weights
     * @return the backwards PDS equivalent to this NWA
     *
     */
    
    wpds::WPDS NWA::NWAtoBackwardsPDScalls( WeightGen & wg ) const
    {
      //TODO: beware the implicit transitions
      //Q: do we need to make all transitions explicit in order to make this correct?

      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  // = wali::getKey("program"); 

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( InternalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
      {  
        // (q,sigma,q') in delta_i goes to <p,q'> -w-> <p,q> in delta_1
        // where the weight w depends on sigma

        State src = Trans::getSource(*iit);
        State tgt = Trans::getTarget(*iit);
        
        if( Trans::getInternalSym(*iit) == WALI_WILD )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getInternalSym(*iit),
                              WeightGen::INTRA,
                              tgt, getClientInfo(tgt));         // w

        result.add_rule(program,                                //from_state (p)
                        tgt,                                    //from_stack (q')
                        program,                                //to_state (p)
                        src,                                    //to_stack1 (q)
                        wgt);                                   //weight (w)     
      }

      //Call Transitions
      for( CallIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
      {    
        State src = Trans::getCallSite(*cit);
        State tgt = Trans::getEntry(*cit);
        for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            // (q_c,sigma,q_e) in delta_c and (q_x,q_c,*,q_r) in delta_r goes to
            // <p,q_e> -w-> <p_q_erc,epsilon> in delta_0
            // and <p_q_erc,q_r> -1-> <p,q_c> in delta_1
            // where p_q_erc = (p,q_e,q_r,q_c) and w depends on sigma
            
            if( Trans::getCallSym(*cit) == WALI_WILD )
              wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); // w  
            else
              wgt = wg.getWeight(src, getClientInfo(src),
                                  Trans::getCallSym(*cit),
                                  WeightGen::CALL_TO_ENTRY,
                                  tgt, getClientInfo(tgt));         // w                  

            Key cstate = getControlLocation(tgt,Trans::getReturnSite(*rit),src);  //p_q_erc

            result.add_rule(program,                              //from_state (p)
                            tgt,                                  //from_stack (q_e)
                            cstate,                               //to_state (p_q_erc == (p,q_e,q_r,q_c))
                            wgt);                                 //weight (w)  

            wgt = wg.getOne();                                    // 1

            result.add_rule(cstate,                               //from_state (p_q_erc == (p,q_e,q_r,q_c))
                            Trans::getReturnSite(*rit),           //from_stack (q_r)
                            program,                              //to_state (p)
                            src,                                  //to_stack (q_c)
                            wgt);                                 //weight (1)    
          }
      } 

      //Return Transitions
      for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
      {
        // (q_x,q_c,sigma,q_r) in delta_r goes to 
        // <p,q_r> -w-> <p,q_x q_r> in delta_2 
        // and the weight w depends on sigma

        State src = Trans::getExit(*rit);
        State tgt = Trans::getReturnSite(*rit);

        if( Trans::getReturnSym(*rit) == WALI_WILD )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); //w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getReturnSym(*rit),
                              WeightGen::EXIT_TO_RET,  
                              tgt, getClientInfo(tgt));      //w
         
        result.add_rule(program,                                //from_state (p)
                        tgt,                                    //from_stack (q_r)
                        program,                                //to_state (p)
                        src,                                    //to_stack1 (q_x)
                        tgt,                                    //to_stack2 (q_r)
                        wgt);                                   //weight (w)  
      }

      return result;
    }
      
    /**
     *
     * @brief tests whether the language accepted by this NWA is empty
     *
     * @return true if the language accepted by this NWA is empty
     *
     */
    
    bool NWA::isEmpty( ) const
    {
      //An automaton with no initial states must accept only the empty language.
      if( sizeInitialStates() == 0 )
        return true;

      //An automaton with no final states must accept only the empty language.
      if( sizeFinalStates() == 0 )
        return true;

      //An automaton with an initial state that is a final state is definitely not empty.
      for( stateIterator it = beginInitialStates(); it != endInitialStates(); it++ )
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
      wali::Key init = getProgramControlLocation();
      initials.addState(init,wgts.zero());
      initials.setInitialState(init);
      //Set up final state.
      wali::Key fin = getKey("fin");
      initials.addState(fin,wgts.zero());
      initials.addFinalState(fin);
      //Add transitions.
      for( stateIterator it = beginInitialStates(); it != endInitialStates(); it++ )
      {
        initials.addTrans(init,*it,fin,wgts.one());
      }

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
      for( stateIterator it = beginFinalStates(); it != endFinalStates(); it++ )
      {
        finals.addTrans(init,*it,fin,wgts.one());
      }

#if 1
      ReachGen reach;
      //Perform poststar to determine reachability.
      //std::cerr << "Before poststar\n";
      wali::wfa::WFA postInitials = poststar(initials,reach);
      //std::cerr << "After poststar!\n";

      //intersect poststar(initials) and finals
      //std::cerr << "Before intersect\n";
      wali::wfa::WFA reachable = postInitials.intersect(finals);
      //std::cerr << "After intersect!\n";
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
    
    wfa::WFA NWA::prestar( wfa::WFA & input, WeightGen & wg ) const
    {
      //Q: does anything need to be done to transform the resulting WFA from the 
      //    PDS vocab back to the NWA vocab?
      return ((NWAtoPDScalls(wg)).prestar(input));
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
    
    void NWA::prestar( wfa::WFA & input, wfa::WFA & output, WeightGen & wg ) const
    {
      //Q: does anything need to be done to transform the resulting WFA from the 
      //    PDS vocab back to the NWA vocab?
      (NWAtoPDScalls(wg)).prestar(input,output);
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
    
    wfa::WFA NWA::poststar( wfa::WFA & input, WeightGen & wg ) const
    {
      //Q: does anything need to be done to transform the resulting WFA from the 
      //    PDS vocab back to the NWA vocab?
      return ((NWAtoPDScalls(wg)).poststar(input));
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
    
    void NWA::poststar( wfa::WFA & input, wfa::WFA & output, WeightGen & wg ) const
    {
      //Q: does anything need to be done to transform the resulting WFA from the 
      //    PDS vocab back to the NWA vocab?
      (NWAtoPDScalls(wg)).poststar(input,output);
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
    
    std::ostream & NWA::print( std::ostream & o ) const
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
    
    std::ostream & NWA::print_dot( std::ostream & o, std::string title, bool abbrev ) const
    {
      //TODO: flag for explicit stuck state transition printing?
      o << "digraph \"NWA\" { \n";
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

      //initial state
       StateSet initials = getInitialStates();
      for(  StateSet::const_iterator it = initials.begin(); it != initials.end(); it++ )
      {
        printKey(o << "\"",*it,abbrev)<<"\" [ style=bold ]";
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
      for(stateIterator exitit = beginStates(); exitit != endStates(); ++exitit)
      {
        for(stateIterator returnit = beginStates(); returnit != endStates(); ++returnit)
        {
          // For each Sym, holds the set of call predecessors for which
          // (exit, pred, sym, return) is in delta_r
           std::map<Symbol, StateSet > returns;

          // Populate it:
          for(ReturnIterator trans = beginReturnTrans(); trans != endReturnTrans(); ++trans)
          {
            if(*exitit == Trans::getExit(*trans)
               && *returnit == Trans::getReturnSite(*trans))
            {
              returns[Trans::getReturnSym(*trans)].insert(Trans::getCallSite(*trans));
            }
          }

          // Now add an edge for each return symbol
          for( std::map<Symbol, StateSet >::const_iterator trans = returns.begin();
              trans != returns.end(); ++trans)
          {
            std::stringstream ss;
            printKey(ss, trans->first,abbrev);
            ss << ";{";
            for( StateSet::const_iterator prediter = trans->second.begin();
                prediter != trans->second.end(); ++prediter)
            {
              printKey(ss, *prediter, abbrev) << ";";
            }
            ss << ";";

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
    
    bool NWA::operator==( const NWA & other ) const
    {
      return ( (states == other.states) 
              && (symbols == other.symbols) 
              && (trans == other.trans) );
    }

    //TODO: add methods like ...
    //virtual void for_each(ConstRuleFunctor &func) const;
    //virtual void for_each(RuleFunctor &func) const;
    //virtual void operator()(wfa::ITrans *t);

    /**
     *
     * @brief add all the states in the given StateSet to the NWA
     *
     * @param - addStateSet: the StateSet that contains the states to add
     *
     */
    
    
    void NWA::addAllStates( StateStorage addStateSet )
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
    
    
     NWA::stateIterator NWA::beginStates( ) const
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
    
    
     NWA::stateIterator NWA::endStates( ) const
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
    
    
    void NWA::addAllInitialStates( StateStorage addStateSet )
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
    
    
     NWA::stateIterator NWA::beginInitialStates( ) const
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
    
    
     NWA::stateIterator NWA::endInitialStates( ) const
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
    
    
    void NWA::addAllFinalStates( StateStorage addStateSet )
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
    
    
     NWA::stateIterator NWA::beginFinalStates( ) const
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
    
    
     NWA::stateIterator NWA::endFinalStates( ) const
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
    
    
    void NWA::addAllSymbols( SymbolStorage addSymbolSet )
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
    
    
     NWA::symbolIterator NWA::beginSymbols( ) const
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
    
    
     NWA::symbolIterator NWA::endSymbols( ) const
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
    
    
     NWA::CallIterator NWA::beginCallTrans( ) const
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
    
    
     NWA::CallIterator NWA::endCallTrans( ) const
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
    
    
     NWA::InternalIterator NWA::beginInternalTrans( ) const
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
    
    
     NWA::InternalIterator NWA::endInternalTrans( ) const
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
    
    
     NWA::ReturnIterator NWA::beginReturnTrans( ) const
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
    
    
     NWA::ReturnIterator NWA::endReturnTrans( ) const
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
    
    void NWA::epsilonClosure( StateSet * newPairs, State st ) const
    {
      //compute the states reachable from st via epsilon transitions
      Internals reachable = trans.getInternals(st,WALI_EPSILON);
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
    
    void NWA::epsilonClosure( std::set<StatePair> * newPairs, StatePair sp, NWA const & first, NWA const & second ) const
    {
      //Compute the cross produce of all states reachable from sp via epsilon transitions.

      //Explore epsilon transitions reachable from the first component of sp.
      Internals reachable = first.trans.getInternals(sp.first, WALI_EPSILON);
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
      reachable = second.trans.getInternals(sp.second, WALI_EPSILON);
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

    
    std::ostream& NWA::marshall( std::ostream& os ) const
    {
      os << "<" << XMLTag << ">\n";

      for(symbolIterator sym = beginSymbols(); sym != endSymbols(); ++sym)
      {
          os << "    <" << SymbolStorage::XMLSymbolTag() << " " << SymbolStorage::XMLNameAttr() << "=\"" << key2str(*sym) << "\"/>\n";
      }

      for(stateIterator state = beginStates(); state != endStates(); ++state)
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


    void
    NWA::combineWith(NWARefPtr rhs)
    {
      this->states.addAll(rhs->states);
      this->addAllSymbols(rhs->symbols);
      this->trans.addAllTrans(rhs->trans);
    }

    bool
    NWA::isMemberNondet( ::wali::nwa::NestedWord const & word ) const
    {
      typedef ::wali::nwa::details::Configuration Configuration;
      
      std::set<Configuration> nextConfigs;
      for(stateIterator iter = beginInitialStates(); iter!=endInitialStates(); ++iter) {
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
                if( Trans::getCallSite(*rit) == config->callPredecessors.back() ) {
                  // Construct a new configuration that's the same as the old
                  // configuration, except with a popped stack and new state
                  Configuration c(*config);
                  c.callPredecessors.pop_back();
                  c.state = Trans::getReturnSite(*rit);
                  nextConfigs.insert(c);
                }
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
          if (config->callPredecessors.size() != 0) {
            std::cerr << "Alert! In SimulateWordNondet, we are ending in a final state with nonempty stack!\n";
            exit(20);
          }
          return true;
        }
      }
            
      return false;
    }

  }
}

