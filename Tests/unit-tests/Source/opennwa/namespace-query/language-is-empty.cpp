#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/language.hpp"

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




namespace opennwa {
        namespace query {
            

            TEST(opennwa$query$$languageIsEmpty, testBatteryOfVariouslyBalancedNwas)
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

            
            TEST(opennwa$query$$getSomeAcceptedWord, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned nwa = 0 ; nwa < num_nwas ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());

                    NestedWordRefPtr word = getSomeAcceptedWord(nwas[nwa]);
                    NestedWordRefPtr shortest_word = getSomeShortestAcceptedWord(nwas[nwa]);
                    
                    if (expected_answers[nwa]) {
                        EXPECT_TRUE(word == NULL);
                        EXPECT_TRUE(shortest_word == NULL);
                    }
                    else {
                        ASSERT_TRUE(word != NULL);
                        ASSERT_TRUE(shortest_word != NULL);
                        EXPECT_TRUE(languageContains(nwas[nwa], *word));
                        EXPECT_TRUE(languageContains(nwas[nwa], *shortest_word));
                    }
                }
            }

            
            TEST(opennwa$query$$languageIsEmpty$and$getSomeAcceptedWord, testEpsilonNwa)
            {
                Nwa nwa;
                SomeElements e;
                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state);

                EXPECT_FALSE(languageIsEmpty(nwa));

                NestedWord eps;

                NestedWordRefPtr word = getSomeAcceptedWord(nwa);
                NestedWordRefPtr shortest_word = getSomeShortestAcceptedWord(nwa);
                
                EXPECT_EQ(eps, *word);
                EXPECT_EQ(eps, *shortest_word);
            }
            
            TEST(opennwa$query$$languageIsEmpty$and$getSomeAcceptedWord, testInternalOnlyNwa)
            {
                //               a              a              a
                //  --> (state) ----> (state2) ----> (state3) ---> ((state4))
                Nwa nwa;
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
                NestedWordRefPtr shortest_word = getSomeShortestAcceptedWord(nwa);
                
                EXPECT_EQ(expected, *word);
                EXPECT_EQ(expected, *shortest_word);
            }

            TEST(opennwa$query$$languageIsEmpty$and$getSomeAcceptedWord, testMiddleCall)
            {
                //               a              (a             a
                //  --> (state) ----> (state2) ----> (state3) ---> ((state4))
                Nwa nwa;
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
                NestedWordRefPtr shortest_word = getSomeShortestAcceptedWord(nwa);
                
                EXPECT_EQ(expected, *word);
                EXPECT_EQ(expected, *shortest_word);
            }


            TEST(opennwa$query$$languageIsEmpty$and$getSomeAcceptedWord, testMiddleReturn)
            {
                //               a              a)             a
                //  --> (state) ----> (state2) ----> (state3) ---> ((state4))
                Nwa nwa;
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
                NestedWordRefPtr shortest_word = getSomeShortestAcceptedWord(nwa);
                
                EXPECT_EQ(expected, *word);
                EXPECT_EQ(expected, *shortest_word);
            }


            TEST(opennwa$query$$getSomeAcceptedWord, testChooseCallOverInternal)
            {
                //               a              a(           a)/state2
                //  --> (state) ----> (state2) ----> (state3) ---> ((state4))
                //                             ---->
                //                              b

                // Why are we doing this? Because the current version of
                // getSomeAcceptedWord calls only getSymbol() for both internal and call
                // transitions. This will return 'b' for what needs to be a
                // call, but 'a b( a)' is not in the language.
                
                Nwa nwa;
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
                NestedWordRefPtr shortest_word = getSomeShortestAcceptedWord(nwa);

                EXPECT_TRUE(languageContains(nwa, *word));
                EXPECT_TRUE(languageContains(nwa, *shortest_word));

                EXPECT_EQ(expected, *word);
                EXPECT_EQ(expected, *shortest_word);
            }


            TEST(opennwa$query$$getSomeAcceptedWord, findsShortest)
            {
                //              a            a
                // --> (state) --> (state2) --> (state3)
                //    b |                        /\ .
                //      V                        | b  (both directions to facilitate
                //     (state4) --> (state5) <---+    switching whether top or bottom
                //                                    path is shorter by changing accepting state)

                Nwa nwa;
                SomeElements e;
                State state4 = getKey("state4");
                State state5 = getKey("state5");
                Symbol b = getKey("b");

                nwa.addInitialState(e.state);
                nwa.addFinalState(state5);

                // Top path
                nwa.addInternalTrans(e.state, e.symbol, e.state2);
                nwa.addInternalTrans(e.state2, e.symbol, e.state3);
                nwa.addInternalTrans(e.state3, e.symbol, state5);

                // Bottom path
                nwa.addInternalTrans(e.state, b, state4);
                nwa.addInternalTrans(state4, b, state5);
                nwa.addInternalTrans(state5, b, e.state3);

                ReachGen rg;
                
                NestedWordRefPtr long_word = getSomeAcceptedWord(nwa);
                NestedWordRefPtr short_word = getSomeShortestAcceptedWord(nwa);

                // This could fail if the iteration order or something
                // changes and the rg version also returns the shortest
                // word. Such is life.
                //EXPECT_LT(short_word->size(), long_word->size());
                EXPECT_EQ(short_word->size(), 2u);
            }


            TEST(opennwa$query$$getSomeShortestAcceptedWord, findsShortestWordNotPath)
            {
                //              a            a
                // --> (state) --> (state2) --> (state3)
                //  eps |                        /\ .
                //      V                        | b
                //     (state4) --> (state5) ----+
                //             eps
                //
                // Note that the lower path is longer but the word is shorter

                Nwa nwa;
                SomeElements e;
                State state4 = getKey("state4");
                State state5 = getKey("state5");
                Symbol b = getKey("b");

                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state3);

                // Top path
                nwa.addInternalTrans(e.state, e.symbol, e.state2);
                nwa.addInternalTrans(e.state2, e.symbol, e.state3);

                // Bottom path
                nwa.addInternalTrans(e.state, EPSILON, state4);
                nwa.addInternalTrans(state4, EPSILON, state5);
                nwa.addInternalTrans(state5, b, e.state3);

                NestedWordRefPtr word = getSomeShortestAcceptedWord(nwa);

                EXPECT_EQ(1u, word->size());

                NestedWord expected;
                expected.appendInternal(b);

                EXPECT_EQ(expected, *word);
            }
            
    }
}

