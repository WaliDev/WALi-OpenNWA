/*!
 * @author Nicholas Kidd
 *
 * @version $Id: Visitor.cpp 1093 2011-06-07 16:50:18Z driscoll $
 */

#include "wali/Common.hpp"
#include "wali/witness/Visitor.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/WitnessTrans.hpp"
#include "wali/witness/WitnessRule.hpp"
#include "wali/witness/WitnessMerge.hpp"
#include <iostream>
#include <cassert>

namespace wali
{
  namespace witness
  {
    bool Visitor::visit( Witness * w ATTR_UNUSED )
    {
      (void) w;
      *waliErr << "[ERROR] wali::witness::Visitor::visit invoked. Aborting.\n";
      return false;
    }

    bool Visitor::visitExtend( WitnessExtend * w ATTR_UNUSED )
    {
      (void) w;
      *waliErr << "[ERROR] wali::witness::Visitor::visitextend invoked. Aborting.\n";
      return false;
    }

    bool Visitor::visitCombine( WitnessCombine * w ATTR_UNUSED )
    {
      (void) w;
      *waliErr << "[ERROR] wali::witness::Visitor::visitCombine invoked. Aborting.\n";
      return false;
    }

    bool Visitor::visitRule( WitnessRule * w ATTR_UNUSED )
    {
      (void) w;
      *waliErr << "[ERROR] wali::witness::Visitor::visitRule invoked. Aborting.\n";
      return false;
    }

    bool Visitor::visitTrans( WitnessTrans * w ATTR_UNUSED )
    {
      (void) w;
      *waliErr << "[ERROR] wali::witness::Visitor::visitTrans invoked. Aborting.\n";
      return false;
    }

    bool Visitor::visitMerge( WitnessMerge * w ATTR_UNUSED )
    {
      (void) w;
      *waliErr << "[ERROR] wali::witness::Visitor::visitMerge invoked. Aborting.\n";
      return false;
    }

  } // namespace witness

} // namespace wali

