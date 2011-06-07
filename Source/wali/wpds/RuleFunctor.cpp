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
      // Is it an non-exit rule?
      if(r->to_stack1() != WALI_EPSILON) {
        addPoint(r->to_stack1(), gamma);
      }
      // Is it a call rule?
      if(r->to_stack2() != WALI_EPSILON) {
        addPoint(r->to_stack2(), gamma);
        addPoint(r->to_stack1(), entryPoints);
        addPoint(r->from_stack(), callPoints);
        addPoint(r->to_stack2(), returnPoints);
      }
    }

    void WpdsStackSymbols::addPoint(Key k, std::set< Key > &s) {
      if(k != WALI_EPSILON) 
        s.insert(k);
    }

    /////////////////////////////////////////////////////////////////
    // class WpdsRules
    /////////////////////////////////////////////////////////////////
    // @author Amanda Burton
    
    WpdsRules::WpdsRules() {}
    
    WpdsRules::~WpdsRules() {}
    
    void WpdsRules::operator() (const rule_t & r)
    {
      if( r->is_rule2() )
        addRule(*r, pushRules);
      else if( r->to_stack1() != WALI_EPSILON )
        addRule(*r, stepRules);
      else
        addRule(*r, popRules);
    }
    
    void WpdsRules::addRule(Rule r, std::set< Rule > &s)
    {
      s.insert(r);
    }

    /////////////////////////////////////////////////////////////////
    // class RuleCopier
    /////////////////////////////////////////////////////////////////
    RuleCopier::RuleCopier(WPDS& wpds,ref_ptr<Wrapper> wr) : w(wpds),wrapper(wr)
    {
    }

    void RuleCopier::operator()( const rule_t & r)
    {
      sem_elem_t se = r->weight();
      if (wrapper.is_valid())
        se = wrapper->unwrap(se);
      w.add_rule(
          r->from_state(), r->from_stack(),
          r->to_state(), r->to_stack1(), r->to_stack2(),
          se);
    }

  } // namespace wpds

} // namespace wali

