#include "gtest/gtest.h"

#include <string>
#include <vector>

#include "wali/domains/KeyedSemElemSet.hpp"
#include "wali/SemElemPair.hpp"
#include "wali/ShortestPathSemiring.hpp"

using std::string;
using namespace wali;
using namespace wali::domains;

struct PKFixtures {
    ref_ptr<PositionKey<int> > i_empty, i_id, i00, i01, i10, i11, i10copy;
    ref_ptr<PositionKey<string> > s_empty, s_id, s00, s01, s10, s11, s10copy;

    // Don't really use these, they're just to make the initialization list
    // readable
    typedef PositionKey<int> Pki;
    typedef PositionKey<string> Pks;

    PKFixtures()
        : i_empty(new Pki(PositionKey<int>::makeZero()))
        , i_id(new Pki(PositionKey<int>::makeOne()))
        , i00(new Pki(0, 0))
        , i01(new Pki(0, 1))
        , i10(new Pki(1, 0))
        , i11(new Pki(1, 1))
        , i10copy(new Pki(*i10))
        , s_empty(new Pks(PositionKey<string>::makeZero()))
        , s_id(new Pks(PositionKey<string>::makeOne()))
        , s00(new Pks("zero", "zero"))
        , s01(new Pks("zero", "one"))
        , s10(new Pks("one", "zero"))
        , s11(new Pks("one", "one"))
        , s10copy(new Pks(*s10))
    {}
};

struct PKLists {
    std::vector<sem_elem_t> ints;
    std::vector<sem_elem_t> strings;

    PKLists() {
        PKFixtures f;
        ints.push_back(f.i_empty);
        ints.push_back(f.i_id);
        ints.push_back(f.i00);
        ints.push_back(f.i01);
        ints.push_back(f.i10);
        ints.push_back(f.i11);

        strings.push_back(f.s_empty);
        strings.push_back(f.s_id);
        strings.push_back(f.s00);
        strings.push_back(f.s01);
        strings.push_back(f.s10);
        strings.push_back(f.s11);
    }
};


namespace {
    PositionKey<int>* downi(sem_elem_t p) {
        return dynamic_cast<PositionKey<int>*>(p.get_ptr());
    }
    PositionKey<string>* downs(sem_elem_t p) {
        return dynamic_cast<PositionKey<string>*>(p.get_ptr());
    }
    KeyedSemElemSet * down_ks(sem_elem_t p) {
        return dynamic_cast<KeyedSemElemSet*>(p.get_ptr());
    }
}


TEST(wali$domains$PositionKey$$extend, zeroExtendBlahGivesZero)
{
    PKFixtures f;

    sem_elem_t
        ai = f.i_empty->extend(f.i_empty),
        bi = f.i10->extend(f.i_empty),
        ci = f.i11->extend(f.i_empty),
        di = f.i_empty->extend(f.i10),
        ei = f.i_empty->extend(f.i_id),
        fi = f.i_id->extend(f.i_empty),
        as = f.s_empty->extend(f.s_empty),
        bs = f.s10->extend(f.s_empty),
        cs = f.s11->extend(f.s_empty),
        ds = f.s_empty->extend(f.s10),
        es = f.s_empty->extend(f.s_id),
        fs = f.s_id->extend(f.s_empty);

    EXPECT_TRUE(downi(ai)->isZero());
    EXPECT_TRUE(downi(bi)->isZero());
    EXPECT_TRUE(downi(ci)->isZero());
    EXPECT_TRUE(downi(di)->isZero());
    EXPECT_TRUE(downi(ei)->isZero());
    EXPECT_TRUE(downi(fi)->isZero());
    
    EXPECT_TRUE(downs(as)->isZero());
    EXPECT_TRUE(downs(bs)->isZero());
    EXPECT_TRUE(downs(cs)->isZero());
    EXPECT_TRUE(downs(ds)->isZero());
    EXPECT_TRUE(downs(es)->isZero());
    EXPECT_TRUE(downs(fs)->isZero());
}

TEST(wali$domains$PositionKey$$extend, oneExtendBlahGivesBlah)
{
    PKFixtures f;

    sem_elem_t
        ai = f.i_id->extend(f.i_id),
        bi = f.i10->extend(f.i_id),
        ci = f.i11->extend(f.i_id),
        di = f.i_id->extend(f.i10),
        as = f.s_id->extend(f.s_id),
        bs = f.s10->extend(f.s_id),
        cs = f.s11->extend(f.s_id),
        ds = f.s_id->extend(f.s10);

    EXPECT_TRUE(ai->equal(f.i_id));
    EXPECT_TRUE(bi->equal(f.i10));
    EXPECT_TRUE(ci->equal(f.i11));
    EXPECT_TRUE(di->equal(f.i10));
    
    EXPECT_TRUE(as->equal(f.s_id));
    EXPECT_TRUE(bs->equal(f.s10));
    EXPECT_TRUE(cs->equal(f.s11));
    EXPECT_TRUE(ds->equal(f.s10));
}

TEST(wali$domains$PositionKey$$extend, nonmatchesGiveZero)
{
    PKFixtures f;

    sem_elem_t
        ai = f.i_empty->extend(f.i11),
        bi = f.i10->extend(f.i11),
        ci = f.i11->extend(f.i00),
        as = f.s_empty->extend(f.s11),
        bs = f.s10->extend(f.s11),
        cs = f.s11->extend(f.s00);

    EXPECT_TRUE(downi(ai)->isZero());
    EXPECT_TRUE(downi(bi)->isZero());
    EXPECT_TRUE(downi(ci)->isZero());
    EXPECT_TRUE(downs(as)->isZero());
    EXPECT_TRUE(downs(bs)->isZero());
    EXPECT_TRUE(downs(cs)->isZero());
}

TEST(wali$domains$PositionKey$$extend, matchesGiveComposition)
{
    PKFixtures f;

    sem_elem_t
        ai = f.i00->extend(f.i00),
        bi = f.i00->extend(f.i01),
        ci = f.i10->extend(f.i01),
        di = f.i10->extend(f.i00),

        as = f.s00->extend(f.s00),
        bs = f.s00->extend(f.s01),
        cs = f.s10->extend(f.s01),
        ds = f.s10->extend(f.s00);

    EXPECT_TRUE(ai->equal(f.i00));
    EXPECT_TRUE(bi->equal(f.i01));
    EXPECT_TRUE(ci->equal(f.i11));
    EXPECT_TRUE(di->equal(f.i10));

    EXPECT_TRUE(as->equal(f.s00));
    EXPECT_TRUE(bs->equal(f.s01));
    EXPECT_TRUE(cs->equal(f.s11));
    EXPECT_TRUE(ds->equal(f.s10));
}



TEST(wali$domains$PositionKey$$PositionKey, constructors)
{
    PKFixtures f;
}


TEST(wali$domains$PositionKey$$equal, eachFixtureUnequal)
{
    PKLists lists;

    for (size_t i=0; i<lists.ints.size(); ++i) {
        for (size_t j=0; j<lists.ints.size(); ++j) {
            sem_elem_t
                left = lists.ints.at(i),
                right = lists.ints.at(j);
            EXPECT_EQ((i==j), left->equal(right));
        }
    }

    for (size_t i=0; i<lists.strings.size(); ++i) {
        for (size_t j=0; j<lists.strings.size(); ++j) {
            sem_elem_t
                left = lists.strings.at(i),
                right = lists.strings.at(j);
            EXPECT_EQ((i==j), left->equal(right));
        }
    }
}


TEST(wali$domains$PositionKey$$containerLessThan, intsAreInCorrectOrder)
{
    PKLists lists;

    for (size_t i=0; i<lists.ints.size(); ++i) {
        for (size_t j=0; j<lists.ints.size(); ++j) {
            sem_elem_t
                left = lists.ints.at(i),
                right = lists.ints.at(j);
            EXPECT_EQ((i<j), left->containerLessThan(right));
        }
    }
}


TEST(wali$domains$PositionKey$$hash, differentElementsProduceDifferentHashes)
{
    PKLists lists;

    for (size_t i=0; i<lists.ints.size(); ++i) {
        for (size_t j=0; j<lists.ints.size(); ++j) {
            sem_elem_t
                left = lists.ints.at(i),
                right = lists.ints.at(j);
            if (i == j) {
                EXPECT_EQ(left->hash(), right->hash());
            }
            else {
                EXPECT_NE(left->hash(), right->hash());
            }
        }
    }

    for (size_t i=0; i<lists.strings.size(); ++i) {
        for (size_t j=0; j<lists.strings.size(); ++j) {
            sem_elem_t
                left = lists.strings.at(i),
                right = lists.strings.at(j);
            if (i == j) {
                EXPECT_EQ(left->hash(), right->hash());
            }
            else {
                EXPECT_NE(left->hash(), right->hash());
            }
        }
    }
}


TEST(wali$domains$PositionKey$$isZero, isZeroForFixture)
{
    PKFixtures f;
    EXPECT_TRUE(f.i_empty->isZero());
    EXPECT_TRUE(f.s_empty->isZero());

    EXPECT_FALSE(f.i_id->isZero());
    EXPECT_FALSE(f.i00->isZero());
    EXPECT_FALSE(f.i01->isZero());
    EXPECT_FALSE(f.i10->isZero());
    EXPECT_FALSE(f.i11->isZero());

    EXPECT_FALSE(f.s_id->isZero());
    EXPECT_FALSE(f.s00->isZero());
    EXPECT_FALSE(f.s01->isZero());
    EXPECT_FALSE(f.s10->isZero());
    EXPECT_FALSE(f.s11->isZero());
}


TEST(wali$domains$PositionKey$$isOne, isOneForFixture)
{
    PKFixtures f;

    EXPECT_TRUE(f.s_id->isOne());
    EXPECT_TRUE(f.s_id->isOne());    
    
    EXPECT_FALSE(f.i_empty->isOne());
    EXPECT_FALSE(f.i00->isOne());
    EXPECT_FALSE(f.i01->isOne());
    EXPECT_FALSE(f.i10->isOne());
    EXPECT_FALSE(f.i11->isOne());
    
    EXPECT_FALSE(f.s_empty->isOne());
    EXPECT_FALSE(f.s00->isOne());
    EXPECT_FALSE(f.s01->isOne());
    EXPECT_FALSE(f.s10->isOne());
    EXPECT_FALSE(f.s11->isOne());
}


TEST(wali$domains$PositionKey$$combine, combineAborts)
{
    PKFixtures f;
    EXPECT_DEATH({
            f.i_empty->combine(f.i_empty);
        },
        "");
}

TEST(wali$domains$PositionKey$$pre$and$post, testAccessors)
{
    PKFixtures f;
    EXPECT_EQ(0, f.i00->pre());
    EXPECT_EQ(0, f.i00->post());
    EXPECT_EQ(1, f.i10->pre());
    EXPECT_EQ("one", f.s11->post());

    EXPECT_EQ(1, f.i10copy->pre());
    EXPECT_EQ(0, f.i10copy->post());
}



struct ShortestPathLengths
{
    sem_elem_t ten, twenty, thirty, fourty, fifty;

    ShortestPathLengths()
        : ten(new ShortestPathSemiring(10))
        , twenty(new ShortestPathSemiring(20))
        , thirty(new ShortestPathSemiring(30))
        , fourty(new ShortestPathSemiring(40))
        , fifty(new ShortestPathSemiring(50))
    {}
};


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

    return *first;
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
