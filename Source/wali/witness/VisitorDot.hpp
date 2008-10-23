#ifndef witness_VISITOR_DOT_GUARD
#define witness_VISITOR_DOT_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/Visitor.hpp"
#include <iostream>
#include <string>

namespace wali
{
  namespace witness
  {
    /**
     * @class VisitorDot
     */ 
    class VisitorDot : public Visitor
    {
      public:
        VisitorDot( std::ostream& o );

        virtual ~VisitorDot();
        virtual bool visit( Witness * w );
        virtual bool visitExtend( WitnessExtend * w );
        virtual bool visitCombine( WitnessCombine * w );
        virtual bool visitRule( WitnessRule * w );
        virtual bool visitTrans( WitnessTrans * w );
        virtual bool visitMerge( WitnessMerge * w );

        //
        // Helper functions for printing in Witness nodes
        // in dotty format
        //
        void printNodeName( Witness * w );

        void printEdge( Witness * head, Witness * tail );

        void printNode( Witness * w, std::string color );

        void printNode( Witness * w, std::string color, std::string label );

        void printNodeName( witness_t& wit ) {
          printNodeName( wit.get_ptr() );
        }
        void printEdge( witness_t& h,witness_t& t ) {
          printEdge(h.get_ptr(),t.get_ptr());
        }
        void printNode( witness_t& w,std::string color ) {
          printNode( w.get_ptr(),color );
        }
      protected:
        std::ostream& os;

    }; // class VisitorDot

  } // namespace witness

} // namespace wali

#endif  // witness_VISITOR_DOT_GUARD

