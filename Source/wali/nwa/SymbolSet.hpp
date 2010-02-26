#ifndef wali_nwa_SymbolSet_GUARD
#define wali_nwa_SymbolSet_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Common.hpp"
#include "wali/Key.hpp"

// ::std
#include <assert.h>

namespace wali
{
  namespace nwa
  {  
    class SymbolSet : public Printable
    { 
      public:
        typedef Key Sym;
        typedef std::set<Sym>::const_iterator const_iterator;

      //
      // Methods
      //

      public:
      //Constructors and Destructor
      SymbolSet( );
      SymbolSet( const SymbolSet & other );
      SymbolSet & operator=( const SymbolSet & other );
  
      ~SymbolSet( ) { }

      //Accessors

      /**
       *  //TODO: remove this!
       */
      static bool isWild( Sym sym )
      {
        return (sym == wali::WALI_WILD);
      };

      /**
       *  //TODO: remove this!
       */
      static Sym getWild( )
      {
        return wali::WALI_WILD;
      }

      /**
       *
       * @brief returns the Key for the epsilon symbol
       *
       * This method provides access to the Key for the epsilon symbol.
       * The epsilon symbol is the same as wali::WALI_EPSILON.
       *
       * @return the Key for the epsilon symbol
       *
       */
      static Sym getEpsilon( )
      {
        return wali::WALI_EPSILON;
      };

      /**
       *  
       * @brief test whether the given symbol is the epsilon symbol
       * 
       * This method determines whether the given symbol is the epsilon symbol.
       *
       * @param - sym: the symbol to test
       * @return true if this symbol is the epsilon symbol, false otherwise
       *
       */
      static bool isEpsilon( Sym sym )
      {
        return (sym == wali::WALI_EPSILON);
      }

      /**
       *
       * @brief provides access to all symbols 
       *
       * This method provides access to all symbols in the form of a set of symbols.
       *
       * @return a set containing all symbols 
       * 
       */
      const std::set<Sym> & getSymbols( ) const;

      /**
       *
       * @brief tests whether the given symbol is a valid symbol
       *
       * This method determines whether the given symbol is a valid symbol in the 
       * symbol pool.  If it is, true is returned, otherwise, false is returned.
       *
       * @param - sym: the symbol to test
       * @return true if the given symbol is a valid symbol
       *
       */
      bool isSymbol( Sym sym ) const;
            
      /**
       *
       * @brief add the given symbol 
       *
       * This method adds the given symbol.  If the symbol already exists,
       * false is returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol to add
       * @return false if the symbol already exists, true otherwise
       *
       */
      bool addSymbol( Sym sym );

      /**
       *
       * @brief add all the symbols in the given collection to this collection 
       *
       * This method adds all of the symbols in the given collection of symbols to this
       * collection of symbols.
       *
       * @param - symSet: the collection of symbols to add to this collection of symbols
       *
       */
      void addAllSymbols( SymbolSet symSet );

      /** 
       *
       * @brief remove the given symbol
       *
       * This method removes the given symbol.  If the symbol does not exist, false is 
       * returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol to remove
       * @return false if the symbol does not exist, true otherwise
       *
       */
      bool removeSymbol( Sym sym );

      /**
       *
       * @brief remove all the symbols in the given collection from this collection
       *
       * This method removes all of the symbols in the given collection of symbols
       * from this collection of symbols.
       *
       * @param - symSet: the collection of symbols to remove from this collection 
       *
       */
      void removeAll( SymbolSet symSet );

      /**
       *
       * @brief removes all symbols 
       *
       * This method removes all symbols from this collection.  
       *
       */
      void clearSymbols( );

      //Utilities
      
      /** 
       *
       * @brief print the collection of symbols
       *
       * This method prints out the symbol set to the output stream provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to 
       *
       */
      std::ostream & print( std::ostream & o ) const;
      
      /** 
       *
       * @brief tests whether this collection of symbols is equivalent to the collection
       *        of symbols 'other'
       *
       * This method tests the equivalence of this set of symbols and the set of symbols
       * 'other'.
       *
       * @param - other: the SymbolSet to which to compare this SymbolSet 
       * @return true if this SymbolSet is equivalent to the SymbolSet 'other'
       *
       */
      bool operator==( const SymbolSet & other ) const;

       /**
       * 
       * @brief provides access to the symbols in the collection 
       *
       * This method provides access to the symbols in this collection through an iterator.
       *
       * @return the starting point of an iterator through the symbols
       *
       */
      const_iterator beginSymbols( ) const;

      /**
       * 
       * @brief provides access to the symbols in the collection 
       *
       * This method provides access to the symbols in the collection through an iterator.
       *
       * @return one place past the exit point of an iterator through the symbols
       *
       */
      const_iterator endSymbols( ) const;

      /**
       *
       * @brief returns the number of symbols in this collection
       *
       * This method returns the number of symbols in this collection.
       *
       * @return the number of symbols in this collection
       *
       */
      size_t sizeSymbols( ) const;

      private:
        std::set<Sym> symbols;
    };

    //Constructors
    SymbolSet::SymbolSet( ) { }
    
    SymbolSet::SymbolSet( const SymbolSet & other )
    {
      symbols = other.symbols;
    }

    SymbolSet & SymbolSet::operator=( const SymbolSet & other )
    {
      if( this == &other )     
        return *this;
      
      symbols = other.symbols;

      return *this;
    }

    //Accessors

    /**
     *
     * @brief provides access to all symbols 
     *
     * @return a set containing all symbols 
     * 
     */
    const std::set<typename SymbolSet::Sym> & SymbolSet::getSymbols( ) const
    {
      return symbols;  
    }

    /**
     *
     * @brief tests whether the given symbol is a valid symbol
     *
     * @param - sym: the symbol to test
     * @return true if the given symbol is a valid symbol
     *
     */
    inline
    bool SymbolSet::isSymbol( Sym sym ) const
    {
      return (symbols.count(sym) > 0);
    }
    
    /**
     *
     * @brief add the given symbol 
     *
     * @param - sym: the symbol to add
     * @return false if the symbol already exists, true otherwise
     *
     */
    bool SymbolSet::addSymbol( Key sym )
    {
      if( isSymbol(sym) )
        return false;
      else
      {
        symbols.insert(sym);
        return true;
      }
    }

    /**
     *
     * @brief add all the symbols in the given collection to this collection 
     *
     * @param - symSet: the collection of symbols to add to this collection of symbols
     *
     */
    inline
    void SymbolSet::addAllSymbols( SymbolSet symSet )
    {
      symbols.insert(symSet.symbols.begin(),symSet.symbols.end());
    }

    /** 
     *
     * @brief remove the given symbol
     *
     * @param - sym: the symbol to remove
     * @return false if the symbol does not exist, true otherwise
     *
     */
    bool SymbolSet::removeSymbol( Key sym )
    { 
      if(! isSymbol(sym) )
        return false;
      else
      {
        symbols.erase(symbols.find(sym));
        return true;
      }
    }

    /**
     *
     * @brief remove all the symbols in the given collection from this collection
     *
     * @param - symSet: the collection of symbols to remove from this collection 
     *
     */
    inline
    void SymbolSet::removeAll( SymbolSet symSet )
    {
      symbols.erase(symSet.symbols.begin(),symSet.symbols.end());
    }

    /**
     *
     * @brief removes all symbols 
     *
     * This method removes all symbols from this collection.  
     *
     */
    inline
    void SymbolSet::clearSymbols( )
    {
      symbols.clear();
    }

    //Utilities
    
    /** 
     *
     * @brief print the collection of symbols
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to 
     *
     */
    std::ostream & SymbolSet::print( std::ostream & o ) const
    {
     //Print the set of all symbols.
      o << "Sigma: ";
      o << "{ ";

      bool first = true;
      for( const_iterator it = symbols.begin(); it != symbols.end(); it++ )
      {
        if( !first )
        {
          o << ", "; 
          first = false;
        }

        if( SymbolSet::isEpsilon(*it) )
          o << "Epsilon";
        else
          printKey(o,*it);
      }

      o << " }";
      
      return o;
    }
    
    /** 
     *
     * @brief tests whether this collection of symbols is equivalent to the collection
     *        of symbols 'other'
     *
     * @param - other: the SymbolSet to which to compare this SymbolSet 
     * @return true if this SymbolSet is equivalent to the SymbolSet 'other'
     *
     */
    bool SymbolSet::operator==( const SymbolSet & other ) const
    {
      for( const_iterator it = symbols.begin(); it != symbols.end(); it++ )
      {
        if (! other.isSymbol(*it) )
          return false;
      }
      
      for( const_iterator it = other.symbols.begin(); it != other.symbols.end(); it++ )
      {
        if (! isSymbol(*it) )
          return false;
      }

      return true;
    }

    /**
     * 
     * @brief provides access to the symbols in the collection 
     *
     * @return the starting point of an iterator through the symbols
     *
     */
    inline
    SymbolSet::const_iterator SymbolSet::beginSymbols( ) const
    {
      return symbols.begin();
    } 

    /**
     * 
     * @brief provides access to the symbols in the collection 
     *
     * @return one place past the exit point of an iterator through the symbols
     *
     */
    inline
    SymbolSet::const_iterator SymbolSet::endSymbols( ) const
    {
      return symbols.end();
    }    

    /**
     *
     * @brief returns the number of symbols in this collection
     *
     * @return the number of symbols in this collection
     *
     */
    inline
    size_t SymbolSet::sizeSymbols( ) const
    {
      return symbols.size();
    }

    /**
     *
     *  This class is used to label the transitions of an NWA.
     *
     */
    class Label
    {
      public:
        typedef Key Sym;
        typedef std::set<Sym>::const_iterator const_iterator;
       
      //
      // Methods
      //

      public:
      //Constructors and Destructor
      //Note: The default starting state for a label is to represent the 
      //absence of symbols.  If the representation of wild is desired, then makeWild() 
      //must be called.  At any point the collecion can be returned to this default
      //state by calling makeAbsent().
      Label( );
      Label( const Label & other );
      Label & operator=( const Label & other );
  
      ~Label( ) { }

      /**
       *
       * @brief tests whether this collection represents the wild symbol
       *
       * This method determines whether this collection represents the wild symbol.
       * It returns true if the collection represents the wild symbol. 
       *
       * @return true if this collection represents the wild symbol
       *
       */
      bool isWild( const SymbolSet & symbolPool ) const;

      /**
       *
       * @brief make this collection represent wild
       *
       * This method erases any symbols currently in this collection and converts the 
       * collection to represent the wild symbol.
       *
       */
      void makeWild( );

      /**
       *
       * @brief tests whether this collection represents the absence of all symbols
       *
       * This method determines whether this collection represents the absence of all
       * symbols.  It returns true if the collection represents the absence of all 
       * symbols. 
       *
       * @return true if this collection represents the absence of all symbols
       *
       */
      bool isAbsent( const SymbolSet & symbolPool ) const;

      /**
       *
       * @brief make this collection represent the absence of all symbols
       *
       * This method erases any symbols currently in this collection and converts the
       * collection to represent the absence of all symbols.
       *
       */
      void makeAbsent( );

      /**
       *
       * @brief tests whether the given symbol is a member of this collection
       *
       * This method determines whether the given symbol is a member of this collection.
       * It returns true if the symbol is a member and false otherwise.
       *
       * @param - sym: the symbol to test
       * @return true if the symbol is a member of this collection of symbols
       *
       */
      bool containsSymbol( Sym sym ) const;

      /**
       *
       * @brief provides access to some symbol in this collection
       *
       * This method provides access to some symbol in this collection.
       *
       * @return some symbol in this collection of symbols
       *
       */
      Sym getAnySymbol( const SymbolSet & symbolPool ) const;
            
      /**
       *
       * @brief add the given symbol 
       *
       * This method adds the given symbol.  If the symbol already exists,
       * false is returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol to add
       * @return false if the symbol already exists, true otherwise
       *
       */
      bool addSymbol( Sym sym );

      /**
       *
       * @brief add all the symbols in the given collection to this collection 
       *
       * This method adds all of the symbols in the given collection of symbols to this
       * collection of symbols.
       *
       * @param - symSet: the collection of symbols to add to this collection of symbols
       *
       */
      void addAll( Label lbl, const SymbolSet & symbolPool );

      /** 
       *
       * @brief remove the given symbol
       *
       * This method removes the given symbol.  If the symbol does not exist, false is 
       * returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol to remove
       * @return false if the symbol does not exist, true otherwise
       *
       */
      bool removeSymbol( Sym sym );

      /**
       *
       * @brief remove all the symbols in the given collection from this collection
       *
       * This method removes all of the symbols in the given collection of symbols
       * from this collection of symbols.
       *
       * @param - symSet: the collection of symbols to remove from this collection 
       *
       */
      void removeAll( Label lbl, const SymbolSet & symbolPool );

      //Utilities
      
      /** 
       *
       * @brief print the collection of symbols
       *
       * This method prints out the symbol set to the output stream provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to 
       *
       */
      std::ostream & print( std::ostream & o, const SymbolSet & symbolPool ) const;
      
      /** 
       *
       * @brief tests whether this collection of symbols is equivalent to the collection
       *        of symbols 'other'
       *
       * This method tests the equivalence of this set of symbols and the set of symbols
       * 'other'.
       *
       * @param - other: the SymbolSet to which to compare this SymbolSet 
       * @return true if this SymbolSet is equivalent to the SymbolSet 'other'
       *
       */
      bool operator==( const Label & other ) const;

      /**
       *
       * @brief provides access to all symbols in the collection
       *
       * This method provides access to all symbols in the collection in the form of a 
       * set of symbols.
       *
       * @return a set containing all symbols in this collection
       * 
       */
      const std::set<Sym> getSymbolsIn( const SymbolSet & symbolPool ) const;

      /**
       *
       * @brief provides access to all symbols not in the collection
       *
       * This method provides access to all symbols not in the collection in the form  
       * of a set of symbols.
       *
       * @return a set containing all symbols not in this collection
       * 
       */
      const std::set<Sym> getSymbolsNotIn( const SymbolSet & symbolPool ) const;

      /**
       *
       * @brief returns the number of symbols in this collection
       *
       * This method returns the number of symbols in this collection.
       *
       * @return the number of symbols in this collection
       *
       */
      size_t sizeSymbolsIn( const SymbolSet & symbolPool ) const;

      /**
       *
       * @brief returns the number of symbols not in this collection
       *
       * This method returns the number of symbols not in this collection.
       *
       * @return the number of symbols not in this collection
       *
       */
      size_t sizeSymbolsNotIn( const SymbolSet & symbolPool ) const;

      //
      // Variables
      //
      
      protected:
      
      std::set<Sym> syms;
      bool neg;

    };

    //Constructors
    Label::Label( )
    { 
      neg = false;
    }
    
    Label::Label( const Label & other )
    {
      syms = other.syms;
      neg = other.neg;
    }

    Label & Label::operator=( const Label & other )
    {
      if( this == &other )     
        return *this;
      
      syms = other.syms;
      neg = other.neg;

      return *this;
    }

    //Accessors

    /**
     *
     * @brief tests whether this collection represents the wild symbol
     *
     * The Label is wild if syms is empty and neg is true ( {}, T )
     * or syms is equal to the symbolPool and neg is false ( symbolPool, F }.
     *
     * @return true if this collection represents the wild symbol
     *
     */
    bool Label::isWild( const SymbolSet & symbolPool ) const
    {
      if( neg )
        return syms.empty();        //No symbols not on the edge.
      else
      {
        //Check that all symbols in the symbolPool are in syms.
        for( const_iterator it = symbolPool.beginSymbols(); it != symbolPool.endSymbols(); it++ )
        {
          if( syms.find(*it) == syms.end() )
            return false;
        }
        return true;                //All symbols are on the edge.
      }        
    }

    /**
     *
     * @brief make this collection represent wild
     *
     * The Label is wild if syms is empty and neg is true ( {}, T ).
     *
     */
    void Label::makeWild( )
    {
      syms.clear();
      neg = true;
    }

    /**
     *
     * @brief tests whether this collection represents the absence of all symbols
     *
     * The Label is absent if syms is empty and neg is false ( {}, F )
     * or syms is equal to the symbolPool and neg is true ( symbolPool, T ).
     *
     * @return true if this collection represents the absence of all symbols
     *
     */
    bool Label::isAbsent( const SymbolSet & symbolPool ) const
    {
      if( neg )
      {
        //Check that all symbols in the symbolPool are in syms.
        for( const_iterator it = symbolPool.beginSymbols(); it != symbolPool.endSymbols(); it++ )
        {
          if( syms.find(*it) == syms.end() )
            return false;
        }
        return true;                //All symbols are not on the edge.
      }
      else
        return ( syms.empty() );    //No symbols are on the edge.
    }

    /**
     *
     * @brief make this collection represent the absence of all symbols
     *
     * The Label is absent if syms is empty and neg is false ( {}, F )
     *
     */
    void Label::makeAbsent( )
    {
      syms.clear(); 
      neg = false;
    }

    /**
     *
     * @brief tests whether the given symbol is a member of this collection
     *
     * @param - sym: the symbol to test
     * @return true if the symbol is a member of this collection of states
     *
     */
    bool Label::containsSymbol( Sym sym ) const
    {
      if(neg)
      {
        //syms records symbols not on the edge, so
        //if the symbol is not in syms, it is in the collection
        return (syms.count(sym) == 0);
      }
      else
      {
        //syms records symbols on the edge, so
        //if the symbol is in syms, it is in the collection
        return (syms.count(sym) > 0);
      }
    }

    /**
     *
     * @brief provides access to some symbol in this collection
     *
     * @return some symbol in this collection of symbols
     *
     */
    Label::Sym Label::getAnySymbol( const SymbolSet & symbolPool ) const
    {
      if(neg)
      {
        for( const_iterator it = symbolPool.beginSymbols(); it != symbolPool.endSymbols(); it++ )
        {
          if( syms.find(*it) == syms.end() )
            return *it;   //Return the first symbol in the symbolPool which  
                          //is not in the set of symbols not on the edge.
        }
        return NULL;
      }
      else
      {
        return *(syms.begin());   //Return the first symbol in the set.
      }
    }

    /**
     *
     * @brief add the given symbol 
     *
     * @param - sym: the symbol to add
     * @return false if the symbol already exists, true otherwise
     *
     */
    bool Label::addSymbol( Sym sym )
    {
      if( containsSymbol(sym) )
        return false;

      if( neg )
      {
        //syms records symbols not on the edge, so remove sym from syms
        syms.erase(syms.find(sym));
      }
      else
      {
        //syms records symbols on the edge, so add sym to syms
        syms.insert(sym);
      }
      return true;
    }

    /**
     *
     * @brief add all the symbols in the given collection to this collection 
     *
     * @param - symSet: the collection of symbols to add to this collection of symbols
     *
     */
    void Label::addAll( Label lbl, const SymbolSet & symbolPool )
    {
      if( lbl.neg )
      {
        //Note: we cannot directly add symbols from syms because syms only 
        //      records symbols not on the edge.
        //Need to add all symbols from the transition pool that are in symSet.
        for( const_iterator it = symbolPool.beginSymbols(); it != symbolPool.endSymbols(); it++ )
        {
          if( lbl.containsSymbol(*it) )
            addSymbol(*it);
        }
      }
      else
      {
        for( const_iterator it = lbl.syms.begin(); it != lbl.syms.end(); it++ )
        {
          addSymbol(*it);
        }
      }
    }

    /** 
     *
     * @brief remove the given symbol
     *
     * @param - sym: the symbol to remove
     * @return false if the symbol does not exist, true otherwise
     *
     */
    bool Label::removeSymbol( Sym sym )
    { 
      if(! containsSymbol(sym) )
        return false;

      if( neg )
      {
        //syms records symbols not on the edge, so add sym to syms
        syms.insert(sym);        
      }
      else
      {
        //syms records symbols on the edge, so remove sym from syms
        syms.erase(syms.find(sym));
      }
      return true;
    }

    /**
     *
     * @brief remove all the symbols in the given collection from this collection
     *
     * @param - symSet: the collection of symbols to remove from this collection 
     *
     */
    void Label::removeAll( Label lbl, const SymbolSet & symbolPool )
    {
      if( lbl.neg )
      {        
        //Note: we cannot directly add symbols from syms because syms only 
        //      records symbols not on the edge.
        //Need to add all symbols from the transition pool that are in symSet.
        for( const_iterator it = symbolPool.beginSymbols(); it != symbolPool.endSymbols(); it++ )
        {
          if( lbl.containsSymbol(*it) )
            removeSymbol(*it);
        }
      }
      else
      {
        for( const_iterator it = lbl.syms.begin(); it != lbl.syms.end(); it++ )
        {
          removeSymbol(*it);
        }
      }
    }

    //Utilities
    
    /** 
     *
     * @brief print the collection of symbols
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to 
     *
     */
    std::ostream & Label::print( std::ostream & o, const SymbolSet & symbolPool ) const
    {
      if( isWild(symbolPool) )
        o << "wild";
      else if( isAbsent(symbolPool) )
        o << "absent";
      else
      {
        if( neg )
          o << "wild - { ";
        else
          o << "{ ";

        bool first = true;
        for( const_iterator it = syms.begin(); it != syms.end(); it++ )
        {
          if( !first )
          {
            o << ", "; 
            first = false;
          }

          if( SymbolSet::isEpsilon(*it) )
            o << "Epsilon";
          else
            printKey(o,*it);
        }

        o << " }";
      }
      
      return o;
    }
    
    /** 
     *
     * @brief tests whether this collection of symbols is equivalent to the collection
     *        of symbols 'other'
     *
     * @param - other: the Label to which to compare this Label
     * @return true if this Label is equivalent to the Label 'other'
     *
     */
    bool Label::operator==( const Label & other ) const
    {
      if( neg == other.neg )
      { //This checks for both wild or both absent without using the symbolPool.
        if( syms.empty() )
          return other.syms.empty();  //returning true if both syms are empty, false otherwise
        else if( other.syms.empty() )
          return false; //we already know that syms is not empty
        else
          return ( syms == other.syms );
      }
      else if( neg )
      {
        //Anything in syms should not be a symbol in 'other'.
        for( const_iterator it = syms.begin(); it != syms.end(); it++ )
        {
          if ( other.containsSymbol(*it) )
            return false;
        }
        return true;
      }
      else
      {
        //Anything in syms should be a symbol in 'other'.
        for( const_iterator it = syms.begin(); it != syms.end(); it++ )
        {
          if (! other.containsSymbol(*it) )
            return false;
        }
        return true;
      }
    }
    
    /**
     *
     * @brief provides access to all symbols in the collection
     *
     * @return a set containing all symbols in this collection
     * 
     */
    const std::set<typename Label::Sym> Label::getSymbolsIn( const SymbolSet & symbolPool ) const
    {     
      if( neg )
      { 
        std::set<Sym> symsIn;
        for( const_iterator it = symbolPool.beginSymbols(); it != symbolPool.endSymbols(); it++ )
        {
          if( containsSymbol(*it) )
            symsIn.insert(*it);
        }
        return symsIn;
      }
      else
      {
        return syms;
      }
    }

    /**
     *
     * @brief provides access to all symbols not in the collection
     *
     * @return a set containing all symbols not in this collection
     * 
     */
    const std::set<typename Label::Sym> Label::getSymbolsNotIn( const SymbolSet & symbolPool ) const
    {      
      if( neg )
      {
        return syms;
      }
      else
      {
        std::set<Sym> symsNotIn;
        for( const_iterator it = symbolPool.beginSymbols(); it != symbolPool.endSymbols(); it++ )
        {
          if(! containsSymbol(*it) )
            symsNotIn.insert(*it);
        } 
        return symsNotIn;    
      }     
    }

    /**
     *
     * @brief returns the number of symbols in this collection
     *
     * @return the number of symbols in this collection
     *
     */
    size_t Label::sizeSymbolsIn( const SymbolSet & symbolPool ) const
    {
      if( neg )
        return (symbolPool.sizeSymbols() - syms.size());
      else
        return syms.size();
    }

    /**
     *
     * @brief returns the number of symbols not in this collection
     *
     * @return the number of symbols not in this collection
     *
     */
    size_t Label::sizeSymbolsNotIn( const SymbolSet & symbolPool ) const
    {
      if( neg )
        return syms.size();
      else
        return (symbolPool.sizeSymbols() - syms.size());
    }
  }
}
#endif