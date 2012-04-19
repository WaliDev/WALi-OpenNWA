#ifndef wali_TOTAL_ORDER_WORKLIST_GUARD
#define wali_TOTAL_ORDER_WORKLIST_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/PriorityWorklist.hpp"

namespace wali
{
  struct LessThan : std::binary_function<const wfa::ITrans*, const wfa::ITrans*, bool>
  {
    bool operator()( const wfa::ITrans* a, const wfa::ITrans* b ) const
    {
      // a + b == a means that a >= b (so a < b is false)
      sem_elem_t tmp = a->weight()->combine( b->weight() );
      if( tmp->equal( a->weight() ) )
        return false;
      else
        return true;
    }
  };


  class TotalOrderWorklist : public PriorityWorklist<LessThan>
  {
    public:
      TotalOrderWorklist();
      virtual ~TotalOrderWorklist();
  }; // class PriorityWorklist

} // namespace wali

#endif  // wali_TOTAL_ORDER_WORKLIST_GUARD

