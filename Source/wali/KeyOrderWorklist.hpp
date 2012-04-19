#ifndef wali_KEY_ORDER_WORKLIST_GUARD
#define wali_KEY_ORDER_WORKLIST_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/PriorityWorklist.hpp"

namespace wali
{
  struct KeyComparer : std::binary_function<const wfa::ITrans*, const wfa::ITrans*, bool>
  {
    bool operator() (const wfa::ITrans* a, const wfa::ITrans* b) const
    {
      // < 0 if a < b
      Triple<Key, Key, Key>
        a_triple(a->from(), a->to(), a->stack()),
        b_triple(b->from(), b->to(), b->stack());

      return a_triple < b_triple;
    }
  };

  class KeyOrderWorklist : public PriorityWorklist<KeyComparer>
  {
    public:
      KeyOrderWorklist();
      virtual ~KeyOrderWorklist();
  }; // class PriorityWorklist

} // namespace wali

#endif  // wali_KEY_ORDER_WORKLIST_GUARD

