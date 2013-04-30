#include "gtest/gtest.h"

#include <sstream>
#include <string>
#include <vector>

#include "wali/domains/RepresentativeString.hpp"

using namespace wali;
using namespace wali::domains;

namespace {
    struct RSFixture
    {
        sem_elem_t zero, eps, a, b, c, ab, abc, abc2, def;
        
        RSFixture()
            : zero(new RepresentativeString())
            , eps(new RepresentativeString(""))
            , a(new RepresentativeString("a"))
            , b(new RepresentativeString("b"))
            , c(new RepresentativeString("c"))
            , ab(new RepresentativeString("ab"))
            , abc(new RepresentativeString("abc"))
            , abc2(new RepresentativeString("abc"))
            , def(new RepresentativeString("def"))
        {}
    };

    TEST(wali$domains$RepresentativeString$$equal, zeroEqualsZero)
    {
        RSFixture f;
        EXPECT_TRUE(f.zero->equal(f.zero));
    }

    TEST(wali$domains$RepresentativeString$$equal, nonzeroSelfEquals)
    {
        RSFixture f;
        EXPECT_TRUE(f.eps->equal(f.eps));
        EXPECT_TRUE(f.a->equal(f.a));
        EXPECT_TRUE(f.ab->equal(f.ab));
        EXPECT_TRUE(f.abc->equal(f.abc));
        EXPECT_TRUE(f.abc2->equal(f.abc2));
        EXPECT_TRUE(f.def->equal(f.def));
    }
    
    TEST(wali$domains$RepresentativeString$$equal, nonzeroOtherEqual)
    {
        RSFixture f;
        EXPECT_TRUE(f.abc->equal(f.abc2));
    }
    

    TEST(wali$domains$RepresentativeString$$equal, zeroDoesNotEqualNonzeroes)
    {
        RSFixture f;
        EXPECT_FALSE(f.zero->equal(f.eps));
        EXPECT_FALSE(f.zero->equal(f.a));
        EXPECT_FALSE(f.zero->equal(f.ab));
        EXPECT_FALSE(f.zero->equal(f.abc));
        EXPECT_FALSE(f.zero->equal(f.abc2));
        EXPECT_FALSE(f.zero->equal(f.def));
    }
        

    TEST(wali$domains$RepresentativeString$$equal, nonzeroesDisequalToEachOther)
    {
        RSFixture f;
        EXPECT_FALSE(f.eps->equal(f.a));
        EXPECT_FALSE(f.eps->equal(f.ab));
        EXPECT_FALSE(f.eps->equal(f.abc));
        EXPECT_FALSE(f.eps->equal(f.abc2));
        EXPECT_FALSE(f.eps->equal(f.def));

        EXPECT_FALSE(f.a->equal(f.eps));
        EXPECT_FALSE(f.a->equal(f.ab));
        EXPECT_FALSE(f.a->equal(f.abc));
        EXPECT_FALSE(f.a->equal(f.abc2));
        EXPECT_FALSE(f.a->equal(f.def));

        EXPECT_FALSE(f.ab->equal(f.eps));
        EXPECT_FALSE(f.ab->equal(f.a));
        EXPECT_FALSE(f.ab->equal(f.abc));
        EXPECT_FALSE(f.ab->equal(f.abc2));
        EXPECT_FALSE(f.ab->equal(f.def));

        EXPECT_FALSE(f.abc->equal(f.eps));
        EXPECT_FALSE(f.abc->equal(f.a));
        EXPECT_FALSE(f.abc->equal(f.ab));
        EXPECT_FALSE(f.abc->equal(f.def));

        EXPECT_FALSE(f.abc2->equal(f.eps));
        EXPECT_FALSE(f.abc2->equal(f.a));
        EXPECT_FALSE(f.abc2->equal(f.ab));
        EXPECT_FALSE(f.abc2->equal(f.def));

        EXPECT_FALSE(f.def->equal(f.eps));
        EXPECT_FALSE(f.def->equal(f.a));
        EXPECT_FALSE(f.def->equal(f.ab));
        EXPECT_FALSE(f.def->equal(f.abc));
        EXPECT_FALSE(f.def->equal(f.abc2));
    }

    TEST(wali$domains$RepresentativeString$$zero, zero)
    {
        RSFixture f;
        sem_elem_t zero2 = f.a->zero(), zero3 = f.b->zero();

        EXPECT_TRUE(f.zero->equal(zero2));
        EXPECT_TRUE(f.zero->equal(zero3));

        EXPECT_FALSE(f.a->equal(zero2));
        EXPECT_FALSE(f.b->equal(zero3));
    }

    TEST(wali$domains$RepresentativeString$$one, one)
    {
        RSFixture f;
        sem_elem_t one2 = f.zero->one();

        EXPECT_TRUE(f.eps->equal(one2));
    }

    TEST(wali$domains$RepresentativeString$$extend, zeroExtendStuffGivesZero)
    {
        RSFixture f;
        sem_elem_t
            zeroExtendZero = f.zero->extend(f.zero),
            zeroExtendEps = f.zero->extend(f.eps),
            zeroExtendA = f.zero->extend(f.a),
            zeroExtendAbc = f.zero->extend(f.abc),
            epsExtendZero = f.eps->extend(f.zero),
            aExtendZero = f.a->extend(f.zero);

        EXPECT_TRUE(f.zero->equal(zeroExtendZero));
        EXPECT_TRUE(f.zero->equal(zeroExtendEps));        
        EXPECT_TRUE(f.zero->equal(zeroExtendA));
        EXPECT_TRUE(f.zero->equal(zeroExtendAbc));
        EXPECT_TRUE(f.zero->equal(epsExtendZero));
        EXPECT_TRUE(f.zero->equal(aExtendZero));
    }

    TEST(wali$domains$RepresentativeString$$extend, epsExtendStuffLeavesUnchanged)
    {
        RSFixture f;
        sem_elem_t
            epsExtendEps = f.eps->extend(f.eps),
            epsExtendA = f.eps->extend(f.a),
            epsExtendDef = f.eps->extend(f.def),
            aExtendEps = f.a->extend(f.eps);

        EXPECT_TRUE(f.eps->equal(epsExtendEps));
        EXPECT_TRUE(f.a->equal(epsExtendA));
        EXPECT_TRUE(f.def->equal(epsExtendDef));
        EXPECT_TRUE(f.a->equal(aExtendEps));
    }

    TEST(wali$domains$RepresentativeString$$extend, extendingOutToAbc)
    {
        RSFixture f;
        sem_elem_t
            a   = f.eps->extend(f.a),
            ab  = a->extend(f.b),
            abc = ab->extend(f.c);

        EXPECT_TRUE(f.a->equal(a));
        EXPECT_TRUE(f.ab->equal(ab));
        EXPECT_TRUE(f.abc->equal(abc));
    }

    TEST(wali$domains$RepresentativeString, testSemElemImplTests)
    {
        RSFixture f;
        test_semelem_impl(f.zero);
        test_semelem_impl(f.eps);
        test_semelem_impl(f.a);
        test_semelem_impl(f.b);
        test_semelem_impl(f.abc);
    }

    TEST(wali$domains$RepresentativeString$$containerLessThan, orderedCorrectly)
    {
        RSFixture f;
        std::vector<sem_elem_t> strings;
        strings.push_back(f.zero);
        strings.push_back(f.eps);
        strings.push_back(f.a);
        strings.push_back(f.ab);
        strings.push_back(f.abc);
        strings.push_back(f.b);
        strings.push_back(f.c);
        strings.push_back(f.def);

        for (size_t first = 0; first != strings.size(); ++first) {
            EXPECT_FALSE(strings.at(first)->containerLessThan(strings.at(first)));
            for (size_t second = first+1; second != strings.size(); ++second) {
                std::stringstream ss;
                ss << "Current test: strings[" << first << "] vs strings[" << second << "]\n";
                SCOPED_TRACE(ss.str());
                
                EXPECT_TRUE(strings.at(first)->containerLessThan(strings.at(second)));
                EXPECT_FALSE(strings.at(second)->containerLessThan(strings.at(first)));
            }
        }
    }


    TEST(wali$domains$RepresentativeString$$containerLessThan, equalElementsNotLessThanEachOther)
    {
        RSFixture f;
        EXPECT_FALSE(f.abc->containerLessThan(f.abc2));
        EXPECT_FALSE(f.abc2->containerLessThan(f.abc));
    }

    
    TEST(wali$domains$RepresentativeString$$hash, hashesOfDifferentElementsDiffer)
    {
        // Make sure all elements have different hashes. This is technically
        // too strict, but whatever, it'll probably work.
        RSFixture f;
        std::set<size_t> hashes;

        hashes.insert(f.zero->hash());
        hashes.insert(f.eps->hash());
        hashes.insert(f.a->hash());
        hashes.insert(f.b->hash());
        hashes.insert(f.c->hash());
        hashes.insert(f.ab->hash());
        hashes.insert(f.abc->hash());
        hashes.insert(f.def->hash());

        EXPECT_EQ(8u, hashes.size());
    }

    TEST(wali$domains$RepresentativeString$$hash, hashesOfEqualElementsSame)
    {
        RSFixture f;
        EXPECT_EQ(f.abc->hash(), f.abc2->hash());
    }


    TEST(wali$domains$RepresentativeString$$print, printedRepresentations)
    {
        RSFixture f;
        EXPECT_EQ("RepresentativeString::zero", f.zero->toString());
        EXPECT_EQ("RepresentativeString()", f.eps->toString());
        EXPECT_EQ("RepresentativeString(a)", f.a->toString());
        EXPECT_EQ("RepresentativeString(abc)", f.abc->toString());
        EXPECT_EQ("RepresentativeString(abc)", f.abc2->toString());
    }
}
