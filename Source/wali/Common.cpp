/**
 * @author Nicholas Kidd
 *
 * @version $Id$
 */

#include "wali/Common.hpp"

#include <iostream>
#include <cstdlib>

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
  //! Default value is false
  static bool b_check_fwpds = false;

  /*!
   * @brief Turn FWPDS verification on and off.
   * @return void
   */
  void set_verify_fwpds( bool enable ) {
    b_check_fwpds = enable;
  }

  /*! 
   * @return true if FWPDS will verify the result
   * of each poststar or prestar query against EWPDS.
   */
  bool get_verify_fwpds() {
    return b_check_fwpds;
  }

  //! @brief the static variable moderating FWPDS laziness
  static bool b_lazy_fwpds = true;

  /*!
   * @brief Turn lazy weights on or off for FWPDS
   * @return void
   */
  void set_lazy_fwpds( bool enable ) {
    b_lazy_fwpds = enable;
  }

  /*! 
   * @return true if FWPDS outputs a WFA whose transitions have lazy
   * weights.returns a lazy automaton.
   */
  bool is_lazy_fwpds() {
    return b_lazy_fwpds;
  }

  static bool b_strict = true;

  /*!
   * @brief Enable or disable strictness on transition "to" states.
   * @return void
   */
  void set_strict( bool enable ) {
    b_strict = enable;
  }

  /*!
   * @return true if WALi is being strict about transitions being
   * to a PDS state.
   */
  bool is_strict() {
    return b_strict;
  }


  void assert_fail(const char* assertion,
		   const char* file,
		   unsigned int line,
		   const char* function)
  {
#ifndef NDEBUG
# ifdef _MSC_VER
    using std::mbstowcs;
    std::cerr << "In function " << function <<":\n";
    size_t sz_assertion = mbstowcs(NULL, assertion, 0);
    size_t sz_file = mbstowcs(NULL, file, 0);
    if (sz_assertion == 0 || sz_file == 0) {
        std::cerr << "  file: " << file << "\n"
                  << "  line: " << line << "\n"
                  << "  assertion failure!\n"
                  << "\n"
                  << "  There was also an error converting one of the above to a wchar_t*";
        abort();
    } 
    wchar_t* wassertion = new wchar_t[sz_assertion + 1];
    wchar_t* wfile = new wchar_t[sz_file + 1];
    mbstowcs(wassertion, assertion, sz_assertion);
    mbstowcs(wfile, file, sz_file);
    _wassert(wassertion, wfile, line);
# else
    std::cerr << "In function " << function <<":\n"
              << "  file: " << file << "\n"
              << "  line: " << line << "\n"
              << "  assertion failure:\n"
              << "    " << assertion << "\n"
              ;
    abort();
# endif
#endif
  }
  

} // namespace wali

