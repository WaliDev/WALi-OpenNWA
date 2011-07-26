#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"

using namespace wali::nwa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))


static NWA buildEmptyishNwa()
{
    NWA nwa;
    WordCollection words;
    nwa.addSymbol(words.zero);
    nwa.addSymbol(words.ret);
    nwa.addSymbol(words.call);
    return nwa;
}


// WARNING: the order of these words must be consistent with the row & column
//          order in the table 'expected_answers' below.
static NWA const nwas[] = {
    buildEmptyishNwa(),
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
static bool expected_answers[][num_nwas] = {
    /*                    empty      balanced    strict left    maybe left   strict right  maybe right   maybe full */
    /* empty        */  { true,      true,       true,          true,        true,         true,         true },
    /* balanced     */  { false,     true,       false,         true,        false,        true,         true },
    /* strict left  */  { false,     false,      true,          true,        false,        false,        true },
    /* maybe left   */  { false,     false,      false,         true,        false,        false,        true },
    /* strict right */  { false,     false,      false,         false,       true,         true,         true },
    /* maybe right  */  { false,     false,      false,         false,       false,        true,         true },
    /* maybe full   */  { false,     false,      false,         false,       false,        false,        true }
};


namespace wali {
    namespace nwa {
        namespace query {

            TEST(wali$nwa$query$$languageSubetEq, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned left = 0 ; left < num_nwas ; ++left) {
                    for (unsigned right = 0 ; right < num_nwas ; ++right) {
                        std::stringstream ss;
                        ss << "Testing NWA " << left << " \\subseteq " << right;
                        SCOPED_TRACE(ss.str());

                        if (expected_answers[left][right]) {
                            EXPECT_TRUE(languageSubsetEq(nwas[left], nwas[right]));
                        }
                        else {
                            EXPECT_FALSE(languageSubsetEq(nwas[left], nwas[right]));
                        }
                    }
                }
            }


            TEST(wali$nwa$query$$languageEquals, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned left = 0 ; left < num_nwas ; ++left) {
                    for (unsigned right = left ; right < num_nwas ; ++right) {
                        std::stringstream ss;
                        ss << "Testing NWA " << left << " = " << right;
                        SCOPED_TRACE(ss.str());

                        // If left subseteq right and right subseteq left,
                        // then they are equal
                        if (expected_answers[left][right]
                            && expected_answers[right][left]) {
                            EXPECT_TRUE(languageEquals(nwas[left], nwas[right]));
                        }
                        else {
                            EXPECT_FALSE(languageEquals(nwas[left], nwas[right]));
                        }
                    }
                }
            }
            
        }
    }
}
