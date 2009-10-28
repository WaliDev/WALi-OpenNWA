#ifndef wali_nwa_Symbol_GUARD
#define wali_nwa_Symbol_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Common.hpp"
#include "wali/Key.hpp"

namespace wali
{

  namespace nwa
  {
  
    template<typename T>
    class Symbol : public Printable
    {
      //
      // Methods
      //

    public:
      //Constructors and Destructor
      Symbol( ) { }
      Symbol( T lbl );
      Symbol( const Symbol & other );
      Symbol & operator=( const Symbol & other );
  
      ~Symbol( ) { }


      //Accessors
      
      /**
       *
       */
      static Key getEpsilon();
      
      
      /**
       *
       */
      Key getLabelKey();
      
      /**
       *  TODO: remove this!!!
       */
      void setLabelKey(Key newKey);
      
      /** 
       *
       */
      T getLabel( );
      
      /** 
       *
       */
      void setLabel( T lbl );

      virtual bool intersect( Symbol other )
      {
        if( symbolKey == other.symbolKey )
          return true;
        else
          return false;
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
      bool operator==( const Symbol & other );
      
      /**
       *
       */
      bool operator<( const Symbol & rhs ) const;

      //
      // Variables
      //
    protected:
      T lbl;
      Key symbolKey;
    };
    
    template<typename T>
    Symbol<T>::Symbol( T lbl )
    {
      this->lbl = lbl;
      symbolKey = wali::getKey(lbl);
    }
    
    template<typename T>
    Symbol<T>::Symbol(const Symbol &other)
    {
      lbl = other.lbl;
      symbolKey = other.symbolKey;
    }
    
    template<typename T>
    Symbol<T> & Symbol<T>::operator=( const Symbol & other )
    {
      symbolKey = other.symbolKey;
      
      return *this;
    }
    
    template<typename T>
    Key Symbol<T>::getEpsilon()
    {
      return wali::WALI_EPSILON;
    }
    
    template<typename T>
    Key Symbol<T>::getLabelKey()
    {
      return symbolKey;
    }
    
    //TODO: remove this once I figure out how to make a pair of two labels
    //have the right type?
    template<typename T>
    void Symbol<T>::setLabelKey(Key newKey)
    {
      symbolKey = newKey;
    }
    
    template<typename T>
    typename T Symbol<T>::getLabel()
    {
      return lbl;
    }
    
    template<typename T>
    void Symbol<T>::setLabel(T lbl)
    {
      //TODO: fix this to reflect templated labels
      this->lbl = lbl;
      symbolKey = wali::getKey(lbl);
    }
    
    template<typename T>
    std::ostream & Symbol<T>::print(std::ostream &o) const
    {
      printKey(o,symbolKey);
      return o;
    }
    
    template<typename T>
    bool Symbol<T>::operator==( const Symbol & other )
    {
      if( symbolKey == other.symbolKey )
        return true;
      else
        return false;
    }
    
    template<typename T>
    bool Symbol<T>::operator<( const Symbol & rhs ) const
    {
      return (symbolKey < rhs.symbolKey);  
    }
  }
}
#endif