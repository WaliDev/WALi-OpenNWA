#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"
#include "wali/ShortestPathSemiring.hpp"

#include "fixtures.hpp"

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

using namespace wali::wfa;

static const WFA fas[] = {
    LoopReject().wfa,
    LoopAccept().wfa,
    EvenAsEvenBs().wfa,
    EpsilonTransitionToAccepting().wfa,
    EpsilonDeterministic().wfa,
    EpsilonTransitionToMiddleToAccepting().wfa,
    ADeterministic().wfa,
    EpsilonTransitionToMiddleToEpsilonToAccepting().wfa,
    AcceptAbOrAcNondet().wfa,
    AEpsilonEpsilonEpsilonA().wfa
};

static const unsigned num_fas = NUM_ELEMENTS(fas);


namespace wali {
    namespace wfa {

        TEST(wali$wfa$$isIsomorphicTo, DISABLED_selfIsomorphismHolds)
        {
            LoopReject f;

            EXPECT_TRUE(f.wfa.isIsomorphicTo(f.wfa));
        }
        
    }
}
