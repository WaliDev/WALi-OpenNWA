#ifndef wali_nwa_StateSet_GUARD
#define wali_nwa_StateSet_GUARD 1

/**
 * @author Amanda Burton
 */


// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/nwa/State.hpp"

// std::c++
#include <iostream>
#include <set>
#include <map>

namespace wali
{
  namespace nwa
  {
    /**
     *
     *  This class is used to keep track of the states of an NWA.
     *  
     */
#ifndef KEY
    template <typename St,typename StName> 
    class StateSet : public Printable
    {
      public:
        typedef typename std::set<StName>::const_iterator iterator;   
        typedef std::map<StName,St*> StMap;   //Names to states mapping.

      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      StateSet( );
      StateSet( StateSet & other );
      StateSet & operator=( const StateSet & other );

      ~StateSet( );

      //State Accessors

      /**
       *
       * @brief removes all states 
       *
       * This method removes all states from this collection.  It also
       * removes all initial states and final states.
       *
       */
      void clearStates( );
        
      /**
       *
       * @brief removes all initial states
       *
       * This method removes all states from the initial state set, but
       * does not remove them from the state set.
       *
       */
      void clearInitialStates( );
        
      /**
       *
       * @brief removes all final states
       *
       * This method removes all states from the final state set, but
       * does not remove them from the state set.
       *
       */
      void clearFinalStates( );
      
      /**
       * 
       * @brief tests whether the given state is a member of this collection
       *
       * This method determines whether the given state is a member of
       * this collection.  It returns true if the state is a member and
       * false otherwise.
       *
       * @param - state: the state to test
       * @return true if the state is a member of this collection of
       *         states, false otherwise
       *
       */
      bool isState( const St * state ) const;
        
      /**
       * 
       * @brief tests whether the given state is an initial state of this 
       *        collection
       *
       * This method determines whether the given state is an initial state
       * of this collection.  It returns true if the state is an initial state
       * and false otherwise.
       *
       * @param - initialState: the state to test
       * @return true if the state is an initial state of this collection,
       *         false otherwise
       *
       */
      bool isInitialState( const St * initialState ) const;
        
      /**
       * 
       * @brief tests whether the given state is a final state of this
       *        collection
       *
       * This method determines whether the given state is a final state
       * of this collection.  It returns true if the state is a final state
       * and false otherwise.
       *
       * @param - finalState: the state to test
       * @return true if the state is a final state of this collection,
       *         false otherwise
       *
       */
      bool isFinalState( const St * finalState ) const;
      
      /**
       *
       * @brief add the given state 
       *
       * This method adds the given state.  If the state already exists, 
       * false is returned.  Otherwise, true is returned.
       *
       * @param - state: the state to add 
       * @return false if the state already exists, true otherwise
       *
       */
      bool addState( St * state );
        
      /**
       *
       * @brief add the given initial state 
       *
       * This method adds the given state to this collection of 
       * states(if it does not already exist).  The given initial state
       * is then added to the initial states.  If the state is already an
       * initial state, false is returned.  Otherwise, true is returned.
       *
       * @param - initialState: the initial state to add 
       * @return false if the state is already an initial state, true otherwise
       *
       */
      bool addInitialState( St * initialState );
        
      /**
       *
       * @brief add the given final state 
       *
       * This method adds the given state to this collection of 
       * states(if it does not already exist).  The given final state
       * is then added to the final states.  If the state is already a
       * final state, false is returned.  Otherwise, true is returned.
       *
       * @param - finalState: the final state to add 
       * @return false if the state is already a final state, true otherwise
       *
       */
      bool addFinalState( St * finalState );
      
      /**
       * 
       * @brief add all states in the given collection to this
       *        collection of states
       *
       * This method adds all of the states in the given collection
       * of states to this collection of states.
       *
       * @param - stateSet: the collection of states to add to this collection
       *                    of states
       *
       */
      void addAll( StateSet<St,StName> stateSet );
        
      /**
       *
       * @brief add all the states in the given StateSet 
       *
       * This method adds all of the given states to the state set.
       *
       * @param - stateSet: the StateSet that contains the states to add
       *
       */
      void addAllStates( StateSet<St,StName> stateSet );
        
      /**
       * 
       * @brief add all the initial states in the given StateSet
       *
       * This method adds all of the given initial states to the
       * initial state set (and thus to the state set if they are 
       * not already elements of the state set).
       *
       * @param - stateSet: the StateSet that contains the states to add
       *
       */
      void addAllInitialStates( StateSet<St,StName> stateSet );
        
      /**
       * 
       * @brief add all the final states in the given StateSet
       *
       * This method adds all of the given final states to the 
       * final state set (and thus to the state set if they are 
       * not already elements of the state set).
       *
       * @param - stateSet: the StateSet that contains the states to add
       *
       */
      void addAllFinalStates( StateSet<St,StName> stateSet );
      
      /**
       * 
       * @brief remove the given state 
       *
       * This method removes the given state.  If the state does not
       * exist, false is returned.  Otherwise, true is returned.
       * Note: If the given state is an initial state or a final state,
       * it is also removed from that set.
       *
       * @param - state: the state to remove
       * @return false if this state does not exist, true otherwise
       *
       */
      bool removeState( const St * state );
        
      /**
       * 
       * @brief remove the given initial state
       *
       * This method removes the given initial state.  If the state
       * is not an initial state, false is returned.  Otherwise, true 
       * is returned.
       * Note: The state is not removed from the state set.
       *
       * @param - initialState: the initial state to remove
       * @return false if this state is not an initial state, true otherwise
       *  
       */
      bool removeInitialState( const St * initialState );
        
      /**
       * 
       * @brief remove the given final state
       *
       * This method removes the given final state.  If the state
       * is not a final state, false is returned.  Otherwise, true
       * is returned.
       * Note: The state is not removed from the state set.
       *
       * @param - finalState: the final state to remove
       * @remove false if this state is not a final state, true otherwise
       *
       */
      bool removeFinalState( const St * finalState );
      
      //Utilities	

      /**
       *
       * @brief print the collection of states
       *
       * This method prints out the state set to the output stream 
       * provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to
       *
       */
      std::ostream & print( std::ostream & o ) const;

      /**
       *
       * @brief tests whether this collection of states is equivalent 
       *        to the collection of states 'other'
       *
       * This method tests the equivalence of this set of states and 
       * the set of states 'other'.
       *
       * @param - other: the StateSet to compare this StateSet to
       * @return true if this StateSet is equivalent to the StateSet 
       *         'other'
       *
       */
      bool operator==( const StateSet<St,StName> & other ) const;
       
      /**
       * 
       * @brief provides access to the states in the collection 
       *
       * This method provides access to the states in this collection 
       * through an iterator.
       *
       * @return the starting point of an iterator through the states
       *
       */
      iterator beginStates( ) const;
        
      /**
       * 
       * @brief provides access to the initial states in the collection
       *
       * This method provides access to the initial states in this 
       * collection through an iterator.
       *
       * @return the starting point of an iterator through the initial states
       *
       */
      iterator beginInitialStates( ) const;
        
      /**
       *
       * @brief provides access to the final states in the collection
       *
       * This method provides access to the final states in this
       * collection through an iterator.
       *
       * @return the starting point of an iterator through the final states
       *
       */
      iterator beginFinalStates( ) const;
        
      /**
       * 
       * @brief provides access to the states in the collection 
       *
       * This method provides access to the states in the collection 
       * through an iterator.
       *
       * @return one place past the exit point of an iterator through 
       *         the states
       *
       */
      iterator endStates( ) const;
        
      /**
       * 
       * @brief provides access to the initial states in the collection
       *
       * This method provides access to the initial states in the collection
       * through an iterator.
       *
       * @return one place past the exit point of an iterator through the
       *         initial states
       *
       */
      iterator endInitialStates( ) const;
        
      /**
       * 
       * @brief provides access to the final states in the collection
       *
       * This method provides access to the final states in the collection
       * through an iterator.
       *
       * @return one place past the exit point of an iterator through the
       *         final states
       *
       */
      iterator endFinalStates( ) const;
      
      /**
       * 
       * @brief provides access to all states in the collection
       *
       * This method provides access to all states in this collection 
       * in the form of a set of states.
       *
       * @return a set containing all states in this collection
       *
       */
      std::set<St*> getStates( ) const;

      /**
       * 
       * @brief provides access to the names of all the states in the 
       *        collection
       *
       * This method provides access to the names of all the states
       * in the collection in the form of a set of state names.
       *
       * @return a set containing the names of all states in the 
       *         collection
       *
       */
      std::set<StName> getStateNames( ) const;
             
      /**
       * 
       * @brief provides access to the names of all the states in the
       *        given set of states
       *
       * This method provides access to the names of all the states in 
       * the given set of states in the form of a set of state names.
       *
       * @param - sts: the states whose names to return
       * @return the names correcponding to the given states
       *
       */
      std::set<StName> getStateNames( const std::set<St> & sts ) const;
     
      /**
       *  
       * @brief provides access to the names of all the initial states
       *        in the collection
       *
       * This method provides access to the names of all the initial
       * states in the collection in the form of a set of state names.
       *
       * @return a set containing the names of all initial states in
       *         the collection
       *
       */
      std::set<StName> getInitialStateNames( ) const;
        
      /**
       * 
       * @brief provides access to the names of all final states in 
       *        the collection
       *
       * This method provides access to the names of all the final 
       * states in the collection in the form of a set of state names.
       *
       * @return a set containing the names of all final states in 
       *         the collection
       *
       */
      std::set<StName> getFinalStateNames( ) const;

      /**
       * 
       * @brief returns the number of states in this collection
       * 
       * This method returns the number of states in this collection.
       *
       * @return the number of states in this collection
       *
       */
      size_t sizeStates( ) const;
      
      /**
       * 
       * @brief returns the number of initial states in this collection
       *
       * This method returns the number of initial states in this collection.
       *
       * @return the number of initial states in this collection
       *
       */
      size_t sizeInitialStates( ) const;
      
      /**
       * 
       * @brief returns the number of final states in this collection
       * 
       * This method returns the number of final states in this collection.
       *
       * @return the number of final states in this collection
       *
       */
      size_t sizeFinalStates( ) const;     
      
      /**
       *  
       * @brief gives 'dup' all the state properties of 'orig'
       *
       * This method checks all the state properties (initial/final)
       * of 'orig' and assigns the same properties to 'dup'.
       *
       * @param - orig: the state whose properties to duplicate
       * @param - dup: the state whose properties are being set
       *
       */
      void dupState( const St * orig, St * dup );

      /**
       *  TODO: make sure that the mapping is unique in it's assignment of
       *        a state to a name
       * @brief provide access to the state with the given name
       *
       * This method provides access to the state in this collection
       * that has the given name.
       *
       * @param - name: the name of the state to retrieve
       * @return the state that has the given name
       *
       */
      St * getState( StName name ) const;

      /**
       * 
       * @brief clear the state name to states map
       *
       * This method clears the map that correlates state names to states.
       *
       */
      void clearMap( );

      //
      // Variables
      //
      
      protected:
              
      std::set<StName> states;
      std::set<StName> initialStates;  
      std::set<StName> finalStates;   
      //std::set<StName> names; 
      StMap name_St;
    };    
    
    //
    // Methods
    //

    //Constructors and Destructor 
    template <typename St,typename StName> 
    StateSet<St,StName>::StateSet( )
    {  }
     
    template <typename St,typename StName> 
    StateSet<St,StName>::StateSet( StateSet & other )
    {
      clearStates();
      
      names = other.names;
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;
      name_St = other.name_St;
    }
     
    template <typename St,typename StName> 
    StateSet<St,StName> & StateSet<St,StName>::operator=( const StateSet<St,StName> & other )
    {
      if (this == &other)     
        return *this;
    
      clearStates();
     
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;
      name_St = other.name_St;
      return *this;
    }
     
    template <typename St,typename StName> 
    StateSet<St,StName>::~StateSet( ) 
    { 
      clearStates();
      clearMap();
    }

    //State Accessors
    
    /**
     *
     * @brief removes all states 
     *
     */
    template <typename St,typename StName> 
    void StateSet<St,StName>::clearStates( )
    {
      states.clear();
      clearInitialStates();
      clearFinalStates();
      
      clearMap();   //If there are no states, there should be anything in the map either.
    }    
    
    /**
     *
     * @brief removes all initial states
     *
     */
    template <typename St,typename StName> 
    void StateSet<St,StName>::clearInitialStates( )
    { 
      for( iterator it = initialStates.begin(); it != initialStates.end(); it++ )
      {
        St * state = getState(*it);
        state->unsetAsInitial();
      }
      initialStates.clear();
    }    
    
    /**
     *
     * @brief removes all final states
     *
     */
    template <typename St,typename StName> 
    void StateSet<St,StName>::clearFinalStates( )
    {
      finalStates.clear();
    }
    
    /**
     * 
     * @brief tests whether the given state is a member of this collection
     *
     * @param - state: the state to test
     * @return true if the state is a member of this collection of
     *         states, false otherwise
     *
     */
    template <typename St,typename StName>
    bool StateSet<St,StName>::isState( const St * state ) const
    {
      return (states.count(state->getName()) > 0);
    } 
     
    /**
     * 
     * @brief tests whether the given state is an initial state of this 
     *        collection
     *
     * @param - initialState: the state to test
     * @return true if the state is an initial state of this collection,
     *         false otherwise
     *
     */
    template <typename St,typename StName>
    bool StateSet<St,StName>::isInitialState( const St * initialState ) const
    {
      return (initialStates.count(initialState->getName()) > 0);
    }
       
    /**
     * 
     * @brief tests whether the given state is a final state of this
     *        collection
     *
     * @param - finalState: the state to test
     * @return true if the state is a final state of this collection,
     *         false otherwise
     *
     */
    template <typename St,typename StName>
    bool StateSet<St,StName>::isFinalState( const St * finalState ) const
    {
      return (finalStates.count(finalState->getName()) > 0);
    }
    
    /**
     *
     * @brief add the given state 
     *
     * @param - state: the state to add 
     * @return false if the state already exists, true otherwise
     *
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::addState( St * addState )
    {
      if(isState(addState) ) 
        return false;
        
      states.insert(addState->getName());
      name_St.insert(std::pair<StName,St*>(addState->getName(),addState));
      
      return true;
    }    
    
    /**
     *
     * @brief add the given initial state 
     *
     * @param - initialState: the initial state to add 
     * @return false if the state is already an initial state, true otherwise
     *
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::addInitialState( St * addInitialState )
    {
      if( isInitialState(addInitialState) )  
        return false;
        
      if( !isState(addInitialState) )
      {
        addState(addInitialState);
      }

      St * state = getState(addInitialState->getName());
      state->setAsInitial( );

      initialStates.insert(addInitialState->getName());
      return true;
    }    
    
    /**
     *
     * @brief add the given final state 
     *
     * @param - finalState: the final state to add 
     * @return false if the state is already a final state, true otherwise
     *
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::addFinalState( St * addFinalState )
    {
      if( isFinalState(addFinalState) )  
        return false;
        
      if( !isState(addFinalState) )
      {
        addState(addFinalState);
      }

      finalStates.insert(addFinalState->getName());
      return true;
    }
      
    /**
     * 
     * @brief add all states in the given collection to this
     *        collection of states
     *
     * @param - stateSet: the collection of states to add to this collection
     *                  of states
     *
     */
    template <typename St,typename StName> 
    void StateSet<St,StName>::addAll( StateSet<St,StName> addStateSet )
    {
      addAllStates(addStateSet);
      addAllInitialStates(addStateSet);
      addAllFinalStates(addStateSet);
    }
    
    /**
     *
     * @brief add all the states in the given StateSet 
     *
     * @param - stateSet: the StateSet that contains the states to add
     *
     */
    template <typename St,typename StName> 
    void StateSet<St,StName>::addAllStates( StateSet<St,StName> addStateSet )
    {
      for( iterator it = addStateSet.states.begin();
            it != addStateSet.states.end(); it++ )
      {
        addState(addStateSet.getState(*it));
      }
    }  
      
    /**
     * 
     * @brief add all the initial states in the given StateSet
     *
     * @param - stateSet: the StateSet that contains the states to add
     *
     */ 
    template <typename St,typename StName> 
    void StateSet<St,StName>::addAllInitialStates( StateSet<St,StName> addStateSet )
    {
      for( iterator it = addStateSet.initialStates.begin();
            it != addStateSet.initialStates.end(); it++ )
      {
        addInitialState(addStateSet.getState(*it));
      }
    }
    
    /**
     * 
     * @brief add all the final states in the given StateSet
     *
     * @param - stateSet: the StateSet that contains the states to add
     *
     */
    template <typename St,typename StName> 
    void StateSet<St,StName>::addAllFinalStates( StateSet<St,StName> addStateSet )
    {
      for( iterator it = addStateSet.finalStates.begin();
            it != addStateSet.finalStates.end(); it++ )
      {
        addFinalState(addStateSet.getState(*it));
      }
    }
    
    /**
     * 
     * @brief remove the given state 
     *
     * @param - state: the state to remove
     * @return false if this state does not exist, true otherwise
     *
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::removeState( const St * removeState )
    {
      if( ! isState(removeState) )
        return false;
      states.erase(removeState->getName());
      name_St.erase(name_St.find(removeState->getName()));
      //remove from initial state set if it exists
      removeInitialState(removeState);
      //remove from final state set if it exists
      removeFinalState(removeState);
      return true;
    }   
     
    /**
     * 
     * @brief remove the given initial state
     *
     * @param - initialState: the initial state to remove
     * @return false if this state is not an initial state, true otherwise
     *  
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::removeInitialState( const St * removeInitialState )
    {
      if( ! isInitialState(removeInitialState) )
        return false;

      St * state = getState(removeInitialState->getName());
      state->unsetAsInitial( );

      initialStates.erase(removeInitialState->getName());
      return true;
    }    
    
    /**
     * 
     * @brief remove the given final state
     *
     * @param - finalState: the final state to remove
     * @remove false if this state is not a final state, true otherwise
     *
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::removeFinalState( const St * removeFinalState )
    {
      if( ! isFinalState(removeFinalState) )
        return false;
      finalStates.erase(removeFinalState->getName());
      return true;
    }
      
    //Utilities	

    /**
     *
     * @brief print the collection of states
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template <typename St,typename StName> 
    std::ostream & StateSet<St,StName>::print( std::ostream & o ) const
    {
      //Print the set of all states.
      o << "Q: ";
      o << "{ ";
      iterator it = beginStates();
      iterator itEND = endStates();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        ((name_St.find(*it))->second)->print(o);
      }
      o << " }" << std::endl;
      
      //Prints the initial states.
      o << "Q0: ";
      o << "{ ";
      it = beginInitialStates();
      itEND = endInitialStates();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        ((name_St.find(*it))->second)->print(o);
      }
      o << " }" << std::endl;
      
      //Print the final states.
      o << "Qf: ";
      o << "{ ";
      it = beginFinalStates();
      itEND = endFinalStates();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        ((name_St.find(*it))->second)->print(o);
      }
      o << " }" << std::endl;
      
      return o;
    }

    /**
     *
     * @brief tests whether this collection of states is equivalent 
     *        to the collection of states 'other'
     *
     * @param - other: the StateSet to compare this StateSet to
     * @return true if this StateSet is equivalent to the StateSet 
     *         'other'
     *
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::operator==( const StateSet<St,StName> & other ) const
    {
      //Check that the state sets are equal.
      for( iterator it = beginStates(); it != endStates(); it++ )
        if( other.isState(other.getState(*it)) )
          return false;
          
      for( iterator it = other.beginStates(); it != other.endStates(); it++ )
        if( isState(getState(*it)) )
          return false;
         
      //Check that the initial state sets are equal.    
      for( iterator it = beginInitialStates(); it != endInitialStates(); it++ )
        if( other.isInitialState(other.getState(*it)) )
          return false;
          
      for( iterator it = other.beginInitialStates(); it != other.endInitialStates(); it++ )
        if( isInitialState(getState(*it)) )
          return false;
        
      //Check that the final state sets are equal.    
      for( iterator it = beginFinalStates(); it != endFinalStates(); it++ )
        if( other.isFinalState(other.getState(*it)) )
          return false;
          
      for( iterator it = other.beginFinalStates(); it != other.endFinalStates(); it++ )
        if( isFinalState(getState(*it)) )
          return false;        
          
      return true;
    }
    
    /**
     * 
     * @brief provides access to the states in the collection 
     *
     * @return the starting point of an iterator through the states
     *
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::beginStates( ) const
    {
      return states.begin();
    }   
     
    /**
     * 
     * @brief provides access to the initial states in the collection
     *
     * @return the starting point of an iterator through the initial states
     *
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::beginInitialStates( ) const
    {
      return initialStates.begin();
    }    
    
    /**
     *
     * @brief provides access to the final states in the collection
     *
     * @return the starting point of an iterator through the final states
     *
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::beginFinalStates( ) const
    {
      return finalStates.begin();
    }
    
    /**
     * 
     * @brief provides access to the states in the collection 
     *
     * @return one place past the exit point of an iterator through 
     *         the states
     *
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::endStates( ) const
    {
      return states.end();
    }    
    
    /**
     * 
     * @brief provides access to the initial states in the collection
     *
     * @return one place past the exit point of an iterator through the
     *         initial states
     *
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::endInitialStates( ) const
    {
      return initialStates.end();
    }    
    
    /**
     * 
     * @brief provides access to the final states in the collection
     *
     * @return one place past the exit point of an iterator through the
     *         final states
     *
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::endFinalStates( ) const
    {
      return finalStates.end();
    }
    
    /**
     * 
     * @brief provides access to all states in the collection
     *
     * @return a set containing all states in this collection
     *
     */
    template <typename St,typename StName>
    std::set<St*> StateSet<St,StName>::getStates( ) const
    {
      std::set<St*> result;
      for( std::set<StName>::const_iterator it = states.begin(); it != states.end(); it++ )
      {
        result.insert(name_St.find(*it)->second);
      }
      return result;
    }

    /**
     * 
     * @brief provides access to the names of all the states in the 
     *        collection
     *
     * @return a set containing the names of all states in the 
     *         collection
     *
     */
    template <typename St,typename StName>
    std::set<StName> StateSet<St,StName>::getStateNames( ) const
    {
      return states;
    }
    
    /**
     * 
     * @brief provides access to the names of all the states in the
     *        given set of states
     *
     * @param - sts: the states whose names to return
     * @return the names correcponding to the given states
     *
     */
    template <typename St,typename StName> 
    std::set<StName> StateSet<St,StName>::getStateNames( const std::set<St> & sts ) const
    {
      std::set<StName> names;
      for(std::set<St>::const_iterator it = sts.begin(); it!=sts.end(); it++) 
        names.insert( it->getName() );
      return names;
    }
    
    /**
     * 
     * @brief provides access to the names of all the initial states
     *        in the collection
     *
     * @return a set containing the names of all initial states in
     *         the collection
     *
     */
    template <typename St,typename StName>
    std::set<StName> StateSet<St,StName>::getInitialStateNames( ) const
    {
      return initialStates;
    }
       
    /**
     *  
     * @brief provides access to the names of all final states in 
     *        the collection
     *
     * @return a set containing the names of all final states in 
     *         the collection
     *
     */
    template <typename St,typename StName>
    std::set<StName> StateSet<St,StName>::getFinalStateNames( ) const
    {
      return finalStates;
    }
    
    /**
     * 
     * @brief returns the number of states in this collection
     * 
     * @return the number of states in this collection
     *
     */
    template <typename St,typename StName> 
    size_t StateSet<St,StName>::sizeStates( ) const
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
    template <typename St,typename StName> 
    size_t StateSet<St,StName>::sizeInitialStates( ) const
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
    template <typename St,typename StName> 
    size_t StateSet<St,StName>::sizeFinalStates( ) const
    {
      return finalStates.size();
    }  
    
    /**
     * 
     * @brief gives 'dup' all the state properties of 'orig'
     *
     * @param - orig: the state whose properties to duplicate
     * @param - dup: the state whose properties are being set
     *
     */
    template <typename St,typename StName>
    void StateSet<St,StName>::dupState( const St * orig, St * dup )
    {
      if( isInitialState(orig) )
        addInitialState(dup);

      if( isFinalState(orig) )
        addFinalState(dup);
    }

    /**
     *  TODO: make sure that the mapping is unique in it's assignment of
     *        a state to a name
     * @brief provide access to the state with the given name
     *
     * This method provides access to the state in this collection
     * that has the given name.
     *
     * @param - name: the name of the state to retrieve
     * @return the state that has the given name
     *
     */
    template <typename St,typename StName> 
    typename St * StateSet<St,StName>::getState( StName name ) const
    {
      StMap::const_iterator it = name_St.find(name);
      if( it != name_St.end() )
        return it->second;
      else
        return NULL;
    }    
    
    /**
     * 
     * @brief clear the state name to states map
     *
     */
    template <typename St,typename StName> 
    void StateSet<St,StName>::clearMap( )
    {
      name_St.clear();
    }
#else  //ifdef KEY
    template<typename Client>
    class StateSet : public Printable
    {
      public:
        typedef State<Client> St;
        typedef typename std::set<St>::const_iterator const_iterator;
        typedef typename std::set<St>::iterator iterator;

      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      StateSet( );
      StateSet( StateSet & other );
      StateSet & operator=( const StateSet & other );

      ~StateSet( );

      //State Accessors

      /**
       *
       * @brief removes all states 
       *
       * This method removes all states from this collection.  It also
       * removes all initial states and final states.
       *
       */
      void clearStates( );
        
      /**
       *
       * @brief removes all initial states
       *
       * This method removes all states from the initial state set, but
       * does not remove them from the state set.
       *
       */
      void clearInitialStates( );
        
      /**
       *
       * @brief removes all final states
       *
       * This method removes all states from the final state set, but
       * does not remove them from the state set.
       *
       */
      void clearFinalStates( );
      
      /**
       * 
       * @brief tests whether the given state is a member of this collection
       *
       * This method determines whether the given state is a member of
       * this collection.  It returns true if the state is a member and
       * false otherwise.
       *
       * @param - state: the state to test
       * @return true if the state is a member of this collection of
       *         states, false otherwise
       *
       */
      bool isState( const St & state ) const;
        
      /**
       * 
       * @brief tests whether the given state is an initial state of this 
       *        collection
       *
       * This method determines whether the given state is an initial state
       * of this collection.  It returns true if the state is an initial state
       * and false otherwise.
       *
       * @param - initialState: the state to test
       * @return true if the state is an initial state of this collection,
       *         false otherwise
       *
       */
      bool isInitialState( const St & initialState ) const;
        
      /**
       * 
       * @brief tests whether the given state is a final state of this
       *        collection
       *
       * This method determines whether the given state is a final state
       * of this collection.  It returns true if the state is a final state
       * and false otherwise.
       *
       * @param - finalState: the state to test
       * @return true if the state is a final state of this collection,
       *         false otherwise
       *
       */
      bool isFinalState( const St & finalState ) const;
      
      /**
       *
       * @brief add the given state 
       *
       * This method adds the given state.  If the state already exists, 
       * false is returned.  Otherwise, true is returned.
       *
       * @param - state: the state to add 
       * @return false if the state already exists, true otherwise
       *
       */
      bool addState( St * state );
        
      /**
       *
       * @brief add the given initial state 
       *
       * This method adds the given state to this collection of 
       * states(if it does not already exist).  The given initial state
       * is then added to the initial states.  If the state is already an
       * initial state, false is returned.  Otherwise, true is returned.
       *
       * @param - initialState: the initial state to add 
       * @return false if the state is already an initial state, true otherwise
       *
       */
      bool addInitialState( St * initialState );
        
      /**
       *
       * @brief add the given final state 
       *
       * This method adds the given state to this collection of 
       * states(if it does not already exist).  The given final state
       * is then added to the final states.  If the state is already a
       * final state, false is returned.  Otherwise, true is returned.
       *
       * @param - finalState: the final state to add 
       * @return false if the state is already a final state, true otherwise
       *
       */
      bool addFinalState( St * finalState );
      
      /**
       * 
       * @brief add all states in the given collection to this
       *        collection of states
       *
       * This method adds all of the states in the given collection
       * of states to this collection of states.
       *
       * @param - stateSet: the collection of states to add to this collection
       *                    of states
       *
       */
      void addAll( StateSet stateSet );
        
      /**
       *
       * @brief add all the states in the given StateSet 
       *
       * This method adds all of the given states to the state set.
       *
       * @param - stateSet: the StateSet that contains the states to add
       *
       */
      void addAllStates( StateSet stateSet );
        
      /**
       * 
       * @brief add all the initial states in the given StateSet
       *
       * This method adds all of the given initial states to the
       * initial state set (and thus to the state set if they are 
       * not already elements of the state set).
       *
       * @param - stateSet: the StateSet that contains the states to add
       *
       */
      void addAllInitialStates( StateSet stateSet );
        
      /**
       * 
       * @brief add all the final states in the given StateSet
       *
       * This method adds all of the given final states to the 
       * final state set (and thus to the state set if they are 
       * not already elements of the state set).
       *
       * @param - stateSet: the StateSet that contains the states to add
       *
       */
      void addAllFinalStates( StateSet stateSet );
      
      /**
       * 
       * @brief remove the given state 
       *
       * This method removes the given state.  If the state does not
       * exist, false is returned.  Otherwise, true is returned.
       * Note: If the given state is an initial state or a final state,
       * it is also removed from that set.
       *
       * @param - state: the state to remove
       * @return false if this state does not exist, true otherwise
       *
       */
      bool removeState( const St & state );
        
      /**
       * 
       * @brief remove the given initial state
       *
       * This method removes the given initial state.  If the state
       * is not an initial state, false is returned.  Otherwise, true 
       * is returned.
       * Note: The state is not removed from the state set.
       *
       * @param - initialState: the initial state to remove
       * @return false if this state is not an initial state, true otherwise
       *  
       */
      bool removeInitialState( const St & initialState );
        
      /**
       * 
       * @brief remove the given final state
       *
       * This method removes the given final state.  If the state
       * is not a final state, false is returned.  Otherwise, true
       * is returned.
       * Note: The state is not removed from the state set.
       *
       * @param - finalState: the final state to remove
       * @remove false if this state is not a final state, true otherwise
       *
       */
      bool removeFinalState( const St & finalState );
      
      //Utilities	

      /**
       *
       * @brief print the collection of states
       *
       * This method prints out the state set to the output stream 
       * provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to
       *
       */
      std::ostream & print( std::ostream & o ) const;

      /**
       *
       * @brief print the names of the states in this collection 
       *
       * This method prints out the names of the states in this state set 
       * to the output stream provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to
       *
       */
      std::ostream & printName( std::ostream & o ) const;

      /**
       *
       * @brief tests whether this collection of states is equivalent 
       *        to the collection of states 'other'
       *
       * This method tests the equivalence of this set of states and 
       * the set of states 'other'.
       *
       * @param - other: the StateSet to compare this StateSet to
       * @return true if this StateSet is equivalent to the StateSet 
       *         'other'
       *
       */
      bool operator==( const StateSet & other ) const;
       
      /**
       * 
       * @brief provides access to the states in the collection 
       *
       * This method provides access to the states in this collection 
       * through an iterator.
       *
       * @return the starting point of an iterator through the states
       *
       */
      const_iterator beginStates( ) const;
        
      /**
       * 
       * @brief provides access to the initial states in the collection
       *
       * This method provides access to the initial states in this 
       * collection through an iterator.
       *
       * @return the starting point of an iterator through the initial states
       *
       */
      const_iterator beginInitialStates( ) const;
        
      /**
       *
       * @brief provides access to the final states in the collection
       *
       * This method provides access to the final states in this
       * collection through an iterator.
       *
       * @return the starting point of an iterator through the final states
       *
       */
      const_iterator beginFinalStates( ) const;
        
      /**
       * 
       * @brief provides access to the states in the collection 
       *
       * This method provides access to the states in the collection 
       * through an iterator.
       *
       * @return one place past the exit point of an iterator through 
       *         the states
       *
       */
      const_iterator endStates( ) const;
        
      /**
       * 
       * @brief provides access to the initial states in the collection
       *
       * This method provides access to the initial states in the collection
       * through an iterator.
       *
       * @return one place past the exit point of an iterator through the
       *         initial states
       *
       */
      const_iterator endInitialStates( ) const;
        
      /**
       * 
       * @brief provides access to the final states in the collection
       *
       * This method provides access to the final states in the collection
       * through an iterator.
       *
       * @return one place past the exit point of an iterator through the
       *         final states
       *
       */
      const_iterator endFinalStates( ) const;
      
      /**
       * 
       * @brief provides access to all states in the collection
       *
       * This method provides access to all states in this collection 
       * in the form of a set of states.
       *
       * @return a set containing all states in this collection
       *
       */
      std::set<St> getStates( ) const;

      /**
       * 
       * @brief provides access to the names of all the states in the 
       *        collection
       *
       * This method provides access to the names of all the states
       * in the collection in the form of a set of state names.
       *
       * @return a set containing the names of all states in the 
       *         collection
       *
       */
      std::set<Key> getStateNames( ) const;
             
      /**
       * 
       * @brief provides access to the names of all the states in the
       *        given set of states
       *
       * This method provides access to the names of all the states in 
       * the given set of states in the form of a set of state names.
       *
       * @param - sts: the states whose names to return
       * @return the names correcponding to the given states
       *
       */
      std::set<Key> getStateNames( const std::set<St> & sts ) const;
     
      /**
       *  
       * @brief provides access to the names of all the initial states
       *        in the collection
       *
       * This method provides access to the names of all the initial
       * states in the collection in the form of a set of state names.
       *
       * @return a set containing the names of all initial states in
       *         the collection
       *
       */
      std::set<Key> getInitialStateNames( ) const;
        
      /**
       * 
       * @brief provides access to the names of all final states in 
       *        the collection
       *
       * This method provides access to the names of all the final 
       * states in the collection in the form of a set of state names.
       *
       * @return a set containing the names of all final states in 
       *         the collection
       *
       */
      std::set<Key> getFinalStateNames( ) const;

      /**
       * 
       * @brief returns the number of states in this collection
       * 
       * This method returns the number of states in this collection.
       *
       * @return the number of states in this collection
       *
       */
      size_t sizeStates( ) const;
      
      /**
       * 
       * @brief returns the number of initial states in this collection
       *
       * This method returns the number of initial states in this collection.
       *
       * @return the number of initial states in this collection
       *
       */
      size_t sizeInitialStates( ) const;
      
      /**
       * 
       * @brief returns the number of final states in this collection
       * 
       * This method returns the number of final states in this collection.
       *
       * @return the number of final states in this collection
       *
       */
      size_t sizeFinalStates( ) const;     
      
      /**
       *  
       * @brief gives 'dup' all the state properties of 'orig'
       *
       * This method checks all the state properties (initial/final)
       * of 'orig' and assigns the same properties to 'dup'.
       *
       * @param - orig: the state whose properties to duplicate
       * @param - dup: the state whose properties are being set
       *
       */
      void dupState( const St & orig, St * dup );

      /**
       *  TODO: make sure that the mapping is unique in it's assignment of
       *        a state to a name
       * @brief provide access to the state with the given name
       *
       * This method provides access to the state in this collection
       * that has the given name.
       *
       * @param - name: the name of the state to retrieve
       * @return the state that has the given name
       *
       */
      St * getState( Key name ) const;

      //
      // Variables
      //
      
      protected:
              
      std::set<St> states;
      std::set<St> initialStates;  
      std::set<St> finalStates;   
      std::set<Key> names; 
    };    
    
    //
    // Methods
    //

    //Constructors and Destructor 
    template<typename Client>
    StateSet<Client>::StateSet( )
    {  }
     
    template<typename Client>
    StateSet<Client>::StateSet( StateSet<Client> & other )
    {
      clearStates();
      
      names = other.names;
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;
    }
     
    template<typename Client>
    StateSet<Client> & StateSet<Client>::operator=( const StateSet<Client> & other )
    {
      if (this == &other)     
        return *this;
    
      clearStates();
      
      names = other.names;
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;
      return *this;
    }
     
    template<typename Client>
    StateSet<Client>::~StateSet( ) 
    { 
      clearStates();
    }

    //State Accessors
    
    /**
     *
     * @brief removes all states 
     *
     */
    template<typename Client>
    void StateSet<Client>::clearStates( )
    {
      names.clear();
      states.clear();
      clearInitialStates();
      clearFinalStates();
    }    
    
    /**
     *
     * @brief removes all initial states
     *
     */
    template<typename Client>
    void StateSet<Client>::clearInitialStates( )
    { 
      initialStates.clear();
    }    
    
    /**
     *
     * @brief removes all final states
     *
     */
    template<typename Client>
    void StateSet<Client>::clearFinalStates( )
    {
      finalStates.clear();
    }
    
    /**
     * 
     * @brief tests whether the given state is a member of this collection
     *
     * @param - state: the state to test
     * @return true if the state is a member of this collection of
     *         states, false otherwise
     *
     */
    template<typename Client>
    bool StateSet<Client>::isState( const St & state ) const
    {
      return (states.count(state) > 0);
    } 
     
    /**
     * 
     * @brief tests whether the given state is an initial state of this 
     *        collection
     *
     * @param - initialState: the state to test
     * @return true if the state is an initial state of this collection,
     *         false otherwise
     *
     */
    template<typename Client>
    bool StateSet<Client>::isInitialState( const St & initialState ) const
    {
      return (initialStates.count(initialState) > 0);
    }
       
    /**
     * 
     * @brief tests whether the given state is a final state of this
     *        collection
     *
     * @param - finalState: the state to test
     * @return true if the state is a final state of this collection,
     *         false otherwise
     *
     */
    template<typename Client>
    bool StateSet<Client>::isFinalState( const St & finalState ) const
    {
      return (finalStates.count(finalState) > 0);
    }
    
    /**
     *
     * @brief add the given state 
     *
     * @param - state: the state to add 
     * @return false if the state already exists, true otherwise
     *
     */
    template<typename Client>
    bool StateSet<Client>::addState( St * addState )
    {
      if(isState(*addState) ) 
        return false;
        
      states.insert(*addState);
      names.insert(addState->getName());
      
      return true;
    }    
    
    /**
     *
     * @brief add the given initial state 
     *
     * @param - initialState: the initial state to add 
     * @return false if the state is already an initial state, true otherwise
     *
     */ 
    template<typename Client>
    bool StateSet<Client>::addInitialState( St * addInitialState )
    {
      if( isInitialState(*addInitialState) )  
        return false;
        
      if( !isState(*addInitialState) )
      {
        addState(addInitialState);
      }

      initialStates.insert(*addInitialState);
      return true;
    }    
    
    /**
     *
     * @brief add the given final state 
     *
     * @param - finalState: the final state to add 
     * @return false if the state is already a final state, true otherwise
     *
     */
    template<typename Client>
    bool StateSet<Client>::addFinalState( St * addFinalState )
    {
      if( isFinalState(*addFinalState) )  
        return false;
        
      if( !isState(*addFinalState) )
      {
        addState(addFinalState);
      }

      finalStates.insert(*addFinalState);
      return true;
    }
      
    /**
     * 
     * @brief add all states in the given collection to this
     *        collection of states
     *
     * @param - stateSet: the collection of states to add to this collection
     *                  of states
     *
     */
    template<typename Client>
    void StateSet<Client>::addAll( StateSet<Client> addStateSet )
    {
      addAllStates(addStateSet);
      addAllInitialStates(addStateSet);
      addAllFinalStates(addStateSet);
    }
    
    /**
     *
     * @brief add all the states in the given StateSet 
     *
     * @param - stateSet: the StateSet that contains the states to add
     *
     */
    template<typename Client>
    void StateSet<Client>::addAllStates( StateSet<Client> addStateSet )
    {
      for( iterator it = addStateSet.states.begin();
            it != addStateSet.states.end(); it++ )
      {
        addState(&(*it));
      }
    }  
      
    /**
     * 
     * @brief add all the initial states in the given StateSet
     *
     * @param - stateSet: the StateSet that contains the states to add
     *
     */  
    template<typename Client>
    void StateSet<Client>::addAllInitialStates( StateSet<Client> addStateSet )
    {
      for( iterator it = addStateSet.initialStates.begin();
            it != addStateSet.initialStates.end(); it++ )
      {
        addInitialState(&(*it));
      }
    }
    
    /**
     * 
     * @brief add all the final states in the given StateSet
     *
     * @param - stateSet: the StateSet that contains the states to add
     *
     */
    template<typename Client>
    void StateSet<Client>::addAllFinalStates( StateSet<Client> addStateSet )
    {
      for( iterator it = addStateSet.finalStates.begin();
            it != addStateSet.finalStates.end(); it++ )
      {
        addFinalState(&(*it));
      }
    }
    
    /**
     * 
     * @brief remove the given state 
     *
     * @param - state: the state to remove
     * @return false if this state does not exist, true otherwise
     *
     */
    template<typename Client>
    bool StateSet<Client>::removeState( const St & removeState )
    {
      if( ! isState(removeState) )
        return false;
      states.erase(removeState);
      names.erase(removeState.getName());
      //remove from initial state set if it exists
      removeInitialState(removeState);
      //remove from final state set if it exists
      removeFinalState(removeState);
      return true;
    }   
     
    /**
     * 
     * @brief remove the given initial state
     *
     * @param - initialState: the initial state to remove
     * @return false if this state is not an initial state, true otherwise
     *  
     */
    template<typename Client>
    bool StateSet<Client>::removeInitialState( const St & removeInitialState )
    {
      if( ! isInitialState(removeInitialState) )
        return false;
      initialStates.erase(removeInitialState);
      return true;
    }    
    
    /**
     * 
     * @brief remove the given final state
     *
     * @param - finalState: the final state to remove
     * @remove false if this state is not a final state, true otherwise
     *
     */
    template<typename Client>
    bool StateSet<Client>::removeFinalState( const St & removeFinalState )
    {
      if( ! isFinalState(removeFinalState) )
        return false;
      finalStates.erase(removeFinalState);
      return true;
    }
      
    //Utilities	

    /**
     *
     * @brief print the collection of states
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template<typename Client>
    std::ostream & StateSet<Client>::print( std::ostream & o ) const
    {
      //Print the set of all states.
      o << "Q: ";
      o << "{ ";
      const_iterator it = beginStates();
      const_iterator itEND = endStates();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        it->print(o);
      }
      o << " }" << std::endl;
      
      //Prints the initial states.
      o << "Q0: ";
      o << "{ ";
      it = beginInitialStates();
      itEND = endInitialStates();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        it->print(o);
      }
      o << " }" << std::endl;
      
      //Print the final states.
      o << "Qf: ";
      o << "{ ";
      it = beginFinalStates();
      itEND = endFinalStates();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        it->print(o);
      }
      o << " }" << std::endl;
      
      return o;
    }

    /**
     *
     * @brief print the names of the states in this collection
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template<typename Client>
    std::ostream & StateSet<Client>::printName( std::ostream & o ) const
    {
      //Print the set of all states.
      o << "Q: ";
      o << "{ ";
      const_iterator it = beginStates();
      const_iterator itEND = endStates();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        it->printName(o);
      }
      o << " }" << std::endl;
      
      //Prints the initial states.
      o << "Q0: ";
      o << "{ ";
      it = beginInitialStates();
      itEND = endInitialStates();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        it->printName(o);
      }
      o << " }" << std::endl;
      
      //Print the final states.
      o << "Qf: ";
      o << "{ ";
      it = beginFinalStates();
      itEND = endFinalStates();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        it->printName(o);
      }
      o << " }" << std::endl;
      
      return o;
    }

    /**
     *
     * @brief tests whether this collection of states is equivalent 
     *        to the collection of states 'other'
     *
     * @param - other: the StateSet to compare this StateSet to
     * @return true if this StateSet is equivalent to the StateSet 
     *         'other'
     *
     */
    template<typename Client>
    bool StateSet<Client>::operator==( const StateSet<Client> & other ) const
    {
      //Check that the state sets are equal.
      for( const_iterator it = beginStates(); it != endStates(); it++ )
        if( other.isState(*it) )
          return false;
          
      for( const_iterator it = other.beginStates(); it != other.endStates(); it++ )
        if( isState(*it) )
          return false;
         
      //Check that the initial state sets are equal.    
      for( const_iterator it = beginInitialStates(); it != endInitialStates(); it++ )
        if( other.isInitialState(*it) )
          return false;
          
      for( const_iterator it = other.beginInitialStates(); it != other.endInitialStates(); it++ )
        if( isInitialState(*it) )
          return false;
        
      //Check that the final state sets are equal.    
      for( const_iterator it = beginFinalStates(); it != endFinalStates(); it++ )
        if( other.isFinalState(*it) )
          return false;
          
      for( const_iterator it = other.beginFinalStates(); it != other.endFinalStates(); it++ )
        if( isFinalState(*it) )
          return false;        
          
      return true;
    }
    
    /**
     * 
     * @brief provides access to the states in the collection 
     *
     * @return the starting point of an iterator through the states
     *
     */
    template<typename Client>
    typename StateSet<Client>::const_iterator StateSet<Client>::beginStates( ) const
    {
      return states.begin();
    }   
     
    /**
     * 
     * @brief provides access to the initial states in the collection
     *
     * @return the starting point of an iterator through the initial states
     *
     */
    template<typename Client>
    typename StateSet<Client>::const_iterator StateSet<Client>::beginInitialStates( ) const
    {
      return initialStates.begin();
    }    
    
    /**
     *
     * @brief provides access to the final states in the collection
     *
     * @return the starting point of an iterator through the final states
     *
     */
    template<typename Client>
    typename StateSet<Client>::const_iterator StateSet<Client>::beginFinalStates( ) const
    {
      return finalStates.begin();
    }
    
    /**
     * 
     * @brief provides access to the states in the collection 
     *
     * @return one place past the exit point of an iterator through 
     *         the states
     *
     */
    template<typename Client>
    typename StateSet<Client>::const_iterator StateSet<Client>::endStates( ) const
    {
      return states.end();
    }    
    
    /**
     * 
     * @brief provides access to the initial states in the collection
     *
     * @return one place past the exit point of an iterator through the
     *         initial states
     *
     */
    template<typename Client>
    typename StateSet<Client>::const_iterator StateSet<Client>::endInitialStates( ) const
    {
      return initialStates.end();
    }    
    
    /**
     * 
     * @brief provides access to the final states in the collection
     *
     * @return one place past the exit point of an iterator through the
     *         final states
     *
     */
    template<typename Client>
    typename StateSet<Client>::const_iterator StateSet<Client>::endFinalStates( ) const
    {
      return finalStates.end();
    }
    
    /**
     * 
     * @brief provides access to all states in the collection
     *
     * @return a set containing all states in this collection
     *
     */
    template<typename Client>
    std::set<typename StateSet<Client>::St> StateSet<Client>::getStates( ) const
    {
      return states;
    }

    /**
     * 
     * @brief provides access to the names of all the states in the 
     *        collection
     *
     * @return a set containing the names of all states in the 
     *         collection
     *
     */
    template<typename Client>
    std::set<Key> StateSet<Client>::getStateNames( ) const
    {
      return names;
    }
    
    /**
     * 
     * @brief provides access to the names of all the states in the
     *        given set of states
     *
     * @param - sts: the states whose names to return
     * @return the names correcponding to the given states
     *
     */
    template<typename Client>
    std::set<Key> StateSet<Client>::getStateNames( const std::set<St> & sts ) const
    {
      std::set<Key> names;
      for(std::set<St>::const_iterator it = sts.begin(); it!=sts.end(); it++) 
        names.insert( (*it).getName() );
      return names;
    }
    
    /**
     * 
     * @brief provides access to the names of all the initial states
     *        in the collection
     *
     * @return a set containing the names of all initial states in
     *         the collection
     *
     */
    template<typename Client>
    std::set<Key> StateSet<Client>::getInitialStateNames( ) const
    {
      return getStateNames(initialStates);
    }
       
    /**
     *  
     * @brief provides access to the names of all final states in 
     *        the collection
     *
     * @return a set containing the names of all final states in 
     *         the collection
     *
     */
    template<typename Client>
    std::set<Key> StateSet<Client>::getFinalStateNames( ) const
    {
      return getStateNames(finalStates);
    }
    
    /**
     * 
     * @brief returns the number of states in this collection
     * 
     * @return the number of states in this collection
     *
     */
    template<typename Client>
    size_t StateSet<Client>::sizeStates( ) const
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
    template<typename Client>
    size_t StateSet<Client>::sizeInitialStates( ) const
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
    template<typename Client>
    size_t StateSet<Client>::sizeFinalStates( ) const
    {
      return finalStates.size();
    }  
    
    /**
     * 
     * @brief gives 'dup' all the state properties of 'orig'
     *
     * @param - orig: the state whose properties to duplicate
     * @param - dup: the state whose properties are being set
     *
     */
    template<typename Client>
    void StateSet<Client>::dupState( const St & orig, St * dup )
    {
      if( isInitialState(orig) )
        addInitialState(dup);

      if( isFinalState(orig) )
        addFinalState(dup);
    }

    /**
     *  TODO: make sure that the mapping is unique in it's assignment of
     *        a state to a name
     * @brief provide access to the state with the given name
     *
     * This method provides access to the state in this collection
     * that has the given name.
     *
     * @param - name: the name of the state to retrieve
     * @return the state that has the given name
     *
     */
    template<typename Client>
    typename StateSet<Client>::St * StateSet<Client>::getState( Key name ) const
    { //TODO
      //StMap::const_iterator it = name_St.find(name);
      //if( it != name_St.end() )
      //  return it->second;
      //else
        return NULL;
    }    
    
#endif
  }
}
#endif