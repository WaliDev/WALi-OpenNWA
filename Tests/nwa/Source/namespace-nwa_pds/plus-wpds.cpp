// NOTE: this doesn't really test anything except that things compile

#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/nwa_pds/conversions.hpp"

#include "Tests/nwa/Source/fixtures.hpp"

using namespace wali::wpds;


namespace wali {
    namespace nwa {
        namespace nwa_pds {

            TEST(wali$nwa$nwa_pds$$plusWpds, compilationTest)
            {
                WPDS wpds;
                NWA nwa;
                wpds = plusWpds(nwa, wpds);
            }

        }
    }
}
