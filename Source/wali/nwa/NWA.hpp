#ifndef wali_nwa_NWA_GUARD
#define wali_nwa_NWA_GUARD 1

/**
 * @author Amanda Burton
 */

#define KEY

// ::wali
#include "wali/Printable.hpp"
#include "wali/Countable.hpp"
#include "wali/KeyContainer.hpp"

#include "wali/nwa/Symbol.hpp"
#include "wali/nwa/SymbolSet.hpp"
#include "wali/nwa/StateSet.hpp"
#include "wali/nwa/TransSet.hpp"

#include "wali/nws/NWS.hpp"
#include "wali/nwa/WordRecConfig.hpp"

#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"

#include "wali/nwa/WeightGen.hpp"

// std::c++
#include <iostream>
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
#ifndef KEY
    template<typename St = State<Key,ClientInfo>,typename StName = Key,typename Sym = Symbol<>>
    class NWA : public Printable
    {
      public:
        typedef typename St::ClientInfoRefPtr ClientInfoRefPtr;

        typedef Triple<StName,Sym,StName> Call;       
        typedef Triple<StName,Sym,StName> Internal;   
        typedef Quad<StName,StName,Sym,StName> Return;    
        typedef TransSet<St,Sym,Call,Internal,Return> Trans;
      
        typedef typename Trans::Calls Calls;
        typedef typename Trans::Internals Internals;
        typedef typename Trans::Returns Returns;
      
        typedef typename Trans::callIterator callIterator;
        typedef typename Trans::internalIterator internalIterator;
        typedef typename Trans::returnIterator returnIterator;
      
        typedef StateSet<St,StName> States;
        typedef std::set<Sym> Symbols;
      
        typedef typename States::iterator stateIterator;
        typedef typename Symbols::iterator symbolIterator;

        typedef std::pair< StName,StName > StatePair;
        //typedef std::set< StatePair > StatePairSet;  this belongs to a broken bit
        //typedef std::map< StatePairSet,St* > StateMap;  this belongs to a broken bit

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
       * @brief access the client information associated with the given state
       *
       * This method provides access to the client information associated with the given 
       * state.
       *
       * @param - name: the name of the state whose client information to retrieve
       * @return the client information associated with the given state
       *
       */
      ClientInfoRefPtr getClientInfo( StName name ) const;

      /**
       * 
       * @brief set the client information associated with the given state
       *
       * This method sets the client information associated with the given state to the 
       * client information provided.
       *
       * @param - name: the name of the state whose client information to set
       * @param - c: the desired client information for the given state
       *
       */
      void setClientInfo( StName name, const ClientInfoRefPtr c );

      /**
       *  
       * @brief tests whether the stuck state is a final state
       *
       * This method tests whether the stuck state is a final state in this NWA.  It 
       * returns true if the stuck state is final and false otherwise.
       *
       * @return true if the stuck state is a final state, false otherwise
       *
       */
      //bool isAbsentAccept( ) const;

      /**
       * 
       * @brief sets the stuck state to be a final state
       *
       * This method sets the stuck state to be a final state.
       *
       */
      //void acceptAbsent( );

      /**
       * 
       * @brief sets the stuck state to be not a final state
       *
       * This method makes the stuck state not a final state.
       *
       */
      //void rejectAbsent( );        

      /**
       *  
       * @brief provides access to the state with the given name
       *
       * This method provides access to the state with the given name via a pointer.
       *
       * @param - name: the name of the desired state
       * @return a pointer to the desired state
       *
       */
      St* getState( const StName & name ) const;

      /**
       *  
       * @brief provides access to all states in the NWA
       *
       * This method provides access to all states in pointer form.
       *
       * @return a set of all states in pointer form
       *
       */
      std::set<St*> getStates( ) const;

      /**
       * 
       * @brief provides access to the names of all states in the NWA
       *
       * This method provides access to the names of all states in the NWA.
       *
       * @return a set of the names of all states in the NWA
       *
       */
      std::set<StName> getStateNames( ) const;
      
      /**
       * 
       * @brief obtains the names of all states that are predecessors of the given state
       *
       * This method returns the names of all states that are predecessors of the given 
       * state.
       *
       * @param - name: the name of the state whose predecessors to look up
       * @return the names of all states that are predecessors of the given state
       *
       */    
      std::set<StName> getPredecessorNames( const StName & name ) const;

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
       * @return a pointer to the newly created duplicate state
       *
       */
      St* duplicateStateOutgoing( const StName & orig, const StName & dup );

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
       * @return a pointer to the newly created duplicate state
       *
       */
      St* duplicateState( const StName & orig, const StName & dup );

      //All States

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
      bool isState( const St * state ) const; 

      /**
       *  
       * @brief add the given state to this NWA
       *
       * This method adds the given state to the state set for this NWA.  If the state 
       * already exists in the NWA, false is returned.  Otherwise, true is returned.
       *
       * @param - state: the state to add
       * @return false if the state already exists in the NWA, true otherwise
       *
       */
      bool addState( St * state );

      /**
       *
       * @brief returns the number of states associated with this NWA
       *
       * This method returns the number of states associated with this NWA. 
       *
       * @return the number of states associated with this NWA
       *
       */
      size_t sizeStates( ) const;

      /**
       *  
       * @brief remove the given state from this NWA
       *
       * This method checks for the given state in the set of initial states, the set of 
       * final states, and the set of states in this NWA.  It then removes the state from 
       * any set that contained it.  Any transitions to or from the state to be removed 
       * are also removed.
       *
       * @param - state: the state to remove
       * @return false if the state does not exist in the NWA, true otherwise
       *
       */
      bool removeState( St * state );

      /**
       *
       * @brief remove all states from the NWA
       *
       * This method removes all states and transitions from the NWA.
       *
       */
      void clearStates( );

      //Initial States

      /**
       *  
       * @brief get the names of all states in the initial state set
       *
       * This method provides access to the names of all states in the initial state set 
       * of this NWA.  Note: An NWA can have an unbounded number of inital states 
       * associated with it.
       *
       * @return set of names for all inital states associated with the NWA
       *
       */
      std::set<StName> getInitialStates( ) const; 
      StName getInitialState( ) const;  //This hinges on the assertion that there is a unique initial state.

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
      bool isInitialState( const St * state ) const;

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
      bool addInitialState( St * state );

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
      bool removeInitialState( St * state );

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
       * @brief get the names of all final states in this NWA
       *
       * This method provides access to the names of all states in the final state set of
       * this NWA.  
       * Note: An NWA can have an unbounded number of final states associated with it.
       *
       * @return set of the names of all final states associated with this NWA
       *
       */
      std::set<StName> getFinalStates( ) const;

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
      bool isFinalState( const St * state ) const;

      /**
       *
       * @brief make the given state a final state
       *
       * This method checks for the given state in the state set of the NWA.  If the state
       * does not exist, it is added to the state set of the NWA.  The given state is then
       * added to the set of final states for the NWA.  If the state already exists in the
       * final state set of the NWA, false is returned.  Otherwise, true is returned.
       *
       * @param - state: the state to add to final state set
       * @return false if the state already exists in the final state set of the NWA
       *
       */
      bool addFinalState( St * state );

      /**
       * 
       * @brief returns the number of final states associated with this NWA
       *
       * This method returns the number of final states associated with this NWA. 
       * Note: This count does include the stuck state if it is a final state. 
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
      bool removeFinalState( St * state );

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
       * @brief test if the given symbol is associated with the NWA
       *
       * This method tests whether the given symbol is in the symbol set associated with 
       * the NWA.
       *
       * @param - sym: the symbol to check
       * @return true if the given symbol is associated with the NWA
       *
       */
      bool isSymbol( const Sym & sym ) const;

      /**
       *
       * @brief add the given symbol to the NWA
       *
       * This method adds the given symbol to the symbol set associated with the NWA. If 
       * the symbol is already associated with the NWA, false is returned. Otherwise, 
       * true is returned.
       *
       * @param - sym: the symbol to add
       * @return false if the symbol is already associated with the NWA
       *
       */
      bool addSymbol( const Sym & sym );

      /**
       *
       * @brief returns the number of symbols associated with this NWA
       *
       * This method returns the number of symbols associated with this NWA.  
       * Note: The epsilon symbol is included in this count.
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
       *
       * @param - sym: the symbol to remove
       * @return false if the symbols is not associated with the NWA
       *
       */
      bool removeSymbol( const Sym & sym );

      /**
       *
       * @brief remove all symbols associated with the NWA
       *
       * This method removes all symbols associated with the NWA.  It also removes all 
       * transitions associated with the NWA as there can be no transitions without at 
       * least one symbol.
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
      bool getSymbol( StName from, StName to, Sym & sym );

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
      bool findTrans( StName from, const Sym & sym, StName to) const;

      /**
       *
       * @brief remove all transitions from the NWA
       *
       * This method removes all transitions from the NWA.  It does not remove any states
       * from the NWA.
       *
       */
      void clearTrans( );

      //Call Transitions

      /**
       * 
       * @brief returns the symbol/entry point pairs associated with the given call site
       *
       * This method provides access to the symbol/entry point pairs associated with the 
       * given call site.
       *
       * @param - callSite: the name of the call site whose symbol/entry point pairs to obtain
       * @return the set of symbol/entry point pairs associated with the given call site
       *
       */
      const std::set<std::pair<Sym,StName>> getEntries( StName callSite ) const;

      /**
       *
       * @brief add a call transition to the NWA
       *
       * This method creates a call transition with the given edge and label information 
       * and adds it to the transition set for the NWA.  If the call transition already 
       * exists in the NWA, false is returned. Otherwise, true is returned.
       *
       * @param - from: the name of the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the name of the state the edge arrives to  
       * @return false if the call transition already exists in the NWA
       *
       */
      bool addCallTrans( StName from, const Sym & sym, StName to );

      /**
       *
       * @brief add a call transition to the NWA
       *
       * This method creates a call transition with the given edge and label information 
       * and adds it to the transition set for the NWA.  If the call transition already 
       * exists in the NWA, false is returned. Otherwise, true is returned.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the call transition already exists in the NWA
       *
       */
      bool addCallTrans( St * from, const Sym & sym, St * to );

      /**
       *
       * @brief add a call transition to the NWA
       *
       * This method adds the given call transition to the transition set  for the NWA.
       * If the call transition already exists in the NWA, false is returned. Otherwise, 
       * true is returned.
       *
       * @param - ct: the call transition to add
       * @return false if the call transition already exists in the NWA
       *
       */
      bool addCallTrans( const Call & ct );

      /**
       *
       * @brief remove a call transition from the NWA
       *
       * This method checks for the call transition with the given edge and label 
       * information in the transition set. If the transition is found, it is removed
       * from the transition set and true is returned.  Otherwise, false is returned.
       *
       * @param - from: name of the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: name of the state the edge arrives to  
       * @return false if the call transition does not exist in the NWA
       *
       */
      bool removeCallTrans( StName from, const Sym & sym, StName to );

      /**
       *
       * @brief remove a call transition from the NWA
       *
       * This method checks for the call transition with the given edge and label 
       * information in the transition set. If the transition is found, it is removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the call transition does not exist in the NWA
       *
       */
      bool removeCallTrans( St * from, const Sym & sym, St * to );

      /**
       *
       * @brief remove a call transition from the NWA
       *
       * This method checks for the call transition in the transition set. If the 
       * transition is found, it is removed from the transition set and true is 
       * returned.  Otherwise, false is returned.
       *
       * @param - ct: the call transition to remove
       * @return false if the call transition does not exist in the NWA
       *
       */
      bool removeCallTrans( const Call & ct );

      //Internal Transitions

      /**
       * 
       * @brief returns the symbol/target pairs associated with the given source
       *
       * This method provides access to the symbol/target pairs associated with the given
       * source.
       *
       * @param - source: the name of the source whose symbol/target pairs to obtain
       * @return the set of symbol/target pairs associated with the given source
       *
       */
      const std::set<std::pair<Sym,StName>> getTargets( StName source ) const;

      /**
       *
       * @brief add an internal transition to the NWA
       *
       * This method creates an internal transition with the given edge and label 
       * information and adds it to the transition set for the NWA.  If the internal 
       * transition already exists in the NWA, false is returned. Otherwise, true is 
       * returned.
       *
       * @param - from: the name of the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the name of the state the edge arrives to  
       * @return false if the internal transition already exists in the NWA
       *
       */
      bool addInternalTrans( StName from, const Sym & sym, StName to );

      /**
       *
       * @brief add an internal transition to the NWA
       *
       * This method creates an internal transition with the given edge and label 
       * information and adds it to the transition set for the NWA.  If the internal 
       * transition already exists in the NWA, false is returned. Otherwise, true is 
       * returned.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the internal transition already exists in the NWA
       *
       */
      bool addInternalTrans( St * from, const Sym & sym, St * to );

      /**
       *
       * @brief add an internal transition to the NWA
       *
       * This method adds the given internal transition to the transition set for the NWA.
       * If the internal transition already exists in the NWA, false is returned. Otherwise, 
       * true is returned.
       *
       * @param - it: internal transition to add to the NWA
       * @return false if the internal transition already exists in the NWA
       *
       */
      bool addInternalTrans( const Internal & it );

      /**
       *
       * @brief remove an internal transition from the NWA
       *
       * This method checks for the internal transition with the given edge and label
       * information in the transition set.  If the transition is found, it is removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       *
       * @param - from: the name of the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the name of the state the edge arrives to  
       * @return false if the internal transition does not exist in the NWA
       *
       */
      bool removeInternalTrans( StName from, const Sym & sym, StName to );

      /**
       *
       * @brief remove an internal transition from the NWA
       *
       * This method checks for the internal transition with the given edge and label 
       * information in the transition set.  If the transition is found, it is removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the internal transition does not exist in the NWA
       *
       */
      bool removeInternalTrans( St * from, const Sym & sym, St * to );

      /**
       *
       * @brief remove an internal transition from the NWA
       *
       * This method checks for the internal transition in the transition set. If the 
       * transition is found, it is removed from the transition set and true is returned.
       * Otherwise, false is returned.
       *
       * @param - it: the internal transition to remove
       * @return false if the internal transition does not exist in the NWA
       *
       */
      bool removeInternalTrans( const Internal & it );

      //Return Transitions

      /**
       * 
       * @brief returns the return sites associated with the given call site
       *
       * This method provides access to the return sites associated with the given
       * call site.
       *
       * @param - callSite: the name of the call site whose return sites to obtain
       * @return the set of return sites associated with the given call site
       *
       */
      std::set<StName> getReturnSites( StName callSite ) const;

      /**
       * 
       * @brief returns the symbol/return site pairs associated with the given exit point 
       *        and call site
       *
       * This method provides access to the symbol/return site pairs associated with the 
       * given exit point and call site.
       *
       * @param - exit: the name of the exit point whose symbol/return site pairs to obtain
       * @param - callSite: the name of the call site whose symbol/return site pairs to obtain
       * @return the set of symbol/return site pairs associated with the given exit point 
       *          and call site
       *
       */
      const std::set<std::pair<Sym,StName>> getReturns( StName exit, StName callSite ) const;

      /**
       *
       * @brief add a return transition to the NWA
       *
       * This method creates a return transition with the given edge and label information 
       * and adds it to the transition set for the NWA.  If the return transition already 
       * exists in the NWA, false is returned. Otherwise, true is returned.
       *
       * @param - from: the name of the state the edge departs from
       * @param - pred: the name of the state from which the call was initiated  
       * @param - sym: the symbol labeling the edge
       * @param - to: the name of the state the edge arrives to  
       * @return false if the return transition already exists in the NWA
       *
       */
      bool addReturnTrans( StName from, StName pred, const Sym & sym, StName to );

      /**
       *
       * @brief add a return transition to the NWA
       *
       * This method creates a return transition with the given edge and label information
       * and adds it to the transition set for the NWA. If the return transition already 
       * exists in the NWA, false is returned. Otherwise, true is returned.
       *
       * @param - from: the state the edge departs from
       * @param - pred: the state from which the call was initiated  
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition already exists in the NWA
       *
       */
      bool addReturnTrans( St * from, St * pred, const Sym & sym, St * to );

      /**
       *
       * @brief add a return transition to the NWA
       *
       * This method adds the given return transition to the transition set for the NWA. 
       * If the return transition already exists in the NWA, false is returned. Otherwise,
       * true is returned.
       *
       * @param - rt: return transition to add to the NWA
       * @return false if the return transition already exists in the NWA
       *
       */
      bool addReturnTrans( const Return & rt );

      /**
       *
       * @brief remove a return transition from the NWA
       *
       * This method checks for all return transitions with the given edge and label
       * information in the transition set.  If any transitions are found, they are removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       *
       * @param - from: the name of the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the name of the state the edge arrives to  
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturnTrans( StName from, const Sym & sym, StName to );

      /**
       *
       * @brief remove a return transition from the NWA
       *
       * This method checks for all return transitions with the given edge and label
       * information in the transition set.  If any transitions are found, they are removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturnTrans( St * from, const Sym & sym, St * to );

      /**
       *
       * @brief remove a return transition from the NWA
       *
       * This method checks for the return transition with the given edge and label 
       * information in the transition set. If the transition is found, it is removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       * 
       * @param - from: name of the state the edge departs from
       * @param - pred: name of the state from which the call was initiated  	   
       * @param - sym: the symbol labeling the edge
       * @param - to: name of the state the edge arrives to  
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturnTrans( StName from, StName pred, const Sym & sym, StName to );

      /**
       *
       * @brief remove a return transition from the NWA
       *
       * This method checks for the return transition with the given edge and label 
       * information in the transition set. If the transition is found, it is removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       * 
       * @param - from: the state the edge departs from
       * @param - pred: the state from which the call was initiated  	   
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturnTrans( St * from, St * pred, const Sym & sym, St * to );

      /**
       *
       * @brief remove a return transition from the NWA
       *
       * This method checks for the return transition in the transition set. If the 
       * transition is found, it is removed from the transition set and true is returned.
       * Otherwise, false is returned.
       *
       * @param - rt: the return transition to remove
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturnTrans( const Return & rt );

      //Building NWAs

      /**
       *
       * @brief constructs the NWA resulting from the union of this NWA and the NWA 'other'
       *
       * This method constructs the union of this NWA and the NWA 'other' by creating a 
       * new initial state with epsilon transitions to all initial states of the two NWAs.
       * The resulting NWA is NOT guaranteed to be deterministic.
       *
       * @param the NWA with which to union this NWA
       * @return the NWA which is the union of this NWA and the NWA 'other'
       *
       */
      //NWA * unionNWA( NWA other );  //FIX

      /**
       *
       * @brief constructs the NWA resulting from the concatenation of this NWA and the 
       *        NWA 'other'
       *
       * This method constructs the concatenation of this NWA and the NWA 'other' by 
       * adding epsilon transitions from all final states of this NWA to all initial 
       * states of the NWA 'other'.  The resulting NWA is NOT guaranteed to be 
       * deterministic.
       *
       * @param the NWA with which to concatenate this NWA
       * @return the NWA which is the concatenation of this NWA and the NWA 'other'
       *
       */
      //NWA * concat( NWA other );  //FIX

      /**
       *
       * @brief constructs the NWA resulting from performing Kleene-* on this NWA
       *
       * This method constructs the Kleene-* of this NWA by adding epsilon transitions 
       * from all final states of the NWA to all initial states of the NWA.  The 
       * resulting NWA is NOT guaranteed to be deterministic.
       *
       * @return the NWA which is the result of performing Kleene-* on this NWA
       *
       */
      //NWA * star( );  //FIX

      /**
       * 
       * @brief constructs the NWA that is the complement of this NWA
       *
       * This method constructs the complement of this NWA by replacing the set of final 
       * states in this NWA with the set of states that are not final in this NWA.  
       *
       * @return the NWA which is the complement of this NWA
       *
       */
      //NWA * complement( );

      /**
       *
       * @brief constructs a deterministic NWA that is equivalent to this NWA.
       *
       * This method constructs a deterministic NWA that is equivalent to this NWA.
       *
       * @return the NWA which is a deterministed equivalent to this NWA
       *
       */
      //NWA & determinize( ); //FIX

      /**
       *
       * @brief tests whether the NWA is deterministic 
       *
       * This method tests whether the NWA is deterministic or nondeterministic. If the 
       * NWA is deterministic, true is returned.  Otherwise, false is returned.
       *
       * @return true if the NWA is deterministic, false otherwise
       *
       */
      bool isDeterministic( );

      /**
       * @brief constructs the NWA which is the reverse of this NWA
       *
       * This method constructs the NWA which is the reverse of this NWA. It reverses 
       * internal transitions and switches call and return transitions.
       *
       * @return the NWA which is the reverse of this NWA
       * 
       */
      //NWA * reverse( );  //FIX

      /**
       *
       * @brief constructs the NWA which is the intersection of this NWA and the NWA 'other'
       *
       * This method constructs the NWA which accepts only nested words that are accepted 
       * by both this NWA and the NWA 'other'.  The resulting NWA is NOT guaranteed to be 
       * deterministic.	
       *
       * @param - other: the NWA with which to perform the intersection with this NWA
       * @param - result: the NWA which is the intersection of this NWA and the NWA 'other'
       * @return the NWA which is the intersection of this NWA and the NWA 'other'
       *	
       */
      void intersect( const NWA & other, NWA & result ) const;

      /**
       * 
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the given states
       * and assigns the resulting client information to the state with the given name.
       * This method should only be used to intersect client information from the initial
       * states of NWAs (all other client information intersections should use the 
       * specialized methods below). 
       *
       * @param - name1: the first state whose client information to intersect
       * @param - name2: the second state whose client information to intersect
       * @param - result: the name of the state which should recieve the computed client 
       *                  information
       *
       */
      virtual void intersectClientInfo( const St * const name1, 
                              const St * const name2, 
                              const StName &result );
      /**
       * 
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the states 'entry1'
       * and 'entry2' given that the transition that is being created is a call transition 
       * with the given symbol using the information in the given states and assigns the 
       * resulting client information to the state with the given name.
       * This method should only be used to intersect client information for states 
       * immediately following a call transition.
       *
       * @param - call1: the call site associated with the first entry whose client 
       *                  information to intersect
       * @param - entry1: the first entry point whose client information to intersect
       * @param - call2: the call site associated with the second entry whose client
       *                  information to intersect
       * @param - entry2: the second entry point whose client information to intersect
       * @param - resSym: the symbol associated with the transition that is being created
       * @param - result: the name of the state which should recieve the computed client 
       *                  information
       *
       */
      virtual void intersectClientInfoCall( const St * const call1, const St * const entry1, 
                                    const St * const call2, const St * const entry2, 
                                    const Sym &resSym,
                                    const StName &result );  

      /**
       * 
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the states 'tgt1' and
       * 'tgt2' given that the transition that is being created is an internal transition 
       * with the given symbol using the information in the given states and assigns the 
       * resulting client information to the state with the given name.
       * This method should only be used to intersect client information for states 
       * immediately following an internal transition.
       *
       * @param - src1: the source associated with the first target whose client 
       *                  information to intersect
       * @param - tgt1: the first target whose client information to intersect
       * @param - src2: the source associated with the second target whose client
       *                  information to intersect
       * @param - tgt2: the second target whose client information to intersect
       * @param - resSym: the symbol associated with the transition that is being created
       * @param - result: the name of the state which should recieve the computed client 
       *                  information
       *
       */
      virtual void intersectClientInfoInternal( const St *const src1, const St* const tgt1,
                                        const St *const src2, const St* const tgt2,
                                        const Sym &resSym,
                                        const StName &result );

      /**
       * 
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the states 'ret1' and
       * 'ret2' given that the transition that is being created is a return transition with
       * the given symbol using the information in the given states and assigns the 
       * resulting client information to the state with the given name.
       * This method should only be used to intersect client information for states 
       * immediately following a return transition.
       *
       * @param - exit1: the exit point associated with the first return site whose client 
       *                  information to intersect
       * @param - call1: the call site associated with the first return site whose client
       *                  information to intersect
       * @param - ret1: the first return site whose client information to intersect
       * @param - exit2: the exit point associated with the second return site whose client
       *                  information to intersect
       * @param - call2: the call site associated with the second return site whose client
       *                  information to intersect
       * @param - ret2: the second return site whose client information to intersect
       * @param - resSym: the symbol associated with the transition that is being created
       * @param - result: the name of the state which should recieve the computed client 
       *                  information
       *
       */
      virtual void intersectClientInfoReturn( const St * const exit1, const St *const call1, const St *const ret1,
                                      const St * const exit2, const St *const call2, const St *const ret2,
                                      const Sym &resSym,
                                      const StName &result );

      /**
       * 
       * @brief intersect states
       * 
       * This method performs the intersection of the given states and returns the result
       * in the reference parameter 'result'.
       *
       * @param - node1: the first state to intersect
       * @param - node2: the second state to intersect
       * @param - result: the state that results from performing the intersection
       * @return true if the intersection between the given states is possible
       *
       */
      virtual bool nodeIntersect( const St * const node1, const St * const node2, St * & result );

      /**
       * 
       * @brief intersect symbols
       * 
       * This method performs the intersection of the given symbols and returns the result
       * in the reference parameter 'result'.
       *
       * @param - label1: the first symbol to intersect
       * @param - label2: the second symbol to intersect
       * @param - result: the symbol that results from performing the intersection
       * @return true if the intersection between the given symbols is possible
       *
       */
      virtual bool edgeIntersect( const Sym & label1, const Sym & label2, Sym & result );

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
       * 
       */
      //wpds::WPDS plusWPDS( wpds::WPDS base ); //FIX

      /**
       *
       * @brief constructs the NWA equivalent to the given PDS
       *
       * This method constructs the NWA that is equivalent to the given PDS.
       *
       * @param - pds: the WPDS to be converted
       * @return the NWA equivalent to the given PDS
       *
       */
      //static nwa::NWA<St,StName,Sym> PDStoNWA( wpds::WPDS pds ); //FIX

      /**
       *  
       * @brief returns the default program control location for PDSs
       *
       * This method provides access to the default program control location for PDSs.
       * 
       * @return the default program control location for PDSs
       *
       */ 
      static wali::Key getProgramControlLocation( );

      /**
       *  
       * @brief returns the program control location corresponding to the given states
       *
       * This method provides access to the program control location corresponding to
       * the given exit point/call site pair.
       *
       * @param - exit: the exit point associated with the new program control location
       * @param - call: the call site associated with the new program control location
       * @return the program control location corresponding to the given states
       *
       */
      static wali::Key getControlLocation( const St & exit, const St & call );     

      /**
       *
       * @brief constructs the PDS equivalent to this NWA
       *
       * This method constructs the PDS that is equivalent to this NWA.
       *
       * @param - wg: the WeightGen responsible for determining the weights for PDS rules
       * @return the PDS equivalent to this NWA
       *
       */ 
      wpds::WPDS NWAtoPDS( WeightGen<St,Sym> & wg ) const;

      /**
       *
       * @brief constructs the backwards PDS equivalent to this NWA
       *
       * This method constructs the backwards PDS that is equivalent to this NWA.
       *
       * @param - wg: the WeightGen responsible for determining the weights for PDS rules
       * @return the backwards PDS equivalent to this NWA
       *
       */ 
      wpds::WPDS NWAtoBackwardsPDS( WeightGen<St,Sym> & wg ) const;  
      
      /**
       *
       * @brief tests whether the language accepted by this NWA is empty
       *
       * This method tests whether the language accepted by this NWA is empty.
       *
       * @return true if the language accepted by this NWA is empty
       *
       */
      //bool isEmpty( );

      /**
       *
       * @brief tests whether the given nested word is a member of the language accepted 
       *        by this NWA
       *
       * This method tests whether the given nested word is a member of the language 
       * accepted by this NWA.
       *
       * @param - word: the nested word to test
       * @return true if the given nested word is a member of the language accepted by 
       *          this NWA
       *
       */
      //bool isMember( nws::NWS word ); //FIX

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
       * @param - title: ??? 
       * @return the output stream that was printed to
       *
       */
      virtual std::ostream & print_dot( std::ostream & o, std::string title) const;

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

      protected:
      
      //States

      /**
       *
       * @brief add all the states in the given StateSet to the NWA
       *
       * This method adds all of the given states to the state set for the NWA.
       *
       * @param - addStateSet: the StateSet that contains the states to add
       *
       */
      void addAllStates( States addStateSet );  

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
      void addAllInitialStates( States addStateSet ); 

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
       * @param - accept: whether or not the stuck state is an accepting state in the
       *                  state set to add
       *
       */
      void addAllFinalStates( States addStateSet, bool accept ); 

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

      //Symbols

      /**
       *
       * @brief add the given symbols to the NWA
       *
       * This method adds all of the given symbols to the set of symbols associated with
       * the NWA.  
       *
       * @param the symbols to add
       *
       */
      void addAllSymbols( Symbols addSymbolSet );    

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

      //Transitions

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
      callIterator beginCallTrans( ) const;  

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
      callIterator endCallTrans( ) const;  

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
      internalIterator beginInternalTrans( ) const;  

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
      internalIterator endInternalTrans( ) const; 

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
      returnIterator beginReturnTrans( ) const;  

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
      returnIterator endReturnTrans( ) const;  

      //Auxiliary 

      /**
       *
       * @brief constructs the transition table and state map for the deterministic NWA 
       *        that is equivalent to this NWA.
       *
       * This method constructs the transition table and state map for the deterministic 
       * NWA that is equivalent to this NWA.
       *
       * @param - nWdAut: ??? 
       * @param - currState: ??? 
       * @param - callPred: ??? 
       * @return the transition table and state map for the deterministic NWA that is
       *          equivalent to this NWA
       *
       */
      //std::pair<NWA,StateMap> det( NWA nWdAut, StateMap stateMap,StatePairSet currState,
      //                            std::stack<StatePairSet> callPred );  //FIX

      /**
       *
       * @brief tests whether the NWA has a path from the current state to a final state
       *        given the call stack and the set of states already visited
       *
       * This method tests whether the NWA contains a path from the current state to any 
       * final state given the call stack and the set of states already visited.
       * 
       * @param - currState: the state from which to start searching for a path
       * @param - calls: the stack of calls that have been made thus far  
       * @param - visited: the states which have already been visited
       * @return true if there is a path through the NWA from the current state to a 
       *          final state, false otherwise
       *
       */
      //bool NWA::hasPath( St currState, std::stack< St > calls, std::set< St > visited );  //FIX

      /** 
       *
       * @brief tests whether the nested word suffix is accepted by this nested word 
       *        automaton starting at the state with the given name
       *
       * This method tests whether the nested word suffix is accepted by this nested word
       * automaton by simulating the word suffix on the automaton starting at the state 
       * with the given name.  
       *
       * @param - currState: the state in the nested word automaton at which to start the 
       *                      simulation
       * @param - configs: the nested word suffix to match  Q: is this right?
       * @return true if the simulation ends in an accepting state
       *
       */
      //bool matchWord( St currState, std::deque<WordRecConfig<St>> configs );  //FIX

      //
      // Variables
      //
      
      protected:
      
        States states;         
        Symbols symbols;        
        Trans * trans;

        //bool absentAcceptance;
    };

    //
    // Methods
    //

    //Constructors and Destructor
    template <typename St,typename StName,typename Sym>
    NWA<St,StName,Sym>::NWA( )
    {
      states = States();      

      symbols = Symbols();
      //addSymbol(Sym::epsilon);  //Q: restore this?

      trans = new Trans();

      //absentAcceptance = false; //Assume stuck state is not a final state.
    }

    template <typename St,typename StName,typename Sym>
    NWA<St,StName,Sym>::NWA( const NWA & other )
    {
      //Copy data over from 'other'
      states = other.states;
      symbols = other.symbols;
      trans = other.trans;
      //absentAcceptance = other.absentAcceptance;
    }

    template <typename St,typename StName,typename Sym>
    NWA<St,StName,Sym> & NWA<St,StName,Sym>::operator=( const NWA & other )
    {
      if (this == &other)     
        return *this;

      //Wipe any pre-existing data.
      clearStates();
      clearSymbols();
      clearTrans();

      //Copy data over from 'other'
      states = other.states;
      symbols = other.symbols;
      trans = other.trans;
      //absentAcceptance = other.absentAcceptance;

      return *this;
    }

    template <typename St,typename StName,typename Sym>
    NWA<St,StName,Sym>::~NWA( )
    {   
      clearStates();       
      clearSymbols();      
      clearTrans();

      trans->~Trans();    
    }

    //State Accessors

    /**
     * 
     * @brief access the client information associated with the given state
     *
     * @param - name: the name of the state whose client information to retrieve
     * @return the client information associated with the given state
     *
     */
    template<typename St,typename StName,typename Sym>
    typename NWA<St,StName,Sym>::ClientInfoRefPtr NWA<St,StName,Sym>::getClientInfo( StName name ) const
    {
      St * st = getState(name);
      return st->getClientInfo();
    }

    /**
     * 
     * @brief set the client information associated with the given state
     *
     * @param - name: the name of the state whose client information to set
     * @param - c: the desired client information for the given state
     *
     */
    template<typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::setClientInfo( StName name, const ClientInfoRefPtr c )
    {
      St * st = getState(name);
      st->setClientInfo(c);
    }

    /**
     *  
     * @brief tests whether the stuck state is a final state
     *
     * @return true if the stuck state is a final state, false otherwise
     *
     */
    /*template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::isAbsentAccept( ) const
    {
      return absentAcceptance;
    }*/

    /**
     * 
     * @brief sets the stuck state to be a final state
     *
     */
    /*template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::acceptAbsent( )
    {
      absentAcceptance = true;
    }*/

    /**
     * 
     * @brief sets the stuck state to be not a final state
     *
     */
    /*template <typename St,typename StName,typename Sym> 
    void NWA<St,StName,Sym>::rejectAbsent( )
    {
      absentAcceptance = false;
    }*/

    /**
     *  
     * @brief provides access to the state with the given name
     *
     * @param - name: the name of the desired state
     * @return a pointer to the desired state
     *
     */
    template <typename St,typename StName,typename Sym>
    St* NWA<St,StName,Sym>::getState( const StName & name ) const
    {
      return states.getState(name);
    }

    /**
     *  
     * @brief provides access to all states in the NWA
     *
     * @return a set of all states in pointer form
     *
     */
    template <typename St,typename StName,typename Sym>
    std::set<St*> NWA<St,StName,Sym>::getStates( ) const
    {
      return states.getStates();
    }

    /**
     * 
     * @brief provides access to the names of all states in the NWA
     *
     * @return a set of the names of all states in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    std::set<StName> NWA<St,StName,Sym>::getStateNames( ) const
    {
      return states.getStateNames();
    }
    
    /**
     * 
     * @brief obtains the names of all states that are predecessors of the given state
     *
     * @param - name: the name of the state whose predecessors to look up
     * @return the names of all states that are predecessors of the given state
     *
     */       
    template <typename St,typename StName,typename Sym>
    std::set<StName> NWA<St,StName,Sym>::getPredecessorNames( const StName & name ) const
    {
      St* state = getState(name);
      std::set<StName> predNames;
      std::set<Internal> internalset = trans->getTransTo(state);
      for( internalIterator it = internalset.begin(); it != internalset.end(); it++ )
      {
        predNames.insert(it->first);
      }
      std::set<Call> callset = trans->getTransEntry(state);
      for( callIterator ct = callset.begin(); ct != callset.end(); ct++ )
      {
        predNames.insert(ct->first);
      }
      std::set<Return> retset = trans->getTransRet(state);
      for( returnIterator rt = retset.begin(); rt != retset.end(); rt++ )
      {
        predNames.insert(rt->first);
      }
      return predNames;
    }

    
    /**
     * 
     * @brief duplicates the original state
     *
     * @param - orig: the name of the original state, i.e. the state to duplicate
     * @param - dup: the name of the duplicate state
     * @return a pointer to the newly created duplicate state
     *
     */
    template <typename St,typename StName,typename Sym>
    St * NWA<St,StName,Sym>::duplicateStateOutgoing( const StName & orig, const StName & dup )
    {
      St * origSt = getState(orig);
      St * dupSt = new St(dup);

      states.addState(dupSt);
      states.dupState(origSt,dupSt); //duplicate state characteristics

      trans->dupTransOutgoing(origSt,dupSt); //duplicate transitions for the original state

      return dupSt;
    }

    /**
     *   
     * @brief duplicates the original state
     *
     * @param - orig: the name of the original state, i.e. the state to duplicate
     * @param - dup: the name of the duplicate state
     * @return a pointer to the newly created duplicate state
     *
     */
    template <typename St,typename StName,typename Sym>
    St * NWA<St,StName,Sym>::duplicateState( const StName & orig, const StName & dup )
    {
      St * origSt = getState(orig);
      St * dupSt = new St(dup);

      states.addState(dupSt);
      states.dupState(origSt,dupSt); //duplicate state characteristics

      trans->dupTrans(origSt,dupSt); //duplicate transitions for the original state

      return dupSt;
    }

    //All States

    /**
     *  
     * @brief test if a given state is a state of this NWA
     *
     * @param - state: the state to check
     * @return true if the given state is a state of this NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::isState( const St * state ) const
    {
      return states.isState(state);
    }    

    /**
     *  
     * @brief add the given state to this NWA
     *
     * @param - state: the state to add
     * @return false if the state already exists in the NWA, true otherwise
     *
     */
    template <typename St,typename StName,typename Sym> 
    bool NWA<St,StName,Sym>::addState( St * state )
    {
      return states.addState(state);
    }

    /**
     *
     * @brief add all the states in the given StateSet to the NWA
     *
     * @param the StateSet that contains the states to add
     *
     */
    template <typename St,typename StName,typename Sym> 
    void NWA<St,StName,Sym>::addAllStates( States addStateSet )
    {
      states.addAllStates(addStateSet);
    }    

    /**
     *
     * @brief returns the number of states associated with this NWA
     *
     * @return the number of states associated with this NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    size_t NWA<St,StName,Sym>::sizeStates( ) const
    {
      return states.sizeStates();
    }

    /**
     *  
     * @brief remove the given state from this NWA
     *
     * @param - state: the state to remove
     * @return false if the state does not exist in the NWA, true otherwise
     *
     */
    template <typename St,typename StName,typename Sym> 
    bool NWA<St,StName,Sym>::removeState( St * state )
    {
      //Remove transitions associated with the state that was removed.
      if( states.removeState(state) )
      {
        trans->removeTransWith(state);
        return true;
      }      
      return false;
    }    

    /**
     *
     * @brief remove all states from the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::clearStates( )
    {      
      states.clearStates();

      //absentAcceptance = false; //default behavior is restored

      //Since all states are being removed, all transitions are removed as well.
      clearTrans();
    }

    /**
     *
     * @brief provide access to the beginning of the state set
     *
     * @return an iterator pointing to the beginning of the state set
     * 
     */
    template <typename St,typename StName,typename Sym>
    typename NWA<St,StName,Sym>::stateIterator NWA<St,StName,Sym>::beginStates( ) const
    {
      return states.beginStates();
    }

    /**
     * 
     * @brief provide access to the end of the state set
     *
     * @return an iterator pointing just past the end of the state set
     *
     */
    template <typename St,typename StName,typename Sym> 
    typename NWA<St,StName,Sym>::stateIterator NWA<St,StName,Sym>::endStates( ) const
    {
     return states.endStates();
    }    

    //Initial States
    
    /**
     *  
     * @brief get the names of all states in the initial state set
     *
     * @return set of names for all inital states associated with the NWA
     *
     */
    template <typename St,typename StName,typename Sym> 
    std::set<StName> NWA<St,StName,Sym>::getInitialStates( ) const  
    {
      return states.getInitialStateNames();
    }

    template <typename St,typename StName,typename Sym>
    StName NWA<St,StName,Sym>::getInitialState( ) const
    { 
      std::set<StName> sts = states.getInitialStateNames();
      assert(sts.size()==1); // Assume unique initial state
      return *(sts.begin());
    }

    /**
     *  
     * @brief test if the given state is an initial state of this NWA
     *
     * @param - state: the state to check
     * @return true if the given state is an initial state, false otherwise
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::isInitialState( const St * state ) const
    {
      return states.isInitialState(state); 
    }    

    /**
     *  
     * @brief make the given state an initial state in this NWA
     *
     * @param - state: the state to add to initial state set
     * @return false if the state already exists in the initial state set of the NWA
     *
     */
    template <typename St,typename StName,typename Sym> 
    bool NWA<St,StName,Sym>::addInitialState( St * state )
    {
      return states.addInitialState(state);
    }

    /**
     *
     * @brief add all the states in the given StateSet to the initial state set associated 
     *        with this NWA
     *
     * @param the StateSet that contains the states to add
     *
     */
    template <typename St,typename StName,typename Sym> 
    void NWA<St,StName,Sym>::addAllInitialStates( States addStateSet )
    {
      states.addAllInitialStates(addStateSet);
    }

    /**
     *
     * @brief returns the number of initial states associated with this NWA
     *
     * @return the number of initial states associated with this NWA
     *
     */
    template <typename St,typename StName,typename Sym> 
    size_t NWA<St,StName,Sym>::sizeInitialStates( ) const
    {
      return states.sizeInitialStates();
    }

    /**
     * 
     * @brief remove the given state from the initial state set of this NWA
     *
     * @param - state: the state to remove from the initial state set
     * @return false if the state does not exist in the initial state set of this NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::removeInitialState( St * state ) 
    {
      return states.removeInitialState(state);
    }    

    /**
     *
     * @brief remove all states from the initial state set of the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::clearInitialStates( )
    {
      states.clearInitialStates();
    }

    /**
     *
     * @brief provide access to the beginning of the initial state set
     *
     * @return an iterator pointing to the beginning of the initial state set
     *
     */
    template <typename St,typename StName,typename Sym>
    typename  NWA<St,StName,Sym>::stateIterator NWA<St,StName,Sym>::beginInitialStates( ) const
    {
      return states.beginInitialStates();
    }

    /**
     * 
     * @brief provide access to the end of the initial state set
     *
     * @return an iterator pointing just past the end of the initial state set
     *
     */
    template <typename St,typename StName,typename Sym> 
    typename NWA<St,StName,Sym>::stateIterator NWA<St,StName,Sym>::endInitialStates( ) const
    {
      return states.endInitialStates();
    }    

    //Final States
    
    /**
     *
     * @brief get the names of all final states in this NWA
     *
     * @return set of the names of all final states associated with this NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    std::set<StName> NWA<St,StName,Sym>::getFinalStates( ) const
    {
      return states.getFinalStateNames();
    }

    /**
     *
     * @brief test if a state with the given name is a final state
     *
     * @param - state: the state to check
     * @return true if the given state is a final state
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::isFinalState( const St * state ) const
    {
      return states.isFinalState(state);
    }    

    /**
     *
     * @brief make the given state a final state
     *
     * @param - state: the state to add to final state set
     * @return false if the state already exists in the final state set of the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::addFinalState( St * state )
    {
      return states.addFinalState(state);
    }

    /**
     *
     * @brief add all the states in the given StateSet to the final state set associated
     *        with this NWA
     *
     * @param - addStateSet: the StateSet that contains the states to add
     *
     */
    template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::addAllFinalStates( States addStateSet, bool accept ) //Q: do I still need accept?
    {
      //if( accept )
      //  absentAcceptance = true;

      states.addAllFinalStates(addStateSet);
    }

    /**
     * 
     * @brief returns the number of final states associated with this NWA
     *
     * @return the number of final states associated with this NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    size_t NWA<St,StName,Sym>::sizeFinalStates( ) const
    {
      //if( absentAcceptance )
      //  return states.sizeFinalStates() + 1;
      //else
        return states.sizeFinalStates();
    } 

    /**
     *
     * @brief remove the given state from the final state set of the NWA
     *
     * @param - state: the state to remove from the final state set
     * @return false if the state does not exist in the final state set of the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::removeFinalState( St * state )
    {
      return states.removeFinalState(state);
    }    

    /**
     *
     * @brief remove all states from the final state set of the NWA
     *
     */
    template <typename St,typename StName,typename Sym> 
    void NWA<St,StName,Sym>::clearFinalStates( )
    {
      states.clearFinalStates();

      //absentAcceptance = false; //default behavior is restored
    }

    /**
     *
     * @brief provide access to the beginning of the final state set
     *
     * @return an iterator pointing to the beginning of the final state set
     *
     */
    template <typename St,typename StName,typename Sym> 
    typename NWA<St,StName,Sym>::stateIterator NWA<St,StName,Sym>::beginFinalStates( ) const
    {
      return states.beginFinalStates();
    }

    /**
     * 
     * @brief provide access to the end of the final state set
     *
     * @return an iterator pointing just past the end of the final state set
     *
     */  
    template <typename St,typename StName,typename Sym>   
    typename NWA<St,StName,Sym>::stateIterator NWA<St,StName,Sym>::endFinalStates( ) const
    {
      return states.endFinalStates();
    }  

    //Symbol Accessors

    /**
     *
     * @brief test if the given symbol is associated with the NWA
     *
     * @param - sym: the symbol to check
     * @return true if the given symbol is associated with the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::isSymbol( const Sym & sym ) const
    {
      return (symbols.count(sym) >  0);
    }    

    /**
     *
     * @brief add the given symbol to the NWA
     *
     * @param - sym: the symbol to add
     * @return false if the symbol is already associated with the NWA
     *
     */
    template <typename St,typename StName,typename Sym> 
    bool NWA<St,StName,Sym>::addSymbol( const Sym & sym )
    {
      if( symbols.count(sym) > 0 )
        return false;
      symbols.insert(sym);
      return true;
    }

    /**
     *  Q: remove this?
     * @brief add the given symbols to the NWA
     *
     * @param the symbols to add
     *
     */
    //template <typename St,typename StName,typename Sym> 
    //void NWA<St,StName,Sym>::addAllSymbols( Symbols addSymbolSet )
    //{
    //  for( symbolIterator sit = addSymbolSet.begin();
    //    sit != addSymbolSet.end(); sit++ )
    //    addSymbol(*sit);
    //}

    /**
     *
     * @brief returns the number of symbols associated with this NWA
     *
     * @return the number of symbols associated with this NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    size_t NWA<St,StName,Sym>::sizeSymbols( ) const
    {
      return symbols.size();
    }

    /**
     *
     * @brief remove the given symbol from the NWA
     *
     * @param - sym: the symbol to remove
     * @return false if the symbols is not associated with the NWA
     *
     */
    template <typename St,typename StName,typename Sym> 
    bool NWA<St,StName,Sym>::removeSymbol( const Sym & sym )
    {

      if( symbols.count(sym) == 0 )
        return false;
      else if( symbols.erase(sym) )
      {
        trans->removeTransSym(sym);
        return true;
      }      
      return false;
    }    

    /**
     *
     * @brief remove all symbols associated with the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::clearSymbols( )
    {
      symbols.clear();

      //Since all symbols are being removed, all transitions are removed 
      //as well.
      clearTrans();

      //The epsilon symbol should always remain in the symbol set.
      //This will also replace transitions with the epsilon symbol for each 
      //state that exists in the state set to the error state .
      //addSymbol(Sym::epsilon);
    }

    /**
     *  Q: remove this?
     * @brief provide access to the beginning of the symbol set
     *
     * @return an iterator pointing to the beginning of the symbol set
     *
     */
    //template <typename St,typename StName,typename Sym>
    //typename NWA<St,StName,Sym>::symbolIterator NWA<St,StName,Sym>::beginSymbols( ) const
    //{
    //  return symbols.begin();
    //}

    /**
     *  Q: remove this?
     * @brief provide access to the end of the symbol set
     *
     * @return an iterator pointing just past the end of the symbol set
     *
     */
    //template <typename St,typename StName,typename Sym>
    //typename NWA<St,StName,Sym>::symbolIterator NWA<St,StName,Sym>::endSymbols( ) const
    //{
    //  return symbols.end();
    //}

    //Transition Accessors

    /**
     * 
     * @brief finds a symbol which occurs in a transition with the given endpoints
     *
     * @param - from: the source of the desired transition
     * @param - to: the target of the desired transition
     * @param - sym: the location in which to place any symbol labeling such a transition
     * @return true if some such transition is found, false otherwise
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::getSymbol( StName from, StName to, Sym & sym )
    {
      St * fromSt = getState(from);
      St * toSt = getState(to);

      return trans->getSymbol(fromSt,toSt,sym);
    }

    /**
     * 
     * @brief tests whether there exists a transition whose source is 'from',
     *        whose symbol is 'sym', and whose target is 'to'
     *
     * @param - from: the source of the transition
     * @param - sym: the symbol that labels the transition
     * @param - to: the target of the transition
     * @return true if such a transition exists, false otherwise
     * 
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::findTrans( StName from, const Sym & sym, StName to ) const
    {
      St* fromSt = getState(from);
      St* toSt = getState(to);

      return trans->findTrans(fromSt,sym, toSt);
    }

    /**
     *
     * @brief remove all transitions from the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::clearTrans( )
    {
      trans->clear();
    }    

    /**
     * 
     * @brief returns the symbol/entry point pairs associated with the given call site
     *
     * @param - callSite: the call site whose symbol and entry point pairs to obtain
     * @return the set of symbol/entry point pairs associated with the given call site
     *
     */
    template <typename St,typename StName,typename Sym>
    const std::set<std::pair<Sym,StName>> NWA<St,StName,Sym>::getEntries( StName callSite ) const
    {
      St * callSiteSt = getState(callSite);

      const std::set<Call> ent = trans->getEntries(callSiteSt);
      std::set<std::pair<Sym,StName>> entries;
      for( std::set<Call>::const_iterator it = ent.begin(); it != ent.end(); it++ )
      {
        entries.insert(std::pair<Sym,StName>(it->second,it->third));
      }

      return entries;
    }

    /**
     *
     * @brief add a call transition to the NWA
     *
     * @param from: the name of the state the edge departs from
     * @param sym: the name of the symbol labeling the edge
     * @param to: the name of the state the edge arrives to  
     * @return false if the call transition already exists in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::addCallTrans( StName from, const Sym & sym, StName to )
    {
      St * fromSt = getState(from);
      St * toSt = getState(to);

      if(! isState(fromSt)
        || ! isState(toSt) )
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
     * @param from: the State the edge departs from
     * @param sym: the symbol labeling the edge
     * @param to: the State the edge arrives to  
     * @return false if the call transition already exists in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::addCallTrans( St * from, const Sym & sym, St * to )
    {
      if(! isState(from)
        || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        addSymbol(sym);

      Call ct = Call(from->getName(),sym,to->getName());

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
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::addCallTrans( const Call & ct )
    {
      if(! isState(getState(ct.first))
        || ! isState(getState(ct.third)) )
        return false;
      if(! isSymbol(ct.second) )
        addSymbol(ct.second);  

      return trans->addCall(ct);
    }

    /**
     *
     * @brief remove a call transition from the NWA
     *
     * @param from: name of the state the edge departs from
     * @param sym: the symbol labeling the edge
     * @param to: name of the state the edge arrives to  
     * @return false if the call transition does not exist in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::removeCallTrans( StName from, const Sym & sym, StName to )
    {
      St * fromSt = getState(from);
      St * toSt = getState(to);

      if(! isState(fromSt)
        || ! isState(toSt) )
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
     * @param from: the state the edge departs from
     * @param sym: the symbol labeling the edge
     * @param to: the state the edge arrives to  
     * @return false if the call transition does not exist in the NWA
     *
     */
    template <typename St,typename StName,typename Sym> 
    bool NWA<St,StName,Sym>::removeCallTrans( St * from,const Sym & sym, St * to )
    {
      if(! isState(from)
        || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        return false;

      Call ct = Call(from->getName(),sym,to->getName());

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
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::removeCallTrans( const Call & ct )
    {
      if(! isState(getState(ct.first))
        || ! isState(getState(ct.third)) )
        return false;
      if(! isSymbol(ct.second) )
        return false;

      return trans->removeCall(ct);
    }

    /**
     *
     * @brief provide access to the beginning of the call transition set
     *
     * @return an iterator pointing to the beginning of the call transition set
     *
     */
    template <typename St,typename StName,typename Sym> 
    typename NWA<St,StName,Sym>::callIterator NWA<St,StName,Sym>::beginCallTrans( ) const
    {
      return trans->beginCall();
    }

    /**
     * 
     * @brief provide access to the end of the call transition set
     *
     * @return an iterator pointing just past the end of the call transition set
     *
     */
    template <typename St,typename StName,typename Sym>
    typename NWA<St,StName,Sym>::callIterator NWA<St,StName,Sym>::endCallTrans( ) const
    {
      return trans->endCall();
    }

    /**
     * 
     * @brief returns the symbol/target pairs associated with the given source
     *
     * @param - source: the name of the source whose symbol/target pairs to obtain
     * @return the set of symbol/target pairs associated with the given source
     *
     */
    template <typename St,typename StName,typename Sym>
    const std::set<std::pair<Sym,StName>> NWA<St,StName,Sym>::getTargets( StName source ) const
    {
      St * sourceSt = getState(source);

      const std::set<Internal> tgt = trans->getTargets(sourceSt);
      std::set<std::pair<Sym,StName>> targets;
      for( std::set<Internal>::const_iterator it = tgt.begin(); it != tgt.end(); it++ )
      {
        targets.insert(std::pair<Sym,StName>(it->second,it->third));
      }

      return targets;
    }

    /**
     *
     * @brief add an internal transition to the NWA
     *
     * @param from: the name of the state the edge departs from
     * @param sym: the symbol labeling the edge
     * @param to: the name of the state the edge arrives to  
     * @return false if the internal transition already exists in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::addInternalTrans( StName from,const Sym & sym, StName to )
    {
      St * fromSt = getState(from);
      St * toSt = getState(to);

      if(! isState(fromSt)
        || ! isState(toSt) )
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
     * @param from: the state the edge departs from
     * @param sym: the symbol labeling the edge
     * @param to: the state the edge arrives to  
     * @return false if the internal transition already exists in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::addInternalTrans( St * from,const Sym & sym, St * to )
    {
      if(! isState(from)
        || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        addSymbol(sym);  

      Internal it = Internal(from->getName(),sym,to->getName());

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
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::addInternalTrans( const Internal & it )
    {
      if(! isState(getState(it.first))
        || ! isState(getState(it.third)) )
        return false;
      if(! isSymbol(it.second) )
        addSymbol(it.second);  

      return trans->addInternal(it);
    }    

    /**
     *
     * @brief remove an internal transition from the NWA
     *
     * @param from: the name of the state the edge departs from
     * @param sym: the symbol labeling the edge
     * @param to: the name of the state the edge arrives to  
     * @return false if the internal transition does not exist in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::removeInternalTrans( StName from,const Sym & sym, StName to )
    {
      St * fromSt = getState(from);
      St * toSt = getState(to);

      if(! isState(fromSt)
        || ! isState(toSt) )
        return false;
      if(! isSymbol(sym) )
        return false;

      Internal it = Internal(from,sym,to);

      return trans->removeInternal(it);
    }

    /**
     *
     * @brief remove the internal transition with the given edge and 
     * label information from the NWA
     * 
     * @param from: the state the edge departs from
     * @param sym: the symbol labeling the edge
     * @param to: the state the edge arrives to  
     * @return false if the internal transition does not exist in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::removeInternalTrans( St * from,const Sym & sym, St * to )
    {
      if(! isState(from)
        || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        return false;

      Internal it = Internal(from->getName(),sym,to->getName());

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
    template <typename St,typename StName,typename Sym> 
    bool NWA<St,StName,Sym>::removeInternalTrans(  const Internal & it )
    {
      if(! isState(getState(it.first))
        || ! isState(getState(it.third)) )
        return false;
      if(! isSymbol(it.second) )
        return false;

      return trans->removeInternal(it);
    }

    /**
     *
     * @brief provide access to the beginning of the internal transition set
     *
     * @return an iterator pointing to the beginning of the internal transition set
     *
     */
    template <typename St,typename StName,typename Sym>
    typename NWA<St,StName,Sym>::internalIterator NWA<St,StName,Sym>::beginInternalTrans( ) const
    {
      return trans->beginInternal();
    }

    /**
     * 
     * @brief provide access to the end of the internal transition set
     *
     * @return an iterator pointing just past the end of the internal transition set
     *
     */
    template <typename St,typename StName,typename Sym>
    typename NWA<St,StName,Sym>::internalIterator NWA<St,StName,Sym>::endInternalTrans( ) const
    {
      return trans->endInternal();
    }

    /**
     * 
     * @brief returns the return sites associated with the given call site
     *
     * @param - callSite: the name of the call site whose return sites to obtain
     * @return the set of return sites associated with the given call site
     *
     */
    template <typename St,typename StName,typename Sym>
    std::set<StName> NWA<St,StName,Sym>::getReturnSites( StName callSite ) const
    {
      St * callSiteSt = getState(callSite);

      std::set<St> returns = trans->getReturnSites(callSiteSt);
      std::set<StName> rets;
      for(std::set<St>::iterator it = returns.begin(); it != returns.end(); it++ )
      {
        rets.insert(it->getName());
      }
      return rets;
    }

    /**
     * 
     * @brief returns the symbol/return site pairs associated with the given exit point 
     *        and call site
     *
     * @param - exit: the name of the exit point whose symbol/return site pairs to obtain
     * @param - callSite: the name of the call site whose symbol/return site pairs to obtain
     * @return the set of symbol/return site pairs associated with the given exit point 
     *          and call site
     *
     */
    template <typename St,typename StName,typename Sym>
    const std::set<std::pair<Sym,StName>> NWA<St,StName,Sym>::getReturns( StName exit, StName callSite ) const
    {
      St * exitSt = getState(exit);
      St * callSiteSt = getState(callSite);

      const std::set<Return> ret = trans->getReturns(exitSt,callSiteSt);
      std::set<std::pair<Sym,StName>> returns;
      for( std::set<Return>::const_iterator it = ret.begin(); it != ret.end(); it++ )
      {
        returns.insert(std::pair<Sym,StName>(it->third,it->fourth));
      }

      return returns;
    }

    /**
     *
     * @brief add a return transition to the NWA
     *
     * @param from: the name of the state the edge departs from
     * @param pred: the name of the state from which the call was initiated  
     * @param sym: the symbol labeling the edge
     * @param to: the name of the state the edge arrives to  
     * @return false if the return transition already exists in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::addReturnTrans( StName from, StName pred,const Sym & sym, StName to )
    {
      St * fromSt = getState(from);
      St * predSt = getState(pred);
      St * toSt = getState(to);

      if(! isState(fromSt)
        || ! isState(predSt)
        || ! isState(toSt) )
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
     * @param from: the state the edge departs from
     * @param pred: the state from which the call was initiated  
     * @param sym: the symbol labeling the edge
     * @param to: the state the edge arrives to  
     * @return false if the return transition already exists in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::addReturnTrans( St * from, St * pred, const Sym & sym, St * to )
    {
      if(! isState(from)
        || ! isState(pred)
        || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        addSymbol(sym);  

      Return rt = Return(from->getName(),pred->getName(),sym,to->getName());

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
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::addReturnTrans( const Return & rt )
    {
      if(! isState(getState(rt.first))
        || ! isState(getState(rt.second))
        || ! isState(getState(rt.fourth)) )
        return false;
      if(! isSymbol(rt.third) )
        addSymbol(rt.third);  

      return trans->addReturn(rt);
    }

    /**
     *
     * @brief remove a return transition from the NWA
     *
     * @param from: the name of the state the edge departs from
     * @param sym: the symbol labeling the edge
     * @param to: the name of the state the edge arrives to  
     * @return false if the return transition does not exist in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::removeReturnTrans( StName from,const Sym & sym, StName to )
    {
      St * fromSt = getState(from);
      St * toSt = getState(to);

      if(! isState(fromSt)
        || ! isState(toSt) )
        return false;
      if(! isSymbol(sym) )
        return false;

      bool removed = false;
      std::set<St> cls = trans->getCallSites(fromSt,toSt);
      std::set<StName> calls;
      for( std::set<St>::iterator it = cls.begin(); it != cls.end(); it++ )
      {
        calls.insert(it->getName());
      }
      for( std::set<StName>::iterator it = calls.begin(); it != calls.end(); it++ )
      {
        St * predSt = getState(*it);

        if(! isState(predSt) )
          return false;

        Return rt = Return(from,predSt->getName(),sym,to);

        removed = removed || trans->removeReturn(rt);
      }
      return removed;
    }

    /**
     *
     * @brief remove a return transition from the NWA
     *
     * @param from: the state the edge departs from
     * @param sym: the symbol labeling the edge
     * @param to: the state the edge arrives to  
     * @return false if the return transition does not exist in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::removeReturnTrans( St * from, const Sym & sym, St * to )
    {
      if(! isState(from)
        || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        return false;

      bool removed = false;
      std::set<St> cls = trans->getCallSites(from,to);
      std::set<StName> calls;
      for( std::set<St>::iterator it = cls.begin(); it != cls.end(); it++ )
      {
        calls.insert(it->getName());
      }

      for( std::set<StName>::iterator it = calls.begin(); it != calls.end(); it++ )
      {
        St * predSt = getState(*it);

        if(! isState(predSt) )
          return false;

        Return rt = Return(from->getName(),predSt->getName(),sym,to->getName());

        removed = removed || trans->removeReturn(rt);
      }
      return removed;
    }

    /**
     *
     * @brief remove a return transition from the NWA
     *
     * @param from: name of the state the edge departs from
     * @param pred: name of the state from which the call was initiated  	   
     * @param sym: the symbol labeling the edge
     * @param to: name of the state the edge arrives to  
     * @return false if the return transition does not exist in the NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::removeReturnTrans( StName from, StName pred, const Sym & sym, StName to )
    {
      St * fromSt = getState(from);
      St * predSt = getState(pred);
      St * toSt = getState(to);

      if(! isState(fromSt)
        || ! isState(predSt)
        || ! isState(toSt) )
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
     * @param from: the state the edge departs from
     * @param pred: the state from which the call was initiated  	   
     * @param sym: the symbol labeling the edge
     * @param to: the state the edge arrives to  
     * @return false if the return transition does not exist in the NWA
     *
     */
    template <typename St,typename StName,typename Sym> 
    bool NWA<St,StName,Sym>::removeReturnTrans( St * from, St * pred, const Sym & sym, St * to )
    {
      if(! isState(from)
        || ! isState(pred)
        || ! isState(to) )
        return false;
      if(! isSymbol(sym) )
        return false;

      Return rt = Return(from->getName(),pred->getName(),sym,to->getName());

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
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::removeReturnTrans( const Return & rt )
    {
      if(! isState(getState(rt.first))
        || ! isState(getState(rt.second))
        || ! isState(getState(rt.fourth)) )
        return false;
      if(! isSymbol(rt.third) )
        return false;

      return trans->removeReturn(rt);
    }

    /**
     *
     * @brief provide access to the beginning of the return transition set
     *
     * @return an iterator pointing to the beginning of the return transition set
     *
     */
    template <typename St,typename StName,typename Sym>
    typename NWA<St,StName,Sym>::returnIterator NWA<St,StName,Sym>::beginReturnTrans( ) const
    {
      return trans->beginReturn();
    }

    /**
     * 
     * @brief provide access to the end of the return transition set
     *
     * @return an iterator pointing just past the end of the return transition set
     *
     */
    template <typename St,typename StName,typename Sym>
    typename NWA<St,StName,Sym>::returnIterator NWA<St,StName,Sym>::endReturnTrans( ) const
    {
      return trans->endReturn();
    }

    //Building NWAs

    /**
     *
     * @brief constructs the NWA resulting from the union of this NWA and the NWA 'other'
     *
     * @param the NWA with which to union this NWA
     * @return the NWA which is the union of this NWA and the NWA 'other'
     *
     */
    /*  Note: broken
    template<typename St,typename StName,typename Sym > 
    NWA<St,StName,Sym> * NWA<St,StName,Sym>::unionNWA( NWA<St,StName,Sym> other )
    { //FIX
      NWA<St,StName,Sym> * newNWA = new NWA<St,StName,Sym>();

      //Generate new states, adding them to the state set as well as
      //to the initial state set and final state set as needed.
      St newSt;
      for( stateIterator fit = beginStates(); fit != endStates(); fit++ )
      {
        for( stateIterator sit = other.beginStates(); sit != other.endStates(); sit++ )
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
    }*/

    /**
     *
     * @brief constructs the NWA resulting from the concatenation of this NWA and the 
     *        NWA 'other'
     *
     * @param the NWA with which to concatenate this NWA
     * @return the NWA which is the concatenation of this NWA and the NWA 'other'
     *
     */
    /*  Note: broken
    template<typename St,typename StName,typename Sym > 
    NWA<St,StName,Sym> * NWA<St,StName,Sym>::concat( NWA<St,StName,Sym> other )
    { //FIX
      NWA<St,StName,Sym> * newNWA = new NWA<St,StName,Sym>();

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
      for( stateIterator fit = beginFinalStates(); fit != endFinalStates(); fit++ )
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
    }*/

    /**
     *
     * @brief constructs the NWA resulting from performing Kleene-* on this NWA
     *
     * @return the NWA which is the result of performing Kleene-* on this NWA
     *
     */
    /*  Note: broken
    //TODO: check this with respect to calls/returns
    template<typename St,typename StName,typename Sym >
    NWA<St,StName,Sym> * NWA<St,StName,Sym>::star( )
    { //FIX
      NWA<St,StName,Sym> * newNWA = new NWA<St,StName,Sym>();

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
    }*/

    /**
     * 
     * @brief constructs the NWA that is the complement of this NWA
     *
     * @return the NWA which is the complement of this NWA
     *
     */
    /* template<typename St,typename StName,typename Sym > 
    NWA<St,StName,Sym> * NWA<St,StName,Sym>::complement( )
    {
      NWA<St,StName,Sym> * newNWA = new NWA<St,StName,Sym>();

      //Start with a deterministic NWA.
      //if(! this->isDeterministic() )
      //  determinize();
      assert( isDeterministic() );  //Note: for now must start with deterministic nwa
      // since determinize is broken      

      //The new NWA will have all the same states and initial states.
      newNWA->addAllStates(states);
      newNWA->addAllInitialStates(states);

      newNWA->addAllSymbols(symbols);

      //The new NWA will have all the same transitions.
      //newNWA->addAllCallTrans(*this->trans);
      //newNWA->addAllInternalTrans(*this->trans);
      //newNWA->addAllReturnTrans(*this->trans);    
      newNWA->trans->addAllTrans(*this->trans);

      //All states that are final states of this NWA
      //are not final states of the new NWA and all states 
      //not in the final state set of this NWA are final
      //states of the new NWA.
      std::set<St*> oldFinalStates;
      for( stateIterator fit = beginFinalStates();
                          fit != endFinalStates(); fit++ )
        oldFinalStates.insert(*fit);

      for( stateIterator sit = beginStates();
                          sit != endStates(); sit++ )
      {
        if( oldFinalStates.count(*sit) == 0 )
        newNWA->addFinalState(*sit);
      }

      //If this NWA doesn't accept the stuck state, the complement should.
      if( !absentAcceptance )
        newNWA->absentAcceptance;

      return newNWA;
    }*/

    /**
     *
     * @brief constructs a deterministic NWA that is equivalent to this NWA.
     *
     * @return the NWA which is a deterministed equivalent to this NWA
     *
     */
    /*  Note: broken
    template<typename St,typename StName,typename Sym > 
    NWA<St,StName,Sym> & NWA<St,StName,Sym>::determinize( )
    { //FIX
      std::pair<NWA<St,StName,Sym>,StateMap> tmp = std::pair<NWA<St,StName,Sym>,StateMap>();

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
    }*/

    /**
     *
     * @brief constructs the transition table and state map for the deterministic NWA 
     *        that is equivalent to this NWA.
     *
     * @return the transition table and state map for the deterministic NWA that is
     *          equivalent to this NWA
     *
     */
    /* broken
    template<typename St,typename StName,typename Sym > 
    std::pair<NWA<St,StName,Sym>,typename NWA<St,StName,Sym>::StateMap> NWA<St,StName,Sym>::det( NWA<St,StName,Sym> nWdAut,
                                                                            StateMap stateMap, StatePairSet currState,
                                                                            std::stack<StatePairSet> callPred )
    { //FIX
      //Internal Transition
      for( symbolIterator it = nWdAut.symbols.begin();
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
        
      return std::pair<NWA<St,StName,Sym>,StateMap>(nWdAut,stateMap);
    }*/

    /**
     *
     * @brief tests whether the NWA is deterministic 
     *
     * @return true if the NWA is deterministic, false otherwise
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::isDeterministic( )
    { 
      //TODO: optimize!!!
      //An NWA is not deterministic if there is not exactly one initial state
      if( sizeInitialStates() != 1) 
        return false;

      //An NWA is not deterministic if there are epsilon transitions 
      //or if there are multiple transitions of some
      //type that are identical except for the to state.
      for( symbolIterator it = symbols.begin(); it != symbols.end(); it++ )
      {
        for( stateIterator sit = beginStates(); sit != endStates(); sit++ )
        {
          //Check call transitions.
          int count = 0;
          bool wild = false;
          for( callIterator cit = trans->beginCall(); cit != trans->endCall(); cit++ )
          {
            //Epsilon transition 
            if( cit->second.isEpsilon() ) 
              return false;
            //Wild symbol
            if( cit->second.isWild() )
              wild = true;  

            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (cit->first == (*sit)) && (cit->second == *it) )
              count++;
          }
          if( count > 1 )
            return false;
          else if( wild && count > 0 )
            return false;

          //Check internal transitions.
          count = 0;
          wild = false;
          for( internalIterator iit = trans->beginInternal(); iit != trans->endInternal(); iit++ )
          {
            //Epsilon transition 
            if( iit->second.isEpsilon() )
              return false;
            //Wild symbol
            if( iit->second.isWild() )
              wild = true;  

            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (iit->first == *sit) && (iit->second == *it) )
              count++;
          }
          if( count > 1 )
            return false;
          else if( wild && count > 0 )
            return false;  

          for( stateIterator pit = beginStates(); pit != endStates(); pit++ )
          {
            //Check return transitions.
            count = 0;
            wild = false;
            for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
            {
              //Epsilon transition to a state other than the error state.
              if( rit->third.isEpsilon() )
                return false;
              //Wild symbol
              if( rit->third.isWild() )
                wild = true; 

              //Keep a count of multiple transitions with the same from
              //state and symbol(that is not epsilon).
              else if( (rit->first == *sit) &&
                (rit->second == *pit) &&
                (rit->third == *it) )
                count++;
            }
            if( count > 1 )
              return false;
            else if( wild && count > 0 )
              return false;  
          }
        }
      }

      return true;
    }

    /**
     * @brief constructs the NWA which is the reverse of this NWA
     *
     * @return the NWA which is the reverse of this NWA
     * 
     */
    /*  Note: broken
    template<typename St,typename StName,typename Sym > 
    NWA<St,StName,Sym> * NWA<St,StName,Sym>::reverse()
    { //FIX
      //TODO: fix this for absent acceptance and wild symbol
      NWA<St,StName,Sym> * newNWA = new NWA<St,StName,Sym>();

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
    }*/

    /**
     *
     * @brief constructs the NWA which is the intersection of this NWA and the NWA 'other'
     *
     * @param - other: the NWA with which to perform the intersection with this NWA
     * @param - result: the NWA which is the intersection of this NWA and the NWA 'other'
     * @return the NWA which is the intersection of this NWA and the NWA 'other'
     *	
     */
    //TODO: incomplete...!!
    template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::intersect( const NWA<St,StName,Sym> & other, NWA<St,StName,Sym> & result ) const
    {
      //assert(!absentAcceptance && !other.absentAcceptance);
      //If both NWAs to be intersected accept absent, the intersection should too.
      //Otherwise, the intersection should not, so keep the default settings.
      //if( absentAcceptance && other.absentAcceptance )
      //  result.acceptAbsent();

      std::set<StatePair> visitedPairs; // All the pairs of states we have ever encountered, whether they are intersectable or not
      std::deque<StatePair> worklistPairs; // Pairs of states yet to be processed
      typedef std::map<StatePair, St*> PairStMap;
      PairStMap pairToStMap; // The corresponding state in the product 
                             // for all intersectable pairs encountered

      //Start the worklist with all possible initial states of the intersection NWA.
      //NOTE: Currently this should be just one state (the product of the single initial states of each machine).
      for( stateIterator fit = beginInitialStates(); fit != endInitialStates(); fit++ ) {
        for( stateIterator sit = other.beginInitialStates(); sit != other.endInitialStates(); sit++ ) {
          St * newSt;
          StatePair sp((*fit), (*sit));
          visitedPairs.insert( sp );
          if( result.nodeIntersect(getState(*fit), other.getState(*sit), newSt) ) {
            result.intersectClientInfo( getState(*fit), other.getState(*sit), newSt->getName());   //Intersect initial client info.
            result.addInitialState( newSt ); 
            //an initial state could also be a final state.
            if(this->isFinalState(getState(*fit)) && other.isFinalState(other.getState(*sit)))
                result.addFinalState( newSt );
            worklistPairs.push_back( sp );
            pairToStMap[sp] = newSt;
          }  
        }      
      }

      while(!worklistPairs.empty() ) {

        StatePair currpair = worklistPairs.front();
        worklistPairs.pop_front();

        //Process outgoing call transitions
        Calls thisCalls = trans->getTransCall(getState(currpair.first));
        Calls otherCalls = other.trans->getTransCall(other.getState(currpair.second));
        for(Calls::const_iterator fit = thisCalls.begin(); fit!=thisCalls.end(); fit++) {
          Sym thisSym = fit->second;
          StName thisTgt = fit->third;
          for(Calls::const_iterator sit = otherCalls.begin(); sit!=otherCalls.end(); sit++) {
            Sym otherSym = sit->second;
            // Are the symbols intersectable
            Sym resSym;
            if( !result.edgeIntersect(thisSym, otherSym, resSym) ) 
              continue; // Symbols not intersectable, do nothing

            StName otherTgt = sit->third;
            StatePair tgtPair(thisTgt, otherTgt);
            
            // If we have already considered tgtPair and found them to be nonintersectable, continue 
            if( visitedPairs.count(tgtPair) != 0 && pairToStMap.count(tgtPair)==0)
              continue;
            visitedPairs.insert(tgtPair);

            St * resSt;
            // Have we seen tgtPair before?
            if(pairToStMap.count(tgtPair)==0) { 
              //We have not seen this pair before
              // Are the tgt nodes intersectable?
              if(!result.nodeIntersect(getState(thisTgt), other.getState(otherTgt), resSt)) 
                continue;
              // We have a new state in resSt!
              if(this->isFinalState(getState(thisTgt)) && other.isFinalState(other.getState(otherTgt)))
                result.addFinalState(resSt);
              else
                result.addState(resSt);
              pairToStMap[tgtPair] = resSt;
              worklistPairs.push_back(tgtPair);
            } else { 
              // we have already seen this pair before
              resSt = pairToStMap[tgtPair];
            }
            
            //result.intersectClientInfoCall(*fit,*sit,resSt->getName());   //Intersect Call Trans client info.
            result.intersectClientInfoCall( getState( fit->first), getState( fit->third ),
                                            other.getState( sit->first), other.getState( sit->third ),
                                            resSym,
                                            resSt->getName());   //Intersect Call Trans client info.
            result.addCallTrans(pairToStMap[currpair], resSym, resSt);
          }
        }

        // Process outgoing internal transitions
        Internals thisInternals = trans->getTransFrom(getState(currpair.first));
        Internals otherInternals = other.trans->getTransFrom(other.getState(currpair.second));
        for(Internals::const_iterator fit = thisInternals.begin(); fit!=thisInternals.end(); fit++) {
          Sym thisSym = fit->second;
          StName thisTgt = fit->third;
          for(Internals::const_iterator sit = otherInternals.begin(); sit!=otherInternals.end(); sit++) {
            Sym otherSym = sit->second;
            // Are the symbols intersectable
            Sym resSym;
            if( !result.edgeIntersect(thisSym, otherSym, resSym) ) 
              continue; // Symbols not intersectable, do nothing

            StName otherTgt = sit->third;
            StatePair tgtPair(thisTgt, otherTgt);

            // If we have already considered tgtPair and found its elements to be nonintersectable, continue 
            if( visitedPairs.count(tgtPair) != 0 && pairToStMap.count(tgtPair)==0)
              continue;
            visitedPairs.insert(tgtPair);

            St * resSt;
            // Have we seen tgtPair before?
            if(pairToStMap.count(tgtPair)==0) { 
              //We have not seen this pair before
              // Are the tgt nodes intersectable?
              if(!result.nodeIntersect( getState( thisTgt ), other.getState( otherTgt ), resSt)) 
                continue;
              // We have a new state in resSt!
              if(this->isFinalState(getState(thisTgt)) && other.isFinalState(other.getState(otherTgt)))
                result.addFinalState(resSt);
              else
                result.addState(resSt);
              pairToStMap[tgtPair] = resSt;
              worklistPairs.push_back(tgtPair);
            } else { 
              // we have already seen this pair before
              resSt = pairToStMap[tgtPair];
            }
            result.intersectClientInfoInternal( getState(fit->first), getState(fit->third),
                                                other.getState(sit->first), other.getState(sit->third),
                                                resSym,
                                                resSt->getName());   //Intersect Internal Trans client info.
            result.addInternalTrans(pairToStMap[currpair], resSym, resSt);
          }
        }
        
        // Process outgoing return transitions where currpair.first and currpair.second are
        // both exit components of the respective return transitions
        Returns thisReturns = trans->getTransExit(getState(currpair.first));
        Returns otherReturns = other.trans->getTransExit(other.getState(currpair.second));
        for(Returns::const_iterator fit = thisReturns.begin(); fit!=thisReturns.end(); fit++) {
          StName thisCall = fit->second;
          Sym thisSym = fit->third;
          StName thisRet = fit->fourth;
          for(Returns::const_iterator sit = otherReturns.begin(); sit!=otherReturns.end(); sit++) {
            Sym otherSym = sit->third;
            // Are the symbols intersectable
            Sym resSym;
            if( !result.edgeIntersect(thisSym, otherSym, resSym) ) 
              continue; // Symbols not intersectable, do nothing

            // Check intersectability and visited status of the respective call components
            StName otherCall = sit->second;
            StatePair callPair(thisCall, otherCall);  // Call components of the respective return transitions
            // Proceed only if the pair of call components has already been visited 
            if(visitedPairs.count(callPair) == 0)
              continue;
            // If we have already considered callPair and found its elements to be nonintersectable, continue 
            if( visitedPairs.count(callPair) != 0 && pairToStMap.count(callPair)==0)
              continue;
            St * callSt = pairToStMap[callPair];

            // Check intersectability and visited status of the respective return components
            StName otherRet = sit->fourth;
            StatePair retPair(thisRet, otherRet);
            // If we have already considered retPair and found its elements to be nonintersectable, continue 
            if( visitedPairs.count(retPair) != 0 && pairToStMap.count(retPair)==0)
              continue;
            visitedPairs.insert(retPair);

            St * retSt;
            // Are the return components intersectable?
            if(pairToStMap.count(retPair)==0) { // Don't know yet
              if(!result.nodeIntersect( getState( thisRet ), other.getState(otherRet), retSt)) 
                continue;
              // We have found a new state in retSt!
              if(this->isFinalState(getState(thisRet)) && other.isFinalState(other.getState(otherRet)))
                result.addFinalState(retSt);
              else
                result.addState(retSt);
              pairToStMap[retPair] = retSt;
              worklistPairs.push_back(retPair);
            } else {  // We have already seen retPair before and its components are intersectable
              retSt = pairToStMap[retPair];
            }
            result.intersectClientInfoReturn( getState(fit->first), getState(fit->second), getState(fit->fourth),
                                              other.getState(sit->first), other.getState(sit->second), other.getState(sit->fourth),
                                              resSym,
                                              retSt->getName());   //Intersect Return Trans client info.
            result.addReturnTrans(pairToStMap[currpair], callSt, resSym, retSt);
          }
        }

        // Process outgoing return transitions where currpair.first and currpair.second are
        // both call components of the respective return transitions 
        thisReturns = trans->getTransPred(getState(currpair.first));
        otherReturns = other.trans->getTransPred(other.getState(currpair.second));
        for(Returns::const_iterator fit = thisReturns.begin(); fit!=thisReturns.end(); fit++) {
          StName thisExit = fit->first;
          Sym thisSym = fit->third;
          StName thisRet = fit->fourth;
          for(Returns::const_iterator sit = otherReturns.begin(); sit!=otherReturns.end(); sit++) {
            Sym otherSym = sit->third;
            // Are the symbols intersectable
            Sym resSym;
            if( !result.edgeIntersect(thisSym, otherSym, resSym) ) 
              continue; // Symbols not intersectable, do nothing

            // Check intersectability and visited status of the respective exit components
            StName otherExit = sit->first;
            StatePair exitPair(thisExit, otherExit);  // Exit components of the respective return transitions
            // Proceed only if the pair of exit components has already been visited 
            if(visitedPairs.count(exitPair) == 0)
              continue;
            // If we have already considered exitPair and found its elements to be nonintersectable, continue 
            if( visitedPairs.count(exitPair) != 0 && pairToStMap.count(exitPair)==0)
              continue;
            St * exitSt = pairToStMap[exitPair];

            // Check intersectability and visited status of the respective return components
            StName otherRet = sit->fourth;
            StatePair retPair(thisRet, otherRet);
            // If we have already considered retPair and found its elements to be nonintersectable, continue 
            if( visitedPairs.count(retPair) != 0 && pairToStMap.count(retPair)==0)
              continue;
            visitedPairs.insert(retPair);

            St * retSt;
            //  Are the return components intersectable?
            if(pairToStMap.count(retPair)==0) { //Don't know yet
              if(!result.nodeIntersect( getState( thisRet ), other.getState(otherRet), retSt)) 
                continue;
              // We have a new state in retSt!
              if(this->isFinalState(getState(thisRet)) && other.isFinalState(other.getState(otherRet)))
                result.addFinalState(retSt);
              else
                result.addState(retSt);
              pairToStMap[retPair] = retSt;
              worklistPairs.push_back(retPair);
            } else { //  We have already seen retPair before and its components are intersectable
              retSt = pairToStMap[retPair];
            }
            result.intersectClientInfoReturn( getState(fit->first), getState(fit->second), getState(fit->fourth),
                                              other.getState(sit->first), other.getState(sit->second), other.getState(sit->fourth),
                                              resSym,
                                              retSt->getName());   //Intersect Return Trans client info.
            result.addReturnTrans(exitSt, pairToStMap[currpair], resSym, retSt);
          }
        }

      }// end while

      return;
    } // end intersect

    /**
     * 
     * @brief intersects the client information associated with the given states
     *
     * @param - name1: the first state whose client information to intersect
     * @param - name2: the second state whose client information to intersect
     * @param - result: the name of the state which should recieve the computed client 
     *                  information
     *
     */
    template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::intersectClientInfo( const St * const name1, const St *name2, const StName &result ) 
    { 
      //Note: When overriding this method your metric must combine any client information associated
      // with the states associated with the given names and set the client information of result
      // to that value.
    }

    /**
     * 
     * @brief intersects client information 
     *
     * @param - call1: the call site associated with the first state whose client 
     *                  information to intersect
     * @param - entry1: the first state whose client information to intersect
     * @param - call2: the call site associated with the second state whose client
     *                  information to intersect
     * @param - entry2: the second state whose client information to intersect
     * @param - resSym: the symbol associated with the transition that is being created
     * @param - result: the name of the state which should recieve the computed client 
     *                  information
     *
     */
    template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::intersectClientInfoCall( const St * const call1, const St * const entry1, 
                                                      const St * const call2, const St * const entry2, 
                                                      const Sym &resSym,
                                                      const StName &result )
    { 
      //Note: When overriding this method your metric must combine any client information associated
      // with the target states of the two transitions and set the client information of result
      // to that value.
    }

    /**
     * 
     * @brief intersects client information 
     *
     * @param - src1: the source associated with the first target whose client 
     *                  information to intersect
     * @param - tgt1: the first target whose client information to intersect
     * @param - src2: the source associated with the second target whose client
     *                  information to intersect
     * @param - tgt2: the second target whose client information to intersect
     * @param - resSym: the symbol associated with the transition that is being created
     * @param - result: the name of the state which should recieve the computed client 
     *                  information
     *
     */
    template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::intersectClientInfoInternal( const St *const src1, const St* const tgt1,
                                                          const St *const src2, const St* const tgt2,
                                                          const Sym &resSym,
                                                          const StName &result )
    { 
      //Note: When overriding this method your metric must combine any client information associated
      // with the target states of the two transitions and set the client information of result
      // to that value.
    }

    /**
     * 
     * @brief intersects client information 
     *
     * @param - exit1: the exit point associated with the first return site whose client 
     *                  information to intersect
     * @param - call1: the call site associated with the first return site whose client
     *                  information to intersect
     * @param - ret1: the first return site whose client information to intersect
     * @param - exit2: the exit point associated with the second return site whose client
     *                  information to intersect
     * @param - call2: the call site associated with the second return site whose client
     *                  information to intersect
     * @param - ret2: the second return site whose client information to intersect
     * @param - resSym: the symbol associated with the transition that is being created
     * @param - result: the name of the state which should recieve the computed client 
     *                  information
     *
     */
    template <typename St,typename StName,typename Sym>
    void NWA<St,StName,Sym>::intersectClientInfoReturn( const St * const exit1, const St *const call1, const St *const ret1,
                                                        const St * const exit2, const St *const call2, const St *const ret2,
                                                        const Sym &resSym,
                                                        const StName &result )
    { 
      //Note: When overriding this method your metric must combine any client information associated
      // with the target states of the two transitions and set the client information of result
      // to that value.
    }

    /**
     * 
     * @brief intersect states
     * 
     * @param - node1: the first state to intersect
     * @param - node2: the second state to intersect
     * @param - result: the state that results from performing the intersection
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::nodeIntersect( const St * const node1, const St * const node2, St * & result )
    {
      //Note: When overriding this method your metric must determine whether
      // the given states are compatible, then create a state and set result
      // to the state just created if they are compatible.

      return false;
    }

    /**
     * 
     * @brief intersect symbols
     * 
     * @param - label1: the first symbol to intersect
     * @param - label2: the second symbol to intersect
     * @param - result: the symbol that results from performing the intersection
     *
     */
    template <typename St,typename StName,typename Sym>
    bool NWA<St,StName,Sym>::edgeIntersect( const Sym & label1, const Sym & label2, Sym & result )
    {
      //Note: When overriding this method your metric must determine an
      // appropriate label, create a symbol with that label, and set result
      // to the symbol just created.

      if( label1.isWild() )  //If we have a wild symbol, whatever the other symbol is survives (even if it is also wild).
      {
        result = label2;
        return true;
      }
      else if( label2.isWild() ) //If the other symbol is wild, whatever this symbol is survives. 
      {
        result = label1;
        return true;
      }
      else if( label1 == label2 ) //This rule still applies for epsilons, i.e. epsilons only match epsilons
      {
        result = label1;
        return true;
      }
      else
        return false;
    }

    //Using NWAs

    /**
     * 
     * @brief constructs the WPDS which is the result of the explicit NWA plus WPDS 
     *        construction from Advanced Querying for Property Checking
     *
     * @param the WPDS that forms the basis for the constructed WPDS
     * @return the WPDS which can be used to perform property checking using PDS 
     *          reachability
     * 
     */
    /*  Note: broken
    template<typename St,typename StName,typename Sym > 
    wpds::WPDS NWA<St,StName,Sym>::plusWPDS( wpds::WPDS base )
    { //FIX
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
    }*/

    /**
     *
     * @brief constructs the NWA equivalent to the given PDS
     *
     * @return the NWA equivalent to the given PDS
     *
     */
    /*  Note: broken
    template<typename St, typename Sym > 
    nwa::NWA<St,StName,Sym> NWA<St,StName,Sym>::PDStoNWA(wpds::WPDS pds)
    { //FIX
      nwa::NWA<St,StName,Sym> result = nwa::NWA<St,StName,Sym>();

      std::map<St,St> call_return;

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
    }*/

    /**
     *  
     * @brief returns the default program control location for PDSs
     *
     * @return the default program control location for PDSs
     *
     */ 
    template <typename St,typename StName,typename Sym> 
    wali::Key NWA<St,StName,Sym>::getProgramControlLocation()
    {
      return wali::getKey("program");
    } 

    /**
     *  
     * @brief returns the program control location corresponding to the given states
     *
     * This method provides access to the program control location corresponding to
     * the given exit point/call site pair.
     *
     * @return the program control location corresponding to the given states
     *
     */
    template <typename St,typename StName,typename Sym>
    wali::Key NWA<St,StName,Sym>::getControlLocation( const St & exit, const St & call )
    {
      return getKey(exit.getStateKey(), call.getStateKey());
    }  

    /**
     *
     * @brief constructs the PDS equivalent to this NWA
     *
     * @return the PDS equivalent to this NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    wpds::WPDS NWA<St,StName,Sym>::NWAtoPDS( WeightGen<St,Sym> & wg ) const
    { 
      wpds::WPDS result = wpds::WPDS();

      //note: if you change this, make sure you modify the code in createCA as well
      Key program = getProgramControlLocation();  // = wali::getKey("program"); 

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( internalIterator iit = trans->beginInternal(); iit != trans->endInternal(); iit++ )
      {      
        // (q,sigma,q') in delta_i goes to <p,q> -w-> <p,q'> in delta_1
        // where the weight w depends on sigma

        if( iit->second.isWild() )
          wgt = wg.getWildWeight(*getState(iit->first),*getState(iit->third));
        else
          wgt = wg.getWeight(*getState(iit->first),
                              iit->second,
                              WeightGen<St,Sym>::INTRA,
                              *getState(iit->third));

        result.add_rule(program,                                //from_state (p)
                        getState(iit->first)->getStateKey(),    //from_stack (q)
                        program,                                //to_state (p)
                        getState(iit->third)->getStateKey(),    //to_stack1 (q')
                        wgt);                                   //weight      
      }

      //Call Transitions
      for( callIterator cit = trans->beginCall(); cit != trans->endCall(); cit++ )
      {           
        // (q_c,sigma,q_e) in delta_c goes to
        // <p,q_c> -w-> <p,q_e q_c> in delta_2 
        // and the weight w depends on sigma

        if( cit->second.isWild() )
          wgt = wg.getWildWeight(*getState(cit->first),*getState(cit->third));
        else
          wgt = wg.getWeight(*getState(cit->first),
                              cit->second,
                              WeightGen<St,Sym>::CALL_TO_ENTRY,
                              *getState(cit->third));

        result.add_rule(program,                                //from_state (p)
                        getState(cit->first)->getStateKey(),    //from_stack (q_c)
                        program,                                //to_state (p)
                        getState(cit->third)->getStateKey(),    //to_stack1 (q_e)
                        getState(cit->first)->getStateKey(),    //to_stack2 (q_c)
                        wgt);                                   //weight  
      } 
      
      //Return Transitions
      for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
      {
        // (q_x,q_c,sigma,q_r) in delta_r goes to 
        // <p,q_x> -w1-> <p_q_x,epsilon> in delta_0
        // and <p_q_x,q_c> -w2-> <p,q_r> in delta_1
        // where p_q_x = (p,q_x), w1 depends on q_x, and w2 depends on sigma

        wgt = wg.getExitWeight( getState(rit->first)->getStateKey() );  //w1

        //Note: if you change this, make sure you modify the code in NWPForest.createCA()
        Key rstate = getControlLocation(program,*getState(rit->first));  //p_q_x

        result.add_rule(program,                              //from_state (p)
                        getState(rit->first)->getStateKey(),  //from_stack (q_x)
                        rstate,                               //to_state (p_q_x == (p,q_x))
                        wgt);                                 //weight (w1) 

        if( rit->third.isWild() )
          wgt = wg.getWildWeight(*getState(rit->first),*getState(rit->fourth)); //w2
        else
          wgt = wg.getWeight(*getState(rit->first), 
                              rit->third,
                              WeightGen<St,Sym>::EXIT_TO_RET,  
                              *getState(rit->fourth));          //w2
         
          result.add_rule(rstate,                               //from_state (p_q_x == (p,q_x))
                          getState(rit->second)->getStateKey(), //from_stack (q_c)
                          program,                              //to_state (p)
                          getState(rit->fourth)->getStateKey(), //to_stack (q_r)
                          wgt);                                 //weight (w2)    

      }

      return result;
    }

    /**
     *
     * @brief constructs the backwards PDS equivalent to this NWA
     *
     * @return the backwards PDS equivalent to this NWA
     *
     */
    template <typename St,typename StName,typename Sym>
    wpds::WPDS NWA<St,StName,Sym>::NWAtoBackwardsPDS( WeightGen<St,Sym> & wg ) const
    {
      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  //= wali::getKey("program"); 

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( internalIterator iit = trans->beginInternal(); iit != trans->endInternal(); iit++ )
      {      
        //(q,sigma,q') in delta_i goes to <p,q> -w-> <p,q'> in delta_1
        //where the weight w depends on sigma

        if( iit->second.isWild() )
          wgt = wg.getWildWeight(*getState(iit->first),*getState(iit->third));
        else
          wgt = wg.getWeight(*getState(iit->first),
                              iit->second,
                              WeightGen<St,Sym>::INTRA,
                              *getState(iit->third));

        result.add_rule(program,                //from_state (p)
                        getState(iit->third)->getStateKey(),   //from_stack (q)
                        program,                //to_state (p)
                        getState(iit->first)->getStateKey(),   //to_stack1 (q')
                        wgt);                   //weight      
      }

      //Call Transitions 
      for( callIterator cit = trans->beginCall(); cit != trans->endCall(); cit++ )
      {
        for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
          if( cit->first == rit->second )
          {

            //(q_c,sigma,q_e) in delta_c and (q_x,q_c,*,q_r) in delta_r goes to
            // <p,q_e> -w1-> <p_q_e,epsilon> in delta_0
            // and <p_q_e,q_c> -w2-> <p,q_c> in delta_1
            // where p_q_e = (q_e,q_r)
            // w1 depends is one, and w2 depends on sigma

            wgt = wg.getOne();

            //Key cstate = getKey((*cit)->third->getStateKey(),(*rit)->fourth->getStateKey());  //p_q_e
            Key cstate = getControlLocation(cit->third,rit->fourth);

            result.add_rule(program,                //from_state (p)
                            getState(cit->third)->getStateKey(),   //from_stack (q_e)
                            cstate,                 //to_state (p_q_e == (q_e,q_r))
                            wgt);                   //weight 

            if( cit->second.isWild() )
              wgt = wg.getWildWeight(*getState(cit->first),*getState(cit->third));
            else
              wgt = wg.getWeight(*getState(cit->first), 
                                  cit->second,
                                  WeightGen<St,Sym>::EXIT_TO_RET,  
                                  *getState(cit->third));  

            result.add_rule(cstate,                   //from_state (p_q_x == (q_x,q_c))
                            getState(cit->first)->getStateKey(),    //from_stack (q_c)
                            program,                  //to_state (p)
                            getState(cit->first)->getStateKey(),    //to_stack (q_c)
                            wgt);                     //weight    
          }  
      }

      //Return Transitions
      for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
      {
        //(q_x,q_c,*,q_r) in delta_r goes to
        // <p,q_r> -w-> <p,q_x q_c> in delta_2 
        // and the weight w depends on sigma

        if( rit->third.isWild() )
          wgt = wg.getWildWeight(*getState(rit->first),*getState(rit->fourth));
        else
          wgt = wg.getWeight(*getState(rit->first),
                              rit->third,
                              WeightGen<St,Sym>::CALL_TO_ENTRY,
                              *getState(rit->fourth));

        result.add_rule(program,                //from_state (p)
                        getState(rit->fourth)->getStateKey(),   //from_stack (q_r)
                        program,                //to_state (p)
                        getState(rit->first)->getStateKey(),   //to_stack1 (q_x)
                        getState(rit->second)->getStateKey(),  //to_stack2 (q_c)
                        wgt);                   //weight      
      }

      return result;
    }

    /**
     *
     * @brief tests whether the language accepted by this NWA is empty
     *
     * @return true if the language accepted by this NWA is empty
     *
     */
    /* broken
    //TODO: determinize prior to checking paths?
    template<typename St,typename StName,typename Sym >
    bool NWA<St,StName,Sym>::isEmpty( )
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
    }*/

    /**
     *
     * @brief tests whether the NWA has a path from the current state to a final state
     *        given the call stack and the set of states already visited
     *
     * @param - currState: the state from which to start searching for a path
     * @param - calls: the stack of calls that have been made thus far  
     * @param - visited: the states which have already been visited
     * @return true if there is a path through the NWA from the current state to a 
     *          final state, false otherwise
     *
     */
    /* broken
    template<typename St,typename StName,typename Sym > 
    bool NWA<St,StName,Sym>::hasPath( St currState, std::stack< St > calls, std::set< St > visited )
    { //FIX
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
    }*/

    /**
     *
     * @brief tests whether the given nested word is a member of the language accepted 
     *        by this NWA
     *
     * @param - word: the nested word to test
     * @return true if the given nested word is a member of the language accepted by 
     *          this NWA
     *
     */
    /* broken
    template<typename St,typename StName,typename Sym >
    bool NWA<St,StName,Sym>::isMember( nws::NWS word )
    { //FIX
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
    }*/

    /** 
     *
     * @brief tests whether the nested word suffix is accepted by this nested word 
     *        automaton starting at the state with the given name
     *
     * @param - currState: the state in the nested word automaton at which to start the 
     *                      simulation
     * @param - configs: the nested word suffix to match  //Q: is this right?
     * @return true if the simulation ends in an accepting state
     *
     */
    /* broken
    template<typename St,typename StName,typename Sym > 
    bool NWA<St,StName,Sym>::matchWord( St currState, std::deque<WordRecConfig<St>> configs )
    { //FIX
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
    }*/

    //Utilities	

    /**
     *
     * @brief print the NWA
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template <typename St,typename StName,typename Sym>
    std::ostream & NWA<St,StName,Sym>::print( std::ostream & o ) const
    {
      states.print(o);

      //if( absentAcceptance )
      //  o << "Stuck state accepted." << "\n";   //TODO: move this to stateset

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

      //Print call transitions.
      o << "Delta_c: \n" << "{ \n";
      callIterator cit = beginCallTrans();
      callIterator citEND = endCallTrans();
      for( bool first=true; cit != citEND; cit++ )
      {
        if( !first )
          o << ", \n";
        o << "(";
        getState(cit->first)->print(o);
        o << ", ";
        cit->second.print(o);
        o << ", "; 
        getState(cit->third)->print(o);
        o << ")";
        first=false;
      }
      o << " \n}\n";
      //Print internal transitions.
      o << "Delta_i:\n" << "{\n";
      internalIterator iit = beginInternalTrans();
      internalIterator iitEND = endInternalTrans();
      for( bool first=true; iit != iitEND; iit++ )
      {
        //assert(*iit);
        if( !first )
          o << ", \n";
        o << "(";
        getState(iit->first)->print(o);
        o << ", ";
        iit->second.print(o);
        o << ", ";
        getState(iit->third)->print(o);
        o << ")";
        first = false;
      }
      o << " \n}\n";
      //Print return transitions.
      o << "Delta_r:\n" << "{\n";
      returnIterator rit = beginReturnTrans();
      returnIterator ritEND = endReturnTrans();
      for( bool first=true; rit != ritEND; rit++ )
      {
        if( !first )
          o << ",\n";
        o << "(";
        getState(rit->first)->print(o);
        o << ", ";
        getState(rit->second)->print(o);
        o << ", "; 
        rit->third.print(o);
        o << ", ";
        getState(rit->fourth)->print(o);
        o << ")";
        first = false;
      }
      o << "\n}\n";
      
      return o;
    }

    /**
     *
     * @brief print the NWA in dot format
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template<typename St,typename StName,typename Sym >
    std::ostream & NWA<St,StName,Sym>::print_dot( std::ostream & out, std::string title) const
    {

      out << "digraph \"NWA\" { \n";
      out << "    label = \"" << title <<"\" ;\n";
      out << "    subgraph cluster_key {\n";
      out << "        label = \"Key\";\n";
      out << "        style=filled;\n";
      out << "        color=white;\n";
      out << "        dummy [shape=box];";
      out << "        call -> entry [color=green];\n";
      out << "        call -> dummy [color=blue];\n";
      out << "        exit -> dummy [color=red];\n";
      out << "        dummy -> return [style=dashed];\n";
      out << "    }\n";
      // initial state
      St *st = getState( getInitialState() );
      st->print(out << "\"") <<"\" [ style=bold ]";
      // final states
      std::set<StName> finals = getFinalStates();
      for(std::set<StName>::const_iterator it = finals.begin(); it!=finals.end(); it++) {
        St *st = getState( *it );
        st->print(out << "\"") <<"\" [ peripheries=2 ]";
      }
      //Print call transitions.
      out << "//Delta_c: \n";
      callIterator cit = beginCallTrans();
      callIterator citEND = endCallTrans();
      for( bool first=true; cit != citEND; cit++ )
      {
        getState(cit->first)->print(out << "\"") << "\"";
        out << "->";
        getState(cit->third)->print(out << "\"") << "\"";
        out << "[";
        out << " label=\"";
        (cit->second).print(out);
        out << "\"";
        out << " color=green";
        out << "];\n";
        
      }
      out << " \n";
      //Print internal transitions.
      out << "// Delta_i:\n" ;
      internalIterator iit = beginInternalTrans();
      internalIterator iitEND = endInternalTrans();
      for( bool first=true; iit != iitEND; iit++ )
      {
        getState(iit->first)->print(out << "\"") << "\"";
        out << "->";
        getState(iit->third)->print(out << "\"") << "\"";
        out << "[";
        out << " label=\"";
        (iit->second).print(out);
        out << "\"";
        out << "];\n";
      }
      out << " \n";
      //Print return transitions.
      out << "// Delta_r:\n";
      returnIterator rit = beginReturnTrans();
      returnIterator ritEND = endReturnTrans();
      for( bool first=true; rit != ritEND; rit++ )
      {
        //dummy
        out << "\"" << std::dec << rit->second << std::dec << ", " << rit->first << "\"";
        out <<"[ shape=box ];\n";

        // exit to dummy
        getState(rit->first)->print(out << "\"") << "\"";
        out << "->";
        out << "\"" << rit->second <<", " << rit->first << "\"";
        out << "[";
        out << " label=\"";
        rit->third.print(out);
        out << "\"";
        out << " color=red";
        out << "];\n";

        // call to dummy
        getState(rit->second)->print(out << "\"") << "\"";
        out << "->";
        out << "\"" << rit->second <<", " << rit->first << "\"";
        out << "[";
        out << " color=blue";
        out << "];\n";

        //dummy to ret
        out << "\"" << rit->second <<", " << rit->first << "\"";
        out << "->";
        getState(rit->fourth)->print(out << "\"") << "\"";
        out << "[ style=dotted ];\n";

      }
      out << "\n";

      out << "}\n";
      out.flush();
      return out;
    }

    /**
     *
     * @brief tests whether this NWA is equivalent to the NWA 'other'
     *
     * @param - other: the NWA to compare this NWA to
     * @return true if this NWA is equivalent to the NWA 'other'
     *
     */
    template <typename St,typename StName,typename Sym> 
    bool NWA<St,StName,Sym>::operator==( const NWA<St,StName,Sym> & other ) const
    {
      return ( (states == other.states) &&
        //(absentAcceptance == other.absentAcceptance) &&
        (symbols == other.symbols) &&   //TODO: deeper check here?
        (trans == other.trans) );
    }
#else //ifdef KEY
    template<typename Client = ClientInfo>
    class NWA : public Printable, public Countable
    {
      public:
        typedef ref_ptr<NWA> NWARefPtr;

        typedef Key St;
        typedef Key Sym;
        typedef StateSet<Client> States;
        typedef typename States::const_iterator stateIterator;
        typedef typename States::ClientInfoRefPtr ClientInfoRefPtr;
        typedef SymbolSet Symbols;
        typedef typename Symbols::const_iterator symbolIterator;

        typedef TransSet<St,Sym> Trans;
        
        typedef typename Trans::Call Call;       
        typedef typename Trans::Internal Internal;   
        typedef typename Trans::Return Return;          
      
        typedef typename Trans::Calls Calls;
        typedef typename Trans::Internals Internals;
        typedef typename Trans::Returns Returns;
      
        typedef typename Trans::callIterator callIterator;
        typedef typename Trans::internalIterator internalIterator;
        typedef typename Trans::returnIterator returnIterator;        

        
        typedef std::set<St> StateSet;
        typedef std::pair<St,St> StatePair;
        typedef std::set<StatePair> StatePairSet;
        typedef std::map<StatePairSet,St> StateMap; 

      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      NWA( );
      NWA( const NWA & other );
      NWA & operator=( const NWA & other );

      void clear( );
      ~NWA( );

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
      ClientInfoRefPtr getClientInfo( St state );

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
      void setClientInfo( St state, const ClientInfoRefPtr c );  
     
      /**
       * 
       * @brief obtains all the states that are predecessors of the given state
       *
       * This method returns all the states that are predecessors of the given 
       * state.
       *
       * @param - state: the state of the state whose predecessors to look up
       * @return the set of all states that are predecessors of the given state
       *
       */    
      const std::set<St> & getPredecessorNames( St state ) const;
      
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
      void duplicateStateOutgoing( St orig, St dup );

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
      void duplicateState( St orig, St dup );

      //All States

      /**
       *   
       * @brief provides access to all states in the NWA
       *
       * This method provides access to all states in pointer form.
       *
       * @return a set of all states in pointer form
       *
       */
      const std::set<St> & getStates( ) const;
      /**
       *   
       * @brief provides access to all states in the NWA
       *
       * This method provides access to all states in pointer form.
       * Note: This method simply calls getStates( ).  It is here to make the transition
       *        from using PDS to using NWA easier.
       *
       * @return a set of all states in pointer form
       *
       */
      const std::set<St> & get_states( ) const;

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
      bool isState( St state ) const; 
      /**
       * 
       * @brief test if a given state is a state of this NWA
       *
       * This method tests whether the given state is in the state set of this NWA.
       * Note: This method simply calls isState( state ).  It is here to make the transition
       *        from using PDS to using NWA easier.
       *
       * @param - state: the state to check
       * @return true if the given state is a state of this NWA
       *
       */
      bool is_nwa_state( St state ) const;

     /**
       *  
       * @brief add the given state to this NWA
       *
       * This method adds the given state to the state set for this NWA.  If the state 
       * already exists in the NWA, false is returned.  Otherwise, true is returned.
       *
       * @param - state: the state to add
       * @return false if the state already exists in the NWA, true otherwise
       *
       */
      bool addState( St state );

      /**
       *
       * @brief returns the number of states associated with this NWA
       *
       * This method returns the number of states associated with this NWA. 
       *
       * @return the number of states associated with this NWA
       *
       */
      size_t sizeStates( ) const;
      /**
       *
       * @brief returns the number of states associated with this NWA
       *
       * This method returns the number of states associated with this NWA.
       * Note: This method simply calls sizeStates( ).  It is here to make the transition
       *        from using PDS to using NWA easier.
       *
       * @return the number of states associated with this NWA
       *
       */
      int num_nwa_states( ) const;

      /**
       *  
       * @brief remove the given state from this NWA
       *
       * This method checks for the given state in the set of initial states, the set of 
       * final states, and the set of states in this NWA.  It then removes the state from 
       * any set that contained it.  Any transitions to or from the state to be removed 
       * are also removed.
       *
       * @param - state: the state to remove
       * @return false if the state does not exist in the NWA, true otherwise
       *
       */
      bool removeState( St state );

      /**
       *
       * @brief remove all states from the NWA
       *
       * This method removes all states and transitions from the NWA.
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
      const std::set<St> & getInitialStates( ) const; 
      /**
       *  TODO: do we still need the single initial state restriction?
       * @brief obtain the initial state
       *
       * This method provides access to the initial state of this NWA.  
       * Note: This hinges on the assertion that there is a unique initial state.
       *
       * @return the inital state associated with the NWA
       *
       */
      St getInitialState( ) const;

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
      bool isInitialState( St state ) const;

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
      bool addInitialState( St state );

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
      bool removeInitialState( St state );

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
      const std::set<St> & getFinalStates( ) const;

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
      bool isFinalState( St state ) const;

      /**
       *
       * @brief make the given state a final state
       *
       * This method checks for the given state in the state set of the NWA.  If the state
       * does not exist, it is added to the state set of the NWA.  The given state is then
       * added to the set of final states for the NWA.  If the state already exists in the
       * final state set of the NWA, false is returned.  Otherwise, true is returned.
       *
       * @param - state: the state to add to final state set
       * @return false if the state already exists in the final state set of the NWA
       *
       */
      bool addFinalState( St state );

      /**
       * 
       * @brief returns the number of final states associated with this NWA
       *
       * This method returns the number of final states associated with this NWA. 
       * Note: This count does include the stuck state if it is a final state. 
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
      bool removeFinalState( St state );

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
      const std::set<Sym> & getSymbols( ) const;

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
      bool isSymbol( Sym sym ) const;

      /**
       *
       * @brief add the given symbol to the NWA
       *
       * This method adds the given symbol to the symbol set associated with the NWA. If 
       * the symbol is already associated with the NWA, false is returned. Otherwise, 
       * true is returned.
       *
       * @param - sym: the symbol to add
       * @return false if the symbol is already associated with the NWA
       *
       */
      bool addSymbol( Sym sym );

      /**
       *
       * @brief returns the number of symbols associated with this NWA
       *
       * This method returns the number of symbols associated with this NWA.  
       * Note: The epsilon symbol is included in this count.
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
       *
       * @param - sym: the symbol to remove
       * @return false if the symbols is not associated with the NWA
       *
       */
      bool removeSymbol( Sym sym );

      /**
       *
       * @brief remove all symbols associated with the NWA
       *
       * This method removes all symbols associated with the NWA.  It also removes all 
       * transitions associated with the NWA as there can be no transitions without at 
       * least one symbol.
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
      bool getSymbol( St from, St to, Sym & sym );

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
      bool findTrans( St from, Sym sym, St to ) const;

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
       * @brief returns the number of transitions associated with this NWA
       *
       * This method returns the number of transitions associated with this NWA.  
       * Note: This method simply calls sizeTrans( ).  It is here to make the transition
       *        from using PDS to using NWA easier.
       *
       * @return the number of transitions associated with this NWA
       *
       */
      int count_rules( ) const;

      /**
       *
       * @brief remove all transitions from the NWA
       *
       * This method removes all transitions from the NWA.  It does not remove any states
       * from the NWA.
       *
       */
      void clearTrans( );

      //Call Transitions

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
      const std::set<std::pair<Sym,St>> getEntries( St callSite ) const;

      /**
       *
       * @brief add a call transition to the NWA
       *
       * This method creates a call transition with the given edge and label information 
       * and adds it to the transition set for the NWA.  If the call transition already 
       * exists in the NWA, false is returned. Otherwise, true is returned.
       * Note: 'sym' cannot be Epsilon
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the call transition already exists in the NWA
       *
       */
      bool addCallTrans( St from, Sym sym, St to );

      /**
       *
       * @brief add a call transition to the NWA
       *
       * This method adds the given call transition to the transition set  for the NWA.
       * If the call transition already exists in the NWA, false is returned. Otherwise, 
       * true is returned.
       * Note: the symbol cannot be Epsilon
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
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the call transition does not exist in the NWA
       *
       */
      bool removeCallTrans( St from, Sym sym, St to );

      /**
       *
       * @brief remove a call transition from the NWA
       *
       * This method checks for the call transition in the transition set. If the 
       * transition is found, it is removed from the transition set and true is 
       * returned.  Otherwise, false is returned.
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
       * @brief returns the symbol/target pairs associated with the given source
       *
       * This method provides access to the symbol/target pairs associated with the given
       * source.
       *
       * @param - source: the source whose symbol/target pairs to obtain
       * @return the set of symbol/target pairs associated with the given source
       *
       */
      const std::set<std::pair<Sym,St>> getTargets( St source ) const;

      /**
       *
       * @brief add an internal transition to the NWA
       *
       * This method creates an internal transition with the given edge and label 
       * information and adds it to the transition set for the NWA.  If the internal 
       * transition already exists in the NWA, false is returned. Otherwise, true is 
       * returned.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the internal transition already exists in the NWA
       *
       */
      bool addInternalTrans( St from, Sym sym, St to );

      /**
       *
       * @brief add an internal transition to the NWA
       *
       * This method adds the given internal transition to the transition set for the NWA.
       * If the internal transition already exists in the NWA, false is returned. Otherwise, 
       * true is returned.
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
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the internal transition does not exist in the NWA
       *
       */
      bool removeInternalTrans( St from, Sym sym, St to );

      /**
       *
       * @brief remove an internal transition from the NWA
       *
       * This method checks for the internal transition in the transition set. If the 
       * transition is found, it is removed from the transition set and true is returned.
       * Otherwise, false is returned.
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
       * @brief returns the return sites associated with the given call site
       *
       * This method provides access to the return sites associated with the given
       * call site.
       *
       * @param - callSite: the call site whose return sites to obtain
       * @return the set of return sites associated with the given call site
       *
       */
      const std::set<St> getReturnSites( St callSite ) const;

      /**
       * 
       * @brief returns the symbol/return site pairs associated with the given exit point 
       *        and call site
       *
       * This method provides access to the symbol/return site pairs associated with the 
       * given exit point and call site.
       *
       * @param - exit: the exit point whose symbol/return site pairs to obtain
       * @param - callSite: the call site whose symbol/return site pairs to obtain
       * @return the set of symbol/return site pairs associated with the given exit point 
       *          and call site
       *
       */
      const std::set<std::pair<Sym,St>> getReturns( St exit, St callSite ) const;

      /**
       *
       * @brief add a return transition to the NWA
       *
       * This method creates a return transition with the given edge and label information 
       * and adds it to the transition set for the NWA.  If the return transition already 
       * exists in the NWA, false is returned. Otherwise, true is returned.
       * Note: 'sym' cannot be Epsilon
       *
       * @param - from: the state the edge departs from
       * @param - pred: the state from which the call was initiated  
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition already exists in the NWA
       *
       */
      bool addReturnTrans( St from, St pred, Sym sym, St to );

      /**
       *
       * @brief add a return transition to the NWA
       *
       * This method adds the given return transition to the transition set for the NWA. 
       * If the return transition already exists in the NWA, false is returned. Otherwise,
       * true is returned.
       * Note: the symbol cannot be Epsilon
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
       *
       * @param - from: of the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturnTrans( St from, Sym sym, St to );

      /**
       *
       * @brief remove a return transition from the NWA
       *
       * This method checks for the return transition with the given edge and label 
       * information in the transition set. If the transition is found, it is removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       * 
       * @param - from: the state the edge departs from
       * @param - pred: the state from which the call was initiated  	   
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturnTrans( St from, St pred, Sym sym, St to );

      /**
       *
       * @brief remove a return transition from the NWA
       *
       * This method checks for the return transition in the transition set. If the 
       * transition is found, it is removed from the transition set and true is returned.
       * Otherwise, false is returned.
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

      /**
       *
       * @brief constructs the NWA resulting from the union of the given NWAs 
       *
       * This method constructs the union of the given NWAs by creating a new initial 
       * state with epsilon transitions to all initial states of the two NWAs.
       * Note: The resulting NWA is NOT guaranteed to be deterministic.
       *
       * @param - first: the NWA to union with 'second'
       * @param - second: the NWA to union with 'first'
       *
       */
      void unionNWA( NWARefPtr first, NWARefPtr second );  

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
       *	
       */
      void intersect( NWARefPtr first, NWARefPtr second );

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
       *
       */
      void concat( NWARefPtr first, NWARefPtr second );  

      /**
       * @brief constructs the NWA which is the reverse of the given NWA
       *
       * This method constructs the NWA which is the reverse of the given NWA. It reverses 
       * internal transitions and switches call and return transitions.
       * Note: the resulting NWA is NOT guaranteed to be deterministic.
       *
       * @param - first: the NWA to reverse
       * 
       */
      void reverse( NWARefPtr first );  

      /**
       *
       * @brief constructs the NWA resulting from performing Kleene-* on the given NWA
       *
       * This method constructs the Kleene-* of the given NWA by adding epsilon transitions 
       * from all final states of the NWA to all initial states of the NWA.  
       * Note: The resulting NWA is NOT guaranteed to be deterministic.
       *
       * @param - first: the NWA to perform the Kleene-* of
       *
       */
      void star( NWARefPtr first ); 

      /**
       * 
       * @brief constructs the NWA that is the complement of the given NWA
       *
       * This method constructs the complement of the given NWA by determinizing it and
       * then replacing the set of final states with the set of states that are not final  
       *
       * @param - first: the NWA to perform the complement of
       *
       */
      void complement( NWARefPtr first );

      /**
       *
       * @brief constructs a deterministic NWA that is equivalent to the given NWA.
       *
       * This method constructs a deterministic NWA that is equivalent to the given NWA.
       *
       * @param - nondet: the NWA to determinize
       *
       */
      void determinize( NWARefPtr nondet ); 

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
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the given states
       * and returns the resulting client information.  
       * Note: This method should only be used to intersect client information from the 
       * initial states of NWAs (all other client information intersections should use the 
       * specialized methods below). 
       *
       * @param - first: the NWA in which to look up the client information for 'name1'
       * @param - name1: the first state whose client information to intersect
       * @param - second: the NWA in which to look up the client information for 'name2'
       * @param - name2: the second state whose client information to intersect
       * @param - resSt: the state which will receive the computed client information
       * @param - result: the client information resulting from the intersection of
       *                  'name1' and 'name2'
       *
       */
      virtual void intersectClientInfo( NWARefPtr first, St name1, 
                                        NWARefPtr second, St name2, 
                                        St resSt, ClientInfoRefPtr result );

      /**
       * 
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the states 'entry1'
       * and 'entry2' given that the transition that is being created is a call transition 
       * with the given symbol using the information in the given states and returns the 
       * resulting client information.
       * Note: This method should only be used to intersect client information for states 
       * immediately following a call transition.
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
       * @param - result: the client information resulting from the intersection of 
       *                  'entry1' and 'entry2'
       *
       */
      virtual void intersectClientInfoCall( NWARefPtr first, St call1, St entry1, 
                                            NWARefPtr second, St call2, St entry2, 
                                            Sym resSym, St resSt, 
                                            ClientInfoRefPtr result );  

      /**
       * 
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the states 'tgt1' and
       * 'tgt2' given that the transition that is being created is an internal transition 
       * with the given symbol using the information in the given states and returns the 
       * resulting client information.
       * Note: This method should only be used to intersect client information for states 
       * immediately following an internal transition.
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
       * @param - result: the client information resulting from the intersection of
       *                'tgt1' and 'tgt2'
       *
       */
      virtual void intersectClientInfoInternal( NWARefPtr first, St src1, St tgt1, 
                                                NWARefPtr second, St src2, St tgt2, 
                                                Sym resSym, St resSt, 
                                                ClientInfoRefPtr result );  

      /**
       * 
       * @brief intersects client information 
       *
       * This method intersects the client information associated with the states 'ret1' and
       * 'ret2' given that the transition that is being created is a return transition with
       * the given symbol using the information in the given states and returns the 
       * resulting client information.
       * Note: This method should only be used to intersect client information for states 
       * immediately following a return transition.
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
       * @param - result: the client information resulting from the intersection of
       *                'ret1' and 'ret2'
       *
       */
      virtual void intersectClientInfoReturn( NWARefPtr first, St exit1, St call1, St ret1,
                                              NWARefPtr second, St exit2, St call2, St ret2,
                                              Sym resSym, St resSt,
                                              ClientInfoRefPtr result );

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
       *
       */
      virtual bool nodeIntersect( NWARefPtr first, St state1, NWARefPtr second, St state2, St & resSt );

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
      virtual bool edgeIntersect( NWARefPtr first, Sym sym1, NWARefPtr second, Sym sym2, Sym & resSym );

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
       * 
       */
      wpds::WPDS plusWPDS( const wpds::WPDS & base ); 

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
       * @brief returns the default program control location for PDSs
       *
       * This method provides access to the default program control location for PDSs.
       * 
       * @return the default program control location for PDSs
       *
       */  
      static wali::Key getProgramControlLocation( )
      {   
        return getKey("program");
      };

      /**
       *  
       * @brief returns the program control location corresponding to the given states
       *
       * This method provides access to the program control location corresponding to
       * the given exit point/call site pair.
       *
       * @param - exit: the exit point corresponding to this control location
       * @param - callSite: the call site corresponding to this control location
       * @return the program control location corresponding to the given states
       *
       */
      static wali::Key getControlLocation( Key exit, Key callSite )
      {
        return getKey(exit,callSite);
      };

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
       * @brief tests whether the given nested word is a member of the language accepted 
       *        by this NWA
       *
       * This method tests whether the given nested word is a member of the language 
       * accepted by this NWA.
       *
       * @param - word: the nested word to test
       * @return true if the given nested word is a member of the language accepted by 
       *          this NWA
       *
       */
      bool isMember( nws::NWS word ); 

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
      virtual wfa::WFA prestar( wfa::WFA & input, WeightGen & wg );

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
      virtual void prestar( wfa::WFA & input, wfa::WFA & output, WeightGen & wg );

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
      virtual wfa::WFA poststar( wfa::WFA & input, WeightGen & wg );

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
      virtual void poststar( wfa::WFA & input, wfa::WFA & output, WeightGen & wg );

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
      virtual std::ostream & print_dot( std::ostream & o, std::string title ) const;

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

      protected:
      
      /**
       *
       * @brief add all the states in the given StateSet to the NWA
       *
       * This method adds all of the given states to the state set for the NWA.
       *
       * @param - addStateSet: the StateSet that contains the states to add
       *
       */
      void addAllStates( States addStateSet );  

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
      void addAllInitialStates( States addStateSet ); 

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
      void addAllFinalStates( States addStateSet ); 

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
      void addAllSymbols( Symbols addSymbolSet );    

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
      callIterator beginCallTrans( ) const;  

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
      callIterator endCallTrans( ) const;  

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
      internalIterator beginInternalTrans( ) const;  

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
      internalIterator endInternalTrans( ) const; 

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
      returnIterator beginReturnTrans( ) const;  

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
      returnIterator endReturnTrans( ) const;  

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
      void epsilonClosure( std::set<St> * newPairs, St sp );

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
     void epsilonClosure( std::set<StatePair> * newPairs, StatePair sp, NWARefPtr first, NWARefPtr second );

      /**
       *
       * @brief constructs the transition table and state map for the deterministic NWA 
       *        that is equivalent to this NWA.
       *
       * This method constructs the transition table and state map for the deterministic 
       * NWA that is equivalent to this NWA.
       *
       * @param - nondet: the NWA to determinize
       * @param - stMap: the map of sets of 'nondet' states to states
       * @param - currSt: the current state under consideration
       * @param - callPred: the stack of open calls at 'currSt'
       *
       */
      void det( NWARefPtr nondet, StateMap & stMap, StatePairSet currSt, 
                std::stack<StatePairSet> callPred );  

      /**
       *
       * @brief tests whether the NWA has a path from the current state to a final state
       *        given the call stack and the set of states already visited
       *
       * This method tests whether the NWA contains a path from the current state to any 
       * final state given the call stack and the set of states already visited.
       * 
       * @param - currState: the state from which to start searching for a path
       * @param - calls: the stack of calls that have been made thus far  
       * @param - visited: the states which have already been visited
       * @return true if there is a path through the NWA from the current state to a 
       *          final state, false otherwise
       *
       */
      bool NWA::hasPath( St currState, std::stack< St > calls, std::set< St > visited );  

      /** 
       *
       * @brief tests whether the nested word suffix is accepted by this nested word 
       *        automaton starting at the state with the given name
       *
       * This method tests whether the nested word suffix is accepted by this nested word
       * automaton by simulating the word suffix on the automaton starting at the state 
       * with the given name.  
       *
       * @param - currState: the state in the nested word automaton at which to start the 
       *                      simulation
       * @param - configs: the nested word suffix to match  //Q: is this right?
       * @return true if the simulation ends in an accepting state
       *
       */
      //bool matchWord( St currState, std::deque<WordRecConfig<St>> configs );  

      //
      // Variables
      //
      
      protected:
      
        States states;         
        Symbols symbols;        
        Trans * trans;
    };

    //
    // Methods
    //

    //Constructors and Destructor
    template <typename Client>
    NWA<Client>::NWA( )
    {
      states = States();
      symbols = Symbols();
      trans = new Trans();  //Q: why is this a pointer and not the others?
    }
    template <typename Client>
    NWA<Client>::NWA( const NWA & other )
    {
      //Copy data over from 'other'
      states = other.states;
      symbols = other.symbols;
      trans = other.trans;
    }
    template <typename Client>
    NWA<Client> & NWA<Client>::operator=( const NWA & other )
    {
      if( this == &other )
        return *this;

      //Erase any pre-existing data.
      clear();

      //Copy data over from 'other'
      states = other.states;
      symbols = other.symbols;
      trans = other.trans;

      return *this;
    }
    
    /**
     *
     * @brief erases all data from the NWA
     *
     * This method restore's the NWA to its default state:
     * the stuck state with no symbols or transitions.
     *
     */
    template <typename Client>
    void NWA<Client>::clear( )
    {
      clearStates();
      clearSymbols();
      clearTrans();
    }

    template <typename Client>
    NWA<Client>::~NWA( )
    {
      clear();

      trans->~Trans();
    }

    //State Accessors

    /**
     * 
     * @brief access the client information associated with the given state
     *
     * @param - state: the state whose client information to retrieve
     * @return the client information associated with the given state
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::ClientInfoRefPtr NWA<Client>::getClientInfo( St state ) 
    {
      return states.getClientInfo(state);
    }

    /**
     * 
     * @brief set the client information associated with the given state
     *
     * @param - state: the  state whose client information to set
     * @param - c: the desired client information for the given state
     *
     */
    template <typename Client>
    inline
    void NWA<Client>::setClientInfo( St state, const ClientInfoRefPtr c )
    {
      states.setClientInfo(state,c);
    }
     
    /**
     * 
     * @brief obtains all the states that are predecessors of the given state
     *
     * @param - state: the state of the state whose predecessors to look up
     * @return the set of all states that are predecessors of the given state
     *
     */ 
    template <typename Client>
    const std::set<typename NWA<Client>::St> & NWA<Client>::getPredecessorNames( St state ) const
    {
      std::set<St> * preds = new std::set<St>();  //TODO: this is not a good idea, it will probably cause a memory leak!
      
      std::set<Call> calls = trans->getTransEntry(state);
      for( callIterator cit = calls.begin(); cit != calls.end(); cit++ )
        preds->insert(Trans::getCallSite(*cit));

      std::set<Internal> internals = trans->getTransTo(state);
      for( internalIterator iit = internals.begin(); iit != internals.end(); iit++ )
        preds->insert(Trans::getSource(*iit));

      std::set<Return> returns = trans->getTransRet(state);
      for( returnIterator rit = returns.begin(); rit != returns.end(); rit++ )
        preds->insert(Trans::getExit(*rit));

      return *preds;
    }
      
    /**
     *   
     * @brief duplicates the original state, but only duplicates outgoing transitions
     *
     * @param - orig: the name of the original state, i.e. the state to duplicate
     * @param - dup: the name of the duplicate state
     *
     */
    template <typename Client>
    void NWA<Client>::duplicateStateOutgoing( St orig, St dup )
    {
      states.addState(dup);
      states.dupState(orig,dup);  //Duplicate state characteristics(initial/final).

      trans->dupTransOutgoing(orig,dup);  //Duplicate outgoing transitions.
    }

    /**
     * 
     * @brief duplicates the original state
     *
     * @param - orig: the name of the original state, i.e. the state to duplicate
     * @param - dup: the name of the duplicate state
     *
     */
    template <typename Client>
    void NWA<Client>::duplicateState( St orig, St dup )
    {
      states.addState(dup);
      states.dupState(orig,dup);  //Duplicate state characteristics(initial/final).

      trans->dupTrans(orig,dup);  //Duplicate all transitions.
    }

    //All States

    /**
     *   
     * @brief provides access to all states in the NWA
     *
     * @return a set of all states 
     *
     */
    template <typename Client>
    inline
    const std::set<typename NWA<Client>::St> & NWA<Client>::getStates( ) const
    {
      return states.getStates();
    }
    /**
     *   
     * @brief provides access to all states in the NWA
     *
     * Note: This method simply calls getStates( ).  It is here to make the transition
     *        from using PDS to using NWA easier.
     *
     * @return a set of all states in pointer form
     *
     */
    template <typename Client>
    inline
    const std::set<typename NWA<Client>::St> & NWA<Client>::get_states( ) const
    {
      return getStates();
    }

    /**
     * 
     * @brief test if a given state is a state of this NWA
     *
     * @param - state: the state to check
     * @return true if the given state is a state of this NWA
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::isState( St state ) const
    {
      return states.isState(state);
    }
    /**
     * 
     * @brief test if a given state is a state of this NWA
     *
     * Note: This method simply calls isState( state ).  It is here to make the transition
     *        from using PDS to using NWA easier.
     *
     * @param - state: the state to check
     * @return true if the given state is a state of this NWA
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::is_nwa_state( St state ) const
    {
      return isState(state);
    }

    /**
     *  
     * @brief add the given state to this NWA
     *
     * @param - state: the state to add
     * @return false if the state already exists in the NWA, true otherwise
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::addState( St state )
    {
      return states.addState(state);
    }

    /**
     *
     * @brief returns the number of states associated with this NWA
     *
     * @return the number of states associated with this NWA
     *
     */
    template <typename Client>
    inline
    size_t NWA<Client>::sizeStates( ) const
    {
      return states.sizeStates();
    }
    /**
     *
     * @brief returns the number of states associated with this NWA
     *
     * Note: This method simply calls sizeStates( ).  It is here to make the transition
     *        from using PDS to using NWA easier.
     *
     * @return the number of states associated with this NWA
     *
     */
    template <typename Client>
    inline
    int NWA<Client>::num_nwa_states( ) const
    {
      return sizeStates();      
    }

    /**
     *  
     * @brief remove the given state from this NWA
     *
     * @param - state: the state to remove
     * @return false if the state does not exist in the NWA, true otherwise
     *
     */
    template <typename Client>
    bool NWA<Client>::removeState( St state )
    {
      bool removed = states.removeState(state);
      //Remove transitions associated with the state that was removed.
      if( removed )
      {
        trans->removeTransWith(state);
      }

      return removed;
    }

    /**
     *
     * @brief remove all states from the NWA
     *
     */
    template <typename Client>
    void NWA<Client>::clearStates( )
    {
      states.clearStates();

      //Since all states are being removed, all transitions are removed as well.
      clearTrans();
    }

    //Initial States

    /**
     *  
     * @brief obtain the states in the initial state set
     *
     * @return set of inital states associated with the NWA
     *
     */
    template <typename Client>
    inline
    const std::set<typename NWA<Client>::St> & NWA<Client>::getInitialStates( ) const
    {
      return states.getInitialStates();
    }
    /**
     *  Q: do we still need the single initial state restriction?
     * @brief obtain the initial state
     *
     * @return the inital state associated with the NWA
     *
     */
    template <typename Client>
    typename NWA<Client>::St NWA<Client>::getInitialState( ) const
    {
      std::set<St> sts = states.getInitialStates();
      assert(sts.size()==1);  //Assuming unique initial state.
      return *(sts.begin());
    }

    /**
     * 
     * @brief test if the given state is an initial state of this NWA
     *
     * @param - state: the state to check
     * @return true if the given state is an initial state, false otherwise
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::isInitialState( St state ) const
    {
      return states.isInitialState(state);
    }

    /**
     * 
     * @brief make the given state an initial state in this NWA
     *
     * @param - state: the state to add to initial state set
     * @return false if the state already exists in the initial state set of the NWA
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::addInitialState( St state )
    {
      return states.addInitialState(state);
    }

    /**
     *
     * @brief returns the number of initial states associated with this NWA
     *
     * @return the number of initial states associated with this NWA
     *
     */
    template <typename Client>
    inline
    size_t NWA<Client>::sizeInitialStates( ) const
    {
      return states.sizeInitialStates();
    }

    /**
     * 
     * @brief remove the given state from the initial state set of this NWA
     *
     * @param - state: the state to remove from the initial state set
     * @return false if the state does not exist in the initial state set of this NWA
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::removeInitialState( St state )
    {
      return states.removeInitialState(state);
    }

    /**
     *
     * @brief remove all states from the initial state set of the NWA
     *
     */
    template <typename Client>
    inline
    void NWA<Client>::clearInitialStates( )
    {
      states.clearInitialStates();
    }

    //Final States

    /**
     *
     * @brief obtain the final states in this NWA
     *
     * @return set of all final states associated with this NWA
     *
     */
    template <typename Client>
    inline
    const std::set<typename NWA<Client>::St> & NWA<Client>::getFinalStates( ) const
    {
      return states.getFinalStates();
    }

    /**
     *
     * @brief test if a state with the given name is a final state
     *
     * @param - state: the state to check
     * @return true if the given state is a final state
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::isFinalState( St state ) const
    {
      return states.isFinalState(state);
    }

    /**
     *
     * @brief make the given state a final state
     *
     * @param - state: the state to add to final state set
     * @return false if the state already exists in the final state set of the NWA
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::addFinalState( St state )
    {
      return states.addFinalState(state);
    }

    /**
     * 
     * @brief returns the number of final states associated with this NWA
     *
     * @return the number of final states associated with this NWA
     *
     */
    template <typename Client>
    inline
    size_t NWA<Client>::sizeFinalStates( ) const
    {
      return states.sizeFinalStates();
    }

    /**
     *
     * @brief remove the given state from the final state set of the NWA
     *
     * @param - state: the state to remove from the final state set
     * @return false if the state does not exist in the final state set of the NWA
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::removeFinalState( St state )
    {
      return states.removeFinalState(state);
    }

    /**
     *
     * @brief remove all states from the final state set of the NWA
     *
     */
    template <typename Client>
    inline
    void NWA<Client>::clearFinalStates( )
    {
      states.clearFinalStates();
    }

    //Symbol Accessors

    /**
     *
     * @brief obtain all symbols in this NWA
     *
     * @return set of all symbols associated with this NWA
     *
     */
    template <typename Client>
    inline
    const std::set<typename NWA<Client>::Sym> & NWA<Client>::getSymbols( ) const
    {
      return symbols.getSymbols();
    }

    /**
     *
     * @brief test if the given symbol is associated with the NWA
     *
     * @param - sym: the symbol to check
     * @return true if the given symbol is associated with the NWA
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::isSymbol( Sym sym ) const
    {
      return symbols.isSymbol(sym);
    }

    /**
     *
     * @brief add the given symbol to the NWA
     *
     * @param - sym: the symbol to add
     * @return false if the symbol is already associated with the NWA
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::addSymbol( Sym sym )
    {
      return symbols.addSymbol(sym);
    }

    /**
     *
     * @brief returns the number of symbols associated with this NWA
     *
     * Note: The epsilon symbol is included in this count.
     *
     * @return the number of symbols associated with this NWA
     *
     */
    template <typename Client>
    inline
    size_t NWA<Client>::sizeSymbols( ) const
    {
      return symbols.sizeSymbols();
    }

    /**
     *
     * @brief remove the given symbol from the NWA
     *
     * @param - sym: the symbol to remove
     * @return false if the symbols is not associated with the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::removeSymbol( Sym sym )
    {
      bool removed = symbols.removeSymbol(sym);
      if( removed )
      {
        trans->removeTransSym(sym);
      }
      return removed;
    }

    /**
     *
     * @brief remove all symbols associated with the NWA
     *
     */
    template <typename Client>
    inline
    void NWA<Client>::clearSymbols( )
    {
      symbols.clearSymbols();

      //Since all symbols are being removed, all transitions are removed as well.
      clearTrans();
    }

    //Transition Accessors

    /**
     * 
     * @brief finds a symbol which occurs in a transition with the given endpoints
     *
     * @param - from: the source of the desired transition
     * @param - to: the target of the desired transition
     * @param - sym: the location in which to place any symbol labeling such a transition
     * @return true if some such transition is found, false otherwise
     *
     */
    template <typename Client>
    inline
    bool NWA<Client>::getSymbol( St from, St to, Sym & sym )
    {
      return trans->getSymbol(from,to,sym);
    }

    /**
     * 
     * @brief tests whether there exists a transition whose source is 'from', 
     *        whose symbol is 'sym', and whose target is 'to'
     *
     * @param - from: the source of the transition
     * @param - sym: the symbol that labels the transition
     * @param - to: the target of the transition
     * @return true if such a transition exists, false otherwise
     * 
     */
    template <typename Client>
    inline
    bool NWA<Client>::findTrans( St from, Sym sym, St to) const
    {
      return trans->findTrans(from,sym,to);
    }

    /**
     *
     * @brief returns the number of transitions associated with this NWA
     *
     * @return the number of transitions associated with this NWA
     *
     */
    template <typename Client>
    inline
    size_t NWA<Client>::sizeTrans( ) const
    {
      return trans->size();
    }
    /**
     *
     * @brief returns the number of transitions associated with this NWA
     *
     * Note: This method simply calls sizeTrans( ).  It is here to make the transition
     *        from using PDS to using NWA easier.
     *
     * @return the number of transitions associated with this NWA
     *
     */
    template <typename Client>
    inline
    int NWA<Client>::count_rules( ) const
    {
      return sizeTrans();
    }

    /**
     *
     * @brief remove all transitions from the NWA
     *
     */
    template <typename Client>
    inline
    void NWA<Client>::clearTrans( )
    {
      trans->clear();
    }

    //Call Transitions

    /**
     * 
     * @brief returns the symbol/entry point pairs associated with the given call site
     *
     * @param - callSite: the call site whose symbol/entry point pairs to obtain
     * @return the set of symbol/entry point pairs associated with the given call site
     *
     */
    template <typename Client>
    const std::set<std::pair<typename NWA<Client>::Sym,typename NWA<Client>::St>> NWA<Client>::getEntries( St callSite ) const
    {
      const Calls ent = trans->getTransCall(callSite);
      std::set<std::pair<Sym,St>> entries;
      for( callIterator it = ent.begin(); it != ent.end(); it++ )
      {
        entries.insert( std::pair<Sym,St>(Trans::getCallSym(*it),Trans::getEntry(*it)) );
      }
      return entries;
    }

    /**
     *
     * @brief add a call transition to the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the call transition already exists in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::addCallTrans( St from, Sym sym, St to )
    {
      assert(! Symbols::isEpsilon(sym) ); //An Epsilon symbol on a call doesn't make sense.

      //Add the states and symbol of this transition to the appropriate sets.
      addState(from);
      addSymbol(sym);
      addState(to);

      return trans->addCall(from,sym,to);
    }

    /**
     *
     * @brief add a call transition to the NWA
     *
     * @param - ct: the call transition to add
     * @return false if the call transition already exists in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::addCallTrans( Call & ct )
    {
      assert(! Symbols::isEpsilon(Trans::getCallSym(ct)) ); //An Epsilon symbol on a call doesn't make sense.

      //Add the states and symbol of this transition to the appropriate sets.
      addState(Trans::getCallSite(ct));
      addSymbol(Trans::getCallSym(ct));
      addState(Trans::getEntry(ct));

      return trans->addCall(ct);
    }

    /**
     *
     * @brief remove a call transition from the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the call transition does not exist in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::removeCallTrans( St from, Sym sym, St to )
    {
      if(! isState(from) || ! isSymbol(sym) || ! isState(to) || Symbols::isEpsilon(sym) )
        return false;

      return trans->removeCall(from,sym,to);
    }

    /**
     *
     * @brief remove a call transition from the NWA
     *
     * @param - ct: the call transition to remove
     * @return false if the call transition does not exist in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::removeCallTrans( const Call & ct )
    {
      if(! isState(Trans::getCallSite(ct))
        || ! isSymbol(Trans::getCallSym(ct))
        || ! isState(Trans::getEntry(ct)) 
        || Symbols::isEpsilon(Trans::getCallSym(ct)) )
        return false;

      return trans->removeCall(ct);
    }

    /**
     *
     * @brief returns the number of call transitions associated with this NWA
     *
     * @return the number of call transitions associated with this NWA
     *
     */
    template <typename Client>
    inline
    size_t NWA<Client>::sizeCallTrans( ) const
    {
      return trans->sizeCall();
    }

    //Internal Transitions

    /**
     * 
     * @brief returns the symbol/target pairs associated with the given source
     *
     * @param - source: the source whose symbol/target pairs to obtain
     * @return the set of symbol/target pairs associated with the given source
     *
     */
    template <typename Client>
    const std::set<std::pair<typename NWA<Client>::Sym,typename NWA<Client>::St>> NWA<Client>::getTargets( St source ) const
    {
      const Internals tgt = trans->getTransFrom(source);
      std::set<std::pair<Sym,St>> targets;
      for( internalIterator it = tgt.begin(); it != tgt.end(); it++ )
      {
        targets.insert( std::pair<Sym,St>(Trans::getInternalSym(*it),Trans::getTarget(*it)) );
      }
      return targets;
    }

    /**
     *
     * @brief add an internal transition to the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the internal transition already exists in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::addInternalTrans( St from, Sym sym, St to )
    {
      //Add the states and symbol of this transition to the appropriate sets.
      addState(from);
      addSymbol(sym);
      addState(to);

      return trans->addInternal(from,sym,to);
    }

    /**
     *
     * @brief add an internal transition to the NWA
     *
     * @param - it: internal transition to add to the NWA
     * @return false if the internal transition already exists in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::addInternalTrans( Internal & it )
    {
      //Add the states and symbol of this transition to the appropriate sets.
      addState(Trans::getSource(it));
      addSymbol(Trans::getInternalSym(it));
      addState(Trans::getTarget(it));

      return trans->addInternal(it);
    }

    /**
     *
     * @brief remove an internal transition from the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the internal transition does not exist in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::removeInternalTrans( St from, Sym sym, St to )
    {
      if(! isState(from) || ! isSymbol(sym) || ! isState(to) )
        return false;

      return trans->removeInternal(from,sym,to);
    }

    /**
     *
     * @brief remove an internal transition from the NWA
     *
     * @param - it: the internal transition to remove
     * @return false if the internal transition does not exist in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::removeInternalTrans( const Internal & it )
    {
      if(! isState(Trans::getSource(it))
        || ! isSymbol(Trans::getInternalSym(it))
        || ! isState(Trans::getTarget(it)) )
        return false;

      return trans->removeInternal(it);
    }

    /**
     *
     * @brief returns the number of internal transitions associated with this NWA
     *
     * @return the number of internal transitions associated with this NWA
     *
     */
    template <typename Client>
    inline
    size_t NWA<Client>::sizeInternalTrans( ) const
    {
      return trans->sizeInternal();
    }

    //Return Transitions

    /**
     * 
     * @brief returns the return sites associated with the given call site
     *
     * @param - callSite: the call site whose return sites to obtain
     * @return the set of return sites associated with the given call site
     *
     */
    template <typename Client>
    inline
    const std::set<typename NWA<Client>::St> NWA<Client>::getReturnSites( St callSite ) const
    {
      return trans->getReturnSites(callSite);
    }

    /**
     * 
     * @brief returns the symbol/return site pairs associated with the given exit point 
     *        and call site
     *
     * @param - exit: the exit point whose symbol/return site pairs to obtain
     * @param - callSite: the call site whose symbol/return site pairs to obtain
     * @return the set of symbol/return site pairs associated with the given exit point 
     *          and call site
     *
     */
    template <typename Client>
    const std::set<std::pair<typename NWA<Client>::Sym,typename NWA<Client>::St>> NWA<Client>::getReturns( St exit, St callSite ) const
    {
      const Returns ret = trans->getTransExit(exit);
      std::set<std::pair<Sym,St>> returns;
      for( returnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        if( Trans::getCallSite(*it) == callSite )
        {
          returns.insert( std::pair<Sym,St>(Trans::getReturnSym(*it),Trans::getReturnSite(*it)) );
        }
      }
      return returns;
    }

    /**
     *
     * @brief add a return transition to the NWA
     *
     * @param - from: the state the edge departs from
     * @param - pred: the state from which the call was initiated  
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the return transition already exists in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::addReturnTrans( St from, St pred, Sym sym, St to )
    {
      assert(! Symbols::isEpsilon(sym) ); //An Epsilon symbol on a return doesn't make sense.

      //Add the states and symbol of this transition to the approprite stes.
      addState(from);
      addState(pred);
      addSymbol(sym);
      addState(to);

      return trans->addReturn(from,pred,sym,to);
    }

    /**
     *
     * @brief add a return transition to the NWA
     *
     * @param - rt: return transition to add to the NWA
     * @return false if the return transition already exists in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::addReturnTrans( Return & rt )
    {
      assert(! Symbols::isEpsilon(Trans::getReturnSym(rt)) ); //An Epsilon symbol on a return doesn't make sense.

      //Add the states and symbol of this transition to the appropriate sets.
      addState(Trans::getExit(rt));
      addState(Trans::getCallSite(rt));
      addSymbol(Trans::getReturnSym(rt));
      addState(Trans::getReturnSite(rt));

      return trans->addReturn(rt);
    }

    /**
     *
     * @brief remove a return transition from the NWA
     *
     * @param - from: of the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the return transition does not exist in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::removeReturnTrans( St from, Sym sym, St to )
    {
      if(! isState(from) || ! isSymbol(sym) || ! isState(to) || Symbols::isEpsilon(sym) )
        return false;
     
      bool removed = false;
      const Trans::States preds = trans->getCallSites(from,to);
      for( Trans::stateIterator it = preds.begin(); it != preds.end(); it++ )
      {
        if(! isState(*it) )
          return false;
        removed = removed || trans->removeReturn(from,*it,sym,to);
      }
      return removed;
    }

    /**
     *
     * @brief remove a return transition from the NWA
     *
     * @param - from: the state the edge departs from
     * @param - pred: the state from which the call was initiated  	   
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the return transition does not exist in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::removeReturnTrans( St from, St pred, Sym sym, St to )
    {
      if(! isState(from) || ! isState(pred) || ! isSymbol(sym) || ! isState(to) 
        || Symbols::isEpsilon(sym) )
        return false;

      return trans->removeReturn(from,pred,sym,to);
    }

    /**
     *
     * @brief remove a return transition from the NWA
     *
     * @param - rt: the return transition to remove
     * @return false if the return transition does not exist in the NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::removeReturnTrans( const Return & rt )
    {
      if(! isState(Trans::getExit(rt))
        || ! isState(Trans::getCallSite(rt))
        || ! isSymbol(Trans::getReturnSym(rt))
        || ! isState(Trans::getReturnSite(rt)) 
        || Symbols::isEpsilon(Trans::getReturnSym(rt)) )
        return false;

      return trans->removeReturn(rt);
    }

    /**
     *
     * @brief returns the number of return transitions associated with this NWA
     *
     * @return the number of return transitions associated with this NWA
     *
     */
    template <typename Client>
    inline
    size_t NWA<Client>::sizeReturnTrans( ) const
    {
      return trans->sizeReturn();
    }

    //Building NWAs

    /**
     *
     * @brief constructs the NWA resulting from the union of the given NWAs 
     *
     * @param - first: the NWA to union with 'second'
     * @param - second: the NWA to union with 'first'
     *
     */
    template <typename Client>
    void NWA<Client>::unionNWA( NWARefPtr first, NWARefPtr second )
    {
      //TODO: write this!

      //Generate a new initial state.
      //Note: This involves generating a new key.  How should this be done?

      //Attach the initial state to all the initial states of the two machines
      //with epsilon transitions.

      //Copy all of the functionality of the two machines.

      //Generate a new final state.
      //Note: This involves generating a new key.  How should this be done?

      //Attach the final state to all the final states of the two machines with
      //epsilon transitions.
    }

    /**
     *
     * @brief constructs the NWA which is the intersection of the given NWAs
     *
     * @param - first: the NWA to intersect with 'second'
     * @param - second: the NWA to intersect with 'first'
     *	
     */
    template <typename Client>
    void NWA<Client>::intersect( NWARefPtr first, NWARefPtr second ) 
    {
      std::set<StatePair> visitedPairs; // All the pairs of states we have ever encountered.
      std::deque<StatePair> worklistPairs; // Pairs of states yet to be processed
      typedef std::map<StatePair, Key> PairStMap;
      PairStMap pairToStMap; // The corresponding state in the product 
                             // for all intersectable pairs encountered

      //Start the worklist with all possible initial states of the intersection NWA.
      //NOTE: Currently this should be just one state (the product of the single initial states of each machine).
      for( stateIterator fit = first->beginInitialStates(); fit != first->endInitialStates(); fit++ ) 
      {
        for( stateIterator sit = second->beginInitialStates(); sit != second->endInitialStates(); sit++ ) 
        {
          St newSt;
          StatePair sp(*fit,*sit);
          visitedPairs.insert(sp);
          ClientInfoRefPtr resCI;
          if( nodeIntersect(first,*fit,second,*sit,newSt) ) 
          {
            intersectClientInfo(first,*fit,second,*sit,newSt,resCI);   //Intersect initial client info.
            addInitialState(newSt); 
            //an initial state could also be a final state.
            if(first->isFinalState(*fit) && second->isFinalState(*sit))
                addFinalState(newSt);

            worklistPairs.push_back(sp);
            pairToStMap[sp] = newSt;
          }

          //perform the epsilon closure of sp
          std::set<StatePair> newPairs;
          epsilonClosure(&newPairs,sp,first,second);
          //add all new pairs to the worklist
          for( std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
          {
            St st;
            ClientInfoRefPtr CI;
            //We don't want to put this on the worklist again.
            visitedPairs.insert(*it); 
            //Check and make sure this intersection makes sense.
            if( nodeIntersect(first,it->first,second,it->second,st) ) 
            {
              //Intersect initial client info.
              intersectClientInfo(first,it->first,second,it->second,st,CI);  
              addInitialState(st);
              //an initital state could also be a final state
              if( first->isFinalState(it->first) && second->isFinalState(it->second) )
                addFinalState(st);

              worklistPairs.push_back(*it);
              pairToStMap[*it] = st;
            }
          } //epsilon closure complete
        }      
      }

      while(! worklistPairs.empty() ) 
      {
        StatePair currpair = worklistPairs.front();
        worklistPairs.pop_front();

        //Process outgoing call transitions
        Calls firstCalls = first->trans->getTransCall(currpair.first);
        Calls secondCalls = second->trans->getTransCall(currpair.second);
        for( Calls::const_iterator fit = firstCalls.begin(); fit != firstCalls.end(); fit++ ) 
        {
          Sym firstSym = Trans::getCallSym(*fit);
          St firstEntry = Trans::getEntry(*fit);
          for( Calls::const_iterator sit = secondCalls.begin(); sit != secondCalls.end(); sit++ ) 
          {
            Sym secondSym = Trans::getCallSym(*sit);
            // Are the symbols intersectable
            Sym resSym;
            if(! edgeIntersect(first,firstSym,second,secondSym,resSym) ) 
              continue; // Symbols not intersectable, do nothing

            St secondEntry = Trans::getEntry(*sit);
            StatePair entryPair(firstEntry,secondEntry);
            
            // If we have already considered tgtPair and found them to be nonintersectable, continue 
            if( visitedPairs.count(entryPair) != 0 && pairToStMap.count(entryPair) == 0 )
              continue;
            visitedPairs.insert(entryPair);

            St resSt;
            ClientInfoRefPtr resCI;
            // Have we seen entryPair before?
            if( pairToStMap.count(entryPair) == 0 ) 
            { 
              //We have not seen this pair before
              // Are the entry nodes intersectable?
              if(! nodeIntersect(first,firstEntry,second,secondEntry,resSt) ) 
                continue;
              // We have a new state in resSt!
              if( first->isFinalState(firstEntry) && second->isFinalState(secondEntry) )
                addFinalState(resSt);
              else
                addState(resSt);

              worklistPairs.push_back(entryPair);
              pairToStMap[entryPair] = resSt;
              
              //perform the epsilon closure of entryPair
              std::set<StatePair> newPairs;
              epsilonClosure(&newPairs,entryPair,first,second);
              //add all new pairs to the worklist
              for( std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
              {
                St st;
                ClientInfoRefPtr CI;
                //If we have already considered this pair and found them nonintersectable, continue
                if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
                  continue;
                visitedPairs.insert(*it);
                //Have we seen this pair before?
                if( pairToStMap.count(*it) == 0 )
                {
                  //Check and make sure this intersection makes sense.
                  if( nodeIntersect(first,it->first,second,it->second,st) )
                  {
                    if( first->isFinalState(it->first) && second->isFinalState(it->second) )
                      addFinalState(st);
                    else
                      addState(st); 

                    worklistPairs.push_back(*it);
                    pairToStMap[*it] = st;
                  }
                  else
                  {
                    //We have seen this pair before.
                    st = pairToStMap[*it];
                  }

                  //Add an edge that is the current call trans with collapsed epsilon internal trans.
                  intersectClientInfoCall(first,Trans::getCallSite(*fit),it->first,
                                          second,Trans::getCallSite(*sit),it->second,
                                          resSym,st,CI);    //Intersect Call Trans client info.
                  addCallTrans(pairToStMap[currpair],resSym,st);  
                }
              }
            } 
            else 
            { 
              //We have seen this pair before.
              resSt = pairToStMap[entryPair];
            }
            
            intersectClientInfoCall(first,Trans::getCallSite(*fit),Trans::getEntry(*fit),
                                    second,Trans::getCallSite(*sit),Trans::getEntry(*sit),
                                    resSym,resSt,resCI);   //Intersect Call Trans client info.
            addCallTrans(pairToStMap[currpair],resSym,resSt);
          }
        }

        // Process outgoing internal transitions
        Internals firstInternals = first->trans->getTransFrom(currpair.first);
        Internals secondInternals = second->trans->getTransFrom(currpair.second);
        for( Internals::const_iterator fit = firstInternals.begin(); fit != firstInternals.end(); fit++ ) 
        {
          Sym firstSym = Trans::getInternalSym(*fit);
          St firstTgt = Trans::getTarget(*fit);
          for( Internals::const_iterator sit = secondInternals.begin(); sit != secondInternals.end(); sit++ ) 
          {
            Sym secondSym = Trans::getInternalSym(*sit);
            // Are the symbols intersectable
            Sym resSym;
            if(! edgeIntersect(first,firstSym,second,secondSym,resSym) ) 
              continue; // Symbols not intersectable, do nothing

            St secondTgt = Trans::getTarget(*sit);
            StatePair tgtPair(firstTgt,secondTgt);

            // If we have already considered tgtPair and found its elements to be nonintersectable, continue 
            if( visitedPairs.count(tgtPair) != 0 && pairToStMap.count(tgtPair) == 0 )
              continue;
            visitedPairs.insert(tgtPair);

            St resSt;
            ClientInfoRefPtr resCI;
            // Have we seen tgtPair before?
            if( pairToStMap.count(tgtPair) == 0 ) 
            { 
              //We have not seen this pair before
              // Are the tgt nodes intersectable?
              if(! nodeIntersect(first,firstTgt,second,secondTgt,resSt) ) 
                continue;
              // We have a new state in resSt!
              if( first->isFinalState(firstTgt) && second->isFinalState(secondTgt) )
                addFinalState(resSt);
              else
                addState(resSt);

              worklistPairs.push_back(tgtPair);
              pairToStMap[tgtPair] = resSt;              
              
              //perform the epsilon closure of tgtPair
              std::set<StatePair> newPairs;
              epsilonClosure(&newPairs,tgtPair,first,second);
              //add all new pairs to the worklist
              for( std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
              {
                St st;
                ClientInfoRefPtr CI;
                //If we have already considered this pair and found them nonintersectable, continue
                if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
                  continue;
                visitedPairs.insert(*it);
                //Have we seen this pair before?
                if( pairToStMap.count(*it) == 0 )
                {
                  //Check and make sure this intersection makes sense.
                  if( nodeIntersect(first,it->first,second,it->second,st) )
                  {
                    if( first->isFinalState(it->first) && second->isFinalState(it->second) )
                      addFinalState(st);
                    else
                      addState(st); 

                    worklistPairs.push_back(*it);
                    pairToStMap[*it] = st;
                  }
                }
                else
                {
                  //We have seen this pair before.
                  st = pairToStMap[*it];
                }

                //Add an edge that is the current internal trans with collapsed epsilon internal trans.
                  intersectClientInfoInternal(first,Trans::getSource(*fit),it->first,
                                              second,Trans::getSource(*sit),it->second,
                                              resSym, st, CI);    //Intersect Internal Trans client info.
                  addInternalTrans(pairToStMap[currpair],resSym,st);  
              }
            } 
            else 
            { 
              // we have already seen this pair before
              resSt = pairToStMap[tgtPair];
            }

            intersectClientInfoInternal(first,Trans::getSource(*fit),Trans::getTarget(*fit),
                                        second,Trans::getSource(*sit),Trans::getTarget(*sit),
                                        resSym,resSt,resCI);   //Intersect Internal Trans client info.
            addInternalTrans(pairToStMap[currpair],resSym,resSt);
          }
        }
        
        // Process outgoing return transitions where currpair.first and currpair.second are
        // both exit components of the respective return transitions
        Returns firstReturns = first->trans->getTransExit(currpair.first);
        Returns secondReturns = second->trans->getTransExit(currpair.second);
        for( Returns::const_iterator fit = firstReturns.begin(); fit != firstReturns.end(); fit++ ) 
        {
          St firstCall = Trans::getCallSite(*fit);
          Sym firstSym = Trans::getReturnSym(*fit);
          St firstRet = Trans::getReturnSite(*fit);
          for( Returns::const_iterator sit = secondReturns.begin(); sit != secondReturns.end(); sit++ ) 
          {
            Sym secondSym = Trans::getReturnSym(*sit);
            // Are the symbols intersectable
            Sym resSym;
            if(! edgeIntersect(first,firstSym,second,secondSym,resSym) ) 
              continue; // Symbols not intersectable, do nothing

            // Check intersectability and visited status of the respective call components
            St secondCall = Trans::getCallSite(*sit);
            StatePair callPair(firstCall,secondCall);  // Call components of the respective return transitions
            // Proceed only if the pair of call components has already been visited 
            if( visitedPairs.count(callPair) == 0 )
              continue;
            // If we have already considered callPair and found its elements to be nonintersectable, continue 
            if( visitedPairs.count(callPair) != 0 && pairToStMap.count(callPair) == 0 )
              continue;
            St callSt = pairToStMap[callPair];

            // Check intersectability and visited status of the respective return components
            St secondRet = Trans::getReturnSite(*sit);
            StatePair retPair(firstRet,secondRet);
            // If we have already considered retPair and found its elements to be nonintersectable, continue 
            if( visitedPairs.count(retPair) != 0 && pairToStMap.count(retPair) == 0 )
              continue;
            visitedPairs.insert(retPair);

            St retSt;
            ClientInfoRefPtr resCI;
            // Are the return components intersectable?
            if( pairToStMap.count(retPair) == 0 ) 
            { // Don't know yet
              if(! nodeIntersect(first,firstRet,second,secondRet,retSt) ) 
                continue;
              // We have found a new state in retSt!
              if( first->isFinalState(firstRet) && second->isFinalState(secondRet) )
                addFinalState(retSt);
              else
                addState(retSt);

              worklistPairs.push_back(retPair);
              pairToStMap[retPair] = retSt;
              
              //perform the epsilon closure of retPair
              std::set<StatePair> newPairs;
              epsilonClosure(&newPairs,retPair,first,second);
              //add all new pairs to the worklist
              for( std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
              {
                St st;
                ClientInfoRefPtr CI;
                //If we have already considered this pair and found them nonintersectable, continue
                if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
                  continue;
                visitedPairs.insert(*it);
                //Have we seen this pair before?
                if( pairToStMap.count(*it) == 0 )
                {
                  //Check and make sure this intersection makes sense.
                  if( nodeIntersect(first,it->first,second,it->second,st) )
                  {
                    if( first->isFinalState(it->first) && second->isFinalState(it->second) )
                      addFinalState(st);
                    else
                      addState(st); 

                    worklistPairs.push_back(*it);
                    pairToStMap[*it] = st;
                  }
                }
                else
                {
                  //We have seen this pair before.
                  st = pairToStMap[*it];
                }

                //Add an edge that is the current return trans with collapsed epsilon internal trans.
                intersectClientInfoReturn(first,Trans::getExit(*fit),Trans::getCallSite(*fit),it->first,
                                          second,Trans::getExit(*sit),Trans::getCallSite(*sit),it->second,
                                          resSym,st,CI);    //Intersect Internal Trans client info.
                addReturnTrans(pairToStMap[currpair],callSt,resSym,st);  
              }
            } 
            else 
            {  // We have already seen retPair before and its components are intersectable
              retSt = pairToStMap[retPair];
            }
            
            intersectClientInfoReturn(first,Trans::getExit(*fit),Trans::getCallSite(*fit),Trans::getReturnSite(*fit),
                                      second,Trans::getExit(*sit),Trans::getCallSite(*sit),Trans::getReturnSite(*sit),
                                      resSym,retSt,resCI);   //Intersect Return Trans client info.
            addReturnTrans(pairToStMap[currpair],callSt,resSym,retSt);
          }
        }

        // Process outgoing return transitions where currpair.first and currpair.second are
        // both call components of the respective return transitions 
        firstReturns = first->trans->getTransPred(currpair.first);
        secondReturns = second->trans->getTransPred(currpair.second);
        for( Returns::const_iterator fit = firstReturns.begin(); fit != firstReturns.end(); fit++ ) 
        {
          St firstExit = Trans::getExit(*fit);
          Sym firstSym = Trans::getReturnSym(*fit);
          St firstRet = Trans::getReturnSite(*fit);
          for( Returns::const_iterator sit = secondReturns.begin(); sit != secondReturns.end(); sit++ ) 
          {
            Sym secondSym = Trans::getReturnSym(*sit);
            // Are the symbols intersectable
            Sym resSym;
            if(! edgeIntersect(first,firstSym,second,secondSym,resSym) ) 
              continue; // Symbols not intersectable, do nothing

            // Check intersectability and visited status of the respective exit components
            St secondExit = Trans::getExit(*sit);
            StatePair exitPair(firstExit, secondExit);  // Exit components of the respective return transitions
            // Proceed only if the pair of exit components has already been visited 
            if( visitedPairs.count(exitPair) == 0 )
              continue;
            // If we have already considered exitPair and found its elements to be nonintersectable, continue 
            if( visitedPairs.count(exitPair) != 0 && pairToStMap.count(exitPair) == 0 ) 
              continue;
            St exitSt = pairToStMap[exitPair];

            // Check intersectability and visited status of the respective return components
            St secondRet = Trans::getReturnSite(*sit);
            StatePair retPair(firstRet, secondRet);
            // If we have already considered retPair and found its elements to be nonintersectable, continue 
            if( visitedPairs.count(retPair) != 0 && pairToStMap.count(retPair) == 0 )
              continue;
            visitedPairs.insert(retPair);

            St retSt;
            ClientInfoRefPtr resCI;
            //  Are the return components intersectable?
            if( pairToStMap.count(retPair) == 0 ) 
            { //Don't know yet
              if(! nodeIntersect(first,firstRet,second,secondRet,retSt) ) 
                continue;
              // We have a new state in retSt!
              if( first->isFinalState(firstRet) && second->isFinalState(secondRet) )
                addFinalState(retSt);
              else
                addState(retSt);

              worklistPairs.push_back(retPair);
              pairToStMap[retPair] = retSt;
              
              //perform the epsilon closure of retPair
              std::set<StatePair> newPairs;
              epsilonClosure(&newPairs,retPair,first,second);
              //add all new pairs to the worklist
              for( std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
              {
                St st;
                ClientInfoRefPtr CI;
                //If we have already considered this pair and found them nonintersectable, continue
                if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
                  continue;
                visitedPairs.insert(*it);
                //Have we seen this pair before?
                if( pairToStMap.count(*it) == 0 )
                {
                  //Check and make sure this intersection makes sense.
                  if( nodeIntersect(first,it->first,second,it->second,st) )
                  {
                    if( first->isFinalState(it->first) && second->isFinalState(it->second) )
                      addFinalState(st);
                    else
                      addState(st); 

                    worklistPairs.push_back(*it);
                    pairToStMap[*it] = st;
                  }
                }
                else
                {
                  //We have seen this pair before.
                  st = pairToStMap[*it];
                }

                //Add an edge that is the current return trans with collapsed epsilon internal trans.
                intersectClientInfoReturn(first,Trans::getExit(*fit),Trans::getCallSite(*fit),it->first,
                                          second,Trans::getExit(*sit),Trans::getCallSite(*sit),it->second,
                                          resSym,st,CI);    //Intersect Internal Trans client info.
                addReturnTrans(exitSt,pairToStMap[currpair],resSym,st);  
              }              
            } 
            else 
            { //  We have already seen retPair before and its components are intersectable
              retSt = pairToStMap[retPair];
            }
            
            intersectClientInfoReturn(first,Trans::getExit(*fit),Trans::getCallSite(*fit),Trans::getReturnSite(*fit),
                                      second,Trans::getExit(*sit),Trans::getCallSite(*sit),Trans::getReturnSite(*sit),
                                      resSym,retSt,resCI);   //Intersect Return Trans client info.
            addReturnTrans(exitSt,pairToStMap[currpair],resSym,retSt);
          }
        }
      }
    }

    /**
     *
     * @brief constructs the NWA resulting from the concatenation of the given NWAs
     *
     * @param - first: the NWA to which 'second' should be concatenated
     * @param - second: the NWA to concatenate onto the end of 'first'
     *
     */
    template <typename Client>
    void NWA<Client>::concat( NWARefPtr first, NWARefPtr second )
    {
      //TODO: write this!

      //Duplicate all of the functionality of the first machine (except the final state property).

      //Duplicate all of the functionality of the second machine (except the initial state property).

      //Add epsilon transitions from the final states of the first machine to the initial
      //states of the second machine.

    }

    /**
     * @brief constructs the NWA which is the reverse of the given NWA
     *
     * @param - first: the NWA to reverse
     * 
     */
    template <typename Client>
    void NWA<Client>::reverse( NWARefPtr first )
    {
      //TODO: write this!

      //Swap initial and final states.

      //Duplicate internal transitions with source/target swapped.

      //Duplicate return transitions as call transitions with (return,sym,exit).

      //Duplicate call transitions with associated return transitions as 
      //return transitions with (entry,return,sym,call).

    }

    /**
     *
     * @brief constructs the NWA resulting from performing Kleene-* on the given NWA
     *
     * @param - first: the NWA to perform the Kleene-* of
     *
     */
    template <typename Client>
    void NWA<Client>::star( NWARefPtr first )
    {
      //TODO: write this!
    }

    /**
     * 
     * @brief constructs the NWA that is the complement of the given NWA
     *
     * @param - first: the NWA to perform the complement of
     *
     */
    template <typename Client>
    void NWA<Client>::complement( NWARefPtr first )
    {
      //Start with a deterministic copy of the given NWA.
      if(! first->isDeterministic() )
      {
        determinize(first);
      }
      else
      {
        addAllStates(first->states);
        addAllInitialStates(first->states);
        addAllFinalStates(first->states);
        
        addAllSymbols(first->symbols);

        trans->addAllTrans(*(first->trans));
      }
      
      //FinalStates = AllStates - FinalStates
      std::set<St> oldFinalStates;
      oldFinalStates.insert(beginFinalStates(),endFinalStates());

      for( stateIterator sit = beginStates(); sit != endStates(); sit++ )
      {
        if( oldFinalStates.count(*sit) == 0 )
          addFinalState(*sit);
      }
    }

    /**
     *
     * @brief constructs a deterministic NWA that is equivalent to the given NWA.
     *
     * @param - nondet: the NWA to determinize
     *
     */
    template <typename Client>
    void NWA<Client>::determinize( NWARefPtr nondet )
    {
      StateMap stMap; //Keeps track of the state associated with each set of states.

      //The deterministic NWAs initial state is 
      //{(q,q) | q is an element of Q in nondeterministic NWA }
      StateSet statesSet;
      StatePairSet initialStateSet;
      for( stateIterator sit = nondet->beginInitialStates(); 
            sit != nondet->endInitialStates(); sit++ )
      {
        initialStateSet.insert(StatePair(*sit,*sit));
        statesSet.insert(wali::getKey(*sit,*sit));

        //Traverse any epsilon transitions starting at an initial state.
        std::set<St> newStates;
        epsilonClosure(&newStates,*sit);
        for( std::set<St>::iterator it = newStates.begin(); it != newStates.end(); it++ )
        {
          initialStateSet.insert(StatePair(*sit,*it));
          statesSet.insert(wali::getKey(*sit,*it));
        }
      }

      St initialState = wali::getKey(statesSet);

      //Add the initial state to the deterministic NWA.
      addInitialState(initialState);

      //Update stateSet to state mapping.
      stMap.insert(std::pair<StatePairSet,St>(initialStateSet,initialState));

      //Perform closure steps.
      det(nondet,stMap,initialStateSet,std::stack<StatePairSet>());

      //A state is final if it contains a pair of the form (q,q') with q an element of 
      //Q_in of the the nondeterministic NWA and q' an element of Q_f of the
      //nondeterminisic NWA.
      for( StateMap::iterator it = stMap.begin(); it != stMap.end(); it++ )   
      { //Check each state of the NWA for final states.
        for( StatePairSet::const_iterator sit = (it->first).begin(); 
                                          sit != (it->first).end(); sit++ )  
        {  //Check each pair composing the state.
          if( nondet->isInitialState(sit->first) && nondet->isFinalState(sit->second) ) 
          { //If the elements of the pair have the proper relationship in the nondeterministic NWA,
            //this state is a final state.
            addFinalState(it->second);
            break;  //Move on to checking the next state.
          }
        }
      }
    }

    /**
     *
     * @brief tests whether this NWA is deterministic 
     *
     * @return true if this NWA is deterministic, false otherwise
     *
     */
    template <typename Client>
    bool NWA<Client>::isDeterministic( )
    {
      //An NWA is not deterministic if there is not exactly one initial state
      if( sizeInitialStates() != 1 ) 
        return false;

      //An NWA is not deterministic if there are epsilon transitions 
      //or if there are multiple transitions of some
      //type that are identical except for the to state.
      for( symbolIterator it = symbols.beginSymbols(); it != symbols.endSymbols(); it++ )
      {
        for( stateIterator sit = beginStates(); sit != endStates(); sit++ )
        {
          //Check call transitions.
          int count = 0;
          bool wild = false;
          for( callIterator cit = trans->beginCall(); cit != trans->endCall(); cit++ )
          {
            //Wild symbol TODO: fix this!
            if( SymbolSet::isWild(Trans::getCallSym(*cit)) )
              wild = true;  

            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (Trans::getCallSite(*cit) == (*sit)) && (Trans::getCallSym(*cit) == *it) )
              count++;
          }
          if( count > 1 )
            return false;
          else if( wild && count > 0 )
            return false;

          //Check internal transitions.
          count = 0;
          wild = false;
          for( internalIterator iit = trans->beginInternal(); iit != trans->endInternal(); iit++ )
          {
            //Epsilon transition 
            if( SymbolSet::isEpsilon(Trans::getInternalSym(*iit)) )
              return false;
            //Wild symbol TODO: fix this!
            if( SymbolSet::isWild(Trans::getInternalSym(*iit)) )
              wild = true;  

            //Keep a count of multiple transitions with the same from
            //state and symbol(that is not epsilon).
            else if( (Trans::getSource(*iit) == *sit) && (Trans::getInternalSym(*iit) == *it) )
              count++;
          }
          if( count > 1 )
            return false;
          else if( wild && count > 0 )
            return false;  

          for( stateIterator pit = beginStates(); pit != endStates(); pit++ )
          {
            //Check return transitions.
            count = 0;
            wild = false;
            for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
            {
              //Wild symbol TODO: fix this!
              if( SymbolSet::isWild(Trans::getReturnSym(*rit)) )
                wild = true; 

              //Keep a count of multiple transitions with the same from
              //state and symbol(that is not epsilon).
              else if( (Trans::getExit(*rit) == *sit) 
                       && (Trans::getCallSite(*rit) == *pit) 
                       && (Trans::getReturnSym(*rit) == *it) )
                count++;
            }
            if( count > 1 )
              return false;
            else if( wild && count > 0 )
              return false;  
          }
        }
      }

      return true;
    }

    /**
     * 
     * @brief intersects client information 
     *
     * @param - first: the NWA in which to look up the client information for 'name1'
     * @param - name1: the first state whose client information to intersect
     * @param - second: the NWA in which to look up the client information for 'name2'
     * @param - name2: the second state whose client information to intersect
     * @param - resSt: the state which will receive the computed client information
     * @param - result: the client information resulting from the intersection of
     *                  'name1' and 'name2'
     *
     */
    template <typename Client>
    void NWA<Client>::intersectClientInfo( NWARefPtr first, St name1, 
                                           NWARefPtr second, St name2, 
                                           St resSt, ClientInfoRefPtr result )
    {
      //Note: When overriding this method your metric must combine any client information associated
      // with the states associated with the given names and set the client information of result
      // to that value.
    }

    /**
     * 
     * @brief intersects client information 
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
     * @param - result: the client information resulting from the intersection of 
     *                  'entry1' and 'entry2'
     *
     */
    template <typename Client>
    void NWA<Client>::intersectClientInfoCall( NWARefPtr first, St call1, St entry1, 
                                               NWARefPtr second, St call2, St entry2, 
                                               Sym resSym, St resSt, 
                                               ClientInfoRefPtr result )
    {
      //Note: When overriding this method your metric must combine any client information associated
      // with the target states of the two transitions and set the client information of result
      // to that value.
    }

    /**
     * 
     * @brief intersects client information 
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
     * @param - result: the client information resulting from the intersection of
     *                'tgt1' and 'tgt2'
     *
     */
    template <typename Client>
    void NWA<Client>::intersectClientInfoInternal( NWARefPtr first, St src1, St tgt1, 
                                                   NWARefPtr second, St src2, St tgt2, 
                                                   Sym resSym, St resSt, 
                                                   ClientInfoRefPtr result )
    {
      //Note: When overriding this method your metric must combine any client information associated
      // with the target states of the two transitions and set the client information of result
      // to that value.
    }

    /**
     * 
     * @brief intersects client information 
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
     * @param - result: the client information resulting from the intersection of
     *                'ret1' and 'ret2'
     *
     */
    template <typename Client>
    void NWA<Client>::intersectClientInfoReturn( NWARefPtr first, St exit1, St call1, St ret1,
                                                 NWARefPtr second, St exit2, St call2, St ret2,
                                                 Sym resSym, St resSt,
                                                 ClientInfoRefPtr result )
    {
      //Note: When overriding this method your metric must combine any client information associated
      // with the target states of the two transitions and set the client information of result
      // to that value.
    }

    /**
     * 
     * @brief intersect states
     * 
     * @param - first: the NWA in which to look up information about 'state1'
     * @param - state1: the first state to intersect
     * @param - second: the NWA in which to look up information about 'state2'
     * @param - state2: the second state to intersect
     * @param - resSt: the state that results from performing the intersection
     *
     */
    template <typename Client>
    bool NWA<Client>::nodeIntersect( NWARefPtr first, St state1, NWARefPtr second, St state2, St & resSt ) 
    {
      //Note: When overriding this method your metric must determine whether the
      //      given states are compatible and set resSt to the appropriate state.

      //Q: Should there be more to this?
      resSt = getKey(state1,state2);

      return true;
    }

    /**
     * 
     * @brief intersect symbols
     * 
     * @param - first: the NWA in which to look up information about 'sym1'
     * @param - sym1: the first symbol to intersect
     * @param - second: the NWA in which to look up information about 'sym2'
     * @param - sym2: the second symbol to intersect
     * @param - resSym: the symbol that results from performing the intersection
     *
     */
    template <typename Client>
    bool NWA<Client>::edgeIntersect( NWARefPtr first, Sym sym1, NWARefPtr second, Sym sym2, Sym & resSym )
    {
      //Note: When overriding this method your metric must determine whether the
      //      given symbols are compatible and set result to the appropriate symbol.

      //Epsilons are treated separately in the algorithms, so epsilons match nothing.
      if( SymbolSet::isEpsilon(sym1) || SymbolSet::isEpsilon(sym2) )
        return false;
      if( sym1 == sym2 ) //Symbols only match the exact same symbol (except for epsilons).
      {
        resSym = sym1;
        return true;
      }
      else
        return false;
    }

    //Using NWAs

    /**
     * 
     * @brief constructs the WPDS which is the result of the explicit NWA plus WPDS 
     *        construction from Advanced Querying for Property Checking
     *
     * @param - base: the WPDS that forms the basis for the constructed WPDS
     * @return the WPDS which can be used to perform property checking using PDS 
     *          reachability
     * 
     */
    template <typename Client>
    wpds::WPDS NWA<Client>::plusWPDS( const wpds::WPDS & base )
    {
      //TODO: write this!
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
    template <typename Client>
    void NWA<Client>::PDStoNWA( const wpds::WPDS & pds )
    {
      //TODO: write this!
    }

    /**
     *
     * @brief constructs the PDS equivalent to this NWA
     *
     * Note: This version keeps returns on the stack.
     *
     * @param - wg: the functions to use in generating weights
     * @return the PDS equivalent to this NWA
     *
     */ 
    template <typename Client>
    wpds::WPDS NWA<Client>::NWAtoPDSreturns( WeightGen & wg ) const
    {
      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  //= wali::getKey("program"); 

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( internalIterator iit = trans->beginInternal(); iit != trans->endInternal(); iit++ )
      {      
        //(q,sigma,q') in delta_i goes to <p,q> -w-> <p,q'> in delta_1
        //where the weight w depends on sigma

        //TODO: fix this with respect to wild labels
        if( SymbolSet::isWild(Trans::getInternalSym(*iit)) )
          wgt = wg.getWildWeight(Trans::getSource(*iit),Trans::getTarget(*iit));
        else
          wgt = wg.getWeight(Trans::getSource(*iit),
                              Trans::getInternalSym(*iit),
                              WeightGen::INTRA,
                              Trans::getTarget(*iit));

        result.add_rule(program,                                //from_state (p)
                        Trans::getSource(*iit),                 //from_stack (q)
                        program,                                //to_state (p)
                        Trans::getTarget(*iit),                 //to_stack1 (q')
                        wgt);                                   //weight      
      }

      //Call Transitions 
      for( callIterator cit = trans->beginCall(); cit != trans->endCall(); cit++ )
      {
        for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            //(q_c,sigma,q_e) in delta_c and (q_x,q_c,*,q_r) in delta_r goes to
            // <p,q_c> -w-> <p,q_e q_r> in delta_2 
            // and the weight w depends on sigma

            //TODO: fix this with respect to wild labels
            if( SymbolSet::isWild(Trans::getCallSym(*cit)) )
              wgt = wg.getWildWeight(Trans::getCallSite(*cit),Trans::getEntry(*cit));
            else
              wgt = wg.getWeight(Trans::getCallSite(*cit), 
                                  Trans::getCallSym(*cit),
                                  WeightGen::CALL_TO_ENTRY,  
                                  Trans::getEntry(*cit));  

            result.add_rule(program,                      //from_state (p)
                            Trans::getCallSite(*cit),     //from_stack (q_c)
                            program,                      //to_state (p)
                            Trans::getEntry(*cit),        //to_stack1 (q_e)
                            Trans::getReturnSite(*rit),   //to_stack2 (q_r)
                            wgt);                         //weight      
          }
      }

      //Return Transitions
      for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
      {
        //(q_x,q_c,*,q_r) in delta_r goes to
        // <p,q_x> -w1-> <p_q_x,epsilon> in delta_0
        // and <p_q_x,q_r> -w2-> <p,q_r> in delta_1
        // where p_q_x = (q_x,q_c)
        // w1 depends is one, and w2 depends on sigma

        wgt = wg.getExitWeight( Trans::getExit(*rit) );  //w1

        Key rstate = getControlLocation(Trans::getExit(*rit),Trans::getCallSite(*rit));    //p_q_x

        result.add_rule(program,                    //from_state (p)
                        Trans::getExit(*rit),       //from_stack (q_x)
                        rstate,                     //to_state (p_q_x == (q_x,q_c))
                        wgt);                       //weight (w1) 

        //TODO: fix this with respect to wild labels
        if( SymbolSet::isWild(Trans::getReturnSym(*rit)) )
          wgt = wg.getWildWeight(Trans::getExit(*rit),Trans::getReturnSite(*rit));  //w2
        else
          wgt = wg.getWeight(Trans::getExit(*rit),
                              Trans::getReturnSym(*rit),
                              WeightGen::EXIT_TO_RET,
                              Trans::getReturnSite(*rit));            //w2

        result.add_rule(rstate,                         //from_state (p_q_x == (q_x,q_c))
                        Trans::getReturnSite(*rit),     //from_stack (q_r)
                        program,                        //to_state (p)
                        Trans::getReturnSite(*rit),     //to_stack (q_r)
                        wgt);                           //weight (w2)    
      }  

      return result;
    }

    /**
     *
     * @brief constructs the backwards PDS equivalent to this NWA
     *
     * Note: This version keeps returns on the stack.
     *
     * @param - wg: the functions to use in generating weights
     * @return the backwards PDS equivalent to this NWA
     *
     */
    template <typename Client>
    wpds::WPDS NWA<Client>::NWAtoBackwardsPDSreturns( WeightGen & wg ) const
    {
      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  //= wali::getKey("program"); 

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( internalIterator iit = trans->beginInternal(); iit != trans->endInternal(); iit++ )
      {      
        //(q,sigma,q') in delta_i goes to <p,q'> -w-> <p,q> in delta_1
        //where the weight w depends on sigma

        //TODO: fix this with respect to wild labels
        if( SymbolSet::isWild(Trans::getInternalSym(*iit)) )
          wgt = wg.getWildWeight(Trans::getSource(*iit),Trans::getTarget(*iit));
        else
          wgt = wg.getWeight(Trans::getSource(*iit),
                              Trans::getInternalSym(*iit),
                              WeightGen::INTRA,
                              Trans::getTarget(*iit));

        result.add_rule(program,                                //from_state (p)
                        Trans::getTarget(*iit),                 //from_stack (q')
                        program,                                //to_state (p)
                        Trans::getSource(*iit),                 //to_stack1 (q)
                        wgt);                                   //weight      
      }

      //Call Transitions 
      for( callIterator cit = trans->beginCall(); cit != trans->endCall(); cit++ )
      {
        for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            //(q_c,sigma,q_e) in delta_c and (q_x,q_c,*,q_r) in delta_r goes to
            // <p,q_e> -w1-> <p_q_e,epsilon> in delta_0
            // and <p_q_e,q_c> -w2-> <p,q_c> in delta_1
            // where p_q_e = (q_e,q_r)
            // w1 depends is one, and w2 depends on sigma

            wgt = wg.getEntryWeight( Trans::getEntry(*cit) );  //w1

            Key cstate = getControlLocation(Trans::getEntry(*cit),Trans::getReturnSite(*rit));    //p_q_e

            result.add_rule(program,                    //from_state (p)
                            Trans::getEntry(*cit),      //from_stack (q_e)
                            cstate,                     //to_state (p_q_e == (q_e,q_r))
                            wgt);                       //weight 

            //TODO: fix this with respect to wild labels
            if( SymbolSet::isWild(Trans::getCallSym(*cit)) )
              wgt = wg.getWildWeight(Trans::getCallSite(*cit),Trans::getEntry(*cit));
            else
              wgt = wg.getWeight(Trans::getCallSite(*cit), 
                                  Trans::getCallSym(*cit),
                                  WeightGen::CALL_TO_ENTRY,  
                                  Trans::getEntry(*cit));  

            result.add_rule(cstate,                         //from_state (p_q_e == (q_e,q_r))
                            Trans::getCallSite(*cit),       //from_stack (q_c)
                            program,                        //to_state (p)
                            Trans::getCallSite(*cit),       //to_stack (q_c)
                            wgt);                           //weight    
          }  
      }

      //Return Transitions
      for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
      {
        //(q_x,q_c,*,q_r) in delta_r goes to
        // <p,q_r> -w-> <p,q_x q_c> in delta_2 
        // and the weight w depends on sigma

        //TODO: fix this with respect to wild labels
        if( SymbolSet::isWild(Trans::getReturnSym(*rit)) )
          wgt = wg.getWildWeight(Trans::getExit(*rit),Trans::getReturnSite(*rit));
        else
          wgt = wg.getWeight(Trans::getExit(*rit),
                              Trans::getReturnSym(*rit),
                              WeightGen::EXIT_TO_RET,
                              Trans::getReturnSite(*rit));

        result.add_rule(program,                      //from_state (p)
                        Trans::getReturnSite(*rit),   //from_stack (q_r)
                        program,                      //to_state (p)
                        Trans::getExit(*rit),         //to_stack1 (q_x)
                        Trans::getCallSite(*rit),     //to_stack2 (q_c)
                        wgt);                         //weight      
      }

      return result;
    }

    /**
     *
     * @brief constructs the PDS equivalent to this NWA
     *
     * Note: This version keeps calls on the stack.
     *
     * @param - wg: the functions to use in generating weights
     * @return the PDS equivalent to this NWA
     *
     */ 
    template <typename Client>
    wpds::WPDS NWA<Client>::NWAtoPDScalls( WeightGen & wg ) const
    {
      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  // = wali::getKey("program"); 

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( internalIterator iit = trans->beginInternal(); iit != trans->endInternal(); iit++ )
      {  
        // (q,sigma,q') in delta_i goes to <p,q> -w-> <p,q'> in delta_1
        // where the weight w depends on sigma

        //TODO: fix this with respect to wild labels
        if( SymbolSet::isWild(Trans::getInternalSym(*iit)) )
          wgt = wg.getWildWeight(Trans::getSource(*iit),Trans::getTarget(*iit));
        else
          wgt = wg.getWeight(Trans::getSource(*iit),
                              Trans::getInternalSym(*iit),
                              WeightGen::INTRA,
                              Trans::getTarget(*iit) );

          result.add_rule(program,                                //from_state (p)
                          Trans::getSource(*iit),                 //from_stack (q)
                          program,                                //to_state (p)
                          Trans::getTarget(*iit),                 //to_stack1 (q')
                          wgt);                                   //weight      
      }

      //Call Transitions
      for( callIterator cit = trans->beginCall(); cit != trans->endCall(); cit++ )
      {           
        // (q_c,sigma,q_e) in delta_c goes to
        // <p,q_c> -w-> <p,q_e q_c> in delta_2 
        // and the weight w depends on sigma

        //TODO: fix this with respect to wild labels
        if( SymbolSet::isWild(Trans::getCallSym(*cit)) )
          wgt = wg.getWildWeight(Trans::getCallSite(*cit),Trans::getEntry(*cit));
        else
          wgt = wg.getWeight(Trans::getCallSite(*cit),
                              Trans::getCallSym(*cit),
                              WeightGen::CALL_TO_ENTRY,
                              Trans::getEntry(*cit) );

        result.add_rule(program,                                //from_state (p)
                        Trans::getCallSite(*cit),               //from_stack (q_c)
                        program,                                //to_state (p)
                        Trans::getEntry(*cit),                  //to_stack1 (q_e)
                        Trans::getCallSite(*cit),               //to_stack2 (q_c)
                        wgt);                                   //weight  
      } 

      //Return Transitions
      for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
      {
        // (q_x,q_c,sigma,q_r) in delta_r goes to 
        // <p,q_x> -w1-> <p_q_x,epsilon> in delta_0
        // and <p_q_x,q_c> -w2-> <p,q_r> in delta_1
        // where p_q_x = (p,q_x), w1 depends on q_x, and w2 depends on sigma

        wgt = wg.getExitWeight(Trans::getExit(*rit));  //w1

        //Note: if you change this, make sure you modify the code in NWPForest.createCA()
        Key rstate = getControlLocation(program,Trans::getExit(*rit));  //p_q_x

        result.add_rule(program,                              //from_state (p)
                        Trans::getExit(*rit),                 //from_stack (q_x)
                        rstate,                               //to_state (p_q_x == (p,q_x))
                        wgt);                                 //weight (w1) 

        //TODO: fix this with respect to wild labels
        if( SymbolSet::isWild(Trans::getReturnSym(*rit)) )
          wgt = wg.getWildWeight(Trans::getExit(*rit),Trans::getReturnSite(*rit)); //w2
        else
          wgt = wg.getWeight(Trans::getExit(*rit), 
                              Trans::getReturnSym(*rit),
                              WeightGen::EXIT_TO_RET,  
                              Trans::getReturnSite(*rit));          //w2
         
        result.add_rule(rstate,                               //from_state (p_q_x == (p,q_x))
                        Trans::getCallSite(*rit),             //from_stack (q_c)
                        program,                              //to_state (p)
                        Trans::getReturnSite(*rit),           //to_stack (q_r)
                        wgt);                                 //weight (w2)    

      }

      return result;
    }

    /**
     *
     * @brief constructs the backwards PDS equivalent to this NWA
     *
     * Note: This version keeps calls on the stack.
     *
     * @param - wg: the functions to use in generating weights
     * @return the backwards PDS equivalent to this NWA
     *
     */
    template <typename Client>
    wpds::WPDS NWA<Client>::NWAtoBackwardsPDScalls( WeightGen & wg ) const
    {
      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  // = wali::getKey("program"); 

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( internalIterator iit = trans->beginInternal(); iit != trans->endInternal(); iit++ )
      {  
        // (q,sigma,q') in delta_i goes to <p,q'> -w-> <p,q> in delta_1
        // where the weight w depends on sigma

        //TODO: fix this with respect to wild labels
        if( SymbolSet::isWild(Trans::getInternalSym(*iit)) )
          wgt = wg.getWildWeight(Trans::getSource(*iit),Trans::getTarget(*iit));
        else
          wgt = wg.getWeight(Trans::getSource(*iit),
                              Trans::getInternalSym(*iit),
                              WeightGen::INTRA,
                              Trans::getTarget(*iit) );

        result.add_rule(program,                                //from_state (p)
                        Trans::getTarget(*iit),                 //from_stack (q')
                        program,                                //to_state (p)
                        Trans::getSource(*iit),                 //to_stack1 (q)
                        wgt);                                   //weight      
      }

      //Call Transitions
      for( callIterator cit = trans->beginCall(); cit != trans->endCall(); cit++ )
      {    
        for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            // (q_c,sigma,q_e) in delta_c and (q_x,q_c,*,q_r) in delta_r goes to
            // <p,q_e> -w1-> <p_q_e,epsilon> in delta_0
            // and <p_q_e,q_r> -w2-> <p,q_c> in delta_1
            // where p_q_e = (p,q_e), w1 depends on q_e, and w2 depends on sigma

            wgt = wg.getEntryWeight(Trans::getEntry(*cit));  //w1

            Key cstate = getControlLocation(program,Trans::getEntry(*cit));  //p_q_e

            result.add_rule(program,                                //from_state (p)
                            Trans::getEntry(*cit),                  //from_stack (q_e)
                            cstate,                                 //to_state (p_q_e == (p,q_e))
                            wgt);                                   //weight (w1) 

            //TODO: fix this with respect to wild labels
            if( SymbolSet::isWild(Trans::getCallSym(*cit)) )
              wgt = wg.getWildWeight(Trans::getCallSite(*cit),Trans::getEntry(*cit));   //w2
            else
              wgt = wg.getWeight(Trans::getCallSite(*cit),
                                  Trans::getCallSym(*cit),
                                  WeightGen::CALL_TO_ENTRY,
                                  Trans::getEntry(*cit));                  //w2

            result.add_rule(cstate,                                //from_state (p_q_e == (p,q_e))
                            Trans::getReturnSite(*rit),           //from_stack (q_r)
                            program,                              //to_state (p)
                            Trans::getCallSite(*cit),             //to_stack (q_c)
                            wgt);                                 //weight (w2)    
          }
      } 

      //Return Transitions
      for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
      {
        // (q_x,q_c,sigma,q_r) in delta_r goes to 
        // <p,q_r> -w-> <p,q_x q_r> in delta_2 
        // and the weight w depends on sigma

        //TODO: fix this with respect to wild labels
        if( SymbolSet::isWild(Trans::getReturnSym(*rit)) )
          wgt = wg.getWildWeight(Trans::getExit(*rit),Trans::getReturnSite(*rit)); //w2
        else
          wgt = wg.getWeight(Trans::getExit(*rit), 
                              Trans::getReturnSym(*rit),
                              WeightGen::EXIT_TO_RET,  
                              Trans::getReturnSite(*rit));          //w2
         
        result.add_rule(program,                                    //from_state (p)
                        Trans::getReturnSite(*rit),             //from_stack (q_r)
                        program,                                //to_state (p)
                        Trans::getExit(*rit),                   //to_stack1 (q_x)
                        Trans::getReturnSite(*rit),             //to_stack2 (q_r)
                        wgt);                                   //weight  
      }

      return result;
    }
      
    /**
     *
     * @brief tests whether the language accepted by this NWA is empty
     *
     * @return true if the language accepted by this NWA is empty
     *
     */
    template <typename Client>
    bool NWA<Client>::isEmpty( )
    {
      //An automaton with no initial states must accept only the empty language.
      if( sizeInitialStates() == 0 )
        return true;

      //An automaton with no final states must accept only the empty language.
      if( sizeFinalStates() == 0 )
        return true;

      //An automaton with no path from an initial state to a final state must accept only
      //the empty language.
      for( stateIterator it = beginInitialStates(); it != endInitialStates(); it++ )
      {
        //If the initial state is also a final state, then the empty string is accepted.
        //Therefore the accepted language is not the empty language.
        if( isFinalState(*it) )
          return false;

        //If a path is found, the automaton accepts a non-empty language.
        std::set<St> visited = std::set<St>();
        visited.insert(*it);
        if( hasPath(*it,std::stack<St>(),visited) )
          return false;
        visited.erase(*it);
      }
      return true;
    }

    /**
     *
     * @brief tests whether the given nested word is a member of the language accepted 
     *        by this NWA
     *
     * @param - word: the nested word to test
     * @return true if the given nested word is a member of the language accepted by 
     *          this NWA
     *
     */
    template <typename Client>
    bool NWA<Client>::isMember( nws::NWS word )
    {
      //TODO: write this!
      return false;
    }

    /**
     *
     * @brief perform the prestar reachability query defined by the given WFA
     *
     * @param - input: the starting point of the reachability query
     * @param - wg: the functions to use in generating weights
     * @return the WFA resulting from performing the prestar reachability query 
     *
     */
    template <typename Client>
    wfa::WFA NWA<Client>::prestar( wfa::WFA & input, WeightGen & wg )
    {
      //TODO: Q: is this how I should do this?
      return ((NWAtoPDScalls(wg)).prestar(input));
    }

    /**
     *
     * @brief perform the prestar reachability query defined by the given WFA
     *
     * Note: Any transitions in output before the query will be there after the query but
     * will have no effect on the reachability query.
     *
     * @param - input: the starting point of the reachability query
     * @param - ouput: the result of performing the reachability query
     * @param - wg: the functions to use in generating weights
     *
     */
    template <typename Client>
    void NWA<Client>::prestar( wfa::WFA & input, wfa::WFA & output, WeightGen & wg )
    {
      //TODO: Q: is this how I should do this?
      (NWAtoPDScalls(wg)).prestar(input,output);
    }

    /**
     *
     * @brief perform the poststar reachability query defined by the given WFA
     *
     * @param - input: the starting point of the reachability query
     * @param - wg: the functions to use in generating weights
     * @return the WFA resulting from performing the poststar reachability query
     *
     */
    template <typename Client>
    wfa::WFA NWA<Client>::poststar( wfa::WFA & input, WeightGen & wg )
    {
      //TODO: Q: is this how I should do this?
      return ((NWAtoPDScalls(wg)).poststar(input));
    }

    /**
     *
     * @brief perform the poststar reachability query defined by the given WFA
     * 
     * Note: Any transitions in output before the query will be there after the query but
     * will have no effect on the reachability query.
     *
     * @param - input: the starting point of the reachability query
     * @param - output: the result of performing the reachability query
     * @param - wg: the functions to use in generating weights
     *
     */
    template <typename Client>
    void NWA<Client>::poststar( wfa::WFA & input, wfa::WFA & output, WeightGen & wg )
    {
      //TODO: Q: is this how I should do this?
      (NWAtoPDScalls(wg)).poststar(input,output);
    }

    //Utilities	

    /**
     *
     * @brief print the NWA
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template <typename Client>
    std::ostream & NWA<Client>::print( std::ostream & o ) const
    {
      states.print(o);
      symbols.print(o);
      trans->print(o);
      return o;
    }

    /**
     *
     * @brief print the NWA in dot format
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template <typename Client>
    std::ostream & NWA<Client>::print_dot( std::ostream & o, std::string title ) const
    {
      o << "digraph \"NWA\" { \n";
      o << "    label = \"" << title <<"\" ;\n";
      o << "    subgraph cluster_key {\n";
      o << "        label = \"Key\";\n";
      o << "        style=filled;\n";
      o << "        color=white;\n";
      o << "        dummy [shape=box];";
      o << "        call -> entry [color=green];\n";
      o << "        call -> dummy [color=blue];\n";
      o << "        exit -> dummy [color=red];\n";
      o << "        dummy -> return [style=dashed];\n";
      o << "    }\n";

      //initial state
      printKey(o << "\"",getInitialState())<<"\" [ style=bold ]";

      //final states
      std::set<St> finals = getFinalStates();
      for(std::set<St>::const_iterator it = finals.begin(); it!=finals.end(); it++) 
      {
        printKey(o << "\"",*it) <<"\" [ peripheries=2 ]";
      }

      //Print call transitions.
      o << "//Delta_c: \n";
      callIterator cit = beginCallTrans();
      callIterator citEND = endCallTrans();
      for( bool first=true; cit != citEND; cit++ )
      {
        printKey(o << "\"",Trans::getCallSite(*cit)) << "\"";
        o << "->";
        printKey(o << "\"",Trans::getEntry(*cit)) << "\"";
        o << "[";
        o << " label=\"";
        printKey(o,Trans::getCallSym(*cit));
        o << "\"";
        o << " color=green";
        o << "];\n";        
      }
      o << " \n";
      //Print internal transitions.
      o << "// Delta_i:\n" ;
      internalIterator iit = beginInternalTrans();
      internalIterator iitEND = endInternalTrans();
      for( bool first=true; iit != iitEND; iit++ )
      {
        printKey(o << "\"",Trans::getSource(*iit)) << "\"";
        o << "->";
        printKey(o << "\"",Trans::getTarget(*iit)) << "\"";
        o << "[";
        o << " label=\"";
        printKey(o,Trans::getInternalSym(*iit));
        o << "\"";
        o << "];\n";
      }
      o << " \n";

      //Print return transitions.
      o << "// Delta_r:\n";
      returnIterator rit = beginReturnTrans();
      returnIterator ritEND = endReturnTrans();
      for( bool first=true; rit != ritEND; rit++ )
      {
        //dummy
        o << "\"" << std::dec << Trans::getCallSite(*rit) << std::dec << ", " << Trans::getExit(*rit) << "\"";
        o <<"[ shape=box ];\n";

        // exit to dummy
        printKey(o << "\"",Trans::getExit(*rit)) << "\"";
        o << "->";
        o << "\"" << Trans::getCallSite(*rit) <<", " << Trans::getExit(*rit) << "\"";
        o << "[";
        o << " label=\"";
        printKey(o,Trans::getReturnSym(*rit));
        o << "\"";
        o << " color=red";
        o << "];\n";

        // call to dummy
        printKey(o << "\"",Trans::getCallSite(*rit)) << "\"";
        o << "->";
        o << "\"" << Trans::getCallSite(*rit) <<", " << Trans::getExit(*rit) << "\"";
        o << "[";
        o << " color=blue";
        o << "];\n";

        //dummy to ret
        o << "\"" << Trans::getCallSite(*rit) <<", " << Trans::getExit(*rit) << "\"";
        o << "->";
        printKey(o << "\"",Trans::getReturnSite(*rit)) << "\"";
        o << "[ style=dotted ];\n";

      }
      o << "\n";

      o << "}\n";
      o.flush();
      return o;
    }

    /**
     *
     * @brief tests whether this NWA is equivalent to the NWA 'other'
     *
     * @param - other: the NWA to compare this NWA to
     * @return true if this NWA is equivalent to the NWA 'other'
     *
     */
    template <typename Client>
    bool NWA<Client>::operator==( const NWA & other ) const
    {
      return ( (states == other.states) 
              && (symbols == other.symbols) 
              && (trans == other.trans) );
    }

    //TODO: add methods like ...
    //virtual void for_each(ConstRuleFunctor &func) const;
    //virtual void for_each(RuleFunctor &func) const;
    //virtual void operator()(wfa::ITrans *t);

    /**
     *
     * @brief add all the states in the given StateSet to the NWA
     *
     * @param - addStateSet: the StateSet that contains the states to add
     *
     */
    template <typename Client>
    inline
    void NWA<Client>::addAllStates( States addStateSet )
    {
      states.addAllStates(addStateSet);
    }

    /**
     *
     * @brief provide access to the beginning of the state set
     *
     * @return an iterator pointing to the beginning of the state set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::stateIterator NWA<Client>::beginStates( ) const
    {
      return states.beginStates();
    }

    /**
     * 
     * @brief provide access to the end of the state set
     *
     * @return an iterator pointing just past the end of the state set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::stateIterator NWA<Client>::endStates( ) const
    {
      return states.endStates();
    }

    /**
     *
     * @brief adds all of the states in the given StateSet to the initial state set 
     *        associated with this NWA
     *
     * @param - addStateSet: the state set whose initial states to add to this NWA's
     *                        initial state set
     *
     */
    template <typename Client>
    inline
    void NWA<Client>::addAllInitialStates( States addStateSet )
    {
      states.addAllInitialStates(addStateSet);
    }

    /**
     *
     * @brief provide access to the beginning of the initial state set
     *
     * @return an iterator pointing to the beginning of the initial state set
     *
     */ 
    template <typename Client>
    inline
    typename NWA<Client>::stateIterator NWA<Client>::beginInitialStates( ) const
    {
      return states.beginInitialStates();
    }

    /**
     * 
     * @brief provide access to the end of the initial state set
     *
     * @return an iterator pointing just past the end of the initial state set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::stateIterator NWA<Client>::endInitialStates( ) const
    {
      return states.endInitialStates();
    }

    /**
     *
     * @brief adds all of the final states in the given StateSet to the final state set
     *        associated with this NWA
     *
     * @param - addStateSet: the StateSet whose final states to add to this NWA's
     *                        final state set
     *
     */
    template <typename Client>
    inline
    void NWA<Client>::addAllFinalStates( States addStateSet )
    {
      states.addAllFinalStates(addStateSet);
    }

    /**
     *
     * @brief provide access to the beginning of the final state set
     *
     * @return an iterator pointing to the beginning of the final state set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::stateIterator NWA<Client>::beginFinalStates( ) const
    {
      return states.beginFinalStates();
    }

    /**
     * 
     * @brief provide access to the end of the final state set
     *
     * @return an iterator pointing just past the end of the final state set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::stateIterator NWA<Client>::endFinalStates( ) const
    {
      return states.endFinalStates();
    }

    /**
     *
     * @brief add the given symbols to the NWA
     *
     * @param - addSymbolSet: the symbols to add
     *
     */
    template <typename Client>
    inline
    void NWA<Client>::addAllSymbols( Symbols addSymbolSet )
    {
      symbols.addAllSymbols(addSymbolSet);
    }

    /**
     *
     * @brief provide access to the beginning of the symbol set
     *
     * @return an iterator pointing to the beginning of the symbol set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::symbolIterator NWA<Client>::beginSymbols( ) const
    {
      return symbols.beginSymbols();
    }

    /**
     * 
     * @brief provide access to the end of the symbol set
     *
     * @return an iterator pointing just past the end of the symbol set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::symbolIterator NWA<Client>::endSymbols( ) const
    {
      return symbols.endSymbols();
    }

    /**
     *
     * @brief provide access to the beginning of the call transition set
     *
     * @return an iterator pointing to the beginning of the call transition set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::callIterator NWA<Client>::beginCallTrans( ) const
    {
      return trans->beginCall();
    }

    /**
     * 
     * @brief provide access to the end of the call transition set
     *
     * @return an iterator pointing just past the end of the call transition set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::callIterator NWA<Client>::endCallTrans( ) const
    {
      return trans->endCall();
    }

    /**
     *
     * @brief provide access to the beginning of the internal transition set
     *
     * @return an iterator pointing to the beginning of the internal transition set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::internalIterator NWA<Client>::beginInternalTrans( ) const
    {
      return trans->beginInternal();
    }

    /**
     * 
     * @brief provide access to the end of the internal transition set
     *
     * @return an iterator pointing just past the end of the internal transition set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::internalIterator NWA<Client>::endInternalTrans( ) const
    {
      return trans->endInternal();
    }

    /**
     *
     * @brief provide access to the beginning of the return transition set
     *
     * @return an iterator pointing to the beginning of the return transition set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::returnIterator NWA<Client>::beginReturnTrans( ) const
    {
      return trans->beginReturn();
    }

    /**
     * 
     * @brief provide access to the end of the return transition set
     *
     * @return an iterator pointing just past the end of the return transition set
     *
     */
    template <typename Client>
    inline
    typename NWA<Client>::returnIterator NWA<Client>::endReturnTrans( ) const
    {
      return trans->endReturn();
    }

    /**
     *
     * @brief computes the epsilon closure of the given states in their respective NWAs
     *
     * @param - sp: the starting point of the closure
     * @param - first: the NWA that determines the transitions available to the first 
     *                  component of the state pair
     * @param - second: the NWA that determines the transitions available to the second
     *                  component of the state pair
     * @return the set of state pairs reachable from the initial state pair by traversing
     *          epsilon transitions
     *
     */
    template <typename Client>
    void NWA<Client>::epsilonClosure( std::set<St> * newPairs, St st )
    {
      //compute the states reachable from st via epsilon transitions
      Internals reachable = trans->getInternals(st,SymbolSet::getEpsilon());
      for( Internals::iterator it = reachable.begin(); it != reachable.end(); it++ )
      {
        St newSt = Trans::getTarget(*it);
        //Add a new pair.
        newPairs->insert(newSt);
        //Recursively explore.
        epsilonClosure(newPairs,newSt); 
      }
    }

    /**
     *
     * @brief computes the epsilon closure of the given states in their respective NWAs
     *
     * @param - sp: the starting point of the closure
     * @param - first: the NWA that determines the transitions available to the first 
     *                  component of the state pair
     * @param - second: the NWA that determines the transitions available to the second
     *                  component of the state pair
     * @return the set of state pairs reachable from the initial state pair by traversing
     *          epsilon transitions
     *
     */
    template <typename Client>
    void NWA<Client>::epsilonClosure( std::set<StatePair> * newPairs, StatePair sp, NWARefPtr first, NWARefPtr second )
    {
      //compute the cross produce of all states reachable from sp via epsilon transitions

      //Explore epsilon transitions reachable from the first component of sp.
      Internals reachable = first->trans->getInternals(sp.first,SymbolSet::getEpsilon());
      for( Internals::iterator it = reachable.begin(); it != reachable.end(); it++ )
      {
        StatePair newSP = StatePair(Trans::getTarget(*it),sp.second);
        //Add a new pair.
        newPairs->insert(newSP);
        //Recursively explore.
        epsilonClosure(newPairs,newSP,first,second); 
      }

      //Explore epsilon transitions reachable from the second component of sp.
      reachable = second->trans->getInternals(sp.second,SymbolSet::getEpsilon());
      for( Internals::iterator it = reachable.begin(); it != reachable.end(); it++ )
      {
        StatePair newSP = StatePair(sp.first,Trans::getTarget(*it));
        //Add a new pair.
        newPairs->insert(newSP);
        //Recursively explore.
        epsilonClosure(newPairs,newSP,first,second);
      }
    }

    /**
     *
     * @brief constructs the transition table and state map for the deterministic NWA 
     *        that is equivalent to this NWA.
     *
     * @param - nondet: the NWA to determinize
     * @param - stMap: the map of sets of 'nondet' states to states
     * @param - currSt: the current state under consideration
     * @param - callPred: the stack of open calls at 'currSt'
     *
     */     
    template <typename Client>
    void  NWA<Client>::det( NWARefPtr nondet, StateMap & stMap, StatePairSet currSt, 
                            std::stack<StatePairSet> callPred )  
    {
      //Internal Transisiton
      for( symbolIterator it = nondet->beginSymbols(); it != nondet->endSymbols(); it++ )
      {
        if( SymbolSet::isEpsilon(*it) )
          continue;   //We don't want to end up with epsilon symbols, they are handled differently.
        Internals internalTrans;

        //Find all internal transitions that use this symbol.
        for( internalIterator iit = nondet->beginInternalTrans(); 
              iit != nondet->endInternalTrans(); iit++ )
        {
          //If the symbols match, add this transition to the set of transitions to coalesce.
          if( *it == Trans::getInternalSym(*iit) )
          {
            internalTrans.insert(*iit);
            
            std::set<St> newStates;
            epsilonClosure(&newStates,Trans::getTarget(*iit));
            for( std::set<St>::iterator eit = newStates.begin(); eit != newStates.end(); eit++ )
            {
              //Collapse (q,a,q') and (q',epsilon,q'') to (q,a,q'').
              internalTrans.insert(Internal(Trans::getSource(*iit),Trans::getInternalSym(*iit),*eit));
            }
          }
        }
        //At each internal position labeled a, the automaton replaces each pair (q,q') in
        //the current state by pairs of the form (q,q'') such that (q',a,q'') is an element
        //of delta_i.
        StateSet internalStates;
        StatePairSet internalStateSet;
        for( StatePairSet::iterator sit = currSt.begin(); sit != currSt.end(); sit++ )
        {
          for( internalIterator iit = internalTrans.begin(); iit != internalTrans.end(); iit++ )
          {
            if( sit->second == Trans::getSource(*iit) )
            {
              internalStateSet.insert(StatePair(sit->first,Trans::getTarget(*iit)));
              internalStates.insert(wali::getKey(sit->first,Trans::getTarget(*iit)));
            }
          }
        }
    
        //We only want to add states that don't already exist.
        St internalState;
        if( stMap.count(internalStateSet) == 0 )
        {
          //Add the new key to the map.
          internalState = wali::getKey(internalStates);
          stMap.insert(std::pair<StatePairSet,Key>(internalStateSet,internalState));
          addState(internalState);
          
          //Determinize all paths through 'nondet' starting from this new summary state.
          det(nondet,stMap,internalStateSet,callPred);
        }
        else
        {
          internalState = (stMap.find(internalStateSet))->second;
        }

        if( stMap.find(currSt) != stMap.end() )
        {
          //Add an internal transition to this state.
          addInternalTrans(stMap.find(currSt)->second,*it,internalState);
        }
      }

      //Call Transition
      for( symbolIterator it = nondet->beginSymbols(); it != nondet->endSymbols(); it++ )
      {
        if( SymbolSet::isEpsilon(*it) )
          continue;   //No call transition will have an epsilon as a symbol, so don't waste time.

        //At a call position labeled a, the summary gets reinitialized: the new state 
        //contains pairs of the form (q,q'), where (q,a,q') is an element of delta_c.
        StatePairSet callStateSet;
        StateSet callStates;
        for( callIterator cit = nondet->beginCallTrans(); cit != nondet->endCallTrans(); cit++ )
        {
          //If the symbols match, add this to the summary state.
          if( *it == Trans::getCallSym(*cit) )
          {
            StatePair sp = StatePair(Trans::getCallSite(*cit),Trans::getEntry(*cit));
            callStateSet.insert(sp);
            callStates.insert(wali::getKey(sp.first,sp.second));

            //Traverse any epsilon transitions beginning at the entry point of this call.
            std::set<St> newStates;
            epsilonClosure(&newStates,Trans::getEntry(*cit));
            for( std::set<St>::iterator eit = newStates.begin(); eit != newStates.end(); eit++ )
            {
              StatePair newSP = StatePair(Trans::getCallSite(*cit),*eit);
              callStateSet.insert(newSP);
              callStates.insert(wali::getKey(newSP.first,newSP.second));
            }
          }          
        }

        //We only want to add states that don't already exist.
        St callState;
        if( stMap.count(callStateSet) == 0 )
        {
          //Add the new key to the map.
          callState = wali::getKey(callStates);
          stMap.insert(std::pair<StatePairSet,Key>(callStateSet,callState));
          addState(callState);
          
          //Determinize all paths through 'nondet' starting from this new summary state.
          callPred.push(currSt);
          det(nondet,stMap,callStateSet,callPred);
        }
        else
        {
          callState = (stMap.find(callStateSet))->second;
        }

        if( stMap.find(currSt) != stMap.end() )
        {
          //Add a call transition to this state.
          addCallTrans(stMap.find(currSt)->second,*it,callState);
        }
      }

      //Return Transition
      for( symbolIterator it = nondet->beginSymbols(); it != nondet->endSymbols(); it++ )
      {
        if( SymbolSet::isEpsilon(*it) )
          continue;   //No call transition will have an epsilon as a symbol, so don't waste time.

        //Consider a return position labeled a, and suppose S denotes the current state
        //and S' denotes the state just before the call-predecessor.  Then (q,q') belongs
        //to the new state, provided there exist states q_1,q_2 such that (q,q_1) is an 
        //element of S' and (q_1,q_2) is an element of S and (q_2,q_1,a,q') is an element
        //of delta_r
        StateSet returnStates;
        StatePairSet returnStateSet;
        StatePairSet Sprime = callPred.top();
        for( returnIterator rit = nondet->beginReturnTrans(); rit != nondet->endReturnTrans(); rit++ )
        {
          //If the symbols match, add this to the summary state.  add this transition to the set of transitions to coalesce.
          if( *it == Trans::getReturnSym(*rit) )
          {
            //S' is callPred.top(), S is currSt

            //Want to find (q_1,q_2) in S given *rit = (q_2,q_1,a,q')
            bool found = false;
            for( StatePairSet::iterator sit = currSt.begin(); sit != currSt.end(); sit++ )
            {
              if( (sit->first == Trans::getCallSite(*rit)) && (sit->second == Trans::getExit(*rit)) )
              {
                found = true;
                break;
              }
            }

            if( found )
            {              
              //Want to find (q,q_1) in S' given *rit = (q_2,q_1,a,q')
              for( StatePairSet::iterator sit = Sprime.begin(); sit != Sprime.end(); sit++ )
              {
                if( sit->second == Trans::getCallSite(*rit) )
                {
                  StatePair sp = StatePair(sit->first,Trans::getReturnSite(*rit));
                  returnStateSet.insert(sp);
                  returnStates.insert(wali::getKey(sp.first,sp.second));

                  //Traverse any epsilon transitions beginning at the entry point of this call.
                  std::set<St> newStates;
                  epsilonClosure(&newStates,Trans::getReturnSite(*rit));
                  for( std::set<St>::iterator eit = newStates.begin(); eit != newStates.end(); eit++ )
                  {
                    StatePair newSP = StatePair(sit->first,*eit);
                    returnStateSet.insert(newSP);
                    returnStates.insert(wali::getKey(newSP.first,newSP.second));
                  }
                }
              }
            }
          }
        }

        //We only want to add states that don't already exist.
        St returnState;
        if( stMap.count(returnStateSet) == 0 )
        {
          //Add the new key to the map.
          returnState = wali::getKey(returnStates);
          stMap.insert(std::pair<StatePairSet,Key>(returnStateSet,returnState));
          addState(returnState);
          
          //Determinize all paths through 'nondet' starting from this new summary state.
          callPred.pop();
          det(nondet,stMap,returnStateSet,callPred);
        }
        else
        {
          returnState = (stMap.find(returnStateSet))->second;
        }

        if( (stMap.find(currSt) != stMap.end()) && (stMap.find(Sprime) != stMap.end()) )
        {
          //Add a return transition to this state.
          addReturnTrans(stMap.find(currSt)->second,stMap.find(Sprime)->second,*it,returnState);
        }
      }
    }

    /**
     *
     * @brief tests whether the NWA has a path from the current state to a final state
     *        given the call stack and the set of states already visited
     *
     * @param - currSt: the state from which to start searching for a path
     * @param - calls: the stack of calls that have been made thus far  
     * @param - visited: the states which have already been visited
     * @return true if there is a path through the NWA from the current state to a 
     *          final state, false otherwise
     *
     */
    template <typename Client>
    bool NWA<Client>::hasPath( St currSt, std::stack< St > calls, std::set< St > visited )
    {
      //Try each possible transition out of this state to see if a final state can be reached.

      //First try call transitions.
      calls.push(currSt);
      for( callIterator cit = trans->beginCall(); cit != trans->endCall(); cit++ )
      {
        St entry = Trans::getEntry(*cit);
        //If we can reach a final state through this transition, we have found a path.
        if( isFinalState(entry) )
          return true;
        //If the call transition starts from the current state and goes to a state not 
        //yet visited, follow the path.
        if( (Trans::getCallSite(*cit) == currSt) && (visited.count(entry) == 0) )
        {
          visited.insert(entry);
          if( hasPath(entry,calls,visited) )
            return true;
          visited.erase(entry);
        }
      }
      calls.pop();

      //If no call transition led to a path, try internal transitions.
      for( internalIterator iit = trans->beginInternal(); iit != trans->endInternal(); iit++ )
      {
        St target = Trans::getTarget(*iit);
        //If we can reach a final state through this transition, we have found a path.
        if( isFinalState(target) )
          return true;
        //If the internal transition starts from the current state and goes to a state 
        //not yet visited, follow the path.
        if( (Trans::getSource(*iit) == currSt) && (visited.count(target) == 0) )
        {
          visited.insert(target);
          if( hasPath(target,calls,visited) )
            return true;
          visited.erase(target);
        }
      }

      //If no internal transition led to a path, try return transitions.
      St callSt = calls.top();
      calls.pop();
      for( returnIterator rit = trans->beginReturn(); rit != trans->endReturn(); rit++ )
      {
        St ret = Trans::getReturnSite(*rit);
        //If we can reach a final state through this transition, we have found a path.
        if( isFinalState(ret) )
          return true;
        //If the return transition starts from the current state with the call site from
        //the top of the call stack and goes to a state not yet visited, follow the path.
        if( (Trans::getExit(*rit) == currSt)
          && (Trans::getCallSite(*rit) == callSt)
          && (visited.count(ret) == 0) )
        {
          visited.insert(ret);
          if( hasPath(ret,calls,visited) )
            return true;
          visited.erase(ret);
        }
      }
      calls.push(callSt);

      return false;
    }

    /** 
     *
     * @brief tests whether the nested word suffix is accepted by this nested word 
     *        automaton starting at the state with the given name
     *
     * @param - currState: the state in the nested word automaton at which to start the 
     *                      simulation
     * @param - configs: the nested word suffix to match  //TODO: is this right?
     * @return true if the simulation ends in an accepting state
     *
     */
   /* template <typename Client>
    bool NWA<Client>::matchWord( St currState, std::deque<WordRecConfig<St>> configs )
    {
      //TODO: write this!
    }*/
#endif
  }
}
#endif