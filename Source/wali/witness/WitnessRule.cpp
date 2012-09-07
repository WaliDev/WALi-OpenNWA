/*!
 * @author Nicholas Kidd
 */
#include "wali/Common.hpp"
#include "wali/witness/WitnessRule.hpp"
#include "wali/witness/Visitor.hpp"

namespace wali
{
  using wpds::Rule;

  namespace witness
  {
    ///////////////////
    // class RuleStub
    ///////////////////
    RuleStub::RuleStub( const Rule& r ) :
      fs(r.from_state()),
      fstk(r.from_stack()),
      ts(r.to_state()),
      tstk1(r.to_stack1()),
      tstk2(r.to_stack2()),
      se(r.weight())
    {}

    Key RuleStub::from_state()
    {
      return fs;
    }

    Key RuleStub::from_stack()
    {
      return fstk;
    }

    Key RuleStub::to_state()
    {
      return ts;
    }

    Key RuleStub::to_stack1()
    {
      return tstk1;
    }

    Key RuleStub::to_stack2()
    {
      return tstk2;
    }

    sem_elem_t RuleStub::weight()
    {
      return se;
    }

    std::ostream& RuleStub::print( std::ostream& o ) const
    {
      o << "<";
      o << key2str(fs);
      o << ", ";
      o << key2str(fstk);
      o << "> -> <";
      o << key2str(ts);
      o << ", ";
      if( tstk1 != WALI_EPSILON )
      {
        o << key2str(tstk1);
        if( tstk2 != WALI_EPSILON )
        {
          o << " ";
          o << key2str(tstk2);
        }
      }
      else {
        // sanity check
        assert( WALI_EPSILON == tstk2);
      }
      o << ">";
      o << "\t" << se->toString();
      return o;
    }

    ///////////////////
    // class WitnessRule
    ///////////////////
    WitnessRule::WitnessRule( const Rule& r_t ) :
      Witness(r_t.weight()),
      stub(r_t)
    {
    }

    //! Destructor does nothing.
    WitnessRule::~WitnessRule()
    {
    }

    //
    // Override Witness::accept
    //
    void WitnessRule::accept( Visitor& v, bool visitOnce ATTR_UNUSED )
    {
      // TODO how does marking work...need a flag maybe
      (void) visitOnce;
      mark();
      if(v.visitRule(this)) {
          v.postvisitRule(this);
      }
    }

    // override Witness::prettyPrint
    std::ostream& WitnessRule::prettyPrint( std::ostream& o,size_t depth ) const
    {
      formatDepth(o,depth);
      o << "WitnessRule: ";
      stub.print(o) << std::endl;
      return o;
    }

    RuleStub& WitnessRule::getRuleStub()
    {
      return stub;
    }

  } // namespace witness

} // namespace wali

/*
 * $Id: WitnessRule.cpp 1617 2012-02-07 23:15:05Z driscoll $
 */

