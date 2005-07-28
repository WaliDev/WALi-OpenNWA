/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include <sstream>

namespace wali
{

    std::string Printable::to_string() const
    {
        std::ostringstream oss;
        print( oss );
        return oss.str();
    }

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
