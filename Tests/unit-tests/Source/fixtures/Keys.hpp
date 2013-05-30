#ifndef WALI_TESTING_KEYS_HPP
#define WALI_TESTING_KEYS_HPP

#include "wali/Key.hpp"

namespace testing
{
  struct Keys {
    wali::Key st1, st2, st3, st4, st5;
    wali::Key a, b;

    Keys():
      st1(wali::getKey("state1")),
      st2(wali::getKey("state2")),
      st3(wali::getKey("state3")),
      st4(wali::getKey("state4")),
      st5(wali::getKey("state5")),
      a(wali::getKey("a")),
      b(wali::getKey("b"))
    {}
  };
}

// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:

#endif
