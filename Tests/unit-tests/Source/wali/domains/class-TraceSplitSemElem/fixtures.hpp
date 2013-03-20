#ifndef TEST_WALI_DOMAINS_TRACESPLITSEMELEM_FIXTURES_HPP
#define TEST_WALI_DOMAINS_TRACESPLITSEMELEM_FIXTURES_HPP

#include "wali/domains/TraceSplitSemElem.hpp"

namespace {
  typedef wali::domains::LiteralGuard<int> IntGuard;
    
  struct LiteralGuards {
    IntGuard::Ptr false_, zero, one;

    LiteralGuards()
      : false_(new IntGuard())
      , zero(new IntGuard(0))
      , one(new IntGuard(1))
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
