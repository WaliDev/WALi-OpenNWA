#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/automaton.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"

using namespace opennwa;
using namespace std;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

// WARNING: the order of these words must be consistent with the row & column
//          order in the table 'expected_answers' below.
static Nwa const nwas[] = {
    Nwa(),
    AcceptsBalancedOnly().nwa,
    AcceptsStrictlyUnbalancedLeft().nwa,
    AcceptsPossiblyUnbalancedLeft().nwa,
    AcceptsStrictlyUnbalancedRight().nwa,
    AcceptsPossiblyUnbalancedRight().nwa,
    AcceptsPositionallyConsistentString().nwa,
    OddNumEvenGroupsNwa().nwa
};

static const unsigned num_nwas = NUM_ELEMENTS(nwas);


// WARNING: the order of the rows and columns in this table must be
//          consistent with the order of 'words' and 'nwas' above.
//
// "What is the number of <type of state> in the nwa?"
static size_t expected_answers[][4] = {
    //                   call   entry   exit   return
    /* empty        */  {0,     0,      0,     0},
    /* balanced     */  {2,     1,      1,     2},
    /* strict left  */  {4,     2,      4,     4},
    /* maybe left   */  {2,     1,      2,     2},
    /* strict right */  {2,     1,      1,     2},
    /* maybe right  */  {2,     1,      1,     2},
    /* maybe full   */  {2,     1,      2,     2},
    /* odd num even */  {3,     2,      2,     3}
};


namespace opennwa {
        namespace query {
            

            TEST(opennwa$query$$stats, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned nwa = 0 ; nwa < num_nwas ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());

                    size_t expected_calls = expected_answers[nwa][0];
                    size_t expected_entries = expected_answers[nwa][1];
                    size_t expected_exits = expected_answers[nwa][2];
                    size_t expected_returns = expected_answers[nwa][3];

                    EXPECT_EQ(expected_calls, numCallSites(nwas[nwa]));
                    EXPECT_EQ(expected_entries, numEntrySites(nwas[nwa]));
                    EXPECT_EQ(expected_exits, numExitSites(nwas[nwa]));
                    EXPECT_EQ(expected_returns, numReturnSites(nwas[nwa]));
                }
            }
    }
}
