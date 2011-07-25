#include <iostream>

#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"

using namespace wali::nwa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

// WARNING: the order of these words must be consistent with the row order
//          in the table 'expected_answers' below.
NWA const nwas[] = {
    AcceptsBalancedOnly().nwa,
    AcceptsStrictlyUnbalancedLeft().nwa,
    AcceptsPossiblyUnbalancedLeft().nwa,
    AcceptsStrictlyUnbalancedRight().nwa,
    AcceptsPossiblyUnbalancedRight().nwa,
    AcceptsPositionallyConsistentString().nwa
};

const unsigned num_nwas = NUM_ELEMENTS(nwas);


// WARNING: the order of these words must be consistent with the column order
//          in the table 'expected_answers' below.
NestedWord const words[] = {
    WordCollection().empty,
    WordCollection().balanced,
    WordCollection().balanced0,
    WordCollection().unbalancedLeft,
    WordCollection().unbalancedLeft0,
    WordCollection().unbalancedRight,
    WordCollection().unbalancedRight0,
    WordCollection().fullyUnbalanced,
    WordCollection().fullyUnbalanced0
};

const unsigned num_words = NUM_ELEMENTS(words);


// WARNING: the order of the rows and columns in this table must be
//          consistent with the order of 'words' and 'nwas' above.
bool expected_answers[][num_words] = {
    /*                     empty    bal    bal0    left   left0    right  right0   full   full0 */
    /* balanced only  */ { true,    true,  true,   false, false,   false, false,   false, false },
    /* strictly left  */ { false,   false, true,   true,  true,    false, false,   false, false },
    /* maybe left     */ { true,    true,  true,   true,  true,    false, false,   false, false },
    /* strictly right */ { false,   false, true,   false, false,   true,  true,    false, false },
    /* maybe right    */ { true,    true,  true,   false, false,   true,  true,    false, false },
    /* maybe fully    */ { true,    true,  true,   true,  true,    true,  true,    true,  true  }
};



namespace wali {
    namespace nwa {
        namespace query {

            TEST(wali$nwa$query$$languageContains, testBatteryOfVariouslyBalancedWordsAndNwas)
            {
                for (unsigned word = 0 ; word < num_words ; ++word) {
                    for (unsigned nwa = 0 ; nwa < num_nwas ; ++nwa) {
                        std::stringstream ss;
                        ss << "Current NWA number " << nwa << " and word number " << word;
                        SCOPED_TRACE(ss.str());

                        if (expected_answers[nwa][word]) {
                            EXPECT_TRUE(languageContains(nwas[nwa], words[word]));
                        }
                        else {
                            EXPECT_FALSE(languageContains(nwas[nwa], words[word]));
                        }
                    }
                }
            }


        }
    }
}
