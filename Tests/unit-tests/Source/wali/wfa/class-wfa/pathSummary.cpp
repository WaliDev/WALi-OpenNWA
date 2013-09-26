#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"

#include "fixtures.hpp"

#include "../../../fixtures/StringWeight.hpp"
using namespace testing;

namespace wali {
    namespace wfa {

        TEST(wali$wfa$$pathSummary, canCallPathSummaryWithNoFinalStates)
        {
            Letters l;
            WFA wfa;
            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();

            Key s1 = getKey("state1");
            Key s2 = getKey("state2");
            Key s3 = getKey("state3");
            Key a = getKey("sym");

            wfa.addState(s1, zero);
            wfa.addState(s2, zero);
            wfa.addState(s3, zero);
            wfa.setInitialState(s1);

            wfa.addTrans(s1, a, s2, one);
            wfa.addTrans(s1, a, s3, one);
            wfa.addTrans(s3, a, s2, one);
            wfa.addTrans(s2, a, s1, one);

            wfa.path_summary_crosscheck_all();
        }

        TEST(wali$wfa$$pathSummary, oneTransitionWfa)
        {
            Letters l;
            WFA wfa;
            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();

            Key s1 = getKey("state1");
            Key s2 = getKey("state2");
            Key a = getKey("sym");

            wfa.addState(s1, zero);
            wfa.addState(s2, zero);
            wfa.setInitialState(s1);
            wfa.addFinalState(s2);
            wfa.addTrans(s1, a, s2, one);

            sem_elem_t initial_weight = wfa.getState(s1)->weight();
            ASSERT_TRUE(initial_weight->equal(zero));

            wfa.path_summary_crosscheck_all();

            initial_weight = wfa.getState(s1)->weight();
            ASSERT_TRUE(initial_weight->equal(one));
        }

        TEST(wali$wfa$$pathSummary, parallelTransitionsWfa)
        {
            Letters l;
            WFA wfa;

            sem_elem_t w1 = new StringWeight("w1");
            sem_elem_t w2 = new StringWeight("w2");
            sem_elem_t either = new StringWeight("w1 | w2");
            sem_elem_t zero = w1->zero();

            Key s1 = getKey("state1");
            Key s2 = getKey("state2");
            Key a = getKey("sym1");
            Key b = getKey("sym2");

            wfa.addState(s1, zero);
            wfa.addState(s2, zero);
            wfa.setInitialState(s1);
            wfa.addFinalState(s2);
            wfa.addTrans(s1, a, s2, w1);
            wfa.addTrans(s1, b, s2, w2);

            sem_elem_t initial_weight = wfa.getState(s1)->weight();
            ASSERT_TRUE(initial_weight->equal(zero));

            wfa.path_summary_crosscheck_all();

            initial_weight = wfa.getState(s1)->weight();
            ASSERT_TRUE(initial_weight->equal(either));
        }

        TEST(wali$wfa$$pathSummary, twoSequentialTransitionsReverseQuery)
        {
            Letters l;
            WFA wfa;

            sem_elem_t w1 = new StringWeight("w1");
            sem_elem_t w2 = new StringWeight("w2");
            sem_elem_t seq = new StringWeight("w2 w1");
            sem_elem_t zero = w1->zero();

            Key s1 = getKey("state1");
            Key s2 = getKey("state2");
            Key s3 = getKey("state3");
            Key a = getKey("sym1");
            
            wfa.addState(s1, zero);
            wfa.addState(s2, zero);
            wfa.addState(s3, zero);
            wfa.setInitialState(s1);
            wfa.addFinalState(s3);
            wfa.addTrans(s1, a, s2, w1);
            wfa.addTrans(s2, a, s3, w2);

            sem_elem_t initial_weight = wfa.getState(s1)->weight();
            ASSERT_TRUE(initial_weight->equal(zero));

            wfa.setQuery(WFA::REVERSE);
            wfa.path_summary_crosscheck_all();

            initial_weight = wfa.getState(s1)->weight();
            std::cerr << initial_weight->toString() << "\n";
            ASSERT_TRUE(initial_weight->equal(seq));
        }
    }
}

