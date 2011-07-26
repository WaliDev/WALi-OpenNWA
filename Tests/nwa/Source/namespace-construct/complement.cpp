#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/nwa/construct/complement.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"

namespace wali {
    namespace nwa {
        namespace construct {

            TEST(wali$nwa$construct$$complement, complementOfACompletelyEmptyAutomatonContainsStuff)
            {
                NWA empty;
                WordCollection words;
                
                empty.addSymbol(words.zero);
                empty.addSymbol(words.ret);
                empty.addSymbol(words.call);
                                
                NWARefPtr comp = complement(empty);

                EXPECT_TRUE(query::languageContains(*comp, words.empty));
                EXPECT_TRUE(query::languageContains(*comp, words.balanced));
                EXPECT_TRUE(query::languageContains(*comp, words.balanced0));
                EXPECT_TRUE(query::languageContains(*comp, words.unbalancedLeft));
                EXPECT_TRUE(query::languageContains(*comp, words.unbalancedLeft0));
                EXPECT_TRUE(query::languageContains(*comp, words.unbalancedRight));
                EXPECT_TRUE(query::languageContains(*comp, words.unbalancedRight0));
                EXPECT_TRUE(query::languageContains(*comp, words.fullyUnbalanced));
                EXPECT_TRUE(query::languageContains(*comp, words.fullyUnbalanced0));
            }

        }
    }
}
