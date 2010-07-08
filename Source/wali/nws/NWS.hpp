#ifndef wali_nws_NWS_GUARD
#define wali_nws_NWS_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/nws/NWSNode.hpp"

// std::c++
#include <iostream>
#include <stack>
#include <deque>
#include <vector>
#include <queue>

namespace wali
{

  namespace nws
  {

    class NWS : public Printable
    {
      public:
      typedef std::deque<NWSNode *>::iterator iterator;
    
      //
      // Methods
      //

    public:
      //Constructors and Destructor
      NWS( );
      NWS( std::stack< NWSNode > st, NWSNode * trace );
      NWS( std::vector< Key > sym, std::queue< std::pair< int,int > > nest );
      NWS( const NWS & other );
      NWS & operator=( const NWS & other );
     
      ~NWS( );


      //Accessors
      
      /** 
       *
       * @brief provide access to the next return node in the nesting
       *
       * This method provides access to the return node on the top of 
       * the nesting stack.
       *
       * @return the next return node in the nesting
       *
       */
      NWSNode * stackTop();
      
      /** 
       *
       * @brief adds the given return node to the nesting
       *
       * This method pushes the given return node onto the nesting stack.
       *
       * @parm the return node to add to the nesting
       *
       */
      void pushStack( NWSNode * exit );
      
      /** 
       *
       * @brief removes the next return node from the nesting
       *
       * This method removes the next return node from the nesting stack.
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
       */
      iterator beginStack();
      
      /**
       *
       */
      iterator endStack();

      /** 
       *
       * @brief appends a symbol to the end of the word suffix
       *
       * This method appends a node with the given symbol to the beginning 
       * of the word suffix trace.
       *
       * @parm the symbol to add to the word suffix
       *
       */
      void addNode( Key sym );
      
      /** 
       *
       * @brief appends a symbol to the beginning of the word suffix
       *
       * This method appends a node with the given symbol and return 
       * node to the beginning of the word suffix trace.  If this node 
       * is already a call node with a different return node, the 
       * call/return link is not created, the node is not appended, and 
       * false is returned.
       *
       * @parm sym: the symbol to add to the word suffix
       * @parm returnNode: the return node that is paired with this call 
       * @return false if the call/return link could not be created
       *
       */
      bool addNode( Key sym, NWSNode * returnNode );
      
      //TODO: addNode( NWSNode * newNode ) - this should allow for automatic
      //update to the nesting stack if this node is a call node or return node
      //but what should be done with a pre-existing next pointer?
      
      /** 
       *
       * @brief provides access to the next node in the word suffix
       *
       * This method provides access to the next node at the beginning 
       * of the word suffix trace.
       *
       * @return the next node in the word suffix
       *
       */
      NWSNode * nextNode();
      
      /**
       * 
       * @brief removes the node at the beginning of the word suffix
       *
       * This method removes the node at the beginning of the word suffix/undoes
       * the last addNode().  It does not change the nesting stack.
       *
       */
      void undoNode();
      
      /** 
       *
       * @brief removes the node at the beginning of the word suffix
       *
       * This method removes the node at the beginning of the word suffix, adds any newly
       * opened nesting to the nesting stack, and returns the node that was removed from the 
       * word suffix trace.
       *
       * @return the node that was removed from the beginning of the word suffix
       *
       */
      NWSNode * removeNode();
      
      //Utilities
      
      /** 
       *
       * @brief prints a representation of the nested word suffix to the 
       * given output stream
       *
       * This method prints a representation of the nested word suffix to 
       * the given output stream
       * and returns the output stream.
       *
       * @parm the output stream to which the representation is to be 
       * printed
       * @return the output stream to which the representation was printed
       *
       */
      std::ostream & print( std::ostream & o ) const;
      
      /** 
       *
       * @brief tests the equivalence of two nested word suffixes
       *
       * This method tests the equivalence of this nested word suffix and 
       * the given nested word suffix.
       *
       * @parm the nested word suffix to which to compare this nested word 
       * suffix
       * @return true if this nested word suffix and the given one are 
       * equivalent, false otherwise
       *
       */
      bool operator==( const NWS & otherNWS ) const;

      //Uses
      
      /** 
       *
       * @brief tests whether this nested word suffix is empty
       * 
       * This method tests whether this nested word suffix is empty.
       *
       * @return true if this nested word suffix is empty, false otherwise
       *
       */
      bool isEmpty();

      //
      // Variables
      //
    protected:
      std::deque< NWSNode * > nesting;
      NWSNode * word;
      //NWSNode * end;
    };

  }

}
#endif
