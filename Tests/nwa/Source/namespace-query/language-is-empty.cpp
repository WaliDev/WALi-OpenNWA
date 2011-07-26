#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"

using namespace wali::nwa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

// WARNING: the order of these words must be consistent with the row & column
//          order in the table 'expected_answers' below.
static NWA const nwas[] = {
    NWA(),
    AcceptsBalancedOnly().nwa,
    AcceptsStrictlyUnbalancedLeft().nwa,
    AcceptsPossiblyUnbalancedLeft().nwa,
    AcceptsStrictlyUnbalancedRight().nwa,
    AcceptsPossiblyUnbalancedRight().nwa,
    AcceptsPositionallyConsistentString().nwa
};

static const unsigned num_nwas = NUM_ELEMENTS(nwas);


// WARNING: the order of the rows and columns in this table must be
//          consistent with the order of 'words' and 'nwas' above.
//
// "Is the row NWA a subset(eq) of the column NWA?"
static bool expected_answers[] = {
    /* empty        */  true,
    /* balanced     */  false,
    /* strict left  */  false,
    /* maybe left   */  false,
    /* strict right */  true,  // WRONG once unbalanced
    /* maybe right  */  false,
    /* maybe full   */  false
};


namespace wali {
    namespace nwa {
        namespace query {

            TEST(wali$nwa$query$$languageIsEmpty, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned nwa = 0 ; nwa < num_nwas ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());
                    
                    if (expected_answers[nwa]) {
                        EXPECT_TRUE(languageIsEmpty(nwas[nwa]));
                    }
                    else {
                        EXPECT_FALSE(languageIsEmpty(nwas[nwa]));
                    }
                }
            }

            
        }
    }
}
