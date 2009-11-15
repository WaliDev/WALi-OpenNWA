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
#include <map>

namespace wali
{
  namespace nwa
  {
    template <typename St,typename StName> 
    class StateSet : public Printable
    {
      //TODO: update comments
    
      public:
        typedef typename std::set<St*>::iterator iterator;    
        typedef std::map<StName,St*> StMap;

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
        //void clear( );
        
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
        //bool add(St addState);
        
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
        bool addState(St* addState);
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
        bool addInitialState(St* addInitialState);
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
        bool addFinalState(St* addFinalState);
      
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
        void addAll(StateSet<St,StName> addStateSet);
        
        /**
         *
         * @brief add all the states in the given StateSet 
         *
         * This method adds all of the given states to the state set.
         *
         * @param the StateSet that contains the states to add
         *
         */
        void addAllStates(StateSet<St,StName> addStateSet);
        /**
         * TODO
         */
        void addAllInitialStates(StateSet<St,StName> addStateSet);
        /**
         * TODO
         */
        void addAllFinalStates(StateSet<St,StName> addStateSet);
      
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
        //bool remove(St removeState);
        
        /**
         * TODO
         */
        bool removeState(St* removeState);
        /**
         * TODO
         */
        bool removeInitialState(St* removeInitialState);
        /**
         * TODO
         */
        bool removeFinalState(St* removeFinalState);
      
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
        bool operator==( const StateSet<St,StName> & other ) const;

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
        //iterator begin();
        
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
        //iterator end();
        
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
        //size_t size( ) const;
        
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
        //bool contains( St state ) const;
        
        /**
         * TODO
         */
        bool containsState( St* state ) const;
        /**
         * TODO
         */
        bool containsInitialState( St* initialState ) const;
        /**
         * TODO
         */
        bool containsFinalState( St* finalState ) const;
        
        /**
         * TODO
         */
        St* getState( StName name ) const;

        /**
         * TODO
         */
        void dupState(St* orig, St* dup);

        /**
         * TODO
         */
        std::set<St*> getStates() const;

        /**
         * TODO
         */
        std::set<StName> getStateNames() const;
        
        /**
         * TODO
         */
        void clearMap();

      //
      // Variables
      //
      protected:
        std::set<StName> names;
        std::set<St*> states;
        std::set<St*> initialStates;  
        std::set<St*> finalStates;   
        
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
      clearStates();
      
      names = other.names;
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
    }

    //State Accessors
    
    /**
     * TODO
     */ 
    template <typename St,typename StName> 
    void StateSet<St,StName>::clearStates( )
    {
      names.clear();
      states.clear();
      clearInitialStates();
      clearFinalStates();
      
      clearMap();
    }    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    void StateSet<St,StName>::clearInitialStates( )
    { 
      initialStates.clear();
    }    
    /**
     * TODO
     */ 
    template <typename St,typename StName> 
    void StateSet<St,StName>::clearFinalStates( )
    {
      finalStates.clear();
    }
    
    /**
     * TODO
     */ 
    template <typename St,typename StName> 
    bool StateSet<St,StName>::addState(St* addState)
    {
      if( states.count(addState) > 0 )
        return false;
      states.insert(addState);
      names.insert(addState->getName());
      name_St.insert(std::pair<StName,St*>(addState->getName(),addState));
      return true;
    }    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::addInitialState(St* addInitialState)
    {
      if( initialStates.count(addInitialState) > 0 )
        return false;
      std::set<St*>::iterator state = states.find(addInitialState);
      if( state == states.end() )
      {
        addState(addInitialState);
        state = states.find(addInitialState);
      }

      initialStates.insert(*state);
      return true;
    }    
    /**
     * TODO
     */ 
    template <typename St,typename StName> 
    bool StateSet<St,StName>::addFinalState(St* addFinalState)
    {
      if( finalStates.count(addFinalState) > 0 )
        return false;
      std::set<St*>::iterator state = states.find(addFinalState);
      if( state == states.end() )
      {
        addState(addFinalState);
        state = states.find(addFinalState);
      }

      finalStates.insert(*state);
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
    template <typename St,typename StName> 
    void StateSet<St,StName>::addAll(StateSet<St,StName> addStateSet)
    {
      addAllStates(addStateSet);
      addAllInitialStates(addStateSet);
      addAllFinalStates(addStateSet);
    }
    
    /**
     * TODO
     */ 
    template <typename St,typename StName> 
    void StateSet<St,StName>::addAllStates(StateSet<St,StName> addStateSet)
    {
      for( iterator it = addStateSet.states.begin();
            it != addStateSet.states.end(); it++ )
      {
        addState(*it);
      }
    }    
    /**
     * TODO
     */ 
    template <typename St,typename StName> 
    void StateSet<St,StName>::addAllInitialStates(StateSet<St,StName> addStateSet)
    {
      for( iterator it = addStateSet.initialStates.begin();
            it != addStateSet.initialStates.end(); it++ )
      {
        addInitialState(*it);
      }
    }
    /**
     * TODO
     */
    template <typename St,typename StName> 
    void StateSet<St,StName>::addAllFinalStates(StateSet<St,StName> addStateSet)
    {
      for( iterator it = addStateSet.finalStates.begin();
            it != addStateSet.finalStates.end(); it++ )
      {
        addFinalState(*it);
      }
    }
    
    /**
     * TODO
     */ 
    template <typename St,typename StName> 
    bool StateSet<St,StName>::removeState(St* removeState)
    {
      if( states.count(removeState) == 0 )
        return false;
      states.erase(removeState);
      names.erase(removeState->getName());
      name_St.erase(name_St.find(removeState->getName()));
      //remove initial
      removeInitialState(removeState);
      //remove final
      removeFinalState(removeState);
      return true;
    }    
    /**
     * TODO
     */ 
    template <typename St,typename StName> 
    bool StateSet<St,StName>::removeInitialState(St* removeInitialState)
    {
      if( initialStates.count(removeInitialState) == 0 )
        return false;
      initialStates.erase(removeInitialState);
      return true;
    }    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::removeFinalState(St* removeFinalState)
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
    template <typename St,typename StName> 
    std::ostream & StateSet<St,StName>::print( std::ostream & o) const
    {
      o << "Q: ";
      o << "{ ";
      std::set<St*>::const_iterator it = states.begin();
      std::set<St*>::const_iterator itEND = states.end();
      for( bool first=true; it != itEND ; it++,first=false )
      {
        if( !first )
        o << ", ";
        (*it)->print(o);
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
        (*it)->print(o);
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
        (*it)->print(o);
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
    template <typename St,typename StName> 
    bool StateSet<St,StName>::operator==( const StateSet<St,StName> & other ) const
    {
      for( std::set<St*>::const_iterator it = states.begin(); it != states.end(); it++ )
        if( other.states.count(*it) == 0 )
          return false;
          
      for( std::set<St*>::const_iterator it = other.states.begin(); it != other.states.end(); it++ )
        if( states.count(*it) == 0 )
          return false;
          
      for( std::set<St*>::const_iterator it = initialStates.begin(); it != initialStates.end(); it++ )
        if( other.initialStates.count(*it) == 0 )
          return false;
          
      for( std::set<St*>::const_iterator it = other.initialStates.begin(); it != other.initialStates.end(); it++ )
        if( initialStates.count(*it) == 0 )
          return false;
          
      for( std::set<St*>::const_iterator it = finalStates.begin(); it != finalStates.end(); it++ )
        if( other.finalStates.count(*it) == 0 )
          return false;
          
      for( std::set<St*>::const_iterator it = other.finalStates.begin(); it != other.finalStates.end(); it++ )
        if( finalStates.count(*it) == 0 )
          return false;        
          
      return true;
    }
    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::beginStates()
    {
      return states.begin();
    }    
    /**
     * TODO
     */ 
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::beginInitialStates()
    {
      return initialStates.begin();
    }    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::beginFinalStates()
    {
      return finalStates.begin();
    }
    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::endStates()
    {
      return states.end();
    }    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::endInitialStates()
    {
      return initialStates.end();
    }    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    typename StateSet<St,StName>::iterator StateSet<St,StName>::endFinalStates()
    {
      return finalStates.end();
    }
    
    /**
     * TODO
     */ 
    template <typename St,typename StName> 
    size_t StateSet<St,StName>::sizeStates( ) const
    {
      return states.size();
    }    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    size_t StateSet<St,StName>::sizeInitialStates( ) const
    {
      return initialStates.size();
    }    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    size_t StateSet<St,StName>::sizeFinalStates( ) const
    {
      return finalStates.size();
    }
    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::containsState( St* state ) const
    {
      return (states.count(state) >  0);
    }    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::containsInitialState( St* initialState ) const
    {
      return (initialStates.count(initialState) >  0);
    }    
    /**
     * TODO
     */
    template <typename St,typename StName> 
    bool StateSet<St,StName>::containsFinalState( St* finalState ) const
    {
      return (finalStates.count(finalState) >  0);
    }
    
    /**
     *
     */
    template <typename St,typename StName> 
    typename St* StateSet<St,StName>::getState( StName name ) const
    {
      StMap::const_iterator it = name_St.find(name);
      if( it != name_St.end() )
        return it->second;
      else
        return NULL;
    }

    /**
     * TODO
     */
    template <typename St,typename StName>
    void StateSet<St,StName>::dupState(St* orig, St* dup)
    {
      if( containsInitialState(orig) )
        addInitialState(dup);

      if( containsFinalState(orig) )
        addFinalState(dup);
    }

    /**
     * TODO
     */
    template <typename St,typename StName>
    std::set<St*> StateSet<St,StName>::getStates() const
    {
      return states;
    }

    /**
     * TODO
     */
    template <typename St,typename StName>
    std::set<StName> StateSet<St,StName>::getStateNames() const
    {
      return names;
    }
    
    /**
     *
     */
    template <typename St,typename StName> 
    void StateSet<St,StName>::clearMap()
    {
      name_St.clear();
    }
  }
}
#endif