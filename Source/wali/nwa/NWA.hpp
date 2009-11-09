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
    template<typename St,typename Sym >
    class NWA : public Printable
    {
      //TODO: update all comments
      public:
      typedef StateSet<St> States;
      typedef std::set<Sym> Symbols;
      typedef typename States::iterator stateIterator;
      typedef typename Symbols::iterator symbolIterator;
      typedef Triple<St,Sym,St> Call;
      typedef Triple<St,Sym,St> Internal;
      typedef Quad<St,St,Sym,St> Return;
      typedef TransSet< St,Sym,Call,Internal,Return > Trans;
      typedef typename Trans::Calls Calls;
      typedef typename Trans::Internals Internals;
      typedef typename Trans::Returns Returns;
      typedef typename Trans::callIterator callIterator;
      typedef typename Trans::internalIterator internalIterator;
      typedef typename Trans::returnIterator returnIterator;
      
      typedef std::pair< St,St > StatePair;
      typedef std::set< StatePair > StatePairSet;
      typedef std::map< StatePairSet,St > StateMap;
          
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
         * @brief tests whether the stuck state is a final state
         *
         * This method tests whether the stuck state is a final state in this
         * NWA.  It returns true if the stuck state is final and false otherwise.
         *
         * @return true if the stuck state is a final state, false otherwise
         *
         */
        bool isAbsentAccept();
        
        /**
         * 
         * @brief sets the stuck state to be a final state
         *
         * This method sets the stuck state to be a final state.
         *
         */
        void acceptAbsent();
        
        /**
         * 
         * @brief sets the stuck state to be not a final state
         *
         * This method makes the stuck state not a final state.
         *
         */
        void rejectAbsent();

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
        bool isState( St name ); 

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
        bool addState( St name );
        
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
        bool removeState( St name );

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
        bool isInitialState( St name ) const;

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
        bool addInitialState( St name );
        
        /**
         *
         * @brief adds all of the states in the given StateSet to
         * the initial state set associated with this NWA
         *
         * This method adds all of the states associated with the 
         * given StateSet to the initial state set associated with this
         * NWA.
         *
         * @parm the state set whose initial states to add to this NWA's
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
        bool removeInitialState( St name );

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
        bool isFinalState( St name ) const;

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
        bool addFinalState( St name );

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
        void addAllFinalStates( States addStateSet, bool accept ); 

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
        bool removeFinalState( St name );

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
        size_t sizeInitialStates( ) const;
        
        /**
         *
         * @brief returns the number of final states associated with this NWA
         *
         * This method returns the number of final states associated with this
         * NWA. Note: This count does includes the stuck state if it is
         * a final state. 
         *
         * @return the number of final states associated with this NWA
         *
         */
        size_t sizeFinalStates( ) const;

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
        const Symbols & getSymbols( );

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
        bool isSymbol( Sym sym );

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
        bool addSymbol( Sym sym );
        
        /**
         *
         * @brief add the given symbols to the NWA
         *
         * This method adds all of the given symbols to the set of symbols 
         * associated with the NWA.  
         *
         * @param the symbols to add
         *
         */
        void addAllSymbols( Symbols addSymbolSet);

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
        bool removeSymbol( Sym sym );

        /**
         *
         * @brief remove all symbols associated with the NWA
         *
         * This method removes all symbols associated with the NWA.
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
        bool addCallTrans( St from, Sym sym, St to );

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
        bool addCallTrans( Call * ct );

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
        bool removeCallTrans( St from, Sym sym, St to );

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
        bool removeCallTrans( Call * ct );

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
        bool addInternalTrans( St from, Sym sym, St to );

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
        bool addInternalTrans( Internal * it );

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
        bool removeInternalTrans( St from, Sym sym, St to );

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
        bool removeInternalTrans( Internal * it );

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
        internalIterator beginInternalTrans() const;
        
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
        internalIterator endInternalTrans() const;

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
        bool addReturnTrans( St from, St pred, Sym sym, St to );

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
        bool addReturnTrans( Return * rt );

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
        bool removeReturnTrans( St from, St pred, Sym sym, St to );

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
        bool removeReturnTrans( Return * rt );
        
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
        NWA * unionNWA( NWA other );  //FIX

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
        NWA * concat( NWA other );  //FIX

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
        NWA * star( );  //FIX

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
        NWA * reverse();  //FIX

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
        wpds::WPDS plusWPDS( wpds::WPDS base ); //FIX
    
        /**
         *
         * @brief constructs the NWA equivalent to the given PDS
         *
         * This method constructs the NWA that is equivalent to the given PDS.
         *
         * @return the NWA equivalent to the given PDS
         *
         */
        static nwa::NWA<St,Sym> PDStoNWA(wpds::WPDS pds); //FIX
    
        /**
         *
         * @brief constructs the PDS equivalent to this NWA
         *
         * This method constructs the PDS that is equivalent to this NWA.
         *
         * @return the PDS equivalent to this NWA
         *
         */
        static wpds::WPDS NWAtoPDS(NWA nwa,wali::sem_elem_t one); //FIX
    
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
        NWA & determinize( ); //FIX

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
        bool isDeterministic( );  //FIX

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
        bool isMember( nws::NWS word ); //FIX

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
        bool operator==( const NWA & other ) const;

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
        bool NWA::hasPath( St currState, std::stack< St > calls, std::set< St > visited );  //FIX
    
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
        bool matchWord( St currState, std::deque<WordRecConfig<St>> configs );  //FIX
        
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
                                    std::stack<StatePairSet> callPred );  //FIX

      //
      // Variables
      //
      protected:
        States states;
        States initialStates;
        States finalStates;
        
        Symbols symbols;
        
        Trans * trans;
        
        bool absentAcceptance;
    };
    
    //
    // Methods
    //

    //Constructors and Destructor
    template<typename St,typename Sym >
    NWA<St,Sym>::NWA( )
    {
      states = States();      
      initialStates = States();
      finalStates = States();
      
      symbols = Symbols();
      //addSymbol(Sym::getEpsilon()); 
      
      trans = new Trans();
      
      absentAcceptance = false; //Assume stuck state is not a final state.
    }
    
    template<typename St,typename Sym >
    NWA<St,Sym>::NWA( const NWA & other )
    {
      operator=(other);
    }
    
    template<typename St,typename Sym >
    NWA<St,Sym> & NWA<St,Sym>::operator=( const NWA & other )
    {
      //Wipe any pre-existing data.
      states.clear();
      initialStates.clear();
      finalStates.clear();
      symbols.clear();
      trans->clear();
      
      //Copy data over from 'other'
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;
      
      symbols = other.symbols;

      trans->addAllTrans(*other.trans);
      
      absentAcceptance = other.absentAcceptance;
      
      return *this;
    }
   
    template<typename St,typename Sym >
    NWA<St,Sym>::~NWA( )
    {         
      states.clear();
      initialStates.clear();
      finalStates.clear();
      
      symbols.clear();
      
      trans->~Trans();    
    }
    
  
    //State Accessors

    /**
     *  
     * @brief tests whether the stuck state is a final state
     *
     * @return true if the stuck state is a final state, false otherwise
     *
     */
    template<typename St,typename Sym >
    bool NWA<St,Sym>::isAbsentAccept()
    {
      return absentAcceptance;
    }
        
    /**
     * 
     * @brief sets the stuck state to be a final state
     *
     */
    template<typename St,typename Sym > 
    void NWA<St,Sym>::acceptAbsent()
    {
      absentAcceptance = true;
    }
    
    /**
     * 
     * @brief sets the stuck state to be not a final state
     *
     */
    template<typename St,typename Sym > 
    void NWA<St,Sym>::rejectAbsent()
    {
      absentAcceptance = false;
    }

    //All States
    /**
     *
     * @brief get all states in the state set of the NWA
     *
     * @return set of Key values for all states associated with the NWA
     *
     */
    template<typename St,typename Sym > 
    const typename NWA<St,Sym>::States & NWA<St,Sym>::getStates( )
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::isState( St name )
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::addState( St name )
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
    template<typename St,typename Sym > 
    void NWA<St,Sym>::addAllStates( States addStateSet)
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::removeState( St name )
    {
      //Remove transitions associated with the state that was removed.
      if( states.remove(name) )
      {
        //If this state existed in either the initial state set or the
        //final state set, then it should be removed from that set too.
        initialStates.remove(name);
        finalStates.remove(name);
      
        //TODO: fix this so that these functions are private and 
        //only one removeState call is made to trans.
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
    template<typename St,typename Sym > 
    void NWA<St,Sym>::clearStates( )
    {      
      initialStates.clear();
      finalStates.clear();
      states.clear();
      
      absentAcceptance = false; //default behavior is restored

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
    template<typename St,typename Sym > 
    typename NWA<St,Sym>::stateIterator NWA<St,Sym>::beginStates()
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
    template<typename St,typename Sym > 
    typename NWA<St,Sym>::stateIterator NWA<St,Sym>::endStates()
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
    template<typename St,typename Sym > 
    const typename NWA<St,Sym>::States & NWA<St,Sym>::getInitialState( )
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::isInitialState( St name ) const
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::addInitialState( St name )
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
    template<typename St,typename Sym > 
    void NWA<St,Sym>::addAllInitialStates( States addStateSet)
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::removeInitialState( St name ) 
    {
      return initialStates.remove(name);
    }

  
    /**
     *
     * @brief remove all states from the initial state set of the NWA
     *
     */
    template<typename St,typename Sym > 
    void NWA<St,Sym>::clearInitialStates( )
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
    template<typename St,typename Sym > 
    typename NWA<St,Sym>::stateIterator NWA<St,Sym>::beginInitialStates()
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
    template<typename St,typename Sym > 
    typename NWA<St,Sym>::stateIterator NWA<St,Sym>::endInitialStates()
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
    template<typename St,typename Sym > 
    const typename NWA<St,Sym>::States & NWA<St,Sym>::getFinalStates( )
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::isFinalState( St name ) const
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::addFinalState( St name )
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
    template<typename St,typename Sym > 
    void NWA<St,Sym>::addAllFinalStates( States addStateSet, bool accept )
    {
      if( accept )
        absentAcceptance = true;
      
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::removeFinalState( St name )
    {
      return finalStates.remove(name);
    }
    
   
    /**
     *
     * @brief remove all states from the final state set of the NWA
     *
     */
    template<typename St,typename Sym > 
    void NWA<St,Sym>::clearFinalStates( )
    {
      finalStates.clear();
      
      absentAcceptance = false; //default behavior is restored
    }
    
    /**
     *
     * @brief provide access to the beginning of the final state set
     *
     * @return an iterator pointing to the beginning of the final state
     *  set
     *
     */
    template<typename St,typename Sym > 
    typename NWA<St,Sym>::stateIterator NWA<St,Sym>::beginFinalStates()
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
    template<typename St,typename Sym >   
    typename NWA<St,Sym>::stateIterator NWA<St,Sym>::endFinalStates()
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
    template<typename St,typename Sym > 
    size_t NWA<St,Sym>::sizeStates( )
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
    template<typename St,typename Sym > 
    size_t NWA<St,Sym>::sizeInitialStates( ) const
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
    template<typename St,typename Sym > 
    size_t NWA<St,Sym>::sizeFinalStates( ) const
    {
      if( absentAcceptance )
        return finalStates.size() + 1;
      else
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
    template<typename St,typename Sym > 
    const typename NWA<St,Sym>::Symbols & NWA<St,Sym>::getSymbols( )
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::isSymbol( Sym sym )
    {
      return (symbols.count(sym) >  0);
    }
   
   
    /**
     *
     * @brief add the given symbol to the NWA
     *
     * @param symbol to add
     * @return false if the symbol is already associated with the NWA
     *
     */
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::addSymbol( Sym sym )
    {
      if( symbols.count(sym) > 0 )
        return false;
      symbols.insert(sym);
      return true;
    }
    
    /**
     *
     * @brief add the given symbols to the NWA
     *
     * @param the symbols to add
     *
     */
    template<typename St,typename Sym > 
    void NWA<St,Sym>::addAllSymbols( Symbols addSymbolSet)
    {
      for( symbolIterator sit = addSymbolSet.begin();
            sit != addSymbolSet.end(); sit++ )
        symbols.add(*sit);
    }
    
   
    /**
     *
     * @brief remove the given symbol from the NWA
     *
     * @param the symbol to remove
     * @return false if the symbol is no associated with the NWA
     *
     */
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::removeSymbol( Sym sym )
    {
      if( symbols.count(sym) == 0 )
        return false;
      else if( symbols.erase(sym) )
      {
        //TODO: fix this so that these functions are private and
        //only one call to removeSymbol must be made to trans
        trans->removeCallTransSym(sym);
        trans->removeInternalTransSym(sym);
        trans->removeReturnTransSym(sym);
        
        return true;
      }      
      return false;
    }
    
   
    /**
     *
     * @brief remove all symbols associated with the NWA
     *
     */
    template<typename St,typename Sym > 
    void NWA<St,Sym>::clearSymbols( )
    {
      symbols.clear();

      //Since all symbols are being removed, all transitions are removed 
      //as well.
      trans->clear();
      
      //The epsilon symbol should always remain in the symbol set.
      //This will also replace transitions with the epsilon symbol for each 
      //state that exists in the state set to the error state .
      addSymbol(Sym::getEpsilon());
    }
    
    /**
     *
     * @brief provide access to the beginning of the symbol set
     *
     * @return an iterator pointing to the beginning of the symbol set
     *
     */
    template<typename St,typename Sym >
    typename NWA<St,Sym>::symbolIterator NWA<St,Sym>::beginSymbols()
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
    template<typename St,typename Sym >
    typename NWA<St,Sym>::symbolIterator NWA<St,Sym>::endSymbols()
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
   template<typename St,typename Sym >
    size_t NWA<St,Sym>::sizeSymbols( )
    {
      return symbols.size();
    }
  
    //Transition Accessors

    /**
     *
     * @brief remove all transitions from the NWA
     *
     */
    template<typename St,typename Sym > 
    void NWA<St,Sym>::clearTrans( )
    {
      trans->clear();
    }
  
  
    /**
     *
     * @brief get all call transitions in the transition set of the NWA.
     *
     * @return set of call transitions associated with the NWA
     * 
     */
    template<typename St,typename Sym > 
    const typename NWA<St,Sym>::Calls & NWA<St,Sym>::getCallTrans( )
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::addCallTrans( St from, Sym sym, St to )
    {
      if(! isState(from)
          || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        addSymbol(sym);
          
      Call ct = Call(from,sym,to);
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::addCallTrans( Call * ct )
    {
      if(! isState(ct->first)
          || ! isState(ct->third) )
        return false;
      if(! isSymbol(ct->second) )
        addSymbol(ct->second);  
        
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::removeCallTrans( St from, Sym sym, St to )
    {
      if(! isState(from)
          || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        return false;
    
      Call ct = Call(from,sym,to);
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
    template<typename St,typename Sym> 
    bool NWA<St,Sym>::removeCallTrans( Call * ct )
    {
      if(! isState(ct->first)
          || ! isState(ct->third) )
        return false;
      if(! isSymbol(ct->second) )
        return false;
        
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
    template<typename St,typename Sym>
    typename NWA<St,Sym>::callIterator NWA<St,Sym>::beginCallTrans()
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
    template<typename St,typename Sym>
    typename NWA<St,Sym>::callIterator NWA<St,Sym>::endCallTrans()
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
    template<typename St,typename Sym > 
    const typename NWA<St,Sym>::Internals & NWA<St,Sym>::getInternalTrans( )
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::addInternalTrans( St from, Sym sym, St to )
    {
      if(! isState(from)
          || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        addSymbol(sym);  
    
      Internal it = Internal(from,sym,to);
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::addInternalTrans( Internal * it )
    {
      if(! isState(it->first)
          || ! isState(it->third) )
        return false;
      if(! isSymbol(it->second) )
        addSymbol(it->second);  
    
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::removeInternalTrans( St from, Sym sym, St to )
    {
      if(! isState(from)
          || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        return false;
    
      Internal it = Internal(from,sym,to);
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::removeInternalTrans( Internal * it )
    {
      if(! isState(it->first)
          || ! isState(it->third) )
        return false;
      if(! isSymbol(it->second) )
        return false;
        
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
    template<typename St,typename Sym>
    typename NWA<St,Sym>::internalIterator NWA<St,Sym>::beginInternalTrans() const
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
    template<typename St,typename Sym>
    typename NWA<St,Sym>::internalIterator NWA<St,Sym>::endInternalTrans() const
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
    template<typename St,typename Sym > 
    const typename NWA<St,Sym>::Returns & NWA<St,Sym>::getReturnTrans( )
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::addReturnTrans( St from, St pred, Sym sym, St to )
    {
      if(! isState(from)
          || ! isState(pred)
          || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        addSymbol(sym);  
    
      Return rt = Return(from,pred,sym,to);
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::addReturnTrans( Return * rt )
    {
      if(! isState(rt->first)
          || ! isState(rt->second)
          || ! isState(rt->fourth) )
        return false;
      if(! isSymbol(rt->third) )
        addSymbol(rt->third);  
        
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::removeReturnTrans( St from, St pred, Sym sym, St to )
    {
      if(! isState(from)
          || ! isState(pred)
          || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        return false;
    
      Return rt = Return(from,pred,sym,to);
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::removeReturnTrans( Return * rt )
    {
      if(! isState(rt->first)
          || ! isState(rt->second)
          || ! isState(rt->fourth) )
        return false;
      if(! isSymbol(rt->third) )
        return false;
        
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
    template<typename St,typename Sym>
    typename NWA<St,Sym>::returnIterator NWA<St,Sym>::beginReturnTrans()
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
    template<typename St,typename Sym>
    typename NWA<St,Sym>::returnIterator NWA<St,Sym>::endReturnTrans()
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
    template<typename St,typename Sym > 
    NWA<St,Sym> * NWA<St,Sym>::unionNWA( NWA<St,Sym> other )
    { //FIX
      //TODO: fix this for absentAcceptance
      NWA<St,Sym> * newNWA = new NWA<St,Sym>();
      
      //Generate new states, adding them to the state set as well as
      //to the initial state set and final state set as needed.
      St newSt;
      for( stateIterator fit = beginStates();
            fit != endStates(); fit++ )
      {
        for( stateIterator sit = other.beginStates();
              sit != other.endStates(); sit++ )
        {
          if( fit->intersect(*sit,&newSt) )
          {
            newNWA->addState(newSt);
          
            if( this->isInitialState(*fit) && other.isInitialState(*sit) )
              newNWA->addInitialState(newSt);
            if( this->isFinalState(*fit) || other.isFinalState(*sit) )
              newNWA->addFinalState(newSt); 
          }
          else
          {
            //these nodes can't be intersected
            //TODO: what should i do here
          }     
        }
      }
      
      //Generate call transitions.
      for( callIterator fcit = this->trans->beginCall();
            fcit != this->trans->endCall(); fcit++ )
      {
        for( callIterator scit = other.trans->beginCall();
            scit != other.trans->endCall(); scit++ )
        {
          Sym sym;
          if( fcit->second->intersect(scit->second,sym) )
          {
            St fromSt, toSt;
            //TODO: what should I do if these calls failed?
            fcit->first->intersect(scit->first,&fromSt);
            fcit->third->intersect(scit->third,&toSt);
            
            newNWA->addCallTrans( fromSt, sym, toSt );
          }
        }
      }
      //Generate internal transitions.
      for( Trans::internalIterator fiit = this->trans->beginInternal();
            fiit != this->trans->endInternal(); fiit++ )
      {
        for( Trans::internalIterator siit = other.trans->beginInternal();
              siit != other.trans->endInternal(); siit++ )
        {
          Sym sym;
          if( fiit->second->intersect(siit->second,sym) )
          {
            St fromSt, toSt;
            //TODO: what should I do if these calls failed?
            fiit->first->intersect(siit->first,&fromSt);
            fiit->third->intersect(siit->third,&toSt);
            
            newNWA->addInternalTrans( fromSt, sym, toSt );
          }
        }
      }
      //Generate return transitions.  
      for( Trans::returnIterator frit = this->trans->beginReturn();
            frit != this->trans->endReturn(); frit++ )
      {
        for( Trans::returnIterator srit = other.trans->beginReturn();
              srit != other.trans->endReturn(); srit++ )
        {
          Sym sym;
          if( frit->third->intersect(srit->third,sym) )
          {
            St fromSt, predSt, toSt;
            //TODO: what should I do if these calls failed?
            frit->first->intersect(srit->first,&fromSt);
            frit->second->intersect(srit->second,&predSt);
            frit->fourth->intersect(srit->fourth,&toSt);
            
            newNWA->addReturnTrans( fromSt, predSt, sym, toSt );
          }
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
    template<typename St,typename Sym > 
    NWA<St,Sym> * NWA<St,Sym>::concat( NWA<St,Sym> other )
    { //FIX
      //TODO: fix this
      NWA<St,Sym> * newNWA = new NWA<St,Sym>();
      
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
          Internal newTrans = Internal(*fit,Sym::getEpsilon(),*iit);
          newNWA->addInternalTrans( &newTrans );
        }
      }
      
      //Final states of the new NWA are the final states of the NWA 'other'
      newNWA->addAllFinalStates(other.finalStates, other.absentAcceptance);

      //Any symbols not in the original set of symbols must be added.
      newNWA->addAllSymbols(symbols);
      newNWA->addAllSymbols(other.symbols);

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
    template<typename St,typename Sym >
    NWA<St,Sym> * NWA<St,Sym>::star( )
    { //FIX
      //TODO: fix this
      NWA<St,Sym> * newNWA = new NWA<St,Sym>();
      
      //Add states.
      newNWA->addAllStates(states);
      newNWA->addAllInitialStates(initialStates);
      newNWA->addAllFinalStates(finalStates,absentAcceptance);
      
      newNWA->addAllSymbols(symbols);
      
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
          Internal newTrans = Internal(fit,Sym::getEpsilon(),iit);
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
    template<typename St,typename Sym > 
    NWA<St,Sym> * NWA<St,Sym>::complement( )
    {
      NWA<St,Sym> * newNWA = new NWA<St,Sym>();
    
      //Start with a deterministic NWA.
      if(! this->isDeterministic() )
        determinize();
      
      //The new NWA will have all the same states and initial states.
      newNWA->addAllStates(states);
      newNWA->addAllInitialStates(initialStates);
      
      newNWA->addAllSymbols(symbols);
      
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
      
      //If this NWA doesn't accept the stuck state, the complement should.
      if( !absentAcceptance )
        newNWA->absentAcceptance;
          
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
    template<typename St,typename Sym > 
    NWA<St,Sym> * NWA<St,Sym>::reverse()
    { //FIX
      //TODO: fix this
      NWA<St,Sym> * newNWA = new NWA<St,Sym>();
          
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
      for( internalIterator iit = this->trans->beginInternal();
            iit != this->trans->endInternal(); iit++ )
      {
        newNWA->addInternalTrans(iit->third,iit->second,iit->first);
      }         
      
      for( returnIterator rit = this->trans->beginReturn();
            rit != this->trans->endReturn(); rit++ )
      {
        newNWA->addCallTrans(rit->third,rit->second,rit->first);
      }
      for( callIterator cit = this->trans->beginCall();
            cit != this->trans->endCall(); cit++ )
      {
        for( returnIterator rit = this->trans->beginReturn();
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
    template<typename St,typename Sym> 
    NWA<St,Sym> * NWA<St,Sym>::intersect( NWA<St,Sym> other )
    {
      //TODO: fix this for absentAcceptance  
      /*  NOTE: This is not optimized and doesn't include epsilon closure or consideration of absentAcceptance.
      NWA<St,Sym> * newNWA = new NWA<St,Sym>();
      
      //Generate new states, adding them to the state set as well as
      //to the initial state set and final state set as needed.
      St newSt;
      for( stateIterator fit = beginStates();
            fit != endStates(); fit++ )
      {
        for( stateIterator sit = other.beginStates();
              sit != other.endStates(); sit++ )
        {        
          fit->intersect(*sit,&newSt);
          
            newNWA->addState(newSt);
          
            if( this->isInitialState(*fit) && other.isInitialState(*sit) )
              newNWA->addInitialState(newSt);
            if( this->isFinalState(*fit) && other.isFinalState(*sit) )
              newNWA->addFinalState(newSt); 
          
            //if these nodes can't be intersected
            //TODO: what should i do 
                   
        }
      }     
      
      //Generate call transitions.
      for( callIterator fcit = this->trans->beginCall();
            fcit != this->trans->endCall(); fcit++ )
      {
        for( callIterator scit = other.trans->beginCall();
            scit != other.trans->endCall(); scit++ )
        {
          Sym sym;
          if( fcit->second->intersect(scit->second,sym) )
          {
            St fromSt, toSt;
            //TODO: what should I do if these calls failed?
            fcit->first->intersect(scit->first,&fromSt);
            fcit->third->intersect(scit->third,&toSt);
            
            newNWA->addCallTrans( fromSt, sym, toSt );
        }
      }
      //Generate internal transitions.
      for( Trans::internalIterator fiit = this->trans->beginInternal();
            fiit != this->trans->endInternal(); fiit++ )
      {
        for( Trans::internalIterator siit = other.trans->beginInternal();
              siit != other.trans->endInternal(); siit++ )
        {
          Sym sym;
          if( fiit->second->intersect(siit->second,sym) )
          {
            St fromSt, toSt;
            //TODO: what should I do if these calls failed?
            fiit->first->intersect(siit->first,&fromSt);
            fiit->third->intersect(siit->third,&toSt);
            
            newNWA->addInternalTrans( fromSt, sym, toSt );
        }
      }
      //Generate return transitions.  
      for( Trans::returnIterator frit = this->trans->beginReturn();
            frit != this->trans->endReturn(); frit++ )
      {
        for( Trans::returnIterator srit = other.trans->beginReturn();
              srit != other.trans->endReturn(); srit++ )
        {
          Sym sym;
          if( frit->third->intersect(srit->third,sym) )
          {
            St fromSt, predSt, toSt;
            //TODO: what should I do if these calls failed?
            frit->first->intersect(srit->first,&fromSt);
            frit->second->intersect(srit->second,&predSt);
            frit->fourth->intersect(srit->fourth,&toSt);
            
            newNWA->addReturnTrans( fromSt, predSt, sym, toSt );
          }
        }
      }   
      
      return newNWA;
      */
      
      // TODO: acceptAbsent consideration
      //NOTE: for now, intersect only works with NWAs that do not accept the stuck state.
      assert(!absentAcceptance && !other.absentAcceptance);
     
      NWA<St,Sym> * worklistNWA = new NWA<St,Sym>();
      
      //If both NWAs to be intersected accept absent, the intersection should too.
      //Otherwise, the intersection should not, so keep the default settings.
      if( absentAcceptance && other.absentAcceptance )
        worklistNWA.acceptAbsent();
      
      std::deque<Triple<St,St,St>> wList; //An element: <joinedState, thisState, otherState>
      std::set<St> visited; //States that have been created in the intersection NWA.
      
      //Start the worklist with all possible initial states of the intersection NWA.
      //NOTE: Currently this should be just one state (the join of the single initial state of each machine).
      for( stateIterator fit = beginInitialStates();
            fit != endInitialStates(); fit++ )
      {
        for( stateIterator sit = other.beginInitialStates();
              sit != other.endInitialStates(); sit++ )
        {
          if( fit->intersect(*sit,&newSt) )
          {
            worklistNWA->addInitialState(newSt); 
            wList.push_back(Triple<St,St,St>(newSt,*fit,*sit));
            visited.insert(newSt);
          }  
        }      
      }
      
      //As long as there are still transitions to consider.
      while(! wList.empty() )
      {
        Triple<St,St,St> from = wList.front();
        
        //Epsilon closure for the nodes that we are currently considering.
        //Check call transitions in this NWA.
        Calls thisCalls = trans->getCalls(from.second,Sym::getEpsilon());
        for( Calls::const_iterator cit = thisCalls.begin(); cit != thisCalls.end(); cit++ )
        {
          St newSt;
          //Check intersectability of states (if false, don't add).           
          if( cit->third->intersect(from.third,&newSt) )
            if( visited.find(newSt) == visited.end() )
            {
              wList.push_back(Triple<St,St,St>(newSt,cit->third,from.third));
              visited.insert(newSt);
                    
              if( isFinalState(cit->third) && other.isFinalState(from.third) )
                worklistNWA->addFinalState(newSt);
              else
                worklistNWA->addState(newSt);
            }                   
        }
        //Check call transitions in the NWA 'other'.
        Calls otherCalls = other.trans->getCalls(from.third,Sym::getEpsilon());
        for( Calls::const_iterator cit = otherCalls.begin(); cit != otherCalls.end(); cit++ )
        {
          St newSt;
          //Check intersectability of states (if false, don't add).
          if( from.second->intersect(cit->third,&newSt) )
            if( visited.find(newSt) == visited.end() )
            {
              wList.push_back(Triple<St,St,St>(newSt,from.second,cit->third));
              visited.insert(newSt);
                 
              if( isFinalState(from.second) && other.isFinalState(cit->third) )
                worklistNWA->addFinalState(newSt);
              else
                worklistNWA->addState(newSt);
            }
        }
              
        //Check internal transitions in this NWA.
        Internals thisInternals = trans->getInternals(from.second,Sym::getEpsilon());
        for( Internals::const_iterator iit = thisInternals.begin(); iit != thisInternals.end(); iit++ )
        {
          St newSt;
          //Check intersectability of states (if false, don't add).
          if( iit->third->intersect(from.third,&newSt) )
            if( visited.find(newSt) == visited.end() )
            {
              wList.push_back(Triple<St,St,St>(newSt,iit->third,from.third));
              visited.insert(newSt);
                  
              if( isFinalState(iit->third) && other.isFinalState(from.second.second) )
                worklistNWA->addFinalState(newSt);
              else
                worklistNWA->addState(newSt);
            }
        }     
        
        //Check internal transitions in the NWA 'other'.
        Internals otherInternals = other.trans->getInternals(from.third,Sym::getEpsilon());
        for( Internals::const_iterator iit = otherInternals.begin(); iit != otherInternals.end(); iit++ )
        {
          St newSt;
          //Check intersectability of states (if false, don't add).
          if( from.second->intersect(iit->third,&newSt) )
            if( visited.find(newSt) == visited.end() )
            {
              wList.push_back(Triple<St,St,St>(newSt,from.second,iit->third));
              visited.insert(newSt);
                   
              if( isFinalState(from.second) && other.isFinalState(iit->third) )
                worklistNWA->addFinalState(newSt);
              else
                worklistNWA->addState(newSt);
             }
        }      
              
        //Check return transitions in this NWA.
        Returns thisReturns = trans->getReturns(from.second,Sym::getEpsilon());
        for( Returns::const_iterator rit = thisReturns.begin(); rit != thisReturns.end(); rit++ )
        {
          St newSt;
          //Check intersectability of states (if false, don't add).
          if( rit->fourth->intersect(from.third,&newSt) )
            if( visited.find(newSt) == visited.end() )
            {
              wList.push_back(Triple<St,St,St>(newSt,rit->fourth,from.third));
              visited.insert(newSt);
                    
              if( isFinalState(rit->fourth) && other.isFinalState(from.third) )
                worklistNWA->addFinalState(newSt);
              else
                worklistNWA->addState(newSt);
            } 
        }
        
        //Check return transitions in the NWA 'other'.
        Returns otherReturns = other.trans->getReturns(from.third,Sym::getEpsilon());
        for( Returns::const_iterator rit = otherReturns.begin(); rit != otherReturns.end(); rit++ )
        {
          St newSt;
          //Check intersectability of states (if false, don't add).
          if( from.second->intersect(rit->fourth,&newSt) )
            if( visited.find(newSt) == visited.end() )
            {
              wList.push_back(Triple<St,St,St>(newSt,from.second,rit->fourth));
              visited.insert(newSt);
                    
              if( isFinalState(from.second) && other.isFinalState(rit->fourth) )
                worklistNWA->addFinalState(newSt);
              else
                worklistNWA->addState(newSt);
            }
        }
        
        //TODO: ???? this is going to hell in a handbasket (i.e. quadratic performance is emminent)
        //For each possible outgoing symbol/edge type.
        for( symbolIterator fsit = symbols.begin(); fsit != symbols.end(); fsit++ )
        {
          if( ! *fsit == Sym::getEpsilon() )  //Symbols other than epsilon.
          for( symbolIterator ssit = other.symbols.begin(); ssit != other.symbols.end(); ssit++ )
          {           
            if (! *ssit == Sym::getEpsilon() )  //Symbols other than epsilon.
            {   
              Sym sym;
              //Check intersectability of edge labels.
              if( fsit->intersect(*ssit,sym) )  //Only continue if the symbols can be intersected.
              {
                //Check call transitions with the desired 'from' state.
                Calls thisCalls = trans->getCalls(from.second,*fsit);
                Calls otherCalls = other.trans->getCalls(from.third,*ssit);  
             
                if( !thisCalls.empty() && !otherCalls.empty() )
                {
                  //there are edges to join.
                  for( Calls::const_iterator fcit = thisCalls.begin();
                        fcit != thisCalls.end(); fcit++ )
                  {
                    for( Calls::const_iterator scit = otherCalls.begin();
                          scit != otherCalls.end(); scit++ )
                    {
                      St toSt;                              
                      //Check intersectability of to states (if false, don't look further).           
                      if( fcit->third->intersect(scit->third,&toSt) )
                        if( visited.find(toSt) == visited.end() )
                        {
                          wList.push_back(Triple<St,St,St>(toSt,fcit->third,scit->third));
                          visited.insert(toSt);
                        }
                      
                      if( isFinalState(fcit->third) && other.isFinalState(scit->third) )
                        worklistNWA->addFinalState(toSt);
                      else
                        worklistNWA->addState(toSt);
                        
                      //Add the transition to the intersection NWA.  
                      worklistNWA->addCallTrans( from.first, sym, toSt );
                    }              
                  }            
                }
                /* NOTE: assuming absentAccept is false for both machines
                else if( thisCalls.empty() && !otherCalls.empty() )
                {
                  //only this has no edges  
                  //if this is acceptAbsent, then follow the path of otherCalls
                  //with just the other state value for the joined state?
                  if( isAbsentAccept() )
                  {
                    for( Calls::const_iterator cit = otherCalls.begin();
                          cit != otherCalls.end(); cit++ )
                    {
                      if( isFinalState(cit->third) )
                        worklistNWA->addFinalState(cit->third);
                      else
                        worklistNWA->addState(cit->third);
                      worklistNWA->addCallTrans( from.first, sym, cit->third );
                    }
                  }
                }
                else if( !thisCalls.empty() && otherCalls.empty() )
                {
                  //only other has no edges
                  //if other is acceptAbsent, then follow the path of thisCalls
                  //with just the this state value for the joined state?
                  if( other.isAbsentAccept() )
                  {
                  }
                }
                //otherwise, both have no matching edges so there is no path to follow
                */
              
                //Check internal transitions with the desired 'from' state and symbol.
                Internals thisInternals = trans->getInternals(from.second,*fsit);
                Internals otherInternals = other.trans->getInternals(from.third,*ssit);  
                
                if( !thisInternals.empty() && !otherInternals.empty() )
                {
                  //there are edges to join.
                  for( Internals::const_iterator fiit = thisInternals.begin();
                        fiit != thisInternals.end(); fiit++ )
                  {
                    for( Internals::const_iterator siit = otherInternals.begin();
                          siit != otherInternals.end(); siit++ )
                    {
                      St toSt;                              
                      //Check intersectability of to states (if false, don't look further).           
                      if( fiit->third->intersect(siit->third,&toSt) )
                        if( visited.find(toSt) == visited.end() )
                        {
                          wList.push_back(Triple<St,St,St>(toSt,fiit->third,siit->third));
                          visited.insert(toSt);
                        }
                      
                      if( isFinalState(fiit->third) && other.isFinalState(siit->third) )
                        worklistNWA->addFinalState(toSt);
                      else
                        worklistNWA->addState(toSt);
                        
                      //Add the transition to the intersection NWA.
                      worklistNWA->addInternalTrans( from.first, sym, toSt );
                    }         
                  }            
                }
              
                /* NOTE: assuming absentAccept is false for both machines
                else if( thisInternals.empty() && !otherInternals.empty() )
                {
                  //only this has no edges  
                  //if this is acceptAbsent, then follow the path of otherInternals
                  //with a dummy value for the this state?
                  //with just the other state value for the joined state?
                }
                else if( !thisInternals.empty() && otherInternals.empty() )
                {
                  //only other has no edges
                  //if other is acceptAbsent, then follow the path of thisInternals
                  //with a dummy value for the other state?
                  //with just the this state value for the joined state?
                }
                //otherwise, both have no matching edges so there is no path to follow
                */
              
                //Check return transitions with the desired 'from' state and symbol.
                Returns thisReturns = trans->getReturns(from.second,*fsit);
                Returns otherReturns = other.trans->getReturns(from.third,*ssit);  
                
                if( !thisReturns.empty() && !otherReturns.empty() )
                {
                  //there are edges to join.
                  for( Returns::const_iterator frit = thisReturns.begin();
                        frit != thisReturns.end(); frit++ )
                  {
                    for( Returns::const_iterator srit = otherReturns.begin();
                          srit != otherReturns.end(); srit++ )
                    {
                      St predSt, toSt;
                      frit->second->intersect(srit->second,&predSt);  //TODO: check that this is a viable call point in the intersected NWA.
                      
                      //Check intersectability of to states (if false, don't look further).    
                      if( frit->fourth->intersect(srit->fourth,&toSt) )
                        if( visited.find(toSt) == visited.end() )
                        {
                          wList.push_back(Triple<St,St,St>(toSt,frit->fourth,srit->fourth));
                          visited.insert(toSt);
                        }
                      
                      if( isFinalState(frit->third) && other.isFinalState(srit->third) )
                        worklistNWA->addFinalState(toSt);
                      else
                        worklistNWA->addState(toSt);
                        
                      //Add the transition to the intersection NWA.  
                      worklistNWA->addReturnTrans( from.first, predSt, sym, toSt );
                    }            
                  }            
                }
              
                /* NOTE: assuming absentAccept is false for both machines
                else if( thisReturns.empty() && !otherReturns.empty() )
                {
                  //only this has no edges  
                  //if this is acceptAbsent, then follow the path of
                  //otherCalls with a dummy value for the this state?
                }
                else if( !thisReturns.empty() && otherReturns.empty() )
                {
                  //only other has no edges
                  //if other is acceptAbsent, then follow the path
                  //of thisCalls with a dummy value for the other state?
                }
                //otherwise, both have no matching edges so there is no path to follow
                */
              }
            }
          }
        }
        
        wList.pop_front();
      }
     
      return worklistNWA;
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
    template<typename St,typename Sym > 
    wpds::WPDS NWA<St,Sym>::plusWPDS( wpds::WPDS base )
    { //FIX
      //TODO: how does this react to absentAcceptance & State/Symbol types
      wpds::WPDS result = wpds::WPDS();
      /*wpds::WpdsRules rules = wpds::WpdsRules();
      
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
      }     */ 
      
      return result;
    }
    
    /**
     *
     * @brief constructs the NWA equivalent to the given PDS
     *
     * @return the NWA equivalent to the given PDS
     *
     */
    template<typename St, typename Sym > 
    nwa::NWA<St,Sym> NWA<St,Sym>::PDStoNWA(wpds::WPDS pds)
    { //FIX
      //TODO: how does this react to absentAcceptance & State/Symbol types
      nwa::NWA<St,Sym> result = nwa::NWA<St,Sym>();
      
      std::map<St,St> call_return;
      /*
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
      */
      return result;    
    }
    
    /**
     *
     * @brief constructs the PDS equivalent to this NWA
     *
     * @return the PDS equivalent to this NWA
     *
     */
    template<typename St,typename Sym> 
    wpds::WPDS NWA<St,Sym>::NWAtoPDS(NWA<St,Sym> nwa,wali::sem_elem_t one)
    //wpds::WPDS NWA<St,Sym>::NWAtoPDS(NWA<St,Sym> nwa,WeightMaker wm)
    { //FIX
      //TODO: how does this react to absentAcceptance???
      //NOTE: assume stuck state is rejecting
      wpds::WPDS result = wpds::WPDS();
      std::map< Key,Key > calls;
      
      Key program = wali::getKey("program"); 
      
      //Internal Transitions
      for( internalIterator iit = nwa.trans->beginInternal();
            iit != nwa.trans->endInternal(); iit++ )
      {      
        wali::sem_elem_t wgt = one;
        //wali::sem_elem_t wgt = wm.getweight(iit->first,iit->second.getLabel(),INTRA,iit->third);
        
        result.add_rule(program,                    //from_state
                        iit->first->getKey(),       //from_stack
                        program,                    //to_state
                        iit->third->getKey(),       //to_stack1
                        wgt);                       //weight      
      }
      
      //Call Transitions
      for( callIterator cit = nwa.trans->beginCall();
            cit != nwa.trans->endCall(); cit++ )
      {
        for( returnIterator rit = nwa.trans->beginReturn();
              rit != nwa.trans->endReturn(); rit++ )
          if( cit->first == rit->second )
          {
            //for each return site with cit->first as call site ...
            Key ret = getKey(rit->fourth->getKey(),rit->fourth->getKey()); // (r,r)
            calls.insert(std::pair<Key,Key>(cit->first->getKey(),ret));
            
            wali::sem_elem_t wgt = one;
            //wali::sem_elem_t wgt = wm.getweight(cit->first,cit->second.getLabel(),CALL_TO_ENTRY,cit->third);
            
            result.add_rule(program,          //from_state
                            cit->first->getKey(),       //from_stack
                            program,          //to_state
                            cit->third->getKey(),       //to_stack1
                            ret,              //to_stack2
                            wgt);       //weight  
          }  
      }
      
      //Return Transitions
      for( returnIterator rit = nwa.trans->beginReturn();
            rit != nwa.trans->endReturn(); rit++ )
      {
        Key rstate = getKey(program,rit->first->getKey()); // (p,x_i)
        result.add_rule(program,          //from_state
                        rit->first->getKey(),       //from_stack
                        rstate,           //to_state
                        one);       //weight 
        
        std::map<Key,Key>::iterator ret = calls.find(rit->second->getKey());
        if( ret != calls.end() )          
          result.add_rule(rstate,         //from_state
                          ret->second->getKey(),    //from_stack
                          program,        //to_state
                          rit->fourth->getKey(),    //to_stack
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
    template<typename St,typename Sym > 
    NWA<St,Sym> & NWA<St,Sym>::determinize( )
    { //FIX
      //TODO: how does this react to absentAcceptance & State/Symbol types
      std::pair<NWA<St,Sym>,StateMap> tmp = std::pair<NWA<St,Sym>,StateMap>();
            
      //New initial state is {(q,q) | q is an element of Q (possibly Q_in?)}
      /*Key newInitialState;
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
      }*/
   
      return tmp.first;
    }
    
    /**
     *
     * @brief tests whether the NWA is deterministic 
     *
     * @return true if the NWA is deterministic, false otherwise
     *
     */
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::isDeterministic( )
    { //FIX
      //TODO: optimize!!!
      //An NWA is not deterministic if there is not exactly one initial state
      if( sizeInitialStates() != 1)
        return false;

      //An NWA is not deterministic if there are epsilon transitions to states
      //other than the error state or if there are multiple transitions of some
      //type that are identical except for the to state.
      for( symbolIterator it = symbols.begin();
              it != symbols.end(); it++ )
      {
        for( stateIterator sit = beginStates();
              sit != endStates(); sit++ )
        {
          //Check call transitions.
          int count = 0;
          for( callIterator cit = trans->beginCall();
                cit != trans->endCall(); cit++ )
          {
            //Epsilon transition to a state other than the error state.
            if( *it == Sym::getEpsilon() )
              if( (cit->second == it) && (cit->third->getKey() != St::getBadKey()) )
                return false;
            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (cit->first == sit) && (cit->second == it) )
              count++;
          }
          if( count > 1 )
            return false;
          
          //Check internal transitions.
          count = 0;
          for( internalIterator iit = trans->beginInternal();
                iit != trans->endInternal(); iit++ )
          {
            //Epsilon transition to a state other than the error state.
            if( *it == Sym::getEpsilon() )
              if( (iit->second == it) && (iit->third->getKey() != St::getBadKey()) )
                return false;
            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (iit->first == sit) && (iit->second == it) )
              count++;
          }
          if( count > 1 )
            return false;
          
          for( stateIterator pit = beginStates();
                pit != endStates(); pit++ )
          {
            //Check return transitions.
            count = 0;
            for( returnIterator rit = trans->beginReturn();
                  rit != trans->endReturn(); rit++ )
            {
              //Epsilon transition to a state other than the error state.
              if( *it == Sym::getEpsilon() )
                if( (rit->third == it) && (rit->fourth->getKey() != St::getBadKey()) )
                  return false;
              //Keep a count of multiple transitions with the same from
              //state and symbol(that is not epsilon).
              else if( (rit->first == sit) &&
                  (rit->second == pit) &&
                  (rit->third == it) )
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
    template<typename St,typename Sym >
    bool NWA<St,Sym>::isEmpty( )
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
        std::set< St > visited = std::set< St >();
        visited.insert(*it);
        if( hasPath(it, std::stack< St >(), visited) )
          return false;
        visited.erase(*it);
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
    template<typename St,typename Sym >
    bool NWA<St,Sym>::isMember( nws::NWS word )
    { //FIX
      //TODO: how does this react to absentAcceptance
      std::deque<WordRecConfig<St>> configs;
      
      //Add a dummy call state for each call on the word stack
      for( wali::nws::NWS::iterator it = word.beginStack();
            it != word.endStack(); it++ );
      {
        WordRecConfig<St> config = WordRecConfig<St>(word);
        for( stateIterator sit = initialStates.begin();
              sit != initialStates.end(); sit++ )
          config.addCallState(*sit);
        
        configs.push_back(config);
      }
        
      //Simulate the nested word 'word' on the NWA.
      for( stateIterator sit = initialStates.begin();
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
    template<typename St,typename Sym >
    std::ostream & NWA<St,Sym>::print( std::ostream & o) const
    {
      o << "Q: ";
      states.print(o);
      
      o << "Q0: ";
      initialStates.print(o);
      
      o << "Qf: ";
      finalStates.print(o);
      if( absentAcceptance )
        o << "Stuck state accepted." << "\n";   //TODO: move this to stateset
    
      o << "Sigma: " << "{ ";
      bool first = true;
      for( Symbols::const_iterator sit = symbols.begin(); sit != symbols.end(); sit++ )
      { 
        if(! first )
          o << ", ";
        else
          first = false;
        sit->print(o);
      }
      o << " }\n";
      
    
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::operator==( const NWA<St,Sym> & other ) const
    {
      return ( (states == other.states) &&
                (initialStates == other.initialStates) &&
                (finalStates == other.finalStates) &&
                (absentAcceptance == other.absentAcceptance) &&
                (symbols == other.symbols) &&
                (trans == other.trans) );
    }
  
    /**
     *
     * @brief returns the number of states associated with this NWA
     *
     * @return the number of states associated with this NWA
     *
     */
    template<typename St,typename Sym > 
    size_t NWA<St,Sym>::numStates( )
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
    template<typename St,typename Sym > 
    size_t NWA<St,Sym>::numTrans( )
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::hasPath( St currState, std::stack< St > calls, std::set< St > visited )
    { //FIX
      //TODO: absentAcceptance
      //Try each possible transition out of this state to see if a final state
      //can be reached.
      
      //First try call transitions.
      calls.push(currState);
      for( callIterator cit = trans->beginCall();
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
      for( internalIterator iit = trans->beginInternal();
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
      St callState = calls.top();
      calls.pop();
      for( returnIterator rit = trans->beginReturn();
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
    template<typename St,typename Sym > 
    bool NWA<St,Sym>::matchWord( St currState, std::deque<WordRecConfig<St>> configs )
    { //FIX
      //TODO: absentAcceptance
      WordRecConfig<St> config = configs.front();
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
        
        for( callIterator cit = trans->beginCall();
              cit != trans->endCall(); cit++ )
        {
          //If this symbol in the word can be matched, try matching
          //the rest of the word.
          if( (cit->first == currState) &&
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
        St callState = config.getCallState();
        config.removeCallState();
        for( returnIterator rit = trans->beginReturn();
              rit != trans->endReturn(); rit++ )
        {
          //If this symbol in the word can be matched, try matching
          //the rest of the word.
          if( (rit->first == currState) &&
              (rit->second == callState) &&
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
        for( internalIterator iit = trans->beginInternal();
              iit != trans->endInternal(); iit++ )
        {
          //If this symbol in the word can be matched, try matching
          //the rest of the word.
          if( (iit->first == currState) &&
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
    template<typename St,typename Sym > 
    std::pair<NWA<St,Sym>,typename NWA<St,Sym>::StateMap> NWA<St,Sym>::det( NWA<St,Sym> nWdAut,
                                          StateMap stateMap, StatePairSet currState,
                                          std::stack<StatePairSet> callPred )
    { //FIX
      //TODO: absentAcceptance & State/Symbol type
      //Internal Transition
      /*for( symbolIterator it = nWdAut.symbols.begin();
            it != nWdAut.symbols.end(); it++ )
      {        
        StatePairSet internalTrans;
        //Find all internal transitions that use this symbol.
        for( internalIterator iit = nWdAut.trans->beginInternal();
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
        //TODO: check isState/isSymbol
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
        for( symbolIterator it = nWdAut.symbols.begin();
              it != nWdAut.symbols.end(); it++ )
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
          //TODO: check isState/isSymbol
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
        for( symbolIterator it = nWdAut.symbols.begin();
              it != nWdAut.symbols.end(); it++ )
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
          //TODO: check isState/isSymbol
          nWdAut.trans->addReturn(newReturnTrans);
          
          if(recurse)
          {
            callPred.pop();
            std::pair<NWA<StKey,Label>,StateMap> result = det(nWdAut,stateMap,tmpReturn,callPred);
            nWdAut = result.first;
            stateMap = result.second;
          }
        }
         */   
      return std::pair<NWA<St,Sym>,StateMap>(nWdAut,stateMap);
    }

  }
}
#endif