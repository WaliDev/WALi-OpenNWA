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
      State( T key );
      State( const State & other );
      State & operator=( const State & other );
  
      ~State( ) { }


      //Accessors
      
      /**
       *
       */
      static Key getBadKey();
      
      /**
       *
       */
      Key getStateKey();
      
      /**
       *  TODO: remove this!!!
       */
      void setStateKey(Key newKey);
      
      /** 
       *
       */
      T getKey( );
      
      /** 
       *
       */
      void setKey( T key );

      virtual bool intersect( State other, State & result )
      {      
        //Join the two states.
        Key newKey = wali::getKey(stateKey,other.stateKey);  
      
        result.setKey( newKey );
      
        return true;
      }
      
      //Utilities
      
      /** 
       *
       * @parm the output stream to which to print the node
       * @return the output stream to which the node was printed
       *
       */
      std::ostream & print( std::ostream & o ) const;
      
      /** 
       *
       */
      bool operator==( const State & other ) const;
      
      /**
       *
       */
      bool operator<( const State & rhs ) const;

      //
      // Variables
      //
    protected:
      T key;
      Key stateKey;
    };
    
    template<typename T>
    State<T>::State( T key )
    {
      this->key = key;
      stateKey = wali::getKey(key);
    }
    
    template<typename T>
    State<T>::State( const State & other )
    {
      key = other.key;
      stateKey = other.stateKey;
    }
    
    template<typename T>
    State<T> & State<T>::operator=( const State & other )
    {
      key = other.key;
      stateKey = other.stateKey;
      
      return *this;
    }
         
    template<typename T>        
    Key State<T>::getBadKey()
    {
      return wali::WALI_BAD_KEY;
    }
    
    template<typename T>
    Key State<T>::getStateKey()
    {
      return stateKey;
    }
    
    //TODO: remove this once I figure out how to make a pair of two labels
    //have the right type?
    template<typename T>
    void State<T>::setStateKey(Key newKey)
    {
      stateKey = newKey;
    }
    
    template<typename T>
    typename T State<T>::getKey()
    {
      return key;
    }
    
    template<typename T>
    void State<T>::setKey(T key)
    {
      this->key = key;
      stateKey = wali::getKey(key);
    }
    
    template<typename T>
    std::ostream & State<T>::print( std::ostream & o ) const
    {
      if(! (stateKey == getBadKey()) )
        printKey(o,stateKey);
      return o;
    }
    
    template<typename T>
    bool State<T>::operator==( const State & other ) const
    {
      if( stateKey == other.stateKey )
        return true;
      else
        return false;
    }

    template<typename T>
    bool State<T>::operator<( const State & rhs ) const
    {
      return (stateKey < rhs.stateKey);  
    }
  }
}
#endif