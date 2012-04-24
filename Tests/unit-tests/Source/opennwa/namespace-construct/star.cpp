#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/language.hpp"
#include "opennwa/construct/star.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"


using namespace opennwa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))



namespace opennwa {
        namespace construct {

            TEST(opennwa$query$$star, testBatteryOfVariouslyBalancedNwas)
            {
                // For every case, L(nwa[i])* = L(nwa[i]), except that the
                // strictly unbalanced ones don't accept epsilon.
                Nwa const nwas[] = {
                    Nwa(),
                    AcceptsBalancedOnly().nwa,
                    AcceptsStrictlyUnbalancedLeft().nwa,
                    AcceptsPossiblyUnbalancedLeft().nwa,
                    AcceptsStrictlyUnbalancedRight().nwa,
                    AcceptsPossiblyUnbalancedRight().nwa,
                    AcceptsPositionallyConsistentString().nwa
                };

                const unsigned num_nwas = NUM_ELEMENTS(nwas);

                
                for (unsigned nwa = 0 ; nwa < num_nwas ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());

                    Nwa src = nwas[nwa];
                    NwaRefPtr closure = star(nwas[nwa]);

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


            TEST(opennwa$query$$star, universalNwa)
            {
                Nwa nwa;
                SomeElements e;

                nwa.addInitialState(e.state);
                nwa.addFinalState(e.state);
                nwa.addInternalTrans(e.state, e.symbol, e.state);
                nwa.addCallTrans(e.state, e.symbol, e.state);
                nwa.addReturnTrans(e.state, e.state, e.symbol, e.state);

                NwaRefPtr closure = star(nwa);

                EXPECT_TRUE(query::languageEquals(nwa, *closure));
            }

    }
}
