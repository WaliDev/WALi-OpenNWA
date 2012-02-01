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

    //Internal Transitions

    /**
     *
     * @brief returns the source states associated with the given symbol
     *
     * @param - symbol: the symbol whose source states to obtain
     * @return the set of source states associated with the given symbol
     *
     */
    
    const std::set< State> getSources_Sym(Nwa const & nwa, Symbol symbol )
    {
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getSources(Nwa const & nwa, Symbol symbol, State target )
    {
      assert(target < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< State, Symbol> > getSources(Nwa const & nwa, State target )
    {
      assert(target < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
	
    const std::set< State> getSources(Nwa const & nwa)
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< Symbol> getInternalSym(Nwa const & nwa)
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< Symbol> getInternalSym(Nwa const & nwa, State source, State target )
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< Symbol> getInternalSym_Source(Nwa const & nwa, State source )
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< Symbol> getInternalSym_Target(Nwa const & nwa, State target )
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< State> getTargets_Sym(Nwa const & nwa, Symbol symbol )
    {
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set< State> getTargets(Nwa const & nwa, State source, Symbol symbol )
    {
      assert(source < wali::WALI_BAD_KEY);
      assert(symbol < wali::WALI_BAD_KEY);

      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();

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
    
    const std::set<std::pair< Symbol, State> > getTargets(Nwa const & nwa, State source )
    {
      assert(source < wali::WALI_BAD_KEY);
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
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
    
    const std::set< State> getTargets(Nwa const & nwa)
    {
      details::TransitionStorage const & trans = nwa._private_get_transition_storage_();
      
      const Internals & tgt = trans.getInternals();
      StateSet targets;
      for( InternalIterator it = tgt.begin(); it != tgt.end(); it++ )
      {
        targets.insert( Trans::getTarget(*it) );
      }
      return targets;
    }


  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

