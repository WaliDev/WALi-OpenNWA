#ifndef wali_nwa_query_TRANSITIONS_HPP
#define wali_nwa_query_TRANSITIONS_HPP

#include "opennwa/NwaFwd.hpp"

namespace opennwa
{
  namespace query
  {
      
    //Transition Accessors

    /**
     * 
     * @brief finds a symbol which occurs in a transition with the given endpoints
     *
     * This method checks for a transition (of any kind) with the given endpoints. If 
     * such a transition exists true is returned and the symbol labeling that edge is
     * placed in the reference parameter 'sym'.
     *
     * @param - from: the source of the desired transition
     * @param - to: the target of the desired transition
     * @param - sym: the location in which to place any symbol labeling such a transition
     * @return true if some such transition is found, false otherwise
     *
     */
    extern bool getSymbol(Nwa const &, State from, State to, Symbol & sym );

    /**
     * 
     * @brief tests whether there exists a transition whose source is 'from', 
     *        whose symbol is 'sym', and whose target is 'to'
     *
     * This method determines whether there exists a transition (internal, call, or 
     * return) whose source is 'from', whose symbol is 'sym', and whose target is 'to'.
     *
     * @param - from: the source of the transition
     * @param - sym: the symbol that labels the transition
     * @param - to: the target of the transition
     * @return true if such a transition exists, false otherwise
     * 
     */
    extern bool findTrans(Nwa const &, State from, Symbol sym, State to );

    /**
     * 
     * @brief obtains all the symbols that label transitions from 'source' to 'target'
     *
     * This method returns all the symbols that label transitions from 
     * 'source' to 'target'
     *
     * @param - source: the source of the desired transitions
     * @param - target: the target of the desired transitions
     * @return the set of symbols that label transitions from 'source' to 'target'
     *
     */
    extern const  SymbolSet getSymbols(Nwa const &, State source, State target );

    /**
     * 
     * @brief obtains all the symbols that label transitions from 'source'
     *
     * This method returns all the symbols that label transitions from 'source'
     *
     * @param - source: the source of the desired transitions
     * @return the set of symbols that label transitions from 'source'
     *
     */
    extern const  SymbolSet getSymbolsFrom(Nwa const &, State source );

    /**
     * 
     * @brief obtains all the symbols that label transitions to 'target'
     *
     * This method returns all the symbols that label transitions to 'target'
     *
     * @param - target: the target of the desired transitions
     * @return the set of symbols that label transitions to 'target'
     *
     */
    extern const  SymbolSet getSymbolsTo(Nwa const &, State target );

    /**
     *  
     * @brief obtains all the states that are predecessors of the given state
     *
     * This method returns all the states that are predecessors of the given 
     * state.
     *
     * @param - state: the state whose predecessors to look up
     * @param - preds: the set of all states that are predecessors of the given state
     *
     */    
    extern void getPredecessors(Nwa const &, State state,  StateSet & preds );
    /**
     * 
     * @brief obtains all the states that are predecessors of the given state
     *
     * This method returns all the states that are predecessors of the given state.
     *
     * @param - state: the state whose predecessors to look up
     * @return the set of all states that are predecessors of the given state
     *
     */
    extern const  StateSet getPredecessors(Nwa const &, State state );

    /**
     * 
     * @brief obtains all the states that are predecessors of the given state with 
     *        respect to the given symbol
     *
     * This method returns all the states that are predecessors of the given state
     * such that the symbol that labels a transition from each predecessor to the
     * given state is the given symbol.
     *
     * @param - symbol: the symbol that should label a transition from each predecessor 
     *                  to the given state
     * @param - state: the state whose predecessors to look up
     * @param - preds: the set of all states that are predecessors of the given state
     *                  with respect to the given symbol
     *
     */
    extern void getPredecessors(Nwa const &, Symbol symbol, State state,  StateSet & preds );
    /**
     * 
     * @brief obtains all the states that are predecessors of the given state with
     *		respect to the given symbol
     *
     * This method returns all the states that are predecessors of the given state
     * such that the symbol that labels a transition from each predecessor to the
     * given state is the given symbol.
     *
     * @param - symbol: the symbol that should label a transition from each predecessor
     *					to the given state
     * @param - state: the state whose predecessors to look up
     * @return the set of all states that are predecessors of the given state with 
     *			respect to the given symbol
     *
     */
    extern const  StateSet getPredecessors(Nwa const &, Symbol symbol, State state );

    /**
     * 
     * @brief obtains all the states that are successors of the given state
     *
     * This method returns all the states that are successors of the given state.
     *
     * @param - state: the state whose successors to lookup
     * @param - succs: the set of all states that are successors of the given state
     *
     */
    extern void getSuccessors(Nwa const &, State state,  StateSet & succs );
    /**
     * 
     * @brief obtains all the states that are successors of the given state
     *
     * This method returns all the states that are successors of the given state.
     *
     * @param - state: the state whose successors to look up
     * @return the set of all states that are successors of the given state
     *
     */
    extern const  StateSet getSuccessors(Nwa const &, State state );

    /**
     * 
     * @brief obtains all the states that are successors of the given state with
     *        respect to the given symbol
     *
     * This method returns all the states that are successors of the given state
     * such that the symbol that labels a transition from the given state to each
     * successor is the given symbol.  
     *
     * @param - symbol: the symbol that should label a transition from the given 
     *                  state to each successor
     * @param - state: the state whose successors to look up
     * @param - succs: the set of all states that are successors of the given state
     *                  with respect to the given symbol
     *
     */
    extern void getSuccessors(Nwa const &, State state, Symbol symbol,  StateSet & succs );
    /**
     * 
     * @brief obtains all the states that are successors of the given state with
     *		respect to the given symbol
     *
     * This method returns all the states that are successors of the given state
     * such that the symbol that labels a transition from the given state to each
     * successor is the given symbol.
     *
     * @param - symbol: the symbol that should label a transition from the given
     *					state to each successor
     * @param - state: the state whose successors to look up
     * @return the set of all states that are successors of the given state
     *			with respect to the given symbol
     *
     */
    extern const  StateSet getSuccessors(Nwa const &, State state, Symbol symbol );

    /**
     * 
     * @brief obtains all the symbols that label return transitions whose 
     *		call-predecessor is 'call' and whose return site is 'ret'
     *
     * This method returns all the symbols that label return transitions
     * whose call-predecessor is 'call' and whose return site is 'ret'
     *
     * @param - call: the call-predecessor of the desired return transitions
     * @param - ret: the return site of the desired return transitions
     * @return the set of symbols that label return transitions whose
     *			call-predecessor is 'call' and whose return site is 'ret'
     *
     */
    extern const  SymbolSet getCallRetSymbols(Nwa const &, State call, State ret );

    /**
     * 
     * @brief obtains all the symbols that label return transitions whose
     *		call-predecessor is 'call'
     *
     * This method returns all the symbols that label return transitions
     * whose call-predecessor is 'call'
     *
     * @param - call: the call-predecessor of the desired return transitions
     * @return the set of symbols that label return transitions whose
     *			call-predecessor is 'call'
     *
     */
    extern const  SymbolSet getCallRetSymbolsFrom(Nwa const &, State call );

    /**
     * 
     * @brief obtains all the symbols that label return transitions whose
     *		return site is 'ret'
     *
     * This method returns all the symbols that label return transitions
     * whose return site is 'ret'
     *
     * @param - ret: the return site of the desired return transitions
     * @return the set of symbols that label return transitions whose
     *			return site is 'ret'
     *
     */
    extern const  SymbolSet getCallRetSymbolsTo(Nwa const &, State ret );

    /**
     *  
     * @brief obtains all the states that are call-predecessors of the given state
     *
     * This method returns all the states that are call-predecessors of the given 
     * state.
     *
     * @param - state: the state whose call-predecessors to look up
     * @param - preds: the set of all states that are call-predecessors of the given state
     *
     */ 
    extern void getCallPredecessors(Nwa const &, State state,  StateSet & c_preds );
    /**
     * 
     * @brief obtains all the states that are call-predecessors of the given state
     *
     * This method returns all the states that are call-predecessors of the given state.
     *
     * @param - state: the state whose call-predecessors to look up
     * @return the set of all states that are call-predecessors of the given state
     *
     */
    extern const  StateSet getCallPredecessors(Nwa const &, State state );
      
    /**
     * 
     * @brief obtains all the states that are call-predecessors of the given state with 
     *        respect to the given symbol
     *
     * This method returns all the states that are call-predecessors of the given state
     * such that the symbol that labels a return transition with each call-predecessor 
     * and the given state is the given symbol.
     *
     * @param - symbol: the symbol that should label a transition with each call-predecessor 
     *                  and the given state
     * @param - state: the state whose call-predecessors to look up
     * @param - preds: the set of all states that are call-predecessors of the given state
     *                  with respect to the given symbol
     *
     */
    extern void getCallPredecessors(Nwa const &, Symbol symbol, State state,  StateSet & c_preds );
    /**
     * 
     * @brief obtains all the states that are call-predecessors of the given state with
     *		respect to the given symbol
     *
     * This method returns all the states that are call-predecessors of the given state
     * such that the symbol that labels a return transition with each call-predecessor
     * and the given state is the given symbol.
     *
     * @param - symbol: the symbol that should label a transition with each call-predecessor
     *					and the given state
     * @param - state: the state whose call-predecessors to look up
     * @return the set of all states that are call-predecessors of the given state
     *			with respect to the given symbol
     *
     */
    extern const  StateSet getCallPredecessors(Nwa const &, Symbol symbol, State state );

    /**
     * 
     * @brief obtains all the states that are call-successors of the given state
     *
     * This method returns all the states that are call-successors of the given state.
     *
     * @param - state: the state whose call-successors to lookup
     * @param - succs: the set of all states that are call-successors of the given state
     *
     */
    extern void getCallSuccessors(Nwa const &, State state,  StateSet & c_succs );
    /**
     * 
     * @brief obtains all the states that are call-successors of the given state
     *
     * This method returns all the states that are call-successors of the given state.
     *
     * @param - state: the state whose call-successors to look up
     * @return the set of all states that are call-successors of the given state
     *
     */
    extern const  StateSet getCallSuccessors(Nwa const &, State state );

    /**
     * 
     * @brief obtains all the states that are call-successors of the given state with
     *        respect to the given symbol
     *
     * This method returns all the states that are call-successors of the given state
     * such that the symbol that labels a return transition with each call-successor 
     * and the given state is the given symbol.  
     *
     * @param - symbol: the symbol that should label a return transition with each call-
     *                  successor and the given state
     * @param - state: the state whose call-successors to look up
     * @param - succs: the set of all states that are call-successors of the given state
     *                  with respect to the given symbol
     *
     */
    extern void getCallSuccessors(Nwa const &, State state, Symbol symbol,  StateSet & c_succs );
    /**
     * 
     * @brief obtains all the states that are call-successors of the given state with 
     *		respect to the given symbol
     *
     * This method returns all the states that are call-successors of the given state
     * such that the symbol that labels a return transition with each call-successor
     * and the given state is the given symbol.
     *
     * @param - symbol: the symbol that should label a return transition with each call-
     *					successor and the given state
     * @param - state: the state whose call-successors to look up
     * @return the set of all states that are call-successors of the given state with
     *			respect to the given symbol
     *
     */
    extern const  StateSet getCallSuccessors(Nwa const &, State state, Symbol symbol );

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

