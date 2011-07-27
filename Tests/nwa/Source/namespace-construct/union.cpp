#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/nwa/construct/union.hpp"
#include "wali/nwa/query/automaton.hpp"
#include "wali/nwa/query/language.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/int-client-info.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"

using namespace wali::nwa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

static const NWA empty,
    balanced = AcceptsBalancedOnly().nwa,
    strict_left = AcceptsStrictlyUnbalancedLeft().nwa,
    maybe_left = AcceptsPossiblyUnbalancedLeft().nwa,
    strict_right = AcceptsStrictlyUnbalancedRight().nwa,
    maybe_right = AcceptsPossiblyUnbalancedRight().nwa,
    maybe_full = AcceptsPositionallyConsistentString().nwa;
    

// WARNING: the order of these words must be consistent with the row & column
//          order in the table 'expected_answers' below.
static NWA const nwas[] = {
    empty,
    balanced,
    strict_left,
    maybe_left,
    strict_right,
    maybe_right,
    maybe_full
};

static const unsigned num_nwas = NUM_ELEMENTS(nwas);


// WARNING: the order of the rows and columns in this table must be
//          consistent with the order of 'nwas' above.
//
// "What is the union of the row and column?"
static const NWA * const expected_answers[][num_nwas] = {
    /*                    empty          balanced      strict left   maybe left   strict right   maybe right   maybe full */
    /* empty        */  { &empty,        &balanced,    &strict_left, &maybe_left, &strict_right, &maybe_right, &maybe_full },
    /* balanced     */  { &balanced,     &balanced,    &maybe_left,  &maybe_left, &maybe_right,  &maybe_right, &maybe_full },
    /* strict left  */  { &strict_left,  &maybe_left,  &strict_left, &maybe_left, NULL,          &maybe_full,  &maybe_full },
    /* maybe left   */  { &maybe_left,   &maybe_left,  &maybe_left,  &maybe_left, &maybe_full,   &maybe_full,  &maybe_full },
    /* strict right */  { &strict_right, &maybe_right, NULL,         &maybe_full, &strict_right, &maybe_right, &maybe_full },
    /* maybe right  */  { &maybe_right,  &maybe_right, &maybe_full,  &maybe_full, &maybe_right,  &maybe_right, &maybe_full },
    /* maybe full   */  { &maybe_full,   &maybe_full,  &maybe_full,  &maybe_full, &maybe_full,   &maybe_full,  &maybe_full }
};


namespace wali {
    namespace nwa {
        namespace construct {

            TEST(wali$nwa$construct$$union, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned left = 0 ; left < num_nwas ; ++left) {
                    for (unsigned right = 0 ; right < num_nwas ; ++right) {
                        std::stringstream ss;
                        ss << "Testing NWA " << left << " union " << right;
                        SCOPED_TRACE(ss.str());

                        NWA const * expected_answer = expected_answers[left][right];

                        // First, make sure that we expect an answer
                        // (i.e. it's not no_answer).
                        if (expected_answer) {
                            NWARefPtr u = unionNWA(nwas[left], nwas[right]);
                            
                            EXPECT_TRUE(query::languageEquals(*expected_answer, *u));
                        }
                    } // for right
                } // for left
            }
            

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
