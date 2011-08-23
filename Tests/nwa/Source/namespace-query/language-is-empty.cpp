#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"

using namespace wali::nwa;
using namespace std;

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
    /* strict right */  false,
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

            
            TEST(wali$nwa$query$$getSomeAcceptedWord, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned nwa = 0 ; nwa < num_nwas ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());

                    NestedWordRefPtr word = getSomeAcceptedWord(nwas[nwa]);
                    
                    if (expected_answers[nwa]) {
                        EXPECT_TRUE(word == NULL);
                    }
                    else {
                        ASSERT_TRUE(word != NULL);
                        EXPECT_TRUE(languageContains(nwas[nwa], *word));
                    }
                }
            }

            
            TEST(wali$nwa$query$$languageIsEmpty$and$getSomeAcceptedWord, testEpsilonNwa)
            {
                NWA nwa;
                SomeElements e;
                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state);

                EXPECT_FALSE(languageIsEmpty(nwa));

                NestedWord eps;

                NestedWordRefPtr word = getSomeAcceptedWord(nwa);
                EXPECT_EQ(eps, *word);
            }
            
            TEST(wali$nwa$query$$languageIsEmpty$and$getSomeAcceptedWord, testInternalOnlyNwa)
            {
                //               a              a              a
                //  --> (state) ----> (state2) ----> (state3) ---> ((state4))
                NWA nwa;
                SomeElements e;
                State state4 = getKey("state4");
                    
                nwa.addInitialState(e.state);
                nwa.addInternalTrans(e.state, e.symbol, e.state2);
                nwa.addInternalTrans(e.state2, e.symbol, e.state3);
                nwa.addInternalTrans(e.state3, e.symbol, state4);
                nwa.addFinalState(state4);
                
                EXPECT_FALSE(languageIsEmpty(nwa));

                NestedWord expected;
                expected.appendInternal(e.symbol);
                expected.appendInternal(e.symbol);
                expected.appendInternal(e.symbol);

                NestedWordRefPtr word = getSomeAcceptedWord(nwa);
                
                EXPECT_EQ(expected, *word);
            }

            TEST(wali$nwa$query$$languageIsEmpty$and$getSomeAcceptedWord, testMiddleCall)
            {
                //               a              (a             a
                //  --> (state) ----> (state2) ----> (state3) ---> ((state4))
                NWA nwa;
                SomeElements e;
                State state4 = getKey("state4");
                    
                nwa.addInitialState(e.state);
                nwa.addInternalTrans(e.state, e.symbol, e.state2);
                nwa.addCallTrans(e.state2, e.symbol, e.state3);
                nwa.addInternalTrans(e.state3, e.symbol, state4);
                nwa.addFinalState(state4);
                
                EXPECT_FALSE(languageIsEmpty(nwa));

                NestedWord expected;
                expected.appendInternal(e.symbol);
                expected.appendCall(e.symbol);
                expected.appendInternal(e.symbol);

                NestedWordRefPtr word = getSomeAcceptedWord(nwa);
                
                EXPECT_EQ(expected, *word);
            }


            TEST(wali$nwa$query$$languageIsEmpty$and$getSomeAcceptedWord, testMiddleReturn)
            {
                //               a              a)             a
                //  --> (state) ----> (state2) ----> (state3) ---> ((state4))
                NWA nwa;
                SomeElements e;
                State state4 = getKey("state4");
                    
                nwa.addInitialState(e.state);
                nwa.addInternalTrans(e.state, e.symbol, e.state2);
                nwa.addReturnTrans(e.state2, e.state, e.symbol, e.state3);
                nwa.addInternalTrans(e.state3, e.symbol, state4);
                nwa.addFinalState(state4);
                
                EXPECT_FALSE(languageIsEmpty(nwa));

                NestedWord expected;
                expected.appendInternal(e.symbol);
                expected.appendReturn(e.symbol);
                expected.appendInternal(e.symbol);

                NestedWordRefPtr word = getSomeAcceptedWord(nwa);
                
                EXPECT_EQ(expected, *word);
            }


            TEST(wali$nwa$query$$getSomeAcceptedWord, testChooseCallOverInternal)
            {
                //               a              a(           a)/state2
                //  --> (state) ----> (state2) ----> (state3) ---> ((state4))
                //                             ---->
                //                              b

                // Why are we doing this? Because the current version of
                // getSomeAcceptedWord calls only getSymbol() for both internal and call
                // transitions. This will return 'b' for what needs to be a
                // call, but 'a b( a)' is not in the language.
                
                NWA nwa;
                SomeElements e;
                State state4 = getKey("state4");
                Symbol distractor = getKey("distractor");
                    
                nwa.addInitialState(e.state);
                nwa.addInternalTrans(e.state, e.symbol, e.state2);
                nwa.addCallTrans(e.state2, e.symbol, e.state3);
                nwa.addInternalTrans(e.state2, distractor, e.state3);
                nwa.addReturnTrans(e.state3, e.state2, e.symbol, state4);
                nwa.addFinalState(state4);
                
                EXPECT_FALSE(languageIsEmpty(nwa));

                NestedWord expected;
                expected.appendInternal(e.symbol);
                expected.appendCall(e.symbol);
                expected.appendReturn(e.symbol);

                ASSERT_TRUE(languageContains(nwa, expected));

                NestedWordRefPtr word = getSomeAcceptedWord(nwa);

                EXPECT_TRUE(languageContains(nwa, *word));
                EXPECT_EQ(expected, *word);
            }
        }
    }
}

