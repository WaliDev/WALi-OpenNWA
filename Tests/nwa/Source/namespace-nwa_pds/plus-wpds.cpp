// NOTE: this doesn't really test anything except that things compile

#include "gtest/gtest.h"

#include "opennwa/NWA.hpp"
#include "opennwa/nwa_pds/plusWpds.hpp"

#include "Tests/nwa/Source/fixtures.hpp"

using namespace wali::wpds;


namespace opennwa {
        namespace nwa_pds {

            TEST(opennwa$nwa_pds$$plusWpds, compilationTest)
            {
                WPDS wpds;
                NWA nwa;
                wpds = plusWpds(nwa, wpds);
            }

    }
}
