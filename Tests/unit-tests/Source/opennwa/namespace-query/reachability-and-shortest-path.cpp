#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/weighted.hpp"
#include "opennwa/WeightGen.hpp"
#include "wali/ShortestPathSemiring.hpp"
#include "wali/ref_ptr.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"

using namespace opennwa;
using namespace std;


namespace opennwa {
	namespace query {


		// The fixture for empty nwa.
		class EmptyNWA : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states


			}

			Nwa nwa;
			// define reachable and unreachable states
			std::set<State> reachable, unreachable;
			std::map<State, unsigned int> shortestPathLengthMap;

		};


		// The fixture for diamond.
		class Diamond : public ::testing::Test {
		protected:

			//	SetUp is called immediately after the constructor (right before each test).
			virtual void SetUp() {

				// create states
				s1 = getKey("s1");
				s2 = getKey("s2"); 
				s3 = getKey("s3"); 
				s4 = getKey("s4");

				lab = getKey("label");

				nwa.addInitialState(s1);
				nwa.addInternalTrans(s1, lab, s2);
				nwa.addInternalTrans(s1, lab, s3);
				nwa.addInternalTrans(s2, lab, s4);
				nwa.addInternalTrans(s3, lab, s4);

			}

			Nwa nwa;
			State s1, s2, s3, s4;
			Symbol lab;
			// define reachable and unreachable states
			std::set<State> reachable, unreachable;
			std::map<State, unsigned int> shortestPathLengthMap;

		};


		// The fixture for diamond and an isolated loop.
		class DiamondIsolatedLoop : public ::testing::Test {
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

				lab = getKey("label");

				nwa.addInitialState(s1);
				nwa.addInternalTrans(s1, lab, s2);
				nwa.addInternalTrans(s1, lab, s3);
				nwa.addInternalTrans(s2, lab, s4);
				nwa.addInternalTrans(s3, lab, s4);
				//isolated loop
				nwa.addInternalTrans(s5, lab, s6); 
				nwa.addInternalTrans(s6, lab, s5); 

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6;
			Symbol lab;
			// define reachable and unreachable states
			std::set<State> reachable, unreachable;
			std::map<State, unsigned int> shortestPathLengthMap;

		};


		// The fixture for diamond and a loop in callee.
		class DiamondLoopInterprocedural : public ::testing::Test {
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

				lab = getKey("label");

				nwa.addInitialState(s1);
				nwa.addInternalTrans(s1, lab, s2);			
				nwa.addInternalTrans(s2, lab, s4);
				nwa.addInternalTrans(s3, lab, s4);
				nwa.addInternalTrans(s4, lab, s3); //backedge!
				nwa.addCallTrans(s3, lab, s5);
				nwa.addReturnTrans(s6, s3, lab, s4);
				// loop in callee
				nwa.addInternalTrans(s5, lab, s6); 
				nwa.addInternalTrans(s6, lab, s5); 

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6;
			Symbol lab;
			// define reachable and unreachable states
			std::set<State> reachable, unreachable;
			std::map<State, unsigned int> shortestPathLengthMap;

		};


		// The fixture for diamond and a loop in callee.
		class DiamondLoopNestedInterprocedural : public ::testing::Test {
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

				lab = getKey("label");

				// create nwa
				nwa.addInitialState(s1);
				nwa.addInternalTrans(s1, lab, s2);		
				nwa.addInternalTrans(s2, lab, s4);
				nwa.addInternalTrans(s3, lab, s4);
				nwa.addInternalTrans(s4, lab, s3); //backedge!
				nwa.addInternalTrans(s7, lab, s8);
				// call and return from main to callee-1
				nwa.addCallTrans(s3, lab, s5);
				nwa.addReturnTrans(s6, s3, lab, s4);
				// call and return from callee-1 to callee-2
				nwa.addCallTrans(s5, lab, s7);
				nwa.addReturnTrans(s8, s5, lab, s6);

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6, s7, s8;
			Symbol lab;
			// define reachable and unreachable states
			std::set<State> reachable, unreachable;
			std::map<State, unsigned int> shortestPathLengthMap;

		};


		// The fixture for diamonds in main and callee-2.
		class DoubleDiamondNestedInterprocedural : public ::testing::Test {
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

				lab = getKey("label");

				// create nwa			
				nwa.addInitialState(s1);
				//Diamond 1
				nwa.addInternalTrans(s1, lab, s2);
				nwa.addInternalTrans(s1, lab, s3);
				nwa.addInternalTrans(s2, lab, s4);
				nwa.addInternalTrans(s3, lab, s4);			
				// Diamond 2
				nwa.addInternalTrans(s9, lab, s10);
				nwa.addInternalTrans(s9, lab, s11);
				nwa.addInternalTrans(s10, lab, s12);
				nwa.addInternalTrans(s11, lab, s12);
				nwa.addInternalTrans(s7, lab, s9);
				nwa.addInternalTrans(s12, lab, s8);
				// call and return from main to callee-1
				nwa.addCallTrans(s3, lab, s5);
				nwa.addReturnTrans(s6, s3, lab, s4);
				// call and return from callee-1 to callee-2
				nwa.addCallTrans(s5, lab, s7);
				nwa.addReturnTrans(s8, s5, lab, s6);

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12;
			Symbol lab;
			// define reachable and unreachable states
			std::set<State> reachable, unreachable;
			std::map<State, unsigned int> shortestPathLengthMap;

		};


		// The fixture for two diamonds having two initial states
		class DoubleDiamondMultipleInitialStates : public ::testing::Test {
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

				lab = getKey("label");

				// create nwa
				nwa.addInitialState(s1);
				nwa.addInitialState(s5);
				//Diamond 1
				nwa.addInternalTrans(s1, lab, s2);
				nwa.addInternalTrans(s1, lab, s3);
				nwa.addInternalTrans(s2, lab, s4);
				nwa.addInternalTrans(s3, lab, s4);			
				// Diamond 2
				nwa.addInternalTrans(s5, lab, s6);
				nwa.addInternalTrans(s5, lab, s7);
				nwa.addInternalTrans(s6, lab, s8);
				nwa.addInternalTrans(s7, lab, s8);

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6, s7, s8;
			Symbol lab;
			// define reachable and unreachable states
			std::set<State> reachable, unreachable;
			std::map<State, unsigned int> shortestPathLengthMap;

		};

		// The fixture for two diamonds having two initial states and interprocedural
		class DoubleDiamondMultipleInitialStatesInterprocedural : public ::testing::Test {
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

				lab = getKey("label");

				// create nwa
				nwa.addInitialState(s1);
				nwa.addInitialState(s5);
				//Diamond 1
				nwa.addInternalTrans(s1, lab, s2);
				nwa.addInternalTrans(s1, lab, s3);			
				// Diamond 2
				nwa.addInternalTrans(s5, lab, s6);
				nwa.addInternalTrans(s5, lab, s7);
				nwa.addInternalTrans(s6, lab, s8);
				nwa.addInternalTrans(s7, lab, s8);
				nwa.addCallTrans(s3, lab, s5);
				nwa.addReturnTrans(s8, s1, lab, s4);

			}

			Nwa nwa;
			State s1, s2, s3, s4, s5, s6, s7, s8;
			Symbol lab;
			// define reachable and unreachable states
			std::set<State> reachable, unreachable;
			std::map<State, unsigned int> shortestPathLengthMap;

		};

		TEST_F(EmptyNWA, EmptyNwaReachability)  
		{
			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

		}

		TEST_F(Diamond, DiamondAllReachable)
		{

			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(Diamond, DiamondAllUnreachableNoInitialState)
		{
			nwa.clearInitialStates();
			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(Diamond, DiamondSomeUnreachable)
		{
			nwa.removeInternalTrans(s1, lab, s3);
			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			unreachable.insert(s3); 
			reachable.insert(s4);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}



		TEST_F(Diamond, DiamondLoopAllReachable)
		{
			nwa.addInternalTrans(s4, lab, s1); //backedge!
			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			reachable.insert(s4);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DiamondIsolatedLoop, DiamondLoopSomeReachable)
		{
			nwa.addInternalTrans(s4, lab, s3); //backedge!
			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			reachable.insert(s4);
			unreachable.insert(s5);
			unreachable.insert(s6);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DiamondLoopInterprocedural, DiamondLoopAllReachableInterprocedural)  
		{

			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			reachable.insert(s4);
			reachable.insert(s5);
			reachable.insert(s6);	

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DiamondLoopNestedInterprocedural, DiamondAllReachableNestedInterprocedural)  
		{

			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			reachable.insert(s4);
			reachable.insert(s5);
			reachable.insert(s6);
			reachable.insert(s7);
			reachable.insert(s8);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DiamondLoopNestedInterprocedural, DiamondSomeReachableNestedInterprocedural)  
		{
			nwa.removeReturnTrans(s8, s5, lab, s6);
			nwa.addReturnTrans(s8, s5, lab, s5);

			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			reachable.insert(s4);
			reachable.insert(s5);
			unreachable.insert(s6);
			reachable.insert(s7);
			reachable.insert(s8);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondAllReachableNestedInterprocedural)  
		{

			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			reachable.insert(s4);
			reachable.insert(s5);
			reachable.insert(s6);
			reachable.insert(s7);
			reachable.insert(s8);
			reachable.insert(s9);
			reachable.insert(s10);
			reachable.insert(s11);
			reachable.insert(s12);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondAllReachableNestedInterproceduralRecursion)  
		{

			nwa.addReturnTrans(s8, s5, lab, s7);

			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			reachable.insert(s4);
			reachable.insert(s5);
			reachable.insert(s6);
			reachable.insert(s7);
			reachable.insert(s8);
			reachable.insert(s9);
			reachable.insert(s10);
			reachable.insert(s11);
			reachable.insert(s12);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondSomeReachableNestedInterproceduralRecursion2)  
		{

			nwa.removeReturnTrans(s8, s5, lab, s6);
			// call and return from callee-1 to callee-2 with distorted call predecessor
			nwa.addReturnTrans(s8, s2, lab, s6);
			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			reachable.insert(s4);
			reachable.insert(s5);
			unreachable.insert(s6);
			reachable.insert(s7);
			reachable.insert(s8);
			reachable.insert(s9);
			reachable.insert(s10);
			reachable.insert(s11);
			reachable.insert(s12);	

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DoubleDiamondMultipleInitialStates, DoubleDiamondMultipleInitialStatesAllReachable)  
		{

			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			reachable.insert(s4);
			reachable.insert(s5); 
			reachable.insert(s6); 
			reachable.insert(s7); 
			reachable.insert(s8);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DoubleDiamondMultipleInitialStatesInterprocedural, DoubleDiamondMultipleInitialStatesSomeUnreachableInterprocedural)  
		{

			// define reachable and unreachable states
			reachable.insert(s1); 
			reachable.insert(s2); 
			reachable.insert(s3); 
			unreachable.insert(s4);
			reachable.insert(s5); 
			reachable.insert(s6); 
			reachable.insert(s7); 
			reachable.insert(s8);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}


		TEST_F(EmptyNWA, EmptyNwaReachabilityInitWeightZero)  
		{
			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

		}

		TEST_F(Diamond, DiamondAllUnreachableInitWeightZero)
		{

			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(Diamond, DiamondAllUnreachableNoInitialStateInitWeightZero)
		{
			nwa.clearInitialStates();
			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}


		TEST_F(Diamond, DiamondLoopAllReachableInitWeightZero)
		{
			nwa.addInternalTrans(s4, lab, s1); //backedge!
			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DiamondIsolatedLoop, DiamondLoopAllUnreachableInitWeightZero)
		{
			nwa.addInternalTrans(s4, lab, s3); //backedge!
			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);
			unreachable.insert(s5);
			unreachable.insert(s6);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DiamondLoopInterprocedural, DiamondLoopAllUnreachableInterproceduralInitWeightZero)  
		{

			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);
			unreachable.insert(s5);
			unreachable.insert(s6);	

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DiamondLoopNestedInterprocedural, DiamondAllUnreachableNestedInterproceduralInitWeightZero)  
		{

			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);
			unreachable.insert(s5);
			unreachable.insert(s6);
			unreachable.insert(s7);
			unreachable.insert(s8);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondAllUnreachableNestedInterproceduralInitWeightZero)  
		{

			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);
			unreachable.insert(s5);
			unreachable.insert(s6);
			unreachable.insert(s7);
			unreachable.insert(s8);
			unreachable.insert(s9);
			unreachable.insert(s10);
			unreachable.insert(s11);
			unreachable.insert(s12);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondAllUnreachableNestedInterproceduralRecursionInitWeightZero)  
		{

			nwa.addReturnTrans(s8, s5, lab, s7);

			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);
			unreachable.insert(s5);
			unreachable.insert(s6);
			unreachable.insert(s7);
			unreachable.insert(s8);
			unreachable.insert(s9);
			unreachable.insert(s10);
			unreachable.insert(s11);
			unreachable.insert(s12);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondAllUnreachableNestedInterproceduralRecursion2InitWeightZero)  
		{

			nwa.removeReturnTrans(s8, s5, lab, s6);
			// call and return from callee-1 to callee-2 with distorted call predecessor
			nwa.addReturnTrans(s8, s2, lab, s6);
			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);
			unreachable.insert(s5);
			unreachable.insert(s6);
			unreachable.insert(s7);
			unreachable.insert(s8);
			unreachable.insert(s9);
			unreachable.insert(s10);
			unreachable.insert(s11);
			unreachable.insert(s12);	

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DoubleDiamondMultipleInitialStates, DoubleDiamondMultipleInitialStatesAllUnreachableInitWeightZero)  
		{

			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);
			unreachable.insert(s5); 
			unreachable.insert(s6); 
			unreachable.insert(s7); 
			unreachable.insert(s8);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}

		TEST_F(DoubleDiamondMultipleInitialStatesInterprocedural, DoubleDiamondMultipleInitialStatesAllUnreachableInterproceduralInitWeightZero)  
		{

			// define reachable and unreachable states
			unreachable.insert(s1); 
			unreachable.insert(s2); 
			unreachable.insert(s3); 
			unreachable.insert(s4);
			unreachable.insert(s5); 
			unreachable.insert(s6); 
			unreachable.insert(s7); 
			unreachable.insert(s8);

			ReachGen rg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, rg, rg.getOne()->zero());

			// check reachability
			for (std::set<State>::const_iterator it=reachable.begin(); it!=reachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()) );
			}

			for (std::set<State>::const_iterator it=unreachable.begin(); it!=unreachable.end(); ++it) {
				EXPECT_TRUE( stateWeightMap[*it]->equal(rg.getOne()->zero()) );
			}

		}


		TEST_F(EmptyNWA, EmptyNwaShortestPath)  
		{
			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

		}

		TEST_F(Diamond, DiamondShortestPath)
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = 0u;
			shortestPathLengthMap[s2] = 1u;
			shortestPathLengthMap[s3] = 1u;
			shortestPathLengthMap[s4] = 2u;


			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}



		TEST_F(Diamond, DiamondNoInitialStateShortestPath)
		{
			nwa.clearInitialStates();

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(Diamond, DiamondLoopShortestPath)
		{
			nwa.addInternalTrans(s4, lab, s1); //backedge!

			// define path lengths for all states
			shortestPathLengthMap[s1] = 0u;
			shortestPathLengthMap[s2] = 1u;
			shortestPathLengthMap[s3] = 1u;
			shortestPathLengthMap[s4] = 2u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DiamondIsolatedLoop, DiamondLoopShortestPath)
		{
			nwa.addInternalTrans(s4, lab, s3); //backedge!

			// define path lengths for all states
			shortestPathLengthMap[s1] = 0u;
			shortestPathLengthMap[s2] = 1u;
			shortestPathLengthMap[s3] = 1u;
			shortestPathLengthMap[s4] = 2u;
			shortestPathLengthMap[s5] = -1u;
			shortestPathLengthMap[s6] = -1u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DiamondLoopInterprocedural, DiamondLoopInterproceduralShortestPath)  
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = 0u;
			shortestPathLengthMap[s2] = 1u;
			shortestPathLengthMap[s3] = 3u;
			shortestPathLengthMap[s4] = 2u;
			shortestPathLengthMap[s5] = 4u;
			shortestPathLengthMap[s6] = 5u;	

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DiamondLoopNestedInterprocedural, DiamondNestedInterproceduralShortestPath)  
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = 0u;
			shortestPathLengthMap[s2] = 1u;
			shortestPathLengthMap[s3] = 3u;
			shortestPathLengthMap[s4] = 2u;
			shortestPathLengthMap[s5] = 4u;
			shortestPathLengthMap[s6] = 7u;
			shortestPathLengthMap[s7] = 5u;
			shortestPathLengthMap[s8] = 6u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondNestedInterproceduralShortestPath)  
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = 0u;
			shortestPathLengthMap[s2] = 1u;
			shortestPathLengthMap[s3] = 1u;
			shortestPathLengthMap[s4] = 2u;
			shortestPathLengthMap[s5] = 2u;
			shortestPathLengthMap[s6] = 8u;
			shortestPathLengthMap[s7] = 3u;
			shortestPathLengthMap[s8] = 7u;
			shortestPathLengthMap[s9] = 4u;
			shortestPathLengthMap[s10] = 5u;
			shortestPathLengthMap[s11] = 5u;
			shortestPathLengthMap[s12] = 6u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondNestedInterproceduralRecursionShortestPath)  
		{

			nwa.addReturnTrans(s8, s5, lab, s7);

			// define path lengths for all states
			shortestPathLengthMap[s1] = 0u;
			shortestPathLengthMap[s2] = 1u;
			shortestPathLengthMap[s3] = 1u;
			shortestPathLengthMap[s4] = 2u;
			shortestPathLengthMap[s5] = 2u;
			shortestPathLengthMap[s6] = 8u;
			shortestPathLengthMap[s7] = 3u;
			shortestPathLengthMap[s8] = 7u;
			shortestPathLengthMap[s9] = 4u;
			shortestPathLengthMap[s10] = 5u;
			shortestPathLengthMap[s11] = 5u;
			shortestPathLengthMap[s12] = 6u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondNestedInterproceduralRecursion2ShortestPath)  
		{

			nwa.removeReturnTrans(s8, s5, lab, s6);
			// call and return from callee-1 to callee-2 with distorted call predecessor
			nwa.addReturnTrans(s8, s2, lab, s6);

			// define path lengths for all states
			shortestPathLengthMap[s1] = 0u;
			shortestPathLengthMap[s2] = 1u;
			shortestPathLengthMap[s3] = 1u;
			shortestPathLengthMap[s4] = 2u;
			shortestPathLengthMap[s5] = 2u;
			shortestPathLengthMap[s6] = -1u;
			shortestPathLengthMap[s7] = 3u;
			shortestPathLengthMap[s8] = 7u;
			shortestPathLengthMap[s9] = 4u;
			shortestPathLengthMap[s10] = 5u;
			shortestPathLengthMap[s11] = 5u;
			shortestPathLengthMap[s12] = 6u;	

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}
		}

		TEST_F(DoubleDiamondMultipleInitialStates, DoubleDiamondMultipleInitialStatesShortestPath)  
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = 0u;
			shortestPathLengthMap[s2] = 1u;
			shortestPathLengthMap[s3] = 1u;
			shortestPathLengthMap[s4] = 2u;
			shortestPathLengthMap[s5] = 0u;
			shortestPathLengthMap[s6] = 1u;
			shortestPathLengthMap[s7] = 1u;
			shortestPathLengthMap[s8] = 2u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DoubleDiamondMultipleInitialStatesInterprocedural, DoubleDiamondMultipleInitialStatesInterproceduralShortestPath)  
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = 0u;
			shortestPathLengthMap[s2] = 1u;
			shortestPathLengthMap[s3] = 1u;
			shortestPathLengthMap[s4] = -1u;
			shortestPathLengthMap[s5] = 0u;
			shortestPathLengthMap[s6] = 1u;
			shortestPathLengthMap[s7] = 1u;
			shortestPathLengthMap[s8] = 2u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(EmptyNWA, EmptyNwaShortestPathInitWeightZero)  
		{
			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

		}

		TEST_F(Diamond, DiamondShortestPathInitWeightZero)
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;


			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}



		TEST_F(Diamond, DiamondNoInitialStateShortestPathInitWeightZero)
		{
			nwa.clearInitialStates();

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(Diamond, DiamondLoopShortestPathInitWeightZero)
		{
			nwa.addInternalTrans(s4, lab, s1); //backedge!

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DiamondIsolatedLoop, DiamondLoopShortestPathInitWeightZero)
		{
			nwa.addInternalTrans(s4, lab, s3); //backedge!

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;
			shortestPathLengthMap[s5] = -1u;
			shortestPathLengthMap[s6] = -1u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DiamondLoopInterprocedural, DiamondLoopInterproceduralShortestPathInitWeightZero)  
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;
			shortestPathLengthMap[s5] = -1u;
			shortestPathLengthMap[s6] = -1u;	

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DiamondLoopNestedInterprocedural, DiamondNestedInterproceduralShortestPathInitWeightZero)  
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;
			shortestPathLengthMap[s5] = -1u;
			shortestPathLengthMap[s6] = -1u;
			shortestPathLengthMap[s7] = -1u;
			shortestPathLengthMap[s8] = -1u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondNestedInterproceduralShortestPathInitWeightZero)  
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;
			shortestPathLengthMap[s5] = -1u;
			shortestPathLengthMap[s6] = -1u;
			shortestPathLengthMap[s7] = -1u;
			shortestPathLengthMap[s8] = -1u;
			shortestPathLengthMap[s9] = -1u;
			shortestPathLengthMap[s10] = -1u;
			shortestPathLengthMap[s11] = -1u;
			shortestPathLengthMap[s12] = -1u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondNestedInterproceduralRecursionShortestPathInitWeightZero)  
		{

			nwa.addReturnTrans(s8, s5, lab, s7);

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;
			shortestPathLengthMap[s5] = -1u;
			shortestPathLengthMap[s6] = -1u;
			shortestPathLengthMap[s7] = -1u;
			shortestPathLengthMap[s8] = -1u;
			shortestPathLengthMap[s9] = -1u;
			shortestPathLengthMap[s10] = -1u;
			shortestPathLengthMap[s11] = -1u;
			shortestPathLengthMap[s12] = -1u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DoubleDiamondNestedInterprocedural, DoubleDiamondNestedInterproceduralRecursion2ShortestPathInitWeightZero)  
		{

			nwa.removeReturnTrans(s8, s5, lab, s6);
			// call and return from callee-1 to callee-2 with distorted call predecessor
			nwa.addReturnTrans(s8, s2, lab, s6);

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;
			shortestPathLengthMap[s5] = -1u;
			shortestPathLengthMap[s6] = -1u;
			shortestPathLengthMap[s7] = -1u;
			shortestPathLengthMap[s8] = -1u;
			shortestPathLengthMap[s9] = -1u;
			shortestPathLengthMap[s10] = -1u;
			shortestPathLengthMap[s11] = -1u;
			shortestPathLengthMap[s12] = -1u;	

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}
		}

		TEST_F(DoubleDiamondMultipleInitialStates, DoubleDiamondMultipleInitialStatesShortestPathInitWeightZero)  
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;
			shortestPathLengthMap[s5] = -1u;
			shortestPathLengthMap[s6] = -1u;
			shortestPathLengthMap[s7] = -1u;
			shortestPathLengthMap[s8] = -1u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}

		TEST_F(DoubleDiamondMultipleInitialStatesInterprocedural, DoubleDiamondMultipleInitialStatesInterproceduralShortestPathInitWeightZero)  
		{

			// define path lengths for all states
			shortestPathLengthMap[s1] = -1u;
			shortestPathLengthMap[s2] = -1u;
			shortestPathLengthMap[s3] = -1u;
			shortestPathLengthMap[s4] = -1u;
			shortestPathLengthMap[s5] = -1u;
			shortestPathLengthMap[s6] = -1u;
			shortestPathLengthMap[s7] = -1u;
			shortestPathLengthMap[s8] = -1u;

			ShortestPathGen spg;
			// run analysis
			std::map<State, sem_elem_t> stateWeightMap = doForwardAnalysis(nwa, spg, spg.getOne()->zero());

			// check reachability
			for (std::map<State, unsigned int>::const_iterator mit=shortestPathLengthMap.begin(); mit!=shortestPathLengthMap.end(); ++mit) {
				sem_elem_t weight = stateWeightMap[mit->first];
				wali::ShortestPathSemiring * shortestPathLengthPtr = dynamic_cast< wali::ShortestPathSemiring *>(weight.get_ptr());
				EXPECT_EQ( shortestPathLengthMap[mit->first], shortestPathLengthPtr->getNum());
			}

		}


	} // end namespace query

} // end namespace opennwa
