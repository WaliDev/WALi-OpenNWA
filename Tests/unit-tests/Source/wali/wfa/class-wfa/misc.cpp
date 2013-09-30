#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"

#include "fixtures.hpp"
#include "fixtures/Keys.hpp"
#include "fixtures/SimpleWeights.hpp"

using namespace testing;

namespace wali {
    namespace wfa {

        struct NonInheritingTransCounter
        {
            int n;
            void operator() (ITrans * UNUSED_PARAMETER(t)) {
                ++n;
            }
            NonInheritingTransCounter()
                : n(0)
            {}
        };

#ifdef WALI_WFA_FUNCTOR_FOR_EACH
        TEST(wali$wfa$WFA$$for_each, TransCounterThatDoesNotInheritFromTransFunctor)
        {
            LoopReject f;
            NonInheritingTransCounter counter;
            f.wfa.for_each(counter);
            EXPECT_EQ(3, counter.n);
        }
#endif
            
        TEST(wali$wfa$WFA$$alphabet, battery)
        {
            Letters letters;

            std::set<Key>
                abc,
                ab,
                a,
                none;

            abc.insert(letters.a);
            abc.insert(letters.b);
            abc.insert(letters.c);
            ab.insert(letters.a);
            ab.insert(letters.b);
            a.insert(letters.a);

            EXPECT_EQ(none, EpsilonTransitionToAccepting().wfa.alphabet());
            EXPECT_EQ(none, EpsilonSemiDeterministic().wfa.alphabet());

            EXPECT_EQ(a,    EpsilonTransitionToMiddleToAccepting().wfa.alphabet());
            EXPECT_EQ(a,    ASemiDeterministic().wfa.alphabet());
            EXPECT_EQ(a,    ADeterministic().wfa.alphabet());
            EXPECT_EQ(a,    EpsilonTransitionToMiddleToEpsilonToAccepting().wfa.alphabet());
            EXPECT_EQ(a,    AEpsilonEpsilonEpsilonA().wfa.alphabet());

            EXPECT_EQ(ab,   EvenAsEvenBs().wfa.alphabet());

            EXPECT_EQ(abc,  EpsilonFull().wfa.alphabet());
            EXPECT_EQ(abc,  LoopReject().wfa.alphabet());
            EXPECT_EQ(abc,  LoopAccept().wfa.alphabet());
            EXPECT_EQ(abc,  AFull().wfa.alphabet());
            EXPECT_EQ(abc,  AcceptAbOrAcNondet().wfa.alphabet());
            EXPECT_EQ(abc,  AcceptAbOrAcSemiDeterministic().wfa.alphabet());
            EXPECT_EQ(abc,  AcceptAbOrAcDeterministic().wfa.alphabet());
        }

        TEST(wali$wfa$WFA$$eraseState, eraseStateRemovesIncomingTransitions)
        {
            Keys keys;
            using namespace testing::ReachWeights;

            WFA wfa, expected;

            // WFA:
            //              a
            // --> (st1) -------> (st2)
            //
            //
            // Expected:
            //
            // --> (st1)

            wfa.addState(keys.st1, reachZero);
            wfa.addState(keys.st2, reachZero);
            wfa.setInitialState(keys.st1);
            wfa.addTrans(keys.st1, keys.a, keys.st2, reachOne);

            expected.addState(keys.st1, reachZero);
            expected.setInitialState(keys.st1);

            ASSERT_FALSE(wfa.equal(expected));

            // Remove st2, test again
            wfa.eraseState(keys.st2);
            
            EXPECT_TRUE(wfa.equal(expected));
        }

        TEST(wali$wfa$WFA$$eraseState, eraseStateRemovesOutgoingTransitions)
        {
            Keys keys;
            using namespace testing::ReachWeights;

            WFA wfa, expected;

            // WFA:
            //                          b
            // --> (st1)      (st2) ---------> (st3)
            //
            //
            // Expected:
            //
            // --> (st1)                       (st3)

            wfa.addState(keys.st1, reachZero);
            wfa.addState(keys.st2, reachZero);
            wfa.addState(keys.st3, reachZero);
            wfa.setInitialState(keys.st1);
            wfa.addTrans(keys.st2, keys.a, keys.st3, reachOne);

            expected.addState(keys.st1, reachZero);
            expected.addState(keys.st3, reachZero);
            expected.setInitialState(keys.st1);

            ASSERT_FALSE(wfa.equal(expected));

            // Remove st2, test again
            wfa.eraseState(keys.st2);
            
            EXPECT_TRUE(wfa.equal(expected));
        }
    }
}
