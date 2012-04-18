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

static const WFA fas_already_deterministic[] = {
    LoopReject().wfa,
    LoopAccept().wfa,
    EvenAsEvenBs().wfa,
    EpsilonDeterministic().wfa,
    ADeterministic().wfa,
    AcceptAbOrAcDeterministic().wfa
};

static const unsigned num_fas_already_deterministic = NUM_ELEMENTS(fas_already_deterministic);


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

        TEST(wali$wfa$$isIsomorphicTo, isomorphismIgnoresStateWeightsWhenCheckWeghtsIsFalse)
        {
            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();
            
            Letters l;
            WFA w0, w1;

            w0.addState(l.a, zero);
            w1.addState(l.a, one);

            EXPECT_TRUE(w0.isIsomorphicTo(w1, false));
        }
        
        TEST(wali$wfa$$isIsomorphicTo, isomorphismIgnoresTransWeightsWhenCheckWeghtsIsFalse)
        {
            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();
            
            Letters l;
            WFA w0, w1;

            w0.addState(l.a, zero);
            w1.addState(l.a, zero);

            w0.addTrans(l.a, l.a, l.a, zero);
            w1.addTrans(l.a, l.a, l.a, one);

            EXPECT_TRUE(w0.isIsomorphicTo(w1, false));
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
                    EXPECT_EQ(i == j, left.isIsomorphicTo(right, true));
                    EXPECT_EQ(i == j, left.isIsomorphicTo(right, false));
                }
            }
        }

        TEST(wali$wfa$$isIsomorphicTo, loopRejectAndSimilar)
        {
            LoopReject f;
            WFA wfa;

            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();
                
            Letters l;
            Key state = getKey("different state");
            ASSERT_NE(state, f.state);

            wfa.addState(state, zero);
            wfa.setInitialState(state);
            wfa.addTrans(state, l.a, state, one);
            wfa.addTrans(state, l.b, state, one);
            wfa.addTrans(state, l.c, state, one);
            
            EXPECT_TRUE(f.wfa.isIsomorphicTo(wfa));
        }
        


        TEST(wali$wfa$$determinize, alreadyDeterministicBattery) {
            for (size_t i=0; i<num_fas_already_deterministic; ++i) {
                std::stringstream ss;
                ss << "Testing FA " << i;
                SCOPED_TRACE(ss.str());

                WFA orig = fas_already_deterministic[i];
                WFA det = orig.determinize();

                EXPECT_TRUE(orig.isIsomorphicTo(det, false));
            }
        }


        TEST(wali$wfa$$determinize, DISABLED_completeEpsilonTransitionToAccepting) {
            EpsilonTransitionToAccepting f;
            EpsilonDeterministic expected;
            WFA wfa = f.wfa.determinize();

            EXPECT_TRUE(wfa.isIsomorphicTo(expected.wfa));
        }
        
        TEST(wali$wfa$$determinize, incompleteEpsilonTransitionToAccepting) {
            EpsilonTransitionToAccepting f;
            WFA wfa = f.wfa.determinize();

            EpsilonSemiDeterministic expected;

            EXPECT_TRUE(wfa.isIsomorphicTo(expected.wfa));
        }


        TEST(wali$wfa$$complete, epsilonSemi)
        {
            EpsilonSemiDeterministic orig;
            EpsilonDeterministic expected;

            Letters l;
            std::set<Key> symbols;
            symbols.insert(l.a);
            symbols.insert(l.b);
            symbols.insert(l.c);

            orig.wfa.complete(symbols);

            EXPECT_TRUE(expected.wfa.isIsomorphicTo(orig.wfa));
        }
    }
}
