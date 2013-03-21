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


  struct TraceSplitSemElems {
    wali::sem_elem_t empty_default_dist0, empty_default_infty,
      guards012_default_dist0, guards012_default_infty,
      guardsF12_default_dist0;

    TraceSplitSemElems() {
      using namespace testing::ShortestPathWeights;
      using wali::domains::TraceSplitSemElem;
      TraceSplitMaps maps;

#define TSSE(map, default) \
  TraceSplitSemElem(TraceSplitSemElem::identity_reducer, map, default)

      empty_default_dist0 =     new TSSE(maps.empty, dist0);
      empty_default_infty =     new TSSE(maps.empty, semiring_zero);
      guards012_default_dist0 = new TSSE(maps.guards012, dist0);
      guards012_default_infty = new TSSE(maps.guards012, semiring_zero);
      guardsF12_default_dist0 = new TSSE(maps.guardsF12, dist0);

#undef TSSE
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
