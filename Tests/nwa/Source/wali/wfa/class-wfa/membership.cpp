#include "gtest/gtest.h"

#include "wali/wfa/WFA.hpp"

#include "fixtures.hpp"

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

using namespace wali::wfa;

static const WFA fas[] = {
    LoopReject().wfa,
    LoopAccept().wfa,
    EvenAsEvenBs().wfa,
    EpsilonTransitionToAccepting().wfa,
    EpsilonTransitionToMiddleToAccepting().wfa,
    EpsilonTransitionToMiddleToEpsilonToAccepting().wfa,
    AcceptAbOrAcNondet().wfa
};

Words ws;

static const WFA::Word words[] = {
    ws.epsilon,
    ws.a,
    ws.b,
    ws.c,
    ws.aa,
    ws.ab,
    ws.ac
};

static const unsigned num_fas = NUM_ELEMENTS(fas);
static const unsigned num_words = NUM_ELEMENTS(words);

static const bool answers[num_fas][num_words] = {
    //                               eps    a      b      c      aa     ab     ac
    { /* loop reject              */ false, false, false, false, false, false, false },
    { /* loop accept              */ true,  true,  true,  true,  true,  true,  true  },
    { /* even As, even Bs         */ true,  false, false, false, true,  false, false },
    { /* eps -> accept            */ true,  false, false, false, false, false, false },
    { /* eps -> mid -> acc        */ false, true,  false, false, false, false, false },
    { /* eps -> mid -> eps -> acc */ false, true,  false, false, false, false, false },
    { /* AB or AC nondet          */ false, false, false, false, false, true,  true  }
};


namespace wali {
    namespace wfa {


        TEST(wali$wfa$$isAcceptedWithNonzeroWeight, testBatteryOfVariousFas)
        {
            for (unsigned fa = 0 ; fa < num_fas ; ++fa) {
                for (unsigned word = 0 ; word < num_words ; ++word) {
                    bool expected = answers[fa][word];
                    bool actual = fas[fa].isAcceptedWithNonzeroWeight(words[word]);

                    std::stringstream ss;
                    ss << "Testing FA #" << fa << " with string #" << word;
                    SCOPED_TRACE(ss.str());
                    
                    EXPECT_EQ(expected, actual);
                }
            }
        }


    }
}
