#include <iostream>

#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/language.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"

using namespace opennwa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

// WARNING: the order of these words must be consistent with the row order
//          in the table 'expected_answers' below.
static Nwa const nwas[] = {
    Nwa(),
    AcceptsBalancedOnly().nwa,
    AcceptsStrictlyUnbalancedLeft().nwa,
    AcceptsPossiblyUnbalancedLeft().nwa,
    AcceptsStrictlyUnbalancedRight().nwa,
    AcceptsPossiblyUnbalancedRight().nwa,
    AcceptsPositionallyConsistentString().nwa
};

static const unsigned num_nwas = NUM_ELEMENTS(nwas);


// WARNING: the order of these words must be consistent with the column order
//          in the table 'expected_answers' below.
static NestedWord const words[] = {
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

static const unsigned num_words = NUM_ELEMENTS(words);


// WARNING: the order of the rows and columns in this table must be
//          consistent with the order of 'words' and 'nwas' above.
static bool expected_answers[][num_words] = {
    /*                     empty    bal    bal0    left   left0    right  right0   full   full0 */
    /* empty NWA      */ { false,   false, false,  false, false,   false, false,   false, false },
    /* balanced only  */ { true,    true,  true,   false, false,   false, false,   false, false },
    /* strictly left  */ { false,   false, false,  true,  true,    false, false,   false, false },
    /* maybe left     */ { true,    true,  true,   true,  true,    false, false,   false, false },
    /* strictly right */ { false,   false, false,  false, false,   true,  true,    false, false },
    /* maybe right    */ { true,    true,  true,   false, false,   true,  true,    false, false },
    /* maybe fully    */ { true,    true,  true,   true,  true,    true,  true,    true,  true  }
};



namespace opennwa {
        namespace query {

            TEST(opennwa$query$$languageContains, testBatteryOfVariouslyBalancedWordsAndNwas)
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


            TEST(opennwa$query$$languageContains, epsilonClosureAtStart)
            {
                Nwa nwa;
                SomeElements e;

                //              *           symbol
                //  --> state ----->  state2 ----> ((state3))

                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state3);

                nwa.addInternalTrans(e.state, EPSILON, e.state2);
                nwa.addInternalTrans(e.state2, e.symbol, e.state3);


                NestedWord nw;
                nw.appendInternal(e.symbol);

                EXPECT_TRUE(languageContains(nwa, nw));
            }

    }
}
