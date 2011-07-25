#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/automaton.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"


namespace wali {
    namespace nwa {
        namespace query {

            TEST(wali$nwa$query$$isDeterministic, emptyNwaIsDeterministic)
            {
                NWA empty;

                EXPECT_TRUE(isDeterministic(empty));
            }

                    
            TEST(wali$nwa$query$$isDeterministic, evenOddNotDeterministic)
            {
                OddNumEvenGroupsNwa fixture;

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }


            TEST(wali$nwa$query$$isDeterministic, evenOddWithoutDummyIsDeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                fixture.nwa.removeState(fixture.dummy);

                EXPECT_TRUE(isDeterministic(fixture.nwa));
            }


            TEST(wali$nwa$query$$isDeterministic, multipleInitialStatesMakesNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;

                fixture.nwa.addInitialState(fixture.q2);

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }


            TEST(wali$nwa$query$$isDeterministic, merePresenceOfWildsDoesNotMakeNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                fixture.nwa.removeState(fixture.dummy);

                fixture.nwa.addInternalTrans(fixture.q0, WALI_WILD, fixture.q1);
                fixture.nwa.addCallTrans(fixture.q3, WALI_WILD, fixture.q1);
                fixture.nwa.addReturnTrans(fixture.q2, fixture.q3, WALI_WILD, fixture.q1);
                // Make sure we test two wild transitions out of the same
                // state with different call predecessors...
                fixture.nwa.addReturnTrans(fixture.q2, fixture.q0, WALI_WILD, fixture.q1);
                // ... and from different states with the same call predecessors
                fixture.nwa.addReturnTrans(fixture.q1, fixture.q3, WALI_WILD, fixture.q1);
                
                EXPECT_TRUE(isDeterministic(fixture.nwa));
            }

            TEST(wali$nwa$query$$isDeterministic, twoWildInternalTransitionsFromSameStateMakesNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                fixture.nwa.removeState(fixture.dummy);

                fixture.nwa.addInternalTrans(fixture.q0, WALI_WILD, fixture.q1);
                fixture.nwa.addInternalTrans(fixture.q0, WALI_WILD, fixture.q2);

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }

            TEST(wali$nwa$query$$isDeterministic, twoWildCallTransitionsFromSameStateMakesNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                fixture.nwa.removeState(fixture.dummy);

                fixture.nwa.addCallTrans(fixture.q3, WALI_WILD, fixture.q1);
                fixture.nwa.addCallTrans(fixture.q3, WALI_WILD, fixture.q1);

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }

            TEST(wali$nwa$query$$isDeterministic, twoWildReturnTransitionsFromSameStateAndPredecessorMakesNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                fixture.nwa.removeState(fixture.dummy);

                fixture.nwa.addReturnTrans(fixture.q2, fixture.q1, WALI_WILD, fixture.q3);
                fixture.nwa.addReturnTrans(fixture.q2, fixture.q1, WALI_WILD, fixture.q0);

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }

        }
    }
}
