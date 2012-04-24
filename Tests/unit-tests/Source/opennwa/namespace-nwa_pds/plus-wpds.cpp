// NOTE: this doesn't really test anything except that things compile

#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/nwa_pds/plusWpds.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"

using namespace wali::wpds;


namespace opennwa {
        namespace nwa_pds {

            TEST(opennwa$nwa_pds$$plusWpds, compilationTest)
            {
                WPDS wpds;
                Nwa nwa;
                wpds = plusWpds(nwa, wpds);
            }

    }
}
