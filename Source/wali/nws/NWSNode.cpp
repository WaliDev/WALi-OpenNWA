/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/nws/NWSNode.hpp"

namespace wali
{	
  namespace nws
  {

    //
    // Methods
    //

    //Constructors and Destructor
    NWSNode::NWSNode( )
    {
      //WALI_EPSILON is the absence of a symbol.
      symbol = wali::WALI_EPSILON; 
      next = NULL;
      exit = NULL;
    }
    NWSNode::NWSNode( Key sym )
    {
      symbol = sym;
      next = NULL;
      exit = NULL;
    }
    NWSNode::NWSNode( Key sym, NWSNode * next )
    {
      symbol = sym;
      this->next = next;
      exit = NULL;
    }
    NWSNode & NWSNode::operator=( const NWSNode & other )
    {
      symbol = other.symbol;
      
      next = other.next;
      exit = other.exit;
      
      return *this;
    }
    
    NWSNode::~NWSNode( )
    {
      symbol = wali::WALI_BAD_KEY;
      next = NULL;
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
    Key NWSNode::getSymbol( )
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
    void NWSNode::setSymbol( Key sym )
    {
      symbol = sym;
    }

    
    /** 
     *
     * @brief tests whether this symbol is the last symbol in the 
     * word suffix
     *
     * @return true if this is not the last symbol in the word suffix
     *
     */
    bool NWSNode::hasNext( )
    {
      return !(next == NULL);
    }   
    
    /** 
     *
     * @brief returns the next node in the word suffix
     *
     * @return the next node in the word suffix
     *
     */
    NWSNode * NWSNode::nextNode( )
    {
      return next;
    }
    
    /** 
     *
     * @brief sets the next node in the word suffix
     * 
     * @parm the node to append to the end of the word suffix
     *
     */
    void NWSNode::setNext( NWSNode * next )
    {
      this->next = next;
    }

    
    /** 
     *
     * @brief tests whether this is a call node
     *
     * @return true if this node is the beginning of a function
     * call
     *
     */
    bool NWSNode::isCall( )
    {
      return !(exit == NULL);
    }  
    
    /** 
     *
     * @brief returns the return node connected to this call node 
     * in the word suffix
     *
     * @return the return node connected to this call node in the 
     * word suffix
     *
     */
    NWSNode * NWSNode::exitNode( )
    {
      return exit;
    }
    
    /** 
     *
     * @brief sets the return node to connect to this call node 
     * in the word suffix
     * 
     * @parm the node to connect to this call node in the word 
     * suffix
     *
     */
    void NWSNode::setExit( NWSNode * exit )
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
    std::ostream & NWSNode::print( std::ostream & o ) const
    {
      printKey(o,symbol);
      if(exit != NULL)
      {
        o<<"(";
        printKey(o,exit->getSymbol());
        o<<")";
      }
      if(next != NULL)
      {
        o<<"->";
        next->print(o);
      }
      return o;
    }   
    
    /** 
     *
     * @brief tests the equivalence of two nested word suffix nodes
     *
     * @parm the node with which to compare this node
     * @return true if this node is equivalent to the node 
     * 'otherNWSNode'
     *
     */
    bool NWSNode::operator==( const NWSNode & other )
    {
      return ((symbol == other.symbol) &&
        (next == other.next) &&
        (exit == other.exit));
    }

  };
}
