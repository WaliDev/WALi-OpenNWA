#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/transitions.hpp"

namespace wali
{
  namespace nwa
  {
    namespace query
    //  {
      
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
    
    
    bool NWA::getSymbol(NWA const & nwa, State from, State to, Symbol & sym )
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
    
    bool NWA::findTrans(NWA const & nwa, State from, Symbol sym, State to)
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
    
    const std::set< Symbol> NWA::getSymbols(NWA const & nwa, State source, State target )
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
    
    const std::set< Symbol> NWA::getSymbolsFrom(NWA const & nwa, State source )
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
    
    const std::set< Symbol> NWA::getSymbolsTo(NWA const & nwa, State target )
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
    
    void NWA::getPredecessors(NWA const & nwa, State state,  StateSet & preds )
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
	  
    const StateSet NWA::getPredecessors(NWA const & nwa, State state )
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
    
    void NWA::getPredecessors(NWA const & nwa, Symbol symbol, State state,  StateSet & preds )
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
	  
    const std::set< State> NWA::getPredecessors(NWA const & nwa, Symbol symbol, State state )
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
    
    void NWA::getSuccessors(NWA const & nwa, State state,  StateSet & succs )
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
	  
    const std::set< State> NWA::getSuccessors(NWA const & nwa, State state )
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
    
    void NWA::getSuccessors(NWA const & nwa, State state, Symbol symbol,  StateSet & succs )
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
    
    const std::set< State> NWA::getSuccessors(NWA const & nwa, State state, Symbol symbol )
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
    
    const std::set< Symbol> NWA::getCallRetSymbols(NWA const & nwa, State call, State ret )
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
    
    const std::set< Symbol> NWA::getCallRetSymbolsFrom(NWA const & nwa, State call )
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
    
    const std::set< Symbol> NWA::getCallRetSymbolsTo(NWA const & nwa, State ret )
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
    
    void NWA::getCallPredecessors(NWA const & nwa, State state,  StateSet & c_preds )
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
	
    const std::set< State> NWA::getCallPredecessors(NWA const & nwa, State state )
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
    
    void NWA::getCallPredecessors(NWA const & nwa, Symbol symbol, State state,  StateSet & c_preds )
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
	
    const std::set< State> NWA::getCallPredecessors(NWA const & nwa, Symbol symbol, State state )
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
    
    void NWA::getCallSuccessors(NWA const & nwa, State state,  StateSet & c_succs )
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
	
    const std::set< State> NWA::getCallSuccessors(NWA const & nwa, State state )
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
    
    void NWA::getCallSuccessors(NWA const & nwa, State state, Symbol symbol,  StateSet & c_succs )
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
	
    const std::set< State> NWA::getCallSuccessors(NWA const & nwa, State state, Symbol symbol )
    {
      StateSet c_succs;
      getCallSuccessors(state, symbol, c_succs);
      return c_succs;
    }


    }
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

