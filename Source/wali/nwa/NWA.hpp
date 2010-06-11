#ifndef wali_nwa_NWA_GUARD
#define wali_nwa_NWA_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Countable.hpp"
#include "wali/KeyContainer.hpp"

#include "wali/nwa/SymbolSet.hpp"
#include "wali/nwa/StateSet.hpp"
#include "wali/nwa/TransSet.hpp"

#include "wali/nws/NWS.hpp"
#include "wali/nwa/WordRecConfig.hpp"

#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"

#include "wali/nwa/WeightGen.hpp"
#include "wali/nwa/RelationOps.hpp"

#include "wali/Reach.hpp"

// std::c++
#include <iostream>
#include <sstream>
#include <map>
#include <deque>

namespace wali
{
  namespace nwa
  {
    template<typename Client = ClientInfo>
    class ReachGen : public wali::nwa::WeightGen<Client>
    {
      public:
        enum Kind {INTRA, CALL_TO_ENTRY, EXIT_TO_RET, CALL_TO_RET};
        typedef wali::Key St;
        typedef wali::Key Sym;
         
        //
        // Methods
        //
          
      public:
        //Constructors and Destructor
        ReachGen( ) { }
            
        sem_elem_t getOne( )
        {
          wali::ref_ptr<Reach> r = new Reach(true);
          return r->one();
        }

        sem_elem_t getWeight( const St & src, const Sym & inst, Kind k, const St & tgt )
        {
          return getOne();
        }
           
        sem_elem_t getWildWeight( const St & src, const St & tgt )
        { 
          return getOne();
        }
    };

    /**
     *
     * This class models nested word automata (NWA). 
     * Note: StName must be a unique identifier for states.
     *
     */
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

        static std::string const & XMLTag() {
          static std::string ret = "NWA";
          return ret;
        }

      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      NWA( );
      NWA( St stuckSt );
      NWA( const NWA & other );
      NWA & operator=( const NWA & other );

      void clear( );
      ~NWA( );

      /**
       * Marshalls the NWA as XML to the output stream os
       */
      virtual std::ostream& marshall( std::ostream& os ) const;

      /**
       *
       * @brief returns the Key for the epsilon symbol
       *
       * This method provides access to the Key for the epsilon symbol.
       * The epsilon symbol is the same as wali::WALI_EPSILON.
       *
       * @return the Key for the epsilon symbol
       *
       */
      static Sym getEpsilon( )
      {
        return SymbolSet::getEpsilon();
      }
      /**
       *  
       * @brief test whether the given symbol is the epsilon symbol
       * 
       * This method determines whether the given symbol is the epsilon symbol.
       *
       * @param - sym: the symbol to test
       * @return true if this symbol is the epsilon symbol, false otherwise
       *
       */
      static bool isEpsilon( Sym sym )
      {
        return SymbolSet::isEpsilon(sym);
      }
      /**
       *
       *  //TODO: Handle wild appropriately.
       *  
       *  //Q: what is the meaning of 'wild'?  
       *  //A: It represents all symbols at the time that exist at the time the edge
       *        is processed for an operation   .
       *  //Ex: Suppose we put a wild on an internal edge from '1' to '2' when sigma = {a,b,c}.
       *        Now suppose we add 'd', 'e', and 'f' to sigma.
       *        Should trans.callExists(1,d,2) yield true or false?  True
       *
       * @brief returns the Key for the wild symbol
       *
       * This method provides access to the Key for the wild symbol.
       * The wild symbol is the same as wali::WALI_WILD.
       *
       * @return the Key for the wild symbol
       *
       */
      static Sym getWild( )
      {
        return SymbolSet::getWild();
      }
      /**
       *  
       * @brief test whether the given symbol is the wild symbol
       *
       * This method determines whether the given symbol is the wild symbol.
       *
       * @param - sym: the symbol to test
       * @return true if this symbol is the wild symbol, false otherwise
       *
       */
      static bool isWild( Sym sym )
      {
        return SymbolSet::isWild(sym);
      }

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
      ClientInfoRefPtr getClientInfo( St state ) const;

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
      inline bool hasStuckState()
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
      St getStuckState( ) const
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
      void setStuckState( St state )
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
      inline bool isStuckState( St state )
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
      const std::set<St> & getStates( ) const;
      /**
       *   
       * @brief provides access to all states in the NWA
       *
       * This method provides access to all states.
       * Note: This method simply calls getStates( ).  It is here to make the transition
       *        from using PDS to using NWA easier.
       *
       * @return a set of all states 
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
       * Note: This method modifies transitions, so it cannot be called on an NWA without
       *        a set stuck state.
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
       * Note: This count does include the stuck state if there is one.
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
       * Note: The stuck state cannot be removed in this manner, only by clearing all states
       *        or realizing all implicit transitions.
       * Note: This method modifies transitions, so it cannot be called on an NWA without
       *        a set stuck state.
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
      const std::set<St> & getInitialStates( ) const; 

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
       * Note: If 'state' is the stuck state, then all implicit transitions are realized
       *        (removing the stuck state property from 'state') prior to making 'state' final.
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
       * Note: This method modifies (implicit) transitions, so it cannot be called on an 
       *        NWA without a set stuck state.
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
      bool removeSymbol( Sym sym );

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
       * @brief obtains all the states that are predecessors of the given state
       *
       * This method returns all the states that are predecessors of the given 
       * state.
       *
       * @param - state: the state of the state whose predecessors to look up
       * @param - preds: the set for all states that are predecessors of the given state
       *
       */    
      void getPredecessors( St state, std::set<St> & preds ) const;
      
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
       * Note: This method modifies transitions, so it cannot be called on an NWA without
       *        a set stuck state.
       * Note: It doesn't make sense to duplicate the stuck state as there can only be
       *        one stuck state set in a given machine.
       *
       * @param - orig: the name of the original state, i.e. the state to duplicate
       * @param - dup: the name of the duplicate state
       *
       */
      void duplicateState( St orig, St dup );


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
       * or symbols from the NWA.
       * Note: This method modifies (implicit) transitions, so it cannot be called on an 
       *        NWA with no stuck state.
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
      const std::set<std::pair<Sym,St> > getEntries( St callSite ) const;

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
      bool addCallTrans( St from, Sym sym, St to );

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
      bool removeCallTrans( St from, Sym sym, St to );

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
       * @brief returns the symbol/target pairs associated with the given source
       *
       * This method provides access to the symbol/target pairs associated with the given
       * source.
       *
       * @param - source: the source whose symbol/target pairs to obtain
       * @return the set of symbol/target pairs associated with the given source
       *
       */
      const std::set<std::pair<Sym,St> > getTargets( St source ) const;

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
      bool addInternalTrans( St from, Sym sym, St to );

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
      bool removeInternalTrans( St from, Sym sym, St to );

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
      const std::set<std::pair<Sym,St> > getReturns( St exit, St callSite ) const;

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
      bool addReturnTrans( St from, St pred, Sym sym, St to );

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
      bool removeReturnTrans( St from, Sym sym, St to );

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
      bool removeReturnTrans( St from, St pred, Sym sym, St to );

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

      /**
       *
       * @brief constructs the NWA resulting from the union of the given NWAs 
       *
       * This method constructs the union of the given NWAs by creating a new initial 
       * state with epsilon transitions to all initial states of the two NWAs.
       * Note: The resulting NWA is guaranteed NOT to be deterministic.
       * Note: This method modifies the transitions of the NWA, so it cannot be 
       *        called on a NWA with no stuck state.
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
       * Note: This method modifies the transitions of the NWA, so it cannot be 
       *        called on a NWA with no stuck state.
       *
       * @param - first: the NWA to intersect with 'second'
       * @param - second: the NWA to intersect with 'first'
       *	
       */
      void intersect( NWARefPtr first, NWARefPtr second );

      /**
       * @brief Detects (immediately) stuck states and removes them
       *
       * Detects stuck states and removes them, leaving the transitions implicit.
       * (It only figures this for states that have no outgoing transitions other
       * than itself; you can still have a SCC that acts as a stuck state.)
       */ 
      //void removeImplicitTransitions();


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
      void concat( NWARefPtr first, NWARefPtr second );  

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
      void reverse( NWARefPtr first );  

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
      void star( NWARefPtr first ); 

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
      void complement( NWARefPtr first );

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
       * @brief determines whether there is any overlap in the states of the given NWAs
       *
       * This method tests whether there is any overlap in the states of the given NWAs.
       *
       * @param - first: one of the NWAs whose states to compare
       * @param - second: one of the NWAs whose states to compare
       * @return true if there is some overlap in the states of the given NWAs, false otherwise
       *
       */
      static bool overlap(NWARefPtr first, NWARefPtr second)
      {
        //Iterate through the smaller collection and do the lookup in the larger collection.
        if( first->sizeStates() > second->sizeStates() )
        {
          for( stateIterator sit = second->beginStates(); sit != second->endStates(); sit++ )
          {
            if( first->isState(*sit) )
              return true;
          }
        }
        else
        {
          for( stateIterator sit = first->beginStates(); sit != first->endStates(); sit++ )
          {
            if( second->isState(*sit) )
              return true;
          }
        }
        return false;
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
      virtual void intersectClientInfoCall( NWARefPtr first, St call1, St entry1, 
                                            NWARefPtr second, St call2, St entry2, 
                                            Sym resSym, St resSt );  

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
      virtual void intersectClientInfoInternal( NWARefPtr first, St src1, St tgt1, 
                                                NWARefPtr second, St src2, St tgt2, 
                                                Sym resSym, St resSt );  

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
      virtual void intersectClientInfoReturn( NWARefPtr first, St exit1, St call1, St ret1,
                                              NWARefPtr second, St exit2, St call2, St ret2,
                                              Sym resSym, St resSt );

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
      virtual bool nodeIntersect( NWARefPtr first, St state1, NWARefPtr second, St state2, 
                                  St & resSt, ClientInfoRefPtr & resCI );

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
      virtual void mergeClientInfo( NWARefPtr nwa, 
                    typename relations::RelationTypedefs<St>::BinaryRelation const & binRel, 
                                  St resSt, ClientInfoRefPtr & resCI );

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
      virtual void mergeClientInfoCall( NWARefPtr nwa, 
                    typename relations::RelationTypedefs<St>::BinaryRelation const & binRelCall, 
                    typename relations::RelationTypedefs<St>::BinaryRelation const & binRelEntry,
                                  St callSt, Sym resSym, St resSt, ClientInfoRefPtr & resCI );

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
      virtual void mergeClientInfoInternal( NWARefPtr nwa, 
                    typename relations::RelationTypedefs<St>::BinaryRelation const & binRelSource, 
                    typename relations::RelationTypedefs<St>::BinaryRelation const & binRelTarget,
                                  St sourceSt, Sym resSym, St resSt, ClientInfoRefPtr & resCI );

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
      virtual void mergeClientInfoReturn( NWARefPtr nwa, 
                    typename relations::RelationTypedefs<St>::BinaryRelation const & binRelExit,
                    typename relations::RelationTypedefs<St>::BinaryRelation const & binRelCall, 
                    typename relations::RelationTypedefs<St>::BinaryRelation const & binRelReturn,
                            St exitSt, St callSt, Sym resSym, St resSt, ClientInfoRefPtr & resCI );


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
        //std::cerr << "  getProgramControlLocation entry\n";
        wali::Key ret = getKey("program");
        //std::cerr << "  getProgramControlLocation returning\n";
        return ret;
      };

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
        //std::string str;
        //str = "(" + key2str(exit) + "," + key2str(callSite) + "," + key2str(returnSite) + ")";
        //wali::Key key = getKey(getProgramControlLocation(),getKey(str));

        std::stringstream ss;
        ss << "(key#"  << exit << "," << callSite << "," << returnSite << ")";
        wali::Key key = getKey(getProgramControlLocation(), getKey(ss.str()));
        std::string str = ss.str();

        //std::cerr << "  getControlLocation(" << exit << ", " << callSite << ", " << returnSite << ") -> " << key << "\n";
        //std::cerr << "    exit: " << key2str(exit).size() << " characters\n";
        //std::cerr << "    callSite: " << key2str(callSite).size() << " characters\n";
        //std::cerr << "    returnSite: " << key2str(returnSite).size() << " characters\n";
        //std::cerr << "    ctrl: " << str.size() << " characters\n";

        return key;
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
      wpds::WPDS NWAtoPDSreturns( WeightGen<Client> & wg ) const;

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
      wpds::WPDS NWAtoBackwardsPDSreturns( WeightGen<Client> & wg ) const;  

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
      wpds::WPDS NWAtoPDScalls( WeightGen<Client> & wg ) const;

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
      wpds::WPDS NWAtoBackwardsPDScalls( WeightGen<Client> & wg ) const; 
      
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
       *        by the given NWA
       *
       * This method tests whether the given nested word is a member of the language 
       * accepted by the given NWA.
       *
       * @param - word: the nested word to test
       * @param - aut: the NWA to check the word on
       * @return true if the given nested word is a member of the language accepted by 
       *          the given NWA
       *
       */
      static bool isMember( nws::NWS word, NWARefPtr aut )
      {
        //Q: should we try to walk the automata according to the word using WordRecConfig
        //    or determinize and then step through?
        //A: determinizing and stepping through will be easier to understand and depending
        //    on the optimization of determinize will probably be more efficient
        
        if( aut->isDeterministic() )
        {
          return simulateWord(word,aut);
        }
        else
        {
          //Q: what should be used as the stuck states here?
          //A: it needs to be something no a state in 'aut' unless it is the stuck state
          std::string s = "stuck";
          Key ss = getKey(s);
          while( aut->isState(ss) && !aut->isStuckState(ss) )
          {
            s = s + "~";
            ss = getKey(s);
          }

          NWARefPtr tmp(new NWA(ss));
          tmp->determinize(aut);

          return simulateWord(word,tmp);
        }
      }

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
      static bool inclusion( NWARefPtr first, NWARefPtr second )
      {
        //Q: what should be used as the stuck states here? 
        //A: it needs to be something not a state in 'first' and not a state in 'second' 
        //    unless it is the stuck state
        std::string s = "stuck";
        Key ss = getKey(s);
        while( first->isState(ss) || (second->isState(ss) && !second->isStuckState(ss)) )
        {
          s = s + "~";
          ss = getKey(s);
        }

        //Check L(a1) contained in L(a2) by checking 
        //if L(a1) intersect (complement L(a2)) is empty.
        NWARefPtr comp(new NWA(ss));
        comp->complement(second);   //complement L(a2)
        NWARefPtr inter(new NWA(ss));
        inter->intersect(first,comp); //L(a1) intersect (complement L(a2))

        return inter->isEmpty();
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
      static bool equal( NWARefPtr first, NWARefPtr second)
      {
        //The languages accepted by two NWAs are equivalent if they are both contained
        //in each other, ie L(a1) contained in L(a2) and L(a2) contained in L(a1).
        return (inclusion(first,second) && inclusion(second,first) );
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
      virtual wfa::WFA prestar( wfa::WFA & input, WeightGen<Client> & wg );

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
      virtual void prestar( wfa::WFA & input, wfa::WFA & output, WeightGen<Client> & wg );

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
      virtual wfa::WFA poststar( wfa::WFA & input, WeightGen<Client> & wg );

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
      virtual void poststar( wfa::WFA & input, wfa::WFA & output, WeightGen<Client> & wg );

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
       * @brief returns the state corresponding to the given binary relation
       *
       * This method provides access to the state corresponding to the given binary relation.
       *
       * @param - R: the binary relation whose state to access
       * @return the state corresponding to the given binary relation
       *
       */
      St makeKey( typename relations::RelationTypedefs<St>::BinaryRelation const & R );

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
      static bool simulateWord( nws::NWS word, NWARefPtr aut )
      {
        //Note: For now require that 'word' be a perfectly balanced nested word.
        assert(word.stackSize() == 0);

        St currSt;                                    //Placement in the automaton
        std::stack< St > callSites;                   //Call sites currently unmatched.
        nws::NWSNode * currNode;                      //Placement in the word
        std::stack< nws::NWSNode * > nesting;         //Exit nodes currently on the stack.
        Sym currSym;                                  //Next symbol to follow.

        //Try starting at each initial state in turn. 
        for( stateIterator sit = aut->beginInitialStates(); sit != aut->endInitialStates(); sit++ )
        {
          currSt = *sit;
          callSites = std::stack< St >();                
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
                for( returnIterator rit = rets.begin(); rit != rets.end(); rit++ )
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
                for( callIterator cit = calls.begin(); cit != calls.end(); cit++ )
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
                for( internalIterator iit = ints.begin(); iit != ints.end(); iit++ )
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

        States states;         
        Symbols symbols;        
        Trans trans;

        //TODO: ponder the following ...
        //Q: should we incrementally maintain a wpds?
        //    if we did, what would the weight gen of the wpds be?
    };

    //
    // Methods
    //

    //Constructors and Destructor
    template <typename Client>
    NWA<Client>::NWA( )
    {
      stuck = false;
    }

    template <typename Client>
    NWA<Client>::NWA( St stuckSt )
    {
      stuck = false;  //To satisfy the assertion of setStuckState()
      setStuckState(stuckSt); 
    }

    template <typename Client>
    NWA<Client>::NWA( const NWA & other )
    {
      //Copy data over from 'other'
      stuck = other.stuck;
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
      stuck = other.stuck;
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
    typename NWA<Client>::ClientInfoRefPtr NWA<Client>::getClientInfo( St state ) const
    {
      assert(state < wali::WALI_BAD_KEY);
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
      assert(state < wali::WALI_BAD_KEY);
      states.setClientInfo(state,c);
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
     * @return a set of all states 
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
      assert(state < wali::WALI_BAD_KEY);
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
      assert(state < wali::WALI_BAD_KEY);
      assert(stuck);
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
      assert(state < wali::WALI_BAD_KEY);
      assert(stuck);
      if( isStuckState(state) )   //Can't remove the stuck state.
        return false;

      bool removed = states.removeState(state);
      //Remove transitions associated with the state that was removed.
      if( removed )
      {
        trans.removeTransWith(state);
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

      stuck = false;    //The stuck state has been removed too.
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
      assert(state < wali::WALI_BAD_KEY);
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
      assert(state < wali::WALI_BAD_KEY);
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
      assert(state < wali::WALI_BAD_KEY);
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
      assert(state < wali::WALI_BAD_KEY);
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
      assert(state < wali::WALI_BAD_KEY);

      //Q: should we allow the stuck state to be made final? if so, do we need to do anything special?
      //A: No.  Yes, realize implicit transitions(removing the stuck state in the process) and allow 
      //           the state to be made final.

      //If we are trying to add the stuck state as a final state, all implicit transitions
      //must be realized and this state can no longer be the stuck state.
      if( isStuckState(state) )
      {
        realizeImplicitTrans();
      }

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
      assert(state < wali::WALI_BAD_KEY);
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
      assert(sym < wali::WALI_BAD_KEY);
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
      assert(sym < wali::WALI_BAD_KEY);
      assert(stuck);
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
      assert(sym < wali::WALI_BAD_KEY);

      bool removed = symbols.removeSymbol(sym);
      if( removed )
      {
        trans.removeTransSym(sym);
      }
      return removed;
    }

    /**
     *
     * @brief remove all symbols associated with the NWA
     *
     */
    template <typename Client>
    void NWA<Client>::clearSymbols( )
    {
      //assert(stuck);
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
      assert(from < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);
      return trans.getSymbol(from,to,sym);
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
      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);
      return trans.findTrans(from,sym,to);
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
    void NWA<Client>::getPredecessors( St state, std::set<St> & preds ) const
    {
      assert(state < wali::WALI_BAD_KEY);
       
      std::set<Call> calls = trans.getTransEntry(state);
      for( callIterator cit = calls.begin(); cit != calls.end(); cit++ )
        preds.insert(Trans::getCallSite(*cit));

      std::set<Internal> internals = trans.getTransTo(state);
      for( internalIterator iit = internals.begin(); iit != internals.end(); iit++ )
        preds.insert(Trans::getSource(*iit));

      std::set<Return> returns = trans.getTransRet(state);
      for( returnIterator rit = returns.begin(); rit != returns.end(); rit++ )
        preds.insert(Trans::getExit(*rit));
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
      assert(orig < wali::WALI_BAD_KEY);
      assert(dup < wali::WALI_BAD_KEY);
      assert(stuck);
      assert(!isStuckState(orig));  //Duplicating the stuck state doesn't make sense.

      states.addState(dup);
      states.dupState(orig,dup);  //Duplicate state characteristics(initial/final).

      trans.dupTransOutgoing(orig,dup);  //Duplicate outgoing transitions.
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
      assert(orig < wali::WALI_BAD_KEY);
      assert(dup < wali::WALI_BAD_KEY);
      assert(stuck);
      assert(!isStuckState(orig));  //Duplicating the stuck state doesn't make sense.

      states.addState(dup);
      states.dupState(orig,dup);  //Duplicate state characteristics(initial/final).

      trans.dupTrans(orig,dup);  //Duplicate all transitions.
    }

    /**
     *  
     * @brief realizes all implicit transitions in the NWA
     *
     */
    template <typename Client>
    void NWA<Client>::realizeImplicitTrans()
    {
      std::set<Triple<St, Sym, St> > returns;
      for( returnIterator ret = beginReturnTrans(); ret != endReturnTrans(); ++ret) 
      {
        returns.insert(Triple<St, Sym, St>(ret->first, ret->second, ret->third));
      }
        
      // For every state
      //    For every symbol
      //       For every kind of transition
      //          If *no* outgoing transition exists of that type,
      //          add a transition to the stuck state.
      //
      // - For return transitions, have to check for a return transition
      //   from every other state and add any that don't exist
      // - Also in the implementation, we don't have a "for every kind of
      //   transition" loop; the three types are done one after the other
      for( stateIterator stit = beginStates(); stit != endStates(); ++stit )
      {
        St state = *stit;

        for( symbolIterator symit = beginSymbols(); symit != endSymbols(); ++symit )
        {
          Sym symbol = *symit;

          // Skip over epsilons. (A) they are handled in other places and
          // (B) there's no implicit epsilon transitions to stuck anyway.
          if( symbol == SymbolSet::getEpsilon() )
          {
            continue;
          }

          if( !trans.callExists(state, symbol) )
          {
            addCallTrans(state, symbol, getStuckState());
          }

          if( !trans.internalExists(state, symbol) )
          {
            addInternalTrans(state, symbol, getStuckState());
          }

          for( stateIterator pred = beginStates(); pred != endStates(); ++pred )
          {
            if( returns.find(Triple<St,Sym,St>(state, *pred, symbol)) == returns.end() )
            {
              addReturnTrans(state, *pred, symbol, getStuckState());
            }
          }
        } // for each symbol
      } // for each state

      stuck = false;  //Since all transitions are explicit, the stuck state is reset.
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
      return trans.size();
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
      //assert(stuck);
      trans.clear();
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
    const std::set<std::pair<typename NWA<Client>::Sym,typename NWA<Client>::St> > NWA<Client>::getEntries( St callSite ) const
    {
      assert(callSite < wali::WALI_BAD_KEY);
      const Calls ent = trans.getTransCall(callSite);
      std::set<std::pair<Sym,St> > entries;
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
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      assert(! isEpsilon(sym) ); //An Epsilon symbol on a call doesn't make sense.

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if( isStuckState(from) )
      {
        assert( isStuckState(to) );
      }

      //Add the states and symbol of this transition to the appropriate sets.
      addState(from);
      addSymbol(sym);
      addState(to);

      return trans.addCall(from,sym,to);
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
      assert(stuck);

      assert(Trans::getCallSite(ct) < wali::WALI_BAD_KEY);
      assert(Trans::getCallSym(ct) < wali::WALI_BAD_KEY);
      assert(Trans::getEntry(ct) < wali::WALI_BAD_KEY);

      assert(! isEpsilon(Trans::getCallSym(ct)) ); //An Epsilon symbol on a call doesn't make sense.

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if( isStuckState(Trans::getCallSite(ct)) )
      {
        assert( isStuckState(Trans::getEntry(ct)) );
      }

      //Add the states and symbol of this transition to the appropriate sets.
      addState(Trans::getCallSite(ct));
      addSymbol(Trans::getCallSym(ct));
      addState(Trans::getEntry(ct));

      return trans.addCall(ct);
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
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      if(! isState(from) || ! isSymbol(sym) || ! isState(to) || isEpsilon(sym) )
        return false;

      return trans.removeCall(from,sym,to);
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
      assert(stuck);

      assert(Trans::getCallSite(ct) < wali::WALI_BAD_KEY);
      assert(Trans::getCallSym(ct) < wali::WALI_BAD_KEY);
      assert(Trans::getEntry(ct) < wali::WALI_BAD_KEY);

      if(! isState(Trans::getCallSite(ct))
        || ! isSymbol(Trans::getCallSym(ct))
        || ! isState(Trans::getEntry(ct)) 
        || isEpsilon(Trans::getCallSym(ct)) )
        return false;

      return trans.removeCall(ct);
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
      return trans.sizeCall();
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
    const std::set<std::pair<typename NWA<Client>::Sym,typename NWA<Client>::St> > NWA<Client>::getTargets( St source ) const
    {
      assert(source < wali::WALI_BAD_KEY);
      const Internals tgt = trans.getTransFrom(source);
      std::set<std::pair<Sym,St> > targets;
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
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if( isStuckState(from) )
      {
        assert(isStuckState(to));
      }

      //Add the states and symbol of this transition to the appropriate sets.
      addState(from);
      addSymbol(sym);
      addState(to);

      return trans.addInternal(from,sym,to);
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
      assert(stuck);

      assert(Trans::getSource(it) < wali::WALI_BAD_KEY);
      assert(Trans::getInternalSym(it) < wali::WALI_BAD_KEY);
      assert(Trans::getTarget(it) < wali::WALI_BAD_KEY);

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if( isStuckState(Trans::getSource(it)) )
      {
        assert( isStuckState(Trans::getTarget(it)) );
      }

      //Add the states and symbol of this transition to the appropriate sets.
      addState(Trans::getSource(it));
      addSymbol(Trans::getInternalSym(it));
      addState(Trans::getTarget(it));

      return trans.addInternal(it);
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
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      if(! isState(from) || ! isSymbol(sym) || ! isState(to) )
        return false;

      return trans.removeInternal(from,sym,to);
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
      assert(stuck);

      assert(Trans::getSource(it) < wali::WALI_BAD_KEY);
      assert(Trans::getInternalSym(it) < wali::WALI_BAD_KEY);
      assert(Trans::getTarget(it) < wali::WALI_BAD_KEY);

      if(! isState(Trans::getSource(it))
        || ! isSymbol(Trans::getInternalSym(it))
        || ! isState(Trans::getTarget(it)) )
        return false;

      return trans.removeInternal(it);
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
      return trans.sizeInternal();
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
      assert(callSite < wali::WALI_BAD_KEY);
      return trans.getReturnSites(callSite);
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
    const std::set<std::pair<typename NWA<Client>::Sym,typename NWA<Client>::St> > NWA<Client>::getReturns( St exit, St callSite ) const
    {
      assert(exit < wali::WALI_BAD_KEY);
      assert(callSite < wali::WALI_BAD_KEY);

      const Returns ret = trans.getTransExit(exit);
      std::set<std::pair<Sym,St> > returns;
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
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(pred < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      assert(! isEpsilon(sym) ); //An Epsilon symbol on a return doesn't make sense.

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if( isStuckState(from) )
      {
        assert( isStuckState(to) );
      }
      else if(  isStuckState(pred) )
      {
        assert( isStuckState(to) );
      }

      //Add the states and symbol of this transition to the approprite stes.
      addState(from);
      addState(pred);
      addSymbol(sym);
      addState(to);

      return trans.addReturn(from,pred,sym,to);
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
      assert(stuck);

      assert(Trans::getExit(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getCallSite(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getReturnSym(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getReturnSite(rt) < wali::WALI_BAD_KEY);

      assert(! isEpsilon(Trans::getReturnSym(rt)) ); //An Epsilon symbol on a return doesn't make sense.

      //Don't allow transitions out of the stuck state (unless they go to the stuck state --
      //otherwise it isn't really a stuck state).
      if(  isStuckState(Trans::getExit(rt)) )
      {
        assert( isStuckState(Trans::getReturnSite(rt)));
      }
      else if( isStuckState(Trans::getCallSite(rt)) )
      {
        assert( isStuckState(Trans::getReturnSite(rt)) );
      }

      //Add the states and symbol of this transition to the appropriate sets.
      addState(Trans::getExit(rt));
      addState(Trans::getCallSite(rt));
      addSymbol(Trans::getReturnSym(rt));
      addState(Trans::getReturnSite(rt));

      return trans.addReturn(rt);
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
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      if(! isState(from) || ! isSymbol(sym) || ! isState(to) || isEpsilon(sym) )
        return false;
     
      bool removed = false;
      const Trans::States preds = trans.getCallSites(from,to);
      for( Trans::stateIterator it = preds.begin(); it != preds.end(); it++ )
      {
        if(! isState(*it) )
          return false;
        removed = removed || trans.removeReturn(from,*it,sym,to);
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
      assert(stuck);

      assert(from < wali::WALI_BAD_KEY);
      assert(pred < wali::WALI_BAD_KEY);
      assert(sym < wali::WALI_BAD_KEY);
      assert(to < wali::WALI_BAD_KEY);

      if(! isState(from) || ! isState(pred) || ! isSymbol(sym) || ! isState(to) 
        || isEpsilon(sym) )
        return false;

      return trans.removeReturn(from,pred,sym,to);
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
      assert(stuck);

      assert(Trans::getExit(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getCallSite(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getReturnSym(rt) < wali::WALI_BAD_KEY);
      assert(Trans::getReturnSite(rt) < wali::WALI_BAD_KEY);

      if(! isState(Trans::getExit(rt))
        || ! isState(Trans::getCallSite(rt))
        || ! isSymbol(Trans::getReturnSym(rt))
        || ! isState(Trans::getReturnSite(rt)) 
        || isEpsilon(Trans::getReturnSym(rt)) )
        return false;

      return trans.removeReturn(rt);
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
      return trans.sizeReturn();
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
      //Q: Do we need to guarantee that there is no overlap in states between machines? 
      //A: YES
      //    If there is overlap, then we could conceivably go through the first half of one
      //    machine and then skip to the other machine, thereby not completing either machine.
      //Q: How should we do this?  
      //    1. Use the same base algorithm as intersection.  
      //    2. Generate new keys by the pairing scheme (1,k) for keys in first and (2,k) for keys in second.
      //    3. Implement a check for overlapping state keys and assert that no such overlap occurs.
      //A: 3 is easiest

      //Q: How should clientInfos be generated for the union NWA?
      //A: The clientInfos from the component machines are copied and added to the union NWA.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      //Test state Key overlap of the two NWAs.
      assert(! overlap(first,second) );

      //Check that the stuck state of this NWA does not exist as a state in either component machine 
      //('first' and 'second') unless it is the stuck state of that machine.
      if(! first->isStuckState(getStuckState()) )
        assert(! first->isState(getStuckState()) );
      if(! second->isStuckState(getStuckState()) )
        assert(! second->isState(getStuckState()) );

      //Copy all of the functionality of the two machines.  
      //States (Initial and final state information included.)
      states.addAll(first->states);   //Note: This includes copying clientInfo information over.  
      states.addAll(second->states);  //Note: This includes copying clientInfo information over.

      //Transitions (Call, internal, and return.)
      trans.addAllTrans(first->trans);
      trans.addAllTrans(second->trans);      
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
      //TODO: ponder the following ...
      //Q: how do we prevent the stuck state from being the same as any of the states that we
      //      generate as a part of this process?
      //A: allow it to be the stuck state of one of the given NWAs or ???

      //Q: do we need to realize all implicit transitions for this to work?  ie
      //   do we want to make explicit transitions to the stuck state so that (m,a,m') and
      //    (n,a,stuck) could potentially produce ( (m,n),a,(m',stuck) )?
      //Note: once one machine is in the stuck state, it will never leave the stuck state,
      //      thus unless the machine in the stuck state has an accepting stuck state,
      //      from any state (m',stuck) there is no path to an accepting state
      //      However, if the stuck state is an accepting state, there could be an accepting
      //      state of the form (m',stuck).  Further, if the other machine's stuck state is
      //      not accepting, then the state (stuck,stuck) will not be accepting.  
      //A: The stuck state is never accepting in any machine, so we do not need to distinguish
      //      (m,stuck) from stuck.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      std::set<StatePair> visitedPairs; // All the pairs of states we have ever encountered.
      std::deque<StatePair> worklistPairs; // Pairs of states yet to be processed
      typedef std::map<StatePair, Key> PairStMap;
      PairStMap pairToStMap; // The corresponding state in the product 
                             // for all intersectable pairs encountered

      //Start the worklist with all possible initial states of the intersection NWA.
      for( stateIterator fit = first->beginInitialStates(); fit != first->endInitialStates(); fit++ ) 
      {
        for( stateIterator sit = second->beginInitialStates(); sit != second->endInitialStates(); sit++ ) 
        {
          St newSt;
          StatePair sp(*fit,*sit);
          visitedPairs.insert(sp);
          ClientInfoRefPtr resCI;
          if( nodeIntersect(first,*fit,second,*sit,newSt,resCI) ) 
          {
            addInitialState(newSt); 
            //an initial state could also be a final state.
            if(first->isFinalState(*fit) && second->isFinalState(*sit))
                addFinalState(newSt);

            //Attach client info to the newly created state.
            states.setClientInfo(newSt,resCI);

            worklistPairs.push_back(sp);
            pairToStMap[sp] = newSt;
          }

          //perform the epsilon closure of sp
          std::set<StatePair> newPairs;
          epsilonClosure(&newPairs,sp,first,second);
          //add all new pairs to the worklist
          for( typename std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
          {
            St st;
            //We don't want to put this on the worklist again.
            visitedPairs.insert(*it); 
            ClientInfoRefPtr CI;
            //Check and make sure this intersection makes sense.
            if( nodeIntersect(first,it->first,second,it->second,st,CI) ) 
            {
              addInitialState(st);
              //an initital state could also be a final state
              if( first->isFinalState(it->first) && second->isFinalState(it->second) )
                addFinalState(st);

              //Attach client info to the newly created state.
              states.setClientInfo(st,CI);

              worklistPairs.push_back(*it);
              pairToStMap[*it] = st;
            }
          } //epsilon closure complete
        }      
      }

      //Process the worklist one pair at a time.
      while(! worklistPairs.empty() ) 
      {
        StatePair currpair = worklistPairs.front();
        worklistPairs.pop_front();

        //Process outgoing call transitions
        Calls firstCalls = first->trans.getTransCall(currpair.first);
        Calls secondCalls = second->trans.getTransCall(currpair.second);
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
            // Have we seen entryPair before?
            if( pairToStMap.count(entryPair) == 0 ) 
            { 
              //We have not seen this pair before
              // Are the entry nodes intersectable?
              ClientInfoRefPtr resCI;
              if(! nodeIntersect(first,firstEntry,second,secondEntry,resSt,resCI) ) 
                continue;
              // We have a new state in resSt!
              if( first->isFinalState(firstEntry) && second->isFinalState(secondEntry) )
                addFinalState(resSt);
              else
                addState(resSt);

              //Attach client info to the newly created state.
              states.setClientInfo(resSt,resCI);

              //Add this to the worklist.
              worklistPairs.push_back(entryPair);
              pairToStMap[entryPair] = resSt;
              
              //perform the epsilon closure of entryPair
              std::set<StatePair> newPairs;
              epsilonClosure(&newPairs,entryPair,first,second);
              //add all new pairs to the worklist
              for( typename std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
              {
                St st;
                //If we have already considered this pair and found them nonintersectable, continue
                if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
                  continue;
                visitedPairs.insert(*it);
                //Have we seen this pair before?
                if( pairToStMap.count(*it) == 0 )
                {
                  //Check and make sure this intersection makes sense.
                  ClientInfoRefPtr CI;
                  if( nodeIntersect(first,it->first,second,it->second,st,CI) )
                  {
                    if( first->isFinalState(it->first) && second->isFinalState(it->second) )
                      addFinalState(st);
                    else
                      addState(st); 

                    //Attach client info to the newly created state.
                    states.setClientInfo(st,CI);

                    //Add this to the worklist.
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
                                          resSym,st);    //Intersect Call Trans client info.
                  addCallTrans(pairToStMap[currpair],resSym,st);  
                }
              }
            } 
            else 
            { 
              //We have seen this pair before.
              resSt = pairToStMap[entryPair];
            }
            
            //Add an edge that traverses the current call transition.
            intersectClientInfoCall(first,Trans::getCallSite(*fit),Trans::getEntry(*fit),
                                    second,Trans::getCallSite(*sit),Trans::getEntry(*sit),
                                    resSym,resSt);   //Intersect Call Trans client info.
            addCallTrans(pairToStMap[currpair],resSym,resSt);
          }
        }

        // Process outgoing internal transitions
        Internals firstInternals = first->trans.getTransFrom(currpair.first);
        Internals secondInternals = second->trans.getTransFrom(currpair.second);
        for( typename Internals::const_iterator fit = firstInternals.begin(); fit != firstInternals.end(); fit++ ) 
        {
          Sym firstSym = Trans::getInternalSym(*fit);
          St firstTgt = Trans::getTarget(*fit);
          for( typename Internals::const_iterator sit = secondInternals.begin(); sit != secondInternals.end(); sit++ ) 
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
            // Have we seen tgtPair before?
            if( pairToStMap.count(tgtPair) == 0 ) 
            { 
              //We have not seen this pair before
              // Are the tgt nodes intersectable?
              ClientInfoRefPtr resCI;
              if(! nodeIntersect(first,firstTgt,second,secondTgt,resSt,resCI) ) 
                continue;
              // We have a new state in resSt!
              if( first->isFinalState(firstTgt) && second->isFinalState(secondTgt) )
                addFinalState(resSt);
              else
                addState(resSt);

              //Attach client info to the newly created state.
              states.setClientInfo(resSt,resCI);

              worklistPairs.push_back(tgtPair);
              pairToStMap[tgtPair] = resSt;              
              
              //perform the epsilon closure of tgtPair
              std::set<StatePair> newPairs;
              epsilonClosure(&newPairs,tgtPair,first,second);
              //add all new pairs to the worklist
              for( typename std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
              {
                St st;
                //If we have already considered this pair and found them nonintersectable, continue
                if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
                  continue;
                visitedPairs.insert(*it);
                //Have we seen this pair before?
                if( pairToStMap.count(*it) == 0 )
                {
                  //Check and make sure this intersection makes sense.
                  ClientInfoRefPtr CI;
                  if( nodeIntersect(first,it->first,second,it->second,st,CI) )
                  {
                    if( first->isFinalState(it->first) && second->isFinalState(it->second) )
                      addFinalState(st);
                    else
                      addState(st); 

                    //Attach client info to the newly created state.
                    states.setClientInfo(st,CI);

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
                                              resSym, st);    //Intersect Internal Trans client info.
                  addInternalTrans(pairToStMap[currpair],resSym,st);  
              }
            } 
            else 
            { 
              // we have already seen this pair before
              resSt = pairToStMap[tgtPair];
            }

            //Add an edge that is the current internal transition.
            intersectClientInfoInternal(first,Trans::getSource(*fit),Trans::getTarget(*fit),
                                        second,Trans::getSource(*sit),Trans::getTarget(*sit),
                                        resSym,resSt);   //Intersect Internal Trans client info.
            addInternalTrans(pairToStMap[currpair],resSym,resSt);
          }
        }
        
        // Process outgoing return transitions where currpair.first and currpair.second are
        // both exit components of the respective return transitions
        Returns firstReturns = first->trans.getTransExit(currpair.first);
        Returns secondReturns = second->trans.getTransExit(currpair.second);
        for( typename Returns::const_iterator fit = firstReturns.begin(); fit != firstReturns.end(); fit++ ) 
        {
          St firstCall = Trans::getCallSite(*fit);
          Sym firstSym = Trans::getReturnSym(*fit);
          St firstRet = Trans::getReturnSite(*fit);
          for( typename Returns::const_iterator sit = secondReturns.begin(); sit != secondReturns.end(); sit++ ) 
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
            // Are the return components intersectable?
            if( pairToStMap.count(retPair) == 0 ) 
            { // Don't know yet
              ClientInfoRefPtr retCI;
              if(! nodeIntersect(first,firstRet,second,secondRet,retSt,retCI) ) 
                continue;
              // We have found a new state in retSt!
              if( first->isFinalState(firstRet) && second->isFinalState(secondRet) )
                addFinalState(retSt);
              else
                addState(retSt);

              //Attach client info to the newly created state.
              states.setClientInfo(retSt,retCI);

              worklistPairs.push_back(retPair);
              pairToStMap[retPair] = retSt;
              
              //perform the epsilon closure of retPair
              std::set<StatePair> newPairs;
              epsilonClosure(&newPairs,retPair,first,second);
              //add all new pairs to the worklist
              for( typename std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
              {
                St st;
                //If we have already considered this pair and found them nonintersectable, continue
                if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
                  continue;
                visitedPairs.insert(*it);
                //Have we seen this pair before?
                if( pairToStMap.count(*it) == 0 )
                {
                  //Check and make sure this intersection makes sense.
                  ClientInfoRefPtr CI;
                  if( nodeIntersect(first,it->first,second,it->second,st,CI) )
                  {
                    if( first->isFinalState(it->first) && second->isFinalState(it->second) )
                      addFinalState(st);
                    else
                      addState(st);

                    //Attach client info to the newly created state.
                    states.setClientInfo(st,CI);

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
                                          resSym,st);    //Intersect Internal Trans client info.
                addReturnTrans(pairToStMap[currpair],callSt,resSym,st);  
              }
            } 
            else 
            {  // We have already seen retPair before and its components are intersectable
              retSt = pairToStMap[retPair];
            }
            
            //Add an edge that is the current return transition.
            intersectClientInfoReturn(first,Trans::getExit(*fit),Trans::getCallSite(*fit),Trans::getReturnSite(*fit),
                                      second,Trans::getExit(*sit),Trans::getCallSite(*sit),Trans::getReturnSite(*sit),
                                      resSym,retSt);   //Intersect Return Trans client info.
            addReturnTrans(pairToStMap[currpair],callSt,resSym,retSt);
          }
        }

        // Process outgoing return transitions where currpair.first and currpair.second are
        // both call components of the respective return transitions 
        firstReturns = first->trans.getTransPred(currpair.first);
        secondReturns = second->trans.getTransPred(currpair.second);
        for( typename Returns::const_iterator fit = firstReturns.begin(); fit != firstReturns.end(); fit++ ) 
        {
          St firstExit = Trans::getExit(*fit);
          Sym firstSym = Trans::getReturnSym(*fit);
          St firstRet = Trans::getReturnSite(*fit);
          for( typename Returns::const_iterator sit = secondReturns.begin(); sit != secondReturns.end(); sit++ ) 
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
            //  Are the return components intersectable?
            if( pairToStMap.count(retPair) == 0 ) 
            { //Don't know yet
              ClientInfoRefPtr retCI;
              if(! nodeIntersect(first,firstRet,second,secondRet,retSt,retCI) ) 
                continue;
              // We have a new state in retSt!
              if( first->isFinalState(firstRet) && second->isFinalState(secondRet) )
                addFinalState(retSt);
              else
                addState(retSt);

              //Attach client info to the newly created state.
              states.setClientInfo(retSt,retCI);

              worklistPairs.push_back(retPair);
              pairToStMap[retPair] = retSt;
              
              //perform the epsilon closure of retPair
              std::set<StatePair> newPairs;
              epsilonClosure(&newPairs,retPair,first,second);
              //add all new pairs to the worklist
              for( typename std::set<StatePair>::iterator it = newPairs.begin(); it != newPairs.end(); it++ )
              {
                St st;
                //If we have already considered this pair and found them nonintersectable, continue
                if( visitedPairs.count(*it) != 0 && pairToStMap.count(*it) == 0 )
                  continue;
                visitedPairs.insert(*it);
                //Have we seen this pair before?
                if( pairToStMap.count(*it) == 0 )
                {
                  //Check and make sure this intersection makes sense.
                  ClientInfoRefPtr CI;
                  if( nodeIntersect(first,it->first,second,it->second,st,CI) )
                  {
                    if( first->isFinalState(it->first) && second->isFinalState(it->second) )
                      addFinalState(st);
                    else
                      addState(st); 

                    //Attach client info to the newly created state.
                    states.setClientInfo(st,CI);

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
                                          resSym,st);    //Intersect Internal Trans client info.
                addReturnTrans(exitSt,pairToStMap[currpair],resSym,st);  
              }              
            } 
            else 
            { //  We have already seen retPair before and its components are intersectable
              retSt = pairToStMap[retPair];
            }
            
            //Add an edge that is the current return transition.
            intersectClientInfoReturn(first,Trans::getExit(*fit),Trans::getCallSite(*fit),Trans::getReturnSite(*fit),
                                      second,Trans::getExit(*sit),Trans::getCallSite(*sit),Trans::getReturnSite(*sit),
                                      resSym,retSt);   //Intersect Return Trans client info.
            addReturnTrans(exitSt,pairToStMap[currpair],resSym,retSt);
          }
        }
      }
    }


#if 0 // Commented out because I don't know how this interacts with the new stuck state behavior
    template <typename Client>
    void NWA<Client>::removeImplicitTransitions()
    {
      stuck = true;
        
      // This proceeds in two steps: detects stuck states, then removes
      // them and transitions to them.

      // First we detect stuck states. We first assume that every state
      // is stuck. Then proceed through all transitions in the graph
      // (first calls, then internals, then returns); if we see a transition
      // p --> q where p!=q, then we know that p isn't a stuck state.
      // Any state left has the property that every transition out of the
      // state goes to the "real" stuck state or back to itself.
      // (However, we can only remove a state if it accepts iff the real
      // stuck state accepts.)
      StateSet stuckStates(beginStates(), endStates());

      for( callIterator call = trans.beginCall(); call != trans.endCall(); ++call)
      {
        if( Trans::getCallSite(*call) != Trans::getEntry(*call)
            && Trans::getEntry(*call) != States::getStuckState())
        {
          stuckStates.erase(Trans::getCallSite(*call));
        }
      }

      for( internalIterator internal = trans.beginInternal();
           internal != trans.endInternal(); ++internal)
      {
        if( Trans::getSource(*internal) != Trans::getTarget(*internal) 
            && Trans::getTarget(*internal) != States::getStuckState())
        {
          stuckStates.erase(Trans::getSource(*internal));
        }
      }

      for( returnIterator ret = trans.beginReturn(); ret != trans.endReturn(); ++ret)
      {
        if( Trans::getExit(*ret) != Trans::getReturnSite(*ret) 
            && Trans::getReturnSite(*ret) != States::getStuckState())
        {
          stuckStates.erase(Trans::getExit(*ret));
        }
      }

      // Now we have a list of stuck states. Go through and remove each,
      // but only if the acceptingness is the same as the true stuck state.
      for( typename StateSet::iterator stuck = stuckStates.begin();
           stuck != stuckStates.end(); ++stuck)
      {
        if( isFinalState(*stuck) == isFinalState(States::getStuckState()) )
        {
          if(States::isStuckState(*stuck)) {
            removeState(*stuck);
          }
          else {
            // Remove all the explicit transitions to the stuck state at least
            trans.removeTransWith(*stuck);
          }
        }
      }
    }
#endif


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
      //Q: Do we need to guarantee that there is no overlap in states between machines?
      //A: YES!
      //    If there is overlap, then we could conceivably go through the first half of one
      //    machine and then skip to the other machine before completing the first machine.
      //Q: How should we do this?  
      //    1. Use the same base algorithm as intersection.  
      //    2. Generate new keys by the pairing scheme (1,k) for keys in first and (2,k) for keys in second.
      //    3. Implement a check for overlapping state keys and assert that no such overlap occurs.
      //A: 3 is easiest

      //Q: How should clientInfos be generated for the concatenated NWA?
      //A: The clientInfos from the component machines are copied and added to the concatenated NWA.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      //Test state Key overlap of the two NWAs.
      assert(! overlap(first,second) );

      //Check that the stuck state of this NWA does not exist as a state in either component machine 
      //('first' and 'second') unless it is the stuck state of that machine.
      if(! first->isStuckState(getStuckState()) )
        assert(! first->isState(getStuckState()) );
      if(! second->isStuckState(getStuckState()) )
        assert(! second->isState(getStuckState()) );

      //Duplicate all of the functionality of the first machine (except the final state property).
      states.addAllStates(first->states);   //Note: This includes copying clientInfo information over.
      states.addAllInitialStates(first->states);
      trans.addAllTrans(first->trans);

      //Duplicate all of the functionality of the second machine (except the initial state property).
      states.addAllStates(second->states);  //Note: This includes copying clientInfo information over.
      states.addAllFinalStates(second->states);
      trans.addAllTrans(second->trans);

      //Add epsilon transitions from the final states of the first machine to the initial
      //states of the second machine.
      for( stateIterator fit = first->beginFinalStates(); fit != first->endFinalStates(); fit++ )
      {
        for( stateIterator sit = second->beginInitialStates(); sit != second->endInitialStates(); sit++ )
        {
          addInternalTrans(*fit,getEpsilon(),*sit);
        }
      }
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
      //Note: (implicit) transitions to the stuck state in the original machine 
      //       cause problems in the reverse machine if the stuck state is a final state 
      //Q: how do I implement this?
      //A: realize all transitions of first before computing the reverse if stuck is final
      //Note: the stuck state can now never be final, so we do not need to worry about this any more

      //Q: How should clientInfos be generated for the reversed NWA?
      //A: The clientInfos from the component machines are copied and added to the reversed NWA.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      //Check that the stuck state of this NWA does not exist as a state in 'first' 
      //Note: it cannot be the stuck state of 'first' because then it would have an outgoing transition.
      assert(! first->isState(getStuckState()) );

      //The reverse machine has all the states of the original machine.
      states.addAllStates(first->states); //Note: This includes copying clientInfo information over. 

      //Swap initial and final state functionality.
      //Note: we know that this does not cause the stuck state to become a final state
      //      because the stuck state was not an initial state of the original machine 
      //      (as it was not even a state in the original machine).
      for( stateIterator it = first->beginInitialStates(); 
        it != first->endInitialStates(); it++ )
      {
        addFinalState(*it);
      }
      for( stateIterator it = first->beginFinalStates(); 
        it != first->endFinalStates(); it++ )
      {
        addInitialState(*it);
      }

      //Duplicate internal transitions with source/target swapped.
      for( internalIterator it = first->beginInternalTrans(); 
        it != first->endInternalTrans(); it++ )
      {
        addInternalTrans(Trans::getTarget(*it),
                          Trans::getInternalSym(*it),
                          Trans::getSource(*it));
      }

      //Duplicate return transitions as call transitions with (return,sym,exit).
      for( returnIterator it = first->beginReturnTrans(); 
        it != first->endReturnTrans(); it++ )
      {
        addCallTrans(Trans::getReturnSite(*it),
                      Trans::getReturnSym(*it),
                      Trans::getExit(*it));
      }

      //Duplicate call transitions with associated return transitions as 
      //return transitions with (entry,return,sym,call).
      for( callIterator cit = first->beginCallTrans(); 
        cit != first->endCallTrans(); cit++ )
      {
        for( returnIterator rit = first->beginReturnTrans(); 
          rit != first->endReturnTrans(); rit++ )
        {
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            addReturnTrans(Trans::getEntry(*cit),
                            Trans::getReturnSite(*rit),
                            Trans::getCallSym(*cit),
                            Trans::getCallSite(*cit));
          }
        }
      }

    }

    /**
     *
     * @brief constructs the NWA resulting from performing Kleene-* on the given NWA
     *
     * @param - first: the NWA to perform Kleene-* on
     *
     */
    template <typename Client>
    void NWA<Client>::star( NWARefPtr first )
    {
      //TODO: ponder the following ... 
      //Q: how do we prevent the stuck state from being the same as any of the states that we
      //      generate as a part of this process?
      //A: check against all possible overlaps?  this might be expensive, but it would work

      //Q: How should clientInfos be generated for the star NWA?
      //A: The clientInfos from the component machine are copied to both the direct copies of
      //    states and the primed copies of states and added to the star NWA.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      //Somehow mark unmatched nesting edges as if they are pending (tag its state so that 
      //at a return the states labeling the incident nesting edges are ignored).
      St prime = wali::getKey("prime");

      //The state-space of A* is Q + Q'.
      states.addAll(first->states); //Note: This includes copying clientInfo information over.    
      for( stateIterator sit = first->beginStates(); sit != first->endStates(); sit++ )
      {
        St sp = wali::getKey(*sit,prime);
        states.addState(sp);

        //Note: The clientInfos for the states in Q' are copies of the clientInfos for the 
        //      corresponding states from Q.
        //Set the clientInfo of this state.
        ClientInfo info = *(first->getClientInfo(*sit));
        ClientInfoRefPtr ci = ClientInfoRefPtr(new ClientInfo(info));
        states.setClientInfo(sp,ci);
      }

      //The initial and final states of A* are Q0'. 
      for( stateIterator sit = first->beginInitialStates(); 
            sit != first->endInitialStates(); sit++ )
      {
        St sp = wali::getKey(*sit,prime);
        addInitialState(sp);
        addFinalState(sp);
      }

      //Transitions of A*:

      //Internal: for each (q,a,p) in delta_i, A* gets (q,a,p) and (q',a,p') and if
      //          p in Qf, then (q,a,r') and (q',a,r') for each r in Q0
      for( internalIterator iit = first->beginInternalTrans();
            iit != first->endInternalTrans(); iit++ )
      {
        St q = Trans::getSource(*iit);
        Sym a = Trans::getInternalSym(*iit);
        St p = Trans::getTarget(*iit);

        //(q,a,p)
        addInternalTrans(q,a,p);

        //(q',a,p')
        St qp = wali::getKey(q,prime);
        St pp = wali::getKey(p,prime);
        addInternalTrans(qp,a,pp);

        //if p in Qf
        if( first->isFinalState(p) )
        {
          //for each r in Q0
          for( stateIterator sit = first->beginInitialStates(); 
                sit != first->endInitialStates(); sit++ )
          {
            St rp = wali::getKey(*sit,prime);

            //(q,a,r')
            addInternalTrans(q,a,rp);

            //(q',a,r')
            addInternalTrans(qp,a,rp);
          }
        }
      }

      //Call: for each(q,a,p) in delta_c, A* gets (q,a,p) and (q',a,p), 
      //      and if p in Qf then (q,a,r') and (q',a,r') for each r in Q0
      for( callIterator cit = first->beginCallTrans();
            cit != first->endCallTrans(); cit++ )
      {
        St q = Trans::getCallSite(*cit);
        Sym a = Trans::getCallSym(*cit);
        St p = Trans::getEntry(*cit);

        //(q,a,p)
        addCallTrans(q,a,p);

        //(q',a,p)
        St qp = wali::getKey(q,prime);
        addCallTrans(qp,a,p);

        //if p in Qf
        if( first->isFinalState(p) )
        {
          //for each r in Q0
          for( stateIterator sit = first->beginInitialStates();
                sit != first->endInitialStates(); sit++ )
          {
            St rp = wali::getKey(*sit,prime);

            //(q,a,r')
            addCallTrans(q,a,rp);

            //(q',a,r')
            addCallTrans(qp,a,rp);
          }
        }
      }

      //Return: for each (q,r,a,p) in delta_r, A* gets (q,r,a,p) and (q,r',a,p'), 
      //          and if p in Qf, then (q,r,a,s') and (q,r',a,s') for each s in Q0
      //          For each (q,r,a,p) in delra_r with r in Q0, 
      //            A* gets (q',s,a,p') for each s in Q union Q' 
      //          and if p in Qf, then (q',s,a,t') for each s in Q union Q' and t in Q0.
      for( returnIterator rit = first->beginReturnTrans();
            rit != first->endReturnTrans(); rit++ )
      {
        St q = Trans::getExit(*rit);
        St r = Trans::getCallSite(*rit);
        Sym a = Trans::getReturnSym(*rit);
        St p = Trans::getReturnSite(*rit);

        //(q,r,a,p)
        addReturnTrans(q,r,a,p);

        //(q,r',a,p')
        St rp = wali::getKey(r,prime);
        St pp = wali::getKey(p,prime);
        addReturnTrans(q,rp,a,pp);

        //if p in Qf
        if( first->isFinalState(p) )
        {
          //for each s in Q0
          for( stateIterator sit = first->beginInitialStates();
                sit != first->endInitialStates(); sit++ )
          { 
            St sp = wali::getKey(*sit,prime);

            //(q,r,a,s')
            addReturnTrans(q,r,a,sp);

            //(q,r',a,s')
            addReturnTrans(q,rp,a,sp);
          } 
        }

        //if r in Q0
        if( first->isInitialState(r) )
        {
          //for each s in Q
          for( stateIterator sit = first->beginStates();
                sit != first->endStates(); sit++ )
          {
            St s = *sit;
            St qp = wali::getKey(q,prime);

            //Handle s
            //(q',s,a,p')
            addReturnTrans(qp,s,a,pp);

            //Handle corresponding s'
            //(q',s',a,p')
            St sp = wali::getKey(s,prime);
            addReturnTrans(qp,sp,a,pp);

            //if p in Qf
            if( first->isFinalState(p) )
            {
              //for each t in Q0
              for( stateIterator it = first->beginInitialStates();
                    it != first->endInitialStates(); it++ )
              {                
                St tp = wali::getKey(*it,prime);

                //Handle s
                //(q',s,a,t')
                addReturnTrans(qp,s,a,tp);

                //Handle corresponding s'
                //(q',s',a,t')
                addReturnTrans(qp,sp,a,tp);
              }
            }
          }
        }
      }
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
      //Q: How should clientInfos be generated for the complemented NWA?
      //A: The clientInfos from the component machines are copied and added to the complemented NWA.

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      //Check that the stuck state of this NWA does not exist as a state in 'first' 
      //Note: it cannot be the stuck state of 'first' because it is going to become a final state
      //      prior to the completion of adding transitions to this machine.
      assert(! first->isState(getStuckState()) );

      //Start with a deterministic copy of the given NWA.
      if(! first->isDeterministic() )
      {
        determinize(first);   //Note: determinize() will take care of clientInfo information.
      }
      else
      {
        states.addAll(first->states); //Note: This includes copying clientInfo information over.          
        addAllSymbols(first->symbols);
        trans.addAllTrans(first->trans);
      }
      
      //Q: If the stuck state becomes accepting do we need to make explicit all
      //    transitions to the stuck state?  
      //A: Yes, we need to make explicit all transitions to the stuck state. 
      //Realize all implicit transitions so that it makes sense for the stuck state to become 
      //a final state.
      //Q: Should this be done in addFinalState() instead?  What does this mean for setStuckState()? 
      //A: it should be done in addFinalState
      //A: setStuckState should not allow setting as a stuck state any state that is final
      //Q: Technically this only needs to be done if the stuck state becomes a final state.  Is this
      //    always the case, or is there some possible situation in which there is a stuck state that
      //    is currently a final state and thus will not be a final state in the complement machine?
      //A: this is always the case since the stuck state can never be a final state
      
      //FinalStates = AllStates - FinalStates
      std::set<St> oldFinalStates;
      oldFinalStates.insert(beginFinalStates(),endFinalStates());
 
      clearFinalStates();
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
      //TODO: ponder the following ...
      //Q: how do we guarantee that the stuck state is not any of the states generated here?
      
      //Q: what should be done with clientInfos here?
      //A: use helper methods similar to the treatment of clientInfos for intersection

      // assert(stuck);

      using namespace wali::relations;

      typedef typename RelationTypedefs<St>::BinaryRelation BinaryRelation;
      typedef typename RelationTypedefs<St>::TernaryRelation TernaryRelation;

      NWARefPtr nondet_copy(new NWA(*nondet));
      nondet = nondet_copy;

      {
        //BlockTimer timer("Reify consumer transitions (inside determinize)");
        nondet->realizeImplicitTrans();
      }

      // Construct Id
      BinaryRelation Id;
      for( stateIterator sit = nondet->beginStates(); sit != nondet->endStates(); sit++ )
      {
        Id.insert(std::pair<St,St>(*sit,*sit));
      }

      //Construct the epsilon closure relation for the states in nondet.
      BinaryRelation pre_close; //Collapse epsilon transitions.
      BinaryRelation Ie;   //Internal transitions with epsilon.
      project_symbol_3(Ie,nondet->trans.getInternals(),SymbolSet::getEpsilon());   
      transitive_closure<St>(pre_close,Ie);

      BinaryRelation close;
      union_(close, pre_close, Id);

      //The deterministic NWAs initial state is 
      //Epsilon Closure( {(q,q) | q is an element of Q_in in nondeterministic NWA } )
      BinaryRelation R0;
      compose<St>(R0,Id,close);

      //Make a key for this state.
      St r0 = makeKey(R0);
      
      //Add the initial state to the deterministic NWA.
      addInitialState(r0);

      //Set the client info for this new state.
      ClientInfoRefPtr CI;
      mergeClientInfo(nondet,R0,r0,CI);
      states.setClientInfo(r0,CI);

      //Put the initial state on the worklist.
      std::set<BinaryRelation> wl;
      wl.insert(R0);

      //Keep track of all visited states.
      std::set<BinaryRelation> visited;
      
      //Process the states on the worklist.
      while(! wl.empty() )
      {
        //Take a state off of the worklist.
        BinaryRelation R = *wl.begin();
        wl.erase(wl.begin());
  
        //Mark this state as visited.
        visited.insert(R);

        //Make a key for this state.
        St r = makeKey(R);


        // Note: The clientInfo for this state was set before it was put on
        // the worklist
        if( isStuckState(r) ) continue;    
        
        //Check each symbol individually.
        for( symbolIterator it = nondet->beginSymbols(); it != nondet->endSymbols(); it++ )
        {
          if (Symbols::isEpsilon(*it)) continue;    //Epsilon is handled with closure.
          if (Symbols::isWild(*it)) continue;       //Wild is matched to every symbol as we go.

          //Process internal transitions.
          //Compute the relation.
          BinaryRelation Ri;
          BinaryRelation Rtmpi;
          BinaryRelation Ii;
          project_symbol_3(Ii,nondet->trans.getInternals(),*it);   
          project_symbol_3(Ii,nondet->trans.getInternals(),Symbols::getWild());   //Every symbol also matches wild.
          compose<St>(Rtmpi,R,Ii);
          compose<St>(Ri,Rtmpi,close);
          //Make a key for this state.
          St ri = makeKey(Ri);

          //Add the state to the deterministic NWA.
          addState(ri);
          //Add the transition to the deterministic NWA.
          addInternalTrans(r,*it,ri);

          //Adjust the client info for this state.
          ClientInfoRefPtr riCI;
          mergeClientInfoInternal(nondet,R,Ri,r,*it,ri,riCI);
          states.setClientInfo(ri,riCI);

          //Determine whether to add this state to the worklist.
          if( visited.find(Ri) == visited.end() )
          {
            wl.insert(Ri);
          }

          //Process call transitions.
          //Compute the relation.
          BinaryRelation IdClose_Delta2;
          BinaryRelation Rc;
          BinaryRelation Ic;
          project_symbol_3(Ic,nondet->trans.getCalls(),*it);  
          project_symbol_3(Ic,nondet->trans.getCalls(),Symbols::getWild());   //Every symbol also matches wild.
          compose<St>(IdClose_Delta2, R0, Ic);
          compose<St>(Rc,IdClose_Delta2,close);
          //Make a key for this state.
          St rc = makeKey(Rc);
          //Add the state to the deterministic NWA.
          addState(rc);
          //Add the transition to the deterministic NWA.
          addCallTrans(r,*it,rc);

          //Adjust the client info for this state.
          ClientInfoRefPtr rcCI;
          mergeClientInfoCall(nondet,R,Rc,r,*it,rc,rcCI);
          states.setClientInfo(rc,rcCI);

          //Determine whether to add this state to the worklist.
          if( visited.find(Rc) == visited.end() )
          {
            wl.insert(Rc);
          }

          //Process return transitions.
          TernaryRelation Ir;
          project_symbol_4(Ir,nondet->trans.getReturns(),*it);    
          project_symbol_4(Ir,nondet->trans.getReturns(),Symbols::getWild());   //Every symbol also matches wild.
          //For each possible call predecessor:
          for( std::set<BinaryRelation>::iterator rit = visited.begin();
                rit != visited.end(); rit++ )
          {
            //Compute the relation.
            BinaryRelation Rr;
            BinaryRelation Rtmpr;
            merge<St>(Rtmpr,R,*rit,Ir);
            compose<St>(Rr,Rtmpr,close);
            //Make a key for this state and the call predecessor state.
            St rr = makeKey(Rr);
            St rc = makeKey(*rit);
            //Add the state to the deterministic NWA.
            addState(rr);
            //Add the transition to the deterministic NWA.
            addReturnTrans(r,rc,*it,rr);

            //Adjust the client info for this state.
            ClientInfoRefPtr rrCI;
            mergeClientInfoReturn(nondet,R,*rit,Rr,r,rc,*it,rr,rrCI);
            states.setClientInfo(rr,rrCI);

            //Determine whether to add this state to the worklist.
            if( visited.find(Rr) == visited.end() )
            {
              wl.insert(Rr);
            }
          }
          //For each possible exit point:
          for( std::set<BinaryRelation>::iterator rit = visited.begin();
                rit != visited.end(); rit++ )
          {
            //Compute the relation.
            BinaryRelation Rr;
            BinaryRelation Rtmpr;
            merge<St>(Rtmpr,*rit,R,Ir);
            compose<St>(Rr,Rtmpr,close);
            //Make a key for this state and the exit point state.
            St rr = makeKey(Rr);
            St re = makeKey(*rit);
            //Add the state to the deterministic NWA.
            addState(rr);
            //Add the transition to the deterministic NWA.
            addReturnTrans(re,r,*it,rr);
            
            //Adjust the client info for this state.
            ClientInfoRefPtr rrCI;
            mergeClientInfo(nondet,Rr,rr,rrCI);
            states.setClientInfo(rr,rrCI);

            //Determine whether to add this state to the worklist.
            if( visited.find(Rr) == visited.end() )
            {
              wl.insert(Rr);
            }
          }
        }

        //Deterministic final states.
        //Necessary components for a final state, i.e., 
        //any final state must contain one of the pairs in 
        //{(init,fin) | init is an initial state and fin is a final state}
        BinaryRelation Rf;
        for( stateIterator iit = nondet->beginInitialStates();
             iit != nondet->endInitialStates(); iit++ )
        {
          for( stateIterator fit = nondet->beginFinalStates();
               fit != nondet->endFinalStates(); fit++ )
          {
            Rf.insert(std::pair<St,St>(*iit,*fit));
          }
        }
        //For each state in the deterministic maching, check whether it is a final state.
        for( std::set<BinaryRelation>::iterator sit = visited.begin();
              sit != visited.end(); sit++ )
        {
          BinaryRelation Rtmpf;
          relations::intersect(Rtmpf,Rf,*sit);
          if(! Rtmpf.empty() )
          {
            St f = makeKey(*sit);
            addFinalState(f);
          }
        }
      }
    }

    /**
     *
     * @brief returns the state corresponding to the given binary relation
     *
     * @param - R: the binary relation whose state to access
     * @return the state corresponding to the given binary relation
     *
     */
    template <typename Client>
    typename NWA<Client>::St NWA<Client>::makeKey(
      typename relations::RelationTypedefs<St>::BinaryRelation const & R )
    {
      // C++ really needs C++0x's 'auto' keyword :-)
      typedef typename relations::RelationTypedefs<St>::BinaryRelation::const_iterator Iterator;

      std::stringstream ss;
      for( Iterator mit = R.begin(); mit != R.end(); mit++ )
      {
        ss << "(" << key2str(mit->first) << ", " << key2str(mit->second) << ") ";
      }

      //std::cerr << "makeKey -> " << ss.str().size() << " characters\n";

      return getKey(ss.str());
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
      
      //An NWA is not deterministic if there are epsilon transitions. 
      for( internalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
      {
        if( isEpsilon(Trans::getInternalSym(*iit)) )
          return false;
      }
      
      //TODO: optimize this, it can be very expensive
      //An NWA is not deterministic if there are multiple transitions of some
      //type that are identical except for the to state.
      //For each state/symbol pair check that <= 1 transition leaves the state with that symbol.
      for( symbolIterator it = symbols.beginSymbols(); it != symbols.endSymbols(); it++ )
      {
        for( stateIterator sit = beginStates(); sit != endStates(); sit++ )
        {
          //Check call transitions.
          int count = 0;
          bool wild = false;
          for( callIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
          {
            //Wild symbol 
            if( isWild(Trans::getCallSym(*cit)) )
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
          for( internalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
          {
            //Wild symbol 
            if( isWild(Trans::getInternalSym(*iit)) )
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
            for( returnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
            {
              //Wild symbol 
              if( isWild(Trans::getReturnSym(*rit)) )
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
    template <typename Client>
    void NWA<Client>::intersectClientInfoCall( NWARefPtr first, St call1, St entry1, 
                                               NWARefPtr second, St call2, St entry2, 
                                               Sym resSym, St resSt )
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
                                                   Sym resSym, St resSt )
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
                                                 Sym resSym, St resSt )
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
    bool NWA<Client>::nodeIntersect( NWARefPtr first, St state1, NWARefPtr second, St state2, 
                                      St & resSt, ClientInfoRefPtr & resCI ) 
    {
      //Note: When overriding this method your metric must determine whether the
      //      given states are compatible and set resSt to the appropriate state.

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
      if( isEpsilon(sym1) || isEpsilon(sym2) )
        return false;
      //Wild symbols match everything.
      if( isWild(sym1) )
      {
        resSym = sym2;
        return true;
      }
      if( isWild(sym2) )
      {
        resSym = sym1;
        return true;
      }
      if( sym1 == sym2 ) //Symbols only match the exact same symbol (except for epsilons and wilds).
      {
        resSym = sym1;
        return true;
      }
      else
        return false;
    }

    /**
     * 
     * @brief merge states for determinize
     * 
     * @param - nwa: the NWA in which to look up information about the states
     * @param - binRel: the states to merge
     * @param - resCI: the client info that results from performing the merge
     *
     */
    template <typename Client>
    void NWA<Client>::mergeClientInfo( NWARefPtr first, 
                  typename relations::RelationTypedefs<St>::BinaryRelation const & binRel, 
                                St resSt, ClientInfoRefPtr & resCI )
    {
      //Note: When overriding this method your metric must combine any client information associated
      //       with the states of the binary relation and set the client information of resSt to 
      //       that value.
    }

    /**
     * 
     * @brief merges clientInfo for determinize
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
    template <typename Client>
    void NWA<Client>::mergeClientInfoCall( NWARefPtr nwa, 
                  typename relations::RelationTypedefs<St>::BinaryRelation const & binRelCall, 
                  typename relations::RelationTypedefs<St>::BinaryRelation const & binRelEntry,
                                St callSt, Sym resSym, St resSt, ClientInfoRefPtr & resCI )
    {
      //Note: When overriding this method your metric must combine any client information associated
      //       with the states of the binary relation and set the client information of resSt to 
      //       that value.
    }

    /**
     * 
     * @brief merges clientInfo for determinize
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
    template <typename Client>
    void NWA<Client>::mergeClientInfoInternal( NWARefPtr nwa, 
                  typename relations::RelationTypedefs<St>::BinaryRelation const & binRelSource, 
                  typename relations::RelationTypedefs<St>::BinaryRelation const & binRelTarget,
                                St sourceSt, Sym resSym, St resSt, ClientInfoRefPtr & resCI )
    {
      //Note: When overriding this method your metric must combine any client information associated
      //       with the states of the binary relation and set the client information of resSt to 
      //       that value.
    }

    /**
     * 
     * @brief merges clientInfo for determinize
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
    template <typename Client>
    void NWA<Client>::mergeClientInfoReturn( NWARefPtr nwa, 
                  typename relations::RelationTypedefs<St>::BinaryRelation const & binRelExit,
                  typename relations::RelationTypedefs<St>::BinaryRelation const & binRelCall, 
                  typename relations::RelationTypedefs<St>::BinaryRelation const & binRelReturn,
                          St exitSt, St callSt, Sym resSym, St resSt, ClientInfoRefPtr & resCI )
    {
      //Note: When overriding this method your metric must combine any client information associated
      //       with the states of the binary relation and set the client information of resSt to 
      //       that value.
    }

    //Using NWAs

    /**
     * 
     * @brief constructs the WPDS which is the result of the explicit NWA plus WPDS 
     *        construction from Advanced Querying for Property Checking
     *
     * Note: For now we are assuming that implicit transitions to the stuck state need to be 
     *       realized for this construction to work.  Therefore, this method can only be called
     *       on an NWA of type 1 and afterwards it is an NWA of type 2.
     *
     * @param - base: the WPDS that forms the basis for the constructed WPDS
     * @return the WPDS which can be used to perform property checking using PDS 
     *          reachability
     * 
     */
    template <typename Client>
    wpds::WPDS NWA<Client>::plusWPDS( const wpds::WPDS & base )
    {
      //Q: do implicit transitions to the stuck state affect this result?
      //A: it changes the states that are reachable

      realizeImplicitTrans();

      wpds::WPDS result;

      wpds::WpdsRules rules = wpds::WpdsRules();

      base.for_each(rules);

      //Step Rules:
      for( std::set<wpds::Rule>::iterator it = rules.stepRules.begin();
            it != rules.stepRules.end(); it++ )
      {
        //<p,n_1> -w-> <p',n_2> in delta_1, (q,n_1,q') in delta_i
        Key sym = it->from_stack();
        //look for internal transitions in the NWA with symbol 'sym'
        for( internalIterator iit = beginInternalTrans();
              iit != endInternalTrans(); iit++ )
        {
          if( sym == Trans::getInternalSym(*iit) )
          {
            //<(p,q),n_1> -w-> <(p',q'),n_2>
            Key from = getKey(it->from_state(),Trans::getSource(*iit));
            Key to = getKey(it->to_state(),Trans::getTarget(*iit));
            result.add_rule(from,sym,to,it->to_stack1(),it->weight());
          }
        }
      }

      //Call Rules:
      for( std::set<wpds::Rule>::iterator it = rules.pushRules.begin();
            it != rules.pushRules.end(); it++ )
      {
        //<p,n_c> -w-> <p',e r_c> in delta_3, (q_c,n_c,q) in delta_c
        Key sym = it->from_stack();
        //look for call transitions in the NWA with symbol 'sym'
        for( callIterator cit = beginCallTrans();
              cit != endCallTrans(); cit++ )
        {
          if( sym == Trans::getCallSym(*cit) )
          {
            //<(p,q_c),n_c> -w-> <(p',q),e (r_c,q_c)>
            Key from = getKey(it->from_state(),Trans::getCallSite(*cit));
            Key to = getKey(it->to_state(),Trans::getEntry(*cit));
            Key stack2 = getKey(it->to_stack2(),Trans::getCallSite(*cit));
            result.add_rule(from,sym,to,it->to_stack1(),stack2,it->weight());
          }
        }
      }

      //Return Rules:
      for( std::set<wpds::Rule>::iterator it = rules.popRules.begin();
            it != rules.popRules.end(); it++ )
      {
        //<p,x> -w-> <p',*> in delta_0, (q_r,q_c,x,q) in delta_r
        Key sym = it->from_stack();
        //look for return transitions in the NWA with symbol 'sym'
        for( returnIterator rit = beginReturnTrans();
              rit != endReturnTrans(); rit++ )
        {
          if( sym == Trans::getReturnSym(*rit) )
          {
            //<(p,q_r),x> -w-> <(p',q_r^x),*>
            Key from = getKey(it->from_state(),Trans::getExit(*rit));
            Key to = getKey(it->to_state(),getKey(Trans::getExit(*rit),sym));
            result.add_rule(from,sym,to,it->weight());
            //For all r_c in delta_2
            for( std::set<wpds::Rule>::iterator dit = rules.pushRules.begin();
                  dit != rules.pushRules.end(); dit++ )
            {
              //<(p',q_r^x),(r_c,q_c)> -1-> <(p',q),r_c>
              from = getKey(it->to_state(),getKey(Trans::getExit(*rit),sym));
              sym = getKey(dit->to_stack2(),Trans::getCallSite(*rit));
              to = getKey(it->to_state(),Trans::getReturnSite(*rit));
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
     * This method constructs the NWA that is equivalent to the given PDS.
     *
     * @param - pds: the pds to convert 
     * @return the NWA equivalent to the given PDS
     *
     */
    template <typename Client>
    void NWA<Client>::PDStoNWA( const wpds::WPDS & pds )
    {
      //TODO: check this!

      //Make sure we can modify transitions in this machine.
      assert(stuck);

      std::map<St,St> call_return;

      wpds::WpdsRules rules = wpds::WpdsRules();
      pds.for_each(rules);

      //Step Rules:
      for( std::set<wpds::Rule>::iterator it = rules.stepRules.begin();
            it != rules.stepRules.end(); it++ )
      {
        //<p,n_1> -w-> <p',n_2> in delta_1 goes to ((p,n_1),n_1,(p',n_2)) in delta_i
        addInternalTrans(getKey(it->from_state(),it->from_stack()), //from
                         it->from_stack(),                          //sym
                         getKey(it->to_state(),it->to_stack1()));   //to
      }

      //Call Rules:
      for( std::set<wpds::Rule>::iterator it = rules.pushRules.begin();
            it != rules.pushRules.end(); it++ )
      {
        //<p,n_c> -w-> <p',e r_c> in delta_2, ((p,n_c),c,(p',e)) in delta_c
        addCallTrans(getKey(it->from_state(),it->from_stack()),   //from
                      it->from_stack(),                           //sym
                      getKey(it->to_state(),it->to_stack1()));    //to
        //add (p,n_c) => r_c to the call-return map
        call_return.insert(std::pair<St,St>(getKey(it->from_state(),it->from_stack()),  //call
                                            it->to_stack2()));                          //ret
      }

      //Return Rules:
      for( std::set<wpds::Rule>::iterator it = rules.popRules.begin();
            it != rules.popRules.end(); it++ )
      {
        //<p,x> -w-> <p',*> in delta_0, ((p,x),(p,n_c),x,(p',r)) in delta_r
        for( std::map<St,St>::iterator cr_it = call_return.begin();
              cr_it != call_return.end(); cr_it++ )
        {
          addReturnTrans(getKey(it->from_state(),it->from_stack()),   //from
                          cr_it->first,                               //pred
                          it->from_stack(),                           //sym
                          getKey(it->to_state(),cr_it->second));      //to
        }
      }
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
    wpds::WPDS NWA<Client>::NWAtoPDSreturns( WeightGen<Client> & wg ) const
    {
      //TODO: beware the implicit transitions
      //Q: do we need to make all transitions explicit in order to make this correct?

      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  //= wali::getKey("program");

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( internalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
      {      
        //(q,sigma,q') in delta_i goes to <p,q> -w-> <p,q'> in delta_1
        //where the weight w depends on sigma

	    St src = Trans::getSource(*iit);
		  St tgt = Trans::getTarget(*iit);
      
      if( isWild(Trans::getInternalSym(*iit)) )
		    wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
      else
        wgt = wg.getWeight(src, getClientInfo(src),
                            Trans::getInternalSym(*iit),
                            WeightGen<Client>::INTRA,
                            tgt, getClientInfo(tgt));  //w

        result.add_rule(program,                       //from_state (p)
                        src,									         //from_stack (q)
                        program,                       //to_state (p)
                        tgt,									         //to_stack1 (q')
                        wgt);                          //weight  (w)      
      }

      //Call Transitions 
      for( callIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
      {
        St src = Trans::getCallSite(*cit);
		    St tgt = Trans::getEntry(*cit);
        for( returnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
        {
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            //(q_c,sigma,q_e) in delta_c and (q_x,q_c,*,q_r) in delta_r goes to
            // <p,q_c> -w-> <p,q_e q_r> in delta_2 and w depends on sigma

            if( isWild(Trans::getCallSym(*cit)) )
              wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); // w
            else
              wgt = wg.getWeight(src, getClientInfo(src),
                                  Trans::getCallSym(*cit),
                                  WeightGen<Client>::CALL_TO_ENTRY,  
                                  tgt, getClientInfo(tgt)); // w   

            result.add_rule(program,                      //from_state (p)
                            src,     //from_stack (q_c)
                            program,                      //to_state (p)
                            tgt,        //to_stack1 (q_e)
                            Trans::getReturnSite(*rit),   //to_stack2 (q_r)
                            wgt);                         //weight  (w)    
          }
        }
      }

      //Return Transitions
      for( returnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
      {
        //(q_x,q_c,*,q_r) in delta_r goes to
        // <p,q_x> -w-> <p_q_xcr,epsilon> in delta_0
        // and <p_q_xcr,q_r> -1-> <p,q_r> in delta_1
        // where p_q_xcr = (p,q_x,q_c,q_r) and w depends on sigma

        St src = Trans::getExit(*rit);
        St tgt = Trans::getReturnSite(*rit);
        
        if( isWild(Trans::getReturnSym(*rit)) )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w  
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getReturnSym(*rit),
                              WeightGen<Client>::EXIT_TO_RET,
                              tgt, getClientInfo(tgt));  // w            

        Key rstate = getControlLocation(src,Trans::getCallSite(*rit),tgt);    //p_q_xcr

        result.add_rule(program,                    //from_state (p)
                        src,                        //from_stack (q_x)
                        rstate,                     //to_state (p_q_xcr == (p,q_x,q_c,q_c))
                        wgt);                       //weight  (w)

        wgt = wg.getOne();                          // 1

        result.add_rule(rstate,                     //from_state (p_q_xcr == (p,q_x,q_c,q_r))
                        tgt,                        //from_stack (q_r)
                        program,                    //to_state (p)
                        tgt,                        //to_stack (q_r)
                        wgt);                       //weight  (1)   
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
    wpds::WPDS NWA<Client>::NWAtoBackwardsPDSreturns( WeightGen<Client> & wg ) const
    {
      //TODO: beware the implicit transitions
      //Q: do we need to make all transitions explicit in order to make this correct?

      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  //= wali::getKey("program");

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( internalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
      {      
        //(q,sigma,q') in delta_i goes to <p,q'> -w-> <p,q> in delta_1
        //where the weight w depends on sigma

        St src = Trans::getSource(*iit);
        St tgt = Trans::getTarget(*iit);
        
        if( isWild(Trans::getInternalSym(*iit)) )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getInternalSym(*iit),
                              WeightGen<Client>::INTRA,
                              tgt, getClientInfo(tgt));          // w

        result.add_rule(program,                                //from_state (p)
                        tgt,                                    //from_stack (q')
                        program,                                //to_state (p)
                        src,                                    //to_stack1 (q)
                        wgt);                                   //weight (w)      
      }

      //Call Transitions 
      for( callIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
      {
        St src = Trans::getCallSite(*cit);
        St tgt = Trans::getEntry(*cit);
        for( returnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            //(q_c,sigma,q_e) in delta_c and (q_x,q_c,*,q_r) in delta_r goes to
            // <p,q_e> -w-> <p_q_erc,epsilon> in delta_0
            // and <p_q_erc,q_c> -1-> <p,q_c> in delta_1
            // where p_q_erc = (p,q_e,q_r,q_c) and w depends on sigma

            if( isWild(Trans::getCallSym(*cit)) )
              wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); // w
            else
              wgt = wg.getWeight(src, getClientInfo(src), 
                                  Trans::getCallSym(*cit),
                                  WeightGen<Client>::CALL_TO_ENTRY,  
                                  tgt, getClientInfo(tgt)); // w    

            Key cstate = getControlLocation(tgt,Trans::getReturnSite(*rit),src);    //p_q_erc

            result.add_rule(program,                    //from_state (p)
                            Trans::getEntry(*cit),      //from_stack (q_e)
                            cstate,                     //to_state (p_q_erc == (p,q_e,q_r,q_c))
                            wgt);                       //weight (w)

            wgt = wg.getOne();                          // 1
            
            result.add_rule(cstate,                     //from_state (p_q_erc == (p,q_e,q_r,q_c))
                            src,                        //from_stack (q_c)
                            program,                    //to_state (p)
                            src,                        //to_stack (q_c)
                            wgt);                       //weight (1)    
          }  
      }

      //Return Transitions
      for( returnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
      {
        //(q_x,q_c,*,q_r) in delta_r goes to
        // <p,q_r> -w-> <p,q_x q_c> in delta_2 
        // and w depends on sigma

        St src = Trans::getExit(*rit);
        St tgt = Trans::getReturnSite(*rit);
        
        if( isWild(Trans::getReturnSym(*rit)) )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getReturnSym(*rit),
                              WeightGen<Client>::EXIT_TO_RET,
                              tgt, getClientInfo(tgt));  // w

        result.add_rule(program,                      //from_state (p)
                        tgt,                          //from_stack (q_r)
                        program,                      //to_state (p)
                        src,                          //to_stack1 (q_x)
                        Trans::getCallSite(*rit),     //to_stack2 (q_c)
                        wgt);                         //weight (w)      
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
    wpds::WPDS NWA<Client>::NWAtoPDScalls( WeightGen<Client> & wg ) const
    {
      //TODO: beware the implicit transitions
      //Q: do we need to make all transitions explicit in order to make this correct?

      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  // = wali::getKey("program");

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( internalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
      {  
        // (q,sigma,q') in delta_i goes to <p,q> -w-> <p,q'> in delta_1
        // where the weight w depends on sigma

        St src = Trans::getSource(*iit);
        St tgt = Trans::getTarget(*iit);
        
        if( isWild(Trans::getInternalSym(*iit)) )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getInternalSym(*iit),
                              WeightGen<Client>::INTRA,
                              tgt, getClientInfo(tgt));           // w

          result.add_rule(program,                                //from_state (p)
                          src,                                    //from_stack (q)
                          program,                                //to_state (p)
                          tgt,                                    //to_stack1 (q')
                          wgt);                                   //weight (w)      
      }

      //Call Transitions
      for( callIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
      {           
        // (q_c,sigma,q_e) in delta_c goes to
        // <p,q_c> -w-> <p,q_e q_c> in delta_2 
        // and the weight w depends on sigma

        St src = Trans::getCallSite(*cit);
        St tgt = Trans::getEntry(*cit);
        
        if( isWild(Trans::getCallSym(*cit)) )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); // w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getCallSym(*cit),
                              WeightGen<Client>::CALL_TO_ENTRY,
                              tgt, getClientInfo(tgt));          // w

        result.add_rule(program,                                //from_state (p)
                        src,                                    //from_stack (q_c)
                        program,                                //to_state (p)
                        Trans::getEntry(*cit),                  //to_stack1 (q_e)
                        src,                                    //to_stack2 (q_c)
                        wgt);                                   //weight (w)  
      } 

      //Return Transitions
      int r_count = 0;
      for( returnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
      {
        ++r_count;
          
        //std::cerr << "Return transition #" << r_count << " of " << trans.size() << "\n";
        // (q_x,q_c,sigma,q_r) in delta_r goes to 
        // <p,q_x> -w-> <p_q_xcr,epsilon> in delta_0
        // and <p_q_xcr,q_c> -1-> <p,q_r> in delta_1
        // where p_q_xcr = (p,q_x,q_c,q_r), and w depends on sigma

        St src = Trans::getExit(*rit);
        St tgt = Trans::getReturnSite(*rit);
        
        if( isWild(Trans::getReturnSym(*rit)) )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w 
        else
          wgt = wg.getWeight(src, getClientInfo(src), 
                              Trans::getReturnSym(*rit),
                              WeightGen<Client>::EXIT_TO_RET,  
                              tgt, getClientInfo(tgt));    // w     

        //Note: if you change this, make sure you modify the code in NWPForest.createCA()
        Key rstate = getControlLocation(src,Trans::getCallSite(*rit),tgt);  //p_q_xcr

        result.add_rule(program,                              //from_state (p)
                        src,                                  //from_stack (q_x)
                        rstate,                               //to_state (p_q_xcr == (p,q_x,q_c,q_r))
                        wgt);                                 //weight (w)

        
        wgt = wg.getOne();                                    // 1                      
         
        result.add_rule(rstate,                               //from_state (p_q_xcr == (p,q_x,q_c,q_r))
                        Trans::getCallSite(*rit),             //from_stack (q_c)
                        program,                              //to_state (p)
                        tgt,                                  //to_stack (q_r)
                        wgt);                                 //weight (1)
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
    wpds::WPDS NWA<Client>::NWAtoBackwardsPDScalls( WeightGen<Client> & wg ) const
    {
      //TODO: beware the implicit transitions
      //Q: do we need to make all transitions explicit in order to make this correct?

      wpds::WPDS result = wpds::WPDS();

      Key program = getProgramControlLocation();  // = wali::getKey("program"); 

      wali::sem_elem_t wgt;

      //Internal Transitions
      for( internalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
      {  
        // (q,sigma,q') in delta_i goes to <p,q'> -w-> <p,q> in delta_1
        // where the weight w depends on sigma

        St src = Trans::getSource(*iit);
        St tgt = Trans::getTarget(*iit);
        
        if( isWild(Trans::getInternalSym(*iit)) )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getInternalSym(*iit),
                              WeightGen<Client>::INTRA,
                              tgt, getClientInfo(tgt));         // w

        result.add_rule(program,                                //from_state (p)
                        tgt,                                    //from_stack (q')
                        program,                                //to_state (p)
                        src,                                    //to_stack1 (q)
                        wgt);                                   //weight (w)     
      }

      //Call Transitions
      for( callIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
      {    
        St src = Trans::getCallSite(*cit);
        St tgt = Trans::getEntry(*cit);
        for( returnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
          if( Trans::getCallSite(*cit) == Trans::getCallSite(*rit) )
          {
            // (q_c,sigma,q_e) in delta_c and (q_x,q_c,*,q_r) in delta_r goes to
            // <p,q_e> -w-> <p_q_erc,epsilon> in delta_0
            // and <p_q_erc,q_r> -1-> <p,q_c> in delta_1
            // where p_q_erc = (p,q_e,q_r,q_c) and w depends on sigma
            
            if( isWild(Trans::getCallSym(*cit)) )
              wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); // w  
            else
              wgt = wg.getWeight(src, getClientInfo(src),
                                  Trans::getCallSym(*cit),
                                  WeightGen<Client>::CALL_TO_ENTRY,
                                  tgt, getClientInfo(tgt));         // w                  

            Key cstate = getControlLocation(tgt,Trans::getReturnSite(*rit),src);  //p_q_erc

            result.add_rule(program,                              //from_state (p)
                            tgt,                                  //from_stack (q_e)
                            cstate,                               //to_state (p_q_erc == (p,q_e,q_r,q_c))
                            wgt);                                 //weight (w)  

            wgt = wg.getOne();                                    // 1

            result.add_rule(cstate,                               //from_state (p_q_erc == (p,q_e,q_r,q_c))
                            Trans::getReturnSite(*rit),           //from_stack (q_r)
                            program,                              //to_state (p)
                            src,                                  //to_stack (q_c)
                            wgt);                                 //weight (1)    
          }
      } 

      //Return Transitions
      for( returnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
      {
        // (q_x,q_c,sigma,q_r) in delta_r goes to 
        // <p,q_r> -w-> <p,q_x q_r> in delta_2 
        // and the weight w depends on sigma

        St src = Trans::getExit(*rit);
        St tgt = Trans::getReturnSite(*rit);

        if( isWild(Trans::getReturnSym(*rit)) )
          wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); //w
        else
          wgt = wg.getWeight(src, getClientInfo(src),
                              Trans::getReturnSym(*rit),
                              WeightGen<Client>::EXIT_TO_RET,  
                              tgt, getClientInfo(tgt));      //w
         
        result.add_rule(program,                                //from_state (p)
                        tgt,                                    //from_stack (q_r)
                        program,                                //to_state (p)
                        src,                                    //to_stack1 (q_x)
                        tgt,                                    //to_stack2 (q_r)
                        wgt);                                   //weight (w)  
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

      //An automaton with an initial state that is a final state is definitely not empty.
      for( stateIterator it = beginInitialStates(); it != endInitialStates(); it++ )
      {
        if( isFinalState(*it) )
          return false;
      }

      //Any final state reachable from an initial state shows this NWA is not empty.

      Reach wgts = Reach(true);

      //Make a WFA with transitions from initial state to final state for each 
      //initial state of the NWA.
      wali::wfa::WFA initials;
      //Set up initial state.
      wali::Key init = getProgramControlLocation();
      initials.addState(init,wgts.zero());
      initials.setInitialState(init);
      //Set up final state.
      wali::Key fin = getKey("fin");
      initials.addState(fin,wgts.zero());
      initials.addFinalState(fin);
      //Add transitions.
      for( stateIterator it = beginInitialStates(); it != endInitialStates(); it++ )
      {
        initials.addTrans(init,*it,fin,wgts.one());
      }

      //Make a WFA with transitions from initial state to final state for each
      //final state of the NWA.
      wali::wfa::WFA finals;
      //Set up initial state.
      finals.addState(init,wgts.zero());
      finals.setInitialState(init);
      //Set up final state.
      finals.addState(fin,wgts.zero());
      finals.addFinalState(fin);
      //Add transitions.
      for( stateIterator it = beginFinalStates(); it != endFinalStates(); it++ )
      {
        finals.addTrans(init,*it,fin,wgts.one());
      }

#if 1
      ReachGen<Client> reach;
      //Perform poststar to determine reachability.
      std::cerr << "Before poststar\n";
      wali::wfa::WFA postInitials = poststar(initials,reach);
      std::cerr << "After poststar!\n";

      //intersect poststar(initials) and finals
      std::cerr << "Before intersect\n";
      wali::wfa::WFA reachable = postInitials.intersect(finals);
      std::cerr << "After intersect!\n";
#else
      //Perform poststar to determine reachability.
      ReachGen<Client> reach;
      std::cerr << "Before prestar\n";
      wali::wfa::WFA preFinals = prestar(finals,reach);
      std::cerr << "After prestar!\n";

      //intersect poststar(initials) and finals
      std::cerr << "Before WFA intersect\n";
      wali::wfa::WFA reachable = preFinals.intersect(initials);
      std::cerr << "After WFA intersect!\n";
#endif

      //If there are no transitions in this intersection, then the NWA is empty.
      wali::wfa::TransCounter tc = wali::wfa::TransCounter();
      reachable.for_each( tc );
      
      if( tc.getNumTrans() == 0 )
        return true;  //There was no path from an initial state to a final state in the NWA.
      else
        return false; //There was a path from an initial state to a final state in the NWA.
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
    wfa::WFA NWA<Client>::prestar( wfa::WFA & input, WeightGen<Client> & wg )
    {
      //Q: does anything need to be done to transform the resulting WFA from the 
      //    PDS vocab back to the NWA vocab?
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
    void NWA<Client>::prestar( wfa::WFA & input, wfa::WFA & output, WeightGen<Client> & wg )
    {
      //Q: does anything need to be done to transform the resulting WFA from the 
      //    PDS vocab back to the NWA vocab?
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
    wfa::WFA NWA<Client>::poststar( wfa::WFA & input, WeightGen<Client> & wg )
    {
      //Q: does anything need to be done to transform the resulting WFA from the 
      //    PDS vocab back to the NWA vocab?
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
    void NWA<Client>::poststar( wfa::WFA & input, wfa::WFA & output, WeightGen<Client> & wg )
    {
      //Q: does anything need to be done to transform the resulting WFA from the 
      //    PDS vocab back to the NWA vocab?
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
      //TODO: flag for explicit stuck state transition printing?
      states.print(o);
      symbols.print(o);
      trans.print(o);
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
      //TODO: flag for explicit stuck state transition printing?
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
      std::set<St> initials = getInitialStates();
      for( typename std::set<St>::const_iterator it = initials.begin(); it != initials.end(); it++ )
      {
        printKey(o << "\"",*it,true)<<"\" [ style=bold ]";
      }

      //final states
      std::set<St> finals = getFinalStates();
      for( typename std::set<St>::const_iterator it = finals.begin(); it != finals.end(); it++ ) 
      {
        printKey(o << "\"",*it,true) <<"\" [ peripheries=2 ]";
      }

      //Print call transitions.
      o << "//Delta_c: \n";
      callIterator cit = beginCallTrans();
      callIterator citEND = endCallTrans();
      for( bool first=true; cit != citEND; cit++ )
      {
        printKey(o << "\"",Trans::getCallSite(*cit),true) << "\"";
        o << "->";
        printKey(o << "\"",Trans::getEntry(*cit),true) << "\"";
        o << "[";
        o << " label=\"";
        printKey(o,Trans::getCallSym(*cit),true);
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
        printKey(o << "\"",Trans::getSource(*iit),true) << "\"";
        o << "->";
        printKey(o << "\"",Trans::getTarget(*iit),true) << "\"";
        o << "[";
        o << " label=\"";
        printKey(o,Trans::getInternalSym(*iit),true);
        o << "\"";
        o << "];\n";
      }
      o << " \n";


      //Print return transitions. (This is so dumbly done)
      o << "// Delta_r:\n";
      for(stateIterator exitit = beginStates(); exitit != endStates(); ++exitit)
      {
        for(stateIterator returnit = beginStates(); returnit != endStates(); ++returnit)
        {
          // For each Sym, holds the set of call predecessors for which
          // (exit, pred, sym, return) is in delta_r
          std::map<Sym, std::set<St> > returns;

          // Populate it:
          for(returnIterator trans = beginReturnTrans(); trans != endReturnTrans(); ++trans)
          {
            if(*exitit == Trans::getExit(*trans)
               && *returnit == Trans::getReturnSite(*trans))
            {
              returns[Trans::getReturnSym(*trans)].insert(Trans::getCallSite(*trans));
            }
          }

          // Now add an edge for each return symbol
          for(typename std::map<Sym, std::set<St> >::const_iterator trans = returns.begin();
              trans != returns.end(); ++trans)
          {
            std::stringstream ss;
            printKey(ss, trans->first,true);
            ss << ";{";
            for(typename std::set<St>::const_iterator prediter = trans->second.begin();
                prediter != trans->second.end(); ++prediter)
            {
              printKey(ss, *prediter, true) << ";";
            }
            ss << ";";

            // Now ss holds the label for the edge exit->return
            printKey(o << "\"", *exitit, true) << "\" ->";
            printKey(o << "\"", *returnit, true) << "\"";
            o << "[ label=\"" << ss.str() << "\" color=red];\n";
          }
        }
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
      return trans.beginCall();
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
      return trans.endCall();
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
      return trans.beginInternal();
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
      return trans.endInternal();
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
      return trans.beginReturn();
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
      return trans.endReturn();
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
      Internals reachable = trans.getInternals(st,getEpsilon());
      for( typename Internals::iterator it = reachable.begin(); it != reachable.end(); it++ )
      {
        St newSt = Trans::getTarget(*it);
        //Check that we haven't seen this pair already.
        if( newPairs->count(newSt) == 0 )
        {
          //Add a new pair.
          newPairs->insert(newSt);
          //Recursively explore.
          epsilonClosure(newPairs,newSt); 
        }
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
      //Compute the cross produce of all states reachable from sp via epsilon transitions.

      //Explore epsilon transitions reachable from the first component of sp.
      Internals reachable = first->trans.getInternals(sp.first,getEpsilon());
      for( typename Internals::iterator it = reachable.begin(); it != reachable.end(); it++ )
      {
        StatePair newSP = StatePair(Trans::getTarget(*it),sp.second);
        //Check that we haven't seen this pair already.
        if( newPairs->count(newSP) == 0 )
        {
          //Add a new pair.
          newPairs->insert(newSP);
          //Recursively explore.
          epsilonClosure(newPairs,newSP,first,second); 
        }
      }

      //Explore epsilon transitions reachable from the second component of sp.
      reachable = second->trans.getInternals(sp.second,getEpsilon());
      for( typename Internals::iterator it = reachable.begin(); it != reachable.end(); it++ )
      {
        StatePair newSP = StatePair(sp.first,Trans::getTarget(*it));
        //Check that we haven't seen this pair already.
        if( newPairs->count(newSP) == 0 )
        {
          //Add a new pair.
          newPairs->insert(newSP);
          //Recursively explore.
          epsilonClosure(newPairs,newSP,first,second);
        }
      }
    }

    template<typename Client>
    std::ostream& NWA<Client>::marshall( std::ostream& os ) const
    {
      os << "<" << XMLTag() << ">\n";

      for(symbolIterator sym = beginSymbols(); sym != endSymbols(); ++sym)
      {
          os << "    <" << SymbolSet::XMLSymbolTag() << " " << SymbolSet::XMLNameAttr() << "=\"" << key2str(*sym) << "\"/>\n";
      }

      for(stateIterator state = beginStates(); state != endStates(); ++state)
      {
          os << "    <" << States::XMLStateTag() << " ";
          os << States::XMLNameAttr() << "=\"" << key2str(*state) << "\"";
          if(isInitialState(*state)) os << " " << States::XMLInitialAttr() << "=\"TRUE\"";
          if(isFinalState(*state)) os << " " << States::XMLFinalAttr() << "=\"TRUE\"";
          os << "/>\n";
      }

      for(callIterator call = trans.beginCall(); call != trans.endCall(); ++call)
      {
          os << "    <" << Trans::CallXMLTag() << " ";
          os << Trans::XMLFromAttr() <<"=\"" << key2str(call->first) << "\" ";
          os << Trans::XMLSymbolAttr() << "=\"" << key2str(call->second) << "\" ";
          os << Trans::XMLToAttr() << "=\"" << key2str(call->third) << "\"/>\n";
      }
      for(internalIterator internal = trans.beginInternal(); internal != trans.endInternal(); ++internal)
      {
          os << "    <" << Trans::InternalXMLTag() << " ";
          os << Trans::XMLFromAttr() <<"=\"" << key2str(internal->first) << "\" ";
          os << Trans::XMLSymbolAttr() << "=\"" << key2str(internal->second) << "\" ";
          os << Trans::XMLToAttr() << "=\"" << key2str(internal->third) << "\"/>\n";
      }
      for(returnIterator return_ = trans.beginReturn(); return_ != trans.endReturn(); ++return_)
      {
          os << "    <" << Trans::ReturnXMLTag() << " ";
          os << Trans::XMLFromAttr() <<"=\"" << key2str(return_->first) << "\" ";
          os << Trans::XMLPredAttr() <<"=\"" << key2str(return_->second) << "\" ";
          os << Trans::XMLSymbolAttr() << "=\"" << key2str(return_->third) << "\" ";
          os << Trans::XMLToAttr() << "=\"" << key2str(return_->fourth) << "\"/>\n";
      }


      os << "</" << XMLTag() << ">\n";

      return os;
    }
  }
}
#endif
