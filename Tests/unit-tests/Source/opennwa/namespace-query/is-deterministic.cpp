#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/automaton.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"


namespace opennwa {
        namespace query {

            TEST(opennwa$query$$isDeterministic, emptyNwaIsDeterministic)
            {
                Nwa empty;

                EXPECT_TRUE(isDeterministic(empty));
            }

                    
            TEST(opennwa$query$$isDeterministic, evenOddNotDeterministic)
            {
                OddNumEvenGroupsNwa fixture;

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }


            TEST(opennwa$query$$isDeterministic, evenOddWithoutDummyIsDeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                ASSERT_TRUE(fixture.nwa.removeState(fixture.dummy));

                EXPECT_TRUE(isDeterministic(fixture.nwa));
            }


            TEST(opennwa$query$$isDeterministic, multipleInitialStatesMakesNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;

                ASSERT_TRUE(fixture.nwa.addInitialState(fixture.q2));

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }


            //////////////////
            // Wilds
            
            TEST(opennwa$query$$isDeterministic, merePresenceOfWildsDoesNotMakeNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                ASSERT_TRUE(fixture.nwa.removeState(fixture.dummy));

                ASSERT_TRUE(fixture.nwa.addInternalTrans(fixture.q0, WILD, fixture.q1));
                ASSERT_TRUE(fixture.nwa.addCallTrans(fixture.q3, WILD, fixture.q1));
                ASSERT_TRUE(fixture.nwa.addReturnTrans(fixture.q2, fixture.q3, WILD, fixture.q1));
                // Make sure we test two wild transitions out of the same
                // state with different call predecessors...
                ASSERT_TRUE(fixture.nwa.addReturnTrans(fixture.q2, fixture.q0, WILD, fixture.q1));
                // ... and from different states with the same call predecessors
                ASSERT_TRUE(fixture.nwa.addReturnTrans(fixture.q1, fixture.q3, WILD, fixture.q1));
                
                EXPECT_TRUE(isDeterministic(fixture.nwa));
            }

            TEST(opennwa$query$$isDeterministic, twoWildInternalTransitionsFromSameStateMakesNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                ASSERT_TRUE(fixture.nwa.removeState(fixture.dummy));

                ASSERT_TRUE(fixture.nwa.addInternalTrans(fixture.q0, WILD, fixture.q1));
                ASSERT_TRUE(fixture.nwa.addInternalTrans(fixture.q0, WILD, fixture.q2));

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }

            TEST(opennwa$query$$isDeterministic, twoWildCallTransitionsFromSameStateMakesNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                ASSERT_TRUE(fixture.nwa.removeState(fixture.dummy));

                ASSERT_TRUE(fixture.nwa.addCallTrans(fixture.q3, WILD, fixture.q1));
                ASSERT_TRUE(fixture.nwa.addCallTrans(fixture.q3, WILD, fixture.q2));

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }

            TEST(opennwa$query$$isDeterministic, twoWildReturnTransitionsFromSameStateAndPredecessorMakesNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                fixture.nwa.removeState(fixture.dummy);

                fixture.nwa.addReturnTrans(fixture.q2, fixture.q1, WILD, fixture.q3);
                fixture.nwa.addReturnTrans(fixture.q2, fixture.q1, WILD, fixture.q0);

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }

            ///////////////////
            // Epsilon transitions

            TEST(opennwa$query$$isDeterministic, internalEpsilonTransitionsMakeNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                ASSERT_TRUE(fixture.nwa.removeState(fixture.dummy));
                
                ASSERT_TRUE(fixture.nwa.addInternalTrans(fixture.q2, EPSILON, fixture.q0));

                EXPECT_FALSE(isDeterministic(fixture.nwa));

                ASSERT_TRUE(fixture.nwa.addInternalTrans(fixture.q2, EPSILON, fixture.q2));

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }


            //////////////////
            // "Normal" transitions

            TEST(opennwa$query$$isDeterministic, twoTransitionsFromTheSameNodeAreNotEnoughToMakeNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                ASSERT_TRUE(fixture.nwa.removeState(fixture.dummy));

                // Same source & destination, different symbol
                ASSERT_TRUE(fixture.nwa.addInternalTrans(fixture.q2, fixture.call, fixture.q3));
                ASSERT_TRUE(fixture.nwa.addCallTrans(fixture.q0, fixture.zero, fixture.q2));

                // Same source, predecessor, & destination; different symbol
                ASSERT_TRUE(fixture.nwa.addReturnTrans(fixture.q3, fixture.q1, fixture.zero, fixture.q0));
                // Same source, symbol, & destination; different predecessor
                ASSERT_TRUE(fixture.nwa.addReturnTrans(fixture.q3, fixture.q2, fixture.ret, fixture.q0));

                EXPECT_TRUE(isDeterministic(fixture.nwa));
            }

            
            TEST(opennwa$query$$isDeterministic, twoInternalsWithSameSymbolMakeNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                ASSERT_TRUE(fixture.nwa.removeState(fixture.dummy));
                
                ASSERT_TRUE(fixture.nwa.addInternalTrans(fixture.q2, fixture.zero, fixture.q0));

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }


            TEST(opennwa$query$$isDeterministic, twoCallsWithSameSymbolMakeNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                ASSERT_TRUE(fixture.nwa.removeState(fixture.dummy));
                
                ASSERT_TRUE(fixture.nwa.addCallTrans(fixture.q0, fixture.call, fixture.q3));

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }

            TEST(opennwa$query$$isDeterministic, twoReturnsWithSamePredecessorAndSymbolMakeNondeterministic)
            {
                OddNumEvenGroupsNwa fixture;
                ASSERT_TRUE(fixture.nwa.removeState(fixture.dummy));
                
                ASSERT_TRUE(fixture.nwa.addReturnTrans(fixture.q3, fixture.q1, fixture.ret, fixture.q2));

                EXPECT_FALSE(isDeterministic(fixture.nwa));
            }

    }
}
