#ifndef witness_VISITOR_DOT_GUARD
#define witness_VISITOR_DOT_GUARD 1

/*!
 * @author Nick Kidd
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
        // Avoid including too many files
        class Witness;
        typedef ref_ptr<Witness> witness_t;

        /*!
         * @class VisitorDot
         */
        class VisitorDot : public Visitor
        {
            public:
                VisitorDot( std::ostream& o );

                virtual ~VisitorDot();
                virtual void visit( Witness * w );
                virtual void visit_extend( WitnessExtend * w );
                virtual void visit_combine( WitnessCombine * w );
                virtual void visit_rule( WitnessRule * w );
                virtual void visit_trans( WitnessTrans * w );

                void print_node_name( Witness * w );
                void print_edge( Witness * head, Witness * tail );
                void print_node( Witness * w, std::string color );

                void print_node_name( witness_t& wit ) {
                    print_node_name( wit.get_ptr() );
                }
                void print_edge( witness_t& h,witness_t& t ) {
                    print_edge(h.get_ptr(),t.get_ptr());
                }
                void print_node( witness_t& w,std::string color ) {
                    print_node( w.get_ptr(),color );
                }
            protected:
                std::ostream& os;

        }; // class VisitorDot

    } // namespace witness

} // namespace wali

#endif  // witness_VISITOR_DOT_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/
