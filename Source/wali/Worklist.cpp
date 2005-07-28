/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Worklist.hpp"
#include "wali/Markable.hpp"

namespace wali
{

    Worklist::Worklist() {}

    Worklist::~Worklist() {}

    void Worklist::put( Markable * item )
    {
        if( !item->marked() ) {
            item->mark();
            wl.push_back( item );
        }
    }

    Markable * Worklist::get()
    {
        Markable * item = wl.back();
        wl.pop_back();
        item->unmark();
        return item;
    }

    bool Worklist::empty() const
    {
        return wl.empty();
    }

    void Worklist::clear()
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

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
