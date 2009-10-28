/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/nws/NWS.hpp"

namespace wali
{	
  namespace nws
  {
    //
    // Methods
    //

    //Constructors and Destructor
    NWS::NWS( )
    {
      nesting = std::deque< NWSNode * >();
      word = NULL;
    }
    NWS::NWS( std::stack< NWSNode > nesting, NWSNode * word )
    {
      while(! nesting.empty() )
      {
        this->nesting.push_front(&nesting.top());
        nesting.pop();
      }
      
      this->word = word;
    }
    NWS::NWS( std::vector< Key > sym, std::queue< std::pair< int,int > > nest )
    {
      if( sym.empty() )
      {
        word = NULL;
      }
      else
      {  
        //Make nodes for each symbol.
        std::deque< NWSNode > word;
        for( std::vector< Key >::iterator it = sym.begin();
              it != sym.end(); it++ )
        {
          word.push_back( NWSNode(*it) );
        }
      
        //Connect nodes appropriately.
        //Next nodes
        for( size_t i = 1; i < word.size(); i++ )
        {
          word[i-1].setNext(&word[i]);
        }
        //Exit nodes
        while( !nest.empty() )
        {
          std::pair< int,int > call = nest.front();
          word[call.first - 1].setExit(&word[call.second - 1]);
          nest.pop();
        }           
        this->word = &word[0];
      }
      
      this->nesting = std::deque< NWSNode* >();
    }
    
    NWS::NWS( const NWS & other )
    {
      operator=(other);
    }
    NWS & NWS::operator=( const NWS & other )
    {
      this->nesting.clear();
        
      for( std::deque< NWSNode * >::const_iterator it = other.nesting.begin();
            it != other.nesting.end(); it++ )
      {
        this->nesting.push_back(*it);
      }
      
      this->word = other.word;
      
      return *this;
    }
    
    NWS::~NWS( )
    {
      nesting.clear();
      word = NULL;
    }
   
   
    //Accessors
    
    /** 
     *
     * @brief provide access to the next return node in the nesting 
     *
     * @return the next return node in the nesting
     *
     */
    NWSNode * NWS::stackTop()
    {
      if( nesting.empty() )
        return NULL;
      else
        return nesting.back();
    }
    
    /** 
     *
     * @brief adds the given return node to the nesting
     *
     * @parm the return node to add to the nesting
     *
     */
    void NWS::pushStack( NWSNode * exit )
    {
      nesting.push_back(exit);
    }
    
    /** 
     *
     * @brief removes the next return node from the nesting
     *
     */
    void NWS::popStack()
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
    size_t NWS::stackSize()
    {
      return nesting.size();
    }
    
    /**
     *
     */
    NWS::iterator NWS::beginStack()
    {
      return nesting.begin();
    }
    
    /**
     *
     */
    NWS::iterator NWS::endStack()
    {
      return nesting.end();
    }

   
    /** 
     *
     * @brief appends a symbol to the beginning of the word suffix
     *
     * @parm the symbol to add to the word suffix
     *
     */
    void NWS::addNode( Key sym )
    {
      NWSNode * node;
      if( word == NULL )
        node = new NWSNode(sym);
      else
        node = new NWSNode(sym,word); 
        
      word = node;
    }
    
    /** 
     *
     * @brief appends a symbol to the beginning of the word suffix
     *
     * @parm sym: the symbol to add to the word suffix
     * @parm returnNode: the return node that is paired with this call 
     *
     */
    bool NWS::addNode( Key sym, NWSNode * returnNode )
    {
      NWSNode * node;
      if( word == NULL )
        node = new NWSNode(sym);
      else
        node = new NWSNode(sym,word); 
        
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
     * @brief provides access to the next node in the word suffix
     *
     * @return the next node in the word suffix
     *
     */
    NWSNode * NWS::nextNode()
    {
      return word;
    }
    
    /**
     * 
     * @brief removes the node at the beginning of the word suffix
     *
     */
    void NWS::undoNode()
    {
      if(! word == NULL )
      {
        NWSNode * node = word;
        if( node->hasNext() )
          word = node->nextNode();
        else
          word = NULL;
      }
    }
    
    /** 
     *
     * @brief removes the node at the beginning of the word suffix
     *
     * @return the node that was removed from the beginning of the word suffix
     *
     */
    NWSNode * NWS::removeNode()
    {
      if( word == NULL )
        return NULL;
             
      NWSNode * node = word;
      if( node->hasNext() )
        word = node->nextNode();
      else
        word = NULL;
  
      if( node->isCall() )
      {
        NWSNode * eNode = node->exitNode();
        nesting.push_back(eNode);
      }
      else if( !nesting.empty() )
        if( node == nesting.back() )
        {
          nesting.pop_back();
        }

      return node;
    }

    
    //Utilities
    
    /** 
     *
     * @brief prints a representation of the nested word suffix to the given output stream
     *
     * @parm the output stream to which the representation is to be printed
     * @return the output stream to which the representation was printed
     *
     */
    std::ostream & NWS::print( std::ostream & o ) const
    {
      for( std::deque< NWSNode * >::const_iterator it = nesting.begin();
            it != nesting.end(); it++ )
      {
        o << "[";
        (*it)->print(o); 
        o << "] ";        
      }
      if(word != NULL)
      {
        word->print(o);
      }
    
      return o;
    }
    
    /** 
     *
     * @brief tests the equivalence of two nested word suffixes
     *
     * @parm the nested word suffix to which to compare this nested word suffix 
     * @return true if this nested word suffix and the given one are equivalent, false otherwise
     *
     */
    bool NWS::operator==( const NWS & otherNWS ) const
    {
      if( !(word == otherNWS.word)||
          (nesting.size() != otherNWS.nesting.size()) )
        return false;
        
      for( std::deque< NWSNode * >::const_iterator it = nesting.begin(), 
            oit = otherNWS.nesting.begin(); it != nesting.end();
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
     * @brief tests whether this nested word suffix is empty
     *
     * @return true if this nested word suffix is empty, false otherwise
     *
     */
    bool NWS::isEmpty()
    {
      return (nesting.empty() && (word == NULL));
    }
  };
}
