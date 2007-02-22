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

    //! When running pre or post* query in FWPDS,
    //! setting this to true will also run the same
    //! query using regular WPDS and verify the results
    //! are the same.
    //!
    //! Default value is true
    bool b_check_fwpds = true;

    void set_verify_wpds( bool enable ) {
        b_check_fwpds = enable;
    }

    bool get_verify_fwpds() {
        return b_check_fwpds;
    }

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
