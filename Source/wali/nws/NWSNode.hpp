#ifndef wali_nws_NWSNode_GUARD
#define wali_nws_NWSNode_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/Common.hpp"

// std::c++
#include <iostream>

namespace wali
{

  namespace nws
  {

    class NWSNode : public Printable
    {
      //
      // Methods
      //

    public:
      //Constructors and Destructor
      NWSNode( );
      NWSNode( Key sym );
      NWSNode( Key sym, NWSNode * next );
      NWSNode & operator=( const NWSNode & otherNWSNode );
  
      ~NWSNode( );


      //Accessors
      
      /** 
       *
       * @brief returns the symbol associated with this node
       *
       * This method provides access to the symbol associated with 
       * this node.
       *
       * @return the Key for the symbol associated with this node
       *
       */
      Key getSymbol( );
      
      /** 
       *
       * @brief sets the symbol associated with this node
       *
       * This method associates the given symbol with this node.
       *
       * @parm the new symbol for this node
       *
       */
      void setSymbol( Key sym );

      /** 
       *
       * @brief tests whether this symbol is the last symbol in  
       * the word suffix
       *
       * This method tests whether this node is the last node in 
       * the trace of the word suffix.
       *
       * @return true if this is not the last symbol in the word
       * suffix
       *
       */
      bool hasNext( );
      
      /** 
       *
       * @brief returns the next node in the word suffix
       *
       * This method provides access to the next node in the 
       * trace of the word suffix.
       *
       * @return the next node in the word suffix
       *
       */
      NWSNode * nextNode( );
      
      /** 
       *
       * @brief sets the next node in the word suffix
       * 
       * This method connects the given node to the end of the 
       * trace of the word suffix.
       *
       * @parm the node to append to the end of the word suffix
       *
       */
      void setNext( NWSNode * next );

      /** 
       *
       * @brief tests whether this is a call node
       *
       * This method tests whether there is a call originating at 
       * this node in the trace of the word suffix.
       *
       * @return true if this node is the beginning of a function
       * call
       *
       */
      bool isCall( );
      
      /** 
       *
       * @brief returns the return node connected to this call node 
       * in the word suffix
       *
       * This method provides access to the return node connected to 
       * this call node in the trace of the word suffix.
       *
       * @return the return node connected to this call node in the 
       * word suffix
       *
       */
      NWSNode * exitNode( );
      
      /** 
       *
       * @brief sets the return node to connect to this call node 
       * in the word suffix
       * 
       * This method connects the given node to this call node in the
       * word suffix.
       *
       * @parm the node to connect to this call node in the word 
       * suffix
       *
       */
      void setExit( NWSNode * exit );
      
      //Utilities
      
      /** 
       *
       * @brief prints a representation of this node to the output 
       * stream specified
       *
       * This method prints a representation of this nested word suffix
       * node to the output stream specified and returns the stream 
       * to which the representation was printed.  Note: The symbol ->
       * signifies the next symbol in the word suffix.  The symbol in 
       * parenthesis signifies the exit symbol for the level of nesting 
       * beginning at this symbol.
       *
       * @parm the output stream to which to print the node
       * @return the output stream to which the node was printed
       *
       */
      std::ostream & print( std::ostream & o ) const;
      
      /** 
       *
       * @brief tests the equivalence of two nested word suffix nodes
       *
       * This method tests whether this node and the node 'otherNWSNode'
       * are equivalent.
       *
       * @parm the node with which to compare this node
       * @return true if this node is equivalent to the node 
       * 'otherNWSNode'
       *
       */
      bool operator==( const NWSNode & otherNWSNode );

      //
      // Variables
      //
    protected:
      Key symbol;
      NWSNode * next;
      NWSNode * exit;
    };
  }
}
#endif
