#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/nwa/construct/union.hpp"
#include "wali/nwa/query/automaton.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/int-client-info.hpp"
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


            TEST(wali$nwa$construct$$union, unionCopiesClientInfo)
            {
                AcceptsBalancedOnly bal;
                NWA nwa; SomeElements e; e.add_to_nwa(&nwa);

                ClientInfoRefPtr ci1 = new IntClientInfo(1);
                ClientInfoRefPtr ci2 = new IntClientInfo(2);
                
                bal.nwa.setClientInfo(bal.q0, ci1);
                nwa.setClientInfo(e.state, ci2);

                NWARefPtr u = unionNWA(bal.nwa, nwa);
                
#define EXPECT_CLIENT_INFO_IS(expect, nwa, state)                       \
                do {                                                    \
                    ClientInfo * ci = (nwa).getClientInfo(state).get_ptr(); \
                    IntClientInfo * ici = dynamic_cast<IntClientInfo *>(ci); \
                    ASSERT_TRUE(ici != NULL);                           \
                    EXPECT_EQ(expect, ici->n);                          \
                } while (0)

                EXPECT_CLIENT_INFO_IS(1, *u, bal.q0);
                EXPECT_CLIENT_INFO_IS(2, *u, e.state);
#undef EXPECT_CLIENT_INFO_IS

                EXPECT_NE(ci1, u->getClientInfo(bal.q0));
                EXPECT_NE(ci2, u->getClientInfo(e.state));
            }

            
        }
    }
}
