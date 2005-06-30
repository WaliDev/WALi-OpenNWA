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
        void Visitor::visit( Witness * w ATTR_UNUSED )
        {
            std::cerr << "[ERROR] wali::Visitor::visit invoked. Aborting.\n";
            assert(0);
        }

        void Visitor::visit_extend( WitnessExtend * w ATTR_UNUSED )
        {
            std::cerr << "[ERROR] wali::Visitor::visit_extend invoked. Aborting.\n";
            assert(0);
        }

        void Visitor::visit_combine( WitnessCombine * w ATTR_UNUSED )
        {
            std::cerr << "[ERROR] wali::Visitor::visit_combine invoked. Aborting.\n";
            assert(0);
        }

        void Visitor::visit_rule( WitnessRule * w ATTR_UNUSED )
        {
            std::cerr << "[ERROR] wali::Visitor::visit_rule invoked. Aborting.\n";
            assert(0);
        }

        void Visitor::visit_trans( WitnessTrans * w ATTR_UNUSED )
        {
            std::cerr << "[ERROR] wali::Visitor::visit_trans invoked. Aborting.\n";
            assert(0);
        }

    } // namespace witness

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
