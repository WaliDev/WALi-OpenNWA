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
    // class RuleDotty
    /////////////////////////////////////////////////////////////////
    RuleDotty::RuleDotty(std::ostream & o, bool _print_states) : os(o), print_states(_print_states) {}

    RuleDotty::~RuleDotty() {}

    void RuleDotty::operator()( const rule_t & r )
    {
      // Make the node labels for each configuration (note: labels may be
      // repeated, but dot plays nicely with this, and there's not an easy way
      // to get a list of labels otherwise; plus, this has the added advantage
      // that only labels which are used are generated, vs. generating labels
      // for the entire stack/state product).

      // from state
      os << "\t"
         << r->from_state()
         << r->from_stack()
         << " [label=\"";
      if( print_states )
      {
        os << key2str( r->from_state() ) << ",";
      }
      os << key2str( r->from_stack() ) << "\"];\n";

      // to if necessary
      if( r->to_stack1() != WALI_EPSILON )
      {
        os << "\t"
           << r->to_state()
           << r->to_stack1()
           << " [label=\"";
        if( print_states )
        {
          os << key2str( r->to_state() ) << ",";
        }
        os << key2str( r->to_stack1() ) << "\"];\n";
      }
      else // otherwise make a node for the epsilon edge to point to
      {
        os << "\tinvisible"
           << r->to_state()
           << r->from_stack()
           << " [";

        if(print_states)
        {
          os << "label=\"" << key2str( r->to_state() ) << "\"";
        }
        else
        {
          os << "style=invisible";
        }
        
        os << "];\n";
      }

      // now generate the edge
      os << "\t"
         << r->from_state()
         << r->from_stack()
         << " -> ";

      // Make a unique invisible state so that there is an edge to "nowhere"
      // for this delta-0 transition.
      if( r->to_stack1() == WALI_EPSILON && r->to_stack2() == WALI_EPSILON )
      {
        os << "invisible" << r->to_state() << r->from_stack() << ";\n";
        return;
      }

      // delta-1
      os << r->to_state() << r->to_stack1();

      // delta-2
      if( r->to_stack2() != WALI_EPSILON )
      {
        os << " [label=\"" << key2str( r->to_stack2() ) << "\"]";
      }

      os << ";\n";
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

