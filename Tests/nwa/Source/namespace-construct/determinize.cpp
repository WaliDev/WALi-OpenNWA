#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/nwa/construct/concatenate.hpp"
#include "wali/nwa/query/automaton.hpp"
#include "wali/nwa/query/language.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/int-client-info.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"

namespace wali {
    namespace nwa {
        namespace construct {

            TEST(wali$nwa$construct$$determinize, takeEpsilonClosureAroundCall)
            {
                SomeElements e;
                NWA nwa;

                //              *          symbol   ______
                //  --> state -----> state2 ----> ((state3))
                //                                  ``````
                
                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state3);

                nwa.addInternalTrans(e.state, WALI_EPSILON, e.state2);
                nwa.addCallTrans(e.state2, e.symbol, e.state3);

                NWARefPtr det = determinize(nwa);

                EXPECT_FALSE(query::languageIsEmpty(*det));
            }
            
        }
    }
}

