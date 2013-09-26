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

    }
}

