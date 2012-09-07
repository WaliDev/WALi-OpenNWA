#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/nwa_pds/conversions.hpp"
#include "wali/Reach.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <vector>
#include <iostream>
#include <algorithm>

using namespace opennwa;
using namespace wali::wpds;
using namespace std;
using boost::algorithm::split;
using boost::algorithm::is_any_of;


vector<Nwa> test_nwas()
{
#   define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

    // WARNING: the order of these words must be consistent with the row & column
    //          order in the table 'expected_answers' below.
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

    return vector<Nwa>(nwas, nwas+num_nwas);
}


extern std::string answers_forwards_returns[],
    answers_forwards_calls[],
    answers_backwards_returns[],
    answers_backwards_calls[];




namespace opennwa {
        namespace nwa_pds {

            template<typename Container>
            void sort(Container & c)
            {
                std::sort(c.begin(), c.end());
            }

            TEST(opennwa$nwa_pds$$NwaToWpdsCalls, DISABLED_testBatteryOfVariouslyBalancedNwas)
            {
                vector<Nwa> nwas = test_nwas();
                for (size_t nwa = 0 ; nwa < nwas.size() ; ++nwa) {
                    std::stringstream ss;
                    ss << "Testing NWA " << nwa;
                    SCOPED_TRACE(ss.str());

                    // {(f)orward, (b)ackward} x {(c)all, (r)eturn}
                    string fc = NwaToWpdsCalls(nwas[nwa], ReachGen()).toString();
                    string fr = NwaToWpdsReturns(nwas[nwa], ReachGen()).toString();
                    string bc = NwaToBackwardsWpdsCalls(nwas[nwa], ReachGen()).toString();
                    string br = NwaToBackwardsWpdsReturns(nwas[nwa], ReachGen()).toString();

                    // Each is a string. We need to split it into lines, then
                    // sort the lines, as the rules can be printed in random
                    // order.

                    vector<string>
                        actual_fc, actual_fr, actual_bc, actual_br,
                        expected_fc, expected_fr, expected_bc, expected_br;
                    
                    split(actual_fc, fc, is_any_of("\n"));
                    split(actual_fr, fr, is_any_of("\n"));
                    split(actual_bc, bc, is_any_of("\n"));
                    split(actual_br, br, is_any_of("\n"));

                    split(expected_fc, answers_forwards_calls[nwa], is_any_of("\n"));
                    split(expected_fr, answers_forwards_returns[nwa], is_any_of("\n"));
                    split(expected_bc, answers_backwards_calls[nwa], is_any_of("\n"));
                    split(expected_br, answers_backwards_returns[nwa], is_any_of("\n"));

                    sort(actual_fc);
                    sort(actual_fr);
                    sort(actual_bc);
                    sort(actual_br);

                    sort(expected_fc);
                    sort(expected_fr);
                    sort(expected_bc);
                    sort(expected_br);

                    EXPECT_EQ(expected_fc, actual_fc);
                    EXPECT_EQ(expected_fr, actual_fr);
                    EXPECT_EQ(expected_bc, actual_bc);
                    EXPECT_EQ(expected_br, actual_br);
                }
            }
            
    }
}
