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
      word = NULL;
    }

   /* NWP::NWP( std::vector< Key > sym, std::queue< std::pair< int,int > > nest )
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
        //Exit nodes
        while( !nest.empty() )
        {
          std::pair< int,int > call = nest.front();
          word[call.first - 1].setExit(&word[call.second - 1]);
          nest.pop();
        }       

        //Prev nodes and nesting
        std::deque< NWPNode * > nest;
        for( size_t i = 1; i < word.size(); i++ )
        {
          word[i].setPrev(&word[i-1]);
          if( word[i-1].isCall() )
            nest.push_back(&word[i-1]);
          if( word[i] == *nest.back() )
            nest.pop_back();
          word[i].setNesting(nest);
        }

        this->word = &word[word.size()-1];
      }
      
    }*/
    
    NWP::NWP( NWP & other )
    {
      operator=(other);
    }

    NWP & NWP::operator=( NWP & other )
    {       
      this->word = other.word;
      
      return *this;
    }
    
    NWP::~NWP( )
    {
      word = NULL;
    }
   
   
    //Accessors

    /** 
     *
     * @brief appends a symbol to the end of the word prefix
     *
     * @parm the symbol to add to the word prefix
     * @return false if the prev link could not be created
     *
     */
    bool NWP::addIntraNode( Key sym )
    {
      NWPNode * node;
      if( word == NULL )
        node = new NWPNode(sym,NWPNode::Internal);
      else  //Make an intra node and create the prev link.
        node = new NWPNode(sym,word,NWPNode::Internal);
              
      word = node;
      return true;
    }

    /** 
     *
     * @brief appends a symbol to the end of the word prefix
     *
     * @parm the symbol to add to the word prefix
     * @return false if the prev link could not be created
     *
     */
    bool NWP::addCallNode( Key sym )
    {
      NWPNode * node;
      if( word == NULL )
        node = new NWPNode(sym,NWPNode::Call);
      else  //Make a call node and create the prev link.
        node = new NWPNode(sym,word,NWPNode::Call);
      
      word = node;
      return true;
    }
      
    /** 
     *
     * @brief appends a symbol to the end of the word prefix
     *
     * @parm sym: the symbol to add to the word prefix
     * @return false if any link could not be created
     *
     */
    bool NWP::addReturnNode( Key sym )
    {
      NWPNode * node;
      if( word == NULL )
        return false;
      else if( nestSize() == 0 )
        return false;
      else //Make a return node and create the prev link.
        node = new NWPNode(sym,word,NWPNode::Ret); 
        
      word = node;
      return true;
    }
      
    /** 
     *
     * @brief appends a symbol to the end of the word prefix
     *
     * @parm the node to add to the word prefix
     * @return false if the prev link could not be created
     *
     */
   /* bool NWP::addIntraNode( NWPNode * node )
    {
      //Check that the node is not a call node.
      if( node->isCall() )
        return false;
  
      if( word == NULL )
      {
        //Check that the node is not already linked with a previous node.
        if( node->hasPrev() )
          return false;
      }
      else
      {

        //Check if the node is already linked with some other previous node.
        if( node->hasPrev() && !(*node->prevNode() == *word) )
		      return false;
	  
	      node->setPrev(word);

        if( word->isCall() )
        {
          //If previous node is a call, add the call to the existing nesting.
          std::deque< NWPNode * > nest = word->getNesting();
          nest.push_back(word);
          node->setNesting(nest);
        }
        else
        {
          //Intraprocedural edge => nesting doesn't change.
          node->setNesting(word->getNesting());
        }
      }

      word = node;
      
      return true;
    }*/

   /** 
    *
    * @brief appends a symbol to the end of the word prefix
    *
    * @parm the node to add to the word prefix
    * @return false if the prev link could not be created
    *
    */
  /* bool NWP::addCallNode( NWPNode * node )
   {  
      if( word == NULL )
      {
        //Check that the node is not already linked with a previous node.
        if( node->hasPrev() )
          return false;
      }
      else
      {
        //Check if the node is already linked with some other previous node.
        if( node->hasPrev() && !(*node->prevNode() == *word) )
		      return false;
	  
	      node->setPrev(word);

        if( word->isCall() )
        {
          //If previous node is a call, add the call to the existing nesting.
          std::deque< NWPNode * > nest = word->getNesting();
          nest.push_back(word);
          node->setNesting(nest);
        }
        else
        {
          //Intraprocedural edge => nesting doesn't change.
          node->setNesting(word->getNesting());
        }
      }

      if( node->exitNode() == NULL )
      {
        //Set the call itself as its exit until we know the return node.
        node->setExit(node);
      }

      word = node;
      return true;
   }*/

    /** 
     *
     * @brief appends a symbol to the end of the word prefix
     *
     * @parm sym: the return node to add to the word prefix
     * @return false if some link could not be created
     *
     */
   /* bool NWP::addReturnNode( NWPNode * node )
    {
      if( word == NULL ) 
        return false;

      //Check that the node is not a call node.
      if( node->isCall() )
        return false;
  
      //Check if the node is already linked with some other previous node.
      if( node->hasPrev() && !(*node->prevNode() == *word) )
		    return false;
	  
      node->setPrev(word);

      std::deque< NWPNode * > nest = word->getNesting();

      //If previous node is a call, add the call to the existing nesting.
      if( word->isCall() )
      {                
        nest.push_back(word);
      }

      //If the  nesting is empty, the return node cannot be linked in.
      if( nest.empty() )
        return false;

      NWPNode * call = nest.back();

      //If the call node is already attached to a different return
      //node, then this link cannot be made.
      if( !( (call->exitNode() == call ) || (call->exitNode() == node) ) )
        return false;

      //Create the call/return link.
      call->setExit(node);
      //Update the nesting.
      nest.pop_back();
      node->setNesting(nest);

      word = node;
      
      return true;
    }*/

    
    /** 
     *
     * @brief provides access to the previous node in the word prefix
     *
     * @return the previous node in the word prefix
     *
     */
    NWPNode * NWP::endNode()
    {
      return word;
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
            
      //Update end of prefix
      NWPNode * node = word;
      if(! node->isFirst() )
        word = node->prevNode();
      else
        word = NULL;
  
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
      std::deque< NWPNode * > nesting = word->getNesting();
      for( std::deque< NWPNode * >::const_iterator it = nesting.begin();
            it != nesting.end(); it++ )
      {
        o << " [";
        (*it)->print(o); 
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
      NWP::iterator it = begin();
      NWP::iterator other_it = otherNWP.begin();
      for( ; it != end() && other_it != otherNWP.end(); it++,other_it++ )
      {
        if( !(*it == *other_it) )
          return false;
      }
      if( it != end() || other_it != end() )
        return false;
              
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
      return (word == NULL);
    }
    
    //Iteration
    NWP::iterator NWP::begin()
    {
      return word;
    }
    NWP::iterator NWP::end()
    {
      return NULL;
    }
    
    //Nesting

    /** 
     *
     * @brief provide access to the next call node in the nesting 
     *
     * @return the next call node in the nesting
     *
     */
    NWPNode * NWP::currCall()
    {
      return word->currCall();
    }
      
    /** 
     *
     * @brief returns the current level of nesting
     *
     * @return the current level of nesting
     *
     */
    size_t NWP::nestSize()
    {
      return word->sizeNesting();
    }
    
  };
}
