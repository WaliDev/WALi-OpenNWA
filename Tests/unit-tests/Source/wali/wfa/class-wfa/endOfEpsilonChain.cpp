#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"
#include "fixtures/SimpleWeights.hpp"
#include "fixtures/StringWeight.hpp"
#include "fixtures/Keys.hpp"

using namespace testing;
using namespace testing::ShortestPathWeights;

using wali::wfa::WFA;
using wali::sem_elem_t;
using wali::WALI_EPSILON;

//   eps    eps    eps     
// 1 ---> 2 ---> 3 ---> 4
struct ChainedEpsilonTransitions
{
    WFA wfa;
    Keys keys;

    ChainedEpsilonTransitions(sem_elem_t first,
                              sem_elem_t second,
                              sem_elem_t third)
    {
        sem_elem_t zero = first->zero();
        
        wfa.addState(keys.st1, zero);
        wfa.addState(keys.st2, zero);
        wfa.addState(keys.st3, zero);
        wfa.addState(keys.st4, zero);
        wfa.setInitialState(keys.st2);

        wfa.addTrans(keys.st1, WALI_EPSILON, keys.st2, first);
        wfa.addTrans(keys.st2, WALI_EPSILON, keys.st3, second);
        wfa.addTrans(keys.st3, WALI_EPSILON, keys.st4, third);
    }
};


//   eps     a
// 1 ---> 2 ---> 3
struct EpsTransThenNormal
{
    WFA wfa;
    Keys keys;

    EpsTransThenNormal(sem_elem_t first,
                       sem_elem_t second)
    {
        sem_elem_t zero = first->zero();

        wfa.addState(keys.st1, zero);
        wfa.addState(keys.st2, zero);
        wfa.addState(keys.st3, zero);
        wfa.setInitialState(keys.st2);

        wfa.addTrans(keys.st1, WALI_EPSILON, keys.st2, first);
        wfa.addTrans(keys.st2, keys.a, keys.st3, second);
    }
};


//            eps
//            ---> 3
//    eps    /
//  1 ---> 2
//           \                          [anti-line-continuation]
//            ---> 4
//            eps
struct Branching
{
    WFA wfa;
    Keys keys;

    Branching(sem_elem_t first,
              sem_elem_t second,
              sem_elem_t third)
    {
        sem_elem_t zero = first->zero();
        
        wfa.addState(keys.st1, zero);
        wfa.addState(keys.st2, zero);
        wfa.addState(keys.st3, zero);
        wfa.addState(keys.st4, zero);
        wfa.setInitialState(keys.st2);

        wfa.addTrans(keys.st1, WALI_EPSILON, keys.st2, first);
        wfa.addTrans(keys.st2, WALI_EPSILON, keys.st3, second);
        wfa.addTrans(keys.st2, WALI_EPSILON, keys.st4, third);
    }
};


namespace wali {
    namespace wfa {

        TEST(wali$wfa$$endOfEpsilonChain, threeEpsTransInARowEndAtEnd)
        {
            ChainedEpsilonTransitions wfa(dist1, dist10, dist20);
            std::pair<Key, sem_elem_t>
                p = wfa.wfa.endOfEpsilonChain(wfa.keys.st1);

            EXPECT_EQ(wfa.keys.st4, p.first);
            EXPECT_TRUE(dist31->equal(p.second));
        }


        TEST(wali$wfa$$endOfEpsilonChain, threeEpsTransInARowEndAtEndExtendOrderIsCorrect)
        {
            sem_elem_t
                first = new StringWeight("first"),
                second = new StringWeight("second"),
                third = new StringWeight("third"),
                all = new StringWeight("first second third ");
            
            ChainedEpsilonTransitions wfa(first, second, third);
            std::pair<Key, sem_elem_t>
                p = wfa.wfa.endOfEpsilonChain(wfa.keys.st1);

            EXPECT_TRUE(all->equal(p.second));
        }

        
        TEST(wali$wfa$$endOfEpsilonChain, noOutgoingEps)
        {
            ChainedEpsilonTransitions wfa(dist1, dist10, dist20);
            std::pair<Key, sem_elem_t>
                p = wfa.wfa.endOfEpsilonChain(wfa.keys.st4);

            EXPECT_EQ(wfa.keys.st4, p.first);
            EXPECT_TRUE(dist0->equal(p.second));
        }


        TEST(wali$wfa$$endOfEpsilonChain, chainStopsAtNormal)
        {
            EpsTransThenNormal wfa(dist1, dist10);
            std::pair<Key, sem_elem_t>
                p = wfa.wfa.endOfEpsilonChain(wfa.keys.st1);

            EXPECT_EQ(wfa.keys.st2, p.first);
            EXPECT_TRUE(dist1->equal(p.second));
        }
             

        TEST(wali$wfa$$endOfEpsilonChain, chainStartingFromNormalStopsThere)
        {
            EpsTransThenNormal wfa(dist1, dist10);
            std::pair<Key, sem_elem_t>
                p = wfa.wfa.endOfEpsilonChain(wfa.keys.st2);

            EXPECT_EQ(wfa.keys.st2, p.first);
            EXPECT_TRUE(dist0->equal(p.second));
        }


        TEST(wali$wfa$$endOfEpsilonChain, chainStopsAtBranch)
        {
            Branching wfa(dist1, dist10, dist10);
            std::pair<Key, sem_elem_t>
                p = wfa.wfa.endOfEpsilonChain(wfa.keys.st1);

            EXPECT_EQ(wfa.keys.st2, p.first);
            EXPECT_TRUE(dist1->equal(p.second));
        }


        TEST(wali$wfa$$endOfEpsilonChain, chainStartingAtBranchStopsThere)
        {
            Branching wfa(dist1, dist10, dist10);
            std::pair<Key, sem_elem_t>
                p = wfa.wfa.endOfEpsilonChain(wfa.keys.st2);

            EXPECT_EQ(wfa.keys.st2, p.first);
            EXPECT_TRUE(dist0->equal(p.second));
        }
    }
}
