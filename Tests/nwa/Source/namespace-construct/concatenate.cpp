#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/nwa/construct/concatenate.hpp"
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
    /*                    empty   balanced       strict left   maybe left    strict right   maybe right    maybe full */
    /* empty        */  { &empty, &empty,        &empty,       &empty,       &empty,        &empty,        &empty      },
    /* balanced     */  { &empty, &balanced,     &strict_left, &maybe_left,  &strict_right, &maybe_right,  &maybe_full },
    /* strict left  */  { &empty, &strict_left,  &strict_left, &strict_left, NULL,          NULL,          NULL,       },
    /* maybe left   */  { &empty, &maybe_left,   &strict_left, &maybe_left,  NULL,          &maybe_full,   &maybe_full },
    /* strict right */  { &empty, &strict_right, NULL,         NULL,         &strict_right, &strict_right, NULL,       },
    /* maybe right  */  { &empty, &maybe_right,  NULL,         &maybe_full,  &strict_right, &maybe_right,  &maybe_full },
    /* maybe full   */  { &empty, &maybe_full,   NULL,         &maybe_full,  NULL,          &maybe_full,   &maybe_full }
};


namespace wali {
    namespace nwa {
        namespace construct {

            TEST(wali$nwa$construct$$concat, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned left = 0 ; left < num_nwas ; ++left) {
                    for (unsigned right = 0 ; right < num_nwas ; ++right) {
                        std::stringstream ss;
                        ss << "Testing NWA " << left << " . " << right;
                        SCOPED_TRACE(ss.str());

                        NWA const * expected_answer = expected_answers[left][right];

                        // First, make sure that we expect an answer
                        // (i.e. it's not no_answer).
                        if (expected_answer && left != right) {
                            NWARefPtr u = concatenate(nwas[left], nwas[right]);

                            EXPECT_TRUE(query::languageEquals(*expected_answer, *u));
                        }
                    } // for right
                } // for left
            }
            

            TEST(wali$nwa$construct$$concat, resultingAutomatonIsNondeterministic)
            {
                SomeElements e;
                NWA left, right;
                left.addInitialState(e.state);
                left.addFinalState(e.state);
                right.addInitialState(e.state2);
                right.addInitialState(e.state2);

                NWARefPtr u = concatenate(left, right);

                EXPECT_FALSE(query::isDeterministic(*u));
            }
            
        }
    }
}
