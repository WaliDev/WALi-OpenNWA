#ifndef wali_COMMON_GUARD
#define wali_COMMON_GUARD 1

/*
 * @author Nick Kidd
 */

#include <climits>
#include <string>
#include <iostream>
#include "wali/Key.hpp"

namespace wali
{
    typedef Key wali_key_t;
    static const Key WALI_EPSILON = 0;
    static const Key WALI_BAD_KEY = UINT_MAX;

    // std::cerr is the default err stream
    extern std::ostream* waliErr;

    //! @return old err ostream
    extern std::ostream* set_wali_err( std::ostream* newErr );

    //! @return old err ostream
    extern std::ostream* setWaliErr( std::ostream* newErr );

} // namespace wali

/*!
 * @macro ATTR_UNUSED
 *
 * This macro disables GCC warnings for unused parameters. #ifdef
 * is used b/c MS VC++ does not like __attribute__.
 */
#if defined(__GNUC__)
#   define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#   if( GCC_VERSION >= 30400 )
#       define ATTR_UNUSED __attribute__((__unused__))
#   else
#       define ATTR_UNUSED
#   endif
#elif defined(_WIN32)
#   pragma once
#   define ATTR_UNUSED
#   if _MSC_VER > 1000
#       pragma warning(disable: 4786)
#   endif
#   if !defined(_CPPRTTI)
#       error RTTI is required by WALi.
#   endif
#endif // defined(__GNUC__)

#endif  // wali_COMMON_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
