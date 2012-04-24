// NOTE: this doesn't really test anything except that things compile

#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/nwa_pds/conversions.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"

using namespace wali::wpds;


namespace opennwa {
        namespace nwa_pds {

            TEST(opennwa$nwa_pds$$WpdsToNwa, compilationTest)
            {
                WPDS wpds;
                NwaRefPtr nwa = WpdsToNwa(wpds);
            }


    }
}
