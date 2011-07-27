#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/nwa/construct/complement.hpp"

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
    /* empty        */  false,
    /* balanced     */  true,
    /* strict left  */  true,
    /* maybe left   */  true,
    /* strict right */  true,
    /* maybe right  */  true,
    /* maybe full   */  true
};


namespace wali {
    namespace nwa {
        namespace construct {

            TEST(wali$nwa$construct$$complement, complementOfACompletelyEmptyAutomatonContainsStuff)
            {
                NWA empty;
                WordCollection words;
                
                empty.addSymbol(words.zero);
                empty.addSymbol(words.ret);
                empty.addSymbol(words.call);
                                
                NWARefPtr comp = complement(empty);

                EXPECT_TRUE(query::languageContains(*comp, words.empty));
                EXPECT_TRUE(query::languageContains(*comp, words.balanced));
                EXPECT_TRUE(query::languageContains(*comp, words.balanced0));
                EXPECT_TRUE(query::languageContains(*comp, words.unbalancedLeft));
                EXPECT_TRUE(query::languageContains(*comp, words.unbalancedLeft0));
                EXPECT_TRUE(query::languageContains(*comp, words.unbalancedRight));
                EXPECT_TRUE(query::languageContains(*comp, words.unbalancedRight0));
                EXPECT_TRUE(query::languageContains(*comp, words.fullyUnbalanced));
                EXPECT_TRUE(query::languageContains(*comp, words.fullyUnbalanced0));
            }

            TEST(wali$nwa$query$$complement, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned nwa = 0 ; nwa < num_nwas ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());

                    NWARefPtr c = complement(nwas[nwa]);
                    
                    if (expected_answers[nwa]) {
                        EXPECT_TRUE(query::languageIsEmpty(*c));
                    }
                    else {
                        EXPECT_FALSE(query::languageIsEmpty(*c));
                    }
                }
            }


        }
    }
}
