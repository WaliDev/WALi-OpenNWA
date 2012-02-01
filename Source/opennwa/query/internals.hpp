#ifndef wali_nwa_query_INTERNALS_HPP
#define wali_nwa_query_INTERNALS_HPP

#include "opennwa/NwaFwd.hpp"


namespace opennwa
{
  namespace query
  {
      
    //Internal Transitions

    /**
     *
     * @brief returns the source states associated with the given symbol
     *
     * This method provides access to the source states associated with the 
     * given symbol.
     *
     * @param - symbol: the symbol whose source states to obtain
     * @return the set of source states associated with the given symbol
     *
     */
    extern const  StateSet getSources_Sym(Nwa const & nwa, Symbol symbol );

    /**
     * 
     * @brief returns the source states associated with the given symbol 
     *        and target state
     *
     * This method provides access to the source states associated with 
     * the given symbol and target state.
     *
     * @param - symbol: the symbol whose source states to obtain
     * @param - target: the target state whose source states to obtain
     * @return the set of source states assoicated with the given
     *          symbol and target state
     *
     */
    extern const  StateSet getSources(Nwa const & nwa, Symbol symbol, State target );

    /**
     * 
     * @brief returns the source/symbol pairs associated with the 
     *        given target state
     *
     * This method provides access to the source/symbol pairs associated
     * with the given target state.
     *
     * @param - target: the target state whose source/symbol pairs to 
     *                  obtain
     * @return the set of source/symbol pairs associated with the given
     *          target state
     *
     */
    extern const  std::set<std::pair<State,Symbol> > getSources(Nwa const & nwa, State target );

    /**
     * 
     * @brief returns the sources of all internal transitions in the NWA
     *
     * This method provides access to the sources of all internal transitions
     * in the NWA.
     *
     * @return the set of sources of all internal transitions in the NWA
     *
     */
    extern const  StateSet getSources(Nwa const & nwa );

    /**
     * 
     * @brief returns the symbols that label internal transitions in the NWA
     *
     * This method provides access to the symbols that label internal transitions
     * in the NWA.
     *
     * @return the set of symbols that label internal transitions in the NWA
     *
     */
    extern const  SymbolSet getInternalSym(Nwa const & nwa );

    /**
     * 
     * @brief returns the symbols that label internal transitions from 'source'
     *        to 'target' in the NWA
     *
     * This method provides access to the symbols that label internal transitions
     * from 'source' to 'target' in the NWA.
     *
     * @param - source: the source of the desired transitions
     * @param - target: the target of the desired transitions
     * @return the set of symbols that label internal transitions from 'source'
     *          to 'target' in the NWA
     *
     */
    extern const  SymbolSet getInternalSym(Nwa const & nwa, State source, State target );

    /**
     * 
     * @brief returns the symbols that label internal transitions from 'source'
     *        in the NWA
     *
     * This method provides access to the symbols that label internal transitions
     * from 'source' in the NWA.
     *
     * @param - source: the source of the desired transitions
     * @return the set of symbols that label internal transitions from 'source'
     *          in the NWA
     *
     */
    extern const  SymbolSet getInternalSym_Source(Nwa const & nwa, State source );

    /**
     * 
     * @brief returns the symbols that label internal transitions 
     *        to 'target' in the NWA
     *
     * This method provides access to the symbols that label internal 
     * transitions to 'target' in the NWA.
     *
     * @param - target: the target of the desired transitions
     * @return the set of symbols that label internal transitions 
     *          to 'target' in the NWA
     *
     */
    extern const  SymbolSet getInternalSym_Target(Nwa const & nwa, State target );

    /**
     * 
     * @brief returns the target states associated with the given symbol
     *
     * This method provides access to the target states associated with
     * the given symbol.
     *
     * @param - symbol: the symbol whose target states to obtain
     * @return the set of target states associated with the given symbol
     *
     */
    extern const  StateSet getTargets_Sym(Nwa const & nwa, Symbol symbol );

    /**
     * 
     * @brief returns the target states associated with the given source
     *        state and symbol
     *
     * This method provides access to the target states associated with
     * the given source state and symbol.
     *
     * @param - source: the source state whose target states to obtain
     * @param - symbol: the symbol whose target states to obtain
     * @return the set of target states associated with the given source
     *          state and symbol
     *
     */
    extern const  StateSet getTargets(Nwa const & nwa, State source, Symbol symbol );

    /**
     * 
     * @brief returns the symbol/target pairs associated with the given source
     *
     * This method provides access to the symbol/target pairs associated with the given
     * source.
     *
     * @param - source: the source whose symbol/target pairs to obtain
     * @return the set of symbol/target pairs associated with the given source
     *
     */
    extern const  std::set<std::pair<Symbol,State> > getTargets(Nwa const & nwa, State source );

    /**
     * 
     * @brief returns the targets of all internal transitions in the NWA
     *
     * This method provides access to the targets of all internal transitions
     * in the NWA.
     *
     * @return the set of targets of all internal transitions in the NWA
     *
     */
    extern const  StateSet getTargets(Nwa const & nwa);

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

