#ifndef wali_nwa_TransSet_GUARD
#define wali_nwa_TransSet_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/nwa/StateSet.hpp"

// std::c++
#include <iostream>
#include <set>

namespace wali
{
  namespace nwa
  {

    template <typename R, typename S, typename T, typename U, typename V>
    class TransSet : public Printable
    {
      public:
        typedef typename StateSet<R>::iterator stateIterator;
        typedef typename S::iterator symbolIterator;
        typedef typename T::iterator callIterator;
        typedef typename U::iterator internalIterator;
        typedef typename V::iterator returnIterator;
      
      //
      // Methods
      //

      public:
        //Constructors and Destructor
        TransSet( );
        TransSet( const TransSet & other );
        TransSet & operator=( const TransSet & other );

        ~TransSet( );


        //State Accessors

        /**
         *
         * @brief get all states in the state set of the NWA
         *
         * This method provides access to all states in the state set of
         * the NWA.  Note: An NWA can have an unbounded number of states 
         * associated with it.
         *
         * @return set of Key values for all states associated with the NWA
         *
         */
        const StateSet<R> & getStates( );

        /**
         *
         * @brief test if a state with the given name is a state of the NWA
         *
         * This method tests whether the state with the given name is in the 
         * state set of the NWA.
         *
         * @param name of the state to check
         * @return true if the state with the given name is a state of the NWA,
         * false otherwise
         *
         */
        bool isState( Key name );

        /**
         *
         * @brief add a state with the given name to the NWA
         *
         * This method adds a state with the given name to the state set for 
         * the NWA.  If the state already exists in the NWA, false is 
         * returned.  Otherwise, true is returned.  Note: For bookkeping
         * purposes a transition of each kind will be added to the transition
         * set from the new state to the error state for each symbol currently
         * in the symbol set (for each state currently in the state set -including
         * the new state- as the pred state and each symbol currently in the 
         * symbol set as the symbol for return transitions).
         *
         * @param name of the state to add
         * @return false if the state already exists in the NWA
         *
         */
        bool addState( Key name );
      
        /**
         *
         * @brief add all the states in the given transset to the NWA
         *
         * This method adds all of the given states to the state set for 
         * the NWA.
         *
         * @param the transset that contains the states to add
         *
         */
        void addAllStates( TransSet addTransSet);

        /**
         *
         * @brief remove the state with the given name from the NWA
         *
         * This method checks for the state with the given name in the 
         * initial state set, the final state set, and the NWA state set.
         * It then removes the state from any state set that contained it.
         * Any transitions to or from the state to be removed are also
         * removed.
         *
         * @param name of the state to remove
         * @return false if the state does not exist in the NWA
         *
         */
        bool removeState( Key name );

        /**
         *
         * @brief remove all states from the NWA
         *
         * This method removes all states from the NWA except the error state.
         * All transitions associated with removed states are also removed 
         * from the NWA.
         *
         */
        void clearStates( );

        /**
         *
         * @brief provide access to the beginning of the state set
         *
         * This method provides access to the beginning of the state set
         * associated with this transition set.
         *
         * @return an iterator pointing to the beginning of the state set
         *
         */
        stateIterator beginStates();
        
        /**
         * 
         * @brief provide access to the end of the state set
         *
         * This method provides access to the position one past the end
         * of the state set associated with this transition set.
         *
         * @return an iterator pointing just past the end of the state set
         *
         */
        stateIterator endStates();

        /**
         *
         * @brief get all states in the initial state set
         *
         * This method provides access to all states in the initial state 
         * set of this NWA.  Note: An NWA can have an unbounded number of 
         * inital states associated with it.  
         *
         * @return set of Key values for all inital states associated with 
         * the NWA
         *
         */
        const StateSet<R> & getInitialStates( );

        /**
         *
         * @brief test if a state with the given name is an initial state
         *
         * This method tests whether the state with the given name is in the 
         * initial state set of the NWA.
         *
         * @param name of the state to check
         * @return true if the state with the given name is an initial state
         *
         */
        bool isInitialState( Key name );

        /**
         *
         * @brief make the state with the given name an initial state
         *
         * This method checks for a state with the given name in the state 
         * set of the NWA.  If the state does not exist, a state with the 
         * given name is added to the state set of the NWA.  The state with 
         * the given name is then added to the set of initial states for the 
         * NWA.  If the state already exists in the intial state set of the 
         * NWA, false is returned.  Otherwise, true is returned.
         *
         * @param name of the state to add to initial state set
         * @return false if the state already exists in the initial state set
         * of the NWA
         *
         */
        bool addInitialState( Key name );

        /**
         *
         * @brief adds all of the initial states in the given trans set to
         * the initial state set associated with this trans set
         *
         * This method adds all of the initial states associated with the 
         * given trans set to the initial state set associated with this
         * trans set.
         *
         * @parm the trans set whose initial states to add to this trans 
         * set's initial state set
         *
         */
        void addAllInitialStates( TransSet addTransSet );

        /**
         *
         * @brief remove the state with the given name from the initial state 
         * set of the NWA
         *
         * This method checks for a state with the given name in the initial
         * state set of the NWA.  If the state exists, it is removed from the
         * initial state set (but not from the set of all states of the NWA). 
         * This method does not remove any transitions from the NWA.
         *
         * @param name of the state to remove from the initial state set
         * @return false if the state does not exist in the initial state set 
         * of the NWA
         *
         */
        bool removeInitialState( Key name );

        /**
         *
         * @brief remove all states from the initial state set of the NWA
         *
         * This method removes all states from the initial state set of the
         * NWA, but does not remove the them from the set of all states of the
         * NWA.  This method does not remove any transitions from the NWA.
         *
         */
        void clearInitialStates( );

        /**
         *
         * @brief provide access to the beginning of the initial state set
         *
         * This method provides access to the beginning of the initial state 
         * set associated with this transition set.
         *
         * @return an iterator pointing to the beginning of the initial state
         *  set
         *
         */
        stateIterator beginInitialStates();
        
        /**
         * 
         * @brief provide access to the end of the initial state set
         *
         * This method provides access to the position one past the end
         * of the initial state set associated with this transition set.
         *
         * @return an iterator pointing just past the end of the initial 
         * state set
         *
         */
        stateIterator endInitialStates();

        /**
         *
         * @brief get all final states
         *
         * This method provides access to all states in the final state 
         * set of this NWA.  Note: An NWA can have an unbounded number of 
         * final states associated with it.
         *
         * @return set of Key values for all final states associated with 
         * the NWA
         *
         */
        StateSet<R> & getFinalStates( );

        /**
         *
         * @brief test if a state with the given name is a final state
         *
         * This method tests whether the state with the given name is in the 
         * final state set of the NWA.
         *
         * @param name of the state to check
         * @return true if the state with the given name is a final state
         *
         */
        bool isFinalState( Key name );

        /**
         *
         * @brief make the state with the given name a final state
         *
         * This method checks for a state with the given name in the state 
         * set of the NWA.  If the state does not exist, a state with the 
         * given name is added to the state set of the NWA.  The state with 
         * the given name is then added to the set of final states for the 
         * NWA.  If the state already exists in the final state set of the 
         * NWA, false is returned.  Otherwise, true is returned.
         *
         * @param name of the state to add to final state set
         * @return false if the state already exists in the final state set
         * of the NWA
         *
         */
        bool addFinalState( Key name );
      
        /**
         *
         * @brief adds all of the final states in the given trans set to
         * the final state set associated with this trans set
         *
         * This method adds all of the final states associated with the 
         * given trans set to the final state set associated with this
         * trans set.
         *
         * @parm the trans set whose final states to add to this trans 
         * set's final state set
         *
         */
        void addAllFinalStates( TransSet addTransSet );

        /**
         *
         * @brief remove the state with the given name from the final state 
         * set of the NWA
         *
         * This method checks for a state with the given name in the final
         * state set of the NWA.  If the state exists, it is removed from the
         * final state set (but not from the set of all states of the NWA). 
         * This method does not remove any transitions from the NWA.
         *
         * @param name of the state to remove from the final state set
         * @return false if the state does not exist in the final state set 
         * of the NWA
         *
         */
        bool removeFinalState( Key name );

        /**
         *
         * @brief remove all states from the final state set of the NWA
         *
         * This method removes all States from the final state set of the
         * NWA, but does not remove the them from the set of all states of the
         * NWA.  This method does not remove any transitions from the NWA.
         *
         */
        void clearFinalStates( );

        /**
         *
         * @brief provide access to the beginning of the final state set
         *
         * This method provides access to the beginning of the final state 
         * set associated with this transition set.
         *
         * @return an iterator pointing to the beginning of the final state
         *  set
         *
         */
        stateIterator beginFinalStates();
        
        /**
         * 
         * @brief provide access to the end of the final state set
         *
         * This method provides access to the position one past the end
         * of the final state set associated with this transition set.
         *
         * @return an iterator pointing just past the end of the final 
         * state set
         *
         */
        stateIterator endFinalStates();

        /**
         *
         * @brief returns the number of states associated with this NWA
         *
         * This method returns the number of states associated with this
         * NWA.  Note: The error state is included in this count.
         *
         * @return the number of states associated with this NWA
         *
         */
        size_t sizeStates( );
        
        /**
         *
         * @brief returns the number of initial states associated with this NWA
         *
         * This method returns the number of initial states associated with 
         * this NWA.
         *
         * @return the number of initial states associated with this NWA
         *
         */
        size_t sizeInitialStates( );
        
        /**
         *
         * @brief returns the number of final states associated with this NWA
         *
         * This method returns the number of final states associated with this
         * NWA.
         *
         * @return the number of final states associated with this NWA
         *
         */
        size_t sizeFinalStates( );

        //Symbol Accessors

        /**
         *
         * @brief get all symbols in the symbol set associated with the NWA
         *
         * This method provides access to all symbols in the symbol set 
         * associated with the NWA.  Note: An NWA can have an unbounded number 
         * of symbols associated with it.
         *
         * @return set of all symbols associated with the NWA
         *
         */
        const S & getSymbols( );

        /**
         *
         * @brief test if the given symbol is associated with the NWA
         *
         * This method tests whether the given symbol is in the symbol set
         * associated with the NWA.
         *
         * @param the symbol to check
         * @return true if the given symbol is associated with the NWA
         *
         */
        bool isSymbol( Key sym );

        /**
         *
         * @brief add the given symbol to the NWA
         *
         * This method adds the given symbol to the symbol set associated with 
         * the NWA.  If the symbol is already associated with the NWA, false is 
         * returned. Otherwise, true is returned. Note: For bookkeeping purposes
         * a transition of each kind is added to the to the transition set for 
         * each state currently in the state set as the from state (for each pair
         * of states currently in the state set as the from state and pred state
         * in the case of return transitions) with the new symbol and the error
         * state as the to state. 
         *
         * @param the symbol to add
         * @return false if the symbol is already associated with the NWA, 
         * true otherwise
         *
         */
        bool addSymbol( Key sym );

        /**
         *
         * @brief add the given symbols to the NWA
         *
         * This method adds all of the given symbols to the symbol set 
         * associated with the NWA.  Transitions of all kinds(call, internal, 
         * and return) with the given symbol for each pair(or triple) of 
         * states to the error state are added to the respective transition 
         * sets.  
         *
         * @param the symbols to add
         *
         */
        void addAllSymbols( S syms );

        /**
         *
         * @brief remove the given symbol from the NWA
         *
         * This method checks for the given symbol in the symbol set 
         * associated with the NWA.  It then removes the symbol from the
         * symbol set.  Any transitions associated with the symbol to be 
         * removed are also removed.
         *
         * @param the symbol to remove
         * @return false if the symbols is not associated with the NWA
         *
         */
        bool removeSymbol( Key sym );

        /**
         *
         * @brief remove all symbols associated with the NWA
         *
         * This method removes all symbols associated with the NWA except 
         * the epsilon symbol.  It also removes all transitions associated 
         * with the removed symbols in the NWA.
         *
         */
        void clearSymbols( );

        /**
         *
         * @brief provide access to the beginning of the symbol set
         *
         * This method provides access to the beginning of the symbol set
         * associated with this transition set.
         *
         * @return an iterator pointing to the beginning of the symbol set
         *
         */
        symbolIterator beginSymbols();
        
        /**
         * 
         * @brief provide access to the end of the symbol set
         *
         * This method provides access to the position one past the end
         * of the symbol set associated with this transition set.
         *
         * @return an iterator pointing just past the end of the symbol set
         *
         */
        symbolIterator endSymbols();
        
        /**
         *
         * @brief returns the number of symbols associated with this NWA
         *
         * This method returns the number of symbols associated with this
         * NWA.  Note: The epsilon symbol is included in this count.
         *
         * @return the number of symbols associated with this NWA
         *
         */
        size_t sizeSymbols( );

        //Transition Accessors

        /**
         *
         * @brief removes all transitions from this collection of transitions
         *
         * This method removes all transitions from this collection of
         * transitions by redirecting all existing transitions to point to the 
         * error state.
         *
         */
        void clear( );
      
        /**
         *
         * @brief get all call transitions in the collection of transitions associated 
         * with the NWA
         *
         * This method provides access to all call transitions in the collection of
         * transitions associated with the NWA.  
         *
         * @return all call transitions in the collection of transitions associated 
         * with the NWA
         *
         */
        const T & getCalls();
        
        /**
         *
         * @brief get all internal transitions in the collection of transitions 
         * associated with the NWA
         *
         * This method provides access to all internal transitions in the collection 
         * of transitions associated with the NWA.  
         *
         * @return all internal transitions in the collection of transitions associated 
         * with the NWA
         *
         */
        const U & getInternals();
        
        /**
         *
         * @brief get all return transitions in the collection of transitions 
         * associated with the NWA
         *
         * This method provides access to all return transitions in the collection 
         * of transitions associated with the NWA.  
         *
         * @return all return transitions in the collection of transitions associated 
         * with the NWA
         *
         */
        const V & getReturns();
      
        /**
         *
         * @brief add the given call transition to the collection of transitions
         *
         * This method adds the given call transition to the collection of 
         * transitions associated with the NWA.  If the call transition with the
         * same from state and symbol but the error state as the to state exists 
         * in the collection of transitions, it is removed.  If this exact call 
         * transition already exists, false is returned. Otherwise, true is returned.
         *
         * @parm the call transition to add to the collection of transitions
         * @return false if the call transition already exists in the collection
         *
         */
        bool addCall(KeyTriple addTrans);
        
        /**
         *
         * @brief add the given internal transition to the collection of transitions
         *
         * This method adds the given internal transition to the collection of 
         * transitions associated with the NWA.  If the internal transition with the
         * same from state and symbol but the error state as the to state exists in 
         * the collection of transitions, it is removed.  If this exact internal 
         * transition already exists, false is returned. Otherwise, true is returned.
         *
         * @parm the internal transition to add to the collection of transitions
         * @return false if the internal transition already exists in the collection
         *
         */
        bool addInternal(KeyTriple addTrans);
        
        /**
         *
         * @brief add the given return transition to the collection of transitions
         *
         * This method adds the given return transition to the collection of 
         * transitions associated with the NWA.  If the return transition with the 
         * same from state, pred state, and symbol but the error state as the to 
         * state exists in the collection of transitions, it is removed.  If this 
         * exact return transition already exists, false is returned. Otherwise, 
         * true is returned.
         *
         * @parm the return transition to add to the collection of transitions
         * @return false if the return transition already exists in the collection
         *
         */
        bool addReturn(KeyQuad addTrans);
      
        /**
         *
         * @brief add all transitions in the given collection to this
         * collection of transitions
         *
         * This method adds all of the transitions in the given collection
         * of transitions to this collection of transitions.
         *
         * @parm the collection of transitions to add to this collection
         * of transitions
         *
         */
        void addAllTrans(TransSet addTransSet);
      
        /**
         *
         * @brief remove the given call transition from the collection of 
         * transitions
         *
         * This method removes the given call transition from the collection 
         * of transitions.  If removing this transition leaves the NWA with no
         * call transitions corresponding to the from state and symbol of the 
         * removed transition, a call transition with the same from state and 
         * symbol as the removed call transition and the error state as the to 
         * state is added to the NWA.  If the given call transition does not 
         * exist in the collection of transitions false is returned.  Otherwise,
         * true is returned.
         *
         * @parm the call transition to remove from the collection
         * @return false if the given call transition does not exist in the 
         * collection, true otherwise.
         *
         */
        bool removeCall(KeyTriple removeTrans);
        
        /**
         *
         * @brief remove the given internal transition from the collection of 
         * transitions
         *
         * This method removes the given internal transition from the collection 
         * of transitions.  If removing this transition leaves the NWA with no 
         * internal transitions corresponding to the from state and symbol of the
         * removed transition, an internal transition with the same from state and
         * symbol as the removed internal transition and the error state as the to
         * state is added to the NWA.  If the given internal transition does not 
         * exist in the collection of transitions false is returned.  Otherwise,
         * true is returned.
         *
         * @parm the internal transition to remove from the collection
         * @return false if the given internal transition does not exist in the 
         * collection, true otherwise.
         *
         */
        bool removeInternal(KeyTriple removeTrans);
        
        /**
         *
         * @brief remove the given return transition from the collection of 
         * transitions
         *
         * This method removes the given return transition from the collection 
         * of transitions.  If removing this transition leaves the NWA with no 
         * return transitions corresponding to the from state, pred state, and 
         * symbol of the removed transition, a return transition with the same 
         * from state, pred state, and symbol as the removed return transition 
         * and the error state as the to state is added to the NWA.  If the 
         * given return transition does not exist in the collection of transitions 
         * false is returned.  Otherwise, true is returned.
         *
         * @parm the return transition to remove from the collection
         * @return false if the given return transition does not exist in the 
         * collection, true otherwise.
         *
         */
        bool removeReturn(KeyQuad removeTrans);
                      
        /**
         *
         * @brief test if the given call transition is in the collection of 
         * transitions associated with the NWA
         *
         * This method tests whether the given call transition is in the collection
         * of transitions associated with the NWA.
         *
         * @param the call transition to check
         * @return true if the given call transition is in the collection of 
         * transitions associated with the NWA
         *
         */
        bool isCall(KeyTriple trans);
        
        /**
         *
         * @brief test if the given internal transition is in the collection of 
         * transitions associated with the NWA
         *
         * This method tests whether the given internal transition is in the 
         * collection of transitions associated with the NWA.
         *
         * @param the internal transition to check
         * @return true if the given internal transition is in the collection of 
         * transitions associated with the NWA
         *
         */
        bool isInternal(KeyTriple trans);
        
        /**
         *
         * @brief test if the given return transition is in the collection of 
         * transitions associated with the NWA
         *
         * This method tests whether the given return transition is in the 
         * collection of transitions associated with the NWA.
         *
         * @param the return transition to check
         * @return true if the given return transition is in the collection of 
         * transitions associated with the NWA
         *
         */
        bool isReturn(KeyQuad trans);              
                      
        //Utilities	

        /**
         *
         * @brief print the collection of transitions
         *
         * This method prints out the transition set to the output stream 
         * provided.
         *
         * @param the output stream to print to
         * @return the output stream that was printed to
         *
         */
        std::ostream & print( std::ostream & o ) const;

        /**
         *
         * @brief tests whether this collection of transitions is equivalent 
         * to the collection of transitions 'other'
         *
         * This method tests the equivalence of this set of transitions and 
         * the set of transitions 'other'.
         *
         * @param the TransSet to compare this TransSet to
         * @return true if this TransSet is equivalent to the TransSet 'other'
         *
         */
        bool operator==( TransSet & other );

        /**
         *
         * @brief provides access to the call transitions in the collection 
         * through an iterator
         *
         * This method provides access to the call transitions in the collection
         * of transitions through an iterator.
         *
         * @return the starting point of an iterator through the call transitions
         * in the collection of transitions
         *
         */
        callIterator beginCall( );
        
        /**
         *
         * @brief provides access to the internal transitions in the collection 
         * through an iterator
         *
         * This method provides access to the internal transitions in the collection
         * of transitions through an iterator.
         *
         * @return the starting point of an iterator through the internal transitions
         * in the collection of transitions
         *
         */
        internalIterator beginInternal( );
        
        /**
         *
         * @brief provides access to the return transitions in the collection 
         * through an iterator
         *
         * This method provides access to the return transitions in the collection
         * of transitions through an iterator.
         *
         * @return the starting point of an iterator through the return transitions
         * in the collection of transitions
         *
         */
        returnIterator beginReturn( );
      
        /**
         *
         * @brief provides access to the call transitions in the collection 
         * through an iterator
         *
         * This method provides access to the call transitions in the collection
         * of transitions through an iterator.
         *
         * @return the exit point of an iterator through the call transitions in
         * the collection of transitions
         *
         */
         callIterator endCall( );
         
         /**
         *
         * @brief provides access to the internal transitions in the collection 
         * through an iterator
         *
         * This method provides access to the internal transitions in the collection
         * of transitions through an iterator.
         *
         * @return the exit point of an iterator through the internal transitions in
         * the collection of transitions
         *
         */
         internalIterator endInternal( );
         
         /**
         *
         * @brief provides access to the return transitions in the collection 
         * through an iterator
         *
         * This method provides access to the return transitions in the collection
         * of transitions through an iterator.
         *
         * @return the exit point of an iterator through the return transitions in
         * the collection of transitions
         *
         */
         returnIterator endReturn( );

        /**
         *
         * @brief returns the number of call transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the number of call transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not 
         * include any transition to or from the error state. 
         *
         * @return the number of call transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t sizeCall( );
        
        /**
         *
         * @brief returns the number of internal transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the number of internal transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not include
         * any transition to or from the error state.
         *
         * @return the number of internal transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t sizeInternal( );
        
        /**
         *
         * @brief returns the number of return transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the number of return transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not include
         * any transition to or from the error state or with the error state as the
         * pred state.
         *
         * @return the number of return transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t sizeReturn( );
        
        /**
         *
         * @brief returns the total number of transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the total number of transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not 
         * include any transitions involving the error state.  This is equivalent 
         * to counting all transitions explicitly added to the NWA.
         *
         * @return the total number of transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t size( );

      private:
        /** 
         *
         * @brief removes all call transitions to or from the state with 
         * the given name  
         *
         * This method removes all call transitions to or from the state 
         * with the given name. If no call transitions exist to or from 
         * this state false is returned.  Otherwise, true is returned.
         *
         * @parm the name of the state whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeCallTransWith( Key name );
      
        /** 
         *
         * @brief removes all internal transitions to or from the state
         * with the given name
         *
         * This method removes all internal transitions to or from the
         * state with the given name.  If no internal transitions exist
         * to or from this state false is returned.  Otherwise, true is 
         * returned. 
         *
         * @parm the name of the state whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeInternalTransWith( Key name );
      
        /** 
         *
         * @brief removes all return transitions to or from the state with
         * the given name as well as return transitions corresponding to 
         * calls from the state with the given name
         *
         * This method removes all return transitions to or from the state
         * with the given name as well as return transitions corresponding
         * to calls from the state with the given name.  If no return 
         * transitions exist to or from this state false is returned.  
         * Otherwise, true is returned.
         *
         * @parm the name of the state whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeReturnTransWith( Key name );
      
        /** 
         *
         * @brief removes all call transitions with the given symbol 
         *
         * This method removes all call transitions with the given symbol. 
         * If no call transitions exist with the given symbol false is 
         * returned.  Otherwise, true is returned.
         *
         * @parm the name of the symbol whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeCallTransSym(Key sym);
        
        /** 
         *
         * @brief removes all internal transitions with the given symbol 
         *
         * This method removes all internal transitions with the given symbol. 
         * If no internal transitions exist with the given symbol false is 
         * returned.  Otherwise, true is returned.
         *
         * @parm the name of the symbol whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeInternalTransSym(Key sym);
        
        /** 
         *
         * @brief removes all return transitions with the given symbol 
         *
         * This method removes all return transitions with the given symbol. 
         * If no return transitions exist with the given symbol false is 
         * returned.  Otherwise, true is returned.
         *
         * @parm the name of the symbol whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeReturnTransSym(Key sym);
      
        /**
         *
         * @brief test if there exists a call transition with the given from state 
         * and symbol in the collection of transitions associated with the NWA
         *
         * This method tests whether there exists a call transition with the given 
         * from state and symbol but not the error state as the to state in the 
         * collection of transitions associated with the NWA.
         *
         * @param from: the desired from state for the call transition
         * @param sym: the desired symbol for the call transition
         * @return true if there exists a call transition with the given from state and
         * symbol in the collection of transitions associated with the NWA
         *
         */
        bool callExists(Key from,Key sym);
        
        /**
         *
         * @brief test if there exists an internal transition with the given from state 
         * and symbol in the collection of transitions associated with the NWA
         *
         * This method tests whether there exists an internal transition with the given 
         * from state and symbol but not the error state as the to state in the collection 
         * of transitions associated with the NWA.
         *
         * @param from: the desired from state for the internal transition
         * @param sym: the desired symbol for the internal transition
         * @return true if there exists an internal transition with the given from state and
         * symbol in the collection of transitions associated with the NWA
         *
         */
        bool internalExists(Key from,Key sym);
        
        /**
         *
         * @brief test if there exists a return transition with the given from state, 
         * predecessor state, and symbol in the collection of transitions associated 
         * with the NWA
         *
         * This method tests whether there exists a return transition with the given 
         * from state, predecessor state, and symbol but not the error state as the to
         * state in the collection of transitions associated with the NWA.
         *
         * @param from: the desired from state for the return transition
         * @param pred: the desired predecessor state for the return transition
         * @param sym: the desired symbol for the return transition
         * @return true if there exists a return transition with the given from state and
         * symbol in the collection of transitions associated with the NWA
         *
         */
        bool returnExists(Key from, Key pred, Key sym);

      //
      // Variables
      //
      public:
        static const Key WALI_ERR_STATE = WALI_BAD_KEY; //TODO: Is this right??
      
      protected: 
        StateSet<R> states;
        StateSet<R> initialStates;
        StateSet<R> finalStates;
        S symbols;
        T callTrans;
        U internalTrans;
        V returnTrans;
    };
    
    //
    // Methods
    //

    //Constructors and Destructor
    template < typename R,typename S,typename T,typename U, typename V >
    TransSet<R,S,T,U,V>::TransSet( )
    {
      states = StateSet<R>();      
      initialStates = StateSet<R>();
      finalStates = StateSet<R>();
      
      symbols = S();
      callTrans = T();
      internalTrans = U();
      returnTrans = V();
      
      states.add(WALI_ERR_STATE);
      addSymbol(WALI_EPSILON);
    }
    
    template < typename R,typename S,typename T,typename U, typename V >
    TransSet<R,S,T,U,V>::TransSet( const TransSet<R,S,T,U,V> & other )
    {
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;
      
      symbols = other.symbols;
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
    }
    
    template < typename R,typename S,typename T,typename U, typename V >
    TransSet<R,S,T,U,V> & TransSet<R,S,T,U,V>::operator=( const TransSet<R,S,T,U,V> & other )
    {
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;
    
      symbols = other.symbols;
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      return *this;
    }

   
    template < typename R,typename S,typename T,typename U, typename V >
    TransSet<R,S,T,U,V>::~TransSet( ) { }
    
    //State Accessors

    /**
     *
     * @brief get all states in the state set of the NWA
     *
     * @return set of Key values for all states associated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    const StateSet<typename R> & TransSet<R,S,T,U,V>::getStates( )
    {
      return states;
    }
   
    /**
     *
     * @brief test if a state with the given name is a state of the NWA
     *
     * @param name of the state to check
     * @return true if the state with the given name is a state of the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::isState( Key name )
    {
      return states.contains(name);
    }
   
    /**
     *
     * @brief add a state with the given name to the NWA
     *
     * @param name of the state to add
     * @return false if the state already exists in the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::addState( Key name )
    {
      if( states.contains(name) )
        return false;
      states.add(name);
      
      //Add a transition of each kind going to the error state for 
      //each symbol in the symbol set.
      for( symbolIterator it = symbols.begin();
            it != symbols.end(); it++ )
      {
        addCall(KeyTriple(name,*it,WALI_ERR_STATE));
        addInternal(KeyTriple(name,*it,WALI_ERR_STATE));
        for( stateIterator sit = states.begin();
              sit != states.end(); sit++ )
        {
          addReturn(KeyQuad(name,*sit,*it,WALI_ERR_STATE));
          addReturn(KeyQuad(*sit,name,*it,WALI_ERR_STATE));
        }
      }
      return true;
    }


    /**
     *
     * @brief add all the states in the given transset to the NWA
     *
     * @param the transset that contains the states to add
     *
     */
    template <typename R,typename S,typename T,typename U,typename V >
    void TransSet<R,S,T,U,V>::addAllStates( TransSet<R,S,T,U,V> addTransSet)
    {
      for( stateIterator sit = addTransSet.beginStates();
            sit != addTransSet.endStates(); sit++ )
        states.add(*sit);
    }
   
    /**
     *
     * @brief remove the state with the given name from the NWA
     *
     * @param name of the state to remove
     * @return false if the state does not exist in the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeState( Key name )
    {
      //Remove transitions associated with the state that was removed.
      if( states.remove(name) )
      {
        removeCallTransWith(name);
        removeInternalTransWith(name);
        removeReturnTransWith(name);
        
        return true;
      }      
      return false;
    }
   
    /**
     *
     * @brief remove all states from the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    void TransSet<R,S,T,U,V>::clearStates( )
    {
      initialStates.clear();
      finalStates.clear();
      states.clear();

      //Since all states are being removed, all transitions are removed 
      //as well.
      callTrans.clear();
      internalTrans.clear();
      returnTrans.clear();
      
      //The error states should always remain in the state set.
      //This will also replace transitions from the error state to the 
      //error state for each symbol that exists in the symbol table.
      addState(WALI_ERR_STATE);
    }
    
    /**
     *
     * @brief provide access to the beginning of the state set
     *
     * @return an iterator pointing to the beginning of the state set
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::stateIterator TransSet<R,S,T,U,V>::beginStates()
    {
      return states.begin();
    }
    
    /**
     * 
     * @brief provide access to the end of the state set
     *
     * @return an iterator pointing just past the end of the state set
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::stateIterator TransSet<R,S,T,U,V>::endStates()
    {
      return states.end();
    }
  
    /**
     *
     * @brief get all states in the initial state set
     *
     * @return set of Key values for all inital states associated with 
     * the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    const StateSet<typename R> & TransSet<R,S,T,U,V>::getInitialStates( )
    {
      return initialStates;
    }
  
    /**
     *
     * @brief test if a state with the given name is an initial state
     *
     * @param name of the state to check
     * @return true if the state with the given name is an initial state
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::isInitialState( Key name )
    {
      return initialStates.contains(name); 
    }
  
    /**
     *
     * @brief make the state with the given name an initial state
     *
     * @param name of the state to add to initial state set
     * @return false if the state already exists in the initial state set
     * of the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::addInitialState( Key name )
    {
      if( !states.contains(name) )
        states.add(name);
      return initialStates.add(name);
    }
    
    /**
     *
     * @brief adds all of the initial states in the given trans set to
     * the initial state set associated with this trans set
     *
     * @parm the trans set whose initial states to add to this trans 
     * set's initial state set
     *
     */
    template <typename R,typename S,typename T,typename U,typename V >
    void TransSet<R,S,T,U,V>::addAllInitialStates( TransSet<R,S,T,U,V> addTransSet )
    {
      for( R::iterator it = addTransSet.initialStates.begin();
            it != addTransSet.initialStates.end(); it++ )
        addInitialState(*it);
    }
  
    /**
     *
     * @brief remove the state with the given name from the initial state 
     * set of the NWA
     *
     * @param name of the state to remove from the initial state set
     * @return false if the state does not exist in the initial state set 
     * of the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeInitialState( Key name )
    {
      return initialStates.remove(name);
    }
  
    /**
     *
     * @brief remove all states from the initial state set of the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    void TransSet<R,S,T,U,V>::clearInitialStates( )
    {
      initialStates.clear();
    }

    /**
     *
     * @brief provide access to the beginning of the initial state set
     *
     * @return an iterator pointing to the beginning of the initial state
     *  set
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::stateIterator TransSet<R,S,T,U,V>::beginInitialStates()
    {
      return initialStates.begin();
    }
    
    /**
     * 
     * @brief provide access to the end of the initial state set
     *
     * @return an iterator pointing just past the end of the initial 
     * state set
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::stateIterator TransSet<R,S,T,U,V>::endInitialStates()
    {
      return initialStates.end();
    }
  
    /**
     *
     * @brief get all final states
     *
     * @return set of Key values for all final states associated with 
     * the NWA
     *
     */
     template < typename R,typename S,typename T,typename U, typename V >
     StateSet<typename R> & TransSet<R,S,T,U,V>::getFinalStates( )
    {
      return finalStates;
    }
  
    /**
     *
     * @brief test if a state with the given name is a final state
     *
     * @param name of the state to check
     * @return true if the state with the given name is a final state
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::isFinalState( Key name )
    {
      return finalStates.contains(name);
    }

  
    /**
     *
     * @brief make the state with the given name a final state
     *
     * @param name of the state to add to final state set
     * @return false if the state already exists in the final state set
     * of the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::addFinalState( Key name )
    {
      if( !states.contains(name) )
        states.add(name);
      return finalStates.add(name);
    }

    /**
     *
     * @brief adds all of the final states in the given trans set to
     * the final state set associated with this trans set
     *
     * @parm the trans set whose final states to add to this trans 
     * set's final state set
     *
     */
    template <typename R,typename S,typename T,typename U,typename V >
    void TransSet<R,S,T,U,V>::addAllFinalStates( TransSet<R,S,T,U,V> addTransSet )
    {
      for( R::iterator it = addTransSet.finalStates.begin();
            it != addTransSet.finalStates.end(); it++ )
            addFinalState(*it);
    }  
  
    /**
     *
     * @brief remove the state with the given name from the final state 
     * set of the NWA
     *
     * @param name of the state to remove from the final state set
     * @return false if the state does not exist in the final state set 
     * of the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeFinalState( Key name )
    {
      return finalStates.remove(name);
    }
   
    /**
     *
     * @brief remove all states from the final state set of the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    void TransSet<R,S,T,U,V>::clearFinalStates( )
    {
      finalStates.clear();
    }

    /**
     *
     * @brief provide access to the beginning of the final state set
     *
     * @return an iterator pointing to the beginning of the final state
     *  set
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::stateIterator TransSet<R,S,T,U,V>::beginFinalStates()
    {
      return finalStates.begin();
    }

    /**
     * 
     * @brief provide access to the end of the final state set
     *
     * This method provides access to the position one past the end
     * of the final state set associated with this transition set.
     *
     * @return an iterator pointing just past the end of the final 
     * state set
     *
     */    
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::stateIterator TransSet<R,S,T,U,V>::endFinalStates()
    {
      return finalStates.end();
    }
    
    /**
     *
     * @brief returns the number of states in this collection
     *
     * @return the number of states in this collection
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    size_t TransSet<R,S,T,U,V>::sizeStates( )
    {
      //The count of states includes the error state.
      return states.size();
    }
    
    /**
     *
     * @brief returns the number of initial states in this collection
     *
     * @return the number of initial states in this collection
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    size_t TransSet<R,S,T,U,V>::sizeInitialStates( )
    {
      return initialStates.size();
    }
    
    /**
     *
     * @brief returns the number of final states in this collection
     *
     * @return the number of final states in this collection
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    size_t TransSet<R,S,T,U,V>::sizeFinalStates( )
    {
      return finalStates.size();
    }   
    
    //Symbol Accessors

    /**
     *
     * @brief get all symbols in the symbol set of the NWA
     *
     * @return set of Key values for all symbols associated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    const S & TransSet<R,S,T,U,V>::getSymbols( )
    {
      return symbols;
    }
  
    /**
     *
     * @brief test if the given symbol is a symbol of the NWA
     *
     * @param the symbol to check
     * @return true if the given symbol is a symbol of the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::isSymbol( Key sym )
    {
      return (symbols.count(sym) > 0);
    }
   
    /**
     *
     * @brief add the given symbol to the NWA
     *
     * @param symbol to add
     * @return false if the symbol is already associated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::addSymbol( Key sym )
    {
      if( symbols.count(sym) > 0 )
        return false;
      symbols.insert(sym); 
      //Add transitions to the transition sets to maintain the property
      //that every state has at least one call transition, one internal
      //transition, and, for every other state, one return transition for
      //each symbol.
      for( stateIterator it = beginStates();
            it != endStates(); it++ )
      {
        addCall(KeyTriple(*it,sym,WALI_ERR_STATE));
        addInternal(KeyTriple(*it,sym,WALI_ERR_STATE));
        for( stateIterator sit = beginStates();
              sit != endStates(); sit++ )
        {
          addReturn(KeyQuad(*it,*sit,sym,WALI_ERR_STATE));
        }
      }   
      
      return true;
    }
    
    /**
     *
     * @brief add the given symbols to the NWA
     *
     * @param the symbols to add
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    void TransSet<R,S,T,U,V>::addAllSymbols( S syms )
    {
      for( typename S::iterator it = syms.begin(); 
            it != syms.end(); it++ )
      {
        addSymbol(*it);
      }
    }
   
    /**
     *
     * @brief remove the given symbol from the NWA
     *
     * @param the symbol to remove
     * @return false if the symbol is no associated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeSymbol( Key sym )
    {
      //Remove transitions associated with the symbol that was removed.
      if( symbols.erase(sym) )
      {
        removeCallTransSym(sym);
        removeInternalTransSym(sym);
        removeReturnTransSym(sym);
        
        return true;
      }      
      return false;
    }
   
    /**
     *
     * @brief remove all symbols associated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    void TransSet<R,S,T,U,V>::clearSymbols( )
    {
      symbols.clear();

      //Since all symbols are being removed, all transitions are removed 
      //as well.
      callTrans.clear();
      internalTrans.clear();
      returnTrans.clear();
      
      //The epsilon symbol should always remain in the symbol set.
      //This will also replace transitions with the epsilon symbol for each 
      //state that exists in the state set to the error state .
      addSymbol(WALI_EPSILON);
    }
    
    /**
     *
     * @brief provide access to the beginning of the symbol set
     *
     * @return an iterator pointing to the beginning of the symbol set
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::symbolIterator TransSet<R,S,T,U,V>::beginSymbols()
    {
      return symbols.begin();
    }
    
    /**
     * 
     * @brief provide access to the end of the symbol set
     *
     * @return an iterator pointing just past the end of the symbol set
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::symbolIterator TransSet<R,S,T,U,V>::endSymbols()
    {
      return symbols.end();
    }
    
    /**
     *
     * @brief returns the number of symbols associated with this NWA
     *
     * @return the number of symbols associated with this NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    size_t TransSet<R,S,T,U,V>::sizeSymbols( )
    {
      return symbols.size();
    }

    //Transition Accessors

    /**
     *
     * @brief removes all transitions from this collection of transitions
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    void TransSet<R,S,T,U,V>::clear( )
    {
      //Since there must always be a transition of every kind for each symbol,
      //clearing the transitions should make all transitions go to the error state.
      T tmpCall = T();
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( !(cit->third == WALI_ERR_STATE) )
          tmpCall.insert(*cit);
      }
      for( callIterator cit = tmpCall.begin(); cit != tmpCall.end(); cit++ )
        removeCall(*cit);
        
      U tmpInternal = U();
      for( internalIterator iit = beginInternal(); iit != endInternal(); iit++ )
      {
        if( !(iit->third == WALI_ERR_STATE) )
          tmpInternal.insert(*iit);
      }
      for( internalIterator iit = tmpInternal.begin(); iit != tmpInternal.end(); iit++ )
        removeInternal(*iit);
        
      V tmpReturn = V();  
      for( returnIterator rit = beginReturn(); rit != endReturn(); rit++ )
      {
        if( !(rit->fourth == WALI_ERR_STATE) )
          tmpReturn.insert(*rit);
      }
      for( returnIterator rit = tmpReturn.begin(); rit != tmpReturn.end(); rit++ )
        removeReturn(*rit);
    }
    
    /**
     *
     * @brief get all call transitions in the collection of transitions associated 
     * with the NWA
     *
     * @return all call transitions in the collection of transitions associated 
     * with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    const typename T & TransSet<R,S,T,U,V>::getCalls()
    {
      return callTrans;
    }
    
    /**
     *
     * @brief get all internal transitions in the collection of transitions 
     * associated with the NWA
     *
     * @return all internal transitions in the collection of transitions associated 
     * with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    const typename U & TransSet<R,S,T,U,V>::getInternals()
    {
      return internalTrans;
    }
    
    /**
     *
     * @brief get all return transitions in the collection of transitions 
     * associated with the NWA
     *
     * @return all return transitions in the collection of transitions associated 
     * with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    const typename V & TransSet<R,S,T,U,V>::getReturns()
    {
      return returnTrans;
    }
      
    /**
     *
     * @brief add the given call transition to the collection of transitions
     *
     * @parm the call transition to add to the collection of transitions
     * @return false if the call transition already exists in the collection
     *
     */   
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::addCall(KeyTriple addTrans)
    {
      if(! isState(addTrans.first)
          || ! isState(addTrans.third) )
        return false;
      if(! isSymbol(addTrans.second) )
        addSymbol(addTrans.second);
      if( callTrans.count(addTrans) > 0 )
        return false;      
      
      callTrans.insert(addTrans);
      if(! (addTrans.third == WALI_ERR_STATE) )
      {
        //If there is a transition to the error state starting at
        //this transition's from state with this transition's symbol
        //then that transition should be removed.
        removeCall(KeyTriple(addTrans.first,addTrans.second,WALI_ERR_STATE));
      }
      return true;
    }
    
    /**
     *
     * @brief add the given internal transition to the collection of transitions
     *
     * @parm the internal transition to add to the collection of transitions
     * @return false if the internal transition already exists in the collection
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::addInternal(KeyTriple addTrans)
    {
      if(! isState(addTrans.first)
          || ! isState(addTrans.third) )
        return false;
      if(! isSymbol(addTrans.second) )
        addSymbol(addTrans.second);  
      if( internalTrans.count(addTrans) > 0 )
        return false;
      internalTrans.insert(addTrans);
      if(! (addTrans.third == WALI_ERR_STATE) )
      {
        //If there is a transition to the error state starting at
        //this transition's from state with this transition's symbol
        //then that transition should be removed.
        removeInternal(KeyTriple(addTrans.first,addTrans.second,WALI_ERR_STATE));
      }
      return true;
    }
    
    /**
     *
     * @brief add the given return transition to the collection of transitions
     *
     * @parm the return transition to add to the collection of transitions
     * @return false if the return transition already exists in the collection
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::addReturn(KeyQuad addTrans)
    {
      if(! isState(addTrans.first)
          || ! isState(addTrans.second)
          || ! isState(addTrans.fourth) )
        return false;
      if(! isSymbol(addTrans.third) )
        addSymbol(addTrans.third);
      if( returnTrans.count(addTrans) > 0 )
        return false;
      returnTrans.insert(addTrans);
      if(! (addTrans.fourth == WALI_ERR_STATE) )
      {
        //If there is a transition to the error state starting at
        //this transition's from state with this transition's predecessor
        //state and symbol then that transition should be removed.
        removeReturn(KeyQuad(addTrans.first,addTrans.second,addTrans.third,
                      WALI_ERR_STATE));
      }
      return true;
    }
      
    /**
     *
     * @brief add all transitions in the given collection to this
     * collection of transitions
     *
     * @parm the collection of transitions to add to this collection
     * of transitions
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    void TransSet<R,S,T,U,V>::addAllTrans(TransSet<R,S,T,U,V> addTransSet)
    {    
      //Any symbols not in the original set of symbols must be added.
      addAllSymbols(addTransSet.symbols);   
    
      for( TransSet::callIterator it = addTransSet.beginCall(); 
            it != addTransSet.endCall(); it ++ )
      {
        addCall(*it);
      }
      for( TransSet::internalIterator it = addTransSet.beginInternal(); 
            it != addTransSet.endInternal(); it ++ )
      {
        addInternal(*it);
      }
      for( TransSet::returnIterator it = addTransSet.beginReturn(); 
            it != addTransSet.endReturn(); it ++ )
      {
        addReturn(*it);
      }
    }
      
    /**
     *
     * @brief remove the given call transition from the collection of 
     * transitions
     *
     * @parm the call transition to remove from the collection
     * @return false if the given call transition does not exist in the 
     * collection, true otherwise.
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeCall(KeyTriple removeTrans)
    {
      if( callTrans.count(removeTrans) == 0 )
        return false;
      callTrans.erase(removeTrans);
      //If there is no other call transition starting at the from
      //state of this transition with the same symbol as this
      //transition, one must be added that goes to the error state.
      if( !callExists(removeTrans.first,removeTrans.second) )
        addCall(KeyTriple(removeTrans.first,removeTrans.second,WALI_ERR_STATE));
      return true;
    }
    
    /**
     *
     * @brief remove the given internal transition from the collection of 
     * transitions
     *
     * @parm the internal transition to remove from the collection
     * @return false if the given internal transition does not exist in the 
     * collection, true otherwise.
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeInternal(KeyTriple removeTrans)
    {
      if( internalTrans.count(removeTrans) == 0 )
        return false;
      internalTrans.erase(removeTrans);
      //If there is no other call transition starting at the from
      //state of this transition with the same symbol as this
      //transition, one must be added that goes to the error state.
      if( !internalExists(removeTrans.first,removeTrans.second) )
        addInternal(KeyTriple(removeTrans.first,removeTrans.second,WALI_ERR_STATE));
      return true;
    }
    
    /**
     *
     * @brief remove the given return transition from the collection of 
     * transitions
     *
     * @parm the return transition to remove from the collection
     * @return false if the given return transition does not exist in the 
     * collection, true otherwise.
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeReturn(KeyQuad removeTrans)
    {
      if( returnTrans.count(removeTrans) == 0 )
        return false;
      returnTrans.erase(removeTrans);
      //If there is no other call transition starting at the from
      //state of this transition with the same symbol as this
      //transition, one must be added that goes to the error state.
      if( !returnExists(removeTrans.first,removeTrans.second,removeTrans.third) )
        addReturn(KeyQuad(removeTrans.first,removeTrans.second,removeTrans.third,WALI_ERR_STATE));
      return true;
    }
     
    /**
     *
     * @brief test if the given call transition is in the collection of 
     * transitions associated with the NWA
     *
     * @param the call transition to check
     * @return true if the given call transition is in the collection of 
     * transitions associated with the NWA
     *
     */   
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::isCall(KeyTriple trans)
    {
      return (callTrans.count(trans) > 0);
    }
    
    /**
     *
     * @brief test if the given internal transition is in the collection of 
     * transitions associated with the NWA
     *
     * @param the internal transition to check
     * @return true if the given internal transition is in the collection of 
     * transitions associated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::isInternal(KeyTriple trans)
    {
      return (internalTrans.count(trans) > 0);
    }
    
    /**
     *
     * @brief test if the given return transition is in the collection of 
     * transitions associated with the NWA
     *
     * @param the return transition to check
     * @return true if the given return transition is in the collection of 
     * transitions associated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::isReturn(KeyQuad trans)
    {
      return (returnTrans.count(trans) > 0);
    } 
         
    //Utilities	

    /**
     *
     * @brief print the collection of transitions
     *
     * @param the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    std::ostream & TransSet<R,S,T,U,V>::print( std::ostream & o) const
    {
      o << "Q: ";
      states.print(o);
      
      o << "Q0: ";
      initialStates.print(o);
      
      o << "Qf: ";
      finalStates.print(o);
    
      o << "Sigma: " << "{ ";
      S::const_iterator it = symbols.begin();
      S::const_iterator itEND = symbols.end();
      for( bool first=true; it != itEND ; it++ )
      {
        if(! (*it == WALI_EPSILON) )
        {
          if( !first )
            o << ", ";            
          printKey(o,*it);
          first = false;
        }
      }
      o << " }\n";
      
      o << "Delta_c: " << "{ ";
      T::const_iterator cit = callTrans.begin();
      T::const_iterator citEND = callTrans.end();
      for( bool first=true; cit != citEND; cit++ )
      {
        if(! (cit->first == WALI_ERR_STATE) && ! (cit->third == WALI_ERR_STATE) )
        {
          if( !first )
            o << ", ";
          o << "(";
          printKey(o,cit->first);
          o << ", ";
          printKey(o,cit->second);
          o << ", "; 
          printKey(o,cit->third);
          o << ")";
          first=false;
        }
      }
      o << " }\n";
      
      o << "Delta_i: " << "{ ";
      U::const_iterator iit = internalTrans.begin();
      U::const_iterator iitEND = internalTrans.end();
      for( bool first=true; iit != iitEND; iit++ )
      {
        if(! (iit->first == WALI_ERR_STATE) && ! (iit->third == WALI_ERR_STATE) )
        {
          if( !first )
            o << ", ";
          o << "(";
          printKey(o,iit->first);
          o << ", ";
          printKey(o,iit->second);
          o << ", ";
          printKey(o,iit->third);
          o << ")";
          first = false;
         }
      }
      o << " }\n";
      
      o << "Delta_r: " << "{ ";
      V::const_iterator rit = returnTrans.begin();
      V::const_iterator ritEND = returnTrans.end();
      for( bool first=true; rit != ritEND; rit++ )
      {
        if(! (rit->first == WALI_ERR_STATE) && ! (rit->second == WALI_ERR_STATE)
              && ! (rit->fourth == WALI_ERR_STATE) )
        {
          if( !first )
            o << ", ";
          o << "(";
          printKey(o,rit->first);
          o << ", ";
          printKey(o,rit->second);
          o << ", "; 
          printKey(o,rit->third);
          o << ", ";
          printKey(o,rit->fourth);
          o << ")";
          first = false;
        }
      }
      o << " }\n";
      
      return o;
    }

    /**
     *
     * @brief tests whether this collection of transitions is equivalent 
     * to the collection of transitions 'other'
     *
     * @param the TransSet to compare this TransSet to
     * @return true if this TransSet is equivalent to the TransSet 'other'
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::operator==( TransSet<R,S,T,U,V> & other )
    {
      return ( (states == other.states) &&
                (initialStates == other.initialStates) &&
                (finalStates == other.finalStates) &&
                (symbols == other.symbols) &&
                (callTrans == other.callTrans) &&
                (internalTrans == other.internalTrans) &&
                (returnTrans == other.returnTrans) );
    }

    /**
     *
     * @brief provides access to the call transitions in the collection 
     * through an iterator
     *
     * @return the starting point of an iterator through the call transitions
     * in the collection of transitions
     *
     */    
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::callIterator TransSet<R,S,T,U,V>::beginCall()
    {
      return callTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     * through an iterator
     *
     * @return the starting point of an iterator through the internal transitions
     * in the collection of transitions
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::internalIterator TransSet<R,S,T,U,V>::beginInternal()
    {
      return internalTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     * through an iterator
     *
     * @return the starting point of an iterator through the return transitions
     * in the collection of transitions
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::returnIterator TransSet<R,S,T,U,V>::beginReturn()
    {
      return returnTrans.begin();
    }
      
    /**
     *
     * @brief provides access to the call transitions in the collection 
     * through an iterator
     *
     * @return the exit point of an iterator through the call transitions in
     * the collection of transitions
     *
     */    
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::callIterator TransSet<R,S,T,U,V>::endCall()
    {
      return callTrans.end();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     * through an iterator
     *
     * @return the exit point of an iterator through the internal transitions in
     * the collection of transitions
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::internalIterator TransSet<R,S,T,U,V>::endInternal()
    {
      return internalTrans.end();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     * through an iterator
     *
     * @return the exit point of an iterator through the return transitions in
     * the collection of transitions
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    typename TransSet<R,S,T,U,V>::returnIterator TransSet<R,S,T,U,V>::endReturn()
    {
      return returnTrans.end();
    }
    
    /**
     *
     * @brief returns the number of call transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the number of call transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    size_t TransSet<R,S,T,U,V>::sizeCall( )
    {
      size_t count = 0;
      for( T::iterator it = callTrans.begin(); it != callTrans.end(); it++ )
        if( !(it->first == WALI_ERR_STATE) && !(it->third == WALI_ERR_STATE) )
          count++;
      return count;
    }
        
    /**
     *
     * @brief returns the number of internal transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the number of internal transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V > 
    size_t TransSet<R,S,T,U,V>::sizeInternal( )
    {
      size_t count = 0;
      for( U::iterator it = internalTrans.begin(); it != internalTrans.end(); it++ )
        if( !(it->first == WALI_ERR_STATE) && !(it->third == WALI_ERR_STATE) )
          count++;
      return count;
    }
        
    /**
     *
     * @brief returns the number of return transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the number of return transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    size_t TransSet<R,S,T,U,V>::sizeReturn( )
    {
      size_t count = 0;
      for( V::iterator it = returnTrans.begin(); it != returnTrans.end(); it++ )
        if( !(it->first == WALI_ERR_STATE) 
            && !(it->second == WALI_ERR_STATE) 
            && !(it->fourth == WALI_ERR_STATE) )
          count++;
      return count;
    }
        
    /**
     *
     * @brief returns the total number of transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the total number of transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    size_t TransSet<R,S,T,U,V>::size( )
    {
      return (sizeCall() + sizeInternal() + sizeReturn());
    }
      
    /** 
     *
     * @brief removes all call transitions to or from the state with 
     * the given name  
     *
     * @parm the name of the state whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeCallTransWith( Key name )
    {
      T removeTrans = T();
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( (cit->first == name) ||
            (cit->third == name) )
            removeTrans.insert(*cit);
      }     
      
      for( callIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeCall(*rit);
      }
      
      return removeTrans.size() > 0;
    }
  
    /** 
     *
     * @brief removes all internal transitions to or from the state
     * with the given name
     *
     * @parm the name of the state whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeInternalTransWith( Key name )
    {
      U removeTrans = U();
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == name) ||
            (iit->third == name) )
            removeTrans.insert(*iit);
      }     
      
      for( internalIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeInternal(*rit);
      }
      
      return removeTrans.size() > 0;
    }
  
    /** 
     *
     * @brief removes all return transitions to or from the state with
     * the given name as well as return transitions corresponding to 
     * calls from the state with the given name
     *
     * @parm the name of the state whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeReturnTransWith( Key name )
    {
      V removeTrans = V();
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == name) ||
            (rit->second == name) ||
            (rit->fourth == name) )
            removeTrans.insert(*rit);
      }     
      
      for( returnIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return removeTrans.size() > 0;
    }
    
    /** 
     *
     * @brief removes all call transitions with the given symbol 
     *
     * @parm the name of the symbol whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeCallTransSym(Key sym)
    {
      std::set< KeyTriple > removeTrans = std::set< KeyTriple >();
      for( std::set< KeyTriple >::iterator cit = callTrans.begin();
            cit != callTrans.end(); cit++ )
      {
        if( cit->second == sym )
          removeTrans.insert(*cit);
      }     
      
      for( std::set< KeyTriple >::iterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        callTrans.erase(*rit);
      }
      
      return removeTrans.size() > 0;  
    }
    
    /** 
     *
     * @brief removes all internal transitions with the given symbol 
     *
     * @parm the name of the symbol whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeInternalTransSym(Key sym)
    {
      std::set< KeyTriple > removeTrans = std::set< KeyTriple >();
      for( std::set< KeyTriple >::iterator iit = internalTrans.begin();
            iit != internalTrans.end(); iit++ )
      {
        if( iit->second == sym )
          removeTrans.insert(*iit);
      }     
      
      for( std::set< KeyTriple >::iterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        internalTrans.erase(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /** 
     *
     * @brief removes all return transitions with the given symbol 
     *
     * @parm the name of the symbol whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::removeReturnTransSym(Key sym)
    {
      std::set< KeyQuad > removeTrans = std::set< KeyQuad >();
      for( std::set< KeyQuad >::iterator rit = returnTrans.begin();
            rit != returnTrans.end(); rit++ )
      {
        if( rit->third == sym )
          removeTrans.insert(*rit);
      }     
      
      for( std::set< KeyQuad >::iterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        returnTrans.erase(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /**
     *
     * @brief test if there exists a call transition with the given from state 
     * and symbol in the collection of transitions associated with the NWA
     *
     * @param from: the desired from state for the call transition
     * @param sym: the desired symbol for the call transition
     * @return true if there exists a call transition with the given from state and
     * symbol in the collection of transitions associated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::callExists(Key from,Key sym)
    {
      for( callIterator cit = beginCall();
            cit != endCall(); cit++ )
      {
        if( (cit->first == from) &&
            (cit->second == sym) &&
            !(cit->third == WALI_ERR_STATE) )
            return true;
      }  
      return false;    
    }
    
    /**
     *
     * @brief test if there exists an internal transition with the given from state 
     * and symbol in the collection of transitions associated with the NWA
     *
     * @param from: the desired from state for the internal transition
     * @param sym: the desired symbol for the internal transition
     * @return true if there exists an internal transition with the given from state and
     * symbol in the collection of transitions associated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::internalExists(Key from,Key sym)
    {
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == from) &&
            (iit->second == sym) &&
            !(iit->third == WALI_ERR_STATE) )
            return true;
      }     
      return false;
    }
    
    /**
     *
     * @brief test if there exists a return transition with the given from state, 
     * predecessor state, and symbol in the collection of transitions associated 
     * with the NWA
     *
     * @param from: the desired from state for the return transition
     * @param pred: the desired predecessor state for the return transition
     * @param sym: the desired symbol for the return transition
     * @return true if there exists a return transition with the given from state and
     * symbol in the collection of transitions associated with the NWA
     *
     */
    template < typename R,typename S,typename T,typename U, typename V >
    bool TransSet<R,S,T,U,V>::returnExists(Key from, Key pred, Key sym)
    {
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == from) &&
            (rit->second == pred) &&
            (rit->third == sym) &&
            !(rit->fourth == WALI_ERR_STATE) )
            return true;
      }     
      return false;
    }    
  }
}
#endif