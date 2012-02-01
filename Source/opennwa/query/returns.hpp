#ifndef wali_nwa_query_RETURNS_HPP
#define wali_nwa_query_RETURNS_HPP

#include "opennwa/NwaFwd.hpp"

namespace opennwa
{
  namespace query
  {
    //Return Transitions

    /**
     * 
     * @brief returns the exit states associated with the given symbol
     *
     * This method provides access to the exit states associated with
     * the given symbol.
     *
     * @param - symbol: the symbol whose exit states to obtain
     * @return the set of exit states associated with the given symbol
     *
     */
    extern const  StateSet getExits_Sym(Nwa const & nwa, Symbol symbol );

    /**
     * 
     * @brief returns the exit states associated with the given call site,
     *        symbol, and return site
     *
     * This method provides access to the exit states associated with the 
     * given call site, symbol, and return site.
     *
     * @param - callSite: the call site state whose exit states to obtain
     * @param - symbol: the symbol whose exit states to obtain
     * @param - returnSite: the return site state whose exit states to obtain
     * @return the set of exit states associated with the given call site,
     *          symbol, and return site
     *
     */
    extern const  StateSet getExits(Nwa const & nwa, State callSite, Symbol symbol, State returnSite );

    /**
     * 
     * @brief returns the exit point/symbol pairs associated with the given 
     *        call site and return site
     *
     * This method provides access to the exit point/symbol pairs associated 
     * with the given call site and return site.
     *
     * @param - callSite: the call site state whose exit point/symbol pairs to obtain
     * @param - returnSite: the return site state whose exit point/symbol pairs to obtain
     * @return the set of exit point/symbol pairs associated with the given call site 
     *          and return site
     *
     */
    extern const  std::set<std::pair<State,Symbol> > getExits(Nwa const & nwa, State callSite, State returnSite );

    /**
     * 
     * @brief returns the exit points of all return transitions in the NWA
     *
     * This method provides access to the exit points of all return transitions
     * in the NWA.
     *
     * @return the set of exit points of all return transitions in the NWA
     *
     */
    extern const  StateSet getExits(Nwa const & nwa );

    /**
     * 
     * @brief returns the exit states associated with the given call site 
     *        and symbol
     * 
     * This method provides access to the exit states associated with the 
     * given call site and symbol.
     *
     * @param - callSite: the call site state whose exit states to obtain
     * @param - symbol: the symbol whose exit states to obtain
     * @return the set of exit states associated with the given call state
     *          and symbol
     *
     */
    extern const  StateSet getExits_Call(Nwa const & nwa, State callSite, Symbol symbol );

    /**
     * 
     * @brief returns the exit point/symbol pairs associated with the
     *        given call site
     *
     * This method provides access to the exit point/symbol pairs associated
     * with the given call site.
     *
     * @param - callSite: the call site state whose exit point/symbol pairs
     *                    to obtain
     * @return the set of exit point/symbol pairs associated with the given
     *          call site
     *
     */
    extern const  std::set<std::pair<State,Symbol> > getExits_Call(Nwa const & nwa, State callSite );

    /**
     * 
     * @brief returns the exit states associated with the given symbol and
     *        return site
     *
     * This method provides access to the exit states associated with the
     * given symbol and return site.
     *
     * @param - symbol: the symbol whose exit states to obtain
     * @param - returnSite: the return site state whose exit states to obtain
     * @return the set of exit states associated with the given symbol and
     *          return site
     *
     */
    extern const  StateSet getExits_Ret(Nwa const & nwa, Symbol symbol, State returnSite );

    /**
     * 
     * @brief returns the exit point/symbol pairs associated with the
     *        given return site
     *
     * This method provides access to the exit point/symbol pairs associated
     * with the given return site.
     *
     * @param - returnSite: the return site state whose exit point/symbol
     *                      pairs to obtain
     * @return the set of exit point/symbol pairs associated with the given
     *          return site
     *
     */
    extern const  std::set<std::pair<State,Symbol> > getExits_Ret(Nwa const & nwa, State returnSite );

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
    extern const  StateSet getCalls_Sym(Nwa const & nwa, Symbol symbol );

    /**
     * 
     * @brief returns the call site states associated with the given exit 
     *        point, symbol, and return site
     *
     * This method provides access to the call site states associated with 
     * the given exit point, symbol, and return site.
     *
     * @param - exitPoint: the exit point whose call site states to obtain
     * @param - symbol: the symbol whose call site states to obtain
     * @param - returnSite: the return site whose call site states to obtain
     * @return the set of call site states assocaited with the given exit
     *          point, symbol, and return site
     * 
     */
    extern const  StateSet getCalls(Nwa const & nwa, State exitPoint, Symbol symbol, State returnSite );

    /**
     * 
     * @brief returns the call site/symbol pairs associated with the given exit 
     *        point and return site states
     *
     * This method provides access to the call site/symbol pairs associated with
     * the given exit point and return site states.
     *
     * @param - exitPoint: the exit point whose call site/symbol pairs to obtain
     * @param - returnSite: the return site whose call site/symbol pairs to obtain
     * @return the set of call site/symbol pairs associated with the given exit 
     *          point and return site states
     *
     */
    extern const  std::set<std::pair<State,Symbol> > getCalls(Nwa const & nwa, State exitPoint, State returnSite );

    /**
     * 
     * @brief returns the call sites of all return transitions in the NWA
     *
     * This method provides access to the call sites of all return transitions
     * in the NWA.
     *
     * @return the set of call sites of all return transitions in the NWA
     *
     */
    extern const  StateSet getCalls(Nwa const & nwa );

    /**
     * 
     * @brief returns the call site states associated with the given exit point 
     *        and symbol
     * 
     * This method provides access to the call site states associated with the
     * given exit point and symbol.
     *
     * @param - exitPoint: the exit point whose call site states to obtain
     * @param - symbol: the symbol whose call site states to obtain
     * @return the set of all call sites associated with the given exit point
     *          and symbol
     *
     */
    extern const  StateSet getCalls_Exit(Nwa const & nwa, State exitPoint, Symbol symbol );

    /**
     * 
     * @brief returns the call site/symbol pairs associated with the given
     *        exit point
     *
     * This method provides access to the call site/symbol pairs associated
     * with the given exit point.
     *
     * @param - exitPoint: the exit point whose call site/symbol pairs to obtain
     * @return the set of call site/symbol pairs associated with the given
     *         exit point
     *
     */
    extern const  std::set<std::pair<State,Symbol> > getCalls_Exit(Nwa const & nwa, State exitPoint );

    /**
     * 
     * @brief returns the call site states associated with the given symbol
     *        and return site
     *
     * This method provides access to the call site states associated with
     * the given symbol and return site.
     *
     * @param - symbol: the symbol whose call site states to obtain
     * @param - returnSite: the return site whose call site states to obtain
     * @return the set of call site states associated with the given symbol
     *          and return site
     *
     */
    extern const  StateSet getCalls_Ret(Nwa const & nwa, Symbol symbol, State returnSite );

    /**
     * 
     * @brief returns the call site/symbol pairs associated with the given
     *        return site
     *
     * This method provides access to the call site/symbol pairs associated
     * with the given return site.
     *
     * @param - returnSite: the return site whose call site/symbol pairs to
     *                      obtain
     * @return the set of all call site/symbol pairs associated with the 
     *          given return site
     *
     */
    extern const  std::set<std::pair<State,Symbol> > getCalls_Ret(Nwa const & nwa, State returnSite );

    /**
     * 
     * @brief returns the symbols that label return transitions in the NWA
     *
     * This method provides access to the symbols that label return transitions
     * in the NWA.
     *
     * @return the set of symbols that label return transitions in the NWA
     *
     */
    extern const  SymbolSet getReturnSym(Nwa const & nwa );

    /**
     * 
     * @brief returns the symbols that label return transitions from 'exitPoint'
     *        to 'returnSite' with call-predecessor 'callSite' in the NWA
     *
     * This method provides access to the symbols that label return transitions
     * from 'exitPoint' to 'returnSite' with call-predecessor 'callSite' in the NWA.
     *
     * @param - exitPoint: the exit point of the desired transition
     * @param - callSite: the call site of the desired transition
     * @param - returnSite: the return site of the desired transition
     * @return the set of symbols that label return transitions from 'exitPoint'
     *          to 'returnSite' with call-predecessor 'callSite' in the NWA
     *
     */
    extern const  SymbolSet getReturnSym(Nwa const & nwa, State exitPoint, State callSite, State returnSite );

    /**
     * 
     * @brief returns the symbols that label return transitions from 'exitPoint'
     *        n the NWA
     *
     * This method provides access to the symbols that label return transitions
     * from 'exitPoint' in the NWA.
     *
     * @param - exitPoint: the exit point of the desired transition
     * @return the set of symbols that label return transitions from 'exitPoint'
     *          in the NWA
     *
     */
    extern const  SymbolSet getReturnSym_Exit(Nwa const & nwa, State exitPoint );

    /**
     * 
     * @brief returns the symbols that label return transitions 
     *        with call-predecessor 'callSite' in the NWA
     *
     * This method provides access to the symbols that label return transitions
     * with call-predecessor 'callSite' in the NWA.
     *
     * @param - callSite: the call site of the desired transition
     * @return the set of symbols that label return transitions 
     *          with call-predecessor 'callSite' in the NWA
     *
     */
    extern const  SymbolSet getReturnSym_Call(Nwa const & nwa, State callSite );

    /**
     * 
     * @brief returns the symbols that label return transitions 
     *        to 'returnSite' in the NWA
     *
     * This method provides access to the symbols that label return transitions
     * to 'returnSite' in the NWA.
     *
     * @param - returnSite: the return site of the desired transition
     * @return the set of symbols that label return transitions 
     *          to 'returnSite' in the NWA
     *
     */
    extern const  SymbolSet getReturnSym_Ret(Nwa const & nwa, State returnSite );

    /**
     * 
     * @brief returns the symbols that label return transitions from 'exitPoint'
     *        with call-predecessor 'callSite' in the NWA
     *
     * This method provides access to the symbols that label return transitions
     * from 'exitPoint' with call-predecessor 'callSite' in the NWA.
     *
     * @param - exitPoint: the exit point of the desired transition
     * @param - callSite: the call site of the desired transition
     * @return the set of symbols that label return transitions from 'exitPoint'
     *          with call-predecessor 'callSite' in the NWA
     *
     */
    extern const  SymbolSet getReturnSym_ExitCall(Nwa const & nwa, State exitPoint, State callSite );

    /**
     * 
     * @brief returns the symbols that label return transitions from 'exitPoint'
     *        to 'returnSite' in the NWA
     *
     * This method provides access to the symbols that label return transitions
     * from 'exitPoint' to 'returnSite' in the NWA.
     *
     * @param - exitPoint: the exit point of the desired transition
     * @param - returnSite: the return site of the desired transition
     * @return the set of symbols that label return transitions from 'exitPoint'
     *          to 'returnSite' in the NWA
     *
     */
    extern const  SymbolSet getReturnSym_ExitRet(Nwa const & nwa, State exitPoint, State returnSite );

    /**
     * 
     * @brief returns the symbols that label return transitions 
     *        to 'returnSite' with call-predecessor 'callSite' in the NWA
     *
     * This method provides access to the symbols that label return transitions
     * to 'returnSite' with call-predecessor 'callSite' in the NWA.
     *
     * @param - callSite: the call site of the desired transition
     * @param - returnSite: the return site of the desired transition
     * @return the set of symbols that label return transitions 
     *          to 'returnSite' with call-predecessor 'callSite' in the NWA
     *
     */
    extern const  SymbolSet getReturnSym_CallRet(Nwa const & nwa, State callSite, State returnSite );


    /**
     * 
     * @brief returns the return site states associated with the given symbol
     *
     * This method provides access to the return site states associated with
     * the given symbol.
     *
     * @param - symbol: the symbol whose reutrn site states to obtain
     * @return the set of return site states associated with the given symbol
     *
     */
    extern const  StateSet getReturns_Sym(Nwa const & nwa, Symbol symbol );

    /**
     * 
     * @brief returns the return site states associated with the given exit 
     *        point, call site, and symbol
     *
     * This method provides access to the return site states associated with
     * the given exit point, call site, and symbol.
     *
     * @param - exitPoint: the exit point whose return site states to obtain
     * @param - callSite: the call site whose return site states to obtain
     * @param - symbol: the symbol whose return site states to obtain
     * @return the set of return site states associated with the given exit
     *          point, call site, and symbol
     *
     */
    extern const  StateSet getReturns(Nwa const & nwa, State exitPoint, State callSite, Symbol symbol );

    /**
     * 
     * @brief returns the symbol/return site pairs associated with the given exit 
     *        point and call site
     *
     * This method provides access to the symbol/return site pairs associated with  
     * the given exit point and call site.
     *
     * @param - exit: the exit point whose symbol/return site pairs to obtain
     * @param - callSite: the call site whose symbol/return site pairs to obtain
     * @return the set of symbol/return site pairs associated with the given exit 
     *          point and call site
     *
     */
    extern const  std::set<std::pair<Symbol,State> > getReturns(Nwa const & nwa, State exitPoint, State callSite );

    /**
     * 
     * @brief returns the return sites of all return transitions in the NWA
     *
     * This method provides access to the return sites of all return transitions
     * in the NWA.
     *
     * @return the set of return sites of all return transitions in the NWA
     *
     */
    extern const  StateSet getReturns(Nwa const & nwa );

    /**
     * 
     * @brief returns the return site states associated with the given exit 
     *        point and symbol
     *
     * This method provides access to the return site states associated with
     * the given exit point and symbol.
     *
     * @param - exitPoint: the exit point whose return site states to obtain
     * @param - symbol: the symbol whose return site states to obtain
     * @return the set of return site states associated with the given exit
     *         point and symbol
     *
     */
    extern const  StateSet getReturns_Exit(Nwa const & nwa, State exitPoint, Symbol symbol );

    /**
     * 
     * @brief returns the symbol/return site pairs associated with the given
     *        exit point
     *
     * This method provides access to the symbol/return site pairs associated
     * with the given exit point.
     *
     * @param - exitPoint: the exit point whose symbol/return site pairs to obtain
     * @return the set of symbol/return site pairs associated with the given 
     *          exit point
     *
     */
    extern const  std::set<std::pair<Symbol,State> > getReturns_Exit(Nwa const & nwa, State exitPoint );

    /**
     * 
     * @brief returns the return site states associated with the given call 
     *        site and symbol
     *
     * This method provides access to the return site states associated with
     * the given call site and symbol.
     *
     * @param - callSite: the call site whose return site states to obtain
     * @param - symbol: the symbol whose return site states to obtain
     * @return the set of return site states associated with the given call
     *          site and symbol
     *
     */
    extern const  StateSet getReturns_Call(Nwa const & nwa, State callSite, Symbol symbol );

    /**
     * 
     * @brief returns the symbol/return site pairs associated with the given
     *        call site
     *
     * This method provides access to the symbol/return site pairs associated
     * with the given call site.
     *
     * @param - callSite: the call site whose symbol/return site pairs to obtain
     * @return the set of symbol/return site pairs associated with the given
     *          call site
     *
     */
    extern const  std::set<std::pair<Symbol,State> > getReturns_Call(Nwa const & nwa, State callSite );

    /**
     * 
     * @brief returns the return sites associated with the given call site
     *
     * This method provides access to the return sites associated with the given
     * call site.
     *
     * @param - callSite: the call site whose return sites to obtain
     * @return the set of return sites associated with the given call site
     *
     */
    extern const  StateSet getReturnSites(Nwa const & nwa, State callSite );

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

