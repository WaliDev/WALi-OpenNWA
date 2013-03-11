#include "gtest/gtest.h"

#include "wali/SemElemPair.hpp"

#include "fixtures.hpp"

using std::string;
using namespace wali;
using namespace wali::domains;


namespace {
    KeyedSemElemSet * down_ks(sem_elem_t p) {
        return dynamic_cast<KeyedSemElemSet*>(p.get_ptr());
    }
}


struct Mappings
{
    PKFixtures keys;
    ShortestPathLengths paths;
    
    KeyedSemElemSet::BackingMap a, b, c, str_a, ac, aca, all_combine;

    Mappings() {
        // 0 ---> 0   length 20
        // 0 ---> 1   length 10
        a[keys.i00].insert(paths.twenty);
        a[keys.i01].insert(paths.ten);

        str_a[keys.s00].insert(paths.twenty);
        str_a[keys.s01].insert(paths.ten);

        // 0 ---> 0   length 10
        // 1 ---> 1   length 10
        b[keys.i00].insert(paths.ten);
        b[keys.i11].insert(paths.ten);

        // 0 ---> 0   length 10
        // 1 ---> 0   length 10
        c[keys.i00].insert(paths.ten);
        c[keys.i10].insert(paths.ten);

        ac[keys.i00].insert(paths.twenty);
        ac[keys.i00].insert(paths.thirty);

        aca[keys.i00].insert(paths.fourty);
        aca[keys.i00].insert(paths.fifty);
        aca[keys.i01].insert(paths.thirty);
        aca[keys.i01].insert(paths.fourty);

        all_combine[keys.i00].insert(paths.twenty);
        all_combine[keys.i00].insert(paths.ten);
        all_combine[keys.i00].insert(paths.ten);
        all_combine[keys.i01].insert(paths.ten);
        all_combine[keys.i11].insert(paths.ten);
        all_combine[keys.i10].insert(paths.ten);
    }
};


struct KeyedSets
{
    Mappings maps;
    KeyedSemElemSet a, b, c, str_a, ac, aca, all_combine;
    
    ShortestPathLengths paths;
    PKFixtures keys;
    
    KeyedSets()
        : a(maps.a)
        , b(maps.b)
        , c(maps.c)
        , str_a(maps.str_a)
        , ac(maps.ac)
        , aca(maps.aca)
        , all_combine(maps.all_combine)
    {}
};


sem_elem_t at(KeyedSemElemSet const & ks, sem_elem_t key)
{
    std::pair<KeyedSemElemSet::const_iterator, KeyedSemElemSet::const_iterator>
        p = ks.equal_range(key);
    KeyedSemElemSet::const_iterator first = p.first;
    
    assert(p.first != p.second);
    p.first++;
    assert(p.first == p.second);

    return first->second;
}


TEST(wali$domains$KeyedSemElemSet$$at, accessors)
{
    KeyedSets sets;

    EXPECT_EQ(2u, sets.a.size());
    EXPECT_TRUE(at(sets.a, sets.keys.i00)->equal(sets.paths.twenty));
    EXPECT_TRUE(at(sets.a, sets.keys.i01)->equal(sets.paths.ten));

    EXPECT_EQ(2u, sets.b.size());
    EXPECT_TRUE(at(sets.b, sets.keys.i00)->equal(sets.paths.ten));
    EXPECT_TRUE(at(sets.b, sets.keys.i11)->equal(sets.paths.ten));
}


TEST(wali$domains$KeyedSemElemSet$$begin$and$end, range)
{
    KeyedSets sets;

    KeyedSemElemSet::BackingMap m;
    for (KeyedSemElemSet::const_iterator iter = sets.a.begin();
         iter != sets.a.end(); ++iter)
    {
        m[iter->first].insert(iter->second);
    }

    // We want to check that m is equal to sets.a, but the easiest way to do
    // that is make a new KeyedSemElemSet. :-)
    ASSERT_EQ(2u, m.size());
    KeyedSemElemSet a_again(m);

    EXPECT_TRUE(sets.a.equal(&a_again));
}


TEST(wali$domains$KeyedSemElemSet$$one, returnsSingletonOne)
{
    KeyedSets sets;
    sem_elem_t
        a_one_se = sets.a.one(),
        b_one_se = sets.b.one(),
        c_one_se = sets.c.one();

    KeyedSemElemSet
        * a_one = down_ks(a_one_se),
        * b_one = down_ks(b_one_se),
        * c_one = down_ks(c_one_se);

    EXPECT_EQ(1u, a_one->size());
    EXPECT_EQ(1u, b_one->size());
    EXPECT_EQ(1u, c_one->size());

    EXPECT_TRUE(at(*a_one, sets.keys.i00->one())->equal(sets.paths.ten->one()));
    EXPECT_TRUE(at(*b_one, sets.keys.i00->one())->equal(sets.paths.ten->one()));
    EXPECT_TRUE(at(*c_one, sets.keys.i00->one())->equal(sets.paths.ten->one()));
}


TEST(wali$domains$KeyedSemElemSet$$zero, zeroGivesEmptySet)
{
    KeyedSets sets;
    sem_elem_t
        a_zero_se = sets.a.zero(),
        b_zero_se = sets.b.zero(),
        c_zero_se = sets.c.zero();

    KeyedSemElemSet
        * a_zero = down_ks(a_zero_se),
        * b_zero = down_ks(b_zero_se),
        * c_zero = down_ks(c_zero_se);

    EXPECT_EQ(0u, a_zero->size());
    EXPECT_EQ(0u, b_zero->size());
    EXPECT_EQ(0u, c_zero->size());
}

TEST(wali$domains$KeyedSemElemSet$$zero, canTakeZeroThenOne)
{
    KeyedSets sets;
    sem_elem_t
        a_one_se = sets.a.zero()->one(),
        b_one_se = sets.b.zero()->one(),
        c_one_se = sets.c.zero()->one();

    KeyedSemElemSet
        * a_one = down_ks(a_one_se),
        * b_one = down_ks(b_one_se),
        * c_one = down_ks(c_one_se);

    EXPECT_EQ(1u, a_one->size());
    EXPECT_EQ(1u, b_one->size());
    EXPECT_EQ(1u, c_one->size());
    
    EXPECT_TRUE(at(*a_one, sets.keys.i00->one())->equal(sets.paths.ten->one()));
    EXPECT_TRUE(at(*b_one, sets.keys.i00->one())->equal(sets.paths.ten->one()));
    EXPECT_TRUE(at(*c_one, sets.keys.i00->one())->equal(sets.paths.ten->one()));
}

TEST(wali$domains$KeyedSemElemSet$$extend, simpleExtend)
{
    KeyedSets sets;

    sem_elem_t a_extend_b_se = sets.a.extend(&sets.b);
    KeyedSemElemSet * a_extend_b = down_ks(a_extend_b_se);

    // 0->0 distance 30; 0->1 distance 20

    EXPECT_EQ(2u, a_extend_b->size());
    EXPECT_TRUE(at(*a_extend_b, sets.keys.i00)->equal(sets.paths.thirty));
    EXPECT_TRUE(at(*a_extend_b, sets.keys.i01)->equal(sets.paths.twenty));
}

TEST(wali$domains$KeyedSemElemSet$$extend, extendsResultingInMultiplePaths)
{
    KeyedSets sets;

    sem_elem_t
        ac_actual_se = sets.a.extend(&sets.c),
        aca_actual_se = ac_actual_se->extend(&sets.a);
    
    KeyedSemElemSet
        * ac_actual = down_ks(ac_actual_se),
        * aca_actual = down_ks(aca_actual_se);

    EXPECT_EQ(2u, ac_actual->size());
    EXPECT_EQ(4u, aca_actual->size());
    
    EXPECT_TRUE(ac_actual_se->equal(&sets.ac));
    EXPECT_TRUE(aca_actual_se->equal(&sets.aca));
}

TEST(wali$domains$KeyedSemElemSet$$extend, extendingWithZeroGivesZero)
{
    KeyedSets sets;

    sem_elem_t
        a_extend_zero_se = sets.a.extend(sets.a.zero()),
        b_extend_zero_se = sets.b.extend(sets.a.zero()),
        c_extend_zero_se = sets.c.extend(sets.a.zero()),
        z_extend_z_se = sets.a.zero()->extend(sets.a.zero());
    
    KeyedSemElemSet
        * a_extend_zero = down_ks(a_extend_zero_se),
        * b_extend_zero = down_ks(b_extend_zero_se),
        * c_extend_zero = down_ks(c_extend_zero_se),
        * zero_extend_zero = down_ks(z_extend_z_se);

    EXPECT_EQ(0u, a_extend_zero->size());
    EXPECT_EQ(0u, b_extend_zero->size());
    EXPECT_EQ(0u, c_extend_zero->size());
    EXPECT_EQ(0u, zero_extend_zero->size());
}

TEST(wali$domains$KeyedSemElemSet$$equal, fixtureComparisons)
{
    KeyedSets sets;

    EXPECT_TRUE(sets.a.equal(&sets.a));
    EXPECT_TRUE(sets.b.equal(&sets.b));
    EXPECT_TRUE(sets.c.equal(&sets.c));

    EXPECT_FALSE(sets.a.equal(&sets.b));
    EXPECT_FALSE(sets.a.equal(&sets.c));
    EXPECT_FALSE(sets.b.equal(&sets.c));
}

TEST(wali$domains$KeyedSemElemSet$$equal, sameKeysDifferentWeightsUnequal)
{
    KeyedSets sets;

    sem_elem_t a_extend_b = sets.a.extend(&sets.b);

    // has 0->0 and 0->1 just like a
    EXPECT_FALSE(sets.a.equal(a_extend_b));
}

TEST(wali$domains$KeyedSemElemSet$$zero, differentZeroTestsEqual)
{
    KeyedSets sets;

    EXPECT_FALSE(sets.a.zero()->equal(&sets.a));

    EXPECT_TRUE(sets.a.zero()->equal(sets.a.zero()));
    EXPECT_TRUE(sets.a.zero()->equal(sets.b.zero()));
    EXPECT_TRUE(sets.c.zero()->equal(sets.b.zero()));
    EXPECT_TRUE(sets.str_a.zero()->equal(sets.str_a.zero()));
}


TEST(wali$domains$KeyedSemElemSet$$combine, allCombine)
{
    KeyedSets sets;

    sem_elem_t actual_se = sets.a.combine(&sets.b)->combine(&sets.c)->combine(sets.c.zero());
    KeyedSemElemSet * actual = down_ks(actual_se);

    EXPECT_EQ(5u, actual->size());
    EXPECT_TRUE(actual_se->equal(&sets.all_combine));
}


TEST(wali$domains$KeyedSemElemSet$$constructor, addingAZeroGuardOmitsEntry)
{
    PKFixtures keys;
    ShortestPathLengths paths;

    KeyedSemElemSet::BackingMap m;
    m[keys.i_empty].insert(paths.ten);
    m[keys.i00].insert(paths.ten);

    KeyedSemElemSet kses(m);

    ASSERT_EQ(2u, m.size());
    EXPECT_EQ(1u, kses.size());
}


TEST(wali$domains$KeyedSemElemSet$$constructor, havingAZeroSizeWeightSetOmitsEntry)
{
    PKFixtures keys;
    ShortestPathLengths paths;

    KeyedSemElemSet::BackingMap m;
    m[keys.i01];
    m[keys.i00].insert(paths.ten);

    KeyedSemElemSet kses(m);

    ASSERT_EQ(2u, m.size());
    EXPECT_EQ(1u, kses.size());
}


TEST(wali$domains$KeyedSemElemSet$$constructor, havingAWeightSetWithZeroWeightOmitsEntry)
{
    PKFixtures keys;
    ShortestPathLengths paths;

    KeyedSemElemSet::BackingMap m;
    m[keys.i01].insert(paths.ten->zero());
    m[keys.i00].insert(paths.ten);

    KeyedSemElemSet kses(m);

    ASSERT_EQ(2u, m.size());
    EXPECT_EQ(1u, kses.size());
}

TEST(wali$domains$KeyedSemElemSet$$constructor, havingAWeightSetWithZeroWeightAndSomethingElseIncludesEntry)
{
    PKFixtures keys;
    ShortestPathLengths paths;

    KeyedSemElemSet::BackingMap m;
    m[keys.i01].insert(paths.ten->zero());
    m[keys.i01].insert(paths.ten);
    m[keys.i00].insert(paths.ten);

    KeyedSemElemSet kses(m);

    ASSERT_EQ(2u, m.size());
    EXPECT_EQ(3u, kses.size());
}

