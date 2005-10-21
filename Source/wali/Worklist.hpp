#ifndef wali_WORKLIST_GUARD
#define wali_WORKLIST_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"

namespace wali
{

    namespace wfa {
        class Trans;
    }

    /*! @class Worklist
     *
     * Worklist defines the interface of a worklist. All items must subclass
     * wali::wfa::Trans. When an item is placed on a worklist, the worklist
     * treats the item's "marked" state as only being modified by the
     * worklist (i.e., do not place a Trans item on two worklists at the
     * same time).
     */

    class Worklist
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
             */
            virtual void put( wfa::Trans *item ) = 0;

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
            virtual wfa::Trans * get() = 0;

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

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
