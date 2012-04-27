#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/language.hpp"
#include "opennwa/construct/reverse.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"


using namespace opennwa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

// For every case, L(nwa[i])* = L(nwa[i]), except that the strictly
// unbalanced ones don't accept epsilon.
static Nwa const nwas[] = {
    Nwa(),
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

static Nwa const expected_answers[] = {
    Nwa(),
    AcceptsBalancedOnly(callKey, returnKey).nwa,
    AcceptsStrictlyUnbalancedRight(callKey, returnKey).nwa,
    AcceptsPossiblyUnbalancedRight(callKey, returnKey).nwa,
    AcceptsStrictlyUnbalancedLeft(callKey, returnKey).nwa,
    AcceptsPossiblyUnbalancedLeft(callKey, returnKey).nwa,
    AcceptsPositionallyConsistentString(callKey, returnKey).nwa
};


namespace opennwa {
        namespace construct {

            TEST(opennwa$query$$reverse, testBatteryOfVariouslyBalancedNwas)
            {
                for (unsigned nwa = 0 ; nwa < num_nwas ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());

                    NwaRefPtr r = reverse(nwas[nwa]);

                    EXPECT_TRUE(query::languageEquals(expected_answers[nwa], *r));
                }
            }


            TEST(opennwa$query$$reverse, universalNwa)
            {
                Nwa nwa;
                SomeElements e;

                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state);
                nwa.addInternalTrans(e.state, e.symbol, e.state);
                nwa.addCallTrans(e.state, e.symbol, e.state);
                nwa.addReturnTrans(e.state, e.state, e.symbol, e.state);

                NwaRefPtr closure = reverse(nwa);

                EXPECT_TRUE(query::languageEquals(nwa, *closure));
            }

        }
}
