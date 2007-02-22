/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"

namespace wali
{
    // std::cerr is the default err stream
    std::ostream* waliErr = &std::cerr;

    //! @return old err ostream
    std::ostream* set_wali_err( std::ostream* newErr )
    {
        std::ostream* tmp = waliErr;
        waliErr = newErr;
        return tmp;
    }

    //! @return old err ostream
    std::ostream* setWaliErr( std::ostream* newErr )
    {
        std::ostream* tmp = waliErr;
        waliErr = newErr;
        return tmp;
    }

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
