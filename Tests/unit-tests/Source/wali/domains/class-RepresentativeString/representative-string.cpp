#include "gtest/gtest.h"

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
}
