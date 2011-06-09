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

#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"

#include "wali/nwa/WeightGen.hpp"


// TODO: remove these after removing deprecated stuff
#include "wali/nwa/construct/complement.hpp"
#include "wali/nwa/construct/concatenate.hpp"
#include "wali/nwa/construct/determinize.hpp"
#include "wali/nwa/construct/intersect.hpp"
#include "wali/nwa/construct/reverse.hpp"
#include "wali/nwa/construct/star.hpp"
#include "wali/nwa/construct/union.hpp"
#include "wali/nwa/nwa_pds/NWAToPDS.hpp"

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
    private:
      typedef ClientInfo Client;
      typedef details::StateStorage StateStorage;
      typedef details::SymbolStorage SymbolStorage;
      typedef details::TransitionStorage Trans;
      typedef std::pair<State,State> StatePair;

      static std::string const XMLTag;

      
    public:
      // {{{ Typedefs

      ///
      /// @brief ref_ptr to a ClientInfo object
      typedef StateStorage::ClientInfoRefPtr ClientInfoRefPtr;

      DEPRECATE("Use (capital-S) StateIterator")
      typedef StateStorage::const_iterator stateIterator;
      /// @brief Iterator for traversing a set of states
      ///
      /// Dereferences to a 'State'. (This is used for the set of states, set
      /// of initial states, and set of accepting states.)
      typedef StateStorage::const_iterator StateIterator;
      
      DEPRECATE("Use (capital-S) SymbolIterator")
      typedef SymbolStorage::const_iterator symbolIterator;
      ///
      /// @brief Iterator for traversing the set of symbols. Dereferences to a 'Symbol'
      typedef SymbolStorage::const_iterator SymbolIterator;


      /// @brief Represents a call transition
      ///
      /// Has the following fields:
      ///    Call::first    The call site (a State)
      ///    Call::second   The symbol (a Symbol)
      ///    Call::third    The entry (a State)
      typedef Trans::Call Call;
      
      /// @brief Represents an internal transition
      ///
      /// Has the following fields:
      ///    Internal::first    The source state (a State)
      ///    Internal::second   The symbol (a Symbol)
      ///    Internal::third    The target state (a State)
      typedef Trans::Internal Internal;
      
      /// @brief Represents a return transition
      ///
      /// Has the following fields:
      ///    Return::first    The exit site (a State)
      ///    Return::second   The call predecessor (a State)
      ///    Return::third    The symbol (a Symbol)
      ///    Return::fourth   The return site (a State)
      typedef Trans::Return Return;          
      
      typedef Trans::Calls Calls;
      typedef Trans::Internals Internals;
      typedef Trans::Returns Returns;

      ///
      /// @brief Iterator to a set of calls. Dereferences to a 'Call'.
      typedef Trans::CallIterator CallIterator;
      ///
      /// @brief Iterator to a set of internals. Dereferences to an 'Internal'
      typedef Trans::InternalIterator InternalIterator;
      ///
      /// @brief Iterator to a set of returns. Dereferences to a 'Return'
      typedef Trans::ReturnIterator ReturnIterator;


      /// @brief Represents a binary relation on states
      ///
      /// There are a couple possibilities for a concrete type for this,
      /// depending on whether you are using the (very experimental) BuDDY
      /// support or just std::maps. See the appropriate version of
      /// RelationOps.hpp.
      typedef relations::RelationTypedefs<State>::BinaryRelation BinaryRelation;

      // }}}



    public:


      ///
      /// @brief Constructs an empty NWA
      NWA( );

      /// @brief Copies 'other'. Does not share structure.
      ///
      /// Will clone the client info objects. TODO-RELEASE
      NWA( const NWA & other );

      /// @brief Assigns 'other' to this NWA. Does not share structure.
      ///
      /// Will clone the client info objects. TODO-RELEASE
      NWA & operator=( const NWA & other );

      /**
       * @brief Removes all states, symbols, and transitions from this NWA
       */
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
     

      
      //////////////////////////////
      // {{{ Query & Mutate States

      /// @brief Returns the set of states in the NWA
      ///
      /// @return The set of all states
      const StateSet & getStates( ) const;

      /// @brief Tests if a given state is in this NWA
      ///
      /// @param - state: the state to check
      /// @return true if the given state is a member of this NWA
      bool isState( State state ) const; 

      /// @brief Adds the given state to this NWA
      ///
      /// If the state is already a member of this NWA. returns false.
      /// Otherwise, returns true.
      ///
      /// @param - state: the state to add
      /// @return 'false' if the state already exists in the NWA, 'true' otherwise
      bool addState( State state );

      /// @brief Returns the number of states in this NWA
      ///
      /// Foo bar
      ///
      /// @return the number of states in this NWA
      size_t sizeStates( ) const;

      /// @brief Returns the largest state ID in the automaton
      ///
      /// @return the largest state ID in the automaton
      State largestState() const {
        return states.largestState();
      }

      /// @brief Removes the given state from this NWA along with any
      ///        associated transitions.
      ///
      ///
      /// This removes 'state' from the set of initial and final states as
      /// well as the set of all states. It also removes any transitions (of
      /// any type) with 'state' as a component.
      ///
      /// If 'state' is not present, this command is a no-op (except for the
      /// return value). The return value indicates whether the state was
      /// actually removed.
      ///
      /// @param - state: the state to remove
      /// @return false if the state did not exist in the NWA, true otherwise
      virtual bool removeState( State state );

      
      /// @brief remove all states and transitions from the NWA
      ///
      /// This method removes all states and transitions from the NWA.
      void clearStates( );

      // }}}
      

      // {{{ Query & Mutate Initial States

      /// @brief Returns the set of initial states
      ///
      /// (Note: An NWA can have an unbounded number of inital states.)
      ///
      /// @return the set of inital states associated with the NWA
      const StateSet & getInitialStates( ) const; 

      /**
       * @brief test if the given state is an initial state of this NWA
       *
       * @param - state: the state to check
       * @return true if the given state is an initial state, false otherwise
       */
      bool isInitialState( State state ) const;

      /// @brief Makes the given state an initial state in this NWA (adding
      ///        it to the NWA if necessary).
      ///
      /// This method checks whether 'state' is in the state set of this NWA.
      /// If the state is not present, it is added to the automaton.  In
      /// either case, the given state is then added to the set of initial
      /// states for the NWA.
      ///
      /// Returns whether the state was added to the set of initial
      /// states. (The return value is 'true' if it was not already an
      /// initial state regardless of whether it needed to be added to the
      /// machine or not.)
      ///
      /// @param - state: the state to add to initial state set
      /// @return false if the state already exists in the initial state set of the NWA
      bool addInitialState( State state );

      /// @brief Returns the number of initial states in this NWA
      ///
      /// @return the number of initial states in this NWA
      size_t sizeInitialStates( ) const;

      /// @brief Removes the given state from the set of initial states.
      ///
      /// If 'state' is not already an initial state, this function is a
      /// no-op (except for the return value), even if 'state' is not in the
      /// NWA at all.
      ///
      /// The return value indicates whether 'state' was an initial state.
      ///
      /// (This method does not remove any transitions from the NWA, nor does
      /// it remove the state from the NWA entirely.)
      ///
      /// @param - state: the state to remove from the initial state set
      /// @return false if the state does not exist in the initial state set of this NWA
      bool removeInitialState( State state );

      /// @brief Clears the set of initial states
      ///
      /// (This function does not remove any states or transitions from the
      /// automaton, it just affects the set of initial states.)
      void clearInitialStates( );

      // }}}

      // {{{ Query and Mutate Final States

      /// @brief Returns the set of final states
      ///
      /// (Note: An NWA can have an unbounded number of inital states.)
      ///
      /// @return the set of inital states associated with the NWA
      const  StateSet & getFinalStates( ) const;

      /**
       * @brief test if a state with the given name is a final state
       *
       * This method tests whether the given state is in the final state set
       * of the NWA.
       *
       * @param - state: the state to check
       * @return true if the given state is a final state
       */
      bool isFinalState( State state ) const;

      /// @brief Makes the given state an final state in this NWA (adding
      ///        it to the NWA if necessary).
      ///
      /// This method checks whether 'state' is in the state set of this NWA.
      /// If the state is not present, it is added to the automaton.  In
      /// either case, the given state is then added to the set of final
      /// states for the NWA.
      ///
      /// Returns whether the state was added to the set of final
      /// states. (The return value is 'true' if it was not already a final
      /// state regardless of whether it needed to be added to the machine or
      /// not.)
      ///
      /// @param - state: the state to add to final state set
      /// @return false if the state already exists in the final state set of the NWA
      bool addFinalState( State state );

      /// @brief Returns the number of final states in this NWA
      ///
      /// @return the number of final states in this NWA
      size_t sizeFinalStates( ) const;

      /// @brief Removes the given state from the set of final states.
      ///
      /// If 'state' is not already an final state, this function is a
      /// no-op (except for the return value), even if 'state' is not in the
      /// NWA at all.
      ///
      /// The return value indicates whether 'state' was an final state.
      ///
      /// (This method does not remove any transitions from the NWA, nor does
      /// it remove the state from the NWA entirely.)
      ///
      /// @param - state: the state to remove from the final state set
      /// @return false if the state does not exist in the final state set of this NWA
      bool removeFinalState( State state );

      /// @brief Clears the set of final states
      ///
      /// (This function does not remove any states or transitions from the
      /// automaton, it just affects the set of final states.)
      void clearFinalStates( );

      // }}}


      // {{{ Query and Mutate Symbols

      /// @brief Returns the set of (non-epsilon, non-wild) symbols in this NWA.
      ///
      /// Note: Not all symbols need to be used on a transition. (This
      /// function will still return them.)
      ///
      /// @return set of all symbols in this NWA
      const  SymbolSet & getSymbols( ) const;

      /// @brief Tests whether the given symbol is a member the NWA.
      ///
      /// This returns 'false' for WALI_EPSILON and WALI_WILD, even if they
      /// are used on a transition.
      ///
      /// @param - sym: the symbol to check
      /// @return true if the given symbol is associated with the NWA
      bool isSymbol( Symbol sym ) const;

      /// @brief Adds the given symbol to the NWA.
      ///
      /// If 'sym' is already present in the NWA, this function is a
      /// no-op. It is also a no-op if 'sym' is either 'WALI_EPSILON' or
      /// 'WALI_WILD'.
      ///
      /// The return value indicates whether 'sym' was added.
      ///
      /// @param - sym: the symbol to add
      /// @return false if the symbol is already associated with the NWA
      bool addSymbol( Symbol sym );

      /// @brief Returns the number of symbols in this NWA (excluding epsilon
      ///        and wild).
      ///
      /// Symbols in the automaton are counted regardless of whether they are
      /// used to label any transitions. Neither 'WALI_EPSILON' nor
      /// 'WALI_WILD' are included in this count at all, regardless of
      /// whether they are used to label transitions or not.
      ///
      /// @return The number of symbols in this NWA
      size_t sizeSymbols( ) const;

      /// @brief Remove the given symbol from the NWA along with any
      ///        associated transitions.
      ///
      /// Removes 'sym' from the symbol set of the NWA as well as any
      /// transitions that have 'sym' as the symbol component.
      ///
      /// If 'sym' is not present, this command is a no-op; the return value
      /// indicates whether this was the case.
      ///
      /// @param - sym: the symbol to remove
      /// @return false if the symbols was not in the NWA
      bool removeSymbol( Symbol sym );

      /// @brief Remove all symbols and transitions from the NWA
      ///
      /// This method removes all symbols and all transitions from the
      /// NWA. It does not touch any states.
      ///
      /// (Note: this also removes all transitions labeled with
      /// 'WALI_EPSILON' and 'WALI_WILD' even though this perhaps needn't be
      /// the case.)
      void clearSymbols( );

      // }}}

      

      /// Please don't call this, in case that's not evident from the
      /// name. It's const, so you won't break the NWA, but I make no
      /// guarantees it'll be around in future versions.
      Trans const & _private_get_transition_storage_() const  {
        return trans;
      }
      

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
       * This method makes explicit all transitions in the NWA by adding the
       * given stuckState (if necessary) then any transitions to the stuck
       * state for each state/symbol(excluding epsilon) pair for which no
       * outgoing edge(one of each kind-call,internal,return) from that state
       * is labeled with that symbol.
       *
       */
      void realizeImplicitTrans(State stuckState);

      
      /// @brief Returns the number of transitions in this NWA (regardless of
      ///        type).
      ///
      /// @return the number of transitions in this NWA
      size_t sizeTrans( ) const;

      /// @brief Removes all transitions from this NWA.
      ///
      /// This function does not remove any states or symbols.
      void clearTrans( );
      

      // {{{ Query and Mutate Internal Transitions

      /// @brief Adds a internal transition to the NWA.
      ///
      /// If the internal transition is already present, this function is a
      /// no-op (other than the return value). The return value indicates
      /// whether the transition was successfully added.
      ///
      /// Note: 'sym' cannot be WALI_EPSILON.
      ///
      /// @param - from: the state the edge departs from
      /// @param - sym: the symbol labeling the edge
      /// @param - to: the state the edge arrives to  
      /// @return false if the internal transition already exists in the NWA
      bool addInternalTrans( State from, Symbol sym, State to );

      /// @brief Adds a internal transition to the NWA.
      ///
      /// If the internal transition is already present, this function is a
      /// no-op (other than the return value). The return value indicates
      /// whether the transition was successfully added.
      ///
      /// @param - ct: the internal transition to add
      /// @return false if the internal transition already exists in the NWA
      bool addInternalTrans( Internal & ct );

      /// @brief Removes the given internal transition from the NWA.
      ///
      /// Removes the specified internal transition from the NWA, if
      /// present. If it was not present, this function is a no-op (apart
      /// from the return value). The return value indicates whether the
      /// transition was successfully removed.
      ///
      /// @param - from: the state the edge departs from
      /// @param - sym: the symbol labeling the edge
      /// @param - to: the state the edge arrives to  
      /// @return false if the internal transition does not exist in the NWA
      bool removeInternalTrans( State from, Symbol sym, State to );

      /// @brief Removes the given internal transition from the NWA.
      ///
      /// Removes the specified internal transition from the NWA, if
      /// present. If it was not present, this function is a no-op (apart
      /// from the return value). The return value indicates whether the
      /// transition was successfully removed.
      ///
      /// @param - ct: the internal transition to remove
      /// @return false if the internal transition does not exist in the NWA
      bool removeInternalTrans( const Internal & ct );

      /// @brief Returns the number of internal transitions in this NWA
      ///
      /// @return the number of internal transitions in this NWA
      size_t sizeInternalTrans( ) const;

      // }}}


      // {{{ Query and Mutate Call Transitions

      /// @brief Adds a call transition to the NWA.
      ///
      /// If the call transition is already present, this function is a no-op
      /// (other than the return value). The return value indicates whether
      /// the transition was successfully added.
      ///
      /// Note: 'sym' cannot be WALI_EPSILON.
      ///
      /// @param - call: the state the edge departs from
      /// @param - sym: the symbol labeling the edge
      /// @param - entry: the state the edge arrives to  
      /// @return false if the call transition already exists in the NWA
      bool addCallTrans( State call, Symbol sym, State entry );

      /// @brief Adds a call transition to the NWA.
      ///
      /// If the call transition is already present, this function is a no-op
      /// (other than the return value). The return value indicates whether
      /// the transition was successfully added.
      ///
      /// @param - ct: the call transition to add
      /// @return false if the call transition already exists in the NWA
      bool addCallTrans( Call & ct );

      /// @brief Removes the given call transition from the NWA.
      ///
      /// Removes the specified call transition from the NWA, if present. If
      /// it was not present, this function is a no-op (apart from the return
      /// value). The return value indicates whether the transition was
      /// successfully removed.
      ///
      /// @param - call: the state the edge departs from
      /// @param - sym: the symbol labeling the edge
      /// @param - entry: the state the edge arrives to  
      /// @return false if the call transition does not exist in the NWA
      bool removeCallTrans( State call, Symbol sym, State entry );

      /// @brief Removes the given call transition from the NWA.
      ///
      /// Removes the specified call transition from the NWA, if present. If
      /// it was not present, this function is a no-op (apart from the return
      /// value). The return value indicates whether the transition was
      /// successfully removed.
      ///
      /// @param - ct: the call transition to remove
      /// @return false if the call transition does not exist in the NWA
      bool removeCallTrans( const Call & ct );

      /// @brief Returns the number of call transitions in this NWA
      ///
      /// @return the number of call transitions in this NWA
      size_t sizeCallTrans( ) const;

      // }}}

      // {{{ Query and Mutate Return Transitions

      /// @brief Adds a return transition to the NWA.
      ///
      /// If the return transition is already present, this function is a
      /// no-op (other than the return value). The return value indicates
      /// whether the transition was successfully added.
      ///
      /// Note: 'sym' cannot be WALI_EPSILON.
      ///
      /// @param - exit: the state the edge departs from
      /// @param - sym: the symbol labeling the edge
      /// @param - ret: the state the edge arrives to  
      /// @return false if the return transition already exists in the NWA
      bool addReturnTrans( State exit, State pred, Symbol sym, State ret );

      /// @brief Adds a return transition to the NWA.
      ///
      /// If the return transition is already present, this function is a
      /// no-op (other than the return value). The return value indicates
      /// whether the transition was successfully added.
      ///
      /// @param - ct: the return transition to add
      /// @return false if the return transition already exists in the NWA
      bool addReturnTrans( Return & ct );

      /// @brief Removes (possibly multiple) matching return transition from
      ///        the NWA.
      ///
      /// Removes any return transition in the NWA with the given 'exit'
      /// state, symbol, and 'ret'urn state. If none were present, this
      /// function is a no-op (apart from the return value). The return value
      /// indicates whether any transition was successfully removed.
      ///
      /// @param - exit: the state the edge departs from
      /// @param - sym: the symbol labeling the edge
      /// @param - to: the state the edge arrives to  
      /// @return false if the return transition does not exist in the NWA
      bool removeReturnTrans( State exit, Symbol sym, State ret );

      /// @brief Removes the given return transition from the NWA.
      ///
      /// Removes the specified return transition from the NWA, if
      /// present. If it was not present, this function is a no-op (apart
      /// from the return value). The return value indicates whether the
      /// transition was successfully removed.
      ///
      /// @param - exit: the state the edge departs from
      /// @param - sym: the symbol labeling the edge
      /// @param - to: the state the edge arrives to  
      /// @return false if the return transition does not exist in the NWA
      bool removeReturnTrans( State exit, State pred, Symbol sym, State ret );

      /// @brief Removes the given return transition from the NWA.
      ///
      /// Removes the specified return transition from the NWA, if
      /// present. If it was not present, this function is a no-op (apart
      /// from the return value). The return value indicates whether the
      /// transition was successfully removed.
      ///
      /// @param - ct: the return transition to remove
      /// @return false if the return transition does not exist in the NWA
      bool removeReturnTrans( const Return & ct );

      /// @brief Returns the number of return transitions in this NWA
      ///
      /// @return the number of return transitions in this NWA
      size_t sizeReturnTrans( ) const;

      // }}}

      

      //Building NWAs


      /***
       * @brief constructs an NWA which is the projection of the given NWA to the states
       * provided
       */
      void projectStates(NWA const & first, StateSet const & prjStates);


      
      //TODO: write comments
      virtual bool isTransitionPossible( const State &src, const Symbol &sym, const State &tgt);

      

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


      // {{{ Deprecated construction functions (& private cheater functions)

      void unionNWA( NWA const & first, NWA const & second ) {
        construct::unionNWA(*this, first, second);
      }
      static NWARefPtr unionNWA( NWA const & first, NWA const & second, State stuck ) {
        (void) stuck;
        return construct::unionNWA(first, second);
      }


      void concat( NWA const & first, NWA const & second ) {
        construct::concatenate(*this, first, second);
      }
      static NWARefPtr concat( NWA const & first, NWA const & second, State stuck ) {
        (void) stuck;
        return construct::concatenate(first, second);
      }


      void reverse( NWA const & first ) {
        construct::reverse(*this, first);
      }
      static NWARefPtr reverse( NWA const & first, State stuck ) {
        (void) stuck;
        return construct::reverse(first);
      }

      void _private_star_( NWA const & first );
      void star( NWA const & first ) {
        construct::star(*this, first);
      }
      static NWARefPtr star( NWA const & first, State stuck ) {
        (void) stuck;
        return construct::star(first);
      }


      void complement( NWA const & first ) {
        construct::complement(*this, first);
      }
      static NWARefPtr complement( NWA const & first, State stuck ) {
        (void) stuck;
        return construct::complement(first);
      }

      void _private_determinize_( NWA const & nondet );
      void determinize( NWA const & nondet ) {
        construct::determinize(*this, nondet);
      }
      static NWARefPtr determinize( NWA const & nondet, State stuck ) {
        (void) stuck;
        return construct::determinize(nondet);
      }

      void _private_intersect_( NWA const & first, NWA const & second );
      void intersect( NWA const & first, NWA const & second ) {
        construct::intersect(*this, first, second);
      }
      static NWARefPtr intersect( NWA const & first, NWA const & second, State stuck ) {
        (void) stuck;
        return construct::intersect(first, second);
      }

      // }}}
      

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
       * @brief determines whether there is any overlap in the states of the given NWAs
       *
       * This method tests whether there is any overlap in the states of the given NWAs.
       *
       *   first->states()  intersect  second->states()  = {]
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
        else {
          return true;
        }
      }



      // {{{ intersection callbacks
      
      /**
       * @brief Intersects client information for the target of a call
       *        transition.
       *
       * This method intersects the client information associated with the states 'entry1'
       * and 'entry2' given that the transition that is being created is a call transition 
       * with the given symbol using the information in the given states and sets the 
       * resulting client information.
       *
       * Note: This method should only be used to intersect client information for states 
       *        immediately following a call transition.
       *
       * This function is called during the exceution of
       * 'construct::intersect'. (This is an instance of the "template
       * method" design pattern.)
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
       */
      virtual void intersectClientInfoCall( NWA const & first, State call1, State entry1, 
                                            NWA const & second, State call2, State entry2, 
                                            Symbol resSym, State resSt );  

      /**
       * 
       * @brief Intersects client information for the target of an internal
       *        transition.
       *
       * This method intersects the client information associated with the states 'tgt1' and
       * 'tgt2' given that the transition that is being created is an internal transition 
       * with the given symbol using the information in the given states and returns the 
       * resulting client information.
       * Note: This method should only be used to intersect client information for states 
       *        immediately following an internal transition.
       *
       * This function is called during the exceution of
       * 'construct::intersect'. (This is an instance of the "template
       * method" design pattern.)
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
       * @brief Intersects client information for the target of a return
       *        transition.
       *
       * This method intersects the client information associated with the states 'ret1' and
       * 'ret2' given that the transition that is being created is a return transition with
       * the given symbol using the information in the given states and returns the 
       * resulting client information.
       * Note: This method should only be used to intersect client information for states 
       *        immediately following a return transition.
       *
       * This function is called during the exceution of
       * 'construct::intersect'. (This is an instance of the "template
       * method" design pattern.)
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
       * @brief Intersect states
       * 
       * This method determines whether the given states can be intersected and returns the result
       * in the reference parameter 'resSt'.
       *
       * This function is called during the exceution of
       * 'construct::intersect'. (This is an instance of the "template
       * method" design pattern.)
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
       * @brief Intersect symbols
       * 
       * This method performs the intersection of the given symbols and returns the result
       * in the reference parameter 'resSym'.
       *
       * This function is called during the exceution of
       * 'construct::intersect'. (This is an instance of the "template
       * method" design pattern.)
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

      // }}}
      
      // {{{ determinization callbacks

      /**
       * 
       * @brief Merges clientInfo for determinize
       * 
       * This method merges the client info for the given states and returns the result in the 
       * reference parameter 'resCI'.
       *
       * This function is called during the exceution of
       * 'construct::determinize'. (This is an instance of the "template
       * method" design pattern.)
       *
       * @param - nwa: the NWA in which to look up information about the states
       * @param - binRel: the states to merge
       * @param - resSt: the state resulting from the merge
       * @param - resCI: the client info that results from performing the merge
       *
       */
      virtual void mergeClientInfo( NWA const & nwa, 
                                    BinaryRelation const & binRel, 
                                    State resSt, ClientInfoRefPtr & resCI );

      /**
       * 
       * @brief Merges clientInfo for determinize for the target of a call
       *        transition.
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
                                        BinaryRelation const & binRelCall, 
                                        BinaryRelation const & binRelEntry,
                                        State callSt, Symbol resSym, State resSt, ClientInfoRefPtr & resCI );

      /**
       * 
       * @brief Merges clientInfo for determinize for the target of an
       *        internal transition.
       * 
       * This method merges the client info for the given target states given that the transition
       * that is being created is an internal transition with the given symbol using the information in 
       * the given states and returns the result in the reference parameter 'resCI'.
       *
       * This function is called during the exceution of
       * 'construct::determinize'. (This is an instance of the "template
       * method" design pattern.)
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
                                            BinaryRelation const & binRelSource, 
                                            BinaryRelation const & binRelTarget,
                                            State sourceSt, Symbol resSym, State resSt, ClientInfoRefPtr & resCI );

      /**
       * 
       * @brief Merges clientInfo for determinize for the target of a return
       *        transition.
       * 
       * This method merges the client info for the given return states given that the transition
       * that is being created is a return transition with the given symbol using the information in 
       * the given states and returns the result in the reference parameter 'resCI'.
       *
       * This function is called during the exceution of
       * 'construct::determinize'. (This is an instance of the "template
       * method" design pattern.)
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
                                          BinaryRelation const & binRelExit,
                                          BinaryRelation const & binRelCall, 
                                          BinaryRelation const & binRelReturn,
                                          State exitSt, State callSt, Symbol resSym, State resSt, ClientInfoRefPtr & resCI );

      // }}}


      //Using NWAs

      /**
       * 
       * @brief constructs the WPDS which is the result of the explicit NWA plus WPDS 
       *        construction from Advanced Querying for Property Checking
       *
       * This method constructs the WPDS which allows WPDS reachability to be used to 
       * perform property checking using this NWA and the given WPDS.
       *
       * @param - base: the WPDS that forms the basis for the constructed WPDS
       * @return the WPDS which can be used to perform property checking using PDS 
       *          reachability
       */
      wpds::WPDS plusWPDS( const wpds::WPDS & base, State stuck ); 
      /**
       * 
       * @brief constructs the WPDS which is the result of the explicit NWA plus WPDS 
       *        construction from Advanced Querying for Property Checking
       *
       * This method constructs the WPDS which allows WPDS reachability to be used to 
       * perform property checking using this NWA and the given WPDS.
       *
       * @param - base: the WPDS that forms the basis for the constructed WPDS
       * @param - nwa: the NWA to process
       * @return the WPDS which can be used to perform property checking using PDS 
       *          reachability
       */
      static wpds::WPDS plusWPDS( const wpds::WPDS & base, NWA & nwa, State stuck )
      {
        return nwa.plusWPDS(base, stuck);
      }

      /**
       *
       * @brief constructs the NWA equivalent to the given PDS
       *
       * This method constructs the NWA that is equivalent to the given
       * PDS. The NWA's state space tracks both the PDS's state and top stack
       * symbol; the NWA's "stack" tracks the remainder of the PDS's stack.
       * The symbol on each transition is the from_stack portion of the
       * corresponding PDS rule.
       *
       *
       * Let the PDS be (P, G, D). The NWA we create is (Q, Q_0, d, Q_f); d
       * is split up into the usual (for NWAs) components d_i, d_c, and d_r.
       *  - Q = P x G    (Tracking the PDS state + top stack symbol)
       *  - d_i = { ((q,a), a, (q',a'))        |  <q, a> -> <q', a'> in D }
       *  - d_c = { ((q,a), a, (q',b))         |  <q, a> -> <q', b a'> in D }
       *  - d_r = { ((q,a), (p,b), a, (q',a')) |  <q, a> -> <q', *> in D  and
       *                                          <p, b> -> <_, _ a'> in D }
       *
       * The last rule is a little funky in a couple ways. First, it has two
       * conditions because the pop rule doesn't let you know what's on the
       * top of the stack, and we need to figure that out from the
       * corresponding push rule.
       *
       * Second, note that the push rules we look at in the condition is
       * totally unconstrained. This is because, with the exception of the
       * "revealed" stack symbol a', everything that the push rule talks
       * about concerns the call predecessor (p,b) and entry node (_,_);
       * nothing the pop rule talks about -- q, a, or q' -- has any relation
       * to those.
       *
       * This last fact means that the NWA we construct can potentially have
       * quadratically-many rules as the input PDS. If the PDS has M push
       * rules and N pop rules, the NWA will have M*N return transitions.
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
       * @param - stuck: dummy parameter
       * @return the NWA equivalent to the given PDS
       *
       */
      static NWARefPtr PDStoNWA( const wpds::WPDS & pds, State stuck )
      {
        (void) stuck;
        NWARefPtr nwa(new NWA());
        nwa->PDStoNWA(pds);
        return nwa;
      }


      // {{{ deprecated nwa_pds functions (and cheater functions)
      wpds::WPDS _private_NwaToPdsReturns_( WeightGen & wg ) const;
      wpds::WPDS NWAtoPDSreturns( WeightGen & wg ) const {
	return nwa_pds::NWAToPDSReturns(*this, wg);
      }
      static wpds::WPDS NWAtoPDSreturns( NWA const & nwa, WeightGen & wg ) {
        return nwa_pds::NWAToPDSReturns(nwa, wg);
      }

      wpds::WPDS _private_NwaToBackwardsPdsReturns_( WeightGen & wg ) const;
      wpds::WPDS NWAtoBackwardsPDSreturns( WeightGen & wg ) const {
	return nwa_pds::NWAToBackwardsPDSReturns(*this, wg);
      }
      static wpds::WPDS NWAtoBackwardsPDSreturns( NWA const & nwa, WeightGen & wg ) {
        return nwa_pds::NWAToBackwardsPDSReturns(nwa, wg);
      }


      wpds::WPDS _private_NwaToPdsCalls_( WeightGen & wg ) const;
      wpds::WPDS NWAtoPDScalls( WeightGen & wg ) const {
	return nwa_pds::NWAToPDSCalls(*this, wg);
      }
      static wpds::WPDS NWAtoPDScalls( NWA const & nwa, WeightGen & wg ) {
        return nwa_pds::NWAToPDSCalls(nwa, wg);
      }


      wpds::WPDS _private_NwaToBackwardsPdsCalls_( WeightGen & wg ) const;
      wpds::WPDS NWAtoBackwardsPDScalls( WeightGen & wg ) const {
	return nwa_pds::NWAToBackwardsPDSCalls(*this, wg);
      }
      static wpds::WPDS NWAtoBackwardsPDScalls( NWA const & nwa, WeightGen & wg ) {
        return nwa_pds::NWAToBackwardsPDSCalls(nwa, wg);
      }

      // }}}

      
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
        //Check L(a1) contained in L(a2) by checking 
        //if L(a1) intersect (complement L(a2)) is empty.
        NWA comp;
        comp.complement(second);   //complement L(a2)
        NWA inter;
        inter.intersect(first,comp); //L(a1) intersect (complement L(a2))

        return inter.isEmpty();
      }


      /**
       *
       * @brief tests whether the languages of the given NWAs are equal
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
       * @brief add the given symbols to the NWA
       *
       * This method adds all of the given symbols to the set of symbols associated with
       * the NWA.  
       *
       * @param - addSymbolSet: the symbols to add
       *
       */
      void addAllSymbols( SymbolStorage addSymbolSet );    


      // {{{ Iterator functions
      
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
      StateIterator beginStates( ) const;  

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
      StateIterator endStates( ) const;  

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
      StateIterator beginInitialStates( ) const;  

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
      StateIterator endInitialStates( ) const;  


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
      StateIterator beginFinalStates( ) const;  

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
      StateIterator endFinalStates( ) const;  

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
      SymbolIterator beginSymbols( ) const;  

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
      SymbolIterator endSymbols( ) const;  

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

      // }}}

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
      State makeKey(  BinaryRelation const & R ) const;

      //
      // Variables
      //
      
    protected:
      
      StateStorage states;         
      SymbolStorage symbols;        
      Trans trans;

      //TODO: ponder the following ...
      //Q: should we incrementally maintain a wpds?
      //    if we did, what would the weight gen of the wpds be?

    public:
      /// @brief Determines whether word is in the language of the given NWA.
      ///
      /// @returns true if 'word' is in L(this), and false otherwise.
      bool
      isMemberNondet( ::wali::nwa::NestedWord const & word ) const;


      /// @brief Adds all states, symbols, and transitions from 'rhs' to 'this'.
      ///
      /// This is like unionNWA(), except that it doesn't modify the initial
      /// or accepting states.
      void combineWith(NWA const & rhs);
    };


    /**
     *  
     * @brief returns the default program control location for PDSs
     *
     * This method provides access to the default program control location for PDSs.
     * 
     * @return the default program control location for PDSs
     *
     */
    inline
    wali::Key
    getProgramControlLocation( )
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
    inline
    wali::Key
    getControlLocation( Key exit, Key callSite, Key returnSite )
    {
      std::stringstream ss;
      ss << "(key#"  << exit << "," << callSite << "," << returnSite << ")";
      wali::Key key = getKey(getProgramControlLocation(), getKey(ss.str()));
      return key;
    }

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

