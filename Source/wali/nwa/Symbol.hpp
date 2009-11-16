#ifndef wali_nwa_Symbol_GUARD
#define wali_nwa_Symbol_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Common.hpp"
#include "wali/Key.hpp"
#include "wali/KeySource.hpp"

namespace wali
{

  namespace nwa
  {
  
    template<typename T>
    class Symbol : public Printable
    {
    
      //
      // Nested Class
      //
      class SymbolSource : public KeySource
      {
        //TODO: write comments
        public:
          SymbolSource( const T _lbl ):lbl(_lbl)
          {
          }
          
          ~SymbolSource() { }
          
          /**
           * TODO: write comments
           */
          bool equal( KeySource * rhs )
          {
            SymbolSource *ssrc = dynamic_cast< SymbolSource* >(rhs);
            if( ssrc != 0 )
              return lbl == ssrc->lbl;
            else
              return false;
          }
          
          /**
           * TODO: write comments
           */
          std::ostream& print( std::ostream& o ) const {
            return lbl.print(o);
          }

          /**
           * TODO: write comments
           */
          size_t hash() const
          {
            return lbl.hash();  
          }
          
          /**
           * TODO: write comments
           */
          T getLabel() const
          {
            return lbl;
          }
          
        private:
          const T lbl;
      };
    
      //
      // Methods
      //

    public:
      //Constructors and Destructor
      Symbol( );
      Symbol( Key key );
      Symbol( T lbl );
      Symbol( const Symbol & other );
      Symbol & operator=( const Symbol & other );
  
      ~Symbol( ) { }


      //Accessors
      
      /**
       *
       * @brief access the epsilon symbol
       * 
       * This method returns the epsilon symbol.
       *
       * @return the epsilon symbol
       *
       */
      static Symbol<T>* getEpsilon();
      
      /**
       *  TODO: write comments
       */
      bool isEpsilon() const;  
      
      /**
       * TODO: write comments
       */
      static Symbol<T>* getWild();
      
      /**
       *  TODO: write comments
       */
      bool isWild() const;      
      
      /**
       *
       * @brief access the Key associated with this symbol
       *
       * This method provides access to the wali::Key associated
       * with this symbol.
       * 
       * @return the Key associated with this symbol
       *
       */
      Key getLabelKey() const;
      
      /**
       *  TODO: remove this, it isn't safe to allow!!!
       * @brief set the Key associated with this symbol
       *
       */
      //void setLabelKey(Key newKey);
      
      /** 
       *
       * @brief access the label associated with this symbol
       *
       * This method provides access to the label associated with
       * this symbol.
       *
       * @return the label associated with this symbol
       *
       */
      T getLabel( ) const;
      
      /** 
       *
       * @brief set the label associated with this symbol
       *
       * This method sets the label associated with this symbol
       * to the label provided and updates the key associated
       * with this symbol to reflect the change.
       *
       * @param the desired label for this symbol
       * 
       */
      //void setLabel( T lbl );


      //Intersection of edge labels
      /**
       *
       * @brief creates the edge that is the intersection of this edge with the 
       * given edge 'other'
       *
       * This method checks that this edge and the given edge 'other' can be 
       * intersected and creates the resulting edge.  If these two edges can 
       * be intersected, true is returned and the resulting edge is passed 
       * back via the address 'result'.  Otherwise, false is returned and no
       * new edge is created.
       * 
       * @parm other: the edge to intersect with this edge
       * @parm result: the  address to use in passing back the joint edge created
       * when it is possible to intersect the two edges.
       * @result true if the two edges can be intersected, false otherwise
       *
       */
      /*virtual bool intersect( Symbol* other, Symbol & result )
      //bool intersect( Symbol other, Symbol & result )
      {
        //Note: When overriding this method your metric must determine an
        // appropriate label, create a symbol with that label, and set result
        // to the symbol just created.
      
        if( isWild() )  //If we have a wild symbol, whatever the other symbol is survives (even if it is also wild).
        {
          result = *other;
          return true;
        }
        else if( other->isWild() ) //If the other symbol is wild, whatever this symbol is survives. 
        {
          result = *this;
          return true;
        }
        else if( symbolKey == other->symbolKey ) //This rule still applies for epsilons
        {
          result = *this;
          return true;
        }
        else
          return false;
      }*/
      
      //Utilities
      
      /** 
       *
       * @brief print the Symbol
       *
       * This method prints out the Symbol to the output stream provided.
       *
       * @parm the output stream to which to print the Symbol
       * @return the output stream to which the Symbol was printed
       *
       */
      std::ostream & print( std::ostream & o ) const;
      
      /** 
       *
       * @brief tests whether this Symbol is equivalent to the Symbol 'other'.
       *
       * This method tests the equivalence of this Symbol and the Symbol
       * 'other'.
       *
       * @param the Symbol to compare this Symbol to
       * @return true if this Symbol is equivalent to the Symbol 'other'
       *
       */
      bool operator==( const Symbol & other ) const;
      
      /**
       *
       * @brief tests the relationship between this Symbol and the Symbol 
       * 'other'.
       *
       * This method tests whether this Symbol is 'less than' the Symbol
       * 'other' in some way.  The default is to order the Symbols based
       * on their key value.
       *
       * @param the Symbol to compare this Symbol to
       * @return true if this Symbol is 'less than' the Symbol 'other'.
       *
       */
      bool operator<( const Symbol & rhs ) const;

      //
      // Variables
      //
    protected:
      T lbl;
      Key symbolKey;
      static Symbol<T> wild;
      static Symbol<T> epsilon;
    };
    
    template<typename T>
    Symbol<T> Symbol<T>::wild = Symbol<T>::Symbol(wali::WALI_BAD_KEY);
    
    template<typename T>
    Symbol<T> Symbol<T>::epsilon = Symbol<T>::Symbol(wali::WALI_EPSILON);
    
    //Constructors
    template<typename T>
    Symbol<T>::Symbol()
    {
      *this = wild;
    }
    
    template<typename T>
    Symbol<T>::Symbol(Key key)
    {
      symbolKey = key;
    }
    
    template<typename T>
    Symbol<T>::Symbol( T lbl )
    {
      SymbolSource *s = new SymbolSource(lbl);
      this->lbl = lbl;
      symbolKey = wali::getKey(s);
    }
    
    template<typename T>
    Symbol<T>::Symbol(const Symbol &other)
    {
      //if( other.isWild() )
      //  *this = wild;
      //else
      //{
        lbl = other.lbl;
        symbolKey = other.symbolKey;
      //}
    }
    
    template<typename T>
    Symbol<T> & Symbol<T>::operator=( const Symbol & other )
    {
      //if( other.isWild() )
      //  *this = wild;
      //else
      //{
        lbl = other.lbl;
        symbolKey = other.symbolKey;
      //}
      return *this;
    }
    
    /**
     *
     * @brief access the epsilon symbol
     * 
     * @return the epsilon symbol
     *
     */
    template<typename T>
    Symbol<T>* Symbol<T>::getEpsilon()
    {
      return &epsilon;
    }
    
    /**
     *  TODO: write comments
     */
    template<typename T>
    bool Symbol<T>::isEpsilon() const
    {
      if( symbolKey == wali::WALI_EPSILON )
        return true;
      else
        return false;
    }  
    
    /**
     *  TODO: write comments
     */
    template<typename T>
    Symbol<T>* Symbol<T>::getWild()
    {
      return &wild;
    }
    
    /**
     *  TODO: write comments
     */
    template<typename T>
    bool Symbol<T>::isWild() const
    {
      if( symbolKey == wali::WALI_BAD_KEY )
        return true;
      else
        return false;
    }
    
    /**
     *
     * @brief access the Key associated with this symbol
     *
     * @return the Key associated with this symbol
     *
     */
    template<typename T>
    Key Symbol<T>::getLabelKey() const
    {
      //TODO: Q: do we want to allow anybody to ask this question?
      return symbolKey;
    }
    
    //TODO: remove this once I figure out how to make a pair of two labels
    //have the right type?
    /**
     *  TODO: remove this, it isn't a safe operation!!!
     * @brief set the Key associated with this symbol
     *
     */
    /*template<typename T>
    void Symbol<T>::setLabelKey(Key newKey)
    {
      symbolKey = newKey;
    }*/
    
    /** 
     *
     * @brief access the label associated with this symbol
     *
     * @return the label associated with this symbol
     *
     */
    template<typename T>
    typename T Symbol<T>::getLabel() const
    {
      //TODO: Q: what should wild and epsilon return from this?
      return lbl;
    }
    
    /** 
     *
     * @brief set the label associated with this symbol
     *
     * @param the desired label for this symbol
     * 
     */
    /*template<typename T>
    void Symbol<T>::setLabel(T lbl)
    {
      //TODO: fix this to reflect templated labels
      this->lbl = lbl;
      symbolKey = wali::getKey(lbl);
    }*/
    
    /** 
     *
     * @brief print the Symbol
     *
     * @parm the output stream to which to print the Symbol
     * @return the output stream to which the Symbol was printed
     *
     */
    template<typename T>
    std::ostream & Symbol<T>::print(std::ostream &o) const
    {
      if( isWild() )
        o << "wild";
      else if( isEpsilon() )
        o << "epsilon";
      else
        printKey(o,symbolKey);
      return o;
    }
    
    /** 
     *
     * @brief tests whether this Symbol is equivalent to the Symbol 'other'.
     *
     * @param the Symbol to compare this Symbol to
     * @return true if this Symbol is equivalent to the Symbol 'other'
     *
     */
    template<typename T>
    bool Symbol<T>::operator==( const Symbol & other ) const
    {
      if( isWild() )
        return other.isWild();
      else if( isEpsilon() )
        return other.isEpsilon();
      else if( symbolKey == other.symbolKey )
        return true;
      else
        return false;
    }
    
    /**
     *
     * @brief tests the relationship between this Symbol and the Symbol 'other'.
     *
     * @param the Symbol to compare this Symbol to
     * @return true if this Symbol is 'less than' the Symbol 'other'.
     *
     */
    template<typename T>
    bool Symbol<T>::operator<( const Symbol & rhs ) const
    {
      return (symbolKey < rhs.symbolKey);  
    }
  }
}
#endif