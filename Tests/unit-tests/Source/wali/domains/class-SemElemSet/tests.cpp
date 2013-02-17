#include "gtest/gtest.h"

#include "wali/domains/SemElemSet.hpp"
#include "wali/SemElemPair.hpp"
#include "wali/ShortestPathSemiring.hpp"

namespace wali {
    namespace domains {

        bool
        operator == (SemElemSet::ElementSet const & left,
                     SemElemSet::ElementSet const & right)
        {
            if (left.size() != right.size()) {
                return false;
            }

            for (SemElemSet::ElementSet::const_iterator e = left.begin();
                 e != left.end(); ++e)
            {
                if (right.find(*e) == right.end()) {
                    return false;
                }
            }

            return true;
        }
        

        void insert(SemElemSet::ElementSet & set, sem_elem_t element)
        {
            //set.push_back(element);
            set.insert(element);
        }

        void insert(SemElemSet::ElementSet & set, sem_elem_t e1, sem_elem_t e2)
        {
            insert(set, e1);
            insert(set, e2);
        }

        void insert(SemElemSet::ElementSet & set, sem_elem_t e1, sem_elem_t e2, sem_elem_t e3)
        {
            insert(set, e1);
            insert(set, e2);
            insert(set, e3);
        }

        void insert(SemElemSet::ElementSet & set, sem_elem_t e1, sem_elem_t e2, sem_elem_t e3, sem_elem_t e4)
        {
            insert(set, e1);
            insert(set, e2);
            insert(set, e3);
            insert(set, e4);
        }



        TEST(wali$domains$SemElemSet$$SemElemSet, oneArgConstructorMakesZero)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet s(SemElemSet::KeepAllNonduplicates, ten);
            EXPECT_EQ(0u, s.getElements().size());
        }        

        
        TEST(wali$domains$SemElemSet$$SemElemSet, twoArgConstructorRemembersElements)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            insert(es, ten);
            
            SemElemSet s(SemElemSet::KeepAllNonduplicates, ten, es);
            //EXPECT_EQ(es, s.getElements());
            EXPECT_EQ(1u, s.getElements().size());
        }    

#define EXPECT_CONTAINS(container, value) EXPECT_FALSE(container.end() == container.find(value))
        
        TEST(wali$domains$SemElemSet$$SemElemSet, twoArgConstructorRemovesDuplicates)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            insert(es, ten);
            insert(es, ten);
            
            SemElemSet s(SemElemSet::KeepAllNonduplicates, ten, es);
            ASSERT_EQ(1u, s.getElements().size());
            //EXPECT_EQ(ten, s.getElements()[0]);
            EXPECT_CONTAINS(s.getElements(), ten);
        }    

        
        TEST(wali$domains$SemElemSet$$SemElemSet, twoArgConstructorRemovesDuplicatesEvenIfTheyAreSeparateObjects)
        {
            sem_elem_t ten1 = new ShortestPathSemiring(10);
            sem_elem_t ten2 = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            insert(es, ten1);
            insert(es, ten2);
            
            SemElemSet s(SemElemSet::KeepAllNonduplicates, ten1, es);
            EXPECT_EQ(1u, s.getElements().size());
            EXPECT_CONTAINS(s.getElements(), ten1); // could be ten2
        }    

        
        TEST(wali$domains$SemElemSet$$SemElemSet, twoArgConstructorDoesNotRemoveNonduplicates)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            sem_elem_t twenty = new ShortestPathSemiring(20);
            SemElemSet::ElementSet es;
            insert(es, ten);
            insert(es, twenty);
            
            SemElemSet s(SemElemSet::KeepAllNonduplicates, ten, es);
            //EXPECT_EQ(es, s.getElements());
            EXPECT_EQ(2u, s.getElements().size());
        }    

        
        TEST(wali$domains$SemElemSet$$zero, zeroProducesZero)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            insert(es, ten);
            SemElemSet s(SemElemSet::KeepAllNonduplicates, ten, es);

            sem_elem_t zero_se = s.zero();
            SemElemSet * zero_down = dynamic_cast<SemElemSet *>(zero_se.get_ptr());
            
            EXPECT_EQ(0u, zero_down->getElements().size());
        }

        
        TEST(wali$domains$SemElemSet$$one, oneProducesOne)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            insert(es, ten);
            SemElemSet s(SemElemSet::KeepAllNonduplicates, ten, es);

            sem_elem_t one_se = s.one();
            SemElemSet * one_down = dynamic_cast<SemElemSet *>(one_se.get_ptr());
            
            EXPECT_EQ(1u, one_down->getElements().size());
            EXPECT_TRUE((*one_down->getElements().begin())
                        ->equal(ten->one()));
        }


        TEST(wali$domains$SemElemSet$$various, waliSemElemTests)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            insert(es, ten);
            
            sem_elem_t interesting_value = new SemElemSet(SemElemSet::KeepAllNonduplicates, ten, es);
            test_semelem_impl(interesting_value);
        }

        
        TEST(wali$domains$SemElemSet$$combine, combineTakesUnion)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            sem_elem_t twenty = new ShortestPathSemiring(20);
            sem_elem_t thirty = new ShortestPathSemiring(30);
            SemElemSet::ElementSet es1, es2, all;
            insert(es1, ten);
            insert(es1, twenty);
            insert(es2, thirty);
            insert(all, ten);
            insert(all, twenty);
            insert(all, thirty);
            
            SemElemSet s1(SemElemSet::KeepAllNonduplicates, ten, es1);
            SemElemSet s2(SemElemSet::KeepAllNonduplicates, thirty, es2);

            sem_elem_t answer = s1.combine(&s2);
            SemElemSet * answer_down = dynamic_cast<SemElemSet *>(answer.get_ptr());
            
            ASSERT_EQ(3u, answer_down->getElements().size());
            //EXPECT_EQ(all, answer_down->getElements()); // too strict: enforces order
        }


        TEST(wali$domains$SemElemSet$$combine, combineSetsHash)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            sem_elem_t twenty = new ShortestPathSemiring(20);
            sem_elem_t thirtyone = new ShortestPathSemiring(31);
            SemElemSet::ElementSet es1, es2;
            insert(es1, ten);
            insert(es1, twenty);
            insert(es2, thirtyone);
            
            SemElemSet s1(SemElemSet::KeepAllNonduplicates, ten, es1);
            SemElemSet s2(SemElemSet::KeepAllNonduplicates, thirtyone, es2);

            sem_elem_t answer = s1.combine(&s2);

            // *technically* these are too strict.
            EXPECT_NE(0u, s1.hash());
            EXPECT_NE(0u, s2.hash());
            EXPECT_NE(0u, answer->hash());
            EXPECT_NE(s1.hash(), answer->hash());
            EXPECT_NE(s2.hash(), answer->hash());
        }
        

        TEST(wali$domains$SemElemSet$$combine, combineWillNotDuplicateAnElementEvenIfTheyAreSeparateObjects)
        {
            sem_elem_t ten1 = new ShortestPathSemiring(10);
            sem_elem_t ten2 = new ShortestPathSemiring(10);
            sem_elem_t thirty = new ShortestPathSemiring(30);
            SemElemSet::ElementSet es1, es2, all;
            insert(es1, ten1);
            insert(es2, thirty);
            insert(es2, ten2);
            
            SemElemSet s1(SemElemSet::KeepAllNonduplicates, ten1, es1);
            SemElemSet s2(SemElemSet::KeepAllNonduplicates, thirty, es2);

            sem_elem_t answer = s1.combine(&s2);
            SemElemSet * answer_down = dynamic_cast<SemElemSet *>(answer.get_ptr());
            
            ASSERT_EQ(2u, answer_down->getElements().size());
            EXPECT_CONTAINS(answer_down->getElements(), ten1);
            EXPECT_CONTAINS(answer_down->getElements(), thirty);
        }

        
        TEST(wali$domains$SemElemSet$$extend, extendTakesPairwiseExtend)
        {
            sem_elem_t
                one        = new ShortestPathSemiring( 1),
                two        = new ShortestPathSemiring( 2),
                ten        = new ShortestPathSemiring(10),
                eleven     = new ShortestPathSemiring(11),
                twelve     = new ShortestPathSemiring(12),
                twenty     = new ShortestPathSemiring(20),
                twenty_one = new ShortestPathSemiring(21),
                twenty_two = new ShortestPathSemiring(22);
            
            SemElemSet::ElementSet es1, es2;
            insert(es1, one);
            insert(es1, two);
            insert(es2, ten);
            insert(es2, twenty);
            
            SemElemSet s1(SemElemSet::KeepAllNonduplicates, one, es1);
            SemElemSet s2(SemElemSet::KeepAllNonduplicates, ten, es2);

            sem_elem_t answer = s1.extend(&s2);
            SemElemSet * answer_down = dynamic_cast<SemElemSet *>(answer.get_ptr());
            
            ASSERT_EQ(4u, answer_down->getElements().size());
            // FIXME
            //EXPECT_TRUE(eleven->equal(answer_down->getElements()[0])); // too strict: enforces order
            //EXPECT_TRUE(twenty_one->equal(answer_down->getElements()[1]));            
            //EXPECT_TRUE(twelve->equal(answer_down->getElements()[2]));
            //EXPECT_TRUE(twenty_two->equal(answer_down->getElements()[3]));
        }


        TEST(wali$domains$SemElemSet$$extend, extendDoesNotDuplicateElementsEvenIfTheyAreSeparateObjects)
        {
            sem_elem_t
                ten    = new ShortestPathSemiring(10),
                twenty = new ShortestPathSemiring(20),
                thirty = new ShortestPathSemiring(30),
                fourty = new ShortestPathSemiring(40),
                fifty  = new ShortestPathSemiring(50),
                sixty  = new ShortestPathSemiring(60);
            
            SemElemSet::ElementSet es1, es2;
            insert(es1, ten);
            insert(es1, twenty);
            insert(es2, thirty);
            insert(es2, fourty);
            
            SemElemSet s1(SemElemSet::KeepAllNonduplicates, ten, es1);
            SemElemSet s2(SemElemSet::KeepAllNonduplicates, thirty, es2);

            sem_elem_t answer = s1.extend(&s2);
            SemElemSet * answer_down = dynamic_cast<SemElemSet *>(answer.get_ptr());
            
            ASSERT_EQ(3u, answer_down->getElements().size());
            // FIXME
            //EXPECT_TRUE(fourty->equal(answer_down->getElements()[0]));
            //EXPECT_TRUE(fifty->equal(answer_down->getElements()[1]));
            //EXPECT_TRUE(sixty->equal(answer_down->getElements()[2]));
        }


        TEST(wali$domains$SemElemSet$$equal, differentSetsAreUnequal)
        {
            sem_elem_t
                ten    = new ShortestPathSemiring(10),
                twenty = new ShortestPathSemiring(20),
                thirty = new ShortestPathSemiring(30),
                fourty = new ShortestPathSemiring(40);
            
            SemElemSet::ElementSet es1, es2;
            insert(es1, ten);
            insert(es1, twenty);
            insert(es2, thirty);
            insert(es2, fourty);
            
            SemElemSet s1(SemElemSet::KeepAllNonduplicates, ten, es1);
            SemElemSet s2(SemElemSet::KeepAllNonduplicates, thirty, es2);

            EXPECT_FALSE(s1.equal(&s2));
        }

        
        TEST(wali$domains$SemElemSet$$equal, equalityIsOrderIndependent)
        {
            sem_elem_t
                ten    = new ShortestPathSemiring(10),
                twenty = new ShortestPathSemiring(20);
            
            SemElemSet::ElementSet es1, es2;
            insert(es1, ten);
            insert(es1, twenty);
            insert(es2, twenty);
            insert(es2, ten);
            
            SemElemSet s1(SemElemSet::KeepAllNonduplicates, ten, es1);
            SemElemSet s2(SemElemSet::KeepAllNonduplicates, ten, es2);

            EXPECT_TRUE(s1.equal(&s2));
        }
        

        TEST(wali$domains$SemElemSet$$equal, equalElementsAreEqualEvenIfTheyAreSeparateObjects)
        {
            sem_elem_t
                ten1 = new ShortestPathSemiring(10),
                ten2 = new ShortestPathSemiring(10);
            
            SemElemSet::ElementSet es1, es2;
            insert(es1, ten1);
            insert(es2, ten2);
            
            SemElemSet s1(SemElemSet::KeepAllNonduplicates, ten1, es1);
            SemElemSet s2(SemElemSet::KeepAllNonduplicates, ten2, es2);

            EXPECT_TRUE(s1.equal(&s2));
        }


        TEST(wali$domains$SemElemSet, subsumedElementIsRemovedDuringCombine)
        {
            sem_elem_t
                two = new ShortestPathSemiring(2),
                ten = new ShortestPathSemiring(10);

            SemElemSet::ElementSet two_set, ten_set;
            insert(two_set, two);
            insert(ten_set, ten);

            SemElemSet
                two_keeping_min(SemElemSet::KeepMinimalElements, two, two_set),
                ten_keeping_min(SemElemSet::KeepMinimalElements, ten, ten_set),
                two_keeping_max(SemElemSet::KeepMaximalElements, two, two_set),
                ten_keeping_max(SemElemSet::KeepMaximalElements, ten, ten_set);

            sem_elem_t
                actual_min1 = two_keeping_min.combine(&ten_keeping_min),
                actual_min2 = ten_keeping_min.combine(&two_keeping_min),
                actual_max1 = two_keeping_max.combine(&ten_keeping_max),
                actual_max2 = ten_keeping_max.combine(&two_keeping_max);

            // Min/max are by the implicit lattice ordering, which is
            // OPPOSITE of what you think because combine is min:
            //    10 + 2 = 10 min 2 = 2, so 10 <= 2 in the lattice

            ASSERT_TRUE(ten->underApproximates(two));            
            
            EXPECT_TRUE(ten_keeping_min.equal(actual_min1));
            EXPECT_TRUE(ten_keeping_min.equal(actual_min2));

            EXPECT_TRUE(two_keeping_max.equal(actual_max1));
            EXPECT_TRUE(two_keeping_max.equal(actual_max2));
        }


        TEST(wali$domains$SemElemSet, setKeepsIncomparableElementsOnCreationAndCombine)
        {
            // Easiest way I can think of to test this is to use SemElemSets
            // as elements in the SemElemSets that I'm actually interested
            // in. (The simpler domains, like ShortestPath, are totally
            // ordered.)

            // So here we set up the ELEMENTS
            sem_elem_t
                two = new ShortestPathSemiring(2),
                six = new ShortestPathSemiring(6);

            SemElemSet::ElementSet empty_set, two_set, six_set, two_six_set;;
            insert(two_set, two);
            insert(six_set, six);
            insert(two_six_set, two);
            insert(two_six_set, six);

            sem_elem_t
                elem   = new SemElemSet(SemElemSet::KeepAllNonduplicates, two, empty_set),
                elem2  = new SemElemSet(SemElemSet::KeepAllNonduplicates, two, two_set),
                elem6  = new SemElemSet(SemElemSet::KeepAllNonduplicates, six, six_set),
                elem26 = new SemElemSet(SemElemSet::KeepAllNonduplicates, six, two_six_set);

            ASSERT_FALSE(elem2->underApproximates(elem6));
            ASSERT_FALSE(elem6->underApproximates(elem2));
            ASSERT_TRUE(elem->underApproximates(elem2));
            ASSERT_TRUE(elem2->underApproximates(elem26));

            // Here we set up the SETS

            SemElemSet::ElementSet
                set0, set1, set2, set3, set4, set5, set6, set7,
                set8, set9, setA, setB, setC, setD, setE, setF;

            //                                        //      Full set                    Keeping maximal            Keeping minimal
            //                                        // {                     }        {          }   (set0)       {          } (set0)
            insert(set1, elem                      ); // { <>                  }        { <>       }   (set1)       { <>       } (set1)
            insert(set2,       elem2               ); // {     <2>             }        { <2>      }   (set2)       { <2>      } (set2)
            insert(set3, elem, elem2               ); // { <>, <2>             }        { <2>      }   (set2*)      { <>       } (set1*)
            insert(set4,              elem6        ); // {          <6>        }        { <6>      }   (set4)       { <6>      } (set4)
            insert(set5, elem,        elem6        ); // { <>,      <6>        }        { <6>      }   (set4*)      { <>       } (set1*)
            insert(set6,       elem2, elem6        ); // {     <2>, <6>        }        { <2>, <6> }   (set6)       { <2>, <6> } (set6)
            insert(set7, elem, elem2, elem6        ); // { <>, <2>, <6>        }        { <2>, <6> }   (set6*)      { <>       } (set1*)
            insert(set8,                     elem26); // {               <2,6> }        { <2,6>    }   (set8)       { <2, 6>   } (set8)
            insert(set9, elem,               elem26); // { <>,           <2,6> }        { <2,6>    }   (set8*)      { <>       } (set1*)
            insert(setA,       elem2,        elem26); // {     <2>,      <2,6> }        { <2,6>    }   (set8*)      { <2>      } (set2*)
            insert(setB, elem, elem2,        elem26); // { <>, <2>,      <2,6> }        { <2,6>    }   (set8*)      { <>       } (set1*)
            insert(setC,              elem6, elem26); // {          <6>, <2,6> }        { <2,6>    }   (set8*)      { <6>      } (set4*)
            insert(setD, elem,        elem6, elem26); // { <>,      <6>, <2,6> }        { <2,6>    }   (set8*)      { <>       } (set1*)
            insert(setE,       elem2, elem6, elem26); // {     <2>, <6>, <2,6> }        { <2,6>    }   (set8*)      { <2>, <6> } (set6*)
            insert(setF, elem, elem2, elem6, elem26); // { <>, <2>, <6>, <2,6> }        { <2,6>    }   (set8*)      { <>       } (set1*)
            

#define SETS(root) \
    root##_keeping_max(SemElemSet::KeepMaximalElements, elem->one(), root), \
    root##_keeping_min(SemElemSet::KeepMinimalElements, elem->one(), root)
            
            SemElemSet
                SETS(set0), SETS(set1), SETS(set2), SETS(set3), SETS(set4), SETS(set5), SETS(set6), SETS(set7),
                SETS(set8), SETS(set9), SETS(setA), SETS(setB), SETS(setC), SETS(setD), SETS(setE), SETS(setF);

#undef SETS

#define EXPECT_SET_REDUCES_TO(root, maximal, minimal) \
    EXPECT_TRUE(root##_keeping_min.equal(&minimal##_keeping_min)); \
    EXPECT_TRUE(root##_keeping_max.equal(&maximal##_keeping_max))

            EXPECT_SET_REDUCES_TO(set0, set0, set0);
            EXPECT_SET_REDUCES_TO(set1, set1, set1);
            EXPECT_SET_REDUCES_TO(set2, set2, set2);
            EXPECT_SET_REDUCES_TO(set3, set2, set1);
            EXPECT_SET_REDUCES_TO(set4, set4, set4);
            EXPECT_SET_REDUCES_TO(set5, set4, set1);
            EXPECT_SET_REDUCES_TO(set6, set6, set6);
            EXPECT_SET_REDUCES_TO(set7, set6, set1);
            EXPECT_SET_REDUCES_TO(set8, set8, set8);
            EXPECT_SET_REDUCES_TO(set9, set8, set1);
            EXPECT_SET_REDUCES_TO(setA, set8, set2);
            EXPECT_SET_REDUCES_TO(setB, set8, set1);
            EXPECT_SET_REDUCES_TO(setC, set8, set4);
            EXPECT_SET_REDUCES_TO(setD, set8, set1);
            EXPECT_SET_REDUCES_TO(setE, set8, set6);
            EXPECT_SET_REDUCES_TO(setF, set8, set1);

            sem_elem_t
                should_be_set2_max1 = set1_keeping_max.combine(&set2_keeping_max),
                should_be_set2_max2 = set2_keeping_max.combine(&set1_keeping_max),
                should_be_set1_min1 = set1_keeping_min.combine(&set2_keeping_min),
                should_be_set1_min2 = set2_keeping_min.combine(&set1_keeping_min),
                should_be_set6_max1 = set2_keeping_max.combine(&set4_keeping_max),
                should_be_set6_max2 = set4_keeping_max.combine(&set2_keeping_max),
                should_be_set6_min1 = set2_keeping_min.combine(&set4_keeping_min),
                should_be_set6_min2 = set4_keeping_min.combine(&set2_keeping_min),
                should_be_set8_max1 = set8_keeping_max.combine(&set2_keeping_max),
                should_be_set2_min1 = set8_keeping_min.combine(&set2_keeping_min);

            EXPECT_TRUE(set2_keeping_max.equal(should_be_set2_max1));
            EXPECT_TRUE(set2_keeping_max.equal(should_be_set2_max2));

            EXPECT_TRUE(set1_keeping_min.equal(should_be_set1_min1));
            EXPECT_TRUE(set1_keeping_min.equal(should_be_set1_min2));

            EXPECT_TRUE(set6_keeping_max.equal(should_be_set6_max1));
            EXPECT_TRUE(set6_keeping_max.equal(should_be_set6_max2));

            EXPECT_TRUE(set6_keeping_min.equal(should_be_set6_min1));
            EXPECT_TRUE(set6_keeping_min.equal(should_be_set6_min2));

            EXPECT_TRUE(set8_keeping_max.equal(should_be_set8_max1));
            EXPECT_TRUE(set2_keeping_min.equal(should_be_set2_min1));
        }


        /////////////////////////////////////////////////////////////////
        // Test the weird keep-a-transformed-copy thing

        // We will use a set of pairs of sets. The first component will act
        // like keep-minimum, and the second component will take unions of
        // things the first component says are subsumed.

        std::pair<sem_elem_t, sem_elem_t>
        merge(sem_elem_t left, sem_elem_t right)
        {
            SemElemPair
                * left_down  = dynamic_cast<SemElemPair *>(left.get_ptr()),
                * right_down = dynamic_cast<SemElemPair *>(right.get_ptr());

            SemElemSet
                * left_first    = dynamic_cast<SemElemSet *>(left_down ->get_first().get_ptr()),
                * left_second   = dynamic_cast<SemElemSet *>(left_down ->get_second().get_ptr()),
                * right_first   = dynamic_cast<SemElemSet *>(right_down->get_first().get_ptr()),
                * right_second  = dynamic_cast<SemElemSet *>(right_down->get_second().get_ptr());
            
            bool
                left_under_right = left_first->underApproximates(right_first),
                right_under_left = right_first->underApproximates(left_first);

            if (!left_under_right && !right_under_left) {
                // Neither approximates the other. Return both.
                return std::make_pair(left, right);
            }
            else if (left_under_right) {
                // Keeping minimal, so keep left
                SemElemPair * p = new SemElemPair(left_first, left_second->combine(right_second));
                return std::make_pair(p, sem_elem_t());
            }
            else {
                // Keeping minimal, so keep right
                SemElemPair * p = new SemElemPair(right_first, left_second->combine(right_second));
                return std::make_pair(p, sem_elem_t());
            }
        }



        TEST(wali$domains$SemElemSet, useNotMaximalNotMinimalNotAllSubsumption)
        {
            // Easiest way I can think of to test this is to use SemElemSets
            // as elements in the SemElemSets that I'm actually interested
            // in. (The simpler domains, like ShortestPath, are totally
            // ordered.)

            // So here we set up the ROOT ELEMENTS
            sem_elem_t
                sp_two = new ShortestPathSemiring(2),
                sp_six = new ShortestPathSemiring(6);

            SemElemSet::ElementSet empty_set, two_set, six_set, two_six_set;;
            insert(two_set, sp_two);
            insert(six_set, sp_six);
            insert(two_six_set, sp_two);
            insert(two_six_set, sp_six);

            // Here we set up the sets which are ELEMENTS OF EACH PAIR

            sem_elem_t
                elem   = new SemElemSet(SemElemSet::KeepAllNonduplicates, sp_two, empty_set),
                elem2  = new SemElemSet(SemElemSet::KeepAllNonduplicates, sp_two, two_set),
                elem6  = new SemElemSet(SemElemSet::KeepAllNonduplicates, sp_six, six_set),
                elem26 = new SemElemSet(SemElemSet::KeepAllNonduplicates, sp_six, two_six_set);

            ASSERT_FALSE(elem2->underApproximates(elem6));
            ASSERT_FALSE(elem6->underApproximates(elem2));
            ASSERT_TRUE(elem->underApproximates(elem2));
            ASSERT_TRUE(elem2->underApproximates(elem26));

            // Here we set up the element SETS

            SemElemSet::ElementSet set1, set2, set4, set6;

            //                                        //      Full set                    Keeping maximal            Keeping minimal
            insert(set1, elem                      ); // { <>                  }        { <>       }   (set1)       { <>       } (set1)
            insert(set2,       elem2               ); // {     <2>             }        { <2>      }   (set2)       { <2>      } (set2)
            insert(set4,              elem6        ); // {          <6>        }        { <6>      }   (set4)       { <6>      } (set4)
            insert(set6,       elem2, elem6        ); // {     <2>, <6>        }        { <2>, <6> }   (set6)       { <2>, <6> } (set6)

            SemElemSet::SemElemSubsumptionComputer merge_fn(merge);

#define SETS(name, setname) \
            name = new SemElemSet(SemElemSet::KeepAllNonduplicates, elem->one(), setname)
            
            ref_ptr<SemElemSet>
                SETS(empty, set1),
                SETS(two, set2),
                SETS(six, set4),
                SETS(both, set6);

#undef SETS

            // Here we set up the PAIRS and singleton SETS OF THOSE PAIRS

#define DEFINE_INPUT(first, second) \
            ref_ptr<SemElemPair> first##_then_##second##_pair = new SemElemPair(first, second); \
            SemElemSet::ElementSet first##_then_##second##_set;                          \
            first##_then_##second##_set.insert(first##_then_##second##_pair);            \
            ref_ptr<SemElemSet> first##_then_##second##_holder = new SemElemSet(merge, first##_then_##second##_pair->one(), first##_then_##second##_set); \
            SemElemSet * first##_then_##second = first##_then_##second##_holder.get_ptr();
            
            DEFINE_INPUT(empty, empty);
            DEFINE_INPUT(two, two);
            DEFINE_INPUT(two, six);
            DEFINE_INPUT(two, both);
            DEFINE_INPUT(six, six);
            DEFINE_INPUT(both, two);
            DEFINE_INPUT(both, six);
            DEFINE_INPUT(both, both);

            SemElemSet::ElementSet two_pairs_two_and_six_set;
            two_pairs_two_and_six_set.insert(two_then_two_pair);
            two_pairs_two_and_six_set.insert(six_then_six_pair);
            ref_ptr<SemElemSet> two_pairs_two_and_six = new SemElemSet(merge, empty_then_empty_pair->one(), two_pairs_two_and_six_set);

            ASSERT_EQ(2u, two_pairs_two_and_six->getElements().size());

#undef DEFINE_INPUT

            // Phew. Now we can check that combines turn out the way we
            // expect. In the following, by 2 I really mean {2}, and by 6 I
            // really mean {6}. (These are elem2 and elem6 above.)

            // <{},  {}>  + <{},    {}>     ==> <{},  {}>
            EXPECT_TRUE(empty_then_empty -> combine(empty_then_empty) -> equal(empty_then_empty));
            
            // <{2}, {2}> + <{2},   {2}>    ==> <{2}, {2}>
            EXPECT_TRUE(two_then_two -> combine(two_then_two) -> equal(two_then_two));

            // <{2}, {2}> + <{2},   {6}>    ==> <{2}, {2,6}>
            EXPECT_TRUE(two_then_two -> combine(two_then_six) -> equal(two_then_both));    //////////////////
            
            // <{2}, {2}> + <{6},   {6}>    ==> <{2}, {2}> and <{6}, {6}>
            EXPECT_TRUE(two_then_two -> combine(six_then_six) -> equal(two_pairs_two_and_six));
            
            // <{2}, {2}> + <{2,6}, {2,6}>  ==> <{2}, {2, 6}>
            EXPECT_TRUE(two_then_two -> combine(both_then_both) -> equal(two_then_both));
            
            // <{2,6}, {2}> + <{2,6}, {6}> ==> <{2,6}, {2,6}>
            EXPECT_TRUE(both_then_two -> combine(both_then_six) -> equal(both_then_both)); ////////////
            
            // <{2,6}, {2,6}> + <{2,6}, {2,6}> ==> <{2,6}, {2,6}>
            EXPECT_TRUE(both_then_both -> combine(both_then_both) -> equal(both_then_both));
        }
        
    }
}

