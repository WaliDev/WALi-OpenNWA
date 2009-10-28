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
         *
         * @brief removes all states from this collection of states
         *
         * This method removes all states from this collection of
         * states.
         *
         */
        void clear( );
      
        /**
         *
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
         *
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
         *
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
         *
         * @brief returns the number of states in this collection
         *
         * This method returns the number of states in this collection
         * of states.
         *
         * @return the number of states in this collection
         *
         */
        size_t size( );
      
        /**
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
        bool contains( T state );

      //
      // Variables
      //
      protected:
        std::set<T> states;
      
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
    }
    
    template <typename T> 
    StateSet<T> & StateSet<T>::operator=( const StateSet<T> & other )
    {
      states = other.states;
      return *this;
    }
    
    template <typename T> 
    StateSet<T>::~StateSet( ) {  }

    //State Accessors

    /**
     *
     * @brief removes all states from this collection of states
     *
     */
    template <typename T> 
    void StateSet<T>::clear( )
    {
      states.clear();
    }
      
    /**
     *
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
     *
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
    }
      
    /**
     *
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
      for( iterator it = states.begin(); it != states.end(); it++ )
        if( other.states.count(*it) == 0 )
          return false;
          
      for( std::set<T>::const_iterator it = other.states.begin(); it != other.states.end(); it++ )
        if( states.count(*it) == 0 )
          return false;
          
      return true;
    }
    
    /**
     *
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
     *
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
     *
     * @brief returns the number of states in this collection
     *
     * @return the number of states in this collection
     *
     */
    template <typename T> 
    size_t StateSet<T>::size( )
    {
      return states.size();
    }
    
    /**
     * @brief tests whether the given state is a member of this
     * collection of states
     *
     * @parm the state to test
     * @return true if the state is a member of this collection of
     * states, false otherwise
     *
     */
    template <typename T> 
    bool StateSet<T>::contains( T state )
    {
      return (states.count(state) >  0);
    }
  }
}
#endif