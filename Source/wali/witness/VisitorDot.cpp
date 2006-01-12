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
            printNode(w,"red");
        }

        void VisitorDot::visitExtend( WitnessExtend * w )
        {
            // left child
            Witness* left = w->left().get_ptr();
            printEdge(w,left);
            left->accept(*this);

            // right child
            Witness* right = w->right().get_ptr();
            printEdge(w,right);
            right->accept(*this);

            // this
            printNode(w,"green");
        }

        void VisitorDot::visitCombine( WitnessCombine * w )
        {
            std::list< witness_t >::iterator it = w->children().begin();
            std::list< witness_t >::iterator itEND = w->children().end();
            // each child
            for( ; it != itEND ; it++ )
            {
                Witness* child = it->get_ptr();
                printEdge(w,child);
                child->accept(*this);

            }

            // this
            printNode(w,"light_blue");
        }

        void VisitorDot::visitRule( WitnessRule * w )
        {
            printNode(w,"yellow");
        }

        void VisitorDot::visitTrans( WitnessTrans * w )
        {
            printNode(w,"orange");
        }

        void VisitorDot::printNodeName( Witness * w )
        {
            os << "\tw" << (unsigned int)w;
        }

        void VisitorDot::printEdge( Witness * head, Witness * tail )
        {
            printNodeName( head );
            os << " -> ";
            printNodeName( tail );
            os << ";\n";
        }

        void VisitorDot::printNode( Witness * w, std::string color )
        {
            os << "\t";
            printNodeName(w);
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

