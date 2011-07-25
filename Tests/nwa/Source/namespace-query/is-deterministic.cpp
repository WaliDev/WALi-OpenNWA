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

        }
    }
}
