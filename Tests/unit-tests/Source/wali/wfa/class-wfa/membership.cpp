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
    EpsilonFull().wfa,
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


        TEST(wali$wfa$$epsilonClose, weightFromTwoEpsilonTransInSequence)
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

        TEST(wali$wfa$$epsilonClose, weightOnDiamond)
        {
            // The goal of this test is to make sure that:
            //
            //   1. The weight on C is set to 2, not 11, despite 11
            //      (hopefully) being encountered first
            //
            //   2. The weight on E is set to 3, not 12, despite E(12)
            //      (hopefully) being encountered before even C(2). This
            //      makes sure that C was reinserted onto the worklist when
            //      we saw that the weight changed.
            //
            //  All transitions are epsilon; given are weights
            //         1       10         1
            // -->A-------->B-------->C------->E
            //    |                   /\                [anti-multi-line comment]
            //    |                   |
            //    \-------->D---------/
            //        1        1
            sem_elem_t distance_zero = new ShortestPathSemiring(0);
            sem_elem_t distance_one = new ShortestPathSemiring(1);
            sem_elem_t distance_two = new ShortestPathSemiring(2);
            sem_elem_t distance_three = new ShortestPathSemiring(3);
            sem_elem_t distance_ten = new ShortestPathSemiring(10);

            // Create the automaton
            Key A = getKey("A");
            Key B = getKey("B");
            Key C = getKey("C");
            Key D = getKey("D");
            Key E = getKey("E");

            if (C < B) {
                // Makes "sure" that we pull B off the worklist before C
                std::swap(C, B);
            }
            if (D < C) {
                std::swap(D, C);
            }

            
            WFA wfa;

            wfa.addState(A, distance_zero);
            wfa.addState(B, distance_zero);
            wfa.addState(C, distance_zero);
            wfa.addState(D, distance_zero);
            wfa.addState(E, distance_zero);

            wfa.setInitialState(A);
            wfa.addFinalState(E);

            wfa.addTrans(A, WALI_EPSILON, B, distance_one);
            wfa.addTrans(A, WALI_EPSILON, D, distance_one);            
            wfa.addTrans(B, WALI_EPSILON, C, distance_ten);
            wfa.addTrans(D, WALI_EPSILON, C, distance_one);
            wfa.addTrans(C, WALI_EPSILON, E, distance_one);

            // Create the queries
            WFA::AccessibleStateMap start_zero;
            start_zero[A] = distance_zero;

            // Issue queries
            WFA::AccessibleStateMap end_from_zero = wfa.epsilonClose(A);

            // Check the answers
            EXPECT_EQ(5u, end_from_zero.size());
            ASSERT_TRUE(end_from_zero.find(A) != end_from_zero.end());
            ASSERT_TRUE(end_from_zero.find(B) != end_from_zero.end());
            ASSERT_TRUE(end_from_zero.find(C) != end_from_zero.end());
            ASSERT_TRUE(end_from_zero.find(D) != end_from_zero.end());
            ASSERT_TRUE(end_from_zero.find(E) != end_from_zero.end());

            check_shortest_distance_eq(0u, end_from_zero[A]);
            check_shortest_distance_eq(1u, end_from_zero[B]);
            check_shortest_distance_eq(2u, end_from_zero[C]);
            check_shortest_distance_eq(1u, end_from_zero[D]);
            check_shortest_distance_eq(3u, end_from_zero[E]);
        }


    }
}
