#include <sstream>

#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/NWAParser.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"

namespace wali {
    namespace nwa {

        TEST(wali$nwa$$read_nwa, runUnitTests)
        {
            parser_test_all();
        }
        
    }
}

