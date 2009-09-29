/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/nwp/NWP.hpp"

namespace wali
{	
  namespace nwp
  {

    //
    // Methods
    //

    //Constructors and Destructor
    NWP::NWP( )
    {
      nesting = std::deque< NWPNode >();
      word = NULL;
    }
    NWP::NWP( std::stack< NWPNode > nesting, NWPNode * word )
    {
      while(! nesting.empty() )
      {
        this->nesting.push_front(nesting.top());
        nesting.pop();
      }
      
      this->word = word;
    }
    NWP::NWP( std::vector< Key > sym, std::queue< std::pair< int,int > > nest )
    {
      if( sym.empty() )
      {
        word = NULL;
      }
      else
      {  
        //Make nodes for each symbol.
        std::deque< NWPNode > word;
        for( std::vector< Key >::iterator it = sym.begin();
              it != sym.end(); it++ )
        {
          word.push_back( NWPNode(*it) );
        }
      
        //Connect nodes appropriately.
        //Prev nodes
        for( size_t i = 1; i < word.size(); i++ )
        {
          word[i].setPrev(&word[i-1]);
        }
        //Exit nodes
        while( !nest.empty() )
        {
          std::pair< int,int > call = nest.front();
          word[call.first - 1].setExit(&word[call.second - 1]);
          nest.pop();
        }           
        this->word = &word[word.size()-1];
      }
      
      this->nesting = std::deque< NWPNode >();
    }
    
    NWP::NWP( NWP & other )
    {
      operator=(other);
    }
    NWP & NWP::operator=( NWP & other )
    {
      this->nesting.clear();
        
      for( std::deque< NWPNode >::iterator it = other.nesting.begin();
            it != other.nesting.end(); it++ )
      {
        this->nesting.push_back(*it);
      }
      
      this->word = other.word;
      
      return *this;
    }
    
    NWP::~NWP( )
    {
      nesting.clear();
      word = NULL;
    }
   
   
    //Accessors
    
    /** 
     *
     * @brief provide access to the next call node in the nesting 
     *
     * @return the next call node in the nesting
     *
     */
    NWPNode * NWP::stackTop()
    {
      if( nesting.empty() )
        return NULL;
      else
        return &nesting.back();
    }
    
    /** 
     *
     * @brief adds the given call node to the nesting
     *
     * @parm the call node to add to the nesting
     *
     */
    void NWP::pushStack( NWPNode * call )
    {
      nesting.push_back(*call);
    }
    
    /** 
     *
     * @brief removes the next call node from the nesting
     *
     */
    void NWP::popStack()
    {
      nesting.pop_back();
    }
    
    /** 
     *
     * @brief returns the current level of nesting
     *
     * @return the current level of nesting
     *
     */
    size_t NWP::stackSize()
    {
      return nesting.size();
    }

   
    /** 
     *
     * @brief appends a symbol to the end of the word prefix
     *
     * @parm the symbol to add to the word prefix
     *
     */
    void NWP::addNode( Key sym )
    {
      NWPNode * node;
      if( word == NULL )
        node = new NWPNode(sym);
      else
        node = new NWPNode(sym,word); 
        
      word = node;
    }
    
    /** 
     *
     * @brief appends a symbol to the end of the word prefix
     *
     * @parm sym: the symbol to add to the word prefix
     * @parm returnNode: the return node that is paired with this call 
     *
     */
    bool NWP::addNode( Key sym, NWPNode * returnNode )
    {
      NWPNode * node;
      if( word == NULL )
        node = new NWPNode(sym);
      else
        node = new NWPNode(sym,word); 
        
      if( node->isCall() && 
            !(node->exitNode() == returnNode) )
      {
        return false;
      }
      
      node->setExit(returnNode);
      word = node;
      
      return true;
    }
    
    /** 
     *
     * @brief provides access to the previous node in the word prefix
     *
     * @return the previous node in the word prefix
     *
     */
    NWPNode * NWP::prevNode()
    {
      return word;
    }
    
    /**
     * 
     * @brief removes the node at the end of the word prefix
     *
     */
    void NWP::undoNode()
    {
      if(! word == NULL )
      {
        NWPNode * node = word;
        if( node->hasPrev() )
          word = node->prevNode();
        else
          word = NULL;
      }
    }
    
    /** 
     *
     * @brief removes the node at the end of the word prefix
     *
     * @return the node that was removed from the end of the word prefix
     *
     */
    NWPNode * NWP::removeNode()
    {
      if( word == NULL )
        return NULL;
             
      NWPNode * node = word;
      if( node->hasPrev() )
        word = node->prevNode();
      else
        word = NULL;
  
      if( node->isCall() )
      {
        if( (!nesting.empty()) && (nesting.back() == *node) )
          nesting.pop_back();
        //Otherwise, error(call node removed was not the top
        //call node on the stack)
      }
      if(! (getCall(node,node) == NULL) ) 
        nesting.push_back(*node);

      return node;
    }

    
    //Utilities
    
    /** 
     *
     * @brief prints a representation of the nested word prefix to the given output stream
     *
     * @parm the output stream to which the representation is to be printed
     * @return the output stream to which the representation was printed
     *
     */
    std::ostream & NWP::print( std::ostream & o ) const
    {
      if(word != NULL)
      {
        word->print(o);
      }
      for( std::deque< NWPNode >::const_iterator it = nesting.begin();
            it != nesting.end(); it++ )
      {
        o << " [";
        it->print(o); 
        o << "]";        
      }   
    
      return o;
    }
    
    /** 
     *
     * @brief tests the equivalence of two nested word prefixes
     *
     * @parm the nested word prefix to which to compare this nested word prefix 
     * @return true if this nested word prefix and the given one are equivalent, 
     * false otherwise
     *
     */
    bool NWP::operator==( NWP & otherNWP )
    {
      if( !(word == otherNWP.word)||
          (nesting.size() != otherNWP.nesting.size()) )
        return false;
        
      for( std::deque< NWPNode >::iterator it = nesting.begin(), 
            oit = otherNWP.nesting.begin(); it != nesting.end();
            it++, oit++ )
      {
        if(! (*it == *oit) )
          return false;
      }  
        
      return true;
    }

    
    //Uses
    
    /** 
     *
     * @brief tests whether this nested word prefix is empty
     *
     * @return true if this nested word prefix is empty, false otherwise
     *
     */
    bool NWP::isEmpty()
    {
      return (nesting.empty() && (word == NULL));
    }
    
    /** 
     *
     * @brief finds the call node associated with the given return node 
     * in the word prefix
     *
     * @return the call node associated with the given return node, or NULL
     * if no such node exists in the word prefix
     *
     */
    NWPNode * NWP::getCall( NWPNode * returnNode, NWPNode * currNode )
    {
      if( currNode->isCall() )
      {
        if( *currNode->exitNode() == *returnNode )
          return currNode;
      }
      while( currNode->hasPrev() )
      {
        //check for call node that has node as an exit
        currNode = currNode->prevNode();
        if( currNode->isCall() )
        {
          if( *currNode->exitNode() == *returnNode )
            return currNode;
        }
      }
      
      return NULL;
    }
    
  };
}
