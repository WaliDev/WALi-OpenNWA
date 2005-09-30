#ifndef wali_WORKLIST_GUARD
#define wali_WORKLIST_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Worklist.hpp"
#include <list>

namespace wali
{

    class Markable;

    /*! @class DefaultWorklist
     *
     * The default DefaultWorklist acts as a Stack and uses
     * std::list to hold items. All items must subclass
     * wali::Trans. When an item is placed on a worklist,
     * the worklist treats the item's "Trans" state as
     * only being modified by the worklist (i.e., do not place
     * a Trans item on two worklists at the same time).
     */

    class DefaultWorklist : public ::wali::Worklist
    {
        public:
            using ::wali::wfa::Trans;

        public:

            DefaultWorklist() {}

            virtual ~DefaultWorklist() {}

            /*!
             * put
             *
             * Put a wlmix * in the DefaultWorklist.
             * This method should be idempotent but
             * it really does not matter.
             */
            virtual void put( Markable *item );

            /*!
             * get
             *
             * Return an item from the worklist.
             * Guaranteed only to be called if 
             * DefaultWorklist is not empty.
             *
             * @return Markable *
             */
            virtual Markable * get();

            /*!
             * emtpy
             *
             * @return true if the DefaultWorklist is empty
             */
            virtual bool empty() const;

            /*!
             * clear
             *
             * Remove and unmark each item in this worklist.
             */
            virtual void clear();

        protected:
            std::list< Markable * > wl; //!< The default worklist data structure

    }; // class DefaultWorklist

} // namespace wali

#endif // wali_WORKLIST_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
