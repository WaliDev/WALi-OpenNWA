#ifndef wali_nwa_SymbolStorage_GUARD
#define wali_nwa_SymbolStorage_GUARD 1

/**
 * @author Amanda Burton
 */

#include "opennwa/deprecate.h"
#include "opennwa/NwaFwd.hpp"

// ::wali
#include "wali/Printable.hpp"
#include "wali/Common.hpp"
#include "wali/Key.hpp"

// ::std
#include <assert.h>

namespace opennwa
{
  namespace details
  {
      
    class SymbolStorage : public wali::Printable
    { 
    public:
      typedef Symbol Sym;
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
      SymbolStorage( );
      SymbolStorage( const SymbolStorage & other );
      SymbolStorage & operator=( const SymbolStorage & other );
  
      //Accessors

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
      void addAllSymbols( SymbolStorage symSet );

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
      void removeAll( SymbolStorage symSet );

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
       * @param - other: the SymbolStorage to which to compare this SymbolStorage 
       * @return true if this SymbolStorage is equivalent to the SymbolStorage 'other'
       *
       */
      bool operator==( const SymbolStorage & other ) const;

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
    const std::set<SymbolStorage::Sym> & SymbolStorage::getSymbols( ) const
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
    bool SymbolStorage::isSymbol( Sym sym ) const
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
    void SymbolStorage::clearSymbols( )
    {
      symbols.clear();

      // Epsilon is always a symbol of the NWA.
      addSymbol( EPSILON );
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
    SymbolStorage::const_iterator SymbolStorage::beginSymbols( ) const
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
    SymbolStorage::const_iterator SymbolStorage::endSymbols( ) const
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
    size_t SymbolStorage::sizeSymbols( ) const
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
      typedef Symbol Sym;
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
      bool isWild( const SymbolStorage & symbolPool ) const;

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
      bool isAbsent( const SymbolStorage & symbolPool ) const;

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
      Sym getAnySymbol( const SymbolStorage & symbolPool ) const;
            
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
      void addAll( Label lbl, const SymbolStorage & symbolPool );

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
      void removeAll( Label lbl, const SymbolStorage & symbolPool );

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
      std::ostream & print( std::ostream & o, const SymbolStorage & symbolPool ) const;
      
      /** 
       *
       * @brief tests whether this collection of symbols is equivalent to the collection
       *        of symbols 'other'
       *
       * This method tests the equivalence of this set of symbols and the set of symbols
       * 'other'.
       *
       * @param - other: the SymbolStorage to which to compare this SymbolStorage 
       * @return true if this SymbolStorage is equivalent to the SymbolStorage 'other'
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
      const std::set<Sym> getSymbolsIn( const SymbolStorage & symbolPool ) const;

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
      const std::set<Sym> getSymbolsNotIn( const SymbolStorage & symbolPool ) const;

      /**
       *
       * @brief returns the number of symbols in this collection
       *
       * This method returns the number of symbols in this collection.
       *
       * @return the number of symbols in this collection
       *
       */
      size_t sizeSymbolsIn( const SymbolStorage & symbolPool ) const;

      /**
       *
       * @brief returns the number of symbols not in this collection
       *
       * This method returns the number of symbols not in this collection.
       *
       * @return the number of symbols not in this collection
       *
       */
      size_t sizeSymbolsNotIn( const SymbolStorage & symbolPool ) const;

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
      size_t count = syms.count(sym);
      if (neg) {
        //syms records symbols not on the edge, so
        //if the symbol is not in syms, it is in the collection
        return count == 0;
      }
      else {
        //syms records symbols on the edge, so
        //if the symbol is in syms, it is in the collection
        return count > 0;
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
        syms.erase(sym);
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
    size_t Label::sizeSymbolsIn( const SymbolStorage & symbolPool ) const
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
    size_t Label::sizeSymbolsNotIn( const SymbolStorage & symbolPool ) const
    {
      if( neg )
        return syms.size();
      else
        return (symbolPool.sizeSymbols() - syms.size());
    }


  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
