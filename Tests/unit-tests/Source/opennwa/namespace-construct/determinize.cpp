#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/language.hpp"
#include "opennwa/construct/concat.hpp"
#include "opennwa/construct/determinize.hpp"
#include "opennwa/query/automaton.hpp"
#include "opennwa/query/language.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/int-client-info.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"

namespace opennwa {
        namespace construct {

            TEST(opennwa$construct$$determinize, allowPendingCallsOnAccepting)
            {
                SomeElements e;
                Nwa nwa;

                //              *          symbol   ______
                //  --> state -----> state2 ----> ((state3))
                //                                  ``````
                
                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state3);

                nwa.addInternalTrans(e.state, EPSILON, e.state2);
                nwa.addCallTrans(e.state2, e.symbol, e.state3);

                NwaRefPtr det = determinize(nwa);

                EXPECT_FALSE(query::languageIsEmpty(*det));
            }

            
            TEST(opennwa$construct$$determinize, initialStateHasEpsilonClosureDone)
            {
                SomeElements e;
                Nwa nwa;

                //              *
                //  --> state -----> ((state2))
                
                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state2);

                nwa.addInternalTrans(e.state, EPSILON, e.state2);

                NwaRefPtr det = determinize(nwa);

                EXPECT_FALSE(query::languageIsEmpty(*det));
            }


            TEST(opennwa$construct$$determinize, useADifferentInitialStateToStartAndAsAPendingReturn)
            {
                SomeElements e;
                Nwa nwa;

                // --> state
                // --> state2 -----------------------> ((state3))
                //            symbol/state (as return)

                nwa.addInitialState(e.state);
                nwa.addInitialState(e.state2);
                nwa.addFinalState(e.state3);

                nwa.addReturnTrans(e.state2, e.state, e.symbol, e.state3);

                NwaRefPtr det = determinize(nwa);

                // Now test to see if it's got 'symbol(' in it, and that it
                // doesn't contain epsilon
                NestedWord empty, word;
                word.appendReturn(e.symbol);

                EXPECT_FALSE(query::languageContains(*det, empty));
                EXPECT_TRUE(query::languageContains(*det, word));

                // Some structure tests. We'll remove the stuck state, then
                // make sure it's isomorphic to
                //       --> o ------> (o)
                // as best we can.
                //
                // I *think* that if all these tests pass, combined with the
                // language tests below, there is only one NWA that can be,
                // and it's correct. (This may not be the case if {} had
                // outgoing transitions though.)
                EXPECT_EQ(3u, det->sizeStates());

                State stuck = getKey("{}"); // implementation detail
                ASSERT_TRUE(det->isState(stuck));
                EXPECT_TRUE(!det->isFinalState(stuck));

                det->removeState(stuck);

                EXPECT_EQ(2u, det->sizeStates());
                EXPECT_EQ(1u, det->sizeInitialStates());
                EXPECT_EQ(1u, det->sizeFinalStates());
                EXPECT_EQ(1u, det->sizeSymbols());
                EXPECT_EQ(0u, det->sizeInternalTrans());
                EXPECT_EQ(0u, det->sizeCallTrans());
                EXPECT_EQ(1u, det->sizeReturnTrans());

                EXPECT_FALSE(query::languageContains(*det, empty));
                EXPECT_TRUE(query::languageContains(*det, word));
            }

        }
}

