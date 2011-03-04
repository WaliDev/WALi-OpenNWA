/**
 * @author Amanda Burton
 */

// ::wali
#include "StateSet.hpp"

namespace wali
{
  namespace nwa
  {
    //
    // Methods
    //

    
    StateSet & StateSet::operator=( const StateSet & other )
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
     
    //Client Info Accessors

    /**
     * 
     * @brief access the client information associated with the given state
     *
     * @param - state: the state whose client information to retrieve
     * @return the client information associated with the given state
     *
     */
    
    StateSet::ClientInfoRefPtr
    StateSet::getClientInfo( St state ) const 
    {
      typedef std::map<St, ClientInfoRefPtr> Map;
      Map::const_iterator it = (stateInfos.find(state));
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
    
    void StateSet::setClientInfo( St state, const ClientInfoRefPtr c )
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
    
    void StateSet::clearStates( )
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
    
    inline
    void StateSet::clearInitialStates( )
    { 
      initialStates.clear();
    }    
    
    /**
     *
     * @brief removes all final states
     *
     */
    
    inline
    void StateSet::clearFinalStates( )
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
    
    inline
    bool StateSet::isState( St state ) const
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
    
    inline
    bool StateSet::isInitialState( St initialState ) const
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
    
    inline
    bool StateSet::isFinalState( St finalState ) const
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
    
    bool StateSet::addState( St state )
    {
	  // If the ClientInfo is requested for the state, it will be null
      bool inserted = states.insert(state).second;
      return inserted;
    }    
    
    /**
     *  
     * @brief add the given initial state 
     *
     * @param - initialState: the initial state to add 
     * @return false if the state is already an initial state, true otherwise
     *
     */ 
    
    bool StateSet::addInitialState( St initialState )
    {
      bool inserted = initialStates.insert(initialState).second;
      if (inserted) {
      // It wasn't a initial state, so we might not have seen it
          addState(initialState);
      }
      
      return inserted;
    }    
    
    /**
     * 
     * @brief add the given final state 
     *
     * @param - finalState: the final state to add 
     * @return false if the state is already a final state, true otherwise
     *
     */
    
    bool StateSet::addFinalState( St finalState )
    {
      bool inserted = finalStates.insert(finalState).second;
      if (inserted) {
        // It wasn't a final state, so we might not have seen it
	      addState(finalState);
      }
      
      return inserted;
    }
      
    /**
     * 
     * @brief add all states in the given collection to this collection of states
     *
     * @param - stateSet: the collection of states to add to this collection of states
     *
     */
    
    void StateSet::addAll( const StateSet & stateSet )
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
    
    void StateSet::addAllStates( const StateSet & stateSet )
    {
      for( const_iterator it = stateSet.beginStates();
           it != stateSet.endStates(); it++ )
      {
        addState(*it);
        //Set the clientInfo of this state.
        //TODO: check this -- if it is not right, fix star() in NWA.hpp
        if (stateSet.getClientInfo(*it) != 0) {
          Client info = *(stateSet.getClientInfo(*it));
          ClientInfoRefPtr ci = ClientInfoRefPtr(new Client(info));
          setClientInfo(*it,ci);
        }
      }
    }  
      
    /**
     * 
     * @brief add all the initial states in the given StateSet
     *
     * @param - stateSet: the StateSet that contains the states to add
     *
     */  
    
    void StateSet::addAllInitialStates( const StateSet & stateSet )
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
    
    void StateSet::addAllFinalStates( const StateSet & stateSet )
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
    
    bool StateSet::removeState( St state )
    {
      //The stuck state cannot be removed in this way.
      if( isStuckState( state ) )
        return false;

      size_t erased = states.erase(state);

      if (erased == 0) {
        return false;
      }
      else {
        removeInitialState(state);
        removeFinalState(state);
        return true;
      }
    }   
     
    /**
     * 
     * @brief remove the given initial state
     *
     * @param - initialState: the initial state to remove
     * @return false if this state is not an initial state, true otherwise
     *  
     */
    
    bool StateSet::removeInitialState( St initialState )
    {
      size_t erased = initialStates.erase(initialState);
      return erased > 0;
    }    
    
    /**
     *  
     * @brief remove the given final state
     *
     * @param - finalState: the final state to remove
     * @remove false if this state is not a final state, true otherwise
     *
     */
    
    bool StateSet::removeFinalState( St finalState )
    {
      size_t erased = finalStates.erase(finalState);
      return erased > 0;
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
    
    std::ostream & StateSet::print( std::ostream & o ) const
    {
      //Print the set of all states.
      o << "Q: {\n  ";
      const_iterator it = beginStates();
      const_iterator itEND = endStates();
      bool first = true;
      for(const_iterator it = beginStates(); it != endStates() ; it++, first=false )
      {
        if( !first )
          o << ",\n  ";
       
        printKey(o,*it);
        o << " (=" << *it << ")";
      }
      o << "}\n" << std::endl;
      
      //Prints the initial states.
      o << "Q0: {\n  ";
      first = true;
      for(const_iterator it = beginInitialStates(); it != endInitialStates() ; it++, first=false )
      {
        if( !first )
          o << ",\n  ";

        printKey(o,*it);
        o << " (=" << *it << ")";
      }
      o << "}\n" << std::endl;
      
      //Print the final states.
      o << "Qf: {\n  ";
      first = true;
      for(const_iterator it = beginFinalStates(); it != endFinalStates() ; it++, first=false )
      {
        if( !first )
          o << ",\n  ";

        printKey(o,*it);
        o << " (=" << *it << ")";
      }
      o << "}" << std::endl;
      
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
    
    bool StateSet::operator==( const StateSet & other ) const
    {
      //Check that the state sets are equal.
      if (stuck != other.stuck) return false;
      if (states != other.states) return false;
      if (initialStates != other.initialStates) return false;
      if (finalStates != other.finalStates) return false;

      // FIXME: Compare client infos as well?
      return true;
    }
    
    /**
     * 
     * @brief provides access to the states in the collection 
     *
     * @return the starting point of an iterator through the states
     *
     */
    
    inline
    StateSet::const_iterator StateSet::beginStates( ) const
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
    
    inline
    StateSet::const_iterator StateSet::beginInitialStates( ) const
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
    
    inline
    StateSet::const_iterator StateSet::beginFinalStates( ) const
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
    
    inline
    StateSet::const_iterator StateSet::endStates( ) const
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
    
    inline
    StateSet::const_iterator StateSet::endInitialStates( ) const
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
    
    inline
    StateSet::const_iterator StateSet::endFinalStates( ) const
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
    
    const StateSet::States & StateSet::getStates( ) const
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
    
    const StateSet::States & StateSet::getInitialStates( ) const
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
    
    const StateSet::States & StateSet::getFinalStates( ) const
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
    
    inline
    size_t StateSet::sizeStates( ) const
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
    
    inline
    size_t StateSet::sizeInitialStates( ) const
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
    
    inline
    size_t StateSet::sizeFinalStates( ) const
    {
      return finalStates.size();
    }  
    
    /**
     * 
     * @brief gives 'dup' all the state properties of 'orig'
     *
     * FIXME: suppose dup is final, orig is not; this won't
     *        remove finality of dup
     *
     * @param - orig: the state whose properties to duplicate
     * @param - dup: the state whose properties are being set
     *
     */
    
    void StateSet::dupState( St orig, St dup )
    {
      if( isInitialState(orig) )
        addInitialState(dup);

      if( isFinalState(orig) )
        addFinalState(dup);
    }
  }
}

