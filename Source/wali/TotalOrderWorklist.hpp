#ifndef wali_TOTAL_ORDER_WORKLIST_GUARD
#define wali_TOTAL_ORDER_WORKLIST_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/PriorityWorklist.hpp"

namespace wali
{
    class TotalOrderWorklist : public PriorityWorklist
    {
        public:
            TotalOrderWorklist();
            virtual ~TotalOrderWorklist();

            /*!
             * Returns -1 if a's weight is less than b's weight.
             */
            virtual int compareTo( const wfa::Trans* a, const wfa::Trans* b ) const;

    }; // class PriorityWorklist

} // namespace wali
#endif  // wali_TOTAL_ORDER_WORKLIST_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

