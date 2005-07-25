#ifndef wali_WORKLIST_GUARD
#define wali_WORKLIST_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include <list>

namespace wali
{

    class Markable;

    /*! @class Worklist
     *
     * The default Worklist acts as a Stack and uses
     * std::list to hold items. All items must subclass
     * wali::Markable. When an item is placed on a worklist,
     * the worklist treats the item's "Markable" state as
     * only being modified by the worklist (i.e., do not place
     * a Markable item on two worklists at the same time).
     */

    class Worklist
    {

        public:

            Worklist();

            virtual ~Worklist();

            /*!
             * put
             *
             * Put a wlmix * in the Worklist.
             * This method should be idempotent but
             * it really does not matter.
             */
            virtual void put( Markable *item );

            /*!
             * get
             *
             * Return an item from the worklist.
             * Guaranteed only to be called if 
             * Worklist is not empty.
             *
             * @return Markable *
             */
            virtual Markable * get();

            /*!
             * emtpy
             *
             * @return true if the Worklist is empty
             */
            virtual bool empty() const;

            /*!
             * clear
             *
             * Remove and unmark each item in this worklist.
             */
            virtual void clear();

        protected:
            std::list< Markable * > wl;

    }; // class Worklist

} // namespace wali

#endif // wali_WORKLIST_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
 */
