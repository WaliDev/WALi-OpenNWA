#include <gtest/gtest.h>

#include "wali/domains/TraceSplitSemElem.hpp"

#include "fixtures.hpp"

namespace wali {
  namespace domains {

    TEST(testing$TraceSplitMaps, MapsHaveTheCorrectSize) {
      TraceSplitMaps maps;

      EXPECT_EQ(0u, maps.empty.size());
      EXPECT_EQ(3u, maps.guards012.size());
      EXPECT_EQ(3u, maps.guardsF12.size());
    }

    ////////////////////////////////////////////////

    TraceSplitSemElem *
    down(sem_elem_t ptr_se) {
      TraceSplitSemElem * ptr_tsse = dynamic_cast<TraceSplitSemElem*>(ptr_se.get_ptr());
      assert(ptr_tsse);
      return ptr_tsse;
    }


    TEST(wali$domains$TraceSplitSemElem$$getWeight, ReturnsDefaultValueForAbsentValues)
    {
      using namespace ::testing::ShortestPathWeights;
      TraceSplitSemElems weights;
      LiteralGuards guards;

      EXPECT_TRUE(down(weights.empty_default_dist0)->getWeight(guards.zero) -> equal(dist0));
      EXPECT_TRUE(down(weights.empty_default_infty)->getWeight(guards.zero) -> equal(semiring_zero));

      EXPECT_TRUE(down(weights.guards012_default_dist0)->getWeight(guards.three) -> equal(dist0));
      EXPECT_TRUE(down(weights.guards012_default_infty)->getWeight(guards.three) -> equal(semiring_zero));

      EXPECT_TRUE(down(weights.guardsF12_default_dist0)->getWeight(guards.three) -> equal(dist0));
    }


    TEST(wali$domains$TraceSplitSemElem$$getWeight, FalseGuardNotAddedAndReturnsDefault)
    {
      using namespace ::testing::ShortestPathWeights;
      TraceSplitSemElems weights;
      LiteralGuards guards;

      EXPECT_TRUE(down(weights.guardsF12_default_dist0)->getWeight(guards.false_) -> equal(dist0));
    }


    TEST(wali$domains$TraceSplitSemElem$$getWeight, ReturnsCorrectValueForPresentValues)
    {
      using namespace ::testing::ShortestPathWeights;
      TraceSplitSemElems weights;
      LiteralGuards guards;

      EXPECT_TRUE(down(weights.guards012_default_dist0)->getWeight(guards.zero) -> equal(dist0));
      EXPECT_TRUE(down(weights.guards012_default_infty)->getWeight(guards.zero) -> equal(dist0));
      EXPECT_TRUE(down(weights.guards012_default_dist0)->getWeight(guards.one)  -> equal(dist1));
      EXPECT_TRUE(down(weights.guards012_default_infty)->getWeight(guards.one)  -> equal(dist1));
      EXPECT_TRUE(down(weights.guards012_default_dist0)->getWeight(guards.two)  -> equal(dist2));
      EXPECT_TRUE(down(weights.guards012_default_infty)->getWeight(guards.two)  -> equal(dist2));

      EXPECT_TRUE(down(weights.guardsF12_default_dist0)->getWeight(guards.one)  -> equal(dist1));
      EXPECT_TRUE(down(weights.guardsF12_default_dist0)->getWeight(guards.two)  -> equal(dist2));
    }


  }
}

// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
