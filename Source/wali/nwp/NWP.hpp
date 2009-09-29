 #ifndef wali_nwp_NWP_GUARD
#define wali_nwp_NWP_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/nwp/NWPNode.hpp"

// std::c++
#include <iostream>
#include <stack>
#include <deque>
#include <vector>
#include <queue>

namespace wali
{

  namespace nwp
  {
  
  class NWP : public Printable
    {
      //
      // Methods
      //

    public:
      //Constructors and Destructor
      NWP( );
      NWP( std::stack< NWPNode > st, NWPNode * trace );
      NWP( std::vector< Key > sym, std::queue< std::pair< int,int > > nest );
      NWP( NWP & other );
      NWP & operator=( NWP & other );
     
      ~NWP( );


      //Accessors
      
      /** 
       *
       * @brief provide access to the next call node in the nesting
       *
       * This method provides access to the call node on the top of 
       * the nesting stack.
       *
       * @return the next call node in the nesting
       *
       */
      NWPNode * stackTop();
      
      /** 
       *
       * @brief adds the given call node to the nesting
       *
       * This method pushes the given call node onto the nesting stack.
       *
       * @parm the call node to add to the nesting
       *
       */
      void pushStack( NWPNode * call );
      
      /** 
       *
       * @brief removes the next call node from the nesting
       *
       * This method removes the next call node from the nesting stack.
       *
       */
      void popStack();
      
      /** 
       *
       * @brief returns the current level of nesting
       *
       * This method returns the current level of nesting (the size of 
       * the stack). 
       *
       * @return the current level of nesting
       *
       */
      size_t stackSize();

      /** 
       *
       * @brief appends a symbol to the end of the word prefix
       *
       * This method appends a node with the given symbol to the end 
       * of the word prefix trace.
       *
       * @parm the symbol to add to the word prefix
       *
       */
      void addNode( Key sym );
      
      /** 
       *
       * @brief appends a symbol to the end of the word prefix
       *
       * This method appends a node with the given symbol and return 
       * node to the end of the word prefix trace.  If this node 
       * is already a call node with a different return node, the 
       * call/return link is not created, the node is not appended, and 
       * false is returned.
       *
       * @parm sym: the symbol to add to the word prefix
       * @parm returnNode: the return node that is paired with this call 
       * @return false if the call/return link could not be created
       *
       */
      bool addNode( Key sym, NWPNode * returnNode );
      
      //TODO: addNode( NWPNode * newNode ) - this should allow for automatic
      //update to the nesting stack if this node is a call node or return node
      //but what should be done with a pre-existing prev pointer?
      
      /** 
       *
       * @brief provides access to the previous node in the word prefix
       *
       * This method provides access to the previous node at the end 
       * of the word prefix trace.
       *
       * @return the previous node in the word prefix
       *
       */
      NWPNode * prevNode();
      
      /**
       * 
       * @brief removes the node at the end of the word prefix
       *
       * This method removes the node at the end of the word prefix/undoes
       * the last addNode().  It does not change the nesting stack.
       *
       */
      void undoNode();
      
      /** 
       *
       * @brief removes the node at the end of the word prefix
       *
       * This method removes the node at the end of the word prefix, adds any newly
       * opened nesting to the nesting stack, and returns the node that was removed from the 
       * word prefix trace.
       *
       * @return the node that was removed from the beginning of the word suffix
       *
       */
      NWPNode * removeNode();
      
      //Utilities
      
      /** 
       *
       * @brief prints a representation of the nested word prefix to the 
       * given output stream
       *
       * This method prints a representation of the nested word prefix to 
       * the given output stream and returns the output stream.
       *
       * @parm the output stream to which the representation is to be 
       * printed
       * @return the output stream to which the representation was printed
       *
       */
      std::ostream & print( std::ostream & o ) const;
      
      /** 
       *
       * @brief tests the equivalence of two nested word prefixes
       *
       * This method tests the equivalence of this nested word prefix and 
       * the given nested word prefix.
       *
       * @parm the nested word prefix to which to compare this nested word 
       * prefix
       * @return true if this nested word prefix and the given one are 
       * equivalent, false otherwise
       *
       */
      bool operator==( NWP & otherNWP );

      //Uses
      
      /** 
       *
       * @brief tests whether this nested word prefix is empty
       * 
       * This method tests whether this nested word prefix is empty.
       *
       * @return true if this nested word prefix is empty, false otherwise
       *
       */
      bool isEmpty();
      
      private:
      /** 
       *
       * @brief finds the call node associated with the given return node 
       * in the word prefix
       *
       * This method finds the call node associated with the given return 
       * node in the word prefix.  If the given node is not the return node
       * for any call node in the word prefix NULL is returned.
       *
       * @return the call node associated with the given return node, or NULL
       * if no such node exists in the word prefix
       *
       */
      NWPNode * getCall( NWPNode * returnNode, NWPNode * currNode );
      

      //
      // Variables
      //
    protected:
      std::deque< NWPNode > nesting;
      //NWPNode * start;
      NWPNode * word;
    };


  }

}
#endif