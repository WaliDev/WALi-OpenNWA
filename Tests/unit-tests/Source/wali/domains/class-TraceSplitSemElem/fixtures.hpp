#ifndef TEST_WALI_DOMAINS_TRACESPLITSEMELEM_FIXTURES_HPP
#define TEST_WALI_DOMAINS_TRACESPLITSEMELEM_FIXTURES_HPP

#include "fixtures/SimpleWeights.hpp"
#include "wali/domains/TraceSplitSemElem.hpp"

namespace {
  typedef wali::domains::LiteralGuard<int> IntGuard;
    
  struct LiteralGuards {
    IntGuard::Ptr false_, zero, one, two, three;

    LiteralGuards()
      : false_(new IntGuard())
      , zero(new IntGuard(0))
      , one(new IntGuard(1))
      , two(new IntGuard(2))
      , three(new IntGuard(3))
    {}
  };


  struct TraceSplitMaps {
    LiteralGuards guards;
    
    wali::domains::TraceSplitSemElem::BackingMap empty, guards012, guardsF12;

    TraceSplitMaps() {
      using namespace testing::ShortestPathWeights;

      guards012[guards.zero] = dist0;
      guards012[guards.one] = dist1;
      guards012[guards.two] = dist2;

      guardsF12[guards.false_] = semiring_zero;
      guardsF12[guards.one] = dist1;
      guardsF12[guards.two] = dist2;
    }
  };
}


// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:

#endif
