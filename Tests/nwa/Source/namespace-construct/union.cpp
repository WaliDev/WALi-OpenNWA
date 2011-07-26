#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/nwa/construct/union.hpp"
#include "wali/nwa/query/automaton.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"

namespace wali {
    namespace nwa {
        namespace construct {

            TEST(wali$nwa$construct$$union, resultingAutomatonIsNondeterministic)
            {
                NWA left, right;
                left.addInitialState(getKey("s"));
                right.addInitialState(getKey("t"));

                NWARefPtr u = unionNWA(left, right);

                EXPECT_FALSE(query::isDeterministic(*u));
            }


            TEST(wali$nwa$construct$$union, unionWithEmptyIsNoop)
            {
                OddNumEvenGroupsNwa fixture;
                NWA empty;

                NWARefPtr u = unionNWA(fixture.nwa, empty);

                EXPECT_EQ(fixture.nwa, *u);

                u = unionNWA(empty, empty);

                EXPECT_EQ(empty, empty);
            }


            TEST(wali$nwa$construct$$union$$DeathTest, overlappingStatesTriggerAssertionViolation)
            {
                NWA nwa;
                nwa.addState(getKey("s"));

                EXPECT_DEATH({
                        NWARefPtr u = unionNWA(nwa, nwa);
                    },
                    "statesOverlap");
            }

            
        }
    }
}
