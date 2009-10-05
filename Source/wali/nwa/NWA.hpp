#ifndef wali_nwa_NWA_GUARD
#define wali_nwa_NWA_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/nwa/TransSet.hpp"
#include "wali/nws/NWS.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/witness/Witness.hpp"

// std::c++
#include <iostream>
#include <map>

namespace wali
{
  namespace nwa
  {
    class NWA : public Printable
    {
      public:
      typedef std::set< Key > States;
      typedef std::set< Key > Symbols;
      typedef std::set< KeyTriple > Calls;
      typedef std::set< KeyTriple > Internals;
      typedef std::set< KeyQuad > Returns;
      typedef TransSet< States,Symbols,Calls,Internals,Returns > Trans;
      
      typedef std::pair< Key,Key > StatePair;
      typedef std::set< StatePair > StatePairSet;
      typedef std::map< StatePairSet,Key > StateMap;
      typedef std::stack< Key,StatePairSet > CallMap;
          
      //
      // Methods
      //

      public:
        //Constructors and Destructor
        NWA( );
        NWA( NWA & other );
        NWA & operator=( NWA & other );

        ~NWA( );

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
        const std::set< Key > & getSymbols( );

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
         * the NWA. If the symbol is already associated with the NWA, false is 
         * returned. Otherwise, true is returned.
         *
         * @param the symbol to add
         * @return false if the symbol is already associated with the NWA
         *
         */
        bool addSymbol( Key sym );

        /**
         *
         * @brief remove the given symbol from the NWA
         *
         * This method checks for the given symbol in the symbol set 
         * associated with the NWA and removes the symbol from the
         * symbol set if necessary.  Any transitions associated with the symbol 
         * to be removed are also removed.
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
         * This method removes all symbols associated with the NWA.
         *
         */
        void clearSymbols( );

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
        const StateSet<NWA::States> & getStates( );

        /**
         *
         * @brief test if a state with the given name is a state of the NWA
         *
         * This method tests whether the state with the given name is in the 
         * state set of the NWA.
         *
         * @param name of the state to check
         * @return true if the state with the given name is a state of the NWA
         *
         */
        bool isState( Key name );

        /**
         *
         * @brief add a state with the given name to the NWA
         *
         * This method adds a state with the given name to the state set for 
         * the NWA.  If the state already exists in the NWA, false is 
         * returned.  Otherwise, true is returned.
         *
         * @param name of the state to add
         * @return false if the state already exists in the NWA
         *
         */
        bool addState( Key name );

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
         * This method removes all states and transitions from the NWA.
         *
         */
        void clearStates( );

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
        const StateSet<NWA::States> & getInitialStates( );

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
        const StateSet<NWA::States> & getFinalStates( );

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

        //Transition Accessors

        /**
         *
         * @brief remove all transitions from the NWA
         *
         * This method removes all transitions from the NWA.  It does not 
         * remove any states from the NWA.
         *
         */
        void clearTrans( );

        /**
         *
         * @brief get all call transitions in the transition set of the NWA.
         *
         * This method provides access to the set of call transitions 
         * associated with this NWA.  Note: An NWA can have an unbounded 
         * number of transitions associated with it.
         *
         * @return set of call transitions associated with the NWA
         *
         */
        const Calls & getCallTrans( );

        /**
         *
         * @brief add a call transition to the NWA
         *
         * This method creates a call transition with the given edge and 
         * label information and adds it to the transition set for the NWA.  
         * If the call transition already exists in the NWA, false is 
         * returned. Otherwise, true is returned.
         *
         * @param from: name of the state the edge departs from
         * @param sym: name of the symbol labeling the edge
         * @param to: name of the state the edge arrives to  
         * @return false if the call transition already exists in the NWA
         *
         */
        bool addCallTrans( Key from, Key sym, Key to );

        /**
         *
         * @brief add a call transition to the NWA
         *
         * This method adds the given call transition to the transition set 
         * for the NWA.  If the call transition already exists in the NWA, 
         * false is returned. Otherwise, true is returned.
         *
         * @param the call transition to add
         * @return false if the call transition already exists in the NWA
         *
         */
        bool addCallTrans( KeyTriple * ct );

        /**
         *
         * @brief remove the call transition with the given edge and label 
         * information from the NWA
         *
         * This method checks for the call transition with the given edge 
         * and label information in the transition set. If the transition is 
         * found, it is removed from the transition set.
         *
         * @param from: name of the state the edge departs from
         * @param sym: name of the symbol labeling the edge
         * @param to: name of the state the edge arrives to  
         * @return false if the call transition does not exist in the NWA
         *
         */
        bool removeCallTrans( Key from, Key sym, Key to );

        /**
         *
         * @brief remove a call transition from the NWA
         *
         * This method checks for the call transition in the transition set. 
         * If the transition is found, it is removed from the transition set.
         *
         * @param the call transition to remove
         * @return false if the call transition does not exist in the NWA
         *
         */
        bool removeCallTrans( KeyTriple * ct );

        /**
         *
         * @brief get all internal transitions in the transition set of the 
         * NWA.
         *
         * This method provides access to the set of internal transitions 
         * associated with this NWA.  Note: An NWA can have an unbounded 
         * number of transitions associated with it.
         *
         * @return set of internal transitions associated with the NWA
         *
         */
        const Internals & getInternalTrans( );

        /**
         *
         * @brief add an internal transition to the NWA
         *
         * This method creates an internal transition with the given edge 
         * and label information and adds it to the transition set for the 
         * NWA.  If the internal transition already exists in the NWA, false 
         * is returned. Otherwise, true is returned.
         *
         * @param from: name of the state the edge departs from
         * @param sym: name of the symbol labeling the edge
         * @param to: name of the state the edge arrives to  
         * @return false if the internal transition already exists in the NWA
         *
         */
        bool addInternalTrans( Key from, Key sym, Key to );

        /**
         *
         * @brief add an internal transition to the NWA
         *
         * This method adds the given internal transition to the transition 
         * set for the NWA.  If the internal transition already exists in 
         * the NWA, false is returned. Otherwise, true is returned.
         *
         * @param internal transition to add to the NWA
         * @return false if the internal transition already exists in the NWA
         *
         */
        bool addInternalTrans( KeyTriple * it );

        /**
         *
         * @brief remove the internal transition with the given edge and 
         * label information from the NWA
         *
         * This method checks for the internal transition with the given 
         * edge and label information in the transition set.  If the 
         * transition is found, it is removed from the transition set.
         *
         * @param from: name of the state the edge departs from
         * @param sym: name of the symbol labeling the edge
         * @param to: name of the state the edge arrives to  
         * @return false if the internal transition does not exist in the NWA
         *
         */
        bool removeInternalTrans( Key from, Key sym, Key to );

        /**
         *
         * @brief remove an internal transition from the NWA
         *
         * This method checks for the internal transition in the transition 
         * set. If the transition is found, it is removed from the 
         * transition set.
         *
         * @param the internal transition to remove
         * @return false if the internal transition does not exist in the NWA
         *
         */
        bool removeInternalTrans( KeyTriple * it );

        /**
         *
         * @brief get all return transitions in the transition set of the NWA.
         *
         * This method provides access to the set of return transitions
         * associated with this NWA.  Note: An NWA can have an unbounded number
         * of transitions associated with it.
         *
         * @return set of return transitions associated with the NWA
         *
         */
        const Returns & getReturnTrans( );

        /**
         *
         * @brief add a return transition to the NWA
         *
         * This method creates a return transition with the given edge and 
         * label information and adds it to the transition set for the NWA.  
         * If the return transition already exists in the NWA, false is 
         * returned. Otherwise, true is returned.
         *
         * @param from: name of the state the edge departs from
         * @param pred: name of the state from which the call was initiated  
         * @param sym: name of the symbol labeling the edge
         * @param to: name of the state the edge arrives to  
         * @return false if the return transition already exists in the NWA
         *
         */
        bool addReturnTrans( Key from, Key pred, Key sym, Key to );

        /**
         *
         * @brief add a return transition to the NWA
         *
         * This method adds the given return transition to the transition 
         * set for the NWA.  If the return transition already exists in the 
         * NWA, false is returned. Otherwise, true is returned.
         *
         * @param return transition to add to the NWA
         * @return false if the return transition already exists in the NWA
         *
         */
        bool addReturnTrans( KeyQuad * rt );

        /**
         *
         * @brief remove the return transition with the given edge and label 
         * information from the NWA
         *
         * This method checks for the return transition with the given edge 
         * and label information in the transition set. If the transition is 
         * found, it is removed from the transition set.
         *
         * @param from: name of the state the edge departs from
         * @param pred: name of the state from which the call was initiated  	   
         * @param sym: name of the symbol labeling the edge
         * @param to: name of the state the edge arrives to  
         * @return false if the return transition does not exist in the NWA
         *
         */
        bool removeReturnTrans( Key from, Key pred, Key sym, Key to );

        /**
         *
         * @brief remove a return transition from the NWA
         *
         * This method checks for the return transition in the transition 
         * set. If the transition is found, it is removed from the 
         * transition set.
         *
         * @param the return transition to remove
         * @return false if the return transition does not exist in the NWA
         *
         */
        bool removeReturnTrans( KeyQuad * rt );

        //Building NWAs

        /**
         *
         * @brief constructs the NWA resulting from the union of this NWA and
         * the NWA 'other'
         *
         * This method constructs the union of this NWA and the NWA 'other' by 
         * creating a new initial state with epsilon transitions to all initial 
         * states of the two NWAs.  The resulting NWA is NOT guaranteed to be 
         * deterministic.
         *
         * @param the NWA with which to union this NWA
         * @return the NWA which is the union of this NWA and the NWA 'other'
         *
         */
        NWA * unionNWA( NWA other );

        /**
         *
         * @brief constructs the NWA resulting from the concatenation of this
         * NWA and the NWA 'other'
         *
         * This method constructs the concatenation of this NWA and the NWA 
         * 'other' by adding epsilon transitions from all final states of this 
         * NWA to all initial states of the NWA 'other'.  The resulting NWA is 
         * NOT guaranteed to be deterministic.
         *
         * @param the NWA with which to concatenate this NWA
         * @return the NWA which is the concatenation of this NWA and the NWA
         * 'other'
         *
         */
        NWA * concat( NWA other );

        /**
         *
         * @brief constructs the NWA resulting from performing Kleene-* on 
         * this NWA
         *
         * This method constructs the Kleene-* of this NWA by adding epsilon 
         * transitions from all final states of the NWA to all initial states 
         * of the NWA.  The resulting NWA is NOT guaranteed to be deterministic.
         *
         * @return the NWA which is the result of performing Kleene-* on 
         * this NWA
         *
         */
        NWA * star( );

        /**
         *
         * @brief constructs the NWA that is the complement of this NWA
         *
         * This method constructs the complement of this NWA by replacing the 
         * set of final states in this NWA with the set of states that are not 
         * final in this NWA.  The resulting NWA is deterministic iff the 
         * initial NWA was.
         *
         * @return the NWA which is the complement of this NWA
         *
         */
        NWA * complement( );

        /**
         * @brief constructs the NWA which is the reverse of this NWA
         *
         * This method constructs the NWA which is the reverse of this NWA.
         * It reverses internal transitions and switches call and return
         * transitions.
         *
         * @return the NWA which is the reverse of this NWA
         * 
         */
        NWA * reverse();

        /**
         *
         * @brief constructs the NWA which is the intersection of this NWA
         * and the NWA 'other'
         *
         * This method constructs the NWA which accepts only nested words 
         * that are accepted by both this NWA and the NWA 'other'.  The 
         * resulting NWA is NOT guaranteed to be deterministic.	
         *
         * @param the NWA with which to perform the intersection with this
         * NWA
         * @return the NWA which is the intersection of this NWA and the NWA
         * 'other'
         *	
         */
        NWA * intersect( NWA other );

        //Using NWAs

        /**
         * 
         * @brief constructs the WPDS which is the result of the explicit NWA 
         * plus WPDS construction from Advanced Querying for Property Checking
         *
         * This method constructs the WPDS which allows WPDS reachability to
         * be used to perform property checking using this NWA and the given
         * WPDS.
         *
         * @param the WPDS that forms the basis for the constructed WPDS
         * @return the WPDS which can be used to perform property checking 
         * using PDS reachability
         * 
         */
        wpds::WPDS plusWPDS( wpds::WPDS base );
    
        /**
         *
         * @brief constructs the NWA equivalent to the given PDS
         *
         * This method constructs the NWA that is equivalent to the given PDS.
         *
         * @return the NWA equivalent to the given PDS
         *
         */
        static nwa::NWA PDStoNWA(wpds::WPDS pds);
    
        /**
         *
         * @brief constructs the PDS equivalent to this NWA
         *
         * This method constructs the PDS that is equivalent to this NWA.
         *
         * @return the PDS equivalent to this NWA
         *
         */
        static wpds::WPDS NWAtoPDS(NWA nwa,witness::Witness wgt);
    
        /**
         *
         * @brief constructs a deterministic NWA that is equivalent to this 
         * NWA.
         *
         * This method constructs a deterministic NWA that is equivalent to 
         * this NWA.
         *
         * @return the NWA which is a deterministed equivalent to this NWA
         *
         */
        NWA & determinize( );

        /**
         *
         * @brief tests whether the NWA is deterministic 
         *
         * This method tests whether the NWA is deterministic or nondeterministic.
         * If the NWA is deterministic, true is returned.  Otherwise, false is
         * returned.
         *
         * @return true if the NWA is deterministic, false otherwise
         *
         */
        bool isDeterministic( );

        /**
         *
         * @brief tests whether the language accepted by this NWA is empty
         *
         * This method tests whether the language accepted by this NWA is empty.
         *
         * @return true if the language accepted by this NWA is empty
         *
         */
        bool isEmpty( );

        /**
         *
         * @brief tests whether the given nested word is a member of the 
         * language accepted by this NWA
         *
         * This method tests whether the given nested word is a member of the 
         * language accepted by this NWA.
         *
         * @param the nested word to test
         * @return true if the given nested word is a member of the language
         * accepted by this NWA
         *
         */
        bool isMember( nws::NWS word );   

        //Utilities	

        /**
         *
         * @brief print the NWA
         *
         * This method prints out the NWA to the output stream provided.
         *
         * @param the output stream to print to
         * @return the output stream that was printed to
         *
         */
        std::ostream & print( std::ostream & o) const;

        /**
         *
         * @brief tests whether this NWA is equivalent to the NWA 'other'
         *
         * This method tests the equivalence of this NWA and the NWA 'other'.
         *
         * @param the NWA to compare this NWA to
         * @return true if this NWA is equivalent to the NWA 'other'
         *
         */
        bool operator==( NWA & other );

        /**
         *
         * @brief returns the number of states associated with this NWA
         *
         * This method returns the number of states associated with this
         * NWA.
         *
         * @return the number of states associated with this NWA
         *
         */
        size_t numStates( );

        /**
         *
         * @brief returns the number of transitions (of all kinds) 
         * associated with this NWA
         *
         * This method returns the number of transitions (call, internal, and
         * return) associated with this NWA.
         *
         * @return the number of transitions (of all kinds) associated
         * with this NWA
         *
         */
        size_t numTrans( );

      protected:
      
        /**
         *
         * @brief tests whether the NWA has a path from the current state to
         * a final state given the call stack and the set of states already visited
         *
         * This method tests whether the NWA contains a path from the current state
         * to any final state given the call stack and the set of states already
         * visited.
         * 
         * @param currState: the state from which to start searching for a path
         * @param calls: the stack of calls that have been made thus far
         * @param visited: the states which have already been visited
         * @return true if there is a path through the NWA from the current state 
         * to a final state, false otherwise
         *
         */
        bool NWA::hasPath( Key currState, std::stack< Key > calls, std::set< Key > visited );
    
        /** 
         *
         * @brief tests whether the nested word suffix is accepted by 
         * this nested word automaton starting at the state with the
         * given name
         *
         * This method tests whether the nested word suffix is accepted
         * by this nested word automaton by simulating the word suffix 
         * on the automaton starting at the state with the given name.  
         *
         * @parm currState: the state in the nested word automaton at 
         * which to start the simulation
         * @parm word: the nested word suffix to match
         * @return true if the simulation ends in an accepting state
         *
         */
        bool matchWord( Key currState, nws::NWS word, std::stack< Key > calls );
        
        /**
         *
         * @brief constructs the transition table and state map for the deterministic 
         * NWA that is equivalent to this NWA.
         *
         * This method constructs the transition table and state map for the 
         * deterministic NWA that is equivalent to this NWA.
         *
         * @return the transition table and state map for the deterministic NWA 
         * that is equivalent to this NWA
         *
         */
        std::pair<Trans,StateMap> det( Trans trans, StateMap stateMap, 
                    StatePairSet currState, std::stack<StatePairSet> callPred );

      //
      // Variables
      //
      protected:
        Trans * trans;
    };
  }
}
#endif