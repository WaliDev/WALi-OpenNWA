#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"
#include "wali/ShortestPathSemiring.hpp"
#include "wali/LongestSaturatingPathSemiring.hpp"
#include "wali/domains/binrel/ProgramBddContext.hpp"
#include "wali/domains/binrel/BinRel.hpp"
#include "wali/domains/SemElemSet.hpp"

#include "fixtures.hpp"

using wali::domains::SemElemSet;
using wali::sem_elem_t;

namespace {
    bool
    is_present(SemElemSet::ElementSet const & set, sem_elem_t goal)
    {
        for (SemElemSet::ElementSet::const_iterator candidate = set.begin();
             candidate != set.end(); ++candidate)
        {
            if (goal->equal(*candidate)) {
                return true;
            }
        }
        return false;
    }


    void insert(SemElemSet::ElementSet & set, sem_elem_t element)
    {
        //set.push_back(element);
        set.insert(element);
    }
    

    SemElemSet::ElementSet
    make_vector(sem_elem_t a = sem_elem_t(), sem_elem_t b = sem_elem_t(),
                sem_elem_t c = sem_elem_t(), sem_elem_t d = sem_elem_t(),
                sem_elem_t e = sem_elem_t(), sem_elem_t f = sem_elem_t())
    {
        SemElemSet::ElementSet vec;
        if (a.get_ptr()) insert(vec, a);
        if (b.get_ptr()) insert(vec, b);
        if (c.get_ptr()) insert(vec, c);
        if (d.get_ptr()) insert(vec, d);
        if (e.get_ptr()) insert(vec, e);
        if (f.get_ptr()) insert(vec, f);
        return vec;
    }
    
}    


namespace wali {
    namespace wfa {

        ::testing::AssertionResult
        accessibleStateSetMap_equal(char const * left_expr,
                                    char const * right_expr,
                                    WFA::AccessibleStateSetMap const & left,
                                    WFA::AccessibleStateSetMap const & right)
        {
            if (left.size() != right.size()) {
                return ::testing::AssertionFailure()
                    << "Reaching weight results are different sizes:\n"
                    << "    " << left_expr << " reaches " << left.size() << " states\n"
                    << "    " << right_expr << " reaches " << right.size() << " states\n";
            }

            for (WFA::AccessibleStateSetMap::const_iterator
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

                if (left_entry->second.size() != right_entry->second.size()) {
                    return ::testing::AssertionFailure()
                        << "The number of weights reachable at state " << key2str(left_entry->first) << " differs:\n"
                        << "    in " << left_expr << ", " << left_entry->second.size() << " weights are reachable\n"
                        << "    in " << right_expr << ", " << right_entry->second.size() << " weights are reachable\n";
                }

                for (SemElemSet::ElementSet::const_iterator left_weight = left_entry->second.begin();
                     left_weight != left_entry->second.end(); ++left_weight)
                {
                    if (!is_present(right_entry->second, *left_weight)) {
                        return ::testing::AssertionFailure()
                            << "A weight reachable in the first set is not reachable in the second:\n"
                            << "    at state " << key2str(left_entry->first) << "\n"
                            << "    the weight " << (*left_weight)->toString() << "\n"
                            << "    is reachable in " << left_expr << "\n"
                            << "    but not in " << right_expr << "\n";
                    }
                }
            }

            return ::testing::AssertionSuccess();
        }
   

#define EXPECT_EQ_REACHABLE(a, b) \
        EXPECT_PRED_FORMAT2(accessibleStateSetMap_equal, a, b)

        
        TEST(wali$wfa$WFA$$computeAllReachingWeights, oneStateAutomatonReachedWithWeightOne)
        {
            Key start = getKey("start");
            sem_elem_t zero = new ShortestPathSemiring(0);
            zero = zero->zero(); // make the name correct

            WFA wfa;
            wfa.addState(start, zero);
            wfa.setInitialState(start);

            WFA::AccessibleStateSetMap expected;
            expected[start] = make_vector(zero->one());

            WFA::AccessibleStateSetMap reachable = wfa.computeAllReachingWeights();

            EXPECT_EQ_REACHABLE(expected, reachable);
        }


        TEST(wali$wfa$WFA$$computeAllReachingWeights, unreachableStatesAreUnreachable)
        {
            Key start = getKey("start"), other = getKey("other");
            sem_elem_t zero = new ShortestPathSemiring(0);
            zero = zero->zero(); // make the name correct

            WFA wfa;
            wfa.addState(start, zero);
            wfa.addState(other, zero);
            wfa.setInitialState(start);

            WFA::AccessibleStateSetMap expected;
            expected[start] = make_vector(zero->one());

            WFA::AccessibleStateSetMap reachable = wfa.computeAllReachingWeights();

            EXPECT_EQ_REACHABLE(expected, reachable);
        }


        TEST(wali$wfa$WFA$$computeAllReachingWeights, statesReachableWithWeightZeroAreReachableWithWeightZero)
        {
            Key start = getKey("start"), other = getKey("other"), sym = getKey("sym");
            sem_elem_t zero = new ShortestPathSemiring(0);
            zero = zero->zero(); // make the name correct

            WFA wfa;
            wfa.addState(start, zero);
            wfa.addState(other, zero);
            wfa.setInitialState(start);
            wfa.addTrans(start, sym, other, zero);

            WFA::AccessibleStateSetMap expected;
            expected[start] = make_vector(zero->one());
            expected[other] = make_vector(zero);

            WFA::AccessibleStateSetMap reachable = wfa.computeAllReachingWeights();

            EXPECT_EQ_REACHABLE(expected, reachable);
        }


        TEST(wali$wfa$WFA$$computeAllReachingWeights, singleTransitionToOther)
        {
            Key start = getKey("start"), other = getKey("other"), sym = getKey("sym");
            sem_elem_t zero = new ShortestPathSemiring(0);
            sem_elem_t distance_one = new ShortestPathSemiring(1);
            zero = zero->zero(); // make the name correct

            WFA wfa;
            wfa.addState(start, zero);
            wfa.addState(other, zero);
            wfa.setInitialState(start);
            wfa.addTrans(start, sym, other, distance_one);

            WFA::AccessibleStateSetMap expected;
            expected[start] = make_vector(zero->one());
            expected[other] = make_vector(distance_one);

            WFA::AccessibleStateSetMap reachable = wfa.computeAllReachingWeights();

            EXPECT_EQ_REACHABLE(expected, reachable);
        }


        TEST(wali$wfa$WFA$$computeAllReachingWeights, veeThenCombine)
        {
            Key start = getKey("start"), other = getKey("other"), final = getKey("final"), sym = getKey("sym");
            sem_elem_t zero = new ShortestPathSemiring(0);
            sem_elem_t distance_one = new ShortestPathSemiring(1);
            sem_elem_t distance_two = new ShortestPathSemiring(2);
            zero = zero->zero(); // make the name correct

            //   start ------------------------> final
            //     |                              /\         [anti multi-line comment]
            //     +-----------> other -----------+
            WFA wfa;
            wfa.addState(start, zero);
            wfa.addState(other, zero);
            wfa.setInitialState(start);
            wfa.addTrans(start, sym, final, distance_one);
            wfa.addTrans(start, sym, other, distance_one);
            wfa.addTrans(other, sym, final, distance_one);

            WFA::AccessibleStateSetMap expected;
            expected[start] = make_vector(zero->one());
            expected[other] = make_vector(distance_one);
            expected[final] = make_vector(distance_one, distance_two);

            WFA::AccessibleStateSetMap reachable = wfa.computeAllReachingWeights();

            EXPECT_EQ_REACHABLE(expected, reachable);
        }


        TEST(wali$wfa$WFA$$computeAllReachingWeights, selfLoop)
        {
            Key start = getKey("start"), other = getKey("other"), final = getKey("final"), sym = getKey("sym");
            sem_elem_t zero = new LongestSaturatingPathSemiring(0, 5); zero = zero->zero(); // make the name correct
            sem_elem_t distance_one = new LongestSaturatingPathSemiring(1, 5);
            sem_elem_t distance_two = new LongestSaturatingPathSemiring(2, 5);
            sem_elem_t distance_three = new LongestSaturatingPathSemiring(3, 5);
            sem_elem_t distance_four = new LongestSaturatingPathSemiring(4, 5);
            sem_elem_t distance_five = new LongestSaturatingPathSemiring(5, 5);

            //    {0}     1     {1,3,5}    1     {2,4,5}
            //   start --------> other --------> final
            //                   /   \                     [anti line-continuation]
            //                   \___/
            WFA wfa;
            wfa.addState(start, zero);
            wfa.addState(other, zero);
            wfa.addState(final, zero);
            wfa.setInitialState(start);
            wfa.addTrans(start, sym, other, distance_one);
            wfa.addTrans(other, sym, other, distance_two);
            wfa.addTrans(other, sym, final, distance_one);

            WFA::AccessibleStateSetMap expected;
            expected[start] = make_vector(zero->one());
            expected[other] = make_vector(distance_one, distance_three, distance_five);
            expected[final] = make_vector(distance_two, distance_four, distance_five);

            WFA::AccessibleStateSetMap reachable = wfa.computeAllReachingWeights();

            EXPECT_EQ_REACHABLE(expected, reachable);
        }


        TEST(wali$wfa$WFA$$computeAllReachingWeights, cycle)
        {
            Key start = getKey("start"), other = getKey("other"), final = getKey("final"), sym = getKey("sym");
            sem_elem_t distance_one = new LongestSaturatingPathSemiring(1, 5);
            sem_elem_t distance_two = new LongestSaturatingPathSemiring(2, 5);
            sem_elem_t distance_three = new LongestSaturatingPathSemiring(3, 5);
            sem_elem_t distance_four = new LongestSaturatingPathSemiring(4, 5);
            sem_elem_t distance_five = new LongestSaturatingPathSemiring(5, 5);
            sem_elem_t zero = distance_one->zero();

            //         <----------------
            //   start ----------------> final
            WFA wfa;
            wfa.addState(start, zero);
            wfa.addState(other, zero);
            wfa.setInitialState(start);
            wfa.addTrans(start, sym, final, distance_one);
            wfa.addTrans(final, sym, start, distance_one);

            WFA::AccessibleStateSetMap expected;
            expected[start] = make_vector(zero->one(), distance_two, distance_four, distance_five);
            expected[final] = make_vector(distance_one, distance_three, distance_five);

            WFA::AccessibleStateSetMap reachable = wfa.computeAllReachingWeights();

            EXPECT_EQ_REACHABLE(expected, reachable);
        }


        TEST(wali$wfa$WFA$$computeAllReachingWeights, biggerExample)
        {
            Key start = getKey("start"),
                loop1_head = getKey("loop1_head"),
                loop2_head = getKey("loop2_head"),
                inside = getKey("inside"),
                final = getKey("final"),
                sym = getKey("sym");

            sem_elem_t
                distance_zero  = new LongestSaturatingPathSemiring(0, 10),
                distance_one   = new LongestSaturatingPathSemiring(1, 10),
                distance_two   = new LongestSaturatingPathSemiring(2, 10),
                distance_three = new LongestSaturatingPathSemiring(3, 10),
                distance_four  = new LongestSaturatingPathSemiring(4, 10),
                distance_five  = new LongestSaturatingPathSemiring(5, 10),
                distance_six   = new LongestSaturatingPathSemiring(6, 10),
                distance_seven = new LongestSaturatingPathSemiring(7, 10),
                distance_eight = new LongestSaturatingPathSemiring(8, 10),
                distance_nine  = new LongestSaturatingPathSemiring(9, 10),
                distance_ten   = new LongestSaturatingPathSemiring(10, 10),
                zero = distance_one->zero();
            
            //            0     {0,4,8,7,10}       1
            //   start --------> loop1_head -----------> final
            //                    |      /\                          [anti line-continuation]
            //                   2|       |2
            //                    \/      |
            //                loop2_head--+   {2,5,6,8,9,10}
            //                 |     /\                              [anti line-continuation]
            //                1|     |2
            //                 \/    |
            //                  inside  {3,6,7,9,10}
            WFA wfa;
            wfa.addState(start, zero);
            wfa.addState(loop1_head, zero);
            wfa.addState(loop2_head, zero);
            wfa.addState(inside, zero);
            wfa.addState(final, zero);            
            wfa.setInitialState(start);
            wfa.addTrans(start,      sym, loop1_head, distance_zero);
            wfa.addTrans(loop1_head, sym, loop2_head, distance_two);
            wfa.addTrans(loop2_head, sym, inside,     distance_one);
            wfa.addTrans(inside,     sym, loop2_head, distance_two);
            wfa.addTrans(loop2_head, sym, loop1_head, distance_two);
            wfa.addTrans(loop1_head, sym, final,      distance_one);

            WFA::AccessibleStateSetMap expected;
            expected[start]      = make_vector(distance_zero);
            expected[loop1_head] = make_vector(distance_zero, distance_four, distance_seven, distance_eight, distance_ten);
            expected[loop2_head] = make_vector(distance_two, distance_five, distance_six, distance_eight, distance_nine, distance_ten);
            expected[inside]     = make_vector(distance_three, distance_six, distance_seven, distance_nine, distance_ten);
            expected[final]      = make_vector(distance_one, distance_five, distance_eight, distance_nine, distance_ten);

            WFA::AccessibleStateSetMap reachable = wfa.computeAllReachingWeights();

            EXPECT_EQ_REACHABLE(expected, reachable);
        }

        TEST(wali$wfa$WFA$$computeAllReachingWeights, cycleKeepingMin)
        {
            Key start = getKey("start"), other = getKey("other"), final = getKey("final"), sym = getKey("sym");
            sem_elem_t distance_one = new LongestSaturatingPathSemiring(1, 5);
            sem_elem_t distance_two = new LongestSaturatingPathSemiring(2, 5);
            sem_elem_t distance_three = new LongestSaturatingPathSemiring(3, 5);
            sem_elem_t distance_four = new LongestSaturatingPathSemiring(4, 5);
            sem_elem_t distance_five = new LongestSaturatingPathSemiring(5, 5);
            sem_elem_t zero = distance_one->zero();

            //         <----------------
            //   start ----------------> final
            WFA wfa;
            wfa.addState(start, zero);
            wfa.addState(other, zero);
            wfa.setInitialState(start);
            wfa.addTrans(start, sym, final, distance_one);
            wfa.addTrans(final, sym, start, distance_one);

            WFA::AccessibleStateSetMap expected;
            expected[start] = make_vector(zero->one());
            expected[final] = make_vector(distance_one);

            WFA::AccessibleStateSetMap reachable = wfa.computeAllReachingWeights(SemElemSet::KeepMinimalElements);

            EXPECT_EQ_REACHABLE(expected, reachable);
        }

        TEST(wali$wfa$WFA$$computeAllReachingWeights, cycleKeepingMax)
        {
            Key start = getKey("start"), other = getKey("other"), final = getKey("final"), sym = getKey("sym");
            sem_elem_t distance_one = new LongestSaturatingPathSemiring(1, 5);
            sem_elem_t distance_two = new LongestSaturatingPathSemiring(2, 5);
            sem_elem_t distance_three = new LongestSaturatingPathSemiring(3, 5);
            sem_elem_t distance_four = new LongestSaturatingPathSemiring(4, 5);
            sem_elem_t distance_five = new LongestSaturatingPathSemiring(5, 5);
            sem_elem_t zero = distance_one->zero();

            //         <----------------
            //   start ----------------> final
            WFA wfa;
            wfa.addState(start, zero);
            wfa.addState(other, zero);
            wfa.setInitialState(start);
            wfa.addTrans(start, sym, final, distance_one);
            wfa.addTrans(final, sym, start, distance_one);

            WFA::AccessibleStateSetMap expected;
            expected[start] = make_vector(distance_five);
            expected[final] = make_vector(distance_five);

            WFA::AccessibleStateSetMap reachable = wfa.computeAllReachingWeights(SemElemSet::KeepMaximalElements);

            EXPECT_EQ_REACHABLE(expected, reachable);
        }

    }
}



// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
