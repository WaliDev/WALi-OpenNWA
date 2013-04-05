#include "gtest/gtest.h"
#include "wali/wpds/WPDS.hpp"
#include "fixtures/SimpleWeights.hpp"
#include "opennwa/fixtures.hpp"

using namespace wali;
using namespace wali::wpds;
using namespace wali::wfa;

namespace {

    TEST(wali$wpds$WPDS$toWfa, emptyWpdsGivesEmptyWfa)
    {
        WPDS empty;
        WFA wfa, expected;
        empty.toWfa(wfa);
        EXPECT_EQ(0u, wfa.numStates());
        EXPECT_TRUE(wfa.isIsomorphicTo(expected));
    }

    TEST(wali$wpds$WPDS$toWfa, simpleOneStatePds)
    {
        using namespace testing::ShortestPathWeights;
        opennwa::SomeElements es;

        // state ------> state2 ------> state3 <---+
        //  /\                            |  |     |
        //   +----------------------------+  +-----+

        WPDS wpds;
        wpds.add_rule(es.symbol, es.state,  es.symbol, es.state2, dist1);
        wpds.add_rule(es.symbol, es.state2, es.symbol, es.state3, dist2);
        wpds.add_rule(es.symbol, es.state3, es.symbol, es.state3, dist3);
        wpds.add_rule(es.symbol, es.state3, es.symbol, es.state,  dist10);

        WFA expected;
        expected.addState(es.state, semiring_zero);
        expected.addState(es.state2, semiring_zero);
        expected.addState(es.state3, semiring_zero);
        expected.addTrans(es.state,  es.state,  es.state2, dist1);
        expected.addTrans(es.state2, es.state2, es.state3, dist2);
        expected.addTrans(es.state3, es.state3, es.state3, dist3);
        expected.addTrans(es.state3, es.state3, es.state,  dist10);

        WFA actual;
        wpds.toWfa(actual);

        // Actually *weaker* than I want: they really ought ot be identical
        EXPECT_TRUE(expected.isIsomorphicTo(actual));
    }


    TEST(wali$wpds$WPDS$toWfa, simpleTwoStatePds)
    {
        using namespace testing::ShortestPathWeights;
        opennwa::SomeElements es;

        //  state and state2 are "in" es.symbol
        //  state3 is "in" es.symbol2
        //
        // state ------> state2 ------> state3 <---+
        //  /\                            |  |     |
        //   +----------------------------+  +-----+

        WPDS wpds;
        wpds.add_rule(es.symbol,  es.state,  es.symbol,  es.state2, dist1);
        wpds.add_rule(es.symbol,  es.state2, es.symbol2, es.state3, dist2);
        wpds.add_rule(es.symbol2, es.state3, es.symbol2, es.state3, dist3);
        wpds.add_rule(es.symbol2, es.state3, es.symbol,  es.state,  dist10);

        WFA expected;
        expected.addState(es.state, semiring_zero);
        expected.addState(es.state2, semiring_zero);
        expected.addState(es.state3, semiring_zero);
        expected.addTrans(es.state,  es.state,  es.state2, dist1);
        expected.addTrans(es.state2, es.state2, es.state3, dist2);
        expected.addTrans(es.state3, es.state3, es.state3, dist3);
        expected.addTrans(es.state3, es.state3, es.state,  dist10);

        WFA actual;
        wpds.toWfa(actual);

        EXPECT_TRUE(expected.isIsomorphicTo(actual));
    }


    TEST(wali$wpds$WPDS$toWfa, PdsWithDelta0RuleAborts)
    {
        using namespace testing::ShortestPathWeights;
        opennwa::SomeElements es;

        WPDS wpds;
        wpds.add_rule(es.symbol,  es.state,  es.symbol,  dist1);
        wpds.add_rule(es.symbol,  es.state2, es.symbol2, es.state3, dist2);

        WFA actual;
    
        EXPECT_DEATH(wpds.toWfa(actual), "r->to_stack1\\(\\) != WALI_EPSILON");
    }

    TEST(wali$wpds$WPDS$toWfa, PdsWithDelta2RuleAborts)
    {
        using namespace testing::ShortestPathWeights;
        opennwa::SomeElements es;

        WPDS wpds;
        wpds.add_rule(es.symbol,  es.state,  es.symbol,  es.state3, es.state3, dist1);
        wpds.add_rule(es.symbol,  es.state2, es.symbol2, es.state3, dist2);

        WFA actual;
    
        EXPECT_DEATH(wpds.toWfa(actual), "r->to_stack2\\(\\) == WALI_EPSILON");
    }
}

