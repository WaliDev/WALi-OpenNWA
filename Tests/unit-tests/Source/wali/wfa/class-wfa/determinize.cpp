#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"
#include "wali/ShortestPathSemiring.hpp"
#include "wali/wfa/DeterminizeWeightGen.hpp"

#include "fixtures.hpp"

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

using namespace wali::wfa;

static const WFA fas[] = {
    LoopReject().wfa,
    LoopAccept().wfa,
    EvenAsEvenBs().wfa,
    EpsilonTransitionToAccepting().wfa,
    EpsilonFull().wfa,
    EpsilonTransitionToMiddleToAccepting().wfa,
    ADeterministic().wfa,
    EpsilonTransitionToMiddleToEpsilonToAccepting().wfa,
    AcceptAbOrAcNondet().wfa,
    AEpsilonEpsilonEpsilonA().wfa
};

static const unsigned num_fas = NUM_ELEMENTS(fas);

static const WFA fas_already_deterministic[] = {
    LoopReject().wfa,
    LoopAccept().wfa,
    EvenAsEvenBs().wfa,
    EpsilonFull().wfa,
    ADeterministic().wfa,
    AcceptAbOrAcDeterministic().wfa
};

static const unsigned num_fas_already_deterministic = NUM_ELEMENTS(fas_already_deterministic);


static const WFA fas_to_determinize_and_answers[4][3] = {
    // Original                                            Semi-det,                            det
    { EpsilonTransitionToAccepting().wfa,                  EpsilonSemiDeterministic().wfa,      EpsilonSemiDeterministic().wfa },
    { EpsilonTransitionToMiddleToAccepting().wfa,          ASemiDeterministic().wfa,            ADeterministic().wfa },
    { EpsilonTransitionToMiddleToEpsilonToAccepting().wfa, ASemiDeterministic().wfa,            ADeterministic().wfa },
    { AcceptAbOrAcNondet().wfa,                            AcceptAbOrAcSemiDeterministic().wfa, AcceptAbOrAcDeterministic().wfa }
};

static const unsigned num_fas_to_determinize = NUM_ELEMENTS(fas_to_determinize_and_answers);


namespace wali {
    namespace wfa {

        TEST(wali$wfa$$isIsomorphicTo, selfIsomorphismHolds)
        {
            LoopReject f;

            EXPECT_TRUE(f.wfa.isIsomorphicTo(f.wfa));
        }
        
        TEST(wali$wfa$$isIsomorphicTo, selfIsomorphismOfDifferentAutsFail)
        {
            LoopReject rej;
            LoopAccept acc;

            EXPECT_FALSE(acc.wfa.isIsomorphicTo(rej.wfa));
        }

        TEST(wali$wfa$$isIsomorphicTo, isomorphismChecksStateWeights)
        {
            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();
            
            Letters l;
            WFA w0, w1;

            w0.addState(l.a, zero);
            w1.addState(l.a, one);

            EXPECT_FALSE(w0.isIsomorphicTo(w1));
        }
        
        TEST(wali$wfa$$isIsomorphicTo, isomorphismChecksTransWeights)
        {
            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();
            
            Letters l;
            WFA w0, w1;

            w0.addState(l.a, zero);
            w1.addState(l.a, zero);

            w0.addTrans(l.a, l.a, l.a, zero);
            w1.addTrans(l.a, l.a, l.a, one);

            EXPECT_FALSE(w0.isIsomorphicTo(w1));
        }

        TEST(wali$wfa$$isIsomorphicTo, isomorphismIgnoresStateWeightsWhenCheckWeghtsIsFalse)
        {
            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();
            
            Letters l;
            WFA w0, w1;

            w0.addState(l.a, zero);
            w1.addState(l.a, one);

            EXPECT_TRUE(w0.isIsomorphicTo(w1, false));
        }
        
        TEST(wali$wfa$$isIsomorphicTo, isomorphismIgnoresTransWeightsWhenCheckWeghtsIsFalse)
        {
            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();
            
            Letters l;
            WFA w0, w1;

            w0.addState(l.a, zero);
            w1.addState(l.a, zero);

            w0.addTrans(l.a, l.a, l.a, zero);
            w1.addTrans(l.a, l.a, l.a, one);

            EXPECT_TRUE(w0.isIsomorphicTo(w1, false));
        }


        TEST(wali$wfa$$isIsomorphicTo, battery)
        {
            for (size_t i=0; i<num_fas; ++i) {
                for (size_t j=0; j<num_fas; ++j) {
                    std::stringstream ss;
                    ss << "Testing FAs " << i << " ~ " << j;
                    SCOPED_TRACE(ss.str());
                    
                    WFA left = fas[i];
                    WFA right = fas[j];

                    EXPECT_EQ(i == j, left.isIsomorphicTo(right));
                    EXPECT_EQ(i == j, left.isIsomorphicTo(right, true));
                    EXPECT_EQ(i == j, left.isIsomorphicTo(right, false));
                }
            }
        }

        TEST(wali$wfa$$isIsomorphicTo, loopRejectAndSimilar)
        {
            LoopReject f;
            WFA wfa;

            sem_elem_t one = Reach(true).one();
            sem_elem_t zero = Reach(true).zero();
                
            Letters l;
            Key state = getKey("different state");
            ASSERT_NE(state, f.state);

            wfa.addState(state, zero);
            wfa.setInitialState(state);
            wfa.addTrans(state, l.a, state, one);
            wfa.addTrans(state, l.b, state, one);
            wfa.addTrans(state, l.c, state, one);
            
            EXPECT_TRUE(f.wfa.isIsomorphicTo(wfa));
        }
        


        TEST(wali$wfa$$determinize, alreadyDeterministicBattery) {
            for (size_t i=0; i<num_fas_already_deterministic; ++i) {
                std::stringstream ss;
                ss << "Testing FA " << i;
                SCOPED_TRACE(ss.str());

                WFA orig = fas_already_deterministic[i];
                WFA det = orig.determinize();

                EXPECT_TRUE(orig.isIsomorphicTo(det, false));
            }
        }


        TEST(wali$wfa$$semideterminize, EpsilonTransitionToAccepting) {
            EpsilonTransitionToAccepting f;
            WFA wfa = f.wfa.semideterminize();

            EpsilonSemiDeterministic expected;

            EXPECT_TRUE(wfa.isIsomorphicTo(expected.wfa));
        }


        TEST(wali$wfa$$complete, DISABLED_epsilonSemi)
        {
            EpsilonSemiDeterministic orig;
            EpsilonFull expected;

            Letters l;
            std::set<Key> symbols;
            symbols.insert(l.a);
            symbols.insert(l.b);
            symbols.insert(l.c);

            orig.wfa.complete(symbols);

            EXPECT_TRUE(expected.wfa.isIsomorphicTo(orig.wfa));
        }


        TEST(wali$wfa$$semideterminize, battery)
        {
            for (size_t i=0; i<num_fas_to_determinize; ++i) {
                std::stringstream ss;
                ss << "Testing FA " << i;
                SCOPED_TRACE(ss.str());
                
                WFA input    = fas_to_determinize_and_answers[i][0];
                WFA expected = fas_to_determinize_and_answers[i][1];

                WFA det = input.semideterminize();

                EXPECT_TRUE(expected.isIsomorphicTo(det));
            }
        }
        
        TEST(wali$wfa$$determinize, battery)
        {
            for (size_t i=0; i<num_fas_to_determinize; ++i) {
                std::stringstream ss;
                ss << "Testing FA " << i;
                SCOPED_TRACE(ss.str());
                
                WFA input    = fas_to_determinize_and_answers[i][0];
                WFA expected = fas_to_determinize_and_answers[i][2];

                WFA det = input.determinize();

                EXPECT_TRUE(expected.isIsomorphicTo(det));
            }
        }


        /// NOTE: THIS IS NOT ACTUALLY A VALID SEMIRING
        struct StringWeight : SemElem
        {
            std::string str;

            StringWeight() {}
            StringWeight(std::string const & s) : str(s) {}

            sem_elem_t one() const {
                static sem_elem_t r = new StringWeight();
                return r;
            }

            sem_elem_t zero() const {
                static sem_elem_t r;
                if (r == NULL) {
                    r = new StringWeight("ZERO");
                }
                return r;
            }

            sem_elem_t extend(SemElem * se) {
                StringWeight * w = dynamic_cast<StringWeight*>(se);
                if (w) {
                    if (this == zero().get_ptr() || w == zero().get_ptr()) {
                        return zero();
                    }
                    return new StringWeight(str + " " + w->str);
                }
                assert (false);
                return NULL;
            }

            sem_elem_t combine(SemElem * se) {
                StringWeight * w = dynamic_cast<StringWeight*>(se);
                if (w) {
                    if (this == zero().get_ptr()) {
                        return w;
                    }
                    else if (w == zero().get_ptr()) {
                        return this;
                    }
                    return new StringWeight(str + " | " + w->str);
                }
                assert (false);
                return NULL;
            }

            bool equal(SemElem * se) const {
                StringWeight * w = dynamic_cast<StringWeight*>(se);
                if (w) {
                    return w->str == str;
                }
                return false;
            }

            std::ostream& print( std::ostream & o ) const {
                o << ("[" + str + "]");
                return o;
            }

        };


        struct TestLifter
            : public LiftCombineWeightGen
        {
            sem_elem_t liftWeight(WFA const & UNUSED_PARAMETER(original_wfa),
                                  ITrans const * trans_in_original) const
            {
                std::stringstream ss;
                ss << key2str(trans_in_original->from())
                   << " --" << key2str(trans_in_original->stack()) << "--> "
                   << key2str(trans_in_original->to());
                return new StringWeight(ss.str());
            }
        };

#define ASSERT_CONTAINS(container, value) ASSERT_FALSE(container.end() == container.find(value))
        
        TEST(wali$wfa$$determinize, weightGen)
        {
            Letters l;
            AcceptAbOrAcNondet nondet;
            WFA det = nondet.wfa.semideterminize(TestLifter());

            std::set<Key> start_set, a_set, ab_set, ac_set;
            start_set.insert(nondet.start);
            a_set.insert(nondet.a_left);
            a_set.insert(nondet.a_top);
            ab_set.insert(nondet.ab);
            ac_set.insert(nondet.ac);

            Key start = getKey(start_set);
            Key a = getKey(a_set);
            Key ab = getKey(ab_set);
            Key ac = getKey(ac_set);

            // Just some sanity checks
            ASSERT_EQ(4u, det.getStates().size());
            ASSERT_CONTAINS(det.getStates(), start);
            ASSERT_CONTAINS(det.getStates(), a);
            ASSERT_CONTAINS(det.getStates(), ab);
            ASSERT_CONTAINS(det.getStates(), ac);

            // Now extract the transitions
            Trans trans_start_a, trans_a_ab, trans_a_ac;
            ASSERT_TRUE(det.find(start, l.a, a, trans_start_a));
            ASSERT_TRUE(det.find(a, l.b, ab, trans_a_ab));
            ASSERT_TRUE(det.find(a, l.c, ac, trans_a_ac));

            // Now extract the weights
            sem_elem_t se_start_a = trans_start_a.weight();
            sem_elem_t se_a_ab = trans_a_ab.weight();
            sem_elem_t se_a_ac = trans_a_ac.weight();

            StringWeight * weight_start_a = dynamic_cast<StringWeight*>(se_start_a.get_ptr());
            StringWeight * weight_a_ab = dynamic_cast<StringWeight*>(se_a_ab.get_ptr());
            StringWeight * weight_a_ac = dynamic_cast<StringWeight*>(se_a_ac.get_ptr());

            ASSERT_TRUE(weight_start_a != NULL);
            ASSERT_TRUE(weight_a_ab != NULL);
            ASSERT_TRUE(weight_a_ac != NULL);

            // Finally, check that they are what we expect. Note: the first
            // test is fragile as the order could change.
            EXPECT_EQ("start --a--> a (top) | start --a--> a (left)", weight_start_a->str);
            EXPECT_EQ("a (top) --b--> ab", weight_a_ab->str);
            EXPECT_EQ("a (left) --c--> ac", weight_a_ac->str);
        }
        
    }
}
