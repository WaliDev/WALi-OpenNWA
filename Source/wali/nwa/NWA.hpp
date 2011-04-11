#ifndef wali_nwa_NWA_GUARD
#define wali_nwa_NWA_GUARD 1

#include "wali/nwa/NWAFwd.hpp"

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Countable.hpp"
#include "wali/KeyContainer.hpp"

#include "wali/nwa/details/SymbolStorage.hpp"
#include "wali/nwa/details/StateStorage.hpp"
#include "wali/nwa/details/TransitionStorage.hpp"

#include "wali/nws/NWS.hpp"

#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"

#include "wali/nwa/WeightGen.hpp"

//#define USE_BUDDY
#ifdef USE_BUDDY
#  include "wali/nwa/RelationOpsBuddy.hpp"
#else
#  include "wali/nwa/RelationOps.hpp"
#endif

// std::c++
#include <iostream>
#include <sstream>
#include <map>
#include <deque>

namespace wali
{
  namespace nwa
  {
    /**
     *
     * This class models nested word automata (NWA). 
     * Note: StName must be a unique identifier for states.
     *
     */
    class NWA : public Printable, public Countable
    {
      public:
        typedef ClientInfo Client;

        typedef details::StateStorage StateStorage;
      
        typedef  StateStorage::const_iterator stateIterator;
      
        typedef  StateStorage::ClientInfoRefPtr ClientInfoRefPtr;
        typedef details::SymbolStorage SymbolStorage;
        typedef  SymbolStorage::const_iterator symbolIterator;

        typedef details::TransitionStorage Trans;
        
        typedef  Trans::Call Call;       
        typedef  Trans::Internal Internal;   
        typedef  Trans::Return Return;          
      
        typedef  Trans::Calls Calls;
        typedef  Trans::Internals Internals;
        typedef  Trans::Returns Returns;

        typedef  Trans::CallIterator CallIterator;
        typedef  Trans::InternalIterator InternalIterator;
        typedef  Trans::ReturnIterator ReturnIterator;
        
        typedef std::set<State> StateSet;
        typedef std::pair<State,State> StatePair;

        static std::string const XMLTag;

      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      NWA( );
      NWA( State stuckSt );
      NWA( const NWA & other );
      NWA & operator=( const NWA & other );

      void clear( );

      /**
       * Marshalls the NWA as XML to the output stream os
       */
      virtual std::ostream& marshall( std::ostream& os ) const;


      //State Accessors

      /**
       * 
       * @brief access the client information associated with the given state
       *
       * This method provides access to the client information associated with the given 
       * state.
       *
       * @param - state: the state whose client information to retrieve
       * @return the client information associated with the given state
       *
       */
      ClientInfoRefPtr getClientInfo( State state ) const;

      /**
       * 
       * @brief set the client information associated with the given state
       *
       * This method sets the client information associated with the given state to the 
       * client information provided.
       *
       * @param - state: the  state whose client information to set
       * @param - c: the desired client information for the given state
       *
       */
      void setClientInfo( State state, const ClientInfoRefPtr c );  
     
      //All States

      /**
       *
       * @brief tests whether there is a stuck state set or not
       *
       * This method determines whether there is a stuck state set or not.
       *
       * @return true if there is a stuck state set, false otherwise
       *
       */
      inline bool hasStuckState() const
      {
        return stuck;
      }

      /**
       *
       * @brief returns the Key for the stuck state
       *
       * This method provides access to the Key for the stuck state.
       * Note: You can only call this method if you have set a stuck state.
       *
       * @return the Key for the stuck state
       *
       */
      State getStuckState( ) const
      {
        assert(stuck);
        return states.getStuckState();
      }

      /**
       *
       * @brief sets the Key for the stuck state
       *
       * This method provides access to the Key for the stuck state.
       * Note: You can only call this method if there is not stuck state set.
       * Note: The stuck state must not currently be a state of the NWA.
       *        (Unless we check for final state and outgoing transitions of the state.)
       *
       * @param - state: the Key for the stuck state
       *
       */
      void setStuckState( State state )
      {
        //TODO: ponder the following ...
        //Note: This method of handling the stuck state might be changing the meaning of an automaton.
        //      For example, suppose we complement a machine(so that the stuck state is "accepting")
        //      then add some transition which adds a state or symbol.  This implicitly adds 
        //      transitions to the stuck state, but these transitions will be to a non-accepting
        //      stuck state.

        //You can't set a stuck state when there already is one. (There can be only one.)
        assert(!stuck);
        //You can't use a state that is currently part of the NWA for the new stuck state.
        assert(!isState(state));
        
        stuck = true;
        addState(state);  //Add the state to the machine.
        states.setStuckState(state);
      }

      /**
       *  
       * @brief test whether the given state is the stuck state
       * 
       * This method determines whether the given state is the stuck state.
       *
       * @param - state: the state to test
       * @return true if this state is the stuck state, false otherwise
       *
       */
      inline bool isStuckState( State state ) const
      {
        if(stuck)
          return states.isStuckState(state);
        else
          return false;   //If there is no stuck state, then this state cannot be the stuck state.
      }

      /**
       *   
       * @brief provides access to all states in the NWA
       *
       * This method provides access to all states.
       *
       * @return a set of all states
       *
       */
      const  StateSet & getStates( ) const;

      /**
       * 
       * @brief test if a given state is a state of this NWA
       *
       * This method tests whether the given state is in the state set of this NWA.
       *
       * @param - state: the state to check
       * @return true if the given state is a state of this NWA
       *
       */
      bool isState( State state ) const; 

     /**
       *  
       * @brief add the given state to this NWA
       *
       * This method adds the given state to the state set for this NWA.  If the state 
       * already exists in the NWA, false is returned.  Otherwise, true is returned.
       * Note: This method modifies transitions, so it cannot be called on an NWA without
       *        a set stuck state.
       *
       * @param - state: the state to add
       * @return false if the state already exists in the NWA, true otherwise
       *
       */
      bool addState( State state );

      /**
       *
       * @brief returns the number of states associated with this NWA
       *
       * This method returns the number of states associated with this NWA. 
       * Note: This count does include the stuck state if there is one.
       *
       * @return the number of states associated with this NWA
       *
       */
      size_t sizeStates( ) const;

      int largestState() const {
        return states.largestState();
      }

      /**
       *  
       * @brief remove the given state from this NWA
       *
       * This method checks for the given state in the set of initial states, the set of 
       * final states, and the set of states in this NWA.  It then removes the state from 
       * any set that contained it.  Any transitions to or from the state to be removed 
       * are also removed.
       * Note: The stuck state cannot be removed in this manner, only by clearing all states
       *        or realizing all implicit transitions.
       * Note: This method modifies transitions, so it cannot be called on an NWA without
       *        a set stuck state.
       *
       * @param - state: the state to remove
       * @return false if the state does not exist in the NWA, true otherwise
       *
       */
      virtual bool removeState( State state );

      /**
       *
       * @brief remove all states from the NWA
       *
       * This method removes all states and transitions from the NWA.
       * Note: This makes the NWA one without a stuck state, so a new
       *      stuck state must be added before transitions can be added.
       *
       */
      void clearStates( );

      //Initial States

      /**
       *  
       * @brief obtain the states in the initial state set
       *
       * This method provides access to the states in the initial state set 
       * of this NWA.  Note: An NWA can have an unbounded number of inital states 
       * associated with it.
       *
       * @return set of inital states associated with the NWA
       *
       */
      const  StateSet & getInitialStates( ) const; 

      /**
       * 
       * @brief test if the given state is an initial state of this NWA
       *
       * This method tests whether the given state is in the initial state set of the NWA.
       *
       * @param - state: the state to check
       * @return true if the given state is an initial state, false otherwise
       *
       */
      bool isInitialState( State state ) const;

      /**
       * 
       * @brief make the given state an initial state in this NWA
       *
       * This method checks for the given state in the state set of this NWA.  If the 
       * state does not exist, it is added to the state set of the NWA.  The given state 
       * is then added to the set of initial states for the NWA.  If the state already 
       * exists in the intial state set of the NWA, false is returned.  Otherwise, true
       * is returned.
       *
       * @param - state: the state to add to initial state set
       * @return false if the state already exists in the initial state set of the NWA
       *
       */
      bool addInitialState( State state );

      /**
       *
       * @brief returns the number of initial states associated with this NWA
       *
       * This method returns the number of initial states associated with this NWA.
       *
       * @return the number of initial states associated with this NWA
       *
       */
      size_t sizeInitialStates( ) const;

      /**
       * 
       * @brief remove the given state from the initial state set of this NWA
       *
       * This method checks for the given state in the initial state set of this NWA.  If
       * the state exists, it is removed from the initial state set (but not from the set 
       * of all states of the NWA).  
       * Note: This method does not remove any transitions from the NWA.
       *
       * @param - state: the state to remove from the initial state set
       * @return false if the state does not exist in the initial state set of this NWA
       *
       */
      bool removeInitialState( State state );

      /**
       *
       * @brief remove all states from the initial state set of the NWA
       *
       * This method removes all states from the initial state set of the NWA, but does 
       * not remove the them from the set of all states of the NWA.  
       * Note: This method does not remove any transitions from the NWA.
       *
       */
      void clearInitialStates( );

      //Final States

      /**
       *
       * @brief obtain the final states in this NWA
       *
       * This method provides access to the states in the final state set of this NWA.  
       * Note: An NWA can have an unbounded number of final states associated with it.
       *
       * @return set of all final states associated with this NWA
       *
       */
      const  StateSet & getFinalStates( ) const;

      /**
       *
       * @brief test if a state with the given name is a final state
       *
       * This method tests whether the given state is in the final state set of the NWA.
       *
       * @param - state: the state to check
       * @return true if the given state is a final state
       *
       */
      bool isFinalState( State state ) const;

      /**
       * 
       * @brief make the given state a final state
       *
       * This method checks for the given state in the state set of the NWA.  If the state
       * does not exist, it is added to the state set of the NWA.  The given state is then
       * added to the set of final states for the NWA.  If the state already exists in the
       * final state set of the NWA, false is returned.  Otherwise, true is returned.
       * Note: If 'state' is the stuck state, then all implicit transitions are realized
       *        (removing the stuck state property from 'state') prior to making 'state' final.
       *
       * @param - state: the state to add to final state set
       * @return false if the state already exists in the final state set of the NWA
       *
       */
      bool addFinalState( State state );

      /**
       * 
       * @brief returns the number of final states associated with this NWA
       *
       * This method returns the number of final states associated with this NWA. 
       *
       * @return the number of final states associated with this NWA
       *
       */
      size_t sizeFinalStates( ) const;

      /**
       *
       * @brief remove the given state from the final state set of the NWA
       *
       * This method checks for the given state in the final state set of the NWA.  If the
       * state exists, it is removed from the final state set (but not from the set of all
       * states of the NWA). This method does not remove any transitions from the NWA.
       *
       * @param - state: the state to remove from the final state set
       * @return false if the state does not exist in the final state set of the NWA
       *
       */
      bool removeFinalState( State state );

      /**
       *
       * @brief remove all states from the final state set of the NWA
       *
       * This method removes all States from the final state set of the NWA, but does not 
       * remove the them from the set of all states of the NWA.  This method does not 
       * remove any transitions from the NWA.
       *
       */
      void clearFinalStates( );

      //Symbol Accessors

      /**
       *
       * @brief obtain all symbols in this NWA
       *
       * This method provides access to the symbols of this NWA.  
       * Note: An NWA can have an unbounded number of symbols associated with it.
       *
       * @return set of all symbols associated with this NWA
       *
       */
      const  std::set<Symbol> & getSymbols( ) const;

      /**
       *
       * @brief test if the given symbol is associated with the NWA
       *
       * This method tests whether the given symbol is in the symbol set associated with 
       * the NWA.
       *
       * @param - sym: the symbol to check
       * @return true if the given symbol is associated with the NWA
       *
       */
      bool isSymbol( Symbol sym ) const;

      /**
       *
       * @brief add the given symbol to the NWA
       *
       * This method adds the given symbol to the symbol set associated with the NWA. If 
       * the symbol is already associated with the NWA, false is returned. Otherwise, 
       * true is returned.
       * Note: This method modifies (implicit) transitions, so it cannot be called on an 
       *        NWA without a set stuck state.
       *
       * @param - sym: the symbol to add
       * @return false if the symbol is already associated with the NWA
       *
       */
      bool addSymbol( Symbol sym );

      /**
       *
       * @brief returns the number of symbols associated with this NWA
       *
       * This method returns the number of symbols associated with this NWA.  
       * Note: The wild symbol is included in this count if there are any transitions
       *        labled with the wild symbol in the NWA.
       *
       * @return the number of symbols associated with this NWA
       *
       */
      size_t sizeSymbols( ) const;

      /**
       *
       * @brief remove the given symbol from the NWA
       *
       * This method checks for the given symbol in the symbol set associated with the NWA
       * and removes the symbol from the symbol set if necessary.  Any transitions 
       * associated with the symbol to be removed are also removed.
       * Note: Symbols can be removed from NWAs without a stuck state because no implicit
       *        transitions are introduced by removing a symbol.
       *
       * @param - sym: the symbol to remove
       * @return false if the symbols is not associated with the NWA
       *
       */
      bool removeSymbol( Symbol sym );

      /**
       *
       * @brief remove all symbols associated with the NWA
       *
       * This method removes all symbols associated with the NWA.  It also removes all 
       * transitions associated with the NWA as there can be no transitions without at 
       * least one symbol.
       * Note: This method modifies (implicit) transitions, so it cannot be called on an 
       *        NWA without a set stuck state.
       *
       */
      void clearSymbols( );

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
      bool getSymbol( State from, State to, Symbol & sym ) const;

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
      bool findTrans( State from, Symbol sym, State to ) const;

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
      const  std::set<Symbol> getSymbols( State source, State target ) const;

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
      const  std::set<Symbol> getSymbolsFrom( State source ) const;

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
      const  std::set<Symbol> getSymbolsTo( State target ) const;

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
      void getPredecessors( State state,  StateSet & preds ) const;
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
      const  StateSet getPredecessors( State state ) const;

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
      void getPredecessors( Symbol symbol, State state,  StateSet & preds ) const;
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
      const  StateSet getPredecessors( Symbol symbol, State state ) const;

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
      void getSuccessors( State state,  StateSet & succs ) const;
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
      const  StateSet getSuccessors( State state ) const;

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
      void getSuccessors( State state, Symbol symbol,  StateSet & succs ) const;
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
      const  StateSet getSuccessors( State state, Symbol symbol ) const;

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
      const  std::set<Symbol> getCallRetSymbols( State call, State ret ) const;

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
      const  std::set<Symbol> getCallRetSymbolsFrom( State call ) const;

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
      const  std::set<Symbol> getCallRetSymbolsTo( State ret ) const;

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
      void getCallPredecessors( State state,  StateSet & c_preds ) const;
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
      const  StateSet getCallPredecessors( State state ) const;
      
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
      void getCallPredecessors( Symbol symbol, State state,  StateSet & c_preds ) const;
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
      const  StateSet getCallPredecessors( Symbol symbol, State state ) const;

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
      void getCallSuccessors( State state,  StateSet & c_succs ) const;
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
      const  StateSet getCallSuccessors( State state ) const;

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
      void getCallSuccessors( State state, Symbol symbol,  StateSet & c_succs ) const;
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
      const  StateSet getCallSuccessors( State state, Symbol symbol ) const;

      /**
       *    
       * @brief duplicates the original state, but only duplicates outgoing transitions
       *
       * This method assigns to the duplicate state all the state properties of the 
       * original state and duplicates all outgoing transitions of the original state for 
       * the duplicate state.  Further, any self-loop outgoing transitions that the 
       * original state are not only duplicated as self-loop transitions for the duplicate 
       * state, but the duplicate state also has transitions to the original state for 
       * each such transition. 
       * Note: This method modifies transitions, so it cannot be called on an NWA without
       *        a set stuck state.
       * Note: It doesn't make sense to duplicate the stuck state as there can only be 
       *        one stuck state set in a given machine.
       *
       * @param - orig: the name of the original state, i.e. the state to duplicate
       * @param - dup: the name of the duplicate state
       *
       */
      void duplicateStateOutgoing( State orig, State dup );

      /**
       * 
       * @brief duplicates the original state
       *
       * This method assigns to the duplicate state all the state properties of the 
       * original state and duplicates all transitions of the original state for the 
       * duplicate state.  Further, any self-loop transitions that the original state are 
       * not only duplicated as self-loop transitions for the duplicate state, but the 
       * duplicate state also has transitions to and from the original state for each 
       * such transition.
       * Note: This method modifies transitions, so it cannot be called on an NWA without
       *        a set stuck state.
       * Note: It doesn't make sense to duplicate the stuck state as there can only be
       *        one stuck state set in a given machine.
       *
       * @param - orig: the name of the original state, i.e. the state to duplicate
       * @param - dup: the name of the duplicate state
       *
       */
      void duplicateState( State orig, State dup );


      /**
       *  
       * @brief realizes all implicit transitions in the NWA
       *
       * This method makes explicit all transitions in the NWA by adding transitions to the stuck
       * state for each state/symbol(excluding epsilon) pair for which no outgoing edge(one of each 
       * kind-call,internal,return) from that state is labeled with that symbol.
       * Note: The NWA no longer has a stuck state after this operation is performed.
       *
       */
      void realizeImplicitTrans();

      /**
       *
       * @brief returns the number of transitions associated with this NWA
       *
       * This method returns the number of transitions associated with this NWA.  
       *
       * @return the number of transitions associated with this NWA
       *
       */
      size_t sizeTrans( ) const;

      /**
       *
       * @brief remove all transitions from the NWA
       *
       * This method removes all transitions from the NWA.  It does not remove any states
       * or symbols from the NWA.
       * Note: This method modifies (implicit) transitions, so it cannot be called on an 
       *        NWA with no stuck state.
       *
       */
      void clearTrans( );

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
      const  StateSet getCallSites_Sym( Symbol symbol ) const;

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
      const  StateSet getCallSites( Symbol symbol, State entryPoint ) const;
      
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
      const  std::set<std::pair<State,Symbol> > getCallSites( State entryPoint ) const;

	  /**
       * 
	   * @brief returns the call sites of all call transitions in the NWA
	   *
	   * This method provides access to the call sites of all call transitions
	   * in the NWA.
	   *
	   * @return the set of call sites of all call transitions in the NWA
	   *
       */
	  const  StateSet getCallSites( ) const;

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
      const  std::set<Symbol> getCallSym( ) const;

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
      const  std::set<Symbol> getCallSym( State callSite, State entryPoint ) const;

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
      const  std::set<Symbol> getCallSym_Call( State callSite ) const;

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
      const  std::set<Symbol> getCallSym_Entry( State entryPoint ) const;

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
      const  std::set<Key> getEntries_Sym( Symbol symbol ) const;
      
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
      const  StateSet getEntries( State callSite, Symbol symbol ) const;

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
      const  std::set<std::pair<Symbol,State> > getEntries( State callSite ) const;

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
	  const  StateSet getEntries( ) const;

      /**
       *
       * @brief add a call transition to the NWA
       *
       * This method creates a call transition with the given edge and label information 
       * and adds it to the transition set for the NWA.  If the call transition already 
       * exists in the NWA, false is returned. Otherwise, true is returned.
       * Note: 'sym' cannot be Epsilon
       * Note: 'from' cannot be the stuck state unless 'to' is as well
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the call transition already exists in the NWA
       *
       */
      bool addCallTrans( State from, Symbol sym, State to );

      /**
       *
       * @brief add a call transition to the NWA
       *
       * This method adds the given call transition to the transition set  for the NWA.
       * If the call transition already exists in the NWA, false is returned. Otherwise, 
       * true is returned.
       * Note: the symbol cannot be Epsilon
       * Note: the from state cannot be the stuck state unless the to state is as well
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - ct: the call transition to add
       * @return false if the call transition already exists in the NWA
       *
       */
      bool addCallTrans( Call & ct );

      /**
       *
       * @brief remove a call transition from the NWA
       *
       * This method checks for the call transition with the given edge and label 
       * information in the transition set. If the transition is found, it is removed
       * from the transition set and true is returned.  Otherwise, false is returned.
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the call transition does not exist in the NWA
       *
       */
      bool removeCallTrans( State from, Symbol sym, State to );

      /**
       *
       * @brief remove a call transition from the NWA
       *
       * This method checks for the call transition in the transition set. If the 
       * transition is found, it is removed from the transition set and true is 
       * returned.  Otherwise, false is returned.
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - ct: the call transition to remove
       * @return false if the call transition does not exist in the NWA
       *
       */
      bool removeCallTrans( const Call & ct );

      /**
       *
       * @brief returns the number of call transitions associated with this NWA
       *
       * This method returns the number of call transitions associated with this NWA.  
       *
       * @return the number of call transitions associated with this NWA
       *
       */
      size_t sizeCallTrans( ) const;

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
      const  StateSet getSources_Sym( Symbol symbol ) const;

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
      const  StateSet getSources( Symbol symbol, State target ) const;

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
      const  std::set<std::pair<State,Symbol> > getSources( State target ) const;

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
	  const  StateSet getSources( ) const;

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
    const  std::set<Symbol> getInternalSym( ) const;

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
      const  std::set<Symbol> getInternalSym( State source, State target ) const;

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
      const  std::set<Symbol> getInternalSym_Source( State source ) const;

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
      const  std::set<Symbol> getInternalSym_Target( State target ) const;

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
      const  StateSet getTargets_Sym( Symbol symbol ) const;

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
      const  StateSet getTargets( State source, Symbol symbol ) const;

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
      const  std::set<std::pair<Symbol,State> > getTargets( State source ) const;

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
	  const  StateSet getTargets( ) const;

      /**
       *
       * @brief add an internal transition to the NWA
       *
       * This method creates an internal transition with the given edge and label 
       * information and adds it to the transition set for the NWA.  If the internal 
       * transition already exists in the NWA, false is returned. Otherwise, true is 
       * returned.
       * Note: 'from' cannot be the stuck state unless 'to' is as well
       * Note: epsilon transitions to the stuck state make no sense
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the internal transition already exists in the NWA
       *
       */
      bool addInternalTrans( State from, Symbol sym, State to );

      /**
       *
       * @brief add an internal transition to the NWA
       *
       * This method adds the given internal transition to the transition set for the NWA.
       * If the internal transition already exists in the NWA, false is returned. Otherwise, 
       * true is returned.
       * Note: the from state cannot be the stuck state unless the to state is as well
       * Note: epsilon transitions to the stuck state make no sense
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - it: internal transition to add to the NWA
       * @return false if the internal transition already exists in the NWA
       *
       */
      bool addInternalTrans( Internal & it );

      /**
       *
       * @brief remove an internal transition from the NWA
       *
       * This method checks for the internal transition with the given edge and label
       * information in the transition set.  If the transition is found, it is removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the internal transition does not exist in the NWA
       *
       */
      bool removeInternalTrans( State from, Symbol sym, State to );

      /**
       *
       * @brief remove an internal transition from the NWA
       *
       * This method checks for the internal transition in the transition set. If the 
       * transition is found, it is removed from the transition set and true is returned.
       * Otherwise, false is returned.
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - it: the internal transition to remove
       * @return false if the internal transition does not exist in the NWA
       *
       */
      bool removeInternalTrans( const Internal & it );

      /**
       *
       * @brief returns the number of internal transitions associated with this NWA
       *
       * This method returns the number of internal transitions associated with this NWA.  
       *
       * @return the number of internal transitions associated with this NWA
       *
       */
      size_t sizeInternalTrans( ) const;

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
      const  StateSet getExits_Sym( Symbol symbol ) const;

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
      const  StateSet getExits( State callSite, Symbol symbol, State returnSite ) const;

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
      const  std::set<std::pair<State,Symbol> > getExits( State callSite, State returnSite ) const;

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
	  const  StateSet getExits( ) const;

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
      const  StateSet getExits_Call( State callSite, Symbol symbol ) const;

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
      const  std::set<std::pair<State,Symbol> > getExits_Call( State callSite ) const;

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
      const  StateSet getExits_Ret( Symbol symbol, State returnSite ) const;

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
      const  std::set<std::pair<State,Symbol> > getExits_Ret( State returnSite ) const;

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
      const  StateSet getCalls_Sym( Symbol symbol ) const;

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
      const  StateSet getCalls( State exitPoint, Symbol symbol, State returnSite ) const;

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
      const  std::set<std::pair<State,Symbol> > getCalls( State exitPoint, State returnSite ) const;

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
	  const  StateSet getCalls( ) const;

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
      const  StateSet getCalls_Exit( State exitPoint, Symbol symbol ) const;

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
      const  std::set<std::pair<State,Symbol> > getCalls_Exit( State exitPoint ) const;

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
      const  StateSet getCalls_Ret( Symbol symbol, State returnSite ) const;

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
      const  std::set<std::pair<State,Symbol> > getCalls_Ret( State returnSite ) const;

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
      const  std::set<Symbol> getReturnSym( ) const;

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
      const  std::set<Symbol> getReturnSym( State exitPoint, State callSite, State returnSite ) const;

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
      const  std::set<Symbol> getReturnSym_Exit( State exitPoint ) const;

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
      const  std::set<Symbol> getReturnSym_Call( State callSite ) const;

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
      const  std::set<Symbol> getReturnSym_Ret( State returnSite ) const;

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
      const  std::set<Symbol> getReturnSym_ExitCall( State exitPoint, State callSite ) const;

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
      const  std::set<Symbol> getReturnSym_ExitRet( State exitPoint, State returnSite ) const;

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
      const  std::set<Symbol> getReturnSym_CallRet( State callSite, State returnSite ) const;


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
      const  StateSet getReturns_Sym( Symbol symbol ) const;

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
      const  StateSet getReturns( State exitPoint, State callSite, Symbol symbol ) const;

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
      const  std::set<std::pair<Symbol,State> > getReturns( State exitPoint, State callSite ) const;

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
	  const  StateSet getReturns(  ) const;

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
      const  StateSet getReturns_Exit( State exitPoint, Symbol symbol ) const;

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
      const  std::set<std::pair<Symbol,State> > getReturns_Exit( State exitPoint ) const;

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
      const  StateSet getReturns_Call( State callSite, Symbol symbol ) const;

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
      const  std::set<std::pair<Symbol,State> > getReturns_Call( State callSite ) const;

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
      const  StateSet getReturnSites( State callSite ) const;

      /**
       *
       * @brief add a return transition to the NWA
       *
       * This method creates a return transition with the given edge and label information 
       * and adds it to the transition set for the NWA.  If the return transition already 
       * exists in the NWA, false is returned. Otherwise, true is returned.
       * Note: 'sym' cannot be Epsilon
       * Note: 'from' cannot be the stuck state unless 'to' is as well
       * Note: 'pred' cannot be the stuck state unless 'to' is as well
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - from: the state the edge departs from
       * @param - pred: the state from which the call was initiated  
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition already exists in the NWA
       *
       */
      bool addReturnTrans( State from, State pred, Symbol sym, State to );

      /**
       *
       * @brief add a return transition to the NWA
       *
       * This method adds the given return transition to the transition set for the NWA. 
       * If the return transition already exists in the NWA, false is returned. Otherwise,
       * true is returned.
       * Note: the symbol cannot be Epsilon
       * Note: the from state cannot be the stuck state unless the to state is as well
       * Note: the call predecessor state cannot be the stuck state unless the to state is as well
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - rt: return transition to add to the NWA
       * @return false if the return transition already exists in the NWA
       *
       */
      bool addReturnTrans( Return & rt );

      /**
       *
       * @brief remove a return transition from the NWA
       *
       * This method checks for all return transitions with the given edge and label
       * information in the transition set.  If transitions are found, they are removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - from: of the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturnTrans( State from, Symbol sym, State to );

      /**
       *
       * @brief remove a return transition from the NWA
       *
       * This method checks for the return transition with the given edge and label 
       * information in the transition set. If the transition is found, it is removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       * Note: Transitions cannot be modified if there is no stuck state.
       * 
       * @param - from: the state the edge departs from
       * @param - pred: the state from which the call was initiated  	   
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturnTrans( State from, State pred, Symbol sym, State to );

      /**
       *
       * @brief remove a return transition from the NWA
       *
       * This method checks for the return transition in the transition set. If the 
       * transition is found, it is removed from the transition set and true is returned.
       * Otherwise, false is returned.
       * Note: Transitions cannot be modified if there is no stuck state.
       *
       * @param - rt: the return transition to remove
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturnTrans( const Return & rt );

      /**
       *
       * @brief returns the number of return transitions associated with this NWA
       *
       * This method returns the number of return transitions associated with this NWA.  
       *
       * @return the number of return transitions associated with this NWA
       *
       */
      size_t sizeReturnTrans( ) const;

      //Building NWAs


      /***
       * @brief constructs an NWA which is the projection of the given NWA to the states
       * provided
      */
      void projectStates(NWA const & first, StateSet const & prjStates);

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      void projectStates(const NWARefPtr &first, const StateSet &prjStates) {
        projectStates(*first, prjStates);
      }
        

      /**
       *
       * @brief constructs the NWA resulting from the union of the given NWAs 
       *
       * This method constructs the union of the given NWAs by unioning the state sets,
       * symbols, transitions, initial state sets, and final state sets of the two NWAs.
       * Note: The resulting NWA is guaranteed NOT to be deterministic.
       * Note: This method modifies the transitions of the NWA, so it cannot be 
       *        called on a NWA with no stuck state.
       *
       * @param - first: the NWA to union with 'second'
       * @param - second: the NWA to union with 'first'
       *
       */
      void unionNWA( NWA const & first, NWA const & second );

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      void unionNWA( NWARefPtr first, NWARefPtr second ) {
        unionNWA(*first, *second);
      }
      
      /**
       *
       * @brief constructs the NWA resulting from the union of the given NWAs 
       *
       * This method constructs the union of the given NWAs by unioning the state sets,
       * symbols, transitions, initial state sets, and final state sets of the two NWAs.
       * Note: The resulting NWA is guaranteed NOT to be deterministic.
       *
       * @param - first: the NWA to union with 'second'
       * @param - second: the NWA to union with 'first'
       * @param - stuck: the stuck state of the NWA to be constructed
       * @return the NWA resulting from the union of the given NWAs
       *
       */
      static NWARefPtr unionNWA( NWA const & first, NWA const & second, State stuck )
      {
        NWARefPtr nwa(new NWA(stuck));
        nwa->unionNWA(first,second);
        return nwa;
      }

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      static NWARefPtr unionNWA( NWARefPtr first, NWARefPtr second, State stuck ) {
        return unionNWA(*first, *second, stuck);
      }
      
      //TODO: write comments
      virtual bool isTransitionPossible( const State &src, const Symbol &sym, const State &tgt);

      /**
       *
       * @brief constructs the NWA which is the intersection of the given NWAs
       *
       * This method constructs the NWA which accepts only nested words that are accepted 
       * by both 'first' and 'second'.  
       * Note: The resulting NWA is NOT guaranteed to be deterministic.	
       * Note: This method modifies the transitions of the NWA, so it cannot be 
       *        called on a NWA with no stuck state.
       *
       * @param - first: the NWA to intersect with 'second'
       * @param - second: the NWA to intersect with 'first'
       *	
       */
      void intersect( NWA const & first, NWA const & second );

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      void intersect( NWARefPtr first, NWARefPtr second ) {
        intersect(*first, *second);
      }

      
      /**
       *
       * @brief constructs the NWA which is the intersection of the given NWAs
       *
       * This method constructs the NWA which accepts only nested words that are accepted 
       * by both 'first' and 'second'.  
       * Note: The resulting NWA is NOT guaranteed to be deterministic.	
       *
       * @param - first: the NWA to intersect with 'second'
       * @param - second: the NWA to intersect with 'first'
       * @param - stuck: the stuck state of the NWA to be constructed
       * @return the NWA resulting from the intersection of the given NWAs
       *	
       */
      static NWARefPtr intersect( NWA const & first, NWA const & second, State stuck )
      {
        NWARefPtr nwa(new NWA(stuck));
        nwa->intersect(first,second);
        return nwa;
      }

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      static NWARefPtr intersect( NWARefPtr first, NWARefPtr second, State stuck ) {
        return intersect(*first, *second, stuck);
      }

      /**
       * @brief Detects (immediately) stuck states and removes them
       *
       * Detects stuck states and removes them, leaving the transitions implicit.
       * (It only figures this for states that have no outgoing transitions other
       * than itself; you can still have a SCC that acts as a stuck state.)
       */ 
      void removeImplicitTransitions();


      // pruning functions

      /**
      * Removes states not reachable from any of the 'sources' states
      * 
      */
      void pruneUnreachableForward(const StateSet & sources);

      /**
      * Removes states from which none of the 'targets' are reachable.
      */
      void pruneUnreachableBackward(const StateSet & targets);

      /**
      * Removes states not reachable from any initial state
      * 
      */
      void pruneUnreachableInitial();

      /**
      * Removes states from which none of the final states are reachable.
      */

      void pruneUnreachableFinal();

      /**
      * Removes states not reachable from any initial state
      * and from which none of the final states are reachable.
      */
      void chop();       

      /**
       *
       * @brief constructs the NWA resulting from the concatenation of the given NWAs
       *
       * This method constructs the concatenation of the given NWAs by adding epsilon 
       * transitions from all final states of 'first' to all initial states of 'second'.  
       * Note: The resulting NWA is NOT guaranteed to be deterministic.
       * Note: This method modifies the transitions of the NWA, so it cannot be 
       *        called on a NWA with no stuck state.
       *
       * @param - first: the NWA to which 'second' should be concatenated
       * @param - second: the NWA to concatenate onto the end of 'first'
       *
       */
      void concat( NWA const & first, NWA const & second );

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      void concat( NWARefPtr first, NWARefPtr second ) {
        concat(*first, *second);
      }
      /**
       *
       * @brief constructs the NWA resulting from the concatenation of the given NWAs
       *
       * This method constructs the concatenation of the given NWAs by adding epsilon 
       * transitions from all final states of 'first' to all initial states of 'second'.  
       * Note: The resulting NWA is NOT guaranteed to be deterministic.
       *
       * @param - first: the NWA to which 'second' should be concatenated
       * @param - second: the NWA to concatenate onto the end of 'first'
       * @param - stuck: the stuck state of the NWA to be constructed
       * @return the NWA resulting from the concatenation of the given NWAs
       *
       */
      static NWARefPtr concat( NWA const & first, NWA const & second, State stuck )
      {
        NWARefPtr nwa(new NWA(stuck));
        nwa->concat(first,second);
        return nwa;
      }

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      static NWARefPtr concat( NWARefPtr first, NWARefPtr second, State stuck )
      {
        return concat(*first, *second, stuck);
      }

      /**
       *
       * @brief constructs the NWA which is the reverse of the given NWA
       *
       * This method constructs the NWA which is the reverse of the given NWA. It reverses 
       * internal transitions and switches call and return transitions.
       * Note: the resulting NWA is NOT guaranteed to be deterministic.
       * Note: This method modifies the transitions of the NWA, so it cannot be 
       *        called on a NWA with no stuck state.
       *
       * @param - first: the NWA to reverse
       * 
       */
      void reverse( NWA const & first );

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      void reverse( NWARefPtr first )
      {
        reverse(*first);
      }
      /**
       *
       * @brief constructs the NWA which is the reverse of the given NWA
       *
       * This method constructs the NWA which is the reverse of the given NWA. It reverses 
       * internal transitions and switches call and return transitions.
       * Note: the resulting NWA is NOT guaranteed to be deterministic.
       *
       * @param - first: the NWA to reverse
       * @param - stuck: the stuck state of the NWA to be constructed
       * @return the NWA resulting from reversing the given NWA
       * 
       */
      static NWARefPtr reverse( NWA const & first, State stuck )
      {
        NWARefPtr nwa(new NWA(stuck));
        nwa->reverse(first);
        return nwa;
      }

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      static NWARefPtr reverse( NWARefPtr first, State stuck )
      {
        return reverse(*first, stuck);
      }

      /**
       *
       * @brief constructs the NWA resulting from performing Kleene-* on the given NWA
       *
       * This method constructs the Kleene-* of the given NWA by adding epsilon transitions 
       * from all final states of the NWA to all initial states of the NWA.  
       * Note: The resulting NWA is NOT guaranteed to be deterministic.
       * Note: This method modifies the transitions of the NWA, so it cannot be 
       *        called on a NWA with no stuck state.
       *
       * @param - first: the NWA to perform the Kleene-* of
       *
       */
      void star( NWA const & first );

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      void star( NWARefPtr first )
      {
        star(*first);
      }
      /**
       *
       * @brief constructs the NWA resulting from performing Kleene-* on the given NWA
       *
       * This method constructs the Kleene-* of the given NWA by adding epsilon transitions 
       * from all final states of the NWA to all initial states of the NWA.  
       * Note: The resulting NWA is NOT guaranteed to be deterministic.
       *
       * @param - first: the NWA to perform the Kleene-* of
       * @param - stuck: the stuck state of the NWA to be constructed
       * @return the NWA resulting from performing Kleene-* on the given NWA
       *
       */
      static NWARefPtr star( NWA const & first, State stuck )
      {
        NWARefPtr nwa(new NWA(stuck));
        nwa->star(first);
        return nwa;
      }

      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      static NWARefPtr star( NWARefPtr first, State stuck )
      {
        return star(*first, stuck);
      }

      /**
       * 
       * @brief constructs the NWA that is the complement of the given NWA
       *
       * This method constructs the complement of the given NWA by determinizing it and
       * then replacing the set of final states with the set of states that are not final
       * Note: The resulting NWA is guaranteed to be deterministic.
       * Note: This method modifies the transitions of the NWA, so it cannot be 
       *        called on a NWA with no stuck state.
       *
       * @param - first: the NWA to perform the complement of
       *
       */
      void complement( NWA const & first );

      DEPRECATE("Use NWA const & version instead of NWARefPtr version")
      void complement( NWARefPtr first )
      {
        complement(*first);
      }
      

      /**
       * 
       * @brief constructs the NWA that is the complement of the given NWA
       *
       * This method constructs the complement of the given NWA by determinizing it and
       * then replacing the set of final states with the set of states that are not final
       * Note: The resulting NWA is guaranteed to be deterministic.
       *
       * @param - first: the NWA to perform the complement of
       * @param - stuck: the stuck state of the NWA to be constructed
       * @return the NWA resulting from complementing the given NWA
       *
       */
      static NWARefPtr complement( NWA const & first, State stuck )
      {
        NWARefPtr nwa(new NWA(stuck));
        nwa->complement(first);
        return nwa;
      }


      DEPRECATE("Use NWA const & version instead of NWARefPtr version")
      static NWARefPtr complement( NWARefPtr first, State stuck )
      {
        return complement(*first, stuck);
      }

      /**
       *
       * @brief constructs a deterministic NWA that is equivalent to the given NWA.
       *
       * This method constructs a deterministic NWA that is equivalent to the given NWA.
       * Note: The resulting NWA is guaranteed to be deterministic.
       * Note: This method modifies the transitions of the NWA, so it cannot be 
       *        called on a NWA with no stuck state.
       *
       * @param - nondet: the NWA to determinize
       *
       */
      void determinize( NWA const & nondet );

      DEPRECATE("Use NWA const & version instead of NWARefPtr version")
      void determinize( NWARefPtr nondet )
      {
        determinize(*nondet);
      }

      /**
       *
       * @brief constructs a deterministic NWA that is equivalent to the given NWA.
       *
       * This method constructs a deterministic NWA that is equivalent to the given NWA.
       * Note: The resulting NWA is guaranteed to be deterministic.
       *
       * @param - nondet: the NWA to determinize
       * @param - stuck: the stuck state of the NWA to be constructed
       * @return the NWA resulting from determinizing the given NWA
       *
       */
      static NWARefPtr determinize( NWA const & nondet, State stuck )
      {
        NWARefPtr nwa(new NWA(stuck));
        nwa->determinize(nondet);
        return nwa;
      }

      DEPRECATE("Use NWA const & version instead of NWARefPtr version")
      static NWARefPtr determinize( NWARefPtr nondet, State stuck )
      {
        return determinize(*nondet, stuck);
      }

      /**
       *
       * @brief tests whether this NWA is deterministic 
       *
       * This method tests whether this NWA is deterministic or nondeterministic. If this 
       * NWA is deterministic, true is returned.  Otherwise, false is returned.
       *
       * @return true if this NWA is deterministic, false otherwise
       *
       */
      bool isDeterministic( );

      /**
       * 
       * @brief determines whether there is any overlap in the states of the given NWAs,
       *        except that the stuck state is allowed to be in both IF it is the stuck in both
       *
       * This method tests whether there is any overlap in the states of the given NWAs.
       *
       *   first->states()  intersect  second->states()  =  {first->stuck()}  intersect  {second->stuck()}
       *
       * @param - first: one of the NWAs whose states to compare
       * @param - second: one of the NWAs whose states to compare
       * @return true if there is some overlap in the states of the given NWAs, false otherwise
       *
       */
      static bool overlap(NWA const & first, NWA const & second)
      {
        StateSet intersection;
        // The following line does 'intersection = first->states() intersect second->states()'
        std::set_intersection(first.beginStates(), first.endStates(),
                              second.beginStates(), second.endStates(),
                              std::inserter(intersection, intersection.begin()));

        if (intersection.size() == 0) {
          // No states are shared
          return false;
        }
        else if (intersection.size() == 1) {
          // If the stuck states are equal, then they are the sole thing in
          // intersection: we will return false. If the stuck states are unequal,
          // there must be something else in intersection: return true.
          return first.getStuckState() != second.getStuckState();
        }
        else {
          return true;
        }
      }
      DEPRECATE("Use const NWA & version instead of NWARefPtr version")
      static bool overlap(NWARefPtr first, NWARefPtr second) {
        return overlap(*first, *second);
      }

      /**
       * 
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the states 'entry1'
       * and 'entry2' given that the transition that is being created is a call transition 
       * with the given symbol using the information in the given states and sets the 
       * resulting client information.
       * Note: This method should only be used to intersect client information for states 
       *        immediately following a call transition.
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
      virtual void intersectClientInfoCall( NWA const & first, State call1, State entry1, 
                                            NWA const & second, State call2, State entry2, 
                                            Symbol resSym, State resSt );  

      /**
       * 
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the states 'tgt1' and
       * 'tgt2' given that the transition that is being created is an internal transition 
       * with the given symbol using the information in the given states and returns the 
       * resulting client information.
       * Note: This method should only be used to intersect client information for states 
       *        immediately following an internal transition.
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
       *
       */
      virtual void intersectClientInfoInternal( NWA const & first, State src1, State tgt1, 
                                                NWA const & second, State src2, State tgt2, 
                                                Symbol resSym, State resSt );  

      /**
       * 
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the states 'ret1' and
       * 'ret2' given that the transition that is being created is a return transition with
       * the given symbol using the information in the given states and returns the 
       * resulting client information.
       * Note: This method should only be used to intersect client information for states 
       *        immediately following a return transition.
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
       *
       */
      virtual void intersectClientInfoReturn( NWA const & first, State exit1, State call1, State ret1,
                                              NWA const & second, State exit2, State call2, State ret2,
                                              Symbol resSym, State resSt );

      /**
       * 
       * @brief intersect states
       * 
       * This method determines whether the given states can be intersected and returns the result
       * in the reference parameter 'resSt'.
       *
       * @param - first: the NWA in which to look up information about 'state1'
       * @param - state1: the first state to intersect
       * @param - second: the NWA in which to look up information about 'state2'
       * @param - state2: the second state to intersect
       * @param - resSt: the state that results from performing the intersection
       * @param - resCI: the client info that results from performing the intersection
       *
       */
      virtual bool stateIntersect( NWA const & first, State state1, NWA const & second, State state2,
                                  State & resSt, ClientInfoRefPtr & resCI );

      /**
       * 
       * @brief intersect symbols
       * 
       * This method performs the intersection of the given symbols and returns the result
       * in the reference parameter 'resSym'.
       *
       * @param - first: the NWA in which to look up information about 'sym1'
       * @param - sym1: the first symbol to intersect
       * @param - second: the NWA in which to look up information about 'sym2'
       * @param - sym2: the second symbol to intersect
       * @param - resSym: the symbol that results from performing the intersection
       *
       */
      virtual bool transitionIntersect( NWA const & first, Symbol sym1, NWA const & second, Symbol sym2,
                                        Symbol & resSym );

      /**
       * 
       * @brief merges clientInfo for determinize
       * 
       * This method merges the client info for the given states and returns the result in the 
       * reference parameter 'resCI'.
       *
       * @param - nwa: the NWA in which to look up information about the states
       * @param - binRel: the states to merge
       * @param - resSt: the state resulting from the merge
       * @param - resCI: the client info that results from performing the merge
       *
       */
      virtual void mergeClientInfo( NWA const & nwa, 
                     relations::RelationTypedefs<State>::BinaryRelation const & binRel, 
                                  State resSt, ClientInfoRefPtr & resCI );

      /**
       * 
       * @brief merges clientInfo for determinize
       * 
       * This method merges the client info for the given entry states given that the transition
       * that is being created is a call transition with the given symbol using the information in 
       * the given states and returns the result in the reference parameter 'resCI'.
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
      virtual void mergeClientInfoCall( NWA const & nwa, 
                     relations::RelationTypedefs<State>::BinaryRelation const & binRelCall, 
                     relations::RelationTypedefs<State>::BinaryRelation const & binRelEntry,
                                  State callSt, Symbol resSym, State resSt, ClientInfoRefPtr & resCI );

      /**
       * 
       * @brief merges clientInfo for determinize
       * 
       * This method merges the client info for the given target states given that the transition
       * that is being created is an internal transition with the given symbol using the information in 
       * the given states and returns the result in the reference parameter 'resCI'.
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
      virtual void mergeClientInfoInternal( NWA const & nwa, 
                     relations::RelationTypedefs<State>::BinaryRelation const & binRelSource, 
                     relations::RelationTypedefs<State>::BinaryRelation const & binRelTarget,
                                  State sourceSt, Symbol resSym, State resSt, ClientInfoRefPtr & resCI );

      /**
       * 
       * @brief merges clientInfo for determinize
       * 
       * This method merges the client info for the given return states given that the transition
       * that is being created is a return transition with the given symbol using the information in 
       * the given states and returns the result in the reference parameter 'resCI'.
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
      virtual void mergeClientInfoReturn( NWA const & nwa, 
                     relations::RelationTypedefs<State>::BinaryRelation const & binRelExit,
                     relations::RelationTypedefs<State>::BinaryRelation const & binRelCall, 
                     relations::RelationTypedefs<State>::BinaryRelation const & binRelReturn,
                            State exitSt, State callSt, Symbol resSym, State resSt, ClientInfoRefPtr & resCI );


      //Using NWAs

      /**
       * 
       * @brief constructs the WPDS which is the result of the explicit NWA plus WPDS 
       *        construction from Advanced Querying for Property Checking
       *
       * This method constructs the WPDS which allows WPDS reachability to be used to 
       * perform property checking using this NWA and the given WPDS.
       * Note: For now we are assuming that implicit transitions to the stuck state need to be 
       *       realized for this construction to work.  Therefore, this method can only be called
       *       on an NWA of type 1 and afterwards it is an NWA of type 2.
       *
       *
       * @param - base: the WPDS that forms the basis for the constructed WPDS
       * @return the WPDS which can be used to perform property checking using PDS 
       *          reachability
       * 
       */
      wpds::WPDS plusWPDS( const wpds::WPDS & base ); 
      /**
       * 
       * @brief constructs the WPDS which is the result of the explicit NWA plus WPDS 
       *        construction from Advanced Querying for Property Checking
       *
       * This method constructs the WPDS which allows WPDS reachability to be used to 
       * perform property checking using this NWA and the given WPDS.
       * Note: For now we are assuming that implicit transitions to the stuck state need to be 
       *       realized for this construction to work.  Therefore, this method can only be called
       *       on an NWA of type 1 and afterwards it is an NWA of type 2.
       *
       *
       * @param - base: the WPDS that forms the basis for the constructed WPDS
       * @param - nwa: the NWA to process
       * @return the WPDS which can be used to perform property checking using PDS 
       *          reachability
       * 
       */
      static wpds::WPDS plusWPDS( const wpds::WPDS & base, NWA & nwa )
      {
        return nwa.plusWPDS(base);
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
      void PDStoNWA( const wpds::WPDS & pds ); 
      /**
       *
       * @brief constructs the NWA equivalent to the given PDS
       *
       * This method constructs the NWA that is equivalent to the given PDS.
       *
       * @param - pds: the pds to convert
       * @param - stuck: the stuck state of the NWA to construct
       * @return the NWA equivalent to the given PDS
       *
       */
      static NWARefPtr PDStoNWA( const wpds::WPDS & pds, State stuck )
      {
        NWARefPtr nwa(new NWA(stuck));
        nwa->PDStoNWA(pds);
        return nwa;
      }

      /**
       *  
       * @brief returns the default program control location for PDSs
       *
       * This method provides access to the default program control location for PDSs.
       * 
       * @return the default program control location for PDSs
       *
       */  
      static wali::Key getProgramControlLocation( )
      {
        static Key key = getKey("program");
        return key;
      }

      /**
       *  
       * @brief returns the program control location corresponding to the given states
       *
       * This method provides access to the program control location corresponding to
       * the given exit point/call site/return site triple.
       *
       * @param - exit: the exit point corresponding to this control location
       * @param - callSite: the call site corresponding to this control location
       * @param - returnSite: the return site corresponding to this control location
       * @return the program control location corresponding to the given states
       *
       */
      static wali::Key getControlLocation( Key exit, Key callSite, Key returnSite )
      {
        std::stringstream ss;
        ss << "(key#"  << exit << "," << callSite << "," << returnSite << ")";
        wali::Key key = getKey(getProgramControlLocation(), getKey(ss.str()));
        return key;
      }

      /**
       *
       * @brief constructs the PDS equivalent to this NWA
       *
       * This method constructs the PDS that is equivalent to this NWA.
       * Note: This version keeps returns on the stack.
       *
       * @param - wg: the functions to use in generating weights
       * @return the PDS equivalent to this NWA
       *
       */ 
      wpds::WPDS NWAtoPDSreturns( WeightGen & wg ) const;
      /**
       *
       * @brief constructs the PDS equivalent to this NWA
       *
       * This method constructs the PDS that is equivalent to this NWA.
       * Note: This version keeps returns on the stack.
       *
       * @param - nwa: the nwa to convert
       * @param - wg: the functions to use in generating weights
       * @return the PDS equivalent to this NWA
       *
       */ 
      static wpds::WPDS NWAtoPDSreturns( NWA const & nwa, WeightGen & wg )
      {
        return nwa.NWAtoPDSreturns(wg);
      }

      DEPRECATE("Use NWA const & version instead of NWARefPtr version")
      static wpds::WPDS NWAtoPDSreturns( NWARefPtr nwa, WeightGen & wg )
      {
        return NWAtoPDSreturns(*nwa, wg);
      }
      /**
       *
       * @brief constructs the backwards PDS equivalent to this NWA
       *
       * This method constructs the backwards PDS that is equivalent to this NWA.
       * Note: This version keeps returns on the stack.
       *
       * @param - wg: the functions to use in generating weights
       * @return the backwards PDS equivalent to this NWA
       *
       */ 
      wpds::WPDS NWAtoBackwardsPDSreturns( WeightGen & wg ) const;  
      /**
       *
       * @brief constructs the backwards PDS equivalent to this NWA
       *
       * This method constructs the backwards PDS that is equivalent to this NWA.
       * Note: This version keeps returns on the stack.
       *
       * @param - nwa: the nwa to convert
       * @param - wg: the functions to use in generating weights
       * @return the backwards PDS equivalent to this NWA
       *
       */ 
      static wpds::WPDS NWAtoBackwardsPDSreturns( NWA const & nwa, WeightGen & wg )
      {
        return nwa.NWAtoBackwardsPDSreturns(wg);
      }

      DEPRECATE("Use NWA const & version instead of NWARefPtr version")
      static wpds::WPDS NWAtoBackwardsPDSreturns( NWARefPtr nwa, WeightGen & wg )
      {
        return NWAtoBackwardsPDSreturns(*nwa, wg);
      }

      /**
       *
       * @brief constructs the PDS equivalent to this NWA
       *
       * This method constructs the PDS that is equivalent to this NWA.
       * Note: This version keeps calls on the stack.
       *
       * @param - wg: the functions to use in generating weights
       * @return the PDS equivalent to this NWA
       *
       */ 
      wpds::WPDS NWAtoPDScalls( WeightGen & wg ) const;
      /**
       *
       * @brief constructs the PDS equivalent to this NWA
       *
       * This method constructs the PDS that is equivalent to this NWA.
       * Note: This version keeps calls on the stack.
       *
       * @param - nwa: the nwa to convert
       * @param - wg: the functions to use in generating weights
       * @return the PDS equivalent to this NWA
       *
       */ 
      static wpds::WPDS NWAtoPDScalls( NWA const & nwa, WeightGen & wg )
      {
        return nwa.NWAtoPDScalls(wg);
      }

      DEPRECATE("Use NWA const & version instead of NWARefPtr version")
      static wpds::WPDS NWAtoPDScalls( NWARefPtr nwa, WeightGen & wg )
      {
        return nwa->NWAtoPDScalls(wg);
      }

      /**
       *
       * @brief constructs the backwards PDS equivalent to this NWA
       *
       * This method constructs the backwards PDS that is equivalent to this NWA.
       * Note: This version keeps calls on the stack.
       *
       * @param - wg: the functions to use in generating weights
       * @return the backwards PDS equivalent to this NWA
       *
       */ 
      wpds::WPDS NWAtoBackwardsPDScalls( WeightGen & wg ) const; 
      /**
       *
       * @brief constructs the backwards PDS equivalent to this NWA
       *
       * This method constructs the backwards PDS that is equivalent to this NWA.
       * Note: This version keeps calls on the stack.
       *
       * @param - nwa: the NWA to convert
       * @param - wg: the functions to use in generating weights
       * @return the backwards PDS equivalent to this NWA
       *
       */
      static wpds::WPDS NWAtoBackwardsPDScalls( NWA const & nwa, WeightGen & wg )
      {
        return nwa.NWAtoBackwardsPDScalls(wg);
      }

      DEPRECATE("Use NWA const & version instead of NWARefPtr version")
      static wpds::WPDS NWAtoBackwardsPDScalls( NWARefPtr nwa, WeightGen & wg )
      {
        return nwa->NWAtoBackwardsPDScalls(wg);
      }
      
      /**
       *
       * @brief tests whether the language accepted by this NWA is empty
       *
       * This method tests whether the language accepted by this NWA is empty.
       *
       * @return true if the language accepted by this NWA is empty
       *
       */
      bool isEmpty( ) const;

      /**
       *
       * @brief tests whether the language of the first NWA is included in the language of 
       *        the second NWA
       *
       * This method tests whether the language of the first NWA is included in the language
       * of the second NWA.
       *
       * @param - first: the proposed subset
       * @param - second: the proposed superset
       * @return true if the language of the first NWA is included in the language of the 
       *          second NWA, false otherwise
       *
       */
      static bool inclusion( NWA const & first, NWA const & second )
      {
        //Q: what should be used as the stuck states here? 
        //A: it needs to be something not a state in 'first' and not a state in 'second' 
        //    unless it is the stuck state
        std::string s = "stuck";
        Key ss = getKey(s);
        while( first.isState(ss) || (second.isState(ss) && !second.isStuckState(ss)) )
        {
          s = s + "~";
          ss = getKey(s);
        }

        //Check L(a1) contained in L(a2) by checking 
        //if L(a1) intersect (complement L(a2)) is empty.
        NWA comp(ss);
        comp.complement(second);   //complement L(a2)
        NWA inter(ss);
        inter.intersect(first,comp); //L(a1) intersect (complement L(a2))

        return inter.isEmpty();
      }

      DEPRECATE("Use NWA const & version instead of NWARefPtr version")
      static bool inclusion( NWARefPtr first, NWARefPtr second )
      {
        return inclusion(*first, *second);
      }

      /**
       *
       * @brief tests whether the languages of the given NWAs are equivalent
       *
       * This method tests the equivalence of the languages accepted by the given NWAs.
       *
       * @param - first: one of the NWAs whose language to test
       * @param - second: one of the NWAs whose language to test
       * @return true if the languages accepted by the given NWAs are equal, false otherwise
       *
       */
      static bool equal( NWA const & first, NWA const & second)
      {
        NWARefPtr det_first = determinize(first, getKey("[stuck-3119]"));
        NWARefPtr det_second = determinize(second, getKey("[stuck-3120]"));
        
        //The languages accepted by two NWAs are equivalent if they are both contained
        //in each other, ie L(a1) contained in L(a2) and L(a2) contained in L(a1).
        bool first_in_second = inclusion(*det_first, *det_second);
        bool second_in_first = inclusion(*det_second, *det_first);

        return (first_in_second && second_in_first );
      }

      DEPRECATE("Use NWA const & version instead of NWARefPtr version")
      static bool equal( NWARefPtr first, NWARefPtr second)
      {
        return equal(*first, *second);
      }

      /**
       *
       * @brief perform the prestar reachability query defined by the given WFA
       *
       * This method performs the prestar reachability query defined by the given WFA.
       *
       * @param - input: the starting point of the reachability query
       * @param - wg: the functions to use in generating weights
       * @return the WFA resulting from performing the prestar reachability query 
       *
       */
      virtual wfa::WFA prestar( wfa::WFA & input, WeightGen & wg ) const;

      /**
       *
       * @brief perform the prestar reachability query defined by the given WFA
       *
       * This method performs the prestar reachability query defined by the given WFA.
       * The result of the query is stored in the 'output' parameter. 
       * Note: Any transitions in output before the query will be there after the query but
       * will have no effect on the reachability query.
       *
       * @param - input: the starting point of the reachability query
       * @param - ouput: the result of performing the reachability query
       * @param - wg: the functions to use in generating weights
       *
       */
      virtual void prestar( wfa::WFA & input, wfa::WFA & output, WeightGen & wg ) const;

      /**
       *
       * @brief perform the poststar reachability query defined by the given WFA
       *
       * This method performs the poststar reachability query defined by the given WFA.
       *
       * @param - input: the starting point of the reachability query
       * @param - wg: the functions to use in generating weights
       * @return the WFA resulting from performing the poststar reachability query
       *
       */
      virtual wfa::WFA poststar( wfa::WFA & input, WeightGen & wg ) const;

      /**
       *
       * @brief perform the poststar reachability query defined by the given WFA
       * 
       * This method performs the poststar reachability query defined by the given WFA.
       * The result of the query is stored in the 'output' parameter.
       * Note: Any transitions in output before the query will be there after the query but
       * will have no effect on the reachability query.
       *
       * @param - input: the starting point of the reachability query
       * @param - output: the result of performing the reachability query
       * @param - wg: the functions to use in generating weights
       *
       */
      virtual void poststar( wfa::WFA & input, wfa::WFA & output, WeightGen & wg ) const;

      //Utilities	

      /**
       *
       * @brief print the NWA
       *
       * This method prints out the NWA to the output stream provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to
       *
       */
      virtual std::ostream & print( std::ostream & o ) const;

      /**
       *
       * @brief print the NWA in dot format
       *
       * This method prints out the NWA in dot format to the output stream provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to
       *
       */
      virtual std::ostream & print_dot( std::ostream & o, std::string title, bool abbrev = true ) const;

      /**
       *
       * @brief tests whether this NWA is equivalent to the NWA 'other'
       *
       * This method tests the equivalence of this NWA and the NWA 'other'.
       *
       * @param - other: the NWA to compare this NWA to
       * @return true if this NWA is equivalent to the NWA 'other'
       *
       */
      bool operator==( const NWA & other ) const;

      //TODO: add methods like ...
      //virtual void for_each(ConstRuleFunctor &func) const;
      //virtual void for_each(RuleFunctor &func) const;
      //virtual void operator()(wfa::ITrans *t);

     /**
       *
       * @brief add all the states in the given StateSet to the NWA
       *
       * This method adds all of the given states to the state set for the NWA.
       *
       * @param - addStateSet: the StateSet that contains the states to add
       *
       */
      void addAllStates( StateStorage addStateSet );  

      /**
       *
       * @brief provide access to the beginning of the state set
       *
       * This method provides access to the beginning of the state set associated with 
       * this NWA.
       *
       * @return an iterator pointing to the beginning of the state set
       *
       */
      stateIterator beginStates( ) const;  

      /**
       * 
       * @brief provide access to the end of the state set
       *
       * This method provides access to the position one past the end of the state set
       * associated with this transition set.
       *
       * @return an iterator pointing just past the end of the state set
       *
       */
      stateIterator endStates( ) const;  

      /**
       *
       * @brief adds all of the states in the given StateSet to the initial state set 
       *        associated with this NWA
       *
       * This method adds all of the states associated with the given StateSet to the 
       * initial state set associated with this NWA.
       *
       * @param - addStateSet: the state set whose initial states to add to this NWA's
       *                        initial state set
       *
       */
      void addAllInitialStates( StateStorage addStateSet ); 

      /**
       *
       * @brief provide access to the beginning of the initial state set
       *
       * This method provides access to the beginning of the initial state set associated
       * with this NWA.
       *
       * @return an iterator pointing to the beginning of the initial state set
       *
       */ 
      stateIterator beginInitialStates( ) const;  

      /**
       * 
       * @brief provide access to the end of the initial state set
       *
       * This method provides access to the position one past the end of the initial 
       * state set associated with this NWA.
       *
       * @return an iterator pointing just past the end of the initial state set
       *
       */
      stateIterator endInitialStates( ) const;  

      /**
       *
       * @brief adds all of the final states in the given StateSet to the final state set
       *        associated with this NWA
       *
       * This method adds all of the final states associated with the given StateSet to 
       * the final state set associated with this NWA.
       *
       * @param - addStateSet: the StateSet whose final states to add to this NWA's
       *                        final state set
       *
       */
      void addAllFinalStates( StateStorage addStateSet ); 

      /**
       *
       * @brief provide access to the beginning of the final state set
       *
       * This method provides access to the beginning of the final state set associated 
       * with this NWA.
       *
       * @return an iterator pointing to the beginning of the final state set
       *
       */
      stateIterator beginFinalStates( ) const;  

      /**
       * 
       * @brief provide access to the end of the final state set
       *
       * This method provides access to the position one past the end of the final state 
       * set associated with this NWA.
       *
       * @return an iterator pointing just past the end of the final state set
       *
       */
      stateIterator endFinalStates( ) const;  

      /**
       *
       * @brief add the given symbols to the NWA
       *
       * This method adds all of the given symbols to the set of symbols associated with
       * the NWA.  
       *
       * @param - addSymbolSet: the symbols to add
       *
       */
      void addAllSymbols( SymbolStorage addSymbolSet );    

      /**
       *
       * @brief provide access to the beginning of the symbol set
       *
       * This method provides access to the beginning of the symbol set associated with 
       * this NWA.
       *
       * @return an iterator pointing to the beginning of the symbol set
       *
       */
      symbolIterator beginSymbols( ) const;  

      /**
       * 
       * @brief provide access to the end of the symbol set
       *
       * This method provides access to the position one past the end of the symbol set 
       * associated with NWA.
       *
       * @return an iterator pointing just past the end of the symbol set
       *
       */
      symbolIterator endSymbols( ) const;  

      /**
       *
       * @brief provide access to the beginning of the call transition set
       *
       * This method provides access to the beginning of the call transition set 
       * associated with this NWA.
       *
       * @return an iterator pointing to the beginning of the call transition set
       *
       */
      CallIterator beginCallTrans( ) const;  

      /**
       * 
       * @brief provide access to the end of the call transition set
       *
       * This method provides access to the position one past the end of the call 
       * transition set associated with this NWA.
       *
       * @return an iterator pointing just past the end of the call transition set
       *
       */
      CallIterator endCallTrans( ) const;  

      /**
       *
       * @brief provide access to the beginning of the internal transition set
       *
       * This method provides access to the beginning of the internal transition set 
       * associated with this NWA.
       *
       * @return an iterator pointing to the beginning of the internal transition set
       *
       */
      InternalIterator beginInternalTrans( ) const;  

      /**
       * 
       * @brief provide access to the end of the internal transition set
       *
       * This method provides access to the position one past the end of the internal 
       * transition set associated with this NWA.
       *
       * @return an iterator pointing just past the end of the internal transition set
       *
       */
      InternalIterator endInternalTrans( ) const; 

      /**
       *
       * @brief provide access to the beginning of the return transition set
       *
       * This method provides access to the beginning of the return transition set 
       * associated with this NWA.
       *
       * @return an iterator pointing to the beginning of the return transition set
       *
       */
      ReturnIterator beginReturnTrans( ) const;  

      /**
       * 
       * @brief provide access to the end of the return transition set
       *
       * This method provides access to the position one past the end of the return 
       * transition set associated with this NWA.
       *
       * @return an iterator pointing just past the end of the return transition set
       *
       */
      ReturnIterator endReturnTrans( ) const;  

    protected:

      /**
       *
       * @brief computes the epsilon closure of the given state
       *
       * This method computes the set of states reachable by starting at the given 
       * state and moving along epsilon transitions.
       *
       * @param - newPairs: the set of states reachable from the initial state pair by 
       *                    traversing epsilon transitions   
       * @param - sp: the starting point of the closure
       *
       */
      void epsilonClosure(  StateSet * newPairs, State sp ) const;

      /**
       *
       * @brief computes the epsilon closure of the given states in their respective NWAs
       *
       * This method computes the set of state pairs reachable by starting at the given 
       * state pair and moving along epsilon transitions.
       *
       * @param - newPairs: the set of state pairs reachable from the initial state pair
       *                     by traversing epsilon transitions
       * @param - sp: the starting point of the closure
       * @param - first: the NWA that determines the transitions available to the first 
       *                  component of the state pair
       * @param - second: the NWA that determines the transitions available to the second
       *                  component of the state pair
       *
       */
      void epsilonClosure(  std::set<StatePair> * newPairs, StatePair sp, NWA const & first, NWA const & second ) const;

      /**
       * 
       * @brief returns the state corresponding to the given binary relation
       *
       * This method provides access to the state corresponding to the given binary relation.
       *
       * @param - R: the binary relation whose state to access
       * @return the state corresponding to the given binary relation
       *
       */
      State makeKey(  relations::RelationTypedefs<State>::BinaryRelation const & R ) const;

      /**
       *
       * @brief tests whether the given nested word is a member of the language accepted 
       *        by the given deterministic NWA
       *
       * This method tests whether the given nested word is a member of the language 
       * accepted by the given deterministic NWA.  
       * Note: 'word' is required to be a perfectly balanced nested word.
       *
       * @param - word: the nested word to test
       * @param - aut: the deterministic NWA to check the word on
       * @return true if the given nested word is a member of the language accepted by 
       *          the given deterministic NWA
       *
       */
      DEPRECATE("I don't think anyone is using this NWS class or this method. If you disagree, talk to Evan.")       
      static bool simulateWord( nws::NWS word, NWARefPtr aut )
      {
        //Note: For now require that 'word' be a perfectly balanced nested word.
        assert(word.stackSize() == 0);

        State currSt;                                    //Placement in the automaton
        std::stack< State > callSites;                   //Call sites currently unmatched.
        nws::NWSNode * currNode;                      //Placement in the word
        std::stack< nws::NWSNode * > nesting;         //Exit nodes currently on the stack.
        Symbol currSym;                                  //Next symbol to follow.

        //Try starting at each initial state in turn. 
        for( stateIterator sit = aut->beginInitialStates(); sit != aut->endInitialStates(); sit++ )
        {
          currSt = *sit;
          callSites = std::stack< State >();                
          currNode = word.nextNode();
          nesting = std::stack< nws::NWSNode * >();

          //Try to walk through the NWA according to the word.
          while( currNode != NULL )   //Keep walking until the end is reached.
          {
            currSym = currNode->getSymbol();

            //At any given point in the traversal of the word, 
            //drive movement through the automata by following the
            //transition labeled with the symbol that occurs on the
            //next NWSNode.  Keep track of any calls on the stack.
            if( currNode == nesting.top() )   //Denotes a return transition.
            {                          
              //Check for return transitions leaving from 'currSt' with 'currSym'
              //Note: 'aut' is guaranteed to be deterministic (because it was determinized
              //      in isMember(), so there is at most one of these.

              //if one is found, 
              //then update stacks and 'currSt' (i.e. follow the transition) 
              //      and continue following transitions
              //else start checking at the next initial state
              if( aut->trans.returnExists( currSt, callSites.top(), currSym ) )
              {
                Returns rets = aut->trans.getReturns(currSt,currSym);
                for( ReturnIterator rit = rets.begin(); rit != rets.end(); rit++ )
                {
                  if( Trans::getCallSite(*rit) == callSites.top() )
                  {
                    callSites.pop();
                    nesting.pop();
                    currSt = Trans::getReturnSite(*rit);
                    break;    //Out of the for loop.
                  }
                }
              }
              else
              {
                break;    //Out of the while loop.
              }
            }
            else if( currNode->isCall() )   //Denotes a call transition.
            {
              //Check for call transitions leaving from 'currSt' with 'currSym'
              //Note: 'aut' is guaranteed to be deterministic (because it was determinized
              //      in isMember(), so there is at most one of these.

              //if one is found,
              //then update stacks and 'currSt' (i.e. follow the transition)
              //      and continue following transitions
              //else start checking at the next initial state

              if( aut->trans.callExists( currSt, currSym ) )
              {
                Calls calls = aut->trans.getCalls(currSt,currSym);
                for( CallIterator cit = calls.begin(); cit != calls.end(); cit++ )
                {
                  callSites.push(currSt);
                  nesting.push(currNode->exitNode());
                  currSt = Trans::getEntry(*cit);
                  break;    //Out of the for loop.    
                }
              }
              else
              {
                break;    //Out of the while loop.
              }
            }
            else    //Must be an internal transition.
            {
              //Check for internal transitions leaving from 'currSt' with 'currSym'
              //Note: 'aut' is guaranteed to be deterministic (because it was determinized
              //      in isMember(), so there is at most one of these.

              //if one is found,
              //then update 'currSt' (i.e. follow the transition)
              //      and continue following transitions
              //else start checking at the next initial state
              
              if( aut->trans.internalExists( currSt, currSym ) )
              {
                Internals ints = aut->trans.getInternals(currSt,currSym);
                for( InternalIterator iit = ints.begin(); iit != ints.end(); iit++ )
                {
                  currSt = Trans::getTarget(*iit);
                  break;    //Out of the for loop.    
                }
              }
              else
              {
                break;    //Out of the while loop.
              }
            }

            //If at any point we land in the stuck state, 
            //it cannot be final, so start checking at the next initial state
            if( aut->isStuckState(currSt) )
              break;

            //Advance to the next node.
            //Note: currNode must have a next, since currNode is not NULL (while condition above)
            //      however, that next might be NULL (at which point we will exit the while loop).
            currNode =  currNode->nextNode();                                      
          } 

          //At the end of the word, if we are in a final state,
          //then return true  (Note: this accepts nested word prefixes)
          //else start checking at the next initial state
          if( (currNode == NULL) && aut->isFinalState(currSt) )
            return true;
        }

        //No path from an initial state accepted the given word.
        return false;   
      }

      //
      // Variables
      //
      
      protected:
      
        bool stuck;

        StateStorage states;         
        SymbolStorage symbols;        
        Trans trans;

        //TODO: ponder the following ...
        //Q: should we incrementally maintain a wpds?
        //    if we did, what would the weight gen of the wpds be?

    public:
      bool
      isMemberNondet( ::wali::nwa::NestedWord const & word ) const;

      void combineWith(NWARefPtr rhs);
    };
  }
}


#endif

