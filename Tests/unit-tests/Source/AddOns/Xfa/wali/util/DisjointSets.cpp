#include <glog/logging.h>
#include "gtest/gtest.h"
#include "wali/util/DisjointSets.hpp"

#include <string>

namespace wali {
    namespace util {

        template class DisjointSets<std::string>;

        typedef DisjointSets<std::string> StringSets;

        TEST(wali$util$DisjointSets, emptyCollectionIsEmpty)
        {
            StringSets sets;

            EXPECT_EQ(sets.begin(), sets.end());
        }

    }
}

