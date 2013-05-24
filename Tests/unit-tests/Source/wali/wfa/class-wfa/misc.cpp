#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"

#include "fixtures.hpp"

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

        TEST(wali$wfa$WFA$$for_each, TransCounterThatDoesNotInheritFromTransFunctor)
        {
            LoopReject f;
            NonInheritingTransCounter counter;
            f.wfa.for_each(counter);
            EXPECT_EQ(3, counter.n);
        }
            
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


    }
}
