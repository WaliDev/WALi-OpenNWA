#include <gtest/gtest.h>

#include "wali/domains/TraceSplitSemElem.hpp"

namespace wali {
  namespace domains {

    typedef LiteralGuard<int> IntGuard;

    struct LiteralGuards {
      IntGuard::Ptr false_, zero, one;

      LiteralGuards()
        : false_(new IntGuard())
        , zero(new IntGuard(0))
        , one(new IntGuard(1))
      {}
    };

    TEST(wali$domains$LiteralGuard, DefaultConstructorProducesFalse)
    {
      LiteralGuards g;
      EXPECT_TRUE(g.false_->isFalse());
    }
    

    TEST(wali$domains$LiteralGuard, ValueConstructorProducesFalse)
    {
      LiteralGuards g;
      EXPECT_FALSE(g.one->isFalse());
      EXPECT_FALSE(g.zero->isFalse());
    }


    // Technically too restrictive a test
    TEST(wali$domains$LiteralGuard, HashesDiffer)
    {
      LiteralGuards g;
      EXPECT_NE(g.one->hash(), g.false_->hash());
      EXPECT_NE(g.one->hash(), g.zero->hash());
      EXPECT_NE(g.zero->hash(), g.false_->hash());
    }


    TEST(wali$domains$LiteralGuard, FalseEqualsFalse)
    {
      LiteralGuards g;
      EXPECT_TRUE(g.false_->equal(g.false_));
    }

    TEST(wali$domains$LiteralGuard, SameValuesAreEqual)
    {
      LiteralGuards g;
      EXPECT_TRUE(g.one->equal(g.one));
      EXPECT_TRUE(g.zero->equal(g.zero));
    }

    TEST(wali$domains$LiteralGuard, FalseDoesNotEqualValues)
    {
      LiteralGuards g;
      EXPECT_FALSE(g.false_->equal(g.one));
      EXPECT_FALSE(g.false_->equal(g.zero));
    }


    TEST(wali$domains$LiteralGuard, DifferentValuesAreUnequal)
    {
      LiteralGuards g;
      EXPECT_FALSE(g.one->equal(g.zero));
      EXPECT_FALSE(g.zero->equal(g.one));
    }


    TEST(wali$domains$LiteralGuard, PrintFunction)
    {
      LiteralGuards g;
      EXPECT_EQ("[FALSE]", g.false_->toString());
      EXPECT_EQ("[1]", g.one->toString());
      EXPECT_EQ("[0]", g.zero->toString());
    }

  }
}


// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
