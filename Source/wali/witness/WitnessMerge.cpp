/**
 * @author Nicholas Kidd
 * @version $Id: WitnessMerge.cpp 1617 2012-02-07 23:15:05Z driscoll $
 */

#include "wali/Common.hpp"
#include "wali/witness/WitnessMerge.hpp"
#include "wali/witness/WitnessMergeFn.hpp"
#include "wali/witness/Visitor.hpp"

namespace wali
{
  namespace witness
  {
    // weight == merge->apply_f(left,right)
    WitnessMerge::WitnessMerge( 
        sem_elem_t the_weight, 
        witness_merge_fn_t the_witness_merge_fn,
        witness_t the_caller, 
        witness_t the_rule, 
        witness_t the_callee ) :
      Witness(the_weight), witness_merge_fn(the_witness_merge_fn),
      fCaller(the_caller),fRule(the_rule),fCallee(the_callee) 
    {
    }

    // virtual destructor
    WitnessMerge::~WitnessMerge() {}

    //
    // Override Witness::accept
    //
    void WitnessMerge::accept( Visitor& v, bool visitOnce )
    {
      if( !marked() || !visitOnce) {
        mark();
        if( v.visitMerge(this) ) {
          if (hasCaller())
            caller()->accept(v,visitOnce);
          if (hasRule())
            rule()->accept(v,visitOnce);
          if( hasCallee() )
            callee()->accept(v,visitOnce);

          v.postvisitMerge(this);
        }
      }
    }

    // Override Witness::prettyPrint
    std::ostream& WitnessMerge::prettyPrint( std::ostream& o, size_t depth ) const
    {
      formatDepth(o,depth);
      o << "WitnessMerge < ";
      witness_merge_fn->print(o) << " > : ";
      user_se->print(o) << std::endl;
      // Dumps out to much information.
      // TODO. Make a compile or runtime flag.
#if 1
      if( hasCaller() )
      {
        fCaller->prettyPrint(o,depth+1);
        if (hasRule())
          fRule->prettyPrint(o,depth+1);
        if( hasCallee() )
          fCallee->prettyPrint(o,depth+1);
      }
      else {
        // TODO : make debug
        assert( !hasCallee() );
      }
#else
      int cnt = 0;
      if (hasCaller()) 
        cnt++;
      if (hasRule()) 
        cnt++;
      if (hasCallee()) 
        cnt++;
      o << "\tHas " << cnt << " children.\n";
#endif
      return o;
    }

    void WitnessMerge::reset_marks() const
    {
      if( marked() ) {
        unmark();
        if (hasCaller()) fCaller->reset_marks();
        if (hasRule())   fRule->reset_marks();
        if (hasCallee()) fCallee->reset_marks();
      }
    }

  } // namespace witness

} // namespace wali

