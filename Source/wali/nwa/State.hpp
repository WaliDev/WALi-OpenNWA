#ifndef wali_nwa_State_GUARD
#define wali_nwa_State_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Common.hpp"
#include "wali/Key.hpp"

// ::std
#include <utility> 

namespace wali
{

  namespace nwa
  {
  
    template<typename T>
    class State : public Printable
    {
      //
      // Methods
      //

    public:
      //Constructors and Destructor
      State( ) { }
      State( T name );
      State( const State & other );
      State & operator=( const State & other );
  
      ~State( ) { }


      //Accessors
      
      /**
       *  TODO: phase this out, instead use getStuckState()?
       */
      static Key getBadKey();
      
      /**
       *
       * @brief access the stuck state
       *
       * This method returns the stuck state.
       *
       * @return the stuck state
       *
       */
      static State<T> getStuckState();
      
      /**
       *
       * @brief access the Key associated with this state
       *
       * This method provides access to the wali::Key associated
       * with this symbol.
       *
       * @return the Key associated with this state
       *
       */
      Key getStateKey();
      
      /**
       *  TODO: remove this, it isn't safe to allow!!!
       * @brief set the Key associated with this symbol
       */
      void setStateKey(Key newKey);
      
      /** 
       *  TODO: rename to getName()
       * @brief access the name associated with this state
       *
       * This method provides access to the name associated with this
       * state.
       *
       * @return the name associated with this state
       *
       */
      T getKey( );
      T getName( );
      
      /** 
       *  TODO: rename to setName( T name )
       * @brief set the name associated with this state
       *
       * This method sets the name associated with this state to the
       * name provided and updates the Key associated with this state
       * to reflect the change.
       *
       * @param the desired name for this state
       *
       */
      void setKey( T key );
      void setName( T name );

      //Intersection of states
      /**
       *
       * @brief creates the state that is the join of this state with the
       * given state 'other'
       *
       * This method checks that this state and the given state 'other' can 
       * be joined and creates the resulting state.  If these two states can 
       * be joined, true is returned and the resulting state is passed back
       * via the address 'result'.  Otherwise, false is returned and no
       * new state is created.
       *
       * @param other: the state to join with this state
       * @param result: the address to use in passing back the joint state 
       * created when it is possible to join the two states
       * @result true if the two states can be joined, false otherwise
       */
      virtual bool intersect( State other, State & result )
      //bool intersect( State other, State & result )
      {      
        //Join the two states.
        Key newKey = wali::getKey(stateKey,other.stateKey);  
      
        result.setStateKey( newKey );
      
        return true;
      }
      
      //Utilities
      
      /** 
       *
       * @brief print the State
       *
       * This method prints out the State to the output stream provided.
       *
       * @parm the output stream to which to print the State
       * @return the output stream to which the State was printed
       *
       */
      std::ostream & print( std::ostream & o ) const;
      
      /** 
       *
       * @brief tests whether this State is equivalent to the State 'other'.
       * 
       * This method tests the equivalence of this State and the State
       * 'other'.
       *
       * @param the State to compare this State to
       * @return true if this State is equivalent to the State 'other'
       *
       */
      bool operator==( const State & other ) const;
      
      /**
       *
       * @brief tests the relationship between this State and the State 
       * 'other'.
       *
       * This method tests whether this State is 'less than' the State 
       * 'other' in some way.  The default is to order the States based 
       * on their key value.
       *
       * @param the State to compare this State to
       * @return true if this State is 'less than' the State 'other'.
       */
      bool operator<( const State & rhs ) const;

      //
      // Variables
      //
    protected:
      T name;
      Key stateKey;
      
      /* TODO: add maps to speed up transition search
      set<InternalTrans*> intraOutgoing;
      set<InternalTrans*> intraIncoming;
      set<CallTrans*> callToEntry;
      set<CallTrans*> entryToCall;
      set<ReturnTrans*> retToExit;
      set<ReturnTrans*> exitToRet;
      set<ReturnTrans*> retToCall; 
      set<ReturnTrans*> callToRet;  
      */
    };
    
    //Constructors
    template<typename T>
    State<T>::State( T name )
    {
      this->name = name;
      stateKey = wali::getKey(name);
    }
    
    template<typename T>
    State<T>::State( const State & other )
    {
      name = other.name;
      stateKey = other.stateKey;
    }
    
    template<typename T>
    State<T> & State<T>::operator=( const State & other )
    {
      name = other.name;
      stateKey = other.stateKey;
      
      return *this;
    }
         
    /**
     *  TODO: phase this out, instead use getStuckState()?
     */     
    template<typename T>        
    Key State<T>::getBadKey()
    {
      return wali::WALI_BAD_KEY;
    }
    
    /**
     *
     * @brief access the stuck state
     *
     * @return the stuck state
     *
     */
    template<typename T>
    State<T> State<T>::getStuckState()
    {
      State<T> stuck;
      stuck.setStateKey(wali::WALI_BAD_KEY);
       
      return stuck;
    }
    
    /**
     *
     * @brief access the Key associated with this state
     *
     * @return the Key associated with this state
     *
     */
    template<typename T>
    Key State<T>::getStateKey()
    {
      return stateKey;
    }
    
    //TODO: remove this once I figure out how to make a pair of two labels
    //have the right type?
    /**
     *  TODO: remove this, it isn't safe to allow!!!
     * @brief set the Key associated with this symbol
     */
    template<typename T>
    void State<T>::setStateKey(Key newKey)
    {
      stateKey = newKey;
    }
    
    /** 
     *  TODO: rename to getName()
     * @brief access the name associated with this state
     *
     * @return the name associated with this state
     *
     */
    template<typename T>
    typename T State<T>::getKey()
    {
      return name;
    }
    template<typename T>
    typename T State<T>::getName()
    {
      return name;
    }
    
    /** 
     *  TODO: rename to setName( T name )
     * @brief set the name associated with this state
     *
     * @param the desired name for this state
     *
     */
    template<typename T>
    void State<T>::setKey(T key)
    {
      this->name = name;
      stateKey = wali::getKey(name);
    }
    template<typename T>
    void State<T>::setName(T name)
    {
      this->name = name;
      stateKey = wali::getKey(name);
    }
    
    /** 
     *
     * @brief print the State
     *
     * @parm the output stream to which to print the State
     * @return the output stream to which the State was printed
     *
     */
    template<typename T>
    std::ostream & State<T>::print( std::ostream & o ) const
    {
      if(! (stateKey == getBadKey()) )
        printKey(o,stateKey);
      return o;
    }
    
    /** 
     *
     * @brief tests whether this State is equivalent to the State 'other'.
     * 
     * @param the State to compare this State to
     * @return true if this State is equivalent to the State 'other'
     *
     */
    template<typename T>
    bool State<T>::operator==( const State & other ) const
    {
      if( stateKey == other.stateKey )
        return true;
      else
        return false;
    }

    /**
     *
     * @brief tests the relationship between this State and the State 
     * 'other'.
     *
     * @param the State to compare this State to
     * @return true if this State is 'less than' the State 'other'.
     */
    template<typename T>
    bool State<T>::operator<( const State & rhs ) const
    {
      return (stateKey < rhs.stateKey);  
    }
  }
}
#endif