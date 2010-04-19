#ifndef wali_nwa_StateSet_GUARD
#define wali_nwa_StateSet_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/nwa/ClientInfo.hpp"

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
    template<typename Client>
    class StateSet : public Printable
    {
      public:
        typedef Key St;
        typedef typename std::set<St> States;
        typedef typename States::const_iterator const_iterator;   
        typedef typename States::iterator iterator; 

        typedef ref_ptr<Client> ClientInfoRefPtr;

      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      StateSet( );
      StateSet( StateSet & other );
      StateSet & operator=( const StateSet & other );

      ~StateSet( );

      //Client Info Accessors

      /**
       * 
       * @brief access the client information associated with the given state
       *
       * This method provides access to the client information associated with this state.
       *
       * @param - state: the state whose client information to retrieve
       * @return the client information associated with the given state
       *
       */
      ClientInfoRefPtr getClientInfo( St state ) const;

      /**
       * Q: If the state doesn't exit should we add the state(and assign it the given info)?
       * @brief set the client information associated with the given state
       *
       * This method sets the client information associated with the given state to the 
       * client information provided.
       * Note: If there is already some client information associated with the given 
       *        state it is lost.
       *
       * @param - state: the state whose client information to set
       * @param - c: the desired client information for this state
       *
       */
      void setClientInfo( St state, const ClientInfoRefPtr c );

      //State Accessors

      /**
       *
       * @brief returns the Key for the stuck state
       *
       * This method provides access to the Key for the stuck state.
       *
       * @return the Key for the stuck state
       *
       */
      inline St getStuckState( )
      {
        return stuck;
      };

      /**
       *
       * @brief sets the Key for the stuck state
       *
       * This method provides access to the Key for the stuck state.
       *
       * @param - state: the Key for the stuck state
       *
       */
      inline void setStuckState( St state )
      {
        stuck = state;
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
        return (state == stuck);
      }

      /**
       *
       * @brief removes all states 
       *
       * This method removes all states from this collection.  It also removes all 
       * initial states and final states.
       *
       */
      void clearStates( );
        
      /**
       *
       * @brief removes all initial states
       *
       * This method removes all states from the initial state set, but does not remove
       * any states from the state set.
       *
       */
      void clearInitialStates( );
        
      /**
       *
       * @brief removes all final states
       *
       * This method removes all states from the final state set, but does not remove 
       * any states from the state set.
       *
       */
      void clearFinalStates( );
      
      /**
       *
       * @brief tests whether the given state is a member of this collection
       *
       * This method determines whether the given state is a member of this collection.
       * It returns true if the state is a member and false otherwise.
       *
       * @param - state: the state to test
       * @return true if the state is a member of this collection of states
       *
       */
      bool isState( St state ) const;
        
      /**
       *   
       * @brief tests whether the given state is an initial state of this collection
       *
       * This method determines whether the given state is an initial state of this 
       * collection.  It returns true if the state is an initial state and false otherwise.
       *
       * @param - initialState: the state to test
       * @return true if the state is an initial state of this collection, false otherwise
       *
       */
      bool isInitialState( St initialState ) const;
        
      /**
       * 
       * @brief tests whether the given state is a final state of this collection
       *
       * This method determines whether the given state is a final state of this
       * collection.  It returns true if the state is a final state and false otherwise.
       *
       * @param - finalState: the state to test
       * @return true if the state is a final state of this collection, false otherwise
       *
       */
      bool isFinalState( St finalState ) const;
      
      /**
       * 
       * @brief add the given state 
       *
       * This method adds the given state.  If the state already exists, false is 
       * returned.  Otherwise, true is returned.
       *
       * @param - state: the state to add 
       * @return false if the state already exists, true otherwise
       *
       */
      bool addState( St state );
        
      /**
       *
       * @brief add the given initial state 
       *
       * This method adds the given state to this collection of states(if it does not
       * already exist).  The given initial state is then added to the initial states.
       * If the state is already an initial state, false is returned.  Otherwise, true
       * is returned.
       *
       * @param - initialState: the initial state to add 
       * @return false if the state is already an initial state, true otherwise
       *
       */
      bool addInitialState( St initialState );
        
      /**
       * 
       * @brief add the given final state 
       *
       * This method adds the given state to this collection of states(if it does not
       * already exist).  The given final state is then added to the final states.  If 
       * the state is already a final state, false is returned.  Otherwise, true is 
       * returned.
       *
       * @param - finalState: the final state to add 
       * @return false if the state is already a final state, true otherwise
       *
       */
      bool addFinalState( St finalState );
      
      /**
       * 
       * @brief add all states in the given collection to this collection of states
       *
       * This method adds all of the states in the given collection of states to this
       * collection of states.
       *
       * @param - stateSet: the collection of states to add to this collection of states
       *
       */
      void addAll( const StateSet<Client> & stateSet );
        
      /**
       *
       * @brief add all the states in the given StateSet 
       *
       * This method adds all of the given states to the state set.
       *
       * @param - stateSet: the StateSet that contains the states to add
       *
       */
      void addAllStates( const StateSet<Client> & stateSet );
        
      /**
       * 
       * @brief add all the initial states in the given StateSet
       *
       * This method adds all of the given initial states to the initial state set (and
       * thus to the state set if they are not already elements of the state set).
       *
       * @param - stateSet: the StateSet that contains the states to add
       *
       */
      void addAllInitialStates( const StateSet<Client> & stateSet );
        
      /**
       * 
       * @brief add all the final states in the given StateSet
       *
       * This method adds all of the given final states to the final state set (and thus
       * to the state set if they are not already elements of the state set).
       *
       * @param - stateSet: the StateSet that contains the states to add
       *
       */
      void addAllFinalStates( const StateSet<Client> & stateSet );
      
      /**
       *  
       * @brief remove the given state 
       *
       * This method removes the given state.  If the state does not exist, false is
       * returned.  Otherwise, true is returned.
       * Note: If the given state is an initial state or a final state, it is also 
       *  removed from that set.
       *
       * @param - state: the state to remove
       * @return false if this state does not exist, true otherwise
       *
       */
      bool removeState( St state );
        
      /**
       * 
       * @brief remove the given initial state
       *
       * This method removes the given initial state.  If the state is not an initial
       * state, false is returned.  Otherwise, true is returned.
       * Note: The state is not removed from the state set.
       *
       * @param - initialState: the initial state to remove
       * @return false if this state is not an initial state, true otherwise
       *  
       */
      bool removeInitialState( St initialState );
        
      /**
       * 
       * @brief remove the given final state
       *
       * This method removes the given final state.  If the state is not a final state, 
       * false is returned.  Otherwise, true is returned.
       * Note: The state is not removed from the state set.
       *
       * @param - finalState: the final state to remove
       * @remove false if this state is not a final state, true otherwise
       *
       */
      bool removeFinalState( St finalState );
      
      //Utilities	

      /**
       *
       * @brief print the collection of states
       *
       * This method prints out the keys associated with the state set to the output 
       * stream provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to
       *
       */
      std::ostream & print( std::ostream & o ) const;

      /**
       *
       * @brief tests whether this collection of states is equivalent to the collection 
       *        of states 'other'
       *
       * This method tests the equivalence of this set of states and the set of states
       * 'other'.
       *
       * @param - other: the StateSet to compare this StateSet to
       * @return true if this StateSet is equivalent to the StateSet 'other'
       *
       */
      bool operator==( const StateSet<Client> & other ) const;
       
      /**
       * 
       * @brief provides access to the states in the collection 
       *
       * This method provides access to the states in this collection through an iterator.
       *
       * @return the starting point of an iterator through the states
       *
       */
      const_iterator beginStates( ) const;
        
      /**
       * 
       * @brief provides access to the initial states in the collection
       *
       * This method provides access to the initial states in this collection through an 
       * iterator.
       *
       * @return the starting point of an iterator through the initial states
       *
       */
      const_iterator beginInitialStates( ) const;
        
      /**
       *
       * @brief provides access to the final states in the collection
       *
       * This method provides access to the final states in this collection through an 
       * iterator.
       *
       * @return the starting point of an iterator through the final states
       *
       */
      const_iterator beginFinalStates( ) const;
        
      /**
       * 
       * @brief provides access to the states in the collection 
       *
       * This method provides access to the states in the collection through an iterator.
       *
       * @return one place past the exit point of an iterator through the states
       *
       */
      const_iterator endStates( ) const;
        
      /**
       * 
       * @brief provides access to the initial states in the collection
       *
       * This method provides access to the initial states in the collection through an
       * iterator.
       *
       * @return one place past the exit point of an iterator through the initial states
       *
       */
      const_iterator endInitialStates( ) const;
        
      /**
       * 
       * @brief provides access to the final states in the collection
       *
       * This method provides access to the final states in the collection through an
       * iterator.
       *
       * @return one place past the exit point of an iterator through the final states
       *
       */
      const_iterator endFinalStates( ) const;
      
      /**
       * 
       * @brief provides access to all states in the collection
       *
       * This method provides access to all states in this collection in the form of a 
       * set of states.
       *
       * @return a set containing all states in this collection
       *
       */
      const States & getStates( ) const;
     
      /**
       *  
       * @brief provides access to the names of all the initial states in the collection
       *
       * This method provides access to the names of all the initial states in the 
       * collection in the form of a set of state names.
       *
       * @return a set containing the names of all initial states in the collection
       *
       */
      const States & getInitialStates( ) const;
        
      /**
       * 
       * @brief provides access to the names of all final states in the collection
       *
       * This method provides access to the names of all the final states in the 
       * collection in the form of a set of state names.
       *
       * @return a set containing the names of all final states in the collection
       *
       */
      const States & getFinalStates( ) const;

      /**
       * 
       * @brief returns the number of states in this collection
       * 
       * This method returns the number of states in this collection.
       * Note: This will always be at least 1 as the stuck state is
       *        always a legitimate state.
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
       * This method checks all the state properties (initial/final) of 'orig' and 
       * assigns the same properties to 'dup'.
       *
       * @param - orig: the state whose properties to duplicate
       * @param - dup: the state whose properties are being set
       *
       */
      void dupState( St orig, St dup );

      //
      // Variables
      //
      
      protected:
              
      St stuck;
      States states;
      States initialStates;  
      States finalStates;   

      std::map<St,ClientInfoRefPtr> stateInfos;
    };    
    
    //
    // Methods
    //

    //Constructors and Destructor 
    template<typename Client>
    StateSet<Client>::StateSet( ) { }
     
    template<typename Client>
    StateSet<Client>::StateSet( StateSet<Client> & other )
    {
      clearStates();
      
      stuck = other.stuck;
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;

      stateInfos = other.stateInfos;
    }
     
    template<typename Client>
    StateSet<Client> & StateSet<Client>::operator=( const StateSet<Client> & other )
    {
      if (this == &other)     
        return *this;
    
      clearStates();
      
      stuck = other.stuck;
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;

      stateInfos = other.stateInfos;

      return *this;
    }
     
    template<typename Client>
    StateSet<Client>::~StateSet( ) 
    { 
      clearStates();
    }

    //Client Info Accessors

    /**
     * 
     * @brief access the client information associated with the given state
     *
     * @param - state: the state whose client information to retrieve
     * @return the client information associated with the given state
     *
     */
    template<typename Client>
    typename StateSet<Client>::ClientInfoRefPtr StateSet<Client>::getClientInfo( St state ) const 
    {
      std::map<St,ClientInfoRefPtr>::const_iterator it = (stateInfos.find(state));
      if( it == stateInfos.end() )
        return NULL;
      else
        return it->second;   
    }

    /**
     * 
     * @brief set the client information associated with the given state
     *
     * @param - state: the state whose client information to set
     * @param - c: the desired client information for this state
     *
     */
    template<typename Client>
    void StateSet<Client>::setClientInfo( St state, const ClientInfoRefPtr c )
    {
      //Check to make sure this is a valid state.
      if(! isState(state) )
      {
        addState(state);
      }
      //Update the state's info.
      stateInfos[state] = c;
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
      states.clear();
      clearInitialStates();
      clearFinalStates();

      stateInfos.clear();
    }    
    
    /**
     *
     * @brief removes all initial states
     *
     */
    template<typename Client>
    inline
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
    inline
    void StateSet<Client>::clearFinalStates( )
    {
      finalStates.clear();
    }
    
    /**
     *  
     * @brief tests whether the given state is a member of this collection
     *
     * @param - state: the state to test
     * @return true if the state is a member of this collection of states, false otherwise
     *
     */
    template<typename Client>
    inline
    bool StateSet<Client>::isState( St state ) const
    {
      return (states.count(state) > 0);
    } 
     
    /**
     * 
     * @brief tests whether the given state is an initial state of this collection
     *
     * @param - initialState: the state to test
     * @return true if the state is an initial state of this collection, false otherwise
     *
     */
    template<typename Client>
    inline
    bool StateSet<Client>::isInitialState( St initialState ) const
    {
      return (initialStates.count(initialState) > 0);
    }
       
    /**
     *  
     * @brief tests whether the given state is a final state of this collection
     *
     * @param - finalState: the state to test
     * @return true if the state is a final state of this collection, false otherwise
     *
     */
    template<typename Client>
    inline
    bool StateSet<Client>::isFinalState( St finalState ) const
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
    bool StateSet<Client>::addState( St state )
    {
      if(isState(state) ) 
        return false;
        
      states.insert(state);
      //TODO: Q: Is there a null ClientInfoRefPtr that I can use here?
      
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
    bool StateSet<Client>::addInitialState( St initialState )
    {
      if( isInitialState(initialState) )  
        return false;
        
      if( !isState(initialState) )
      {
        addState(initialState);
      }

      initialStates.insert(initialState);

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
    bool StateSet<Client>::addFinalState( St finalState )
    {
      if( isFinalState(finalState) )  
        return false;
        
      if( !isState(finalState) )
      {
        addState(finalState);
      }

      finalStates.insert(finalState);

      return true;
    }
      
    /**
     * 
     * @brief add all states in the given collection to this collection of states
     *
     * @param - stateSet: the collection of states to add to this collection of states
     *
     */
    template<typename Client>
    void StateSet<Client>::addAll( const StateSet<Client> & stateSet )
    {
      addAllStates(stateSet);
      addAllInitialStates(stateSet);
      addAllFinalStates(stateSet);
    }
    
    /**
     *
     * @brief add all the states in the given StateSet 
     *
     * @param - stateSet: the StateSet that contains the states to add
     *
     */
    template<typename Client>
    void StateSet<Client>::addAllStates( const StateSet<Client> & stateSet )
    {
      for( const_iterator it = stateSet.beginStates();
            it != stateSet.endStates(); it++ )
      {
        addState(*it);
        //Set the clientInfo of this state.
        //TODO: check this -- if it is not right, fix star() in NWA.hpp
        ClientInfo info = *(stateSet.getClientInfo(*it));
        ClientInfoRefPtr ci = ClientInfoRefPtr(new ClientInfo(info));
        setClientInfo(*it,ci);
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
    void StateSet<Client>::addAllInitialStates( const StateSet<Client> & stateSet )
    {
      for( const_iterator it = stateSet.beginInitialStates();
            it != stateSet.endInitialStates(); it++ )
      {
        addInitialState(*it);
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
    void StateSet<Client>::addAllFinalStates( const StateSet<Client> & stateSet )
    {
      for( const_iterator it = stateSet.beginFinalStates();
            it != stateSet.endFinalStates(); it++ )
      {
        addFinalState(*it);
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
    bool StateSet<Client>::removeState( St state )
    {
      //The stuck state cannot be removed in this way.
      if( isStuckState( state ) )
        return false;

      if( ! isState(state) )
        return false;
      states.erase(state);

      //remove from initial state set if it is an initial state
      removeInitialState(state);
      //remove from final state set if it is a final state
      removeFinalState(state);

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
    bool StateSet<Client>::removeInitialState( St initialState )
    {
      if( ! isInitialState(initialState) )
        return false;

      initialStates.erase(initialState);

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
    bool StateSet<Client>::removeFinalState( St finalState )
    {
      if( ! isFinalState(finalState) )
        return false;

      finalStates.erase(finalState);

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
       
        printKey(o,*it);
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

        printKey(o,*it);
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

        printKey(o,*it);
      }
      o << " }" << std::endl;
      
      return o;
    }

    /**
     *
     * @brief tests whether this collection of states is equivalent to the collection 
     *        of states 'other'
     *
     * @param - other: the StateSet to compare this StateSet to
     * @return true if this StateSet is equivalent to the StateSet 'other'
     *
     */
    template<typename Client>
    bool StateSet<Client>::operator==( const StateSet<Client> & other ) const
    {
      //Check that the state sets are equal.
      if(! (stuck == other.stuck) )
        return false;

      for( const_iterator it = beginStates(); it != endStates(); it++ )
        if(! other.isState(*it) )
          return false;
          
      for( const_iterator it = other.beginStates(); it != other.endStates(); it++ )
        if(! isState(*it) )
          return false;
         
      //Check that the initial state sets are equal.    
      for( const_iterator it = beginInitialStates(); it != endInitialStates(); it++ )
        if(! other.isInitialState(*it) )
          return false;
          
      for( const_iterator it = other.beginInitialStates(); it != other.endInitialStates(); it++ )
        if(! isInitialState(*it) )
          return false;
        
      //Check that the final state sets are equal.    
      for( const_iterator it = beginFinalStates(); it != endFinalStates(); it++ )
        if(! other.isFinalState(*it) )
          return false;
          
      for( const_iterator it = other.beginFinalStates(); it != other.endFinalStates(); it++ )
        if(! isFinalState(*it) )
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
    inline
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
    inline
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
    inline
    typename StateSet<Client>::const_iterator StateSet<Client>::beginFinalStates( ) const
    {
      return finalStates.begin();
    }
    
    /**
     * 
     * @brief provides access to the states in the collection 
     *
     * @return one place past the exit point of an iterator through the states
     *
     */
    template<typename Client>
    inline
    typename StateSet<Client>::const_iterator StateSet<Client>::endStates( ) const
    {
      return states.end();
    }    
    
    /**
     * 
     * @brief provides access to the initial states in the collection
     *
     * @return one place past the exit point of an iterator through the initial states
     *
     */
    template<typename Client>
    inline
    typename StateSet<Client>::const_iterator StateSet<Client>::endInitialStates( ) const
    {
      return initialStates.end();
    }    
    
    /**
     * 
     * @brief provides access to the final states in the collection
     *
     * @return one place past the exit point of an iterator through the final states
     *
     */
    template<typename Client>
    inline
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
    const typename StateSet<Client>::States & StateSet<Client>::getStates( ) const
    {
      return states;
    }
    
    /**
     * 
     * @brief provides access to the names of all the initial states in the collection
     *
     * @return a set containing the names of all initial states in the collection
     *
     */
    template<typename Client>
    const typename StateSet<Client>::States & StateSet<Client>::getInitialStates( ) const
    {
      return initialStates;
    }
       
    /**
     *  
     * @brief provides access to the names of all final states in the collection
     *
     * @return a set containing the names of all final states in the collection
     *
     */
    template<typename Client>
    const typename StateSet<Client>::States & StateSet<Client>::getFinalStates( ) const
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
    template<typename Client>
    inline
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
    inline
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
    inline
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
    void StateSet<Client>::dupState( St orig, St dup )
    {
      if( isInitialState(orig) )
        addInitialState(dup);

      if( isFinalState(orig) )
        addFinalState(dup);
    }
  }
}
#endif