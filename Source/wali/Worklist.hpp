#ifndef wali_WORKLIST_GUARD
#define wali_WORKLIST_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Countable.hpp"

namespace wali
{

  /*! @class Worklist
   *
   * Worklist defines the interface of a worklist. All items must subclass
   * wali::Markable. When an item is placed on a worklist, the worklist
   * treats the item's "marked" state as only being modified by the worklist
   * (i.e., do not place a Markable item on two worklists at the same time).
   */

  template< typename T > class Worklist : public Countable
  {

    public:

      Worklist() {}

      virtual ~Worklist() {}

      /*!
       * put
       *
       * Put a Trans * in the Worklist.
       * This method should be idempotent but
       * it really does not matter.
       *
       * @return true if the item was added, false if not
       * A false return value means the item was already "marked"
       * and hence, is already on this worklist.
       */
      virtual bool put( T *item ) = 0;

      /*!
       * get
       *
       * Return an item from the worklist.
       * Guaranteed only to be called if 
       * Worklist is not empty. May throw an exception or return 
       * NULL if the worklist is not empty.
       *
       * @return Trans *
       */
      virtual T * get() = 0;

      /*!
       * emtpy
       *
       * @return true if the Worklist is empty
       */
      virtual bool empty() const = 0;

      /*!
       * clear
       *
       * Remove and unmark each item in this worklist.
       */
      virtual void clear() = 0;

    protected:

  }; // class Worklist

} // namespace wali

#endif // wali_WORKLIST_GUARD

