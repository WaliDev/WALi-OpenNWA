#include "gtest/gtest.h"

#include "wali/Key.hpp"
#include "wali/Common.hpp"
#include "opennwa/NwaFwd.hpp"

namespace wali {

    struct KeyFixture {
        Key const string_key, char_star_key, int_key, set_key, pair_key;
        // key_src_t overload omitted
        KeyFixture()
            : string_key(getKey(std::string("string")))
            , char_star_key(getKey("char*"))
            , int_key(getKey(12))
            , set_key(getKey(std::set<Key>()))
            , pair_key(getKey(string_key, int_key))
        {}
    };
    

    TEST(wali$getKey, returnsSequentialAcrossTypes)
    {
        clearKeyspace();

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
        EXPECT_EQ(keys.pair_key, getKey(keys.string_key, keys.int_key));
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


    TEST(wali$key2str, getStringRepresentation)
    {
        KeyFixture keys;

        std::set<Key> keyset;
        keyset.insert(getKey("1"));
        Key const keyset_key_1 = getKey(keyset);
        keyset.insert(getKey("2"));
        Key const keyset_key_1_2 = getKey(keyset);

        EXPECT_EQ("string", key2str(keys.string_key));
        EXPECT_EQ("char*", key2str(keys.char_star_key));
        EXPECT_EQ("12", key2str(keys.int_key));
        EXPECT_EQ("{}", key2str(keys.set_key));
        EXPECT_EQ("{1}", key2str(keyset_key_1));
        // Depending on the order the tests were run in, this may go either way.
        if (getKey("1") < getKey("2")) {
            EXPECT_EQ("{1,2}", key2str(keyset_key_1_2));
        }
        else {
            EXPECT_EQ("{2,1}", key2str(keyset_key_1_2));
        }
        EXPECT_EQ("(string,12)", key2str(keys.pair_key));

        EXPECT_EQ("*", key2str(WALI_EPSILON));
        EXPECT_EQ("@", key2str(WALI_WILD));
    }

    TEST(wali$clearKeySpace, clearingKeySpaceLeavesWildAndEpsilon)
    {
        clearKeyspace();

        EXPECT_EQ(2u, getKey("heeeey"));
        EXPECT_EQ("*", key2str(WALI_EPSILON));
        EXPECT_EQ("@", key2str(WALI_WILD));
    }

    TEST(opennwa$epslion$$and$wild, opennwaAndWaliEpsilonAndWildAgree)
    {
        EXPECT_EQ(wali::WALI_EPSILON, opennwa::EPSILON);
        EXPECT_EQ(wali::WALI_WILD, opennwa::WILD);
    }
}
