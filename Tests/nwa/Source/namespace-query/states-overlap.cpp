#include "gtest/gtest.h"

#include "opennwa/NWA.hpp"
#include "opennwa/query/automaton.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"


namespace wali {
    namespace nwa {
        namespace query {

            TEST(wali$nwa$query$$statesOverlap, emptyDoesNotOverlap)
            {
                OddNumEvenGroupsNwa fixture;
                NWA empty;

                EXPECT_FALSE(statesOverlap(empty, empty));
                EXPECT_FALSE(statesOverlap(empty, fixture.nwa));
            }


            TEST(wali$nwa$query$$statesOverlap, oddEvenOverlapsWithItself)
            {
                OddNumEvenGroupsNwa f1, f2;

                EXPECT_TRUE(statesOverlap(f1.nwa, f2.nwa));
            }


            TEST(wali$nwa$query$$statesOverlap, subsetsOverlap)
            {
                OddNumEvenGroupsNwa fixture;
                NWA nwa;
                
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


            TEST(wali$nwa$query$$statesOverlap, largeDisjointLargeDoNotOverlap)
            {
                OddNumEvenGroupsNwa fixture;
                SomeElements e;
                NWA nwa;

                e.add_to_nwa(&nwa);

                EXPECT_FALSE(statesOverlap(fixture.nwa, nwa));
            }

            
            TEST(wali$nwa$query$$statesOverlap, incomparableDoNotOverlap)
            {
                OddNumEvenGroupsNwa fixture;
                NWA nwa1 = fixture.nwa;
                NWA nwa2 = fixture.nwa;

                ASSERT_TRUE(nwa1.removeState(fixture.dummy));
                ASSERT_TRUE(nwa2.removeState(fixture.q0));

                EXPECT_TRUE(statesOverlap(nwa1, nwa2));
            }

        }
    }
}
