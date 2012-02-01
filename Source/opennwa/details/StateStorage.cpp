/**
 * @author Amanda Burton
 */

// ::wali
#include "StateStorage.hpp"

using wali::printKey;

namespace opennwa
{

  namespace details
  {
      
    //
    // Methods
    //
    StateStorage::StateStorage()
    {
    }
        

    StateStorage::StateStorage(StateStorage const & other)
      : Printable(other)
      , states(other.states)
      , initialStates(other.initialStates)
      , finalStates(other.finalStates)
    {
      for (std::map<State,ClientInfoRefPtr>::const_iterator other_cis = other.stateInfos.begin() ;
           other_cis != other.stateInfos.end(); ++other_cis)
      {
        ClientInfoRefPtr ci = other_cis->second;
        if (ci.is_valid()) {
          stateInfos[other_cis->first] = ci->cloneRp();
        }
      }
    }
      
    
    StateStorage & StateStorage::operator=( const StateStorage & other )
    {
      if (this == &other)     
        return *this;
    
      clearStates();
      
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;

      stateInfos.clear();
      for (std::map<State,ClientInfoRefPtr>::const_iterator other_cis = other.stateInfos.begin() ;
           other_cis != other.stateInfos.end(); ++other_cis)
      {
        stateInfos[other_cis->first] = other_cis->second->cloneRp();
      }

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
    
    StateStorage::ClientInfoRefPtr
    StateStorage::getClientInfo( State state ) const 
    {
      typedef std::map<State, ClientInfoRefPtr> Map;
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
    
    void StateStorage::setClientInfo( State state, const ClientInfoRefPtr c )
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
    
    void StateStorage::clearStates( )
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
    
    void StateStorage::clearInitialStates( )
    { 
      initialStates.clear();
    }    
    
    /**
     *
     * @brief removes all final states
     *
     */
    
    void StateStorage::clearFinalStates( )
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
    
    bool StateStorage::isState( State state ) const
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
    
    bool StateStorage::isInitialState( State initialState ) const
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
    
    bool StateStorage::isFinalState( State finalState ) const
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
    
    bool StateStorage::addState( State state )
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
    
    bool StateStorage::addInitialState( State initialState )
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
    
    bool StateStorage::addFinalState( State finalState )
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
    
    void StateStorage::addAll( const StateStorage & stateSet )
    {
      addAllStates(stateSet);
      addAllInitialStates(stateSet);
      addAllFinalStates(stateSet);
    }
    
    /**
     *
     * @brief add all the states in the given StateStorage 
     *
     * @param - stateSet: the StateStorage that contains the states to add
     *
     */
    
    void StateStorage::addAllStates( const StateStorage & stateSet )
    {
      for( const_iterator it = stateSet.beginStates();
           it != stateSet.endStates(); it++ )
      {
        addState(*it);
        //Set the clientInfo of this state.
        //TODO: check this -- if it is not right, fix star() in Nwa.hpp
        if (stateSet.getClientInfo(*it) != 0) {
          ClientInfoRefPtr ci = stateSet.getClientInfo(*it)->cloneRp();
          setClientInfo(*it,ci);
        }
      }
    }  
      
    /**
     * 
     * @brief add all the initial states in the given StateStorage
     *
     * @param - stateSet: the StateStorage that contains the states to add
     *
     */  
    
    void StateStorage::addAllInitialStates( const StateStorage & stateSet )
    {
      for( const_iterator it = stateSet.beginInitialStates();
           it != stateSet.endInitialStates(); it++ )
      {
        addInitialState(*it);
      }
    }
    
    /**
     * 
     * @brief add all the final states in the given StateStorage
     *
     * @param - stateSet: the StateStorage that contains the states to add
     *
     */
    
    void StateStorage::addAllFinalStates( const StateStorage & stateSet )
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
    
    bool StateStorage::removeState( State state )
    {
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
    
    bool StateStorage::removeInitialState( State initialState )
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
    
    bool StateStorage::removeFinalState( State finalState )
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
    
    std::ostream & StateStorage::print( std::ostream & o ) const
    {
      //Print the set of all states.
      o << "Q: {\n  ";
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
     * @param - other: the StateStorage to compare this StateStorage to
     * @return true if this StateStorage is equivalent to the StateStorage 'other'
     *
     */
    
    bool StateStorage::operator==( const StateStorage & other ) const
    {
      //Check that the state sets are equal.
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
    
    StateStorage::const_iterator StateStorage::beginStates( ) const
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
    
    StateStorage::const_iterator StateStorage::beginInitialStates( ) const
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
    
    StateStorage::const_iterator StateStorage::beginFinalStates( ) const
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
    
    StateStorage::const_iterator StateStorage::endStates( ) const
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
    
    StateStorage::const_iterator StateStorage::endInitialStates( ) const
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
    
    StateStorage::const_iterator StateStorage::endFinalStates( ) const
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
    
    const StateSet & StateStorage::getStates( ) const
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
    
    const StateSet & StateStorage::getInitialStates( ) const
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
    
    const StateSet & StateStorage::getFinalStates( ) const
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
    
    size_t StateStorage::sizeStates( ) const
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
    
    size_t StateStorage::sizeInitialStates( ) const
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
    
    size_t StateStorage::sizeFinalStates( ) const
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
    
    void StateStorage::dupState( State orig, State dup )
    {
      if( isInitialState(orig) )
        addInitialState(dup);

      if( isFinalState(orig) )
        addFinalState(dup);
    }


  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

