#include "gtest/gtest.h"

#include "wali/Key.hpp"

namespace wali {

    struct KeyFixture {
        Key const string_key, char_star_key, int_key, set_key, pair_key;
        // key_src_t overload omitted
        KeyFixture()
            : string_key(getKey(std::string("string")))
            , char_star_key(getKey("char*"))
            , int_key(getKey(12))
            , set_key(getKey(std::set<Key>()))
            , pair_key(getKey(1, 2))
        {}
    };
    

    TEST(wali$getKey, returnsSequentialAcrossTypes)
    {
        KeyFixture keys;

        EXPECT_EQ(keys.char_star_key, keys.string_key + 1);
        EXPECT_EQ(keys.int_key, keys.string_key + 2);
        EXPECT_EQ(keys.set_key, keys.string_key + 3);
        EXPECT_EQ(keys.pair_key, keys.string_key + 4);
    }

    TEST(wali$getKey, sameValueGivesSameKey)
    {
        KeyFixture keys;

        EXPECT_EQ(keys.string_key, getKey(std::string("string")));
        EXPECT_EQ(keys.char_star_key, getKey("char*"));
        EXPECT_EQ(keys.int_key, getKey(12));
        EXPECT_EQ(keys.set_key, getKey(std::set<Key>()));
        EXPECT_EQ(keys.pair_key, getKey(1, 2));
    }

    TEST(wali$getKey, differentValueGiveDifferentKeys)
    {
        KeyFixture keys;

        std::set<Key> keyset;
        keyset.insert(1);

        EXPECT_NE(keys.string_key, getKey(std::string("string 2")));
        EXPECT_NE(keys.char_star_key, getKey("a different char *"));
        EXPECT_NE(keys.int_key, getKey(42));
        EXPECT_NE(keys.set_key, getKey(keyset));
        EXPECT_NE(keys.pair_key, getKey(1, 3));
    }

}
