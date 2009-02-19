/*!
 * @author Nicholas Kidd
 * @author Akash Lal
 */

#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/WPDS.hpp"

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
      r->marshall( os << "\t" ) << "\n";
    }

    /////////////////////////////////////////////////////////////////
    // class WpdsStackSymbols
    /////////////////////////////////////////////////////////////////
    WpdsStackSymbols::WpdsStackSymbols( ) {}

    WpdsStackSymbols::~WpdsStackSymbols() {}

    void WpdsStackSymbols::operator()( const rule_t & r )
    {
      addPoint(r->from_state(), pdsStates);
      addPoint(r->to_state(), pdsStates);
      addPoint(r->from_stack(), gamma);
      addPoint(r->to_stack1(), gamma);
      addPoint(r->to_stack2(), gamma);
      addPoint(r->to_stack2(), returnPoints);
      if(r->to_stack2() != WALI_EPSILON) {
        addPoint(r->to_stack1(), entryPoints);
      }
    }

    void WpdsStackSymbols::addPoint(Key k, std::set< Key > &s) {
      if(k != WALI_EPSILON) 
        s.insert(k);
    }

    /////////////////////////////////////////////////////////////////
    // class RuleCopier
    /////////////////////////////////////////////////////////////////
    RuleCopier::RuleCopier(WPDS& w) : w(w)
    {
    }

    void RuleCopier::operator()( const rule_t & r)
    {
      w.add_rule(
          r->from_state(), r->from_stack(),
          r->to_state(), r->to_stack1(), r->to_stack2(),
          r->weight());
    }

  } // namespace wpds

} // namespace wali

