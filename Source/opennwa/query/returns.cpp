#include "opennwa/Nwa.hpp"
#include "opennwa/query/internals.hpp"

namespace opennwa
{
  namespace query
  {
    typedef details::TransitionStorage Trans;
    typedef  Trans::CallIterator CallIterator;
    typedef  Trans::InternalIterator InternalIterator;
    typedef  Trans::ReturnIterator ReturnIterator;
    typedef  Trans::Call Call;       
    typedef  Trans::Internal Internal;   
    typedef  Trans::Return Return;          
      
    typedef  Trans::Calls Calls;
    typedef  Trans::Internals Internals;
    typedef  Trans::Returns Returns;

      
    //Return Transitions

    /**
     * 
     * @brief returns the exit states associated with the given symbol
     *
     * @param - symbol: the symbol whose exit states to obtain
     * @return the set of exit states associated with the given symbol
     *
     */
    
    const std::set< State> getExits_Sym(Nwa const & nwa, Symbol symbol )
    {
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getExits(Nwa const & nwa, State callSite, Symbol symbol, State returnSite )
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< State, Symbol> > getExits(Nwa const & nwa, State callSite, State returnSite )
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
	
    const std::set< State> getExits(Nwa const & nwa)
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< State> getExits_Call(Nwa const & nwa, State callSite, Symbol symbol )
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< State, Symbol> > getExits_Call(Nwa const & nwa, State callSite )
    {
      assert(callSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getExits_Ret(Nwa const & nwa, Symbol symbol, State returnSite )
    {
      assert(returnSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< State, Symbol> > getExits_Ret(Nwa const & nwa, State returnSite )
    {
      assert(returnSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getCalls_Sym(Nwa const & nwa, Symbol symbol )
    {
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getCalls(Nwa const & nwa, State exitPoint, Symbol symbol, State returnSite )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< State, Symbol> > getCalls(Nwa const & nwa, State exitPoint, State returnSite )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
	
    const std::set< State> getCalls(Nwa const & nwa)
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< State> getCalls_Exit(Nwa const & nwa, State exitPoint, Symbol symbol )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< State, Symbol> > getCalls_Exit(Nwa const & nwa, State exitPoint )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const  std::set< State> getCalls_Ret(Nwa const & nwa, Symbol symbol, State returnSite )
    {
      assert(returnSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< State, Symbol> > getCalls_Ret(Nwa const & nwa, State returnSite )
    {
      assert(returnSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< Symbol> getReturnSym(Nwa const & nwa)
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< Symbol> getReturnSym(Nwa const & nwa, State exitPoint, State callSite, State returnSite  )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(callSite < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< Symbol> getReturnSym_Exit(Nwa const & nwa, State exitPoint )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< Symbol> getReturnSym_Call(Nwa const & nwa, State callSite )
    {
      assert(callSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< Symbol> getReturnSym_Ret(Nwa const & nwa, State returnSite )
    {
      assert(returnSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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

    const std::set< Symbol> getReturnSym_ExitCall(Nwa const & nwa, State exitPoint, State callSite  )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(callSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< Symbol> getReturnSym_ExitRet(Nwa const & nwa, State exitPoint, State returnSite  )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< Symbol> getReturnSym_CallRet(Nwa const & nwa, State callSite, State returnSite  )
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(returnSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getReturns_Sym(Nwa const & nwa, Symbol symbol )
    {
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
      
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
    
    const std::set< State> getReturns(Nwa const & nwa, State exitPoint, State callSite, Symbol symbol )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(callSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< Symbol, State> > getReturns(Nwa const & nwa, State exit, State callSite )
    {
      assert(exit < wali::WALI_BAD_KEY);
      assert(callSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
	
    const std::set< State> getReturns(Nwa const & nwa)
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< State> getReturns_Exit(Nwa const & nwa, State exitPoint, Symbol symbol )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< Symbol, State> > getReturns_Exit(Nwa const & nwa, State exitPoint )
    {
      assert(exitPoint < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getReturns_Call(Nwa const & nwa, State callSite, Symbol symbol )
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< Symbol, State> > getReturns_Call(Nwa const & nwa, State callSite )
    {
      assert(callSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    
    const std::set< State> getReturnSites(Nwa const & nwa, State callSite )
    {
      assert(callSite < wali::WALI_BAD_KEY);
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
      return trans.getReturnSites(callSite);
    }


  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

