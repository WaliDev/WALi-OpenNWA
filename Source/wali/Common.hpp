#ifndef wali_COMMON_GUARD
#define wali_COMMON_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include <climits>
#include <string>
#include <iostream>
#include "wali/Key.hpp"

namespace wali
{
  typedef Key wali_key_t;
  static const Key WALI_EPSILON = 0;
  static const Key WALI_WILD = 1;
  static const Key WALI_BAD_KEY = UINT_MAX;

  // std::cerr is the default err stream
  extern std::ostream* waliErr;

  //! Sets error stream waliErr to param newErr
  //! @param newErr std::ostream*
  //! @return old err ostream
  extern std::ostream* set_wali_err( std::ostream* newErr );

  //! @return old err ostream
  extern std::ostream* setWaliErr( std::ostream* newErr );

  /*!
   * When running pre or post* query in FWPDS,
   * setting this to true will also run the same
   * query using regular EWPDS and verify the results
   * are the same.
   *
   * @param enable turn FWPDS verification on/off
   */
  extern void set_verify_fwpds( bool enable );

  //! @return whether FWPDS reachability is to be verified
  extern bool get_verify_fwpds();

  /*!
   * @brief Turn lazy weights on or off for FWPDS
   * @return void
   */
  extern void set_lazy_fwpds( bool enable );

  /*! 
   * @return true if FWPDS outputs a WFA whose transitions have lazy
   * weights.returns a lazy automaton.
   */
  extern bool is_lazy_fwpds();

  /*!
   * @brief Enable or disable strictness on transition "to" states.
   * @return void
   */
  extern void set_strict( bool enable );

  /*!
   * @return true if WALi is being strict about transitions being
   * to a PDS state.
   */
  extern bool is_strict();

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
#       pragma warning(disable: 4250)
#   endif
#   if !defined(_CPPRTTI)
#       error RTTI is required by WALi.
#   endif
#endif // defined(__GNUC__)

#endif  // wali_COMMON_GUARD

