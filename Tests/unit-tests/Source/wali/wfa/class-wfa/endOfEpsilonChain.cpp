#include "gtest/gtest.h"
#include "wali/wfa/WFA.hpp"
#include "fixtures/SimpleWeights.hpp"
#include "fixtures/StringWeight.hpp"
#include "fixtures/Keys.hpp"
#include "fixtures.hpp"

using namespace testing;
using namespace testing::ShortestPathWeights;

using wali::wfa::WFA;
using wali::sem_elem_t;
using wali::WALI_EPSILON;

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

	TEST(wali$wfa$$removeStatesWithInDegree0, willNotRemoveStartState)
	{
	    SingleState f1(dist0), f2(dist0);

	    f1.wfa.removeStatesWithInDegree0();
	    EXPECT_TRUE(f1.wfa.equal(f2.wfa));
	}

	TEST(wali$wfa$$removeStatesWithInDegree0, removeIsolatedState)
	{
	    SingleState f1(dist0), f2(dist0);
	    f1.wfa.addState(f1.keys.st2, dist0->zero());

	    f1.wfa.removeStatesWithInDegree0();
	    EXPECT_TRUE(f1.wfa.equal(f2.wfa));

	    std::cout << "=================\n";
	    f1.wfa.print(std::cout);
	    std::cout << "=================\n";
	    f2.wfa.print(std::cout);
	}

	//TEST(wali$wfa$$removeStatesWithInDegree0, 
    }
}
