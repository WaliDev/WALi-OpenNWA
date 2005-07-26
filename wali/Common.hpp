#ifndef wali_COMMON_GUARD
#define wali_COMMON_GUARD 1

/*
 * @author Nick Kidd
 */

#include <climits>

namespace wali
{
    typedef unsigned int wali_key_t;
    static const wali_key_t WALI_EPSILON = 0;
    static const wali_key_t WALI_BAD_KEY = UINT_MAX;

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
#endif

#endif  // wali_COMMON_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
