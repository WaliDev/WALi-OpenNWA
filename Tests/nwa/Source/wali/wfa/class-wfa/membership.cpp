#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"
#include "wali/ShortestPathSemiring.hpp"

#include "fixtures.hpp"

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

using namespace wali::wfa;

static const WFA fas[] = {
    LoopReject().wfa,
    LoopAccept().wfa,
    EvenAsEvenBs().wfa,
    EpsilonTransitionToAccepting().wfa,
    EpsilonDeterministic().wfa,
    EpsilonTransitionToMiddleToAccepting().wfa,
    ADeterministic().wfa,
    EpsilonTransitionToMiddleToEpsilonToAccepting().wfa,
    AcceptAbOrAcNondet().wfa,
    AEpsilonEpsilonEpsilonA().wfa
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
    { /* epsilon deterministic    */ true,  false, false, false, false, false, false },    
    { /* eps -> mid -> acc        */ false, true,  false, false, false, false, false },
    { /* A deterministic          */ false, true,  false, false, false, false, false },    
    { /* eps -> mid -> eps -> acc */ false, true,  false, false, false, false, false },
    { /* AB or AC nondet          */ false, false, false, false, false, true,  true  },
    { /* A eps eps eps A          */ false, false, false, false, true,  false, false }
};


namespace wali {
    namespace wfa {

#define EXPECT_CONTAINS(container, value) EXPECT_FALSE(container.end() == container.find(value))

        TEST(wali$wfa$$epsilonClose, EpsilonTransitionToMiddleToEpsilonToAccepting)
        {
            EpsilonTransitionToMiddleToEpsilonToAccepting fixture;

            Key start = getKey("start");
            Key middle = getKey("middle");
            Key almost = getKey("almost");
            Key accept = getKey("accept");

            WFA::AccessibleStateMap start_closure = fixture.wfa.epsilonClose(start);
            WFA::AccessibleStateMap middle_closure = fixture.wfa.epsilonClose(middle);
            WFA::AccessibleStateMap almost_closure = fixture.wfa.epsilonClose(almost);
            WFA::AccessibleStateMap accept_closure = fixture.wfa.epsilonClose(accept);

            WFA::AccessibleStateMap::const_iterator iter;

            EXPECT_EQ(2u, start_closure.size());
            EXPECT_CONTAINS(start_closure, start);
            EXPECT_CONTAINS(start_closure, middle);

            EXPECT_EQ(1u, middle_closure.size());
            EXPECT_CONTAINS(middle_closure, middle);

            EXPECT_EQ(2u, almost_closure.size());
            EXPECT_CONTAINS(almost_closure, almost);
            EXPECT_CONTAINS(almost_closure, accept);

            EXPECT_EQ(1u, accept_closure.size());
            EXPECT_CONTAINS(accept_closure, accept);
        }


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


        void check_shortest_distance_eq(unsigned expected, sem_elem_t actual_rp)
        {
            ShortestPathSemiring * actual =
                dynamic_cast<ShortestPathSemiring*>(actual_rp.get_ptr());

            ASSERT_TRUE(actual != NULL);
            EXPECT_EQ(expected, actual->getNum());
        }


        TEST(wali$wfa$$epsilonClose, weightFromOneEpsilonTrans)
        {
            //     eps(d2)
            // -->o-------->(o)
            sem_elem_t distance_zero = new ShortestPathSemiring(0);
            sem_elem_t distance_two = new ShortestPathSemiring(2);

            // Create the automaton
            Key start = getKey("start");
            Key accept = getKey("accept");

            WFA wfa;

            wfa.addState(start, distance_zero);
            wfa.addState(accept, distance_zero);

            wfa.setInitialState(start);
            wfa.addFinalState(accept);

            wfa.addTrans(start, WALI_EPSILON, accept, distance_two);

            // Create the queries
            WFA::AccessibleStateMap start_zero, start_ten;
            start_zero[start] = distance_zero;

            // Issue queries
            WFA::AccessibleStateMap end_from_zero = wfa.epsilonClose(start);

            // Check the answers
            EXPECT_EQ(2u, end_from_zero.size());
            ASSERT_TRUE(end_from_zero.find(start) != end_from_zero.end());
            ASSERT_TRUE(end_from_zero.find(accept) != end_from_zero.end());
            check_shortest_distance_eq(0u, end_from_zero[start]);
            check_shortest_distance_eq(2u, end_from_zero[accept]);
        }


        TEST(wali$wfa$$epsilonClose, DISABLED_weightFromTwoEpsilonTransInSequence)
        {
            //     eps(d1)   eps(d2)
            // -->o-------->o------->(o)
            sem_elem_t distance_zero = new ShortestPathSemiring(0);
            sem_elem_t distance_one = new ShortestPathSemiring(1);
            sem_elem_t distance_two = new ShortestPathSemiring(2);

            // Create the automaton
            Key start = getKey("start");
            Key middle = getKey("middle");
            Key accept = getKey("accept");

            WFA wfa;

            wfa.addState(start, distance_zero);
            wfa.addState(middle, distance_zero);
            wfa.addState(accept, distance_zero);

            wfa.setInitialState(start);
            wfa.addFinalState(accept);

            wfa.addTrans(start, WALI_EPSILON, middle, distance_one);
            wfa.addTrans(middle, WALI_EPSILON, accept, distance_two);

            // Create the queries
            WFA::AccessibleStateMap start_zero, start_ten;
            start_zero[start] = distance_zero;

            // Issue queries
            WFA::AccessibleStateMap end_from_zero = wfa.epsilonClose(start);

            // Check the answers
            EXPECT_EQ(3u, end_from_zero.size());
            ASSERT_TRUE(end_from_zero.find(start) != end_from_zero.end());
            ASSERT_TRUE(end_from_zero.find(middle) != end_from_zero.end());
            ASSERT_TRUE(end_from_zero.find(accept) != end_from_zero.end());
            check_shortest_distance_eq(0u, end_from_zero[start]);
            check_shortest_distance_eq(1u, end_from_zero[middle]);
            check_shortest_distance_eq(3u, end_from_zero[accept]);
        }

    }
}
