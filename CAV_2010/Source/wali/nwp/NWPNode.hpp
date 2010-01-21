#ifndef wali_nwp_NWPNode_GUARD
#define wali_nwp_NWPNode_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/Common.hpp"

// std::c++
#include <iostream>
#include <deque>

namespace wali
{

  namespace nwp
  {
  
    class NWPNode : public Printable
    {
    public: 
      enum Type {Call,Internal,Ret};
      
      //
      // Methods
      //

    public:
      //Constructors and Destructor
      NWPNode( Key sym, Type nodeType ); //This is the last node.
      NWPNode( Key sym, NWPNode * prev, Type nodeType );
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
      bool isFirst( );
      
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
      bool isCallNode( );
      bool isRetNode();
      bool isInternalNode();
            
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
      NWPNode * callNode( );
      
      /**
       *
       * @brief returns the innermost open call of the nesting
       *
       * This method returns the innermost open call of the nesting.
       *
       * @return the innermost open call of the nesting
       *
       */
      NWPNode * currCall();
      
      /**
       *
       * @brief returns the nesting of the prefix at this node 
       *
       * This method provides access to the nesting of the prefix at 
       * this node.
       *
       * @return the nesting of the prefix at this node
       *
       */
      std::deque< NWPNode * > getNesting( );
      
      /**
       *
       * @brief sets the nesting of the prefix at this node 
       *
       * This method records the current nesting of the prefix.
       *
       * @param the nesting of the prefix at this node
       *
       */
      void pushCall(NWPNode* call);
      NWPNode* popCall();    
      
      size_t sizeNesting();  
      
      //Iteration
      NWPNode * operator++();
      
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
      Type nodeType;
      NWPNode * prev;
      NWPNode * call;
      std::deque< NWPNode * > nesting;
    };

  }
}
#endif