#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/nwa/construct/star.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"


using namespace wali::nwa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

// For every case, L(nwa[i])* = L(nwa[i]), except that the strictly
// unbalanced ones don't accept epsilon.
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


namespace wali {
    namespace nwa {
        namespace construct {

            TEST(wali$nwa$query$$star, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned nwa = 0 ; nwa < num_nwas ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());

                    NWA src = nwas[nwa];
                    NWARefPtr closure = star(nwas[nwa]);

                    // Now we split. If the original language does not
                    // include epsilon, then I don't want to do anything so
                    // as to minimize what I'm testing.
                    NestedWord epsilon;

                    if (!query::languageContains(src, epsilon)) {
                        // Since closure=src* contains epsilon but src does
                        // not, we either have to remove epsilon from src* or
                        // add it to src before we compare. We'll do the
                        // latter.
                        State newstart = getKey("newStart");
                        
                        src.addInitialState(newstart);
                        src.addFinalState(newstart);
                    }

                    EXPECT_TRUE(query::languageEquals(src, *closure));
                }
            }


            TEST(wali$nwa$query$$star, universalNwa)
            {
                NWA nwa;
                SomeElements e;

                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state);
                nwa.addInternalTrans(e.state, e.symbol, e.state);
                nwa.addCallTrans(e.state, e.symbol, e.state);
                nwa.addReturnTrans(e.state, e.state, e.symbol, e.state);

                NWARefPtr closure = star(nwa);

                EXPECT_TRUE(query::languageEquals(nwa, *closure));
            }
        }
    }
}
