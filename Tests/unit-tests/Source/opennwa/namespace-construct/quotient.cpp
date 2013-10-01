#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/language.hpp"
#include "opennwa/construct/quotient.hpp"
#include "opennwa/query/automaton.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/int-client-info.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"

namespace opennwa {
	namespace construct {

		class opennwa$construct$$quotient$$$EmptyNWA : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states

			}

			Nwa nwa;

		};

		// The fixture for diamond
		class opennwa$construct$$quotient$$$Diamond : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states
				s1 = getKey("s1");
				s2 = getKey("s2"); 
				s3 = getKey("s3"); 
				s4 = getKey("s4");

				lab1 = getKey("label1");
				lab2 = getKey("label2");

				// create nwa
				nwa.addInitialState(s1);
				nwa.addFinalState(s4);
				//Diamond 1
				nwa.addInternalTrans(s1, lab1, s2);
				nwa.addInternalTrans(s1, lab2, s3);
				nwa.addInternalTrans(s2, lab2, s4);
				nwa.addInternalTrans(s3, lab1, s4);			

			}

			Nwa nwa;
			State s1, s2, s3, s4;
			Symbol lab1, lab2;
		};

		// The fixture for diamond with epsilon transitions
		class opennwa$construct$$quotient$$$DiamondEpsilonTransitions : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states
				s1 = getKey("s1");
				s2 = getKey("s2"); 
				s3 = getKey("s3"); 
				s4 = getKey("s4");

				lab1 = getKey("label1");
				lab2 = getKey("label2");

				// create nwa
				nwa.addInitialState(s1);
				nwa.addFinalState(s4);
				//Diamond 1
				nwa.addInternalTrans(s1, EPSILON, s2);
				nwa.addInternalTrans(s1, lab1, s3);
				nwa.addInternalTrans(s2, lab2, s4);
				nwa.addInternalTrans(s3, EPSILON, s4);			

			}

			Nwa nwa;
			State s1, s2, s3, s4;
			Symbol lab1, lab2;
		};

		// The fixture for diamond with epsilon transitions
		class opennwa$construct$$quotient$$$DiamondWildTransitions : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states
				s1 = getKey("s1");
				s2 = getKey("s2"); 
				s3 = getKey("s3"); 
				s4 = getKey("s4");

				lab1 = getKey("label1");
				lab2 = getKey("label2");

				// create nwa
				nwa.addInitialState(s1);
				nwa.addFinalState(s4);
				//Diamond 1
				nwa.addInternalTrans(s1, WILD, s2);
				nwa.addInternalTrans(s1, lab1, s3);
				nwa.addInternalTrans(s2, lab2, s4);
				nwa.addInternalTrans(s3, WILD, s4);			

			}

			Nwa nwa;
			State s1, s2, s3, s4;
			Symbol lab1, lab2;
		};

		// The fixture for diamond and an isolated loop.
		class opennwa$construct$$quotient$$$DiamondIsolatedLoop : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states
				s1 = getKey("s1");
				s2 = getKey("s2"); 
				s3 = getKey("s3"); 
				s4 = getKey("s4");
				s5 = getKey("s5"); 
				s6 = getKey("s6");

				lab1 = getKey("label1");
				lab2 = getKey("label2");

				nwa.addInitialState(s1);
				nwa.addFinalState(s4);
				nwa.addInternalTrans(s1, lab1, s2);
				nwa.addInternalTrans(s1, lab2, s3);
				nwa.addInternalTrans(s2, lab2, s4);
				nwa.addInternalTrans(s3, lab1, s4);
				//isolated loop
				nwa.addInternalTrans(s5, lab1, s6); 
				nwa.addInternalTrans(s6, lab2, s5); 

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6;
			Symbol lab1, lab2;

		};

		// The fixture for two diamonds having two initial states
		class opennwa$construct$$quotient$$$DoubleDiamondMultipleInitialStates : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states
				s1 = getKey("s1");
				s2 = getKey("s2"); 
				s3 = getKey("s3"); 
				s4 = getKey("s4");
				s5 = getKey("s5"); 
				s6 = getKey("s6");
				s7 = getKey("s7"); 
				s8 = getKey("s8");

				lab1 = getKey("label1");
				lab2 = getKey("label2");

				// create nwa
				nwa.addInitialState(s1);
				nwa.addInitialState(s5);
				nwa.addFinalState(s4);
				nwa.addFinalState(s8);
				//Diamond 1
				nwa.addInternalTrans(s1, lab1, s2);
				nwa.addInternalTrans(s1, lab2, s3);
				nwa.addInternalTrans(s2, lab2, s4);
				nwa.addInternalTrans(s3, lab1, s4);			
				// Diamond 2
				nwa.addInternalTrans(s5, lab2, s6);
				nwa.addInternalTrans(s5, lab1, s7);
				nwa.addInternalTrans(s6, lab1, s8);
				nwa.addInternalTrans(s7, lab2, s8);

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6, s7, s8;
			Symbol lab1, lab2;
		};

		// The fixture for two diamonds having two initial states
		class opennwa$construct$$quotient$$$DoubleDiamondMultipleInitialStatesEpsilonAndWildTransitions : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states
				s1 = getKey("s1");
				s2 = getKey("s2"); 
				s3 = getKey("s3"); 
				s4 = getKey("s4");
				s5 = getKey("s5"); 
				s6 = getKey("s6");
				s7 = getKey("s7"); 
				s8 = getKey("s8");

				lab1 = getKey("label1");
				lab2 = getKey("label2");

				// create nwa
				nwa.addInitialState(s1);
				nwa.addInitialState(s5);
				nwa.addFinalState(s4);
				nwa.addFinalState(s8);
				//Diamond 1
				nwa.addInternalTrans(s1, EPSILON, s2);
				nwa.addInternalTrans(s1, lab1, s3);
				nwa.addInternalTrans(s2, lab2, s4);
				nwa.addInternalTrans(s3, WILD, s4);			
				// Diamond 2
				nwa.addInternalTrans(s5, lab2, s6);
				nwa.addInternalTrans(s5, WILD, s7);
				nwa.addInternalTrans(s6, EPSILON, s8);
				nwa.addInternalTrans(s7, lab1, s8);

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6, s7, s8;
			Symbol lab1, lab2;
		};

		// The fixture for two diamonds having two initial states and interprocedural
		class opennwa$construct$$quotient$$$DoubleDiamondMultipleInitialStatesInterprocedural : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states
				s1 = getKey("s1");
				s2 = getKey("s2"); 
				s3 = getKey("s3"); 
				s4 = getKey("s4");
				s5 = getKey("s5"); 
				s6 = getKey("s6");
				s7 = getKey("s7"); 
				s8 = getKey("s8");

				lab1 = getKey("label1");
				lab2 = getKey("label2");

				// create nwa
				nwa.addInitialState(s1);
				nwa.addInitialState(s5);
				nwa.addFinalState(s4);
				//Diamond 1
				nwa.addInternalTrans(s1, lab1, s2);
				nwa.addInternalTrans(s1, lab2, s3);			
				// Diamond 2
				nwa.addInternalTrans(s5, lab1, s6);
				nwa.addInternalTrans(s5, lab2, s7);
				nwa.addInternalTrans(s6, lab2, s8);
				nwa.addInternalTrans(s7, lab1, s8);
				nwa.addCallTrans(s3, lab1, s5);
				nwa.addReturnTrans(s8, s3, lab2, s4);

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6, s7, s8;
			Symbol lab1, lab2;
		};

		// The fixture for diamonds in main and callee-2.
		class opennwa$construct$$quotient$$$DoubleDiamondNestedInterprocedural : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states
				s1 = getKey("s1");
				s2 = getKey("s2"); 
				s3 = getKey("s3"); 
				s4 = getKey("s4");
				s5 = getKey("s5"); 
				s6 = getKey("s6");
				s7 = getKey("s7"); 
				s8 = getKey("s8");
				s9 = getKey("s9"); 
				s10 = getKey("s10");
				s11 = getKey("s11"); 
				s12 = getKey("s12");

				lab1 = getKey("label1");
				lab2 = getKey("label2");

				// create nwa			
				nwa.addInitialState(s1);
				nwa.addFinalState(s4);
				nwa.addFinalState(s6);
				//Diamond 1
				nwa.addInternalTrans(s1, lab1, s2);
				nwa.addInternalTrans(s1, lab2, s3);
				nwa.addInternalTrans(s2, lab2, s4);
				nwa.addInternalTrans(s3, lab1, s4);			
				// Diamond 2
				nwa.addInternalTrans(s9, lab2, s10);
				nwa.addInternalTrans(s9, lab1, s11);
				nwa.addInternalTrans(s10, lab1, s12);
				nwa.addInternalTrans(s11, lab2, s12);
				nwa.addInternalTrans(s7, lab1, s9);
				nwa.addInternalTrans(s12, lab2, s8);
				// call and return from main to callee-1
				nwa.addCallTrans(s3, lab1, s5);
				nwa.addReturnTrans(s6, s3, lab2, s4);
				// call and return from callee-1 to callee-2
				nwa.addCallTrans(s5, lab1, s7);
				nwa.addReturnTrans(s8, s5, lab2, s6);

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12;
			Symbol lab1, lab2;
		};

		// Tests begin ...

		TEST(opennwa$construct$$quotientStates, repUniqueKeyMapPrint)
		{
			wali::util::DisjointSets<State> partition;

			State s1 = getKey("s1");
			State s2 = getKey("s2");
			partition.merge_sets(s1, s2);

			State s3 = getKey("s3");
			State s4 = getKey("s4");
			partition.merge_sets(s3, s4);

			State s5 = getKey("s5");
			partition.merge_sets(s2, s5);

			partition.output(std::cout << "Printing partition : " <<std::endl);

			Nwa out;
			Nwa nwa;
			State resSt;
			ClientInfoRefPtr resCI;
			std::map<State, State> repUniqueKeyMap;

			for( wali::util::DisjointSets<State>::const_iterator outer_iter = partition.begin(); outer_iter != partition.end(); outer_iter++ )
			{   
				std::set<State> equivalenceClass;
				for( wali::util::DisjointSets<State>::InnerSet::const_iterator inner_iter = outer_iter->begin(); inner_iter != outer_iter->end(); ++inner_iter ) {
					equivalenceClass.insert(*inner_iter);
				}
				out.statesQuotient(nwa, equivalenceClass, resSt, resCI) ;
				repUniqueKeyMap[ partition.representative( *(equivalenceClass.begin()) ) ] = resSt;
			}
			std::cout << std::endl;
			std::cout << partition.representative(s1) << " -> " << repUniqueKeyMap[ partition.representative(s1) ] << std::endl;
			std::cout << partition.representative(s3) << " -> " << repUniqueKeyMap[ partition.representative(s3) ] << std::endl;

		}

		TEST_F(opennwa$construct$$quotient$$$EmptyNWA, EmptyNWA)
		{
			wali::util::DisjointSets<State> partition;

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageIsEmpty(out) );

		}

		TEST_F(opennwa$construct$$quotient$$$Diamond, Diamond)
		{
			wali::util::DisjointSets<State> partition;

			partition.insert(s1);
			partition.merge_sets(s2, s3);
			partition.insert(s4);

			Nwa coalesced;

			// create states
			State state1 = getKey("state1");
			State state2 = getKey("state2"); 
			State state4 = getKey("state4");

			// create nwa
			coalesced.addInitialState(state1);
			coalesced.addFinalState(state4);
			coalesced.addInternalTrans(state1, lab1, state2);
			coalesced.addInternalTrans(state2, lab1, state4);
			coalesced.addInternalTrans(state1, lab2, state2);
			coalesced.addInternalTrans(state2, lab2, state4);

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageSubsetEq( nwa ,out ) );
			EXPECT_TRUE( opennwa::query::languageEquals( coalesced ,out ) );
			EXPECT_FALSE( opennwa::query::statesOverlap(out, nwa) );

		}

		TEST_F(opennwa$construct$$quotient$$$DiamondEpsilonTransitions, DiamondEpsilonTransitions)
		{
			wali::util::DisjointSets<State> partition;

			partition.insert(s1);
			partition.merge_sets(s2, s3);
			partition.insert(s4);

			Nwa coalesced;

			// create states
			State state1 = getKey("state1");
			State state2 = getKey("state2"); 
			State state4 = getKey("state4");

			// create nwa
			coalesced.addInitialState(state1);
			coalesced.addFinalState(state4);
			coalesced.addInternalTrans(state1, EPSILON, state2);
			coalesced.addInternalTrans(state2, EPSILON, state4);
			coalesced.addInternalTrans(state1, lab1, state2);
			coalesced.addInternalTrans(state2, lab2, state4);

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageSubsetEq( nwa ,out ) );
			EXPECT_TRUE( opennwa::query::languageEquals( coalesced ,out ) );
			EXPECT_FALSE( opennwa::query::statesOverlap(out, nwa) );

		}

		TEST_F(opennwa$construct$$quotient$$$DiamondWildTransitions, DiamondWildTransitions)
		{
			wali::util::DisjointSets<State> partition;

			partition.insert(s1);
			partition.merge_sets(s2, s3);
			partition.insert(s4);

			Nwa coalesced;

			// create states
			State state1 = getKey("state1");
			State state2 = getKey("state2"); 
			State state4 = getKey("state4");

			// create nwa
			coalesced.addInitialState(state1);
			coalesced.addFinalState(state4);
			coalesced.addInternalTrans(state1, WILD, state2);
			coalesced.addInternalTrans(state2, WILD, state4);

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageSubsetEq( nwa ,out ) );
			EXPECT_TRUE( opennwa::query::languageEquals( coalesced ,out ) );
			EXPECT_FALSE( opennwa::query::statesOverlap(out, nwa) );

		}

		TEST_F(opennwa$construct$$quotient$$$DiamondIsolatedLoop, DiamondIsolatedLoop)
		{
			wali::util::DisjointSets<State> partition;

			partition.insert(s1);
			partition.merge_sets(s2, s3);
			partition.merge_sets(s2, s5);
			partition.merge_sets(s4, s6);

			Nwa coalesced;

			// create states
			State state1 = getKey("state1");
			State state2 = getKey("state2"); 
			State state4 = getKey("state4");

			// create nwa
			coalesced.addInitialState(state1);
			coalesced.addFinalState(state4);
			coalesced.addInternalTrans(state1, lab1, state2);
			coalesced.addInternalTrans(state2, lab1, state4);
			coalesced.addInternalTrans(state1, lab2, state2);
			coalesced.addInternalTrans(state2, lab2, state4);
			coalesced.addInternalTrans(state4, lab2, state2);

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageSubsetEq( nwa ,out ) );
			EXPECT_TRUE( opennwa::query::languageEquals( coalesced ,out ) );
			EXPECT_FALSE( opennwa::query::statesOverlap(out, nwa) );

		}

		TEST_F(opennwa$construct$$quotient$$$DoubleDiamondMultipleInitialStates, DoubleDiamondMultipleInitialStates)
		{
			wali::util::DisjointSets<State> partition;

			partition.merge_sets(s1, s5);
			partition.merge_sets(s2, s6);
			partition.merge_sets(s3, s7);
			partition.merge_sets(s4, s8);

			Nwa coalesced;

			// create states
			State state1 = getKey("state1");
			State state2 = getKey("state2"); 
			State state3 = getKey("state3"); 
			State state4 = getKey("state4");

			// create nwa
			coalesced.addInitialState(state1);
			coalesced.addFinalState(state4);
			//Diamond
			coalesced.addInternalTrans(state1, lab1, state2);
			coalesced.addInternalTrans(state1, lab1, state3);
			coalesced.addInternalTrans(state2, lab1, state4);
			coalesced.addInternalTrans(state3, lab1, state4);
			coalesced.addInternalTrans(state1, lab2, state2);
			coalesced.addInternalTrans(state1, lab2, state3);
			coalesced.addInternalTrans(state2, lab2, state4);
			coalesced.addInternalTrans(state3, lab2, state4);

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageSubsetEq( nwa ,out ) );
			EXPECT_TRUE( opennwa::query::languageEquals( coalesced ,out ) );
			EXPECT_FALSE( opennwa::query::statesOverlap(out, nwa) );

		}

		TEST_F(opennwa$construct$$quotient$$$DoubleDiamondMultipleInitialStatesEpsilonAndWildTransitions, DoubleDiamondMultipleInitialStatesEpsilonAndWildTransitions)
		{
			wali::util::DisjointSets<State> partition;

			partition.merge_sets(s1, s5);
			partition.merge_sets(s2, s6);
			partition.merge_sets(s3, s7);
			partition.merge_sets(s4, s8);

			Nwa coalesced;

			// create states
			State state1 = getKey("state1");
			State state2 = getKey("state2"); 
			State state3 = getKey("state3"); 
			State state4 = getKey("state4");

			// create nwa
			coalesced.addInitialState(state1);
			coalesced.addFinalState(state4);
			//Diamond
			coalesced.addInternalTrans(state1, EPSILON, state2);
			coalesced.addInternalTrans(state1, lab2, state2);
			coalesced.addInternalTrans(state1, WILD, state3);
			coalesced.addInternalTrans(state2, lab2, state4);
			coalesced.addInternalTrans(state2, EPSILON, state4);
			coalesced.addInternalTrans(state3, WILD, state4);

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageSubsetEq( nwa ,out ) );
			EXPECT_TRUE( opennwa::query::languageEquals( coalesced ,out ) );
			EXPECT_FALSE( opennwa::query::statesOverlap(out, nwa) );

		}

		TEST_F(opennwa$construct$$quotient$$$DoubleDiamondMultipleInitialStates, DoubleDiamondMultipleInitialStatesSelfLoop)
		{
			nwa.addInternalTrans(s5, lab2, s1);

			wali::util::DisjointSets<State> partition;

			partition.merge_sets(s1, s5);
			partition.merge_sets(s2, s6);
			partition.merge_sets(s3, s7);
			partition.merge_sets(s4, s8);

			Nwa coalesced;

			// create states
			State state1 = getKey("state1");
			State state2 = getKey("state2"); 
			State state3 = getKey("state3"); 
			State state4 = getKey("state4");

			// create nwa
			coalesced.addInitialState(state1);
			coalesced.addFinalState(state4);
			//Diamond with self loop
			coalesced.addInternalTrans(state1, lab1, state2);
			coalesced.addInternalTrans(state1, lab1, state3);
			coalesced.addInternalTrans(state2, lab1, state4);
			coalesced.addInternalTrans(state3, lab1, state4);
			coalesced.addInternalTrans(state1, lab2, state2);
			coalesced.addInternalTrans(state1, lab2, state3);
			coalesced.addInternalTrans(state2, lab2, state4);
			coalesced.addInternalTrans(state3, lab2, state4);
			coalesced.addInternalTrans(state1, lab2, state1);

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageSubsetEq( nwa ,out ) );
			EXPECT_TRUE( opennwa::query::languageEquals( coalesced ,out ) );
			EXPECT_FALSE( opennwa::query::statesOverlap(out, nwa) );

		}

		TEST_F(opennwa$construct$$quotient$$$DoubleDiamondMultipleInitialStates, DoubleDiamondMultipleInitialStatesUnmatchedCall)
		{
			nwa.addCallTrans(s5, lab1, s8);

			wali::util::DisjointSets<State> partition;

			partition.merge_sets(s1, s5);
			partition.merge_sets(s2, s6);
			partition.merge_sets(s3, s7);
			partition.merge_sets(s4, s8);

			Nwa coalesced;

			// create states
			State state1 = getKey("state1");
			State state2 = getKey("state2"); 
			State state3 = getKey("state3"); 
			State state4 = getKey("state4");

			// create nwa
			coalesced.addInitialState(state1);
			coalesced.addFinalState(state4);
			//Diamond with self loop
			coalesced.addInternalTrans(state1, lab1, state2);
			coalesced.addInternalTrans(state1, lab1, state3);
			coalesced.addInternalTrans(state2, lab1, state4);
			coalesced.addInternalTrans(state3, lab1, state4);
			coalesced.addInternalTrans(state1, lab2, state2);
			coalesced.addInternalTrans(state1, lab2, state3);
			coalesced.addInternalTrans(state2, lab2, state4);
			coalesced.addInternalTrans(state3, lab2, state4);
			coalesced.addCallTrans(state1, lab1, state4);

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageSubsetEq( nwa ,out ) );
			EXPECT_TRUE( opennwa::query::languageEquals( coalesced ,out ) );
			EXPECT_FALSE( opennwa::query::statesOverlap(out, nwa) );

		}

		TEST_F(opennwa$construct$$quotient$$$DoubleDiamondMultipleInitialStatesInterprocedural, DoubleDiamondMultipleInitialStatesInterprocedural)
		{

			wali::util::DisjointSets<State> partition;

			partition.merge_sets(s1, s5);
			partition.merge_sets(s2, s6);
			partition.merge_sets(s3, s7);
			partition.merge_sets(s4, s8);

			Nwa coalesced;

			// create states
			State state1 = getKey("state1");
			State state2 = getKey("state2"); 
			State state3 = getKey("state3"); 
			State state4 = getKey("state4");

			// create nwa
			coalesced.addInitialState(state1);
			coalesced.addFinalState(state4);
			//Diamond 1
			coalesced.addInternalTrans(state1, lab1, state2);
			coalesced.addInternalTrans(state1, lab2, state3);
			coalesced.addInternalTrans(state2, lab2, state4);
			coalesced.addInternalTrans(state3, lab1, state4);

			coalesced.addCallTrans(state3, lab1, state1);
			coalesced.addReturnTrans(state4, state3, lab2, state4);

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageSubsetEq( nwa ,out ) );
			EXPECT_TRUE( opennwa::query::languageEquals( coalesced ,out ) );
			EXPECT_FALSE( opennwa::query::statesOverlap(out, nwa) );

		}

		TEST_F(opennwa$construct$$quotient$$$DoubleDiamondNestedInterprocedural, DoubleDiamondNestedInterprocedural)
		{

			wali::util::DisjointSets<State> partition;

			partition.merge_sets(s1, s5);
			partition.merge_sets(s1, s9);
			partition.merge_sets(s2, s10);
			partition.merge_sets(s3, s11);
			partition.merge_sets(s4, s12);
			partition.merge_sets(s4, s6);
			partition.insert(s7);
			partition.insert(s8);

			Nwa coalesced;

			// create states
			State state1 = getKey("state1");
			State state2 = getKey("state2"); 
			State state3 = getKey("state3"); 
			State state4 = getKey("state4");
			State state7 = getKey("state7");
			State state8 = getKey("state8");

			// create nwa
			coalesced.addInitialState(state1);
			coalesced.addFinalState(state4);
			//Diamond 1
			coalesced.addInternalTrans(state1, lab1, state2);
			coalesced.addInternalTrans(state1, lab1, state3);
			coalesced.addInternalTrans(state2, lab1, state4);
			coalesced.addInternalTrans(state3, lab1, state4);
			coalesced.addInternalTrans(state1, lab2, state2);
			coalesced.addInternalTrans(state1, lab2, state3);
			coalesced.addInternalTrans(state2, lab2, state4);
			coalesced.addInternalTrans(state3, lab2, state4);

			coalesced.addInternalTrans(state7, lab1, state1);
			coalesced.addInternalTrans(state4, lab2, state8);

			coalesced.addCallTrans(state3, lab1, state1);
			coalesced.addCallTrans(state1, lab1, state7);
			coalesced.addReturnTrans(state4, state3, lab2, state4);
			coalesced.addReturnTrans(state8, state1, lab2, state4);

			Nwa out = *quotient( nwa, partition );

			EXPECT_TRUE( opennwa::query::languageSubsetEq( nwa ,out ) );
			EXPECT_TRUE( opennwa::query::languageEquals( coalesced ,out ) );
			EXPECT_FALSE( opennwa::query::statesOverlap(out, nwa) );

		}

		// Tests end ...

	} // end 'namespace opennwa' !!!
} // end 'namespace opennwa' !!!

