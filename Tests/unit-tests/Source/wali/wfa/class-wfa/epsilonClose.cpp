#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"
#include "wali/ShortestPathSemiring.hpp"
#include "wali/LongestSaturatingPathSemiring.hpp"
#include "wali/domains/binrel/ProgramBddContext.hpp"
#include "wali/domains/binrel/BinRel.hpp"

#include "fixtures.hpp"

namespace wali {
    namespace wfa {

        std::string
        clamp(std::string const & str)
        {
            const size_t maxlen = 50;
            if (str.size() < maxlen) {
                return str;
            }
            else {
                return str.substr(0, maxlen-3) + "...";
            }
        }
            

        ::testing::AssertionResult
        assert_accessibleStateMaps_equal(char const * left_expr,
                                         char const * right_expr,
                                         WFA::AccessibleStateMap const & left,
                                         WFA::AccessibleStateMap const & right)
        {
            if (left.size() != right.size()) {
                return ::testing::AssertionFailure()
                    << "Epsilon closure results are different sizes:\n"
                    << "    " << left_expr << " has " << left.size() << " states\n"
                    << "    " << right_expr << " has " << right.size() << " states";
            }

            for (WFA::AccessibleStateMap::const_iterator
                     left_entry = left.begin(),
                     right_entry = right.begin();
                 left_entry != left.end();
                 ++left_entry, ++right_entry)
            {
                if (left_entry->first != right_entry->first) {
                    assert(right.find(left_entry->first) == right.end());
                    
                    return ::testing::AssertionFailure()
                        << "Epsilon closure results have a different set of states;\n"
                        << "    " << left_expr << " contains state " << key2str(left_entry->first) << " (" << left_entry->first << ")\n"
                        << "    while " << right_expr << " does not";
                }

                if (!left_entry->second->equal(right_entry->second)) {
                    std::string
                        left_weight_str = clamp(left_entry->second->to_string()),
                        right_weight_str = clamp(right_entry->second->to_string());
                    
                    return ::testing::AssertionFailure()
                        << "Epsilon closure results have a different weight for a state;\n"
                        << "    the state is " << key2str(left_entry->first) << " (" << left_entry->first << ")\n"
                        << "    which for " << left_expr << " has weight " << left_weight_str << "\n"
                        << "    and for " << right_expr << " has weight " << right_weight_str;
                }
            }
            
            return ::testing::AssertionSuccess();
        }
        

        WFA::AccessibleStateMap
        checkedEpsilonClose(WFA const & wfa, Key state)
        {
            WFA::AccessibleStateMap
                default_close = wfa.epsilonClose(state),
                mohri_close = wfa.epsilonClose_Mohri(state),
                fwpds_close = wfa.epsilonClose_Fwpds(state);

            EXPECT_PRED_FORMAT2(assert_accessibleStateMaps_equal, default_close, mohri_close);
            EXPECT_PRED_FORMAT2(assert_accessibleStateMaps_equal, default_close, fwpds_close);

            return default_close;
        }



        ::testing::AssertionResult
        assert_epsilonCloseCache_equal(char const * left_expr,
                                       char const * right_expr,
                                       WFA::EpsilonCloseCache const & left,
                                       WFA::EpsilonCloseCache const & right)
        {
            if (left.size() != right.size()) {
                return ::testing::AssertionFailure()
                    << "The two expressions below have epsilon closure information for a differing number of states\n"
                    << "    " << left_expr << " reports e-closures for " << left.size() << " states\n"
                    << "    " << right_expr << " reports e-closures for " << right.size() << " states";
            }

            for (WFA::EpsilonCloseCache::const_iterator
                     left_entry = left.begin(),
                     right_entry = right.begin();
                 left_entry != left.end();
                 ++left_entry, ++right_entry)
            {
                if (left_entry->first != right_entry->first) {
                    if (right.find(left_entry->first) == right.end()) {                   
                        return ::testing::AssertionFailure()
                            << "The two expressions below report epsilon closure information for a differing set of states\n"
                            << "    " << left_expr << " reports an e-close for state " << key2str(left_entry->first) << " (" << left_entry->first << ")\n"
                            << "    while " << right_expr << " does not";
                    }
                    else {
                        assert(left.find(right_entry->first) == left.end());
                        return ::testing::AssertionFailure()
                            << "The two expressions below report epsilon closure information for a differing set of states\n"
                            << "    " << left_expr << " does NOT report an e-close for state " << key2str(left_entry->first) << " (" << left_entry->first << ")\n"
                            << "    while " << right_expr << " DOES";
                    }
                }

                std::stringstream left_ss, right_ss;

                left_ss << left_expr << " [from " << key2str(left_entry->first) << " (" << left_entry->first << ")]";
                right_ss << right_expr << " [from " << key2str(right_entry->first) << " (" << right_entry->first << ")]";
                
                std::string
                    full_left_expr = left_ss.str(),
                    full_right_expr = right_ss.str();

                ::testing::AssertionResult result_closures =
                      assert_accessibleStateMaps_equal(full_left_expr.c_str(),
                                                       full_right_expr.c_str(),
                                                       left_entry->second,
                                                       right_entry->second);

                if (!result_closures) {
                    return ::testing::AssertionFailure()
                        << "The epsilon closures differ for a state:\n"
                        << result_closures.message();
                }
            }
            
            return ::testing::AssertionSuccess();
        }


        ::testing::AssertionResult
        check_all_source_epsilon_closure(char const * expr, WFA const & wfa)
        {
            WFA::EpsilonCloseCache mohri_closures, fwpds_singles_closures, fwpds_multi_closures;

            wfa.epsilonCloseCached_MohriAll(wfa.getInitialState(), mohri_closures);
            wfa.epsilonCloseCached_FwpdsAllSingles(wfa.getInitialState(), fwpds_singles_closures);
            wfa.epsilonCloseCached_FwpdsAllMulti(wfa.getInitialState(), fwpds_multi_closures);

            ::testing::AssertionResult
                  eq12 = assert_epsilonCloseCache_equal("mohri_closures",
                                                        "fwpds_singles_closures",
                                                        mohri_closures,
                                                        fwpds_singles_closures),
                  eq13 = assert_epsilonCloseCache_equal("mohri_closures",
                                                        "fwpds_multi_closures",
                                                        mohri_closures,
                                                        fwpds_multi_closures);

            if (!eq12) {
                return eq12;
            }
            else {
                return eq13;
            }
        }

#define EXPECT_CONSISTENT_EPSILON_CLOSURES(wfa) \
        EXPECT_PRED_FORMAT1(check_all_source_epsilon_closure, wfa)
        

#define EXPECT_CONTAINS(container, value) EXPECT_FALSE(container.end() == container.find(value))

        TEST(wali$wfa$$epsilonClose, EpsilonTransitionToMiddleToEpsilonToAccepting)
        {
            EpsilonTransitionToMiddleToEpsilonToAccepting fixture;

            Key start = getKey("start");
            Key middle = getKey("middle");
            Key almost = getKey("almost");
            Key accept = getKey("accept");

            WFA::AccessibleStateMap start_closure = checkedEpsilonClose(fixture.wfa, start);
            WFA::AccessibleStateMap middle_closure = checkedEpsilonClose(fixture.wfa, middle);
            WFA::AccessibleStateMap almost_closure = checkedEpsilonClose(fixture.wfa, almost);
            WFA::AccessibleStateMap accept_closure = checkedEpsilonClose(fixture.wfa, accept);

            EXPECT_CONSISTENT_EPSILON_CLOSURES(fixture.wfa);            

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
            WFA::AccessibleStateMap end_from_zero = checkedEpsilonClose(wfa, start);
            EXPECT_CONSISTENT_EPSILON_CLOSURES(wfa);
            
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
            WFA::AccessibleStateMap end_from_zero = checkedEpsilonClose(wfa, start);
            EXPECT_CONSISTENT_EPSILON_CLOSURES(wfa);

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
            WFA::AccessibleStateMap end_from_zero = checkedEpsilonClose(wfa, A);
            EXPECT_CONSISTENT_EPSILON_CLOSURES(wfa);

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


        TEST(wali$wfa$$epsilonClose, epsilonCycleKeepsIterating)
        {
            //        eps
            //   A <------> B
            Key A = getKey("A");
            Key B = getKey("B");

            sem_elem_t distance_one = new LongestSaturatingPathSemiring(1, 10);
            test_semelem_impl(distance_one);

            WFA wfa;
            wfa.addState(A, distance_one);
            wfa.addState(B, distance_one);

            wfa.setInitialState(A);

            wfa.addTrans(A, WALI_EPSILON, B, distance_one);
            wfa.addTrans(B, WALI_EPSILON, A, distance_one);

            WFA::AccessibleStateMap accessible = checkedEpsilonClose(wfa, A);
            EXPECT_CONSISTENT_EPSILON_CLOSURES(wfa);

            EXPECT_EQ(2u, accessible.size());
            EXPECT_NE(accessible.find(A), accessible.end());
            EXPECT_NE(accessible.find(B), accessible.end());

            LongestSaturatingPathSemiring
                * w_a = dynamic_cast<LongestSaturatingPathSemiring*>(accessible[A].get_ptr()),
                * w_b = dynamic_cast<LongestSaturatingPathSemiring*>(accessible[B].get_ptr());

            ASSERT_TRUE(w_a != 0);
            ASSERT_TRUE(w_b != 0);

            EXPECT_EQ(10u, w_a->getNum()); // 0 max (1+1) max (1+1+1+1) max ...max 10
            EXPECT_EQ(10u, w_b->getNum());  // 1 max (1+1+1) max (1+1+1+1+1) max ... max 10
        }



        TEST(wali$wfa$$epsilonClose, closureExtendsDoneInCorrectOrder)
        {
            using namespace wali::domains::binrel;
            
            // I need something that is non-commutative.
            ProgramBddContext voc;
            voc.addIntVar("x", 4);

            // Make the weights:
            //   to_zero * havoc   = havoc
            //   havoc   * to_zero = to_zero
            sem_elem_t to_zero = new BinRel(&voc, voc.Assign("x", voc.Const(0)));
            sem_elem_t havoc = new BinRel(&voc, voc.True());

            // The following is needed because of some extra stuff that
            // Prathmesh does that I don't understand.
            //
            // to_zero starts out as:  <x':0>
            // havoc starts out as:    <__regSize:2, __regA':1>
            // thus havoc*to_zero is:  <x':0, __regSize:2, __regA':1>
            //
            // The following line deals with those extra regSize/regA things
            // so that they are present in to_zero as well.
            to_zero = havoc->extend(to_zero);

            ASSERT_TRUE(to_zero->equal(havoc->extend(to_zero)));
            ASSERT_TRUE(!havoc->equal(havoc->extend(to_zero)));

            // Make the WFA
            //    eps          eps
            //  A ---------> B ------> C
            //    [havoc x]    [x:=0]
            WFA wfa;
            Key A = getKey("A");
            Key B = getKey("B");
            Key C = getKey("C");

            wfa.addState(A, havoc->zero());
            wfa.addState(B, havoc->zero());
            wfa.addState(C, havoc->zero());
            wfa.setInitialState(A);

            wfa.addTrans(A, WALI_EPSILON, B, havoc);
            wfa.addTrans(B, WALI_EPSILON, C, to_zero);

            // A should be reachable with weight one
            // B should be reachable with weight [havoc x]
            // C should be reachable with weight ([havoc x] * [x:=0]) = [x:=0]
            WFA::AccessibleStateMap accessible = checkedEpsilonClose(wfa, A);
            EXPECT_CONSISTENT_EPSILON_CLOSURES(wfa);

            EXPECT_EQ(3u, accessible.size());
            EXPECT_NE(accessible.find(A), accessible.end());
            EXPECT_NE(accessible.find(B), accessible.end());
            EXPECT_NE(accessible.find(C), accessible.end());

#if 0
            wfa.print(std::cout);
            std::cout << "C weight: ";
            accessible[C]->print(std::cout);
#endif

            EXPECT_TRUE(to_zero->one()->equal(accessible[A]));
            EXPECT_TRUE(havoc->equal(accessible[B]));
            EXPECT_TRUE(to_zero->equal(accessible[C]));
        }

        TEST(wali$wfa$$removeEpsilons, epsilonCycleTerminates)
        {
            //        eps
            //   A <------> B
            Key A = getKey("A");
            Key B = getKey("B");

            sem_elem_t one = Reach(true).one();

            WFA wfa;
            wfa.addState(A, one);
            wfa.addState(B, one);

            wfa.setInitialState(A);

            wfa.addTrans(A, WALI_EPSILON, B, one);
            wfa.addTrans(B, WALI_EPSILON, A, one);

            WFA no_eps = wfa.removeEpsilons();
        }


        TEST(wali$wfa$$epsilonClose, unreachableNodes)
        {
            //      
            //  -> A     (B)
            Key A = getKey("A");
            Key B = getKey("B");

            sem_elem_t one = Reach(true).one();

            WFA wfa;
            wfa.addState(A, one);
            wfa.addState(B, one);

            wfa.setInitialState(A);
            wfa.addFinalState(B);

            EXPECT_CONSISTENT_EPSILON_CLOSURES(wfa);

            WFA::AccessibleStateMap eclose_A = checkedEpsilonClose(wfa, A);
            WFA::AccessibleStateMap eclose_B = checkedEpsilonClose(wfa, B);
            
            EXPECT_EQ(1u, eclose_A.size());
            EXPECT_EQ(1u, eclose_B.size());
            
            EXPECT_CONTAINS(eclose_A, A);
            EXPECT_CONTAINS(eclose_B, B);
        }

        
    }
}
