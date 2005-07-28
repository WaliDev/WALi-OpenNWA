/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/VisitorDot.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/WitnessRule.hpp"
#include "wali/witness/WitnessTrans.hpp"
#include <fstream>

namespace wali
{
    namespace witness
    {
        VisitorDot::VisitorDot( std::ostream& oparam ) :
            os(oparam)
        {
            os << "digraph \"VisitorDot@" << std::hex << (void*)this << std::dec;
            os << "\" {\n";
        }

        VisitorDot::~VisitorDot()
        {
            os << "}\n";
        }

        void VisitorDot::visit( Witness * w )
        {
            print_node(w,"red");
        }

        void VisitorDot::visit_extend( WitnessExtend * w )
        {
            // left child
            print_edge(w,w->left().get_ptr());

            // right child
            print_edge(w,w->right().get_ptr());

            // this
            print_node(w,"green");
        }

        void VisitorDot::visit_combine( WitnessCombine * w )
        {
            std::list< witness_t >::iterator it = w->children().begin();
            std::list< witness_t >::iterator itEND = w->children().end();
            // each child
            for( ; it != itEND ; it++ )
            {
                print_edge(w,it->get_ptr());
            }

            // this
            print_node(w,"light_blue");
        }

        void VisitorDot::visit_rule( WitnessRule * w )
        {
            print_node(w,"yellow");
        }

        void VisitorDot::visit_trans( WitnessTrans * w )
        {
            print_node(w,"orange");
        }

        void VisitorDot::print_node_name( Witness * w )
        {
            os << "\tw" << (unsigned int)w;
        }

        void VisitorDot::print_edge( Witness * head, Witness * tail )
        {
            print_node_name( head );
            os << " -> ";
            print_node_name( tail );
            os << ";\n";
        }

        void VisitorDot::print_node( Witness * w, std::string color )
        {
            os << "\t";
            print_node_name(w);
            os << " [label=\"" << w->weight()->to_string();
            os << "\",color=" << color << ",style=filled];\n";
        }

    } // namespace witness

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

