/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Markable.hpp"

namespace wali
{
    Markable::Markable() : marker(false) {}

    /*!
     * Anytime a Markable is created it is in the unmarked state.
     * This makes the copy constructor do nothing.
     */
    Markable::Markable( const Markable& m ATTR_UNUSED ) : marker(false) {}

    /*!
     * The Markable object is already created and may or may not
     * be marked. operator= does not perform any action b/c the
     * interface of Markable specifies that state may only
     * be changed through mark and unmark.
     */
    Markable& Markable::operator=( const Markable& m ATTR_UNUSED )
    {
        return *this;
    }

    Markable::~Markable() throw() {}

    void Markable::mark() throw()
    {
        marker = true;
    }

    void Markable::unmark() throw()
    {
        marker = false;
    }

    bool Markable::marked() const throw()
    {
        return marker;
    }
}

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
