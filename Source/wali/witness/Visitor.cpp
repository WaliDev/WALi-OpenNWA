/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/Visitor.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/WitnessTrans.hpp"
#include "wali/witness/WitnessRule.hpp"
#include <iostream>
#include <cassert>

namespace wali
{
    namespace witness
    {
        //
        // TODO : Replace std::cerr with a WALi error ostream.
        //
        bool Visitor::visit( Witness * w ATTR_UNUSED )
        {
            std::cerr << "[ERROR] wali::Visitor::visit invoked. Aborting.\n";
            return false;
        }

        bool Visitor::visitExtend( WitnessExtend * w ATTR_UNUSED )
        {
            std::cerr << "[ERROR] wali::Visitor::visitextend invoked. Aborting.\n";
            return false;
        }

        bool Visitor::visitCombine( WitnessCombine * w ATTR_UNUSED )
        {
            std::cerr << "[ERROR] wali::Visitor::visitCombine invoked. Aborting.\n";
            return false;
        }

        bool Visitor::visitRule( WitnessRule * w ATTR_UNUSED )
        {
            std::cerr << "[ERROR] wali::Visitor::visitRule invoked. Aborting.\n";
            return false;
        }

        bool Visitor::visitTrans( WitnessTrans * w ATTR_UNUSED )
        {
            std::cerr << "[ERROR] wali::Visitor::visitTrans invoked. Aborting.\n";
            return false;
        }

    } // namespace witness

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
