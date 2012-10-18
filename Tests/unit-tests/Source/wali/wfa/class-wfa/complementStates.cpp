#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/TransFunctor.hpp"

#include "fixtures.hpp"


namespace wali {
    namespace wfa {

        TEST(wali$wfa$$complementStates, AcceptAbOrAcDeterministic$invertsAccepting)
        {
            AcceptAbOrAcDeterministic w;
            Key start = getKey("start");
            Key a = getKey("a (state)");
            Key ab = getKey("ab");           // originally accepting
            Key ac = getKey("ac");           // originally accepting
            Key reject = getKey("reject");

            w.wfa.complementStates();

            EXPECT_TRUE( w.wfa.isFinalState(start));
            EXPECT_TRUE( w.wfa.isFinalState(a));
            EXPECT_TRUE(!w.wfa.isFinalState(ab));
            EXPECT_TRUE(!w.wfa.isFinalState(ac));
            EXPECT_TRUE( w.wfa.isFinalState(reject));
        }

        TEST(wali$wfa$$complementStates, AcceptAbOrAcDeterministic$leavesRest)
        {
            AcceptAbOrAcDeterministic w, orig;
            w.wfa.complementStates();

            // Sanity check
            Key start = getKey("start"); // originally rejecting
            ASSERT_TRUE( w.wfa.isFinalState(start));

            EXPECT_EQ(w.wfa.getStates(), orig.wfa.getStates());

            // We check just that the number of transitions is the same,
            // because I don't really want to do something more involved.
            TransCounter c1, c2;
            w.wfa.for_each(c1);
            w.wfa.for_each(c2);
            EXPECT_EQ(c1.getNumTrans(), c2.getNumTrans());
        }
    }
}

