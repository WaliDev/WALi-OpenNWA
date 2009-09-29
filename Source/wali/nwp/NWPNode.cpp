/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/nwp/NWPNode.hpp"
#include "wali/Common.hpp"

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
      symbol = WALI_EPSILON; 
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
      symbol = WALI_BAD_KEY;
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
      return ((symbol == other.symbol) &&
        (prev == other.prev) &&
        (exit == other.exit));
    }
  };
}
