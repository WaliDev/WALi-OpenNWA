#ifndef wali_nwa_query_CALLS_HPP
#define wali_nwa_query_CALLS_HPP

#include "opennwa/NwaFwd.hpp"

namespace opennwa
{
  namespace query
  {
    //Call Transitions

    /** 
     * 
     * @brief returns the call site states associated with the given symbol
     *
     * This method provides access to the call site states associated with
     * the given symbol.
     *
     * @param - symbol: the symbol whose call site states to obtain
     * @return the set of call site states associated with the given symbol
     *
     */
    extern const  StateSet getCallSites_Sym(Nwa const & nwa, Symbol symbol );

    /**
     * 
     * @brief returns the call site states associated with the given symbol
     *        and entry point
     *
     * This method provides access to the call site states associated with
     * the given symbol and entry point.
     *
     * @param - symbol: the symbol whose call site states to obtain
     * @param - entryPoint: the entry point state whose call site states to
     *                    obtain
     * @return the set of call site states associated with the given symbol
     *          and entry point
     *
     */
    extern const  StateSet getCallSites(Nwa const & nwa, Symbol symbol, State entryPoint );
      
    /**
     * 
     * @brief returns the call site/symbol pairs associated with the given 
     *        entry point
     *
     * This method provides access to the call site/symbol pairs associated 
     * with the given entry point state.
     *
     * @param - entryPoint: the entry point state whose call site/symbol 
     *                      pairs to obtain
     * @return the set of call site/symbol pairs associated with the given 
     *          entry point
     *
     */
    extern const  std::set<std::pair<State,Symbol> > getCallSites(Nwa const & nwa, State entryPoint );

    /**
     * 
     * @brief returns the call sites of all call transitions in the Nwa
     *
     * This method provides access to the call sites of all call transitions
     * in the NWA.
     *
     * @return the set of call sites of all call transitions in the NWA
     *
     */
    extern const  StateSet getCallSites(Nwa const & nwa );

    /**
     * 
     * @brief returns the symbols that label call transitions in the NWA
     *
     * This method provides access to the symbols that label call transitions
     * in the NWA.
     *
     * @return the set of symbols that label call transitions in the NWA
     *
     */
    extern const  SymbolSet getCallSym(Nwa const & nwa );

    /**
     * 
     * @brief returns the symbols that label the call transitions from 
     *        'callSite' to 'entryPoint' in the NWA
     *
     * This method provides access to the symbols that label the call 
     * transitions from 'callSite' to 'entryPoint' in the NWA.
     *
     * @param - callSite: the call site of the desired transitions
     * @param - entryPoint: the entry point of the desired transitions
     * @return the set of symbols that label the call transitions from
     *          'callSite' to 'entryPoint' in the NWA
     *
     */
    extern const  SymbolSet getCallSym(Nwa const & nwa, State callSite, State entryPoint );

    /**
     * 
     * @brief returns the symbols that label the call transitions from 
     *        'callSite' in the NWA
     *
     * This method provides access to the symbols that label the call 
     * transitions from 'callSite' in the NWA.
     *
     * @param - callSite: the call site of the desired transitions
     * @return the set of symbols that label the call transitions from
     *          'callSite' in the NWA
     *
     */
    extern const  SymbolSet getCallSym_Call(Nwa const & nwa, State callSite );

    /**
     * 
     * @brief returns the symbols that label the call transitions to
     *        'entryPoint' in the NWA
     *
     * This method provides access to the symbols that label the call 
     * transitions to 'entryPoint' in the NWA.
     *
     * @param - entryPoint: the entry point of the desired transitions
     * @return the set of symbols that label the call transitions to 
     *          'entryPoint' in the NWA
     *
     */
    extern const  SymbolSet getCallSym_Entry(Nwa const & nwa, State entryPoint );

    /**
     * 
     * @brief returns the entry point states associated with the given symbol
     *
     * This method provides access to the entry point states associated with 
     * the given symbol.
     *
     * @param - symbol: the symbol whose entry point states to obtain
     * @return the set of entry point states associated with the given symbol
     *
     */
    extern const  StateSet getEntries_Sym(Nwa const & nwa, Symbol symbol );
      
    /**
     * 
     * @brief returns the entry point states associated with the given call
     *        site and symbol
     *
     * This method provides access to the entry point states assoicated with  
     * the given call site and symbol.
     *
     * @param - callSite: the call site whose entry point states to obtain
     * @param - symbol: the symbol whose entry point states to obtain
     * @return the set of entry point states associated with the given call
     *          site and symbol
     *
     */
    extern const  StateSet getEntries(Nwa const & nwa, State callSite, Symbol symbol );

    /**
     * 
     * @brief returns the symbol/entry point pairs associated with the given call site
     *
     * This method provides access to the symbol/entry point pairs associated with the 
     * given call site.
     *
     * @param - callSite: the call site whose symbol/entry point pairs to obtain
     * @return the set of symbol/entry point pairs associated with the given call site
     *
     */
    extern const  std::set<std::pair<Symbol,State> > getEntries(Nwa const & nwa, State callSite );

    /**
     * 
     * @brief returns the entry points of all call transitions in the NWA
     *
     * This method provides access to the entry points of all call transitions
     * in the NWA.
     *
     * @return the set of entry points of all call transitions in the NWA
     *
     */
    extern const  StateSet getEntries(Nwa const & nwa );      

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

