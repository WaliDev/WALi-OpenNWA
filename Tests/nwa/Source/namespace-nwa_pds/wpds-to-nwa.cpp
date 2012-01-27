// NOTE: this doesn't really test anything except that things compile

#include "gtest/gtest.h"

#include "opennwa/NWA.hpp"
#include "opennwa/nwa_pds/conversions.hpp"

#include "Tests/nwa/Source/fixtures.hpp"

using namespace wali::wpds;


namespace opennwa {
        namespace nwa_pds {

            TEST(wali$nwa$nwa_pds$$WpdsToNwa, compilationTest)
            {
                WPDS wpds;
                NWARefPtr nwa = WpdsToNwa(wpds);
            }


    }
}
