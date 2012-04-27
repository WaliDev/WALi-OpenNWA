#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/automaton.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"


namespace opennwa {
        namespace query {

            TEST(opennwa$query$$statesOverlap, emptyDoesNotOverlap)
            {
                OddNumEvenGroupsNwa fixture;
                Nwa empty;

                EXPECT_FALSE(statesOverlap(empty, empty));
                EXPECT_FALSE(statesOverlap(empty, fixture.nwa));
            }


            TEST(opennwa$query$$statesOverlap, oddEvenOverlapsWithItself)
            {
                OddNumEvenGroupsNwa f1, f2;

                EXPECT_TRUE(statesOverlap(f1.nwa, f2.nwa));
            }


            TEST(opennwa$query$$statesOverlap, subsetsOverlap)
            {
                OddNumEvenGroupsNwa fixture;
                Nwa nwa;
                
                // 1 element overlap
                ASSERT_TRUE(nwa.addState(fixture.q0));
                EXPECT_TRUE(statesOverlap(fixture.nwa, nwa));

                // 2 element overlap
                ASSERT_TRUE(nwa.addState(fixture.q1));
                EXPECT_TRUE(statesOverlap(fixture.nwa, nwa));

                // Large overlap
                nwa = fixture.nwa;
                ASSERT_TRUE(nwa.removeState(fixture.dummy));
                EXPECT_TRUE(statesOverlap(fixture.nwa, nwa));
            }


            TEST(opennwa$query$$statesOverlap, largeDisjointLargeDoNotOverlap)
            {
                OddNumEvenGroupsNwa fixture;
                SomeElements e;
                Nwa nwa;

                e.add_to_nwa(&nwa);

                EXPECT_FALSE(statesOverlap(fixture.nwa, nwa));
            }

            
            TEST(opennwa$query$$statesOverlap, incomparableDoNotOverlap)
            {
                OddNumEvenGroupsNwa fixture;
                Nwa nwa1 = fixture.nwa;
                Nwa nwa2 = fixture.nwa;

                ASSERT_TRUE(nwa1.removeState(fixture.dummy));
                ASSERT_TRUE(nwa2.removeState(fixture.q0));

                EXPECT_TRUE(statesOverlap(nwa1, nwa2));
            }

    }
}
