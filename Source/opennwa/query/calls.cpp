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

      
    //Call Transitions

    /** 
     * 
     * @brief returns the call site states associated with the given symbol
     *
     * @param - symbol: the symbol whose call site states to obtain
     * @return the set of call site states associated with the given symbol
     *
     */
    
    const std::set< State> getCallSites_Sym(Nwa const & nwa, Symbol symbol )
    {
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getCallSites(Nwa const & nwa, Symbol symbol, State entryPoint )
    {
      assert(entryPoint < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< State, Symbol> > getCallSites(Nwa const & nwa, State entryPoint )
    {
      assert(entryPoint < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
	
    const std::set< State> getCallSites(Nwa const & nwa)
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< Symbol> getCallSym(Nwa const & nwa)
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< Symbol> getCallSym(Nwa const & nwa, State callSite, State entryPoint )
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(entryPoint < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< Symbol> getCallSym_Call(Nwa const & nwa, State callSite )
    {
      assert(callSite < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< Symbol> getCallSym_Entry(Nwa const & nwa, State entryPoint )
    {
      assert(entryPoint < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getEntries_Sym(Nwa const & nwa, Symbol symbol )
    {
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getEntries(Nwa const & nwa, State callSite, Symbol symbol )
    {
      assert(callSite < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< Symbol, State> > getEntries(Nwa const & nwa, State callSite )
    {
      assert(callSite < wali::WALI_BAD_KEY);
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
	
    const std::set< State> getEntries(Nwa const & nwa)
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
      const Calls & call = trans.getCalls();
      StateSet entries;
      for( CallIterator it = call.begin(); it != call.end(); it++ )
      {
        entries.insert( Trans::getEntry(*it) );
      }
      return entries;
    }

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

