/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/nwp/NWPNode.hpp"

namespace wali
{	
  namespace nwp
  {
  
    //
    // Methods
    //

    //Constructors and Destructor
    NWPNode::NWPNode( )
    {
      //WALI_EPSILON is the absence of a symbol.
      symbol = wali::WALI_EPSILON; 
      prev = NULL;
      exit = NULL;
    }
    NWPNode::NWPNode( Key sym )
    {
      symbol = sym;
      prev = NULL;
      exit = NULL;
    }
    NWPNode::NWPNode( Key sym, NWPNode * prev )
    {
      symbol = sym;
      this->prev = prev;
      exit = NULL;
    }
    NWPNode & NWPNode::operator=( const NWPNode & other )
    {
      symbol = other.symbol;
      
      prev = other.prev;
      exit = other.exit;
      
      return *this;
    }
    
    NWPNode::~NWPNode( )
    {
      symbol = wali::WALI_BAD_KEY;
      prev = NULL;
      exit = NULL;
    }
    
    
    //Accessors
    
    /** 
     *
     * @brief returns the symbol associated with this node
     *
     * @return the Key for the symbol associated with this node
     *
     */
    Key NWPNode::getSymbol( )
    {
      return symbol;
    }
    
    /** 
     *
     * @brief sets the symbol associated with this node
     *
     * @parm the new symbol for this node
     *
     */
    void NWPNode::setSymbol( Key sym )
    {
      symbol = sym;
    }

    
    /** 
     *
     * @brief tests whether this symbol is the first symbol in  
     * the word prefix
     *
     * @return true if this is not the first symbol in the word
     * prefix
     *
     */
    bool NWPNode::hasPrev( )
    {
      return !(prev == NULL);
    }   
    
    /** 
     *
     * @brief returns the previous node in the word prefix
     *
     * @return the previous node in the word prefix
     *
     */
    NWPNode * NWPNode::prevNode( )
    {
      return prev;
    }
    
    /** 
     *
     * @brief sets the previous node in the word prefix
     * 
     * @parm the node to append to the end of the word prefix
     *
     */
    void NWPNode::setPrev( NWPNode * prev )
    {
      this->prev = prev;
    }

    
    /** 
     *
     * @brief tests whether this is a call node
     *
     * @return true if this node is the beginning of a function
     * call
     *
     */
    bool NWPNode::isCall( )
    {
      return !(exit == NULL);
    }  
    
    /** 
     *
     * @brief returns the return node connected to this call node 
     * in the word prefix
     *
     * @return the return node connected to this call node in the 
     * word prefix
     *
     */
    NWPNode * NWPNode::exitNode( )
    {
      return exit;
    }
    
    /** 
     *
     * @brief sets the return node to connect to this call node 
     * in the word prefix
     * 
     * @parm the node to connect to this call node in the word 
     * prefix
     *
     */
    void NWPNode::setExit( NWPNode * exit )
    {
      this->exit = exit;
    }

    /**
     *
     * @brief returns the nesting of the prefix at this node 
     *
     * @return the nesting of the prefix at this node
     *
     */
    std::deque< NWPNode * > NWPNode::getNesting( )
    {
      return nesting;
    }
      
    /**
     *
     * @brief sets the nesting of the prefix at this node 
     *
     * @param the nesting of the prefix at this node
     *
     */
    void NWPNode::setNesting( std::deque< NWPNode * > nesting )
    {
      this->nesting = nesting;
    }

    /**
     *
     * @brief returns the innermost open call of the nesting
     *
     * @return the innermost open call of the nesting
     *
     */
    NWPNode * NWPNode::currCall()
    {
      return nesting.back();
    }
    
    //Iteration
    NWPNode * NWPNode::operator++()
    {
      return prev;
    }
    
    //Utilities
    
    /** 
     *
     * @brief prints a representation of this node to the output 
     * stream specified
     *
     * @parm the output stream to which to print the node
     * @return the output stream to which the node was printed
     *
     */
    std::ostream & NWPNode::print( std::ostream & o ) const
    {      
      if(prev != NULL)
      {
        prev->print(o);
        o<<"<-";        
      }
      printKey(o,symbol);
      if(exit != NULL)
      {
        o<<"(";
        printKey(o,exit->getSymbol());
        o<<")";
      }
      
      return o;
    }   
    
    /** 
     *
     * @brief tests the equivalence of two nested word prefix nodes
     *
     * @parm the node with which to compare this node
     * @return true if this node is equivalent to the node 
     * 'otherNWSNode'
     *
     */
    bool NWPNode::operator==( const NWPNode & other )
    {
      //Check symbol equivalence.
      if( symbol != other.symbol )
        return false;

      //Check prev link equivalence.
      if( (prev == NULL) && (other.prev != NULL) )
        return false;
      else if( (prev != NULL) && (other.prev == NULL) )
        return false;
      else if( (prev != NULL) && (other.prev != NULL) )
        if( !(*prev == *other.prev) )
          return false;

      //Check call equivalence.
      if( (exit == NULL) && (other.exit != NULL) )
        return false;
      else if( (exit != NULL) && (other.exit == NULL) )
        return false;
      else if( (exit != NULL) && (other.exit != NULL) )
        if( !(*exit == *other.exit) )
          return false;

      //Check nesting equivalence.
      std::deque< NWPNode * >::iterator it = nesting.begin();
      std::deque< NWPNode * >::const_iterator other_it = other.nesting.begin();
      for( ; (it != nesting.end()) && (other_it != other.nesting.end()); it++, other_it++)
      {
        if(*it != *other_it)
          return false;
      } 
      if( it != nesting.end() )
        return false;
      if( other_it != other.nesting.end() )
        return false;

      return true;
    }
  };
}
