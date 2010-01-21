/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/Visitor.hpp"

namespace wali
{
  namespace witness
  {
    // weight == came from where?
    WitnessExtend::WitnessExtend( sem_elem_t weight ) :
      Witness(weight),lchild(0),rchild(0) {}

    // weight == left
    WitnessExtend::WitnessExtend( sem_elem_t weight, witness_t left ) :
      Witness(weight),lchild(left),rchild(0) {}

    // weight == left->extend( right )
    WitnessExtend::WitnessExtend( sem_elem_t weight, witness_t left, witness_t right ) :
      Witness(weight),lchild(left),rchild(right) {}

    // virtual destructor
    WitnessExtend::~WitnessExtend() {}

    //
    // Override Witness::accept
    //
    void WitnessExtend::accept( Visitor& v, bool visitOnce )
    {
      if( !marked() || !visitOnce) {
        mark();
        if( v.visitExtend(this) ) {
          if( hasLeft() )
            left()->accept(v,visitOnce);
          if( hasRight() )
            right()->accept(v,visitOnce);
        }
      }
    }

    // Override Witness::prettyPrint
    std::ostream& WitnessExtend::prettyPrint( std::ostream& o, size_t depth ) const
    {
      formatDepth(o,depth);
      o << "WitnessExtend: ";
      user_se->print(o) << std::endl;
      // Dumps out to much information.
      // TODO. Make a compile or runtime flag.
      if( 1 ) {
        if( hasLeft() )
        {
          lchild->prettyPrint(o,depth+1);
          if( hasRight() )
            rchild->prettyPrint(o,depth+1);
        }
        else {
          // TODO : make debug
          assert( !hasRight() );
        }
      }
      else {
        int cnt = 0;
        if( hasLeft() ) cnt++;
        if( hasRight() ) cnt++;
        o << "\tHas " << cnt << " children.\n";
      }
      return o;
    }

    void WitnessExtend::reset_marks() const
    {
      if( marked() ) {
        unmark();
        if( hasLeft() ) lchild->reset_marks();
        if( hasRight() ) rchild->reset_marks();
      }
    }

  } // namespace witness

} // namespace wali

