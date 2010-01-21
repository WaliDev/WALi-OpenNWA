#ifndef wali_witness_VISITOR_PRINTER_GUARD
#define wali_witness_VISITOR_PRINTER_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/ref_ptr.hpp"

#include "wali/witness/Visitor.hpp"

#include <iostream>
#include <string>

namespace wali 
{
  namespace witness 
  {
    class VisitorPrinter : public Visitor
    {
      public:
        VisitorPrinter( std::ostream& o );
        virtual ~VisitorPrinter();
        virtual bool visit( Witness * w );
        virtual bool visitExtend( WitnessExtend * w );
        virtual bool visitCombine( WitnessCombine * w );
        virtual bool visitRule( WitnessRule * w );
        virtual bool visitTrans( WitnessTrans * w );
        virtual bool visitMerge( WitnessMerge * w );

        //! Prints "  |" to parameter o for each level of depth
        void formatDepth() const;

      protected:
        std::ostream& os;
        size_t depth;

      private:
        VisitorPrinter( std::ostream& os, size_t depth );
    };

  } // namespace witness

} // namespace wali

#endif  // wali_witness_VISITOR_PRINTER_GUARD

