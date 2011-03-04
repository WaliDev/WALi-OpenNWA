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
    
    class StateSet : public Printable
    {
      public:
        typedef ClientInfo Client;
        typedef Key St;
        typedef std::set<St> States;
        typedef States::const_iterator const_iterator;   
        typedef States::iterator iterator; 

        typedef ref_ptr<Client> ClientInfoRefPtr;


        // The following macro fakes static data declarations with
        // initializers in a template class to work around C++ being
        // dumb. Static data in a template class is almost useless
        // because you have to explicitly define it for every
        // instantiation. Instead, make a static member function that
        // returns the value. (In this case, it's stored as a
        // function-static variable, but this is somewhat irrelevant.
#define DEFINE_FAKE_STATIC_DATA(name, value)    \
        static std::string const & name() {  \
          static std::string ret = value;    \
          return ret;                        \
        }

        DEFINE_FAKE_STATIC_DATA(XMLFinalAttr, "final")
        DEFINE_FAKE_STATIC_DATA(XMLInitialAttr, "initial")
        DEFINE_FAKE_STATIC_DATA(XMLNameAttr, "name")
        DEFINE_FAKE_STATIC_DATA(XMLStateTag, "State")

#undef DEFINE_FAKE_STATIC_DATA

      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      StateSet & operator=( const StateSet & other );

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
      inline St getStuckState( ) const
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
      inline bool isStuckState( St state ) const
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
      void addAll( const StateSet & stateSet );
        
      /**
       *
       * @brief add all the states in the given StateSet 
       *
       * This method adds all of the given states to the state set.
       *
       * @param - stateSet: the StateSet that contains the states to add
       *
       */
      void addAllStates( const StateSet & stateSet );
        
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
      void addAllInitialStates( const StateSet & stateSet );
        
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
      void addAllFinalStates( const StateSet & stateSet );
      
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
      bool operator==( const StateSet & other ) const;
       
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

      size_t largestState() const {
        return *states.rbegin();
      }
      
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
#endif
