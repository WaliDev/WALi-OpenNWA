#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/nwa/construct/reverse.hpp"

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


// Yes, these are correct: in the reverse automaton, calls become returns &
// vice versa.
const Symbol callKey = AcceptsBalancedOnly().ret;
const Symbol returnKey = AcceptsBalancedOnly().call;

static NWA const expected_answers[] = {
    NWA(),
    AcceptsBalancedOnly(callKey, returnKey).nwa,
    AcceptsStrictlyUnbalancedRight(callKey, returnKey).nwa,
    AcceptsPossiblyUnbalancedRight(callKey, returnKey).nwa,
    AcceptsStrictlyUnbalancedLeft(callKey, returnKey).nwa,
    AcceptsPossiblyUnbalancedLeft(callKey, returnKey).nwa,
    AcceptsPositionallyConsistentString(callKey, returnKey).nwa
};


namespace wali {
    namespace nwa {
        namespace construct {

            TEST(wali$nwa$query$$reverse, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned nwa = 0 ; nwa < num_nwas ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());

                    NWARefPtr r = reverse(nwas[nwa]);

                    EXPECT_TRUE(query::languageEquals(expected_answers[nwa], *r));
                }
            }


            TEST(wali$nwa$query$$reverse, universalNwa)
            {
                NWA nwa;
                SomeElements e;

                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state);
                nwa.addInternalTrans(e.state, e.symbol, e.state);
                nwa.addCallTrans(e.state, e.symbol, e.state);
                nwa.addReturnTrans(e.state, e.state, e.symbol, e.state);

                NWARefPtr closure = reverse(nwa);

                EXPECT_TRUE(query::languageEquals(nwa, *closure));
            }
        }
    }
}
