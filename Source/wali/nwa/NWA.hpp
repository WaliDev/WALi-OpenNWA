#ifndef wali_nwa_NWA_GUARD
#define wali_nwa_NWA_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/KeyContainer.hpp"

#include "wali/nwa/State.hpp"
#include "wali/nwa/Symbol.hpp"
#include "wali/nwa/TransSet.hpp"

#include "wali/nws/NWS.hpp"
#include "wali/nwa/WordRecConfig.hpp"

#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"

// std::c++
#include <iostream>
#include <map>
#include <deque>

namespace wali
{
  namespace nwa
  {
    template<typename StKey,typename Label >
    class NWA : public Printable
    {
      public:
      typedef typename StateSet<State<StKey>>::iterator stateIterator;
      typedef StateSet<State<StKey>> States;
      typedef std::set<Symbol<Label>> Symbols;
      typedef std::set< KeyTriple > Calls;
      typedef std::set< KeyTriple > Internals;
      typedef std::set< KeyQuad > Returns;
      typedef TransSet< State<StKey>,Symbol<Label>,Calls,Internals,Returns > Trans;
      typedef typename Trans::callIterator callIterator;
      typedef typename Trans::internalIterator internalIterator;
      typedef typename Trans::returnIterator returnIterator;
      
      typedef std::pair< State<StKey>,State<StKey> > StatePair;
      typedef std::set< StatePair > StatePairSet;
      typedef std::map< StatePairSet,State<StKey> > StateMap;
      typedef std::stack< State<StKey>,StatePairSet > CallMap; //?? do i even use this??
          
      //
      // Methods
      //

      public:
        //Constructors and Destructor
        NWA( );
        NWA( const NWA & other );
        NWA & operator=( const NWA & other );

        ~NWA( );

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
        const States & getStates( );

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
        bool isState( State<StKey> name ); 

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
        bool addState( State<StKey> name );
        
        /**
         *
         * @brief add all the states in the given StateSet to the NWA
         *
         * This method adds all of the given states to the state set for 
         * the NWA.
         *
         * @param the StateSet that contains the states to add
         *
         */
        void addAllStates( States addStateSet);

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
        bool removeState( State<StKey> name );

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
         * @brief provide access to the beginning of the state set
         *
         * This method provides access to the beginning of the state set
         * associated with this NWA.
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
        const States & getInitialState( );

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
        bool isInitialState( State<StKey> name ); 

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
        bool addInitialState( State<StKey> name );
        
        /**
         *
         * @brief adds all of the states in the given StateSet to
         * the initial state set associated with this NWA
         *
         * This method adds all of the states associated with the 
         * given StateSet to the initial state set associated with this
         * NWA.
         *
         * @parm the trans set whose initial states to add to this NWA's
         * initial state set
         *
         */
        void addAllInitialStates( States addStateSet); 

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
        bool removeInitialState( State<StKey> name );

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
         * set associated with this NWA.
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
         * of the initial state set associated with this NWA.
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
        const States & getFinalStates( );

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
        bool isFinalState( State<StKey> name ); 

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
        bool addFinalState( State<StKey> name );

        /**
         *
         * @brief adds all of the final states in the given StateSet to
         * the final state set associated with this NWA
         *
         * This method adds all of the final states associated with the 
         * given StateSet to the final state set associated with this
         * NWA.
         *
         * @parm the StateSet whose final states to add to this NWA's
         * final state set
         *
         */
        void addAllFinalStates( States addStateSet); 

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
        bool removeFinalState( State<StKey> name );

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
         * set associated with this NWA.
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
         * of the final state set associated with this NWA.
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
         * NWA. 
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
        const StateSet< Symbol<Label> > & getSymbols( );

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
        bool isSymbol( Symbol<Label> sym );

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
        bool addSymbol( Symbol<Label> sym );

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
        bool removeSymbol( Symbol<Label> sym );

        /**
         *
         * @brief remove all symbols associated with the NWA
         *
         * This method removes all symbols associated with the NWA.
         *
         */
        void clearSymbols( );

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
         * @brief provide access to the beginning of the call transition set
         *
         * This method provides access to the beginning of the call transition
         * set associated with this NWA.
         *
         * @return an iterator pointing to the beginning of the call transition
         *  set
         *
         */
        callIterator beginCallTrans();
        
        /**
         * 
         * @brief provide access to the end of the call transition set
         *
         * This method provides access to the position one past the end
         * of the call transition set associated with this NWA.
         *
         * @return an iterator pointing just past the end of the call 
         * transition set
         *
         */
        callIterator endCallTrans();

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
         * @brief provide access to the beginning of the internal transition set
         *
         * This method provides access to the beginning of the internal transition
         * set associated with this NWA.
         *
         * @return an iterator pointing to the beginning of the internal transition
         *  set
         *
         */
        internalIterator beginInternalTrans();
        
        /**
         * 
         * @brief provide access to the end of the internal transition set
         *
         * This method provides access to the position one past the end
         * of the internal transition set associated with this NWA.
         *
         * @return an iterator pointing just past the end of the internal
         * transition set
         *
         */
        internalIterator endInternalTrans();

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
        
        /**
         *
         * @brief provide access to the beginning of the return transition set
         *
         * This method provides access to the beginning of the return transition
         * set associated with this NWA.
         *
         * @return an iterator pointing to the beginning of the return transition
         *  set
         *
         */
        returnIterator beginReturnTrans();
        
        /**
         * 
         * @brief provide access to the end of the return transition set
         *
         * This method provides access to the position one past the end
         * of the return transition set associated with this NWA.
         *
         * @return an iterator pointing just past the end of the return 
         * transition set
         *
         */
        returnIterator endReturnTrans();

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
        static wpds::WPDS NWAtoPDS(NWA nwa,wali::sem_elem_t one);
    
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
        bool matchWord( State<StKey> currState, std::deque<WordRecConfig<StKey>> configs );
        
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
        std::pair<NWA,StateMap> det( NWA nWdAut, StateMap stateMap,StatePairSet currState,
                                    std::stack<StatePairSet> callPred );

      //
      // Variables
      //
      protected:
        States states;
        States initialStates;
        States finalStates;
        Trans * trans;
    };
    
    //
    // Methods
    //

    //Constructors and Destructor
    template<typename StKey,typename Label >
    NWA<StKey,Label>::NWA( )
    {
      states = StateSet<State<StKey>>();      
      initialStates = StateSet<State<StKey>>();
      finalStates = StateSet<State<StKey>>();
      
      trans = new Trans();
    }
    
    template<typename StKey,typename Label >
    NWA<StKey,Label>::NWA( const NWA & other )
    {
      operator=(other);
    }
    
    template<typename StKey,typename Label >
    NWA<StKey,Label> & NWA<StKey,Label>::operator=( const NWA & other )
    {
      //Wipe any pre-existing data.
      states.clear();
      initialStates.clear();
      finalStates.clear();
      trans->clear();
      
      //Copy data over from 'other'
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;
      
      trans->addAllSymbols(*other.trans);
      trans->addAllTrans(*other.trans);
      
      return *this;
    }
   
    template<typename StKey,typename Label >
    NWA<StKey,Label>::~NWA( )
    {         
      states.clear();
      initialStates.clear();
      finalStates.clear();
      
      trans->~Trans();    
    }
    
  
    //State Accessors

    //All States
    /**
     *
     * @brief get all states in the state set of the NWA
     *
     * @return set of Key values for all states associated with the NWA
     *
     */
    template<typename StKey,typename Label > 
    const typename NWA<StKey,Label>::States & NWA<StKey,Label>::getStates( )
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
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::isState( State<StKey> name )
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
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::addState( State<StKey> name )
    {
      if( states.contains(name) )
        return false;
      states.add(name);
      
      return true;
    }
    
    /**
     *
     * @brief add all the states in the given StateSet to the NWA
     *
     * @param the StateSet that contains the states to add
     *
     */
    template<typename StKey,typename Label > 
    void NWA<StKey,Label>::addAllStates( States addStateSet)
    {
      for( stateIterator sit = addStateSet.begin();
            sit != addStateSet.end(); sit++ )
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
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::removeState( State<StKey> name )
    {
      //Remove transitions associated with the state that was removed.
      if( states.remove(name) )
      {
        //If this state existed in either the initial state set or the
        //final state set, then it should be removed from that set too.
        initialStates.remove(name);
        finalStates.remove(name);
      
        trans->removeCallTransWith(name);
        trans->removeInternalTransWith(name);
        trans->removeReturnTransWith(name);
        
        return true;
      }      
      return false;
    }
    
   
    /**
     *
     * @brief remove all states from the NWA
     *
     */
    template<typename StKey,typename Label > 
    void NWA<StKey,Label>::clearStates( )
    {      
      initialStates.clear();
      finalStates.clear();
      states.clear();

      //Since all states are being removed, all transitions are removed 
      //as well.
      trans->clear();
    }
    
     /**
     *
     * @brief provide access to the beginning of the state set
     *
     * @return an iterator pointing to the beginning of the state set
     *
     */
    template<typename StKey,typename Label > 
    typename NWA<StKey,Label>::stateIterator NWA<StKey,Label>::beginStates()
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
    template<typename StKey,typename Label > 
    typename NWA<StKey,Label>::stateIterator NWA<StKey,Label>::endStates()
    {
      return states.end();
    }
  
  
    //Initial States
    /**
     *
     * @brief get all states in the initial state set
     *
     * @return set of Key values for all inital states associated with 
     * the NWA
     *
     */
    template<typename StKey,typename Label > 
    const typename NWA<StKey,Label>::States & NWA<StKey,Label>::getInitialState( )
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
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::isInitialState( State<StKey> name )
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
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::addInitialState( State<StKey> name )
    {
      if( initialStates.size() > 0 )
        return false;
        
      if( !states.contains(name) )
        states.add(name);
      return initialStates.add(name);
    }
    
    /**
     *
     * @brief add all the states in the given StateSet to the
     * initial state set associated with this NWA
     *
     * @param the StateSet that contains the states to add
     *
     */
    template<typename StKey,typename Label > 
    void NWA<StKey,Label>::addAllInitialStates( States addStateSet)
    {
      if( addStateSet.size() > 0 )
        return;
        
      for( stateIterator sit = addStateSet.begin();
            sit != addStateSet.end(); sit++ )
        initialStates.add(*sit);
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
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::removeInitialState( State<StKey> name ) 
    {
      return initialStates.remove(name);
    }

  
    /**
     *
     * @brief remove all states from the initial state set of the NWA
     *
     */
    template<typename StKey,typename Label > 
    void NWA<StKey,Label>::clearInitialStates( )
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
    template<typename StKey,typename Label > 
    typename NWA<StKey,Label>::stateIterator NWA<StKey,Label>::beginInitialStates()
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
    template<typename StKey,typename Label > 
    typename NWA<StKey,Label>::stateIterator NWA<StKey,Label>::endInitialStates()
    {
      return initialStates.end();
    }
  
  
    //Final States
    /**
     *
     * @brief get all final states
     *
     * @return set of Key values for all final states associated with 
     * the NWA
     *
     */
    template<typename StKey,typename Label > 
    const typename NWA<StKey,Label>::States & NWA<StKey,Label>::getFinalStates( )
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
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::isFinalState( State<StKey> name )
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
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::addFinalState( State<StKey> name )
    {
      if( !states.contains(name) )
        states.add(name);
      return finalStates.add(name);
    }

    /**
     *
     * @brief add all the states in the given StateSet to the
     * final state set associated with this NWA
     *
     * @param the StateSet that contains the states to add
     *
     */
    template<typename StKey,typename Label > 
    void NWA<StKey,Label>::addAllFinalStates( States addStateSet)
    {
      for( stateIterator sit = addStateSet.begin();
            sit != addStateSet.end(); sit++ )
        finalStates.add(*sit);
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
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::removeFinalState( State<StKey> name )
    {
      return finalStates.remove(name);
    }
    
   
    /**
     *
     * @brief remove all states from the final state set of the NWA
     *
     */
    template<typename StKey,typename Label > 
    void NWA<StKey,Label>::clearFinalStates( )
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
    template<typename StKey,typename Label > 
    typename NWA<StKey,Label>::stateIterator NWA<StKey,Label>::beginFinalStates()
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
    template<typename StKey,typename Label >   
    typename NWA<StKey,Label>::stateIterator NWA<StKey,Label>::endFinalStates()
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
    template<typename StKey,typename Label > 
    size_t NWA<StKey,Label>::sizeStates( )
    {
      return states.size();
    }
    
    /**
     *
     * @brief returns the number of initial states in this collection
     *
     * @return the number of initial states in this collection
     *
     */
    template<typename StKey,typename Label > 
    size_t NWA<StKey,Label>::sizeInitialStates( )
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
    template<typename StKey,typename Label > 
    size_t NWA<StKey,Label>::sizeFinalStates( )
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
    template<typename StKey,typename Label > 
    const StateSet<Symbol<Label>> & NWA<StKey,Label>::getSymbols( )
    {
        return trans->getSymbols();
    }
  
    /**
     *
     * @brief test if the given symbol is a symbol of the NWA
     *
     * @param the symbol to check
     * @return true if the given symbol is a symbol of the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::isSymbol( Symbol<Label> sym )
    {
      return trans->isSymbol(sym);
    }
   
   
    /**
     *
     * @brief add the given symbol to the NWA
     *
     * @param symbol to add
     * @return false if the symbol is already associated with the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::addSymbol( Symbol<Label> sym )
    {
      return trans->addSymbol(sym);
    }
    
   
    /**
     *
     * @brief remove the given symbol from the NWA
     *
     * @param the symbol to remove
     * @return false if the symbol is no associated with the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::removeSymbol( Symbol<Label> sym )
    {
      return trans->removeSymbol(sym);
    }
    
   
    /**
     *
     * @brief remove all symbols associated with the NWA
     *
     */
    template<typename StKey,typename Label > 
    void NWA<StKey,Label>::clearSymbols( )
    {
      trans->clearSymbols();
    }
    
  
  
    //Transition Accessors

    /**
     *
     * @brief remove all transitions from the NWA
     *
     */
    template<typename StKey,typename Label > 
    void NWA<StKey,Label>::clearTrans( )
    {
      trans->clear();
    }
  
  
    //Transitions
    /**
     *
     * @brief get all call transitions in the transition set of the NWA.
     *
     * @return set of call transitions associated with the NWA
     * 
     */
    template<typename StKey,typename Label > 
    const typename NWA<StKey,Label>::Calls & NWA<StKey,Label>::getCallTrans( )
    {
      return trans->getCalls();
    }
 
    /**
     *
     * @brief add a call transition to the NWA
     *
     * @param from: name of the State the edge departs from
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the State the edge arrives to  
     * @return false if the call transition already exists in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::addCallTrans( Key from, Key sym, Key to )
    {
      if(! isState(from)
          || ! isState(to) )
        return false;
        
      KeyTriple ct = KeyTriple(from,sym,to);
      return trans->addCall(ct);
    }
  
    /**
     *
     * @brief add a call transition to the NWA
     *
     * @param the call transition to add
     * @return false if the call transition already exists in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::addCallTrans( KeyTriple * ct )
    {
      if(! isState(ct->first)
          || ! isState(ct->third) )
        return false;
        
      return trans->addCall(*ct);
    }
  
    /**
     *
     * @brief remove the call transition with the given edge and label 
     * information from the NWA
     *
     * @param from: name of the State the edge departs from
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the State the edge arrives to  
     * @return false if the call transition does not exist in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::removeCallTrans( Key from, Key sym, Key to )
    {
      KeyTriple ct = KeyTriple(from,sym,to);
      return trans->removeCall(ct);
    }
  
    /**
     *
     * @brief remove a call transition from the NWA
     *
     * @param the call transition to remove
     * @return false if the call transition does not exist in the NWA
     *
     */
    template<typename StKey,typename Label> 
    bool NWA<StKey,Label>::removeCallTrans( KeyTriple * ct )
    {
      return trans->removeCall(*ct);
    }
  
    /**
     *
     * @brief provide access to the beginning of the call transition set
     *
     * @return an iterator pointing to the beginning of the call transition
     *  set
     *
     */
    template<typename StKey,typename Label>
    typename NWA<StKey,Label>::callIterator NWA<StKey,Label>::beginCallTrans()
    {
      return trans->beginCall();
    }
        
    /**
     * 
     * @brief provide access to the end of the call transition set
     *
     * @return an iterator pointing just past the end of the call 
     * transition set
     *
     */
    template<typename StKey,typename Label>
    typename NWA<StKey,Label>::callIterator NWA<StKey,Label>::endCallTrans()
    {
      return trans->endCall();
    }
  
    /**
     *
     * @brief get all internal transitions in the transition set of the 
     * NWA.
     *
     * @return set of internal transitions associated with the NWA
     *
     */
    template<typename StKey,typename Label > 
    const typename NWA<StKey,Label>::Internals & NWA<StKey,Label>::getInternalTrans( )
    {
      return trans->getInternals();
    }
  
    /**
     *
     * @brief add an internal transition to the NWA
     *
     * @param from: name of the state the edge departs from
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the state the edge arrives to  
     * @return false if the internal transition already exists in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::addInternalTrans( Key from, Key sym, Key to )
    {
      if(! isState(from)
          || ! isState(to) )
        return false;
    
      KeyTriple it = KeyTriple(from,sym,to);
      return trans->addInternal(it);
    }
 
    /**
     *
     * @brief add an internal transition to the NWA
     *
     * @param internal transition to add to the NWA
     * @return false if the internal transition already exists in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::addInternalTrans( KeyTriple * it )
    {
      if(! isState(it->first)
          || ! isState(it->third) )
        return false;
    
      return trans->addInternal(*it);
    }
  
    /**
     *
     * @brief remove the internal transition with the given edge and 
     * label information from the NWA
     * 
     * @param from: name of the state the edge departs from
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the state the edge arrives to  
     * @return false if the internal transition does not exist in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::removeInternalTrans( Key from, Key sym, Key to )
    {
      KeyTriple it = KeyTriple(from,sym,to);
      return trans->removeInternal(it);
    }
  
    /**
     *
     * @brief remove an internal transition from the NWA
     *
     * @param the internal transition to remove
     * @return false if the internal transition does not exist in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::removeInternalTrans( KeyTriple * it )
    {
      return trans->removeInternal(*it);
    }
  
    /**
     *
     * @brief provide access to the beginning of the internal transition set
     *
     * @return an iterator pointing to the beginning of the internal transition
     *  set
     *
     */
    template<typename StKey,typename Label>
    typename NWA<StKey,Label>::internalIterator NWA<StKey,Label>::beginInternalTrans()
    {
      return trans->beginInternal();
    }
        
    /**
     * 
     * @brief provide access to the end of the internal transition set
     *
     * @return an iterator pointing just past the end of the internal
     * transition set
     *
     */
    template<typename StKey,typename Label>
    typename NWA<StKey,Label>::internalIterator NWA<StKey,Label>::endInternalTrans()
    {
      return trans->endInternal();
    }
  
    /**
     *
     * @brief get all return transitions in the transition set of the NWA.
     *
     * @return set of return transitions associated with the NWA
     *
     */
    template<typename StKey,typename Label > 
    const typename NWA<StKey,Label>::Returns & NWA<StKey,Label>::getReturnTrans( )
    {
      return trans->getReturns();
    }
   
    /**
     *
     * @brief add a return transition to the NWA
     *
     * @param from: name of the state the edge departs from
     * @param pred: name of the state from which the call was initiated  
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the state the edge arrives to  
     * @return false if the return transition already exists in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::addReturnTrans( Key from, Key pred, Key sym, Key to )
    {
      if(! isState(from)
          || ! isState(pred)
          || ! isState(to) )
        return false;
    
      KeyQuad rt = KeyQuad(from,pred,sym,to);
      return trans->addReturn(rt);
    }
   
    /**
     *
     * @brief add a return transition to the NWA
     *
     * @param return transition to add to the NWA
     * @return false if the return transition already exists in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::addReturnTrans( KeyQuad * rt )
    {
      if(! isState(rt->first)
          || ! isState(rt->second)
          || ! isState(rt->fourth) )
        return false;
        
      return trans->addReturn(*rt);
    }
  
    /**
     *
     * @brief remove the return transition with the given edge and label 
     * information from the NWA
     *
     * @param from: name of the state the edge departs from
     * @param pred: name of the state from which the call was initiated  	   
     * @param sym: name of the symbol labeling the edge
     * @param to: name of the state the edge arrives to  
     * @return false if the return transition does not exist in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::removeReturnTrans( Key from, Key pred, Key sym, Key to )
    {
      KeyQuad rt = KeyQuad(from,pred,sym,to);
      return trans->removeReturn(rt);
    }
   
    /**
     *
     * @brief remove a return transition from the NWA
     *
     * @param the return transition to remove
     * @return false if the return transition does not exist in the NWA
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::removeReturnTrans( KeyQuad * rt )
    {
      return trans->removeReturn(*rt);
    }
    
    /**
     *
     * @brief provide access to the beginning of the return transition set
     *
     * @return an iterator pointing to the beginning of the return transition
     *  set
     *
     */
    template<typename StKey,typename Label>
    typename NWA<StKey,Label>::returnIterator NWA<StKey,Label>::beginReturnTrans()
    {
      return trans->beginReturn();
    }
        
    /**
     * 
     * @brief provide access to the end of the return transition set
     *
     * @return an iterator pointing just past the end of the return
     * transition set
     *
     */
    template<typename StKey,typename Label>
    typename NWA<StKey,Label>::returnIterator NWA<StKey,Label>::endReturnTrans()
    {
      return trans->endReturn();
    }
  
    //Building NWAs

    /**
     *
     * @brief constructs the NWA resulting from the union of this NWA and
     * the NWA 'other'
     *
     * @param the NWA with which to union this NWA
     * @return the NWA which is the union of this NWA and the NWA 'other'
     *
     */
    template<typename StKey,typename Label > 
    NWA<StKey,Label> * NWA<StKey,Label>::unionNWA( NWA<StKey,Label> other )
    {
      NWA<StKey,Label> * newNWA = new NWA<StKey,Label>();
      
      //Generate new states, adding them to the state set as well as
      //to the initial state set and final state set as needed.
      Key newKey;
      for( StateSet<State<StKey>>::iterator fit = beginStates();
            fit != endStates(); fit++ )
      {
        for( StateSet<State<StKey>>::iterator sit = other.beginStates();
              sit != other.endStates(); sit++ )
        {
          if( (*fit == WALI_BAD_KEY) && (*sit == WALI_BAD_KEY) )
            newKey = WALI_BAD_KEY;         
          else
            newKey = wali::getKey(fit->getKey(),sit->getKey());
          
          newNWA->addState(newKey);
          
          if( this->isInitialState(*fit) && other.isInitialState(*sit) )
            newNWA->addInitialState(newKey);
          if( this->isFinalState(*fit) || other.isFinalState(*sit) )
            newNWA->addFinalState(newKey);      
        }
      }
      
      //Generate call transitions.
      for( Trans::callIterator fcit = this->trans->beginCall();
            fcit != this->trans->endCall(); fcit++ )
      {
        for( Trans::callIterator scit = other.trans->beginCall();
            scit != other.trans->endCall(); scit++ )
        {
          if( fcit->second == scit->second )
            newNWA->addCallTrans( wali::getKey(fcit->first,scit->first),
                                  fcit->second, 
                                  wali::getKey(fcit->third,scit->third) );
        }
      }
      //Generate internal transitions.
      for( Trans::internalIterator fiit = this->trans->beginInternal();
            fiit != this->trans->endInternal(); fiit++ )
      {
        for( Trans::internalIterator siit = other.trans->beginInternal();
              siit != other.trans->endInternal(); siit++ )
        {
          if( fiit->second == siit->second )
            newNWA->addInternalTrans( wali::getKey(fiit->first,siit->first),
                                      fiit->second, 
                                      wali::getKey(fiit->third,siit->third) );
        }
      }
      //Generate return transitions.  
      for( Trans::returnIterator frit = this->trans->beginReturn();
            frit != this->trans->endReturn(); frit++ )
      {
        for( Trans::returnIterator srit = other.trans->beginReturn();
              srit != other.trans->endReturn(); srit++ )
        {
          if( frit->third == srit->third )
            newNWA->addReturnTrans( wali::getKey(frit->first,srit->first),
                                    wali::getKey(frit->second,srit->second), 
                                    frit->third,
                                    wali::getKey(frit->fourth,srit->fourth) );
        }
      }   
      
      return newNWA;
    }
  
    /**
     *
     * @brief constructs the NWA resulting from the concatenation of this
     * NWA and the NWA 'other'
     *
     * @param the NWA with which to concatenate this NWA
     * @return the NWA which is the concatenation of this NWA and the NWA
     * 'other'
     * 
     */
    template<typename StKey,typename Label > 
    NWA<StKey,Label> * NWA<StKey,Label>::concat( NWA<StKey,Label> other )
    {
      NWA<StKey,Label> * newNWA = new NWA<StKey,Label>();
      
      //Join the state sets
      newNWA->addAllStates(this->states);
      newNWA->addAllStates(other.states);
      
      //Initial states of the new NWA are the initial states of this NWA
      newNWA->addAllInitialStates(this->initialStates);
      
      //Idea:
      //To speed this up we could create a new state, connect all final
      //states to this state, and connect this state to all initial states.
      //Then it will take time O(m+n) instead of O(m*n) where m and n are
      //the sizes of the initial and final state sets.
      
      //Add internal epsilon transitions from the final states of the first 
      //NWA to the initial states of the second NWA.
      for( stateIterator fit = beginFinalStates();
        fit != endFinalStates(); fit++ )
      {
        for( stateIterator iit = other.beginInitialStates();
          iit != other.endInitialStates(); iit++)
        {
          KeyTriple newTrans = KeyTriple(fit->getKey(),WALI_EPSILON,iit->getKey());
          newNWA->addInternalTrans( &newTrans );
        }
      }
      
      //Final states of the new NWA are the final states of the NWA 'other'
      newNWA->addAllFinalStates(other.finalStates);

      //Join the transition sets
      newNWA->trans->addAllTrans(*this->trans);
      newNWA->trans->addAllTrans(*other.trans);

      return newNWA;
    }
  
    /**
     *
     * @brief constructs the NWA resulting from performing Kleene-* on 
     * this NWA
     *
     * @return the NWA which is the result of performing Kleene-* on 
     * this NWA
     *
     */
    //TODO: check this with respect to calls/returns
    template<typename StKey,typename Label >
    NWA<StKey,Label> * NWA<StKey,Label>::star( )
    {
      NWA<StKey,Label> * newNWA = new NWA<StKey,Label>();
      
      //Add states.
      newNWA->addAllStates(states);
      newNWA->addAllInitialStates(initialStates);
      newNWA->addAllFinalStates(finalStates);
      
      //Add trans.
      newNWA->trans->addAllTrans(*this->trans);
      
      //Idea:
      //The same possible speedup as in concat applies here.
      
      //Add internal epsilon transitions from final states to initial states.
      for( stateIterator fit = beginFinalStates();
        fit != endFinalStates(); fit++ )
      {
        for( stateIterator iit = beginInitialStates();
          iit != endInitialStates(); iit++ )
        {
          KeyTriple newTrans = KeyTriple(fit->getKey(),WALI_EPSILON,iit->getKey());
          newNWA->addInternalTrans( &newTrans );
        }
      }
  
      return newNWA;
    }
 
    /**
     *
     * @brief constructs the NWA that is the complement of this NWA
     *
     * @return the NWA which is the complement of this NWA
     *
     */
    template<typename StKey,typename Label > 
    NWA<StKey,Label> * NWA<StKey,Label>::complement( )
    {
      NWA<StKey,Label> * newNWA = new NWA<StKey,Label>();
    
      //Start with a deterministic NWA.
      if(! this->isDeterministic() )
        determinize();
      
      //The new NWA will have all the same states and initial states.
      newNWA->addAllStates(states);
      newNWA->addAllInitialStates(initialStates);
      
      //The new NWA will have all the same transitions.
      newNWA->trans->addAllTrans(*this->trans);    
    
      //All states that are final states of this NWA
      //are not final states of the new NWA and all states 
      //not in the final state set of this NWA are final
      //states of the new NWA.
      States oldFinalStates = States();
      for( stateIterator fit = beginFinalStates();
            fit != endFinalStates(); fit++ )
        oldFinalStates.add(*fit);
      
      for( stateIterator sit = beginStates();
            sit != endStates(); sit++ )
      {
        if( !oldFinalStates.contains(*sit) )
          newNWA->addFinalState(*sit);
      }
          
      return newNWA;
    }
  
    /**
     * @brief constructs the NWA which is the reverse of this NWA
     *
     * This method constructs the NWA which is the reverse of this NWA.
     *
     * @return the NWA which is the reverse of this NWA
     * 
     */
    template<typename StKey,typename Label > 
    NWA<StKey,Label> * NWA<StKey,Label>::reverse()
    {
      NWA<StKey,Label> * newNWA = new NWA<StKey,Label>();
          
      //The states remain the same.
      newNWA->addAllStates(states);
    
      //Swap the initial state set with the final state set.
      for( stateIterator it = beginFinalStates();
            it != endFinalStates(); it++ )
        newNWA->addInitialState(*it);
      for( stateIterator it = beginInitialStates();
            it != endInitialStates(); it++ )
        newNWA->addFinalState(*it);
      
      //Reverse all transitions.
      for( Trans::internalIterator iit = this->trans->beginInternal();
            iit != this->trans->endInternal(); iit++ )
      {
        newNWA->addInternalTrans(iit->third,iit->second,iit->first);
      }         
      
      for( Trans::returnIterator rit = this->trans->beginReturn();
            rit != this->trans->endReturn(); rit++ )
      {
        newNWA->addCallTrans(rit->third,rit->second,rit->first);
      }
      for( Trans::callIterator cit = this->trans->beginCall();
            cit != this->trans->endCall(); cit++ )
      {
        for( Trans::returnIterator rit = this->trans->beginReturn();
              rit != this->trans->endReturn(); rit++ )
        {
          if( cit->first == rit->second )
          {
            newNWA->addReturnTrans(cit->third,rit->first,
                                    cit->second,cit->first);
            break;
          } 
        }
      }
      
      return newNWA;
    }
  
    /**
     *
     * @brief constructs the NWA which is the intersection of this NWA
     * and the NWA 'other'
     *
     * @param the NWA with which to perform the intersection with this
     * NWA
     * @return the NWA which is the intersection of this NWA and the NWA
     * 'other'
     *
     */
    template<typename StKey,typename Label > 
    NWA<StKey,Label> * NWA<StKey,Label>::intersect( NWA<StKey,Label> other )
    {
      NWA<StKey,Label> * newNWA = new NWA<StKey,Label>();
      
      //Generate new states, adding them to the state set as well as
      //to the initial state set and final state set as needed.
      Key newKey;
      for( StateSet<State<StKey>>::iterator fit = beginStates();
            fit != endStates(); fit++ )
      {
        for( StateSet<State<StKey>>::iterator sit = other.beginStates();
              sit != other.endStates(); sit++ )
        {
          if( (*fit == WALI_BAD_KEY) && (*sit == WALI_BAD_KEY) )
            newKey = WALI_BAD_KEY;         
          else
            newKey = wali::getKey(fit->getKey(),sit->getKey());
          
          newNWA->addState(newKey);
          
          if( this->isInitialState(*fit) && other.isInitialState(*sit) )
            newNWA->addInitialState(newKey);
          if( this->isFinalState(*fit) && other.isFinalState(*sit) )
            newNWA->addFinalState(newKey);      
        }
      }
      
      //Generate call transitions.
      for( Trans::callIterator fcit = this->trans->beginCall();
            fcit != this->trans->endCall(); fcit++ )
      {
        for( Trans::callIterator scit = other.trans->beginCall();
            scit != other.trans->endCall(); scit++ )
        {
          if( fcit->second == scit->second )
            newNWA->addCallTrans( wali::getKey(fcit->first,scit->first),
                                  fcit->second, 
                                  wali::getKey(fcit->third,scit->third) );
        }
      }
      //Generate internal transitions.
      for( Trans::internalIterator fiit = this->trans->beginInternal();
            fiit != this->trans->endInternal(); fiit++ )
      {
        for( Trans::internalIterator siit = other.trans->beginInternal();
              siit != other.trans->endInternal(); siit++ )
        {
          if( fiit->second == siit->second )
            newNWA->addInternalTrans( wali::getKey(fiit->first,siit->first),
                                      fiit->second, 
                                      wali::getKey(fiit->third,siit->third) );
        }
      }
      //Generate return transitions.  
      for( Trans::returnIterator frit = this->trans->beginReturn();
            frit != this->trans->endReturn(); frit++ )
      {
        for( Trans::returnIterator srit = other.trans->beginReturn();
              srit != other.trans->endReturn(); srit++ )
        {
          if( frit->third == srit->third )
            newNWA->addReturnTrans( wali::getKey(frit->first,srit->first),
                                    wali::getKey(frit->second,srit->second), 
                                    frit->third,
                                    wali::getKey(frit->fourth,srit->fourth) );
        }
      }   
      
      return newNWA;
    }
  
    //Using NWAs
    
    /**
     * 
     * @brief constructs the WPDS which is the result of the explicit NWA 
     * plus WPDS construction from Advanced Querying for Property Checking
     *
     * @param the WPDS that forms the basis for the constructed WPDS
     * @return the WPDS which can be used to perform property checking 
     * using PDS reachability
     * 
     */
    template<typename StKey,typename Label > 
    wpds::WPDS NWA<StKey,Label>::plusWPDS( wpds::WPDS base )
    {
      wpds::WPDS result = wpds::WPDS();
      wpds::WpdsRules rules = wpds::WpdsRules();
      
      base.for_each(rules);
      
      //Step Rules:
      for( std::set< wpds::Rule >::iterator it = rules.stepRules.begin();
            it != rules.stepRules.end(); it++ )
      {
        //<p,n_1> -w-> <p',n_2> in delta_1, (q,n_1,q') in delta_i
        Key sym = it->from_stack();
        //look for internal transitions in the NWA with symbol sym
        for( std::set< KeyTriple >::iterator iit = trans->beginInternal();
            iit != trans->endInternal(); iit++ )
        {
          if( iit->second == sym )
          {
            //<(p,q),n_1> -w-> <(p',q'),n_2>
            Key from = getKey(it->from_state(),iit->first);
            Key to = getKey(it->to_state(),iit->third);
            result.add_rule(from,sym,to,it->to_stack1(),it->weight());
          }
        }
      }
      
      //Call Rules:
      for( std::set< wpds::Rule >::iterator it = rules.pushRules.begin();
            it != rules.pushRules.end(); it++ )
      {
        //<p,n_c> -w-> <p',e r_c> in delta_2, (q_c,n_c,q) in delta_c
        Key sym = it->from_stack();
        //look for call transitions in the NWA with symbol sym
        for( std::set< KeyTriple >::iterator cit = trans->beginCall();
              cit != trans->endCall(); cit++ )
        {
          if( cit->second == sym )
          {
            //<(p,q_c),n_c> -w-> <(p',q),e (r_c,q_c)>
            Key from = getKey(it->from_state(),cit->first);
            Key to = getKey(it->to_state(),cit->third);
            Key stack2 = getKey(it->to_stack2(),cit->first);
            result.add_rule(from,sym,to,it->to_stack1(),stack2,it->weight());
          }
        }
      }
      
      //Return Rules:
      for( std::set< wpds::Rule >::iterator it = rules.popRules.begin();
            it != rules.popRules.end(); it++ )
      {
        //<p,x> -w-> <p',*> in delta_0, (q_r,q_c,x,q) in delta_r
        Key sym = it->from_stack();
        //look for return transitions in the NWA with symbol sym
        for( std::set< KeyQuad >::iterator rit = trans->beginReturn();
              rit != trans->endReturn(); rit++ )
        {
          if( rit->third == sym )
          {
            //<(p,q_r),x> -w-> <(p',q_r^x),*>
            Key from = getKey(it->from_state(),rit->first);
            Key to = getKey( it->to_state(),getKey(rit->first,sym) );
            result.add_rule(from,sym,to,it->weight());
            //For all r_c in delta_2
            for( std::set< wpds::Rule >::iterator dit = rules.pushRules.begin();
                  dit != rules.pushRules.end(); dit++ )
            {
              //<(p',q_r^x),(r_c,q_c)> -1-> <(p',q),r_c>
              from = getKey( it->to_state(),getKey(rit->first,sym) );
              sym = getKey(dit->to_stack2(),rit->second);
              to = getKey(it->to_state(),rit->fourth);
              result.add_rule(from,sym,to,dit->to_stack2(),it->weight()->one());
            }
          }
        }
      }      
      
      return result;
    }
    
    /**
     *
     * @brief constructs the NWA equivalent to the given PDS
     *
     * @return the NWA equivalent to the given PDS
     *
     */
    template<typename StKey,typename Label > 
    nwa::NWA<StKey,Label> NWA<StKey,Label>::PDStoNWA(wpds::WPDS pds)
    {
      nwa::NWA<StKey,Label> result = nwa::NWA<StKey,Label>();
      
      std::map<Key,Key> call_return;
      
      wpds::WpdsRules rules = wpds::WpdsRules();
      
      pds.for_each(rules);
      
      //Step Rules:
      for( std::set< wpds::Rule >::iterator it = rules.stepRules.begin();
            it != rules.stepRules.end(); it++ )
      {
        //<p,n_1> -w-> <p',n_2> in delta_1 goes to ((p,n_1),n_1,(p',n_2)) in delta_i
        result.addInternalTrans(getKey( it->from_state(),it->from_stack() ),  //from
                                it->from_stack(),                             //sym
                                getKey( it->to_state(),it->to_stack1() ));    //to
      }
      
      //Call Rules:
      for( std::set< wpds::Rule >::iterator it = rules.pushRules.begin();
            it != rules.pushRules.end(); it++ )
      {
        //<p,n_c> -w-> <p',e r_c> in delta_2, ((p,n_c),c,(p',e)) in delta_c
        result.addCallTrans(getKey( it->from_state(),it->from_stack() ),      //from
                            it->from_stack(),                                 //sym
                            getKey( it->to_state(),it->to_stack1() ));        //to
        //add (p,n_c) => r_c to the call-return map
        call_return.insert(std::pair<Key,Key>(getKey( it->from_state(),it->from_stack() ),  //call
                                              it->to_stack2()));                            //ret
      }
      
      //Return Rules:
      for( std::set< wpds::Rule >::iterator it = rules.popRules.begin();
            it != rules.popRules.end(); it++ )
      {
        //<p,x> -w-> <p',*> in delta_0, ((p,x),(p,n_c),x,(p',r)) in delta_r
        for(std::map<Key,Key>::iterator cr_it = call_return.begin(); 
            cr_it != call_return.end(); cr_it++  )
          result.addReturnTrans(getKey( it->from_state(),it->from_stack() ),  //from
                                cr_it->first,                                 //pred
                                it->from_stack(),                             //sym
                                getKey( it->to_state(),cr_it->second ));      //to
      }
      
      return result;    
    }
    
    /**
     *
     * @brief constructs the PDS equivalent to this NWA
     *
     * @return the PDS equivalent to this NWA
     *
     */
    template<typename StKey,typename Label > 
    wpds::WPDS NWA<StKey,Label>::NWAtoPDS(NWA<StKey,Label> nwa,wali::sem_elem_t one)
    {
      wpds::WPDS result = wpds::WPDS();
      std::map< Key,Key > calls;
      
      Key program = wali::getKey("program"); 
      
      //Internal Transitions
      for( std::set< KeyTriple >::iterator iit = nwa.trans->beginInternal();
            iit != nwa.trans->endInternal(); iit++ )
      {
        result.add_rule(program,          //from_state
                        iit->first,       //from_stack
                        program,          //to_state
                        iit->third,       //to_stack1
                        one);       //weight      
      }
      
      //Call Transitions
      for( std::set< KeyTriple >::iterator cit = nwa.trans->beginCall();
            cit != nwa.trans->endCall(); cit++ )
      {
        for( std::set< KeyQuad >::iterator rit = nwa.trans->beginReturn();
              rit != nwa.trans->endReturn(); rit++ )
          if( cit->first == rit->second )
          {
            //for each return site with cit->first as call site ...
            Key ret = getKey(rit->fourth,rit->fourth); // (r,r)
            calls.insert(std::pair<Key,Key>(cit->first,ret));
            
            result.add_rule(program,          //from_state
                            cit->first,       //from_stack
                            program,          //to_state
                            cit->third,       //to_stack1
                            ret,              //to_stack2
                            one);       //weight  
          }  
      }
      
      //Return Transitions
      for( std::set< KeyQuad >::iterator rit = nwa.trans->beginReturn();
            rit != nwa.trans->endReturn(); rit++ )
      {
        Key rstate = getKey(program,rit->first); // (p,x_i)
        result.add_rule(program,          //from_state
                        rit->first,       //from_stack
                        rstate,           //to_state
                        one);       //weight 
        
        std::map<Key,Key>::iterator ret = calls.find(rit->second);
        if( ret != calls.end() )          
          result.add_rule(rstate,         //from_state
                          ret->second,    //from_stack
                          program,        //to_state
                          rit->fourth,    //to_stack
                          one);     //weight    
        
      }
      
      return result;
    }

    /**
     *
     * @brief constructs a deterministic NWA that is equivalent to this 
     * NWA.
     *
     * @return the NWA which is a deterministed equivalent to this NWA
     *
     */
    template<typename StKey,typename Label > 
    NWA<StKey,Label> & NWA<StKey,Label>::determinize( )
    {
      std::pair<NWA<StKey,Label>,StateMap> tmp = std::pair<NWA<StKey,Label>,StateMap>();
            
      //New initial state is {(q,q) | q is an element of Q (possibly Q_in?)}
      Key newInitialState;
      std::set<Key> initialStateSet;
      StatePairSet initialState = StatePairSet(); 
      bool first = true;
      for( stateIterator sit = beginStates();
            sit != endStates(); sit++ )
      {
        initialState.insert( StatePair(*sit,*sit) );
        if( first )
        {
          initialStateSet.insert(wali::getKey(sit->getKey(),sit->getKey()));
          first = false;
        }
        else
          initialStateSet.insert(wali::getKey(sit->getKey(),sit->getKey()));
      }
      
      newInitialState = wali::getKey(initialStateSet);
      //Add the initial state to the deterministic NWA.
      tmp.first.addInitialState(newInitialState);      
      //Add to the map the initial state set and the initial state.
      tmp.second.insert(std::pair<StatePairSet,Key>(initialState,newInitialState));
      
      tmp = det(tmp.first,tmp.second,initialState,std::stack<StatePairSet>());
          
      //A state is final if it contains a pair of the form (q,q') with
      //q an element of Q_in and q' an element of Q_f. 
      for( StateMap::iterator it = tmp.second.begin();
            it != tmp.second.end(); it++ )
      {
        for( StatePairSet::const_iterator sit = it->first.begin();
              sit != it->first.end(); sit++ )
        {
          if( isInitialState(sit->first) && 
              isFinalState(sit->second) )
          {
              tmp.first.addFinalState(it->second);
              break;
          }
        }          
      }
   
      return tmp.first;
    }
    
    /**
     *
     * @brief tests whether the NWA is deterministic 
     *
     * @return true if the NWA is deterministic, false otherwise
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::isDeterministic( )
    {
      //TODO: optimize!!!
      //An NWA is not deterministic if there is not exactly one initial state
      if( sizeInitialStates() != 1)
        return false;

      //An NWA is not deterministic if there are epsilon transitions to states
      //other than the error state or if there are multiple transitions of some
      //type that are identical except for the to state.
      for( Trans::symbolIterator it = trans->beginSymbols();
            it != trans->endSymbols(); it++ )
      {
        for( stateIterator sit = beginStates();
              sit != endStates(); sit++ )
        {
          //Check call transitions.
          int count = 0;
          for( Trans::callIterator cit = trans->beginCall();
                cit != trans->endCall(); cit++ )
          {
            //Epsilon transition to a state other than the error state.
            if( *it == wali::WALI_EPSILON )
              if( (cit->second == it->getLabelKey()) && (cit->third != wali::WALI_BAD_KEY) )
                return false;
            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (cit->first == sit->getKey()) && (cit->second == it->getLabelKey()) )
              count++;
          }
          if( count > 1 )
            return false;
          
          //Check internal transitions.
          count = 0;
          for( Trans::internalIterator iit = trans->beginInternal();
                iit != trans->endInternal(); iit++ )
          {
            //Epsilon transition to a state other than the error state.
            if( *it == wali::WALI_EPSILON )
              if( (iit->second == it->getLabelKey()) && (iit->third != wali::WALI_BAD_KEY) )
                return false;
            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (iit->first == sit->getKey()) && (iit->second == it->getLabelKey()) )
              count++;
          }
          if( count > 1 )
            return false;
          
          for( stateIterator pit = beginStates();
                pit != endStates(); pit++ )
          {
            //Check return transitions.
            count = 0;
            for( Trans::returnIterator rit = trans->beginReturn();
                  rit != trans->endReturn(); rit++ )
            {
              //Epsilon transition to a state other than the error state.
              if( *it == wali::WALI_EPSILON )
                if( (rit->third == it->getLabelKey()) && (rit->fourth != wali::WALI_BAD_KEY) )
                  return false;
              //Keep a count of multiple transitions with the same from
              //state and symbol(that is not epsilon).
              else if( (rit->first == sit->getKey()) &&
                  (rit->second == pit->getKey()) &&
                  (rit->third == it->getLabelKey()) )
                count++;
            }
            if( count > 1 )
              return false;
          }
        }
      }
            
      return true;
    }
  
    /**
     *
     * @brief tests whether the language accepted by this NWA is empty
     *
     * @return true if the language accepted by this NWA is empty
     *
     */
    //TODO: determinize prior to checking paths?
    template<typename StKey,typename Label >
    bool NWA<StKey,Label>::isEmpty( )
    {
      //An automaton with no initial states must accept only the empty
      //language.
      if( sizeInitialStates() == 0 )
        return true;
        
      //An automaton with no final states must accept only the empty 
      //language.
      if( sizeFinalStates() == 0 )
        return true;
        
      //An automaton with no path from an initial state to a final state
      //must accept only the empty language.       
      for( stateIterator it = beginInitialStates();
            it != endInitialStates(); it++ )
      {
        //If an initial state is also a final state, then
        //the empty string is accepted.  Therefore the
        //accepted language is not the empty language.
        if( isFinalState(*it) )
          return false;
        std::set< Key > visited = std::set< Key >();
        visited.insert(it->getKey());
        if( hasPath(it->getKey(), std::stack< Key >(), visited) )
          return false;
        visited.erase(it->getKey());
      }
      return true;
    }
  
    /**
     *
     * @brief tests whether the given nested word is a member of the 
     * language accepted by this NWA
     *
     * @param the nested word to test
     * @return true if the given nested word is a member of the language
     * accepted by this NWA
     *
     */
    template<typename StKey,typename Label >
    bool NWA<StKey,Label>::isMember( nws::NWS word )
    {
      std::deque<WordRecConfig<StKey>> configs;
      
      //Add a dummy call state for each call on the word stack
      for( wali::nws::NWS::iterator it = word.beginStack();
            it != word.endStack(); it++ );
      {
        WordRecConfig<StKey> config = WordRecConfig<StKey>(word);
        for( StateSet<State<StKey>>::iterator sit = initialStates.begin();
              sit != initialStates.end(); sit++ )
          config.addCallState(*sit);
        
        configs.push_back(config);
      }
        
      //Simulate the nested word 'word' on the NWA.
      for( StateSet<State<StKey>>::iterator sit = initialStates.begin();
            sit != initialStates.end(); sit++ ) 
      {
        //Follow the word from here and try to match it.
        if( matchWord(*sit,configs) )
          return true; 
      }
      return false;
    }
 
    //Utilities	

    /**
     *
     * @brief print the NWA
     *
     * @param the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template<typename StKey,typename Label >
    std::ostream & NWA<StKey,Label>::print( std::ostream & o) const
    {
      o << "Q: ";
      states.print(o);
      
      o << "Q0: ";
      initialStates.print(o);
      
      o << "Qf: ";
      finalStates.print(o);
    
      trans->print(o);
      return o;
    }
 
    /**
     *
     * @brief tests whether this NWA is equivalent to the NWA 'other'
     *
     * @param the NWA to compare this NWA to
     * @return true if this NWA is equivalent to the NWA 'other'
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::operator==( NWA<StKey,Label> & other )
    {
      return ( (states == other.states) &&
                (initialStates == other.initialStates) &&
                (finalStates == other.finalStates) &&
                (trans == other.trans) );
    }
  
    /**
     *
     * @brief returns the number of states associated with this NWA
     *
     * @return the number of states associated with this NWA
     *
     */
    template<typename StKey,typename Label > 
    size_t NWA<StKey,Label>::numStates( )
    {
      return sizeStates();
    }
  
    /**
     *
     * @brief returns the number of transitions (of all kinds) 
     * associated with this NWA
     *
     * @return the number of transitions (of all kinds) associated
     * with this NWA
     *
     */
    template<typename StKey,typename Label > 
    size_t NWA<StKey,Label>::numTrans( )
    {
      return trans->size();
    }
    
    /**
     *
     * @brief tests whether the NWA has a path from the current state to
     * a final state given the call stack and the set of states already visited
     *
     * @param currState: the state from which to start searching for a path
     * @param calls: the stack of calls that have been made thus far
     * @param visited: the states which have already been visited
     * @return true if there is a path through the NWA from the current state 
     * to a final state, false otherwise
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::hasPath( Key currState, std::stack< Key > calls, std::set< Key > visited )
    {
      //Try each possible transition out of this state to see if a final state
      //can be reached.
      
      //First try call transitions.
      calls.push(currState);
      for( Trans::callIterator cit = trans->beginCall();
            cit != trans->endCall(); cit++ )
      {
        //If the call starts from the current state and goes to a state not
        //yet visited, follow the path.
        if( (cit->first == currState) && (visited.count(cit->third) == 0) )
        {
          visited.insert(cit->third);
          if( hasPath(cit->third,calls,visited) )
              return true;
          visited.erase(cit->third);
        }    
      }
      calls.pop();
      
      //If no call transition led to a path, try internal transitions.
      for( Trans::internalIterator iit = trans->beginInternal();
            iit != trans->endInternal(); iit++ )
      {
        //If the call starts from the current state and goes to a state not
        //yet visited, follow the path.
        if( (iit->first == currState) && (visited.count(iit->third) == 0) )
        {
          visited.insert(iit->third);
          if( hasPath(iit->third,calls,visited) )
              return true;
          visited.erase(iit->third);
        }    
      }
      
      //If no internal transition led to a path, try return transitions.
      Key callState = calls.top();
      calls.pop();
      for( Trans::returnIterator rit = trans->beginReturn();
            rit != trans->endReturn(); rit++ )
      {
         if( (rit->first == currState) &&
             (rit->second == callState) &&
             (visited.count(rit->fourth) == 0) )
          {
            visited.insert(rit->fourth);
            if( hasPath(rit->fourth,calls,visited) )
              return true;
            visited.erase(rit->fourth);
          }    
        }
        calls.push(callState);      
      
      return false; 
    }
    
    /** 
     *
     * @brief tests whether the nested word suffix is accepted by 
     * this nested word automaton starting at the state with the
     * given name
     *
     * @parm currState: the state in the nested word automaton at 
     * which to start the simulation
     * @parm word: the nested word suffix to match
     * @return true if the simulation ends in an accepting state
     *
     */
    template<typename StKey,typename Label > 
    bool NWA<StKey,Label>::matchWord( State<StKey> currState, std::deque<WordRecConfig<StKey>> configs )
    {
      WordRecConfig<StKey> config = configs.front();
      configs.pop_front();  //remove this config
      
      //Base case: the word suffix has been completely processed 
      // and acceptance is determined by whether the current
      // state is a final state or not.
      if( config.isEmpty() )
      {
        return isFinalState(currState);        
      }
      else if( !config.hasNext() )
      {
        return false;
      }
      
      //Induction: the next node in the word suffix must be 
      // processed and the current state of the automaton
      // advanced properly.      
      nws::NWSNode * currNode = config.getNext();
      config.removeNext();      
      
      //Check for transitions that match.
      
      //Nodes with an exit node match call transitions
      //Nodes with no exit node match
      //  return transitions if the node is the node at the top of the nesting
      //  internal transitions otherwise
      if( currNode->isCall() )
      {
        config.addCallState(currState);
        
        for( Trans::callIterator cit = trans->beginCall();
              cit != trans->endCall(); cit++ )
        {
          //If this symbol in the word can be matched, try matching
          //the rest of the word.
          if( (cit->first == currState.getKey()) &&
              (cit->second == currNode->getSymbol()) )
          {
            configs.push_back(config);
            if( matchWord(cit->third,configs) )
              return true;
          }    
        }
        config.removeCallState();
        //If no call transition led to a matched word, there is no
        //accepting path through this state for the given word suffix.
        return false;
      }
      else if( currNode == config.getCall() )
      {
        State<StKey> callState = config.getCallState();
        config.removeCallState();
        for( Trans::returnIterator rit = trans->beginReturn();
              rit != trans->endReturn(); rit++ )
        {
          //If this symbol in the word can be matched, try matching
          //the rest of the word.
          if( (rit->first == currState.getKey()) &&
              (rit->second == callState.getKey()) &&
              (rit->third == currNode->getSymbol()) )
          {
            configs.push_back(config);
            if( matchWord(rit->fourth,configs) )
              return true;
          }    
        }
        config.addCallState(callState);
        //If no return transition led to a matched word, there is no
        //accepting path through this state for the given word suffix.
        return false;
      }
      else
      {
        for( Trans::internalIterator iit = trans->beginInternal();
              iit != trans->endInternal(); iit++ )
        {
          //If this symbol in the word can be matched, try matching
          //the rest of the word.
          if( (iit->first == currState.getKey()) &&
              (iit->second == currNode->getSymbol()) )
          {
            configs.push_back(config);
            if( matchWord(iit->third,configs) )
              return true;
          }    
        }
        //If no internal transition led to a matched word, there is no
        //accepting path through this state for the given word suffix.
        return false;
      }
      return false;
    }
    
    
    /**
     *
     * @brief constructs the transition table and state map for the deterministic 
     * NWA that is equivalent to this NWA.
     *
     * @return the transition table and state map for the deterministic NWA 
     * that is equivalent to this NWA
     *
     */
    template<typename StKey,typename Label > 
    std::pair<NWA<StKey,Label>,typename NWA<StKey,Label>::StateMap> NWA<StKey,Label>::det( NWA<StKey,Label> nWdAut,StateMap stateMap, StatePairSet currState,
                                          std::stack<StatePairSet> callPred )
    {
      //Internal Transition
      for( Trans::symbolIterator it = nWdAut.trans->beginSymbols();
            it != nWdAut.trans->endSymbols(); it++ )
      {        
        StatePairSet internalTrans;
        //Find all internal transitions that use this symbol.
        for( Trans::internalIterator iit = nWdAut.trans->beginInternal();
              iit != nWdAut.trans->endInternal(); iit++ )
        {
          //If the symbols match, make a new state pair set.
          if( *it == iit->second )
          {
            internalTrans.insert( StatePair(iit->first,iit->second) );
          }
        }
        //At an internal position labeled a, the automaton replaces each
        //pair (q,q') in the current state by pairs of the form (q,q'') 
        //such that(q',a,q'') is an element of Delta_i.
        Key tmpInternalKey;
        StatePairSet tmpInternal;
        bool first;
        for( StatePairSet::iterator sit = currState.begin();
              sit != currState.end(); sit++ )
        {
          bool replaced = false;
          for( StatePairSet::iterator iit = internalTrans.begin();
                iit != internalTrans.end(); iit++ )
          {
            if( sit->second == iit->first )
            {
              tmpInternal.insert(StatePair(sit->first,iit->second));
              if(first)
              {
                tmpInternalKey = wali::getKey(sit->first.getKey(),iit->second.getKey());
                first = false;
              }
              else
                tmpInternalKey = wali::getKey( wali::getKey(sit->first.getKey(),iit->second.getKey()), tmpInternalKey); 
              replaced = true;
            }
          }
          if(!replaced)
          {
            tmpInternal.insert(*sit);
            if(first)
            {
              tmpInternalKey = wali::getKey(sit->first.getKey(),sit->second.getKey());
              first = false;
            }
            else
              tmpInternalKey = wali::getKey( wali::getKey(sit->first.getKey(),sit->second.getKey()), tmpInternalKey);
          }
        }
              
        //We only want to add states that don't already exist.        
        bool recurse = false;
        if( stateMap.count(tmpInternal) == 0 )
        {
          //Add the new key to the map.
          stateMap.insert(std::pair<StatePairSet,Key>(tmpInternal,tmpInternalKey));
          nWdAut.addState(tmpInternalKey);
          recurse = true;
        }
        else
          tmpInternalKey = (stateMap.find(tmpInternal))->second.getKey();
            
        //Add a transition.
        KeyTriple newInternalTrans = 
              KeyTriple(stateMap.find(currState)->second.getKey(),it->getLabelKey(),tmpInternalKey);
        nWdAut.trans->addInternal(newInternalTrans);
        
        if(recurse)
        {
          std::pair<NWA<StKey,Label>,StateMap> result = det(nWdAut,stateMap,tmpInternal,callPred);
          nWdAut = result.first;
          stateMap = result.second;
        }
      }    
           
        //Call Transition  
        //??Does this only need to be done once??      
        for( Trans::symbolIterator it = nWdAut.trans->beginSymbols();
              it != nWdAut.trans->endSymbols(); it++ )
        {        
          //At a call position labeled a, the summary gets reinitialized: 
          //the new state contains pairs of the form (q,q'), where (q,a,q')
          // is an element of Delta_c.
          Key tmpCallKey;
          StatePairSet tmpCall;
          bool first = true;
          for( Trans::callIterator cit = nWdAut.trans->beginCall();
                cit != nWdAut.trans->endCall(); cit++ )
          {
            //If the symbols match, make a new state pair set.
            if( *it == cit->second )
            {
              tmpCall.insert(StatePair(cit->first,cit->third));
              if(first)
              {
                tmpCallKey = wali::getKey(cit->first,cit->third);
                first = false;
              }
              else
                tmpCallKey = wali::getKey( wali::getKey(cit->first,cit->third), tmpCallKey);
            }
          }
          //We only want to add states that don't already exist.          
          bool recurse = false;
          if( stateMap.count(tmpCall) == 0 )
          {
            //Add the new key to the map.
            stateMap.insert(std::pair<StatePairSet,Key>(tmpCall,tmpCallKey));
            nWdAut.addState(tmpCallKey);
            recurse = true;
          }
          else
            tmpCallKey = (stateMap.find(tmpCall))->second.getKey();
            
          //Add a transition.
          KeyTriple newCallTrans = 
                KeyTriple(stateMap.find(currState)->second.getKey(),it->getLabelKey(),tmpCallKey);
          nWdAut.trans->addCall(newCallTrans);
          
          if(recurse)
          {
            callPred.push(currState);
            std::pair<NWA<StKey,Label>,StateMap> result = det(nWdAut,stateMap,tmpCall,callPred);
            nWdAut = result.first;
            stateMap = result.second;
          }
        }
        
        //Return Transition
        for( Trans::symbolIterator it = nWdAut.trans->beginSymbols();
              it != nWdAut.trans->endSymbols(); it++ )
        { 
          //Consider a return position labeled a, and suppose S denotes the
          //current state and S' denotes the state just before the call-predecessor.
          //Then (q,q') belongs to the new state, provided there exist states 
          //q_1,q_2 such that (q,q_1) is an element of S' and (q_1,q_2) is an 
          //element of S and (q_2,q_1,a,q') is an element of Delta_r.
          Key tmpReturnKey;
          StatePairSet tmpReturn;
          for( Trans::returnIterator rit = nWdAut.trans->beginReturn();
                rit != nWdAut.trans->endReturn(); rit++ )
          {
            //If the symbols match, make a new state pair set.
            if( *it == rit->third )
            {
              //S' is callPred.top(), S is currState
              
              //Want to find (q_1,q_2) in S
              bool found = false;
              for( StatePairSet::iterator sit = currState.begin();
                    sit != currState.end(); sit++ )
              {
                if( (sit->first == rit->second) && (sit->second == rit->first) )
                  found = true;
              }
              
              if( found )
              {
                //Want to find (q,q_1) in S'
                bool first = true;
                for( StatePairSet::iterator sit = callPred.top().begin();
                      sit != callPred.top().end(); sit++ )
                {
                  if( sit->second == rit->second )
                  {
                    tmpReturn.insert(StatePair(sit->first,rit->fourth));
                    if(first)
                    {
                      tmpReturnKey = wali::getKey( sit->first.getKey(),rit->fourth );
                      first = false;
                    }
                    else
                      tmpReturnKey = wali::getKey( wali::getKey( sit->first.getKey(),rit->fourth ), tmpReturnKey );
                  }
                }
              }
            }
          }          
          //We only want to add states that don't already exist.
          bool recurse = false;
          if( stateMap.count(tmpReturn) == 0 )
          {
            //Add the new key to the map.
            stateMap.insert(std::pair<StatePairSet,Key>(tmpReturn,tmpReturnKey));
            nWdAut.addState(tmpReturnKey);
            recurse = true;
          }
          else
            tmpReturnKey = (stateMap.find(tmpReturn))->second.getKey();
          
          Key tmpCallKey = (stateMap.find(callPred.top()))->second.getKey();
            
          //Add a transition.
          KeyQuad newReturnTrans = 
                KeyQuad(stateMap.find(currState)->second.getKey(),tmpCallKey,it->getLabelKey(),tmpReturnKey);
          nWdAut.trans->addReturn(newReturnTrans);
          
          if(recurse)
          {
            callPred.pop();
            std::pair<NWA<StKey,Label>,StateMap> result = det(nWdAut,stateMap,tmpReturn,callPred);
            nWdAut = result.first;
            stateMap = result.second;
          }
        }
            
      return std::pair<NWA<StKey,Label>,StateMap>(nWdAut,stateMap);
    }
  }
}
#endif