#ifndef wali_nwp_NWPNode_GUARD
#define wali_nwp_NWPNode_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"

// std::c++
#include <iostream>

namespace wali
{

  namespace nwp
  {
  
    class NWPNode : public Printable
    {
      //
      // Methods
      //

    public:
      //Constructors and Destructor
      NWPNode( );
      NWPNode( Key sym );
      NWPNode( Key sym, NWPNode * prev );
      NWPNode & operator=( const NWPNode & otherNWPNode );
  
      ~NWPNode( );


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
       * @brief tests whether this symbol is the first symbol in  
       * the word prefix
       *
       * This method tests whether this node is the first node in 
       * the trace of the word prefix.
       *
       * @return true if this is not the first symbol in the word
       * prefix
       *
       */
      bool hasPrev( );
      
      /** 
       *
       * @brief returns the previous node in the word prefix
       *
       * This method provides access to the previous node in the 
       * trace of the word prefix.
       *
       * @return the previous node in the word prefix
       *
       */
      NWPNode * prevNode( );
      
      /** 
       *
       * @brief sets the previous node in the word prefix
       * 
       * This method connects the given node to the end of the 
       * trace of the word prefix.
       *
       * @parm the node to append to the end of the word prefix
       *
       */
      void setPrev( NWPNode * prev );

      /** 
       *
       * @brief tests whether this is a call node
       *
       * This method tests whether there is a call originating at 
       * this node in the trace of the word prefix.
       *
       * @return true if this node is the beginning of a function
       * call
       *
       */
      bool isCall( );
      
      /** 
       *
       * @brief returns the return node connected to this call node 
       * in the word prefix
       *
       * This method provides access to the return node connected to 
       * this call node in the trace of the word prefix.
       *
       * @return the return node connected to this call node in the 
       * word prefix
       *
       */
      NWPNode * exitNode( );
      
      /** 
       *
       * @brief sets the return node to connect to this call node 
       * in the word prefix
       * 
       * This method connects the given node to this call node in the
       * word prefix.
       *
       * @parm the node to connect to this call node in the word 
       * prefix
       *
       */
      void setExit( NWPNode * exit );
      
      //Utilities
      
      /** 
       *
       * @brief prints a representation of this node to the output 
       * stream specified
       *
       * This method prints a representation of this nested word prefix
       * node to the output stream specified and returns the stream 
       * to which the representation was printed.  Note: The symbol <-
       * signifies the previous symbol in the word suffix.  The symbol in 
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
       * @brief tests the equivalence of two nested word prefix nodes
       *
       * This method tests whether this node and the node 'otherNWPNode'
       * are equivalent.
       *
       * @parm the node with which to compare this node
       * @return true if this node is equivalent to the node 
       * 'otherNWPNode'
       *
       */
      bool operator==( const NWPNode & otherNWPNode );

      //
      // Variables
      //
    protected:
      Key symbol;
      NWPNode * prev;
      //NWPNode * next;
      NWPNode * exit;
      //NWPNode * call;
    };

  }
}
#endif