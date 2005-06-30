#ifndef wali_VISITOR_GUARD
#define wali_VISITOR_GUARD 1

/*!
 * @author Nick Kidd
 */
#include "wali/Common.hpp"

namespace wali
{
    namespace witness
    {
        /*!
         * @class Visitor
         * Declares the methods a Visitor must implement to traverse a Witness
         * DAG. Class Visitor provides a default method for each visit_* method
         * that simply aborts the computation.
         *
         * @see Visitable
         * @see Witness
         * @see WitnessExtend
         * @see WitnessCombine
         * @see WitnessRule
         * @see WitnessTrans
         */
        class Witness;
        class WitnessExtend;
        class WitnessCombine;
        class WitnessRule;
        class WitnessTrans; 

        class Visitor
        {
            public:
                virtual ~Visitor() {}
                virtual void visit( Witness * w );
                virtual void visit_extend( WitnessExtend * w );
                virtual void visit_combine( WitnessCombine * w );
                virtual void visit_rule( WitnessRule * w );
                virtual void visit_trans( WitnessTrans * w );

        }; // class Visitor

    } // namespace witness

} // namespace wali

#endif  // wali_VISITOR_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
