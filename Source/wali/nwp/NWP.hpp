#ifndef wali_nwp_NWP_GUARD
#define wali_nwp_NWP_GUARD 1

/**
 * @author Amanda Burton
 */

#include "wali/nwa/deprecate.h"

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
    class
    DEPRECATE("I don't think anyone is using this. If you are, talk to Evan.")
    NWP : public Printable
    {
      typedef NWPNode * iterator;
      
      //
      // Methods
      //

    public:
      //Constructors and Destructor
      NWP( );
      //NWP( std::vector< Key > sym, std::queue< std::pair< int,int > > nest ); 
      NWP( NWP & other );
      NWP & operator=( NWP & other );
     
      ~NWP( );


      //Accessors

      /** 
       *
       * @brief appends a symbol to the end of the word prefix
       *
       * This method appends an intraprocedural node with the 
       * given symbol to the end of the word prefix trace.  
       *
       * @parm the symbol to add to the word prefix
       * @return false if the prev link could not be created
       *
       */
      bool addIntraNode( Key sym );

      /** 
       *
       * @brief appends a symbol to the end of the word prefix
       *
       * This method appends a call node with the 
       * given symbol to the end of the word prefix trace.  
       *
       * @parm the symbol to add to the word prefix
       * @return false if the prev link could not be created
       *
       */
      bool addCallNode( Key sym );
      
      /** 
       *
       * @brief appends a symbol to the end of the word prefix
       *
       * This method appends a return node with the given symbol 
       * to the end of the word prefix trace.  
       *
       * @parm sym: the symbol to add to the word prefix
       * @return false if any link could not be created
       *
       */
      bool addReturnNode( Key sym );
      
	   /** 
      *
      * @brief appends a symbol to the end of the word prefix
      *
      * This method appends the given intraprocedural node to 
      * the end of the word prefix trace.  If this node is already linked 
      * with a different previous node, the prev link is not created, the node 
      * is not appended, and false is returned.  
      *
      * @parm the node to add to the word prefix
	    * @return false if the prev link could not be created
      *
      */
     //bool addIntraNode( NWPNode * node );

     /** 
      *
      * @brief appends a symbol to the end of the word prefix
      *
      * This method appends the given call node to 
      * the end of the word prefix trace.  If this node is already linked 
      * with a different previous node, the prev link is not created, the node 
      * is not appended, and false is returned.  
      *
      * @parm the node to add to the word prefix
	    * @return false if the prev link could not be created
      *
      */
     //bool addCallNode( NWPNode * node );

      /** 
       *
       * @brief appends a symbol to the end of the word prefix
       *
       * This method appends the given return node 
       * to the end of the word prefix trace.  If this node is already
       * linked with a different previous node, the prev link is not
       * created, the node is not appended, and false is returned.  If
       * there is no open call on the nesting, the call/return link is 
       * not created, the node is not appended, and false is returned.
       *
       * @parm sym: the return node to add to the word prefix
       * @return false if some link could not be created
       *
       */
      //bool addReturnNode( NWPNode * node );
      
      /** 
       *
       * @brief provides access to the end node in the word prefix
       *
       * This method provides access to the node at the end 
       * of the word prefix trace.
       *
       * @return the end node in the word prefix
       *
       */
      NWPNode * endNode();
      
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
      
      //Iteration
      iterator begin();
      iterator end();      
      
      //Nesting

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
      NWPNode * currCall();
      
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
      size_t nestSize();

      //
      // Variables
      //
    protected:
      NWPNode * word;
    };


  }

}
#endif
