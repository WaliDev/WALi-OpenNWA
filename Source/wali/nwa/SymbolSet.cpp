#include "SymbolSet.hpp"

namespace wali {
namespace nwa {

//Constructors
SymbolSet::SymbolSet( )
{
  //Epsilon is always a symbol of the NWA.
  addSymbol( getEpsilon() );
}

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
  else if(sym == wali::WALI_EPSILON || sym == wali::WALI_WILD)
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
//inline  // TODO: remove comment -Evan
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
//inline  // TODO: Remove comment -Evan
void SymbolSet::removeAll( SymbolSet symSet )
{
  symbols.erase(symSet.symbols.begin(),symSet.symbols.end());
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


///////// Class Label

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
const std::set<Label::Sym> Label::getSymbolsIn( const SymbolSet & symbolPool ) const
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
const std::set<Label::Sym> Label::getSymbolsNotIn( const SymbolSet & symbolPool ) const
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





} // namespace nwa
} // namespace wali
