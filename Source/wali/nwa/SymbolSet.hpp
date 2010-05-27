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

        static std::string const & XMLSymbolTag() {
          static std::string ret = "Symbol";
          return ret;
        }

        static std::string const & XMLNameAttr() {
          static std::string ret = "name";
          return ret;
        }

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

    //Accessors

    /**
     *
     * @brief provides access to all symbols 
     *
     * @return a set containing all symbols 
     * 
     */
    inline
    const std::set<SymbolSet::Sym> & SymbolSet::getSymbols( ) const
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
     * @brief removes all symbols 
     *
     * This method removes all symbols from this collection.  
     *
     */
    inline
    void SymbolSet::clearSymbols( )
    {
      symbols.clear();

      //Wpsilon is always a symbol of the NWA.
      addSymbol( getEpsilon() );
    }

    //Utilities
    
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


    /**
     *
     * @brief tests whether the given symbol is a member of this collection
     *
     * @param - sym: the symbol to test
     * @return true if the symbol is a member of this collection of states
     *
     */
    inline
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
     * @brief add the given symbol 
     *
     * @param - sym: the symbol to add
     * @return false if the symbol already exists, true otherwise
     *
     */
    inline
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
     * @brief returns the number of symbols in this collection
     *
     * @return the number of symbols in this collection
     *
     */
    inline
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
    inline
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
