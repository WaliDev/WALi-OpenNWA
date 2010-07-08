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
    NWPNode::NWPNode( Key sym, Type nodeType )
    {
      symbol = sym;
      this->nodeType = nodeType;
      prev = NULL;
      call = NULL;
    }
    NWPNode::NWPNode( Key sym, NWPNode * prev, Type nodeType )
    {
      symbol = sym;
      this->nodeType = nodeType;
      this->prev = prev;
      nesting = prev->getNesting();
      
      //Add a call to the stack immediately after it is passed.
      if( prev->isCallNode() )
      {
        nesting.push_back(prev);
      }
      
      //Remove a call from the stack and attach it to the return node.
      if( nodeType == Ret )
      {
        call = nesting.back();
        nesting.pop_back(); 
      }
    }
    NWPNode & NWPNode::operator=( const NWPNode & other )
    {
      symbol = other.symbol;
      
      prev = other.prev;
      call = other.call;
      
      nesting = other.nesting;
      
      return *this;
    }
    
    NWPNode::~NWPNode( )
    {
      symbol = wali::WALI_BAD_KEY;
      prev = NULL;
      call = NULL;
      nesting.clear();
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
     * @brief tests whether this symbol is the first symbol in  
     * the word prefix
     *
     * @return true if this is not the first symbol in the word
     * prefix
     *
     */
    bool NWPNode::isFirst( )
    {
      return (prev == NULL);
    }   
        
    /** 
     *
     * @brief tests whether this is a call node
     *
     * @return true if this node is the beginning of a function
     * call
     *
     */
    bool NWPNode::isCallNode( )
    {
      return nodeType == Call;
    }  
    bool NWPNode::isRetNode( )
    {
      return nodeType == Ret;
    }
    bool NWPNode::isInternalNode( )
    {
      return nodeType == Internal;
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
     * @brief returns the return node connected to this call node 
     * in the word prefix
     *
     * @return the return node connected to this call node in the 
     * word prefix
     *
     */
    NWPNode * NWPNode::callNode( )
    {
      return call;
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
      if( nesting.empty() )
        return NULL;
      else
        return nesting.back();
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
      
    void NWPNode::pushCall(NWPNode* call)
    {
      nesting.push_back(call);
    }
    NWPNode* NWPNode::popCall()
    {
      NWPNode* call = nesting.back();
      nesting.pop_back();
      return call;
    }
    
    size_t NWPNode::sizeNesting()
    {
      return nesting.size();
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
      if(call != NULL)
      {
        o<<"(";
        printKey(o,call->getSymbol());
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
      if( (call == NULL) && (other.call != NULL) )
        return false;
      else if( (call != NULL) && (other.call == NULL) )
        return false;
      else if( (call != NULL) && (other.call != NULL) )
        if( !(*call == *other.call) )
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
