#ifndef wali_nwa_StateStorage_GUARD
#define wali_nwa_StateStorage_GUARD 1

/**
 * @author Amanda Burton
 */

#include "opennwa/NwaFwd.hpp"

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/ref_ptr.hpp"
#include "opennwa/ClientInfo.hpp"

// std::c++
#include <iostream>
#include <set>
#include <map>

namespace opennwa
{
  namespace details
  {
      
    /**
     *
     *  This class is used to keep track of the states of an NWA.
     *  
     */
    
    class StateStorage : public wali::Printable
    {
    public:
      typedef ClientInfo Client;
      typedef StateSet::const_iterator const_iterator;   
      typedef StateSet::iterator iterator; 

      typedef ref_ptr<Client> ClientInfoRefPtr;


      // The following macro fakes static data declarations with
      // initializers in a template class to work around C++ being
      // dumb. Static data in a template class is almost useless
      // because you have to explicitly define it for every
      // instantiation. Instead, make a static member function that
      // returns the value. (In this case, it's stored as a
      // function-static variable, but this is somewhat irrelevant.
#define DEFINE_FAKE_STATIC_DATA(name, value)    \
      static std::string const & name() {       \
        static std::string ret = value;         \
        return ret;                             \
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
      StateStorage();
      StateStorage(StateStorage const & other);
      StateStorage & operator=( const StateStorage & other );

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
      ClientInfoRefPtr getClientInfo( State state ) const;

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
      void setClientInfo( State state, const ClientInfoRefPtr c );

      //State Accessors


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
      bool isState( State state ) const;
        
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
      bool isInitialState( State initialState ) const;
        
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
      bool isFinalState( State finalState ) const;
      
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
      bool addState( State state );
        
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
      bool addInitialState( State initialState );
        
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
      bool addFinalState( State finalState );
      
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
      void addAll( const StateStorage & stateSet );
        
      /**
       *
       * @brief add all the states in the given StateStorage 
       *
       * This method adds all of the given states to the state set.
       *
       * @param - stateSet: the StateStorage that contains the states to add
       *
       */
      void addAllStates( const StateStorage & stateSet );
        
      /**
       * 
       * @brief add all the initial states in the given StateStorage
       *
       * This method adds all of the given initial states to the initial state set (and
       * thus to the state set if they are not already elements of the state set).
       *
       * @param - stateSet: the StateStorage that contains the states to add
       *
       */
      void addAllInitialStates( const StateStorage & stateSet );
        
      /**
       * 
       * @brief add all the final states in the given StateStorage
       *
       * This method adds all of the given final states to the final state set (and thus
       * to the state set if they are not already elements of the state set).
       *
       * @param - stateSet: the StateStorage that contains the states to add
       *
       */
      void addAllFinalStates( const StateStorage & stateSet );
      
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
      bool removeState( State state );
        
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
      bool removeInitialState( State initialState );
        
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
      bool removeFinalState( State finalState );
      
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
       * @param - other: the StateStorage to compare this StateStorage to
       * @return true if this StateStorage is equivalent to the StateStorage 'other'
       *
       */
      bool operator==( const StateStorage & other ) const;
       
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
      const StateSet & getStates( ) const;
     
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
      const StateSet & getInitialStates( ) const;
        
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
      const StateSet & getFinalStates( ) const;

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
      void dupState( State orig, State dup );

      //
      // Variables
      //
      
    protected:
              
      StateSet states;
      StateSet initialStates;  
      StateSet finalStates;   

      std::map<State,ClientInfoRefPtr> stateInfos;
    };


  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

