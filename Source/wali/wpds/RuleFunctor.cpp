/*!
 * @author Nick Kidd
 */

#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"

namespace wali
{
    namespace wpds
    {
        /////////////////////////////////////////////////////////////////
        // class RulePrinter
        /////////////////////////////////////////////////////////////////
        RulePrinter::RulePrinter( std::ostream & o ) : os(o) {}

        RulePrinter::~RulePrinter() {}

        void RulePrinter::operator()( const rule_t & r )
        {
            r->print( os ) << std::endl;
        }

        /////////////////////////////////////////////////////////////////
        // class RuleMarshaller
        /////////////////////////////////////////////////////////////////
        RuleMarshaller::RuleMarshaller( std::ostream & o ) : os(o) {}

        RuleMarshaller::~RuleMarshaller() {}

        void RuleMarshaller::operator()( const rule_t & r )
        {
            r->marshall( os ) << std::endl;
        }

    }
}

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
