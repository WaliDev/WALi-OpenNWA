/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/DefaultWorklist.hpp"
#include "wali/wfa/Trans.hpp"

namespace wali
{
    DefaultWorklist::DefaultWorklist() : Worklist() {}

    DefaultWorklist::~DefaultWorklist() {
        // clear all items that may still be in
        // this worklist
        clear();
    }

    bool DefaultWorklist::put( wfa::Trans * item )
    {
        if( !item->marked() ) {
            item->mark();
            wl.push_back( item );
            return true;
        }
        else
            return false;
    }

    wfa::Trans * DefaultWorklist::get()
    {
        wfa::Trans * item = 0;
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
        std::list< wfa::Trans * >::iterator it = wl.begin();

        // iterate through turning off inworklist flag
        for( ; it != wl.end() ; it++ ) {
            wfa::Trans * item = *it;
            item->unmark();
        }
        // clear the list
        wl.clear();
    }

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
