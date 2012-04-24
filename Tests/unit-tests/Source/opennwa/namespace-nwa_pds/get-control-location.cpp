#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/nwa_pds/conversions.hpp"

using wali::key2str;
using wali::getKey;
using wali::clearKeyspace;

namespace opennwa
{
    namespace nwa_pds
    {
	TEST(opennwa$nwa_pds$$getProgramControlLocation, doesNotCacheAnswer)
	{
	    Key orig_key = getProgramControlLocation();
	    EXPECT_EQ(key2str(orig_key), "program");

	    clearKeyspace();

	    if (orig_key == 2) {
		Key dummy_key = getKey("goanteheobkarcoh");
		ASSERT_EQ(2u, dummy_key);

		Key new_key = getProgramControlLocation();
		EXPECT_EQ(3u, new_key);
	    }
	    else {
		Key new_key = getProgramControlLocation();
		EXPECT_EQ(2u, new_key);
	    }

	}
    }
}
