#ifndef wali_nwa_StateSet_GUARD
#define wali_nwa_StateSet_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/KeyContainer.hpp"

// std::c++
#include <iostream>
#include <set>

namespace wali
{
  namespace nwa
  {
    template <typename T> class StateSet : public Printable
    {
      //TODO: make this coordinate states, initial states, and final states
      //TODO: update comments
    
      public:
        typedef typename std::set<T>::iterator iterator;
    
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
         * TODO: remove
         * @brief removes all states from this collection of states
         *
         * This method removes all states from this collection of
         * states.
         *
         */
        void clear( );
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
         * TODO: remove
         * @brief add the given state to this collection of states
         *
         * This method adds the given state to this collection of 
         * states.  If the state already exists, false is returned.
         * Otherwise, true is returned.
         *
         * @parm the state to add to this collection of states
         * @return false if the state already exists in the collection
         *
         */
        bool add(T addState);
        /**
         *
         * @brief add the given state 
         *
         * This method adds the given state.  If the state already exists, 
         * false is returned.  Otherwise, true is returned.
         *
         * @parm the state to add 
         * @return false if the state already exists
         *
         */
        bool addState(T addState);
        /**
         *
         * @brief add the given initial state 
         *
         * This method adds the given state to this collection of 
         * states(if it does not already exist).  The given initial state
         * is then added to the initial states.  If the state is already an
         * initial state, false is returned.  Otherwise, true is returned.
         *
         * @parm the initial state to add 
         * @return false if the state is already an initial state
         *
         */
        bool addInitialState(T addInitialState);
        /**
         *
         * @brief add the given final state 
         *
         * This method adds the given state to this collection of 
         * states(if it does not already exist).  The given final state
         * is then added to the final states.  If the state is already a
         * final state, false is returned.  Otherwise, true is returned.
         *
         * @parm the final state to add 
         * @return false if the state is already a final state
         *
         */
        bool addFinalState(T addFinalState);
      
        /**
         * TODO
         * @brief add all states in the given collection to this
         * collection of states
         *
         * This method adds all of the states in the given collection
         * of states to this collection of states.
         *
         * @parm the collection of states to add to this collection
         * of states
         *
         */
        void addAll(StateSet<T> addStateSet);
        /**
         *
         * @brief add all the states in the given StateSet 
         *
         * This method adds all of the given states to the state set.
         *
         * @param the StateSet that contains the states to add
         *
         */
        void addAllStates(StateSet<T> addStateSet);
        
        /**
         * TODO
         */
        void addAllInitialStates(StateSet<T> addStateSet);
        
        /**
         * TODO
         */
        void addAllFinalStates(StateSet<T> addStateSet);
      
        /**
         * TODO: remove
         * @brief remove the given state from this collection of 
         * states
         *
         * This method removes the given state from this collection 
         * of states.  If the given state does not exist in this
         * collection of states false is returned.  Otherwise,
         * true is returned.
         *
         * @parm the state to remove from this collection
         * @return false if the given state does not exist in this 
         * collection, true otherwise.
         *
         */
        bool remove(T removeState);
        
        /**
         * TODO
         */
        bool removeState(T removeState);
        
        /**
         * TODO
         */
        bool removeInitialState(T removeInitialState);
        
        /**
         * TODO
         */
        bool removeFinalState(T removeFinalState);
      
        //Utilities	

        /**
         *
         * @brief print the collection of states
         *
         * This method prints out the state set to the output stream 
         * provided.
         *
         * @param the output stream to print to
         * @return the output stream that was printed to
         *
         */
        std::ostream & print( std::ostream & o) const;

        /**
         *
         * @brief tests whether this collection of states is equivalent 
         * to the collection of states 'other'
         *
         * This method tests the equivalence of this set of states and 
         * the set of states 'other'.
         *
         * @param the StateSet to compare this StateSet to
         * @return true if this StateSet is equivalent to the StateSet 
         * 'other'
         *
         */
        bool operator==( const StateSet<T> & other );

        /**
         * TODO: remove
         * @brief provides access to the states in the collection 
         * through an iterator
         *
         * This method provides access to the states in this collection
         * of states through an iterator.
         *
         * @return the starting point of an iterator through this collection
         * of states
         *
         */
        iterator begin();
        
        /**
         * TODO
         */
        iterator beginStates();
        
        /**
         * TODO
         */
        iterator beginInitialStates();
        
        /**
         * TODO
         */
        iterator beginFinalStates();
      
        /**
         * TODO: remove
         * @brief provides access to the states in the collection 
         * through an iterator
         *
         * This method provides access to the states in this collection
         * of states through an iterator.
         *
         * @return the exit point of an iterator through this collection of
         * states
         *
         */
        iterator end();
        
        /**
         * TODO
         */
        iterator endStates();
        
        /**
         * TODO
         */
        iterator endInitialStates();
        
        /**
         * TODO
         */
        iterator endFinalStates();

        /**
         * TODO: remove
         * @brief returns the number of states in this collection
         *
         * This method returns the number of states in this collection
         * of states.
         *
         * @return the number of states in this collection
         *
         */
        size_t size( ) const;
        
        /**
         * TODO
         */
        size_t sizeStates( ) const;
        
        /**
         * TODO
         */
        size_t sizeInitialStates( ) const;
        
        /**
         * TODO
         */
        size_t sizeFinalStates( ) const;
      
        /**
         * TODO: remove
         * @brief tests whether the given state is a member of this
         * collection of states
         *
         * This method tests whether the given state is a member of
         * this collection of states.
         *
         * @parm the state to test
         * @return true if the state is a member of this collection of
         * states, false otherwise
         *
         */
        bool contains( T state ) const;
        
        /**
         * TODO
         */
        bool containsState( T state );
        
        /**
         * TODO
         */
        bool containsInitialState( T initialState );
        
        /**
         * TODO
         */
        bool containsFinalState( T finalState );

      //
      // Variables
      //
      protected:
        std::set<T> states;
        std::set<T> initialStates;  //TODO: make this a set of pointers to states in the state set?
        std::set<T> finalStates;    //TODO: make this a set of pointers to states in the state set?  
    };
    
    
    //
    // Methods
    //

    //Constructors and Destructor
    template <typename T> 
    StateSet<T>::StateSet( )
    {  }
    
    template <typename T> 
    StateSet<T>::StateSet( StateSet & other )
    {
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;
    }
    
    template <typename T> 
    StateSet<T> & StateSet<T>::operator=( const StateSet<T> & other )
    {
      states = other.states;
      initialStates = other.initialStates;
      finalStates = other.finalStates;
      return *this;
    }
    
    template <typename T> 
    StateSet<T>::~StateSet( ) {  }

    //State Accessors

    /**
     * TODO: remove
     * @brief removes all states from this collection of states
     *
     */
    template <typename T> 
    void StateSet<T>::clear( )
    {
      states.clear();
      initialStates.clear();
      finalStates.clear();
    }
    
    /**
     * TODO
     */
    template <typename T>
    void StateSet<T>::clearStates( )
    {
      states.clear();
      initialStates.clear();
      finalStates.clear();
    }
    
    /**
     * TODO
     */
    template <typename T>
    void StateSet<T>::clearInitialStates( )
    { 
      initialStates.clear();
    }
    
    /**
     * TODO
     */
    template <typename T>
    void StateSet<T>::clearFinalStates( )
    {
      finalStates.clear();
    }
      
    /**
     * TODO: remove
     * @brief add the given state to this collection of states
     *
     * @parm the state to add to this collection of states
     * @return false if the state already exists in the collection
     *
     */
    template <typename T> 
    bool StateSet<T>::add(T addState)
    {
      if( states.count(addState) > 0 )
        return false;
      states.insert(addState);
      return true;
    }
    
    /**
     * TODO
     */
    template <typename T>
    bool StateSet<T>::addState(T addState)
    {
      if( states.count(addState) > 0 )
        return false;
      states.insert(addState);
      return true;
    }
    
    /**
     * TODO
     */
    template <typename T>
    bool StateSet<T>::addInitialState(T addInitialState)
    {
      if( initialStates.count(addInitialState) > 0 )
        return false;
      initialStates.insert(addInitialState);
      return true;
    }
    
    /**
     * TODO
     */
    template <typename T>
    bool StateSet<T>::addFinalState(T addFinalState)
    {
      if( finalStates.count(addFinalState) > 0 )
        return false;
      finalStates.insert(addFinalState);
      return true;
    }
      
    /**
     * TODO
     * @brief add all states in the given collection to this
     * collection of states
     *
     * @parm the collection of states to add to this collection
     * of states
     *
     */
    template <typename T> 
    void StateSet<T>::addAll(StateSet<T> addStateSet)
    {
      for( iterator it = addStateSet.states.begin();
            it != addStateSet.states.end(); it++ )
      {
        states.insert(*it);
      }
      for( iterator it = addStateSet.initialStates.begin();
            it != addStateSet.initialStates.end(); it++ )
      {
        initialStates.insert(*it);
      }
      for( iterator it = addStateSet.finalStates.begin();
            it != addStateSet.finalStates.end(); it++ )
      {
        finalStates.insert(*it);
      }
    }
    
    /**
     * TODO
     */
    template <typename T> 
    void StateSet<T>::addAllStates(StateSet<T> addStateSet)
    {
      for( iterator it = addStateSet.states.begin();
            it != addStateSet.states.end(); it++ )
      {
        states.insert(*it);
      }
    }
    
    /**
     * TODO
     */
    template <typename T> 
    void StateSet<T>::addAllInitialStates(StateSet<T> addStateSet)
    {
      for( iterator it = addStateSet.initialStates.begin();
            it != addStateSet.initialStates.end(); it++ )
      {
        initialStates.insert(*it);
      }
    }
    
    /**
     * TODO
     */
    template <typename T> 
    void StateSet<T>::addAllFinalStates(StateSet<T> addStateSet)
    {
      for( iterator it = addStateSet.finalStates.begin();
            it != addStateSet.finalStates.end(); it++ )
      {
        finalStates.insert(*it);
      }
    }
      
    /**
     * TODO: remove
     * @brief remove the given state from this collection of 
     * states
     *
     * @parm the state to remove from this collection
     * @return false if the given state does not exist in this 
     * collection, true otherwise.
     *
     */
    template <typename T> 
    bool StateSet<T>::remove(T removeState)
    {
      if( states.count(removeState) == 0 )
        return false;
      states.erase(removeState);
      return true;
    }
    
    /**
     * TODO
     */
    template <typename T> 
    bool StateSet<T>::removeState(T removeState)
    {
      if( states.count(removeState) == 0 )
        return false;
      states.erase(removeState);
      return true;
    }
    
    /**
     * TODO
     */
    template <typename T> 
    bool StateSet<T>::removeInitialState(T removeInitialState)
    {
      if( initialStates.count(removeInitialState) == 0 )
        return false;
      initialStates.erase(removeInitialState);
      return true;
    }
    
    /**
     * TODO
     */
    template <typename T> 
    bool StateSet<T>::removeFinalState(T removeFinalState)
    {
      if( finalStates.count(removeFinalState) == 0 )
        return false;
      finalStates.erase(removeFinalState);
      return true;
    }
      
    //Utilities	

    /**
     *
     * @brief print the collection of states
     *
     * @param the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template <typename T> 
    std::ostream & StateSet<T>::print( std::ostream & o) const
    {
      o << "Q: ";
      o << "{ ";
      std::set<T>::const_iterator it = states.begin();
      std::set<T>::const_iterator itEND = states.end();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        it->print(o);
      }
      o << " }" << std::endl;
      
      o << "Q0: ";
      o << "{ ";
      it = initialStates.begin();
      itEND = initialStates.end();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        it->print(o);
      }
      o << " }" << std::endl;
      
      o << "Qf: ";
      o << "{ ";
      it = finalStates.begin();
      itEND = finalStates.end();
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
     * @brief tests whether this collection of states is equivalent 
     * to the collection of states 'other'
     *
     * @param the StateSet to compare this StateSet to
     * @return true if this StateSet is equivalent to the StateSet 
     * 'other'
     *
     */
    template <typename T> 
    bool StateSet<T>::operator==( const StateSet<T> & other )
    {
      for( std::set<T>::const_iterator it = states.begin(); it != states.end(); it++ )
        if( other.states.count(*it) == 0 )
          return false;
          
      for( std::set<T>::const_iterator it = other.states.begin(); it != other.states.end(); it++ )
        if( states.count(*it) == 0 )
          return false;
          
      for( std::set<T>::const_iterator it = initialStates.begin(); it != initialStates.end(); it++ )
        if( other.initialStates.count(*it) == 0 )
          return false;
          
      for( std::set<T>::const_iterator it = other.initialStates.begin(); it != other.initialStates.end(); it++ )
        if( initialStates.count(*it) == 0 )
          return false;
          
      for( std::set<T>::const_iterator it = finalStates.begin(); it != finalStates.end(); it++ )
        if( other.finalStates.count(*it) == 0 )
          return false;
          
      for( std::set<T>::const_iterator it = other.finalStates.begin(); it != other.finalStates.end(); it++ )
        if( finalStates.count(*it) == 0 )
          return false;        
          
      return true;
    }
    
    /**
     * TODO: remove
     * @brief provides access to the states in the collection 
     * through an iterator
     *
     * @return the starting point of an iterator through this collection
     * of states
     *
     */
    template <typename T> 
    typename StateSet<T>::iterator StateSet<T>::begin()
    {
      return states.begin();
    }
    
    /**
     * TODO
     */
    template <typename T> 
    typename StateSet<T>::iterator StateSet<T>::beginStates()
    {
      return states.begin();
    }
    
    /**
     * TODO
     */
    template <typename T> 
    typename StateSet<T>::iterator StateSet<T>::beginInitialStates()
    {
      return initialStates.begin();
    }
    
    /**
     * TODO
     */
    template <typename T> 
    typename StateSet<T>::iterator StateSet<T>::beginFinalStates()
    {
      return finalStates.begin();
    }
      
    /**
     * TODO: remove
     * @brief provides access to the states in the collection 
     * through an iterator
     *
     * @return the exit point of an iterator through this collection of
     * states
     *
     */
    template <typename T> 
    typename StateSet<T>::iterator StateSet<T>::end()
    {
      return states.end();
    }
    
    /**
     * TODO
     */
    template <typename T> 
    typename StateSet<T>::iterator StateSet<T>::endStates()
    {
      return states.end();
    }
    
    /**
     * TODO
     */
    template <typename T> 
    typename StateSet<T>::iterator StateSet<T>::endInitialStates()
    {
      return initialStates.end();
    }
    
    /**
     * TODO
     */
    template <typename T> 
    typename StateSet<T>::iterator StateSet<T>::endFinalStates()
    {
      return finalStates.end();
    }

    /**
     * TODO: remove
     * @brief returns the number of states in this collection
     *
     * @return the number of states in this collection
     *
     */
    template <typename T> 
    size_t StateSet<T>::size( ) const
    {
      return states.size();
    }
    
    /**
     * TODO
     */
    template <typename T> 
    size_t StateSet<T>::sizeStates( ) const
    {
      return states.size();
    }
    
    /**
     * TODO
     */
    template <typename T> 
    size_t StateSet<T>::sizeInitialStates( ) const
    {
      return initialStates.size();
    }
    
    /**
     * TODO
     */
    template <typename T> 
    size_t StateSet<T>::sizeFinalStates( ) const
    {
      return finalStates.size();
    }
    
    /**
     * TODO: remove
     * @brief tests whether the given state is a member of this
     * collection of states
     *
     * @parm the state to test
     * @return true if the state is a member of this collection of
     * states, false otherwise
     *
     */
    template <typename T> 
    bool StateSet<T>::contains( T state ) const
    {
      return (states.count(state) >  0);
    }
    
    /**
     * TODO
     */
    template <typename T> 
    bool StateSet<T>::containsState( T state )
    {
      return (states.count(state) >  0);
    }
    
    /**
     * TODO
     */
    template <typename T> 
    bool StateSet<T>::containsInitialState( T initialState )
    {
      return (initialStates.count(state) >  0);
    }
    
    /**
     * TODO
     */
    template <typename T> 
    bool StateSet<T>::containsFinalState( T finalState )
    {
      return (finalStates.count(state) >  0);
    }
  }
}
#endif