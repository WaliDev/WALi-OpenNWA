#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/language.hpp"
#include "opennwa/construct/complement.hpp"
#include "opennwa/construct/intersect.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"


using namespace opennwa;
using std::vector;

namespace {
    vector<Nwa> test_nwas() {
#       define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))
        
        // WARNING: the order of these words must be consistent with the row & column
        //          order in the table 'expected_answers' below.
        Nwa const nwas[] = {
            Nwa(),
            AcceptsBalancedOnly().nwa,
            AcceptsStrictlyUnbalancedLeft().nwa,
            AcceptsPossiblyUnbalancedLeft().nwa,
            AcceptsStrictlyUnbalancedRight().nwa,
            AcceptsPossiblyUnbalancedRight().nwa,
            AcceptsPositionallyConsistentString().nwa
        };
        
        const unsigned num_nwas = NUM_ELEMENTS(nwas);
        
        return vector<Nwa>(nwas, nwas+num_nwas);
    }
}

// WARNING: the order of the rows and columns in this table must be
//          consistent with the order of 'words' and 'nwas' above.
//
// "Is the row NWA a subset(eq) of the column NWA?"
static bool expected_answers[] = {
    /* empty        */  false,
    /* balanced     */  true,
    /* strict left  */  false,
    /* maybe left   */  true,
    /* strict right */  false,
    /* maybe right  */  true,
    /* maybe full   */  true
};


namespace opennwa {
        namespace construct {

            TEST(opennwa$construct$$complement, complementOfACompletelyEmptyAutomatonContainsStuff)
            {
                Nwa empty;
                WordCollection words;
                
                empty.addSymbol(words.zero);
                empty.addSymbol(words.ret);
                empty.addSymbol(words.call);
                                
                NwaRefPtr comp = complement(empty);

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

            TEST(opennwa$query$$complement, testBatteryOfVariouslyBalancedNwas)
            {
                vector<Nwa> nwas = test_nwas();
                for (unsigned nwa = 0 ; nwa < nwas.size() ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());

                    NwaRefPtr c = complement(nwas[nwa]);
                    c = intersect(*c, AcceptsBalancedOnly().nwa);
                    
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
