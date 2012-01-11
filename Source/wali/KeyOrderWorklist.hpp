#ifndef wali_KEY_ORDER_WORKLIST_GUARD
#define wali_KEY_ORDER_WORKLIST_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/PriorityWorklist.hpp"

namespace wali
{
  class KeyOrderWorklist : public PriorityWorklist
  {
    public:
      KeyOrderWorklist();
      virtual ~KeyOrderWorklist();

      /*!
       * Returns -1 if a's weight is less than b's weight.
       */
      virtual int compareTo( const wfa::ITrans* a, const wfa::ITrans* b ) const;

  }; // class PriorityWorklist

} // namespace wali

#endif  // wali_KEY_ORDER_WORKLIST_GUARD

