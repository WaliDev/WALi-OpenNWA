#include <gtest/gtest.h>

#include <cassert>
#include <set>

#include <wali/domains/genkill/GenKillWeightNoBottom.hpp>
#include <wali/domains/genkill/SortedContainerSetAdapter.hpp>

#include <wali/domains/genkill/GenKillWeight.hpp>


namespace {
    typedef std::set<int> IntSet;
    
    struct IntSetWithBottom
        : wali::domains::genkill::SortedContainerSetAdapter<IntSet>
    {
        typedef wali::domains::genkill::SortedContainerSetAdapter<IntSet> BaseClass;

        IntSetWithBottom(IntSetWithBottom const & other)
            : BaseClass(other)
        {}

        IntSetWithBottom(BaseClass const & other)
            : BaseClass(other)
        {}

        IntSetWithBottom(IntSet const & other)
            : BaseClass(other)
        {}

        IntSetWithBottom()
        {}
        
        static
        IntSetWithBottom &
        UniverseSet() {
            static IntSetWithBottom universe;
            return universe;
        }
    };
    
    // Abbreviations
    typedef wali::domains::genkill::SortedContainerSetAdapter<IntSet> IntSetAdapter;

    //typedef wali::domains::genkill::GenKillWeightNoBottom<IntSetAdapter> Transformer;
    typedef wali::domains::genkill::GenKillWeight<IntSetWithBottom> Transformer;


    struct SetFixture
    {
        // These leave out some numbers on purpose
        IntSet evens;
        IntSet odds;

        SetFixture()
        {
            evens.insert(2);
            evens.insert(4);
            evens.insert(6);
            odds.insert(1);
            odds.insert(3);
            odds.insert(5);
        }
    };


    // Oh god our API makes us to terrible things
    struct ConstructionRunner
    {
        ConstructionRunner(void (*func)()) {
            func();
        }
    };
    
    void make_universal_set_happy()
    {
        IntSetWithBottom & u = IntSetWithBottom::UniverseSet();
        u.insert(0);
        u.insert(1);
        u.insert(2);
        u.insert(3);
        u.insert(4);
        u.insert(5);
        u.insert(6);
        u.insert(7);
        u.insert(8);
        u.insert(9);

        // Cannot use ASSERT/EXPECT assertions here.
        assert(IntSetWithBottom::UniverseSet().size() == 10u);
    }

    ConstructionRunner make_happy(make_universal_set_happy);
}


namespace wali {
    namespace domains {
        namespace genkill {

            TEST(wali$domains$$GenKillTransformer$$make, emptyEmptyGivesOne)
            {
                IntSet empty1, empty2;

                ref_ptr<Transformer> trans = Transformer::make(empty1, empty2);

                // gen = kill = {} corresponds to a transformer of \x.x, aka
                // semiring 1. Make sure it's 1, in several different ways.
                EXPECT_TRUE(trans->IsOne());
                EXPECT_TRUE(trans->one()->equal(trans));
                EXPECT_EQ(0u, trans->getKill().size());
                EXPECT_EQ(0u, trans->getGen().size());

                // Make sure it's not another special value
                EXPECT_FALSE(trans->IsZero());
            }


            TEST(wali$domains$$GenKillTransformer$$make, makeZeroGivesZero)
            {
                ref_ptr<Transformer> trans = Transformer::makeZero();

                // Make sure it's zero, in a couple different ways
                EXPECT_TRUE(trans->IsZero());
                EXPECT_TRUE(trans->zero()->equal(trans));

                // Asking for the gen/kill sets of zero doesn't make sense;
                // semiring 0 is not really a "native" value. Thus we make
                // sure you can't.
                EXPECT_DEATH({ trans->getGen(); }, "!this->IsZero()");
                EXPECT_DEATH({ trans->getKill(); }, "!this->IsZero()");
            }


            TEST(wali$domains$$GenKillTransformer$$make, makeOneGivesOne)
            {
                ref_ptr<Transformer> trans = Transformer::makeOne();

                // Make sure it's zero, in a couple different ways
                EXPECT_TRUE(trans->IsOne());
                EXPECT_TRUE(trans->one()->equal(trans));
                EXPECT_EQ(0u, trans->getKill().size());
                EXPECT_EQ(0u, trans->getGen().size());
            }


            TEST(wali$domains$$GenKillTransformer$$makeOne$and$makeZero, zeroAndOneAreDifferent)
            {
                ref_ptr<Transformer>
                    one = Transformer::makeOne(),
                    zero = Transformer::makeZero();

                EXPECT_TRUE(!one->equal(zero));
            }
            

            TEST(wali$domains$$GenKillTransformer$$make, genAndKillSetsAreStored)
            {
                SetFixture sets;

                ref_ptr<Transformer> trans = Transformer::make(sets.evens, sets.odds);

                EXPECT_EQ(sets.evens, trans->getKill());
                EXPECT_EQ(sets.odds, trans->getGen());
            }


            TEST(wali$domains$$GenKillTransformer$$make, genAndKillSetsAreCopied)
            {
                SetFixture sets;

                ref_ptr<Transformer> trans = Transformer::make(sets.evens, sets.odds);
                sets.evens.insert(10);
                sets.odds.insert(10);

                EXPECT_NE(sets.evens, trans->getKill());
                EXPECT_NE(sets.odds, trans->getGen());
            }


            TEST(wali$domains$$GenKillTransformer$$create, createMakesNormalSetsLikeMake)
            {
                SetFixture sets;

                ref_ptr<Transformer>
                    zero = Transformer::makeZero(),
                    expected = Transformer::make(sets.evens, sets.odds),
                    actual = zero->create(sets.evens, sets.odds);

                EXPECT_EQ(sets.evens, actual->getKill());
                EXPECT_EQ(sets.odds, actual->getGen());
                EXPECT_TRUE(expected->equal(actual));
            }


            TEST(wali$domains$$GenKillTransformer$$make$and$makeOne, oneElementsHaveUniqueRepresentation)
            {
                IntSet empty1, empty2, empty3, empty4;

                ref_ptr<Transformer>
                    one1 = Transformer::make(empty1, empty2),
                    one2 = Transformer::make(empty3, empty4),
                    one3 = Transformer::makeOne(),
                    one4 = Transformer::makeOne();

                sem_elem_t
                    one5 = one1->one(),
                    one6 = one1->create(empty1, empty2);

                EXPECT_EQ(one2, one1);
                EXPECT_EQ(one3, one1);
                EXPECT_EQ(one4, one1);
                EXPECT_EQ(one5, one1);
                EXPECT_EQ(one6, one1);
            }


            TEST(wali$domains$$GenKillTransformer$makeZero, zeroElementsHaveUniqueRepresentation)
            {
                ref_ptr<Transformer>
                    zero1 = Transformer::makeZero(),
                    zero2 = Transformer::makeZero();
                
                sem_elem_t zero3 = zero1->zero();

                EXPECT_EQ(zero2, zero1);
                EXPECT_EQ(zero3, zero1);
            }


            TEST(wali$domains$$GenKillTransformer$make, constructorNormalizes)
            {
                IntSet
                    gen_start = SetFixture().evens,
                    kill_start = SetFixture().odds;

                int extra_gen = 1, extra_kill = 2;

                // Now modify them to make it more interesting. We add
                // additional elements to both the gen and kill sets.
                gen_start.insert(extra_gen);
                kill_start.insert(extra_kill);

                // Now both of these elements overlap, so they should BOTH be
                // removed from the resulting kill set, as gen takes
                // precedence.
                IntSet
                    gen_expected = gen_start,
                    kill_expected = kill_start;

                kill_expected.erase(extra_gen);
                kill_expected.erase(extra_kill);
                    
                // Now do the actual test

                ref_ptr<Transformer> trans = Transformer::make(kill_start, gen_start);

                EXPECT_EQ(kill_expected, trans->getKill());
                EXPECT_EQ(gen_expected, trans->getGen());
            }
            

            TEST(wali$domains$$GenKillTransformer, waliSemElemTestsPass)
            {
                SetFixture sets;

                ref_ptr<Transformer> trans = Transformer::make(sets.evens, sets.odds);

                test_semelem_impl(trans);
            }


            TEST(wali$domains$$GenKillTransformer$$equal, equalSetsGiveEqualTransformers)
            {
                SetFixture sets1, sets2;

                ref_ptr<Transformer>
                    trans1 = Transformer::make(sets1.evens, sets1.odds),
                    trans2 = Transformer::make(sets2.evens, sets2.odds);

                // The following test is "too strict". It would be "fine" if
                // Transformer cached results and returned the same
                // representative. However, this is... unlikely, and I'm
                // putting it here because if it fails, there should be many
                // other tests that someone will want to write.
                EXPECT_TRUE(trans1 != trans2);

                // There are multiple versions of the equality function:
                EXPECT_TRUE(trans1->equal(trans2));
                EXPECT_TRUE(trans1->equal(trans2.get_ptr()));
                EXPECT_TRUE(trans1->isEqual(trans2.get_ptr()));
            }


            TEST(wali$domains$$GenKillTransformer$$equal, unequalSetsGiveEqualTransformers)
            {
                SetFixture sets1, sets2;

                ref_ptr<Transformer>
                    trans1 = Transformer::make(sets1.evens, sets1.odds),
                    trans2 = Transformer::make(sets2.odds, sets2.evens);

                // There are multiple versions of the equality function:
                EXPECT_TRUE(trans1 != trans2);
                EXPECT_FALSE(trans1->equal(trans2));
                EXPECT_FALSE(trans1->equal(trans2.get_ptr()));
                EXPECT_FALSE(trans1->isEqual(trans2.get_ptr()));
            }

            
            // If we have (x * y), there are nine possibilities for what
            // happens. Each cell in the following table shows the net effect
            // for what happens if x and y do not do anything (-) with an
            // element, gen an element (g), or kill an element (k). Shown in
            // parentheses is the int that will be used to illustrate that
            // effect in this test.
            //
            //               x
            //         -     g     k
            //      ----------
            //    - | -(1)  g(4)  k(7)
            // y  g | g(2)  g(5)  g(8)
            //    k | k(3)  k(6)  k(9)
            //
            // Thus:
            //    gen_x  = {4, 5, 6}
            //    kill_x = {7, 8, 9}
            //    gen_y  = {2, 5, 8}
            //    kill_y = {3, 6, 9}
            //    gen_expected  = {2, 4, 5, 8}
            //    kill_expected = {3, 6, 7, 9}

            TEST(wali$domains$$GenKillTransformer$$extend, extendProducesCorrectAnswers)
            {
                // Set up inputs and expected results
                IntSet gen_x, kill_x, gen_y, kill_y, gen_expected, kill_expected;

                gen_x.insert(4);  gen_x.insert(5);  gen_x.insert(6);
                kill_x.insert(7); kill_x.insert(7); kill_x.insert(9);
                
                gen_y.insert(2);  gen_y.insert(5);  gen_y.insert(8);
                kill_y.insert(3); kill_y.insert(6); kill_y.insert(9);
                
                gen_expected.insert(2);  gen_expected.insert(4);
                gen_expected.insert(5);  gen_expected.insert(8);
                kill_expected.insert(3); kill_expected.insert(6);
                kill_expected.insert(7); kill_expected.insert(9);

                // Do the actual test
                ref_ptr<Transformer>
                    x = Transformer::make(kill_x, gen_x),
                    y = Transformer::make(kill_y, gen_y);

                sem_elem_t extend_se = x->extend(y.get_ptr());
                Transformer * extend = dynamic_cast<Transformer*>(extend_se.get_ptr());

                ASSERT_TRUE(extend != NULL);
                EXPECT_EQ(kill_expected, extend->getKill());
                EXPECT_EQ(gen_expected, extend->getGen());
            }


            // If we have (x + y), there are nine possibilities for what
            // happens. Each cell in the following table shows the net effect
            // for what happens if x and y do not do anything (-) with an
            // element, gen an element (g), or kill an element (k). Shown in
            // parentheses is the int that will be used to illustrate that
            // effect in this test.
            //
            //               x
            //         -     g     k
            //      ----------
            //    - | -(1)  g(4)  -(7)
            // y  g | g(2)  g(5)  g(8)
            //    k | -(3)  g(6)  k(9)
            //
            // Thus:
            //    gen_x  = {4, 5, 6}
            //    kill_x = {7, 8, 9}
            //    gen_y  = {2, 5, 8}
            //    kill_y = {3, 6, 9}
            //    gen_expected  = {2, 4, 5, 6, 8}
            //    kill_expected = {9}

            TEST(wali$domains$$GenKillTransformer$$combine, combineProducesCorrectAnswers)
            {
                // Set up inputs and expected results
                IntSet gen_x, kill_x, gen_y, kill_y, gen_expected, kill_expected;

                gen_x.insert(4);  gen_x.insert(5);  gen_x.insert(6);
                kill_x.insert(7); kill_x.insert(7); kill_x.insert(9);
                
                gen_y.insert(2);  gen_y.insert(5);  gen_y.insert(8);
                kill_y.insert(3); kill_y.insert(6); kill_y.insert(9);
                
                gen_expected.insert(2); gen_expected.insert(4);
                gen_expected.insert(5); gen_expected.insert(6);
                gen_expected.insert(8);
                kill_expected.insert(9);

                // Do the actual test
                ref_ptr<Transformer>
                    x = Transformer::make(kill_x, gen_x),
                    y = Transformer::make(kill_y, gen_y);

                sem_elem_t combine_se = x->combine(y.get_ptr());
                Transformer * combine = dynamic_cast<Transformer*>(combine_se.get_ptr());

                ASSERT_TRUE(combine != NULL);
                EXPECT_EQ(kill_expected, combine->getKill());
                EXPECT_EQ(gen_expected, combine->getGen());
            }



        }
    }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 4
// End:

