#ifndef wali_COMMON_GUARD
#define wali_COMMON_GUARD 1

/*
 * @author Nick Kidd
 * @version $Revision: 1.21 $
 */

#include <climits>

namespace wali
{
    typedef unsigned int wali_key_t;
    static const wali_key_t WALI_EPSILON = 0;
    static const wali_key_t WALI_BAD_KEY = UINT_MAX;

} // namespace wali

/*!
 * @macro UNUSED_PARAM
 *
 * This macro disables GCC warnings for unused parameters. #ifdef
 * is used b/c MS VC++ does not like __attribute__.
 */
#if defined(__GNUG__)
#   if( __GNUG__ > 3 )
#       define UNUSED_PARAM __attribute__((__unused__))
#   else
#       define UNUSED_PARAM
#   endif
#elif defined(_WIN32)
#   pragma once
#   define UNUSED_PARAM
#   if _MSC_VER > 1000
#       pragma warning(disable: 4786)
#   endif
#   if !defined(_CPPRTTI)
#       error RTTI is required by WALi
#   endif
#endif

#endif  // wali_COMMON_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

