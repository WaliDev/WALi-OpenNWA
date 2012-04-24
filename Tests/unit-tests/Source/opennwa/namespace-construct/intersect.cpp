#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/language.hpp"
#include "opennwa/construct/intersect.hpp"
#include "opennwa/query/automaton.hpp"
#include "opennwa/query/language.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/int-client-info.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"

using namespace opennwa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

static const Nwa empty,
    balanced = AcceptsBalancedOnly().nwa,
    strict_left = AcceptsStrictlyUnbalancedLeft().nwa,
    maybe_left = AcceptsPossiblyUnbalancedLeft().nwa,
    strict_right = AcceptsStrictlyUnbalancedRight().nwa,
    maybe_right = AcceptsPossiblyUnbalancedRight().nwa,
    maybe_full = AcceptsPositionallyConsistentString().nwa;
    

// WARNING: the order of these words must be consistent with the row & column
//          order in the table 'expected_answers' below.
static Nwa const nwas[] = {
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
static const Nwa * const expected_answers[][num_nwas] = {
    /*                    empty   balanced   strict left   maybe left    strict right   maybe right    maybe full */
    /* empty        */  { &empty, &empty,    &empty,       &empty,       &empty,        &empty,        &empty        },
    /* balanced     */  { &empty, &balanced, &empty,       &balanced,    &empty,        &balanced,     &balanced     },
    /* strict left  */  { &empty, &empty,    &strict_left, &strict_left, &empty,        &empty,        &strict_left  },
    /* maybe left   */  { &empty, &balanced, &strict_left, &maybe_left,  &empty,        &balanced,     &maybe_left   },
    /* strict right */  { &empty, &empty,    &empty,       &empty,       &strict_right, &strict_right, &strict_right },
    /* maybe right  */  { &empty, &balanced, &empty,       &balanced,    &strict_right, &maybe_right,  &maybe_right  },
    /* maybe full   */  { &empty, &balanced, &strict_left, &maybe_left,  &strict_right, &maybe_right,  &maybe_full   }
};


namespace opennwa {
        namespace construct {

            TEST(opennwa$construct$$intersection, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned left = 0 ; left < num_nwas ; ++left) {
                    for (unsigned right = 0 ; right < num_nwas ; ++right) {
                        std::stringstream ss;
                        ss << "Testing NWA " << left << " intersect " << right;
                        SCOPED_TRACE(ss.str());

                        Nwa const * expected_answer = expected_answers[left][right];

                        // First, make sure that we expect an answer
                        // (i.e. it's not no_answer).
                        if (expected_answer && left != right) {
                            NwaRefPtr u = intersect(nwas[left], nwas[right]);
                            
                            EXPECT_TRUE(query::languageEquals(*expected_answer, *u));
                        }
                    } // for right
                } // for left
            }
            

            TEST(opennwa$construct$$intersection, resultingAutomatonIsDeterministic)
            {
                Nwa left, right;
                left.addInitialState(getKey("s"));
                right.addInitialState(getKey("t"));

                NwaRefPtr u = intersect(left, right);

                EXPECT_TRUE(query::isDeterministic(*u));
            }


            
    }
}
