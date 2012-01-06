/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/TotalOrderWorklist.hpp"
#include "wali/wfa/Trans.hpp"

namespace wali
{
  TotalOrderWorklist::TotalOrderWorklist() : PriorityWorklist()
  {
  }

  TotalOrderWorklist::~TotalOrderWorklist()
  {
  }

  int TotalOrderWorklist::compareTo(
      const wfa::ITrans* a
      , const wfa::ITrans* b) const
  {
    sem_elem_t tmp = a->weight()->combine( b->weight() );
    if( tmp->equal( a->weight() ) )
      return -1;
    else
      return 1;
  }

}

