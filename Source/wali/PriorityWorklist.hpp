#ifndef wali_PRIORITY_WORKLIST_GUARD
#define wali_PRIORITY_WORKLIST_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Worklist.hpp"
#include "wali/wfa/Trans.hpp"
#include <map>
#include <set>

namespace wali
{

    /*!
     * @ class StackSymLT
     *
     * Implements comparator to use a set for sorting transitions
     * according in a priority queue.
     */
    struct StackSymLT {
        bool operator()( const wfa::Trans* a, const wfa::Trans* b) const;
    };

    class PriorityWorklist : public Worklist<wfa::Trans>
    {
        public:
            typedef std::multiset< wfa::Trans*, StackSymLT > pwl_t;

        public:
            PriorityWorklist();
            virtual ~PriorityWorklist();

            virtual bool put( wfa::Trans *t );

            virtual wfa::Trans * get();

            virtual bool empty() const;

            virtual void clear();

        protected:
            pwl_t workset;

    }; // class PriorityWorklist

} // namespace wali

#endif  // wali_PRIORITY_WORKLIST_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

