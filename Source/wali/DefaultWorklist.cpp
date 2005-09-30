/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Markable.hpp"
#include "wali/DefaultWorklist.hpp"

namespace wali
{
    namespace wpds
    {
        DefaultWorklist::DefaultWorklist() : Worklist() {}

        DefaultWorklist::~DefaultWorklist() {
            // clear all items that may still be in
            // this worklist
            clear();
        }

        void DefaultWorklist::put( Markable * item )
        {
            if( !item->marked() ) {
                item->mark();
                wl.push_back( item );
            }
        }

        Markable * DefaultWorklist::get()
        {
            Markable * item = 0;
            if( !empty() ) {
                item = wl.back();
                wl.pop_back();
                item->unmark();
            }
            return item;
        }

        bool DefaultWorklist::empty() const
        {
            return wl.empty();
        }

        void DefaultWorklist::clear()
        {
            std::list< Markable * >::iterator it = wl.begin();

            // iterate through turning off inworklist flag
            for( ; it != wl.end() ; it++ ) {
                Markable * item = *it;
                item->unmark();
            }
            // clear the list
            wl.clear();
        }

    } // namespace wpds

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
