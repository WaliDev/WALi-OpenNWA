#include "gtest/gtest.h"

#include "opennwa/NWA.hpp"

namespace opennwa
{
        //////////////////////////////////
        // Supporting stuff

        
        ///
        /// Uses EXPECT macros to make sure 'nwa' is empty
        extern void expect_nwa_is_empty(NWA const & nwa);

        /// Uses EXPECT macros to make sure NWAs are equal. (Does a more
        /// thorough test than just a == b.)
        extern void expect_nwas_are_equal(NWA const & a, NWA const & b);


        /// Adds one of each kind of entitiy to 'copy' and makes sure it
        /// doesn't change 'reference'.
        extern void expect_nwas_are_physically_different(NWA const & reference, NWA & copy);

}
