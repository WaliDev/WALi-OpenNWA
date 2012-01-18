/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/KeyOrderWorklist.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/KeyContainer.hpp"

namespace wali
{
  KeyOrderWorklist::KeyOrderWorklist() : PriorityWorklist()
  {
  }

  KeyOrderWorklist::~KeyOrderWorklist()
  {
  }

  int KeyOrderWorklist::compareTo(
      const wfa::ITrans* a
      , const wfa::ITrans* b) const
  {
    // < 0 if a < b
    Triple<Key, Key, Key>
      a_triple(a->from(), a->to(), a->stack()),
      b_triple(b->from(), b->to(), b->stack());

    if (a_triple < b_triple) {
      return 1;
    }
    else if (a_triple == b_triple) {
      return 0;
    }
    else {
      return -1;
    }
  }

}

