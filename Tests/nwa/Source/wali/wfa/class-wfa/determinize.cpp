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

        TEST(wali$wfa$$isIsomorphicTo, selfIsomorphismHolds)
        {
            LoopReject f;

            EXPECT_TRUE(f.wfa.isIsomorphicTo(f.wfa));
        }
        
        TEST(wali$wfa$$isIsomorphicTo, selfIsomorphismOfDifferentAutsFail)
        {
            LoopReject rej;
            LoopAccept acc;

            EXPECT_FALSE(acc.wfa.isIsomorphicTo(rej.wfa));
        }

        TEST(wali$wfa$$isIsomorphicTo, isomorphismChecksStateWeights)
        {
            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();
            
            Letters l;
            WFA w0, w1;

            w0.addState(l.a, zero);
            w1.addState(l.a, one);

            EXPECT_FALSE(w0.isIsomorphicTo(w1));
        }
        
        TEST(wali$wfa$$isIsomorphicTo, isomorphismChecksTransWeights)
        {
            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();
            
            Letters l;
            WFA w0, w1;

            w0.addState(l.a, zero);
            w1.addState(l.a, zero);

            w0.addTrans(l.a, l.a, l.a, zero);
            w1.addTrans(l.a, l.a, l.a, one);

            EXPECT_FALSE(w0.isIsomorphicTo(w1));
        }

        TEST(wali$wfa$$isIsomorphicTo, battery)
        {
            for (size_t i=0; i<num_fas; ++i) {
                for (size_t j=0; j<num_fas; ++j) {
                    std::stringstream ss;
                    ss << "Testing FAs " << i << " ~ " << j;
                    SCOPED_TRACE(ss.str());
                    
                    WFA left = fas[i];
                    WFA right = fas[j];

                    EXPECT_EQ(i == j, left.isIsomorphicTo(right));
                }
            }
        }



    }
}
