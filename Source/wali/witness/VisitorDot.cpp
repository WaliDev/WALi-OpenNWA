/**
 * @author Nicholas Kidd
 * @version $Id: VisitorDot.cpp 467 2008-10-23 20:47:11Z kidd $
 */

#include "wali/Common.hpp"
#include "wali/witness/VisitorDot.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/WitnessRule.hpp"
#include "wali/witness/WitnessTrans.hpp"
#include "wali/witness/WitnessMerge.hpp"
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

    bool VisitorDot::visit( Witness * w )
    {
      printNode(w,"red");
      return true;
    }

    bool VisitorDot::visitExtend( WitnessExtend * w )
    {
      // left child
      if( w->hasLeft() ) {
        Witness* left = w->left().get_ptr();
        printEdge(w,left);
      }

      // right child
      if( w->hasRight() ) {
        Witness* right = w->right().get_ptr();
        printEdge(w,right);
      }

      // this
      printNode(w, "green", "(x) " + w->weight()->toString());
      return true;
    }

    bool VisitorDot::visitCombine( WitnessCombine * w )
    {
      std::list< witness_t >::iterator it = w->children().begin();
      std::list< witness_t >::iterator itEND = w->children().end();
      // each child
      for( ; it != itEND ; it++ )
      {
        Witness* child = it->get_ptr();
        printEdge(w,child);
      }

      // this
      printNode(w, "lightblue", "(+) " + w->weight()->toString());
      return true;
    }

    bool VisitorDot::visitRule( WitnessRule * w )
    {
      printNode(w,"yellow",w->getRuleStub().toString());
      return true;
    }

    bool VisitorDot::visitTrans( WitnessTrans * w )
    {
      printNode(w,"orangered",w->getTrans().toString());
      return true;
    }

    bool VisitorDot::visitMerge( WitnessMerge * w ) {
      // Caller
      if (w->hasCaller()) {
        Witness* caller = w->caller().get_ptr();
        printEdge(w,caller);
      }

      // Rule
      if (w->hasRule()) {
        Witness *rule = w->rule().get_ptr();
        printEdge(w,rule);
      }

      // Callee
      if (w->hasCallee()) {
        Witness* callee = w->callee().get_ptr();
        printEdge(w,callee);
      }

      // this
      printNode(w, "purple", "(M) " + w->weight()->toString());
      return true;
    }

    void VisitorDot::printNodeName( Witness * w )
    {
      os << "\tw" << (size_t)w;
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
      printNode(w,color,w->weight()->toString());
    }

    void VisitorDot::printNode( Witness * w, std::string color, std::string label )
    {
      os << "\t";
      printNodeName(w);
      os << " [label=\"" << label;
      os << "\",color=" << color << ",style=filled];\n";
    }

  } // namespace witness

} // namespace wali

