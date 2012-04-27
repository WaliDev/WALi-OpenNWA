#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"

namespace opennwa
{
        //////////////////////////////////
        // Supporting stuff

        
        /// All of these functions add 'state' to 'nwa', and checks that it
        /// is present in the appropriate sets and only those sets. (Symbol
        /// does the same with 'symbol'.) 'expect_addition' controls whether
        /// the state should be added.

        void
        add_to_states_and_check(Nwa * nwa, State state, bool expect_addition)
        {
            size_t original_size_trans = nwa->sizeTrans();
            
            EXPECT_EQ(expect_addition, nwa->addState(state));

            EXPECT_TRUE(nwa->isState(state));
            EXPECT_FALSE(nwa->isInitialState(state));
            EXPECT_FALSE(nwa->isFinalState(state));
            EXPECT_FALSE(nwa->isSymbol(state));

            EXPECT_EQ(original_size_trans, nwa->sizeTrans());
        }
        
        void
        add_to_initial_states_and_check(Nwa * nwa, State state, bool expect_addition)
        {
            size_t original_size_trans = nwa->sizeTrans();
            
            EXPECT_EQ(expect_addition, nwa->addInitialState(state));

            EXPECT_TRUE(nwa->isState(state));
            EXPECT_TRUE(nwa->isInitialState(state));
            EXPECT_FALSE(nwa->isFinalState(state));
            EXPECT_FALSE(nwa->isSymbol(state));

            EXPECT_EQ(original_size_trans, nwa->sizeTrans());
        }

        void
        add_to_final_states_and_check(Nwa * nwa, State state, bool expect_addition)
        {
            size_t original_size_trans = nwa->sizeTrans();
            
            EXPECT_EQ(expect_addition, nwa->addFinalState(state));

            EXPECT_TRUE(nwa->isState(state));
            EXPECT_FALSE(nwa->isInitialState(state));
            EXPECT_TRUE(nwa->isFinalState(state));
            EXPECT_FALSE(nwa->isSymbol(state));

            EXPECT_EQ(original_size_trans, nwa->sizeTrans());
        }
        
        void
        add_to_symbols_and_check(Nwa * nwa, Symbol symbol, bool expect_addition)
        {
            size_t original_size_trans = nwa->sizeTrans();
            
            EXPECT_EQ(expect_addition, nwa->addSymbol(symbol));

            EXPECT_FALSE(nwa->isState(symbol));
            EXPECT_FALSE(nwa->isInitialState(symbol));
            EXPECT_FALSE(nwa->isFinalState(symbol));
            EXPECT_TRUE(nwa->isSymbol(symbol));

            EXPECT_EQ(original_size_trans, nwa->sizeTrans());
        }
        

        /// Checks that, in 'nwa', the sizes returned by 'sizeBlah()' is the
        /// same as the number of elements between 'beginBlah()' and
        /// 'endBlah()'.
        void expect_size_consistent_with_range(Nwa const & nwa)
        {
#define CHECK_CONSISTENCY(Attribute)                                    \
            EXPECT_EQ(nwa.size##Attribute(),                            \
                      static_cast<size_t>(std::distance(nwa.begin##Attribute(), \
                                                        nwa.end##Attribute())))
            
            CHECK_CONSISTENCY(States);
            CHECK_CONSISTENCY(InitialStates);
            CHECK_CONSISTENCY(FinalStates);
            CHECK_CONSISTENCY(Symbols);
            CHECK_CONSISTENCY(InternalTrans);
            CHECK_CONSISTENCY(CallTrans);
            CHECK_CONSISTENCY(ReturnTrans);
#undef CHECK_CONSISTENCY
        }


        void expect_nwas_nearly_the_same(Nwa const & reference,
                                         Nwa const & nwa,
                                         bool states,
                                         bool initials,
                                         bool finals,
                                         bool symbols,
                                         bool internals,
                                         bool calls,
                                         bool returns)
        {
            std::vector<Nwa::Internal>
                reference_internals(reference.beginInternalTrans(), reference.endInternalTrans()),
                nwa_internals(nwa.beginInternalTrans(), nwa.endInternalTrans());
            std::vector<Nwa::Call>
                reference_calls(reference.beginCallTrans(), reference.endCallTrans()),
                nwa_calls(nwa.beginCallTrans(), nwa.endCallTrans());
            std::vector<Nwa::Return>
                reference_returns(reference.beginReturnTrans(), reference.endReturnTrans()),
                nwa_returns(nwa.beginReturnTrans(), nwa.endReturnTrans());

            if (states)   EXPECT_EQ(reference.getStates(), nwa.getStates());
            if (initials) EXPECT_EQ(reference.getInitialStates(), nwa.getInitialStates());
            if (finals)   EXPECT_EQ(reference.getFinalStates(), nwa.getFinalStates());
            
            if (symbols)  EXPECT_EQ(reference.getSymbols(), nwa.getSymbols());
      
            if (internals) EXPECT_EQ(reference_internals, nwa_internals);
            if (calls)     EXPECT_EQ(reference_calls, nwa_calls);
            if (returns)   EXPECT_EQ(reference_returns, nwa_returns);
        }

        
        /////////////////////////////////
        // Now begin the actual tests
        

        // For XXX in each of States, InitialStates, FinalStates, InternalTrans,
        // CallTrans, ReturnTrans:
        // 
        //   getXXX()
        //     - I don't think we can really do anything with this separately. This
        //       is tested in concert with other functions except as follows:
        // 
        //     - For getSymbols(), check in an automaton that uses epsilon and wild on
        //       transitions. Make sure that neither is present in the given set.
        TEST(opennwa$Nwa$getSymbols, epsilonAndWildAreNotPresent)
        {
            Nwa nwa;
            Key q = getKey('q');
            nwa.addInternalTrans(q, EPSILON, q);
            nwa.addInternalTrans(q, WILD, q);
            nwa.addCallTrans(q, WILD, q);
            nwa.addReturnTrans(q, q, WILD, q);


            // Sanity check
            EXPECT_EQ(2u, nwa.sizeInternalTrans());

            // Actual check (see checkSizeOfEmptyIsConsistent below)
            EXPECT_EQ(0u, nwa.sizeSymbols());
        }


        //////////////////
        //   sizeXXX()
        //     - Make sure this agrees with the size of getXXX()       
        TEST(opennwa$Nwa$getXXX, checkSizeConsistentForEmptyNwa)
        {
            Nwa empty;
            expect_size_consistent_with_range(empty);
        }

        TEST(opennwa$Nwa$getXXX, checkSizeConsistentForBigNwa)
        {
            OddNumEvenGroupsNwa fixture;
            expect_size_consistent_with_range(fixture.nwa);
        }


        //////////////////
        //   isXXX() and addXXX() for states and symbols
        //
        //     - Check if an XXX is a member of the empty Nwa       
        TEST(opennwa$Nwa$isXXX, checkNothingMemberOfEmpty)
        {
            Nwa empty;
            SomeElements::expect_not_present(empty);
        }

        //     - Add unrelated items of each type; check.
        TEST(opennwa$Nwa$isXXX, checkNothingMemberOfStuff)
        {
            OddNumEvenGroupsNwa fixture;
            SomeElements::expect_not_present(fixture.nwa);
        }

        
        //     - Add item being checked to an empty NWA; check
        //     - Check that adding an XXX adds it to the set returned by
        //       getXXX and returns true       
        TEST(opennwa$Nwa$isState$$and$addState, addToEmptyAndCheck)
        {
            Nwa empty;
            SomeElements e;
            add_to_states_and_check(&empty, e.state, true);
        }
        
        TEST(opennwa$Nwa$isInitialState$$and$addInitialState, addToEmptyAndCheck)
        {
            Nwa empty;
            SomeElements e;
            add_to_initial_states_and_check(&empty, e.state, true);
        }

        TEST(opennwa$Nwa$isFinalState$$and$addFinalState, addToEmptyAndCheck)
        {
            Nwa empty;
            SomeElements e;
            add_to_final_states_and_check(&empty, e.state, true);
        }

        TEST(opennwa$Nwa$isSymbol$$and$addSymbol, addToEmptyAndCheck)
        {
            Nwa empty;
            SomeElements e;
            add_to_symbols_and_check(&empty, e.symbol, true);
        }

        
        //     - Add both unrelated and checked item, then more unrelated
        //       items; check. (I'm not doing the "then more
        //       unrelated". TODO?)
        TEST(opennwa$Nwa$isState$$and$addState, addToBusyAndCheck)
        {
            OddNumEvenGroupsNwa fixture;
            SomeElements e;
            add_to_states_and_check(&fixture.nwa, e.state, true);
        }
        
        TEST(opennwa$Nwa$isInitialState$$and$addInitialState, addToBusyAndCheck)
        {
            OddNumEvenGroupsNwa fixture;
            SomeElements e;
            add_to_initial_states_and_check(&fixture.nwa, e.state, true);
        }

        TEST(opennwa$Nwa$isFinalState$$and$addFinalState, addToBusyAndCheck)
        {
            OddNumEvenGroupsNwa fixture;
            SomeElements e;
            add_to_final_states_and_check(&fixture.nwa, e.state, true);
        }

        TEST(opennwa$Nwa$isSymbol$$and$addSymbol, addToBusyAndCheck)
        {
            OddNumEvenGroupsNwa fixture;
            SomeElements e;
            add_to_symbols_and_check(&fixture.nwa, e.symbol, true);
        }

        
        //     - For isSymbol(), make sure epsilon and wild return false both when they
        //       are used in transitions and not.       
        // This is done partially inline with the above

        TEST(opennwa$Nwa$isSymbol, epsilonAndWildAreNotSymbolsAccordingToIsSymbol)
        {
            OddNumEvenGroupsNwa fixture;

            EXPECT_FALSE(fixture.nwa.isSymbol(EPSILON));
            EXPECT_FALSE(fixture.nwa.isSymbol(WILD));

            fixture.nwa.addInternalTrans(fixture.q0, EPSILON, fixture.q1);
            fixture.nwa.addInternalTrans(fixture.q0, WILD, fixture.q1);
            fixture.nwa.addCallTrans(fixture.q0, WILD, fixture.q1);
            fixture.nwa.addReturnTrans(fixture.q0, fixture.q2, WILD, fixture.q1);

            EXPECT_FALSE(fixture.nwa.isSymbol(EPSILON));
            EXPECT_FALSE(fixture.nwa.isSymbol(WILD));
        }
        

        //   addXXX()
        //     - Check that adding it again leaves the return from getXXX
        //       unchanged, and returns false
        TEST(opennwa$Nwa$addState, addTwiceToEmptyAndCheck)
        {
            Nwa nwa;
            SomeElements e;
            add_to_states_and_check(&nwa, e.state, true);

            Nwa copy = nwa;
            add_to_states_and_check(&nwa, e.state, false);
            
            expect_nwas_are_equal(nwa, copy);
        }
        
        TEST(opennwa$Nwa$addInitialState, addTwiceToEmptyAndCheck)
        {
            Nwa nwa;
            SomeElements e;
            add_to_initial_states_and_check(&nwa, e.state, true);

            Nwa copy = nwa;
            add_to_initial_states_and_check(&nwa, e.state, false);

            expect_nwas_are_equal(nwa, copy);
        }

        TEST(opennwa$Nwa$addFinalState, addTwiceToNwaAndCheck)
        {
            Nwa nwa;
            SomeElements e;
            add_to_final_states_and_check(&nwa, e.state, true);

            Nwa copy = nwa;
            add_to_final_states_and_check(&nwa, e.state, false);

            expect_nwas_are_equal(nwa, copy);
        }

        TEST(opennwa$Nwa$addSymbol, addTwiceToEmptyAndCheck)
        {
            Nwa nwa;
            SomeElements e;
            add_to_symbols_and_check(&nwa, e.symbol, true);

            Nwa copy = nwa;
            add_to_symbols_and_check(&nwa, e.symbol, false);

            expect_nwas_are_equal(nwa, copy);
        }

        
        //     - Check that the sets that should not be modified are not
        //       modified.
        //
        // Done inline, mostly. (I don't check that transitions are unmodified.)

        
        //     - For addInitialState and addFinalState, try from both the
        //       situation where the state was and wasn't present in the NWA
        //       at all. If they weren't present, check that they are after
        //       the addition.
        //
        // The above checks the situation where it was not present, and
        // checks that it was added.
        TEST(opennwa$Nwa$addInitialState, addInitialStateOfAStateAlreadyPresent)
        {
            Nwa nwa;
            SomeElements e;
            EXPECT_TRUE(nwa.addState(e.state));

            // Sanity checks; these should hold from above
            EXPECT_TRUE(nwa.isState(e.state));
            EXPECT_FALSE(nwa.isInitialState(e.state));

            // ** Now the real thing. Add two initial states.
            EXPECT_TRUE(nwa.addInitialState(e.state));
            EXPECT_TRUE(nwa.addInitialState(e.state2));

            // ** Make sure both are initial states
            EXPECT_TRUE(nwa.isInitialState(e.state));
            EXPECT_TRUE(nwa.isInitialState(e.state2));

            EXPECT_EQ(2u, nwa.sizeInitialStates());
        }
        

        //     - For addSymbol(), make sure adding epsilon or wild returns false even if
        //       they are the first things done
        TEST(opennwa$Nwa$addSymbol, addingEpsilonOrWildShouldFail)
        {
            Nwa nwa;

            EXPECT_FALSE(nwa.addSymbol(EPSILON));
            EXPECT_FALSE(nwa.addSymbol(WILD));
        }

        
        //   removeXXX()
        //     - Try from empty automaton, one containing given item, one not
        //       containing given item. Check correct value is returned.
        TEST(opennwa$Nwa$removeXXX, removingFromEmptyShouldFail)
        {
            Nwa nwa;
            SomeElements e;

            EXPECT_FALSE(nwa.removeState(e.state));
            EXPECT_FALSE(nwa.removeInitialState(e.state));
            EXPECT_FALSE(nwa.removeFinalState(e.state));
            EXPECT_FALSE(nwa.removeSymbol(e.symbol));
            EXPECT_FALSE(nwa.removeInternalTrans(e.internal));
            EXPECT_FALSE(nwa.removeCallTrans(e.call));
            EXPECT_FALSE(nwa.removeReturnTrans(e.ret));
        }

        TEST(opennwa$Nwa$removeXXX, removingSingleItemsShouldLeaveEmpty)
        {
            Nwa nwa;
            SomeElements e;
            SomeElements::add_to_nwa(&nwa);

            // Couple sanity checks to make sure the SomeElements NWA is what
            // I think it is.
            ASSERT_TRUE(nwa.isInitialState(e.state));
            ASSERT_TRUE(nwa.isFinalState(e.state2));

            // Have to remove transitions first so they don't remove states
            EXPECT_TRUE(nwa.removeInternalTrans(e.internal));
            EXPECT_EQ(0u, nwa.sizeInternalTrans());
            EXPECT_EQ(1u, nwa.sizeCallTrans());
            EXPECT_EQ(1u, nwa.sizeReturnTrans());

            EXPECT_TRUE(nwa.removeCallTrans(e.call));
            EXPECT_EQ(0u, nwa.sizeInternalTrans());
            EXPECT_EQ(0u, nwa.sizeCallTrans());
            EXPECT_EQ(1u, nwa.sizeReturnTrans());

            EXPECT_TRUE(nwa.removeReturnTrans(e.ret));
            EXPECT_EQ(0u, nwa.sizeInternalTrans());
            EXPECT_EQ(0u, nwa.sizeCallTrans());
            EXPECT_EQ(0u, nwa.sizeReturnTrans());

            // Check up on stuff.
            EXPECT_EQ(0u, nwa.sizeTrans());
            EXPECT_EQ(1u, nwa.sizeSymbols());
            EXPECT_EQ(1u, nwa.sizeInitialStates());
            EXPECT_EQ(1u, nwa.sizeFinalStates());
            EXPECT_EQ(3u, nwa.sizeStates());

            // Remove symbol
            EXPECT_TRUE(nwa.removeSymbol(e.symbol));
            EXPECT_EQ(0u, nwa.sizeSymbols());

            // Have to remove initial and final first so removing a state
            // doesn't disturb anything. 
            EXPECT_TRUE(nwa.removeInitialState(e.state));
            EXPECT_EQ(0u, nwa.sizeInitialStates());
            EXPECT_EQ(1u, nwa.sizeFinalStates());
            EXPECT_EQ(3u, nwa.sizeStates());

            EXPECT_TRUE(nwa.removeFinalState(e.state2));
            EXPECT_EQ(0u, nwa.sizeInitialStates());
            EXPECT_EQ(0u, nwa.sizeFinalStates());
            EXPECT_EQ(3u, nwa.sizeStates());

            EXPECT_TRUE(nwa.removeState(e.state));
            EXPECT_EQ(2u, nwa.sizeStates());
            
            EXPECT_TRUE(nwa.removeState(e.state2));
            EXPECT_EQ(1u, nwa.sizeStates());

            EXPECT_TRUE(nwa.removeState(e.state3));

            expect_nwa_is_empty(nwa);
        }

        TEST(opennwa$Nwa$removeXXX, removingItemNotInNwaShouldLeaveItUnchanged)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;
            SomeElements e;

            EXPECT_FALSE(nwa.removeInternalTrans(e.internal));
            EXPECT_FALSE(nwa.removeCallTrans(e.call));
            EXPECT_FALSE(nwa.removeReturnTrans(e.ret));
            EXPECT_FALSE(nwa.removeSymbol(e.symbol));
            EXPECT_FALSE(nwa.removeInitialState(e.state));
            EXPECT_FALSE(nwa.removeFinalState(e.state2));
            EXPECT_FALSE(nwa.removeState(e.state));
            EXPECT_FALSE(nwa.removeState(e.state2));
            EXPECT_FALSE(nwa.removeState(e.state3));

            expect_nwas_are_equal(fixture.nwa, nwa);
        }

        TEST(opennwa$Nwa$removeXXX, removingItemWhenThereIsMoreThanOneOfThatKindLeavesTheOthers)
        {
            // TODO: this test doesn't really test that the right transitions
            // were removed.
            
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;

            // We'll add a couple more for good fun
            ASSERT_TRUE(nwa.addInitialState(fixture.q2));
            ASSERT_TRUE(nwa.addFinalState(fixture.q0));

            // TheNwa transitions first
            EXPECT_EQ(4u, nwa.sizeInternalTrans());
            EXPECT_TRUE(nwa.removeInternalTrans(fixture.q2, fixture.zero, fixture.q3));
            EXPECT_EQ(3u, nwa.sizeInternalTrans());
            EXPECT_TRUE(nwa.removeInternalTrans(fixture.q2, EPSILON, fixture.dummy));
            EXPECT_EQ(2u, nwa.sizeInternalTrans());

            EXPECT_EQ(3u, nwa.sizeCallTrans());
            EXPECT_TRUE(nwa.removeCallTrans(fixture.q0, fixture.call, fixture.q2));
            EXPECT_EQ(2u, nwa.sizeCallTrans());

            EXPECT_EQ(3u, nwa.sizeReturnTrans());
            EXPECT_TRUE(nwa.removeReturnTrans(fixture.q3, fixture.q1, fixture.ret, fixture.q0));
            EXPECT_EQ(2u, nwa.sizeReturnTrans());

            // Symbols
            EXPECT_EQ(3u, nwa.sizeSymbols());
            EXPECT_TRUE(nwa.removeSymbol(fixture.zero)); // **
            EXPECT_EQ(2u, nwa.sizeSymbols());
            EXPECT_TRUE(nwa.removeSymbol(fixture.call)); // **
            EXPECT_EQ(1u, nwa.sizeSymbols());

            EXPECT_FALSE(nwa.isSymbol(fixture.zero));
            EXPECT_FALSE(nwa.isSymbol(fixture.call));
            EXPECT_TRUE(nwa.isSymbol(fixture.ret));

            // Initial states
            EXPECT_EQ(2u, nwa.sizeInitialStates());
            EXPECT_EQ(2u, nwa.sizeFinalStates());
            
            EXPECT_TRUE(nwa.removeInitialState(fixture.q0)); // **
            
            EXPECT_EQ(1u, nwa.sizeInitialStates());
            EXPECT_EQ(2u, nwa.sizeFinalStates());

            // Final states          
            EXPECT_TRUE(nwa.removeFinalState(fixture.q1)); // **
            EXPECT_EQ(1u, nwa.sizeInitialStates());
            EXPECT_EQ(1u, nwa.sizeFinalStates());

            EXPECT_TRUE(nwa.isInitialState(fixture.q2));
            EXPECT_TRUE(nwa.isFinalState(fixture.q0));

            // States
            EXPECT_EQ(5u, nwa.sizeStates());
            EXPECT_TRUE(nwa.removeState(fixture.dummy)); // **
            EXPECT_EQ(4u, nwa.sizeStates());
            EXPECT_FALSE(nwa.isState(fixture.dummy));
        }
        
        //     - In each case, check that sets that shouldn't change are not modified.
        //
        // This has kinda been done all along...

        //     - For removeState(), make sure that removing a state that is
        //       an initial and/or final state works correctly
        TEST(opennwa$Nwa$removeState, removingAStateRemovesFromInitialsAndFinals)
        {
            OddNumEvenGroupsNwa fixture;

            ASSERT_TRUE(fixture.nwa.addInitialState(fixture.q2)); // q0 and q2 are inital
            ASSERT_TRUE(fixture.nwa.addFinalState(fixture.q0)); // q1 and q1 are accepting
            ASSERT_EQ(2u, fixture.nwa.sizeInitialStates());
            ASSERT_EQ(2u, fixture.nwa.sizeFinalStates());

            // Remove just an initial state
            EXPECT_TRUE(fixture.nwa.removeState(fixture.q2));
            EXPECT_EQ(1u, fixture.nwa.sizeInitialStates());
            EXPECT_EQ(2u, fixture.nwa.sizeFinalStates());

            // Remove just a accepting state
            EXPECT_TRUE(fixture.nwa.removeState(fixture.q1));
            EXPECT_EQ(1u, fixture.nwa.sizeInitialStates());
            EXPECT_EQ(1u, fixture.nwa.sizeFinalStates());

            // Remove a state that is both initial and accepting
            EXPECT_TRUE(fixture.nwa.removeState(fixture.q0));
            EXPECT_EQ(0u, fixture.nwa.sizeInitialStates());
            EXPECT_EQ(0u, fixture.nwa.sizeFinalStates());
        }

        //     - For removeState(), try from automaton in which the state is
        //       and is not involved in any transitions. Make sure you get
        //       the state in all applicable coordinates: source/target of
        //       internal, call/entry of call, and exit/pred/return of
        //       return.
        TEST(opennwa$Nwa$removeState, removingAStateRemovesAssociatedTransitions)
        {
            OddNumEvenGroupsNwa fixture;

            // Add some extra stuff so we can distinguish removing stuff from
            // clearing transitions.
            ASSERT_TRUE(fixture.nwa.addInternalTrans(fixture.q0, EPSILON, fixture.q1));
            ASSERT_TRUE(fixture.nwa.addInternalTrans(fixture.q0, fixture.zero, fixture.q1));
            ASSERT_TRUE(fixture.nwa.addCallTrans(fixture.q0, fixture.call, fixture.q1));

            // Add one transition to get q2 as the predecessor of a return
            // and one to get it as the target of a return
            ASSERT_TRUE(fixture.nwa.addReturnTrans(fixture.q0, fixture.q2, fixture.zero, fixture.q3));
            ASSERT_TRUE(fixture.nwa.addReturnTrans(fixture.q0, fixture.q1, fixture.zero, fixture.q2));

            ASSERT_EQ(6u, fixture.nwa.sizeInternalTrans());
            ASSERT_EQ(4u, fixture.nwa.sizeCallTrans());
            ASSERT_EQ(5u, fixture.nwa.sizeReturnTrans());

            // This removes transitions where q2 is the:
            //
            // Internal: source      (q2 ---> q3 on zero, and q2 ---> dummy on epsilon and wild)
            //           target      (q3 ---> q2 on zero)
            // Call:     source      (q2 ---> dummy)
            //           target      (q0 ---> q2 on call, q1 ---> q2 on call)
            // Return:   source      (q2 ---> dummy on ret (q1 pred))
            //           predecessor (q0 ---> q3 on zero (q2 pred); this was added here)
            //           target      (q0 ---> q2 on zero (q1 pred); this was added here)
            EXPECT_TRUE(fixture.nwa.removeState(fixture.q2));

            // We removed 4 internal, 3 call, and 3 return transitions
            EXPECT_EQ(2u, fixture.nwa.sizeInternalTrans());
            EXPECT_EQ(1u, fixture.nwa.sizeCallTrans());
            EXPECT_EQ(2u, fixture.nwa.sizeReturnTrans());

            
            ///////
            // 'dummy' is no longer involved in any transitions. Removing it
            // should not change anything except the set of states. We'll
            // test this by making a copy of the current NWA, then removing
            // 'dummy' and reinserting it.
            Nwa copy = fixture.nwa;
            
            EXPECT_TRUE(fixture.nwa.removeState(fixture.dummy));

            // Some extra checks...
            EXPECT_EQ(2u, fixture.nwa.sizeInternalTrans());
            EXPECT_EQ(1u, fixture.nwa.sizeCallTrans());
            EXPECT_EQ(2u, fixture.nwa.sizeReturnTrans());

            // Reinsert dummy
            EXPECT_TRUE(fixture.nwa.addState(fixture.dummy));
            expect_nwas_are_equal(copy, fixture.nwa);
        }

        
        //     - For removeSymbol(), try from automaton in which the symbol is and is not
        //       involved in any transtions. Test all three kinds of transitions.
        TEST(opennwa$Nwa$removeSymbol, removingASymbolRemovesAssociatedTransitions)
        {
            OddNumEvenGroupsNwa fixture;

            // Add a transition of 'zero' to the calls and returns, so
            // we can make sure we remove those kinds
            ASSERT_TRUE(fixture.nwa.addCallTrans(fixture.q0, fixture.zero, fixture.q1));
            ASSERT_TRUE(fixture.nwa.addReturnTrans(fixture.q0, fixture.q1, fixture.zero, fixture.q3));

            ASSERT_EQ(4u, fixture.nwa.sizeInternalTrans());
            ASSERT_EQ(4u, fixture.nwa.sizeCallTrans());
            ASSERT_EQ(4u, fixture.nwa.sizeReturnTrans());

            // Now, test removing 'zero' removes those transitions
            EXPECT_TRUE(fixture.nwa.removeSymbol(fixture.zero));

            // Should remove the following transitions:
            //   Internal: q2 ---> q3 on zero
            //             q3 ---> q2 on zero
            //   Call:     q0 ---> q1 on zero (added above)
            //   Return:   q0 ---> q3 on zero (q1 pred; added above)
            EXPECT_EQ(2u, fixture.nwa.sizeInternalTrans());
            EXPECT_EQ(3u, fixture.nwa.sizeCallTrans());
            EXPECT_EQ(3u, fixture.nwa.sizeReturnTrans());
        }

        TEST(opennwa$Nwa$removeSymbol, removingAnUnusedSymbolDoesntDoAnythingElse)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;
            Symbol one = getKey("one");

            ASSERT_TRUE(nwa.addSymbol(one));

            EXPECT_TRUE(nwa.removeSymbol(one));
            expect_nwas_are_equal(fixture.nwa, nwa);
        }


        //   clearXXX()
        //     - Make sure that the set of XXX is empty after.
        //     - Make sure that sets that shouldn't be different are
        //       not different.
        TEST(opennwa$Nwa$clearInitialStates, checkClearingAndPersisting)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;

            nwa.clearInitialStates();

            // Check changes
            EXPECT_EQ(0u, nwa.sizeInitialStates());

            // Check preservation: only initials should have changed
            expect_nwas_nearly_the_same(fixture.nwa, nwa,
                                        true, false, true, // states
                                        true,              // symbols
                                        true, true, true); // transitions
        }

        TEST(opennwa$Nwa$clearFinalStates, checkClearingAndPersisting)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;

            nwa.clearFinalStates();

            // Check changes
            EXPECT_EQ(0u, nwa.sizeFinalStates());

            // Check preservation: only finals should have changed
            expect_nwas_nearly_the_same(fixture.nwa, nwa,
                                        true, true, false, // states
                                        true,              // symbols
                                        true, true, true); // transitions
        }

        //     - For clearStates(), make sure initial/accepting states and
        //       transitions have all been cleared too.
        TEST(opennwa$Nwa$clearStates, checkClearingAndPersisting)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;

            nwa.clearStates();

            // Check changes
            EXPECT_EQ(0u, nwa.sizeStates());
            EXPECT_EQ(0u, nwa.sizeInitialStates());
            EXPECT_EQ(0u, nwa.sizeFinalStates());
            EXPECT_EQ(0u, nwa.sizeTrans());
            EXPECT_EQ(0u, nwa.sizeInternalTrans());
            EXPECT_EQ(0u, nwa.sizeCallTrans());
            EXPECT_EQ(0u, nwa.sizeReturnTrans());

            // Check preservation: there should be no states and no transitions
            expect_nwas_nearly_the_same(fixture.nwa, nwa,
                                        false, false, false,  // states
                                        true,                 // symbols
                                        false, false, false); // transitions
        }

        TEST(opennwa$Nwa$clearSymbols, checkClearingAndPersisting)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;

            nwa.clearSymbols();

            // Check changes
            EXPECT_EQ(0u, nwa.sizeSymbols());
            EXPECT_EQ(0u, nwa.sizeTrans());
            EXPECT_EQ(0u, nwa.sizeInternalTrans());
            EXPECT_EQ(0u, nwa.sizeCallTrans());
            EXPECT_EQ(0u, nwa.sizeReturnTrans());

            // Check preservation: there should be no symbols and no transitions
            expect_nwas_nearly_the_same(fixture.nwa, nwa,
                                        true, true, true,     // states
                                        false,                // symbols
                                        false, false, false); // transitions
        }

        //     - For clearSymbols(), make sure transitions are all clear too.
        TEST(opennwa$Nwa$clearTrans, checkClearingAndPersisting)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;

            nwa.clearTrans();

            // Check changes
            EXPECT_EQ(0u, nwa.sizeTrans());
            EXPECT_EQ(0u, nwa.sizeInternalTrans());
            EXPECT_EQ(0u, nwa.sizeCallTrans());
            EXPECT_EQ(0u, nwa.sizeReturnTrans());

            // Check preservation: only transitions should have changed
            expect_nwas_nearly_the_same(fixture.nwa, nwa,
                                        true, true, true,     // states
                                        true,                 // symbols
                                        false, false, false); // transitions
        }

#if 0   // We don't have these functions right now, but here are some tests
        // in case we one day do.
        
        TEST(opennwa$Nwa$clearInternalTrans, checkClearingAndPersisting)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;

            nwa.clearInternalTrans();

            // Check changes
            EXPECT_EQ(0u, nwa.sizeInternalTrans());

            // Check preservation: only initials should have changed
            expect_nwas_nearly_the_same(fixture.nwa, nwa,
                                        true, true, true,   // states
                                        true,               // symbols
                                        false, true, true); // transitions
        }

        TEST(opennwa$Nwa$clearCallTrans, checkClearingAndPersisting)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;

            nwa.clearCallTrans();

            // Check changes
            EXPECT_EQ(0u, nwa.sizeCallTrans());

            // Check preservation: only initials should have changed
            expect_nwas_nearly_the_same(fixture.nwa, nwa,
                                        true, true, true,   // states
                                        true,               // symbols
                                        true, false, true); // transitions
        }
        
        TEST(opennwa$Nwa$clearReturnTrans, checkClearingAndPersisting)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa nwa = fixture.nwa;

            nwa.clearReturnTrans();

            // Check changes
            EXPECT_EQ(0u, nwa.sizeReturnTrans());

            // Check preservation: only initials should have changed
            expect_nwas_nearly_the_same(fixture.nwa, nwa,
                                        true, true, true,   // states
                                        true,               // symbols
                                        true, true, false); // transitions
        }
#endif
        
        
        // TODO: transition functions. In particular:
        //
        //     - For addYYYTrans, try from the situation where the states and symbol are
        //       and are not present. If they were not, check that they are after the
        //       addition.

        TEST(opennwa$Nwa$addInternalTrans, addingNewTransitionMakesItPresent)
        {
            OddNumEvenGroupsNwa fixture;
            size_t const starting_num = fixture.nwa.sizeInternalTrans();

            EXPECT_TRUE(fixture.nwa.addInternalTrans(fixture.q0, fixture.zero, fixture.q1));

            EXPECT_EQ(starting_num + 1, fixture.nwa.sizeInternalTrans());
        }

        TEST(opennwa$Nwa$addInternalTrans, addExistingTransitionDoesntChange)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy = fixture.nwa;

            EXPECT_FALSE(fixture.nwa.addInternalTrans(fixture.q2, fixture.zero, fixture.q3));

            expect_nwas_are_equal(copy, fixture.nwa);
        }

        TEST(opennwa$Nwa$addInternalTrans, addNewTransitionOnNonexistingElements)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy = fixture.nwa;
            SomeElements e;
            
            size_t const starting_num_internals = fixture.nwa.sizeInternalTrans();
            size_t const starting_num_states = fixture.nwa.sizeStates();
            size_t const starting_num_symbols = fixture.nwa.sizeSymbols();

            EXPECT_TRUE(fixture.nwa.addInternalTrans(e.state, e.symbol, e.state2));

            // Two different ways of checking that the states and symbols
            // were added. First by number, then we check explicitly.
            EXPECT_EQ(starting_num_internals + 1, fixture.nwa.sizeInternalTrans());
            EXPECT_EQ(starting_num_states + 2, fixture.nwa.sizeStates());
            EXPECT_EQ(starting_num_symbols + 1, fixture.nwa.sizeSymbols());

            EXPECT_TRUE(fixture.nwa.isState(e.state));
            EXPECT_TRUE(fixture.nwa.isState(e.state2));
            EXPECT_TRUE(fixture.nwa.isSymbol(e.symbol));

            // Now make sure nothing else changed
            expect_nwas_nearly_the_same(fixture.nwa, copy,
                                        false, true, true,   // states
                                        false,               // symbols
                                        false, true,true);   // transitions
        }


        TEST(opennwa$Nwa$addCallTrans, addingNewTransitionMakesItPresent)
        {
            OddNumEvenGroupsNwa fixture;
            size_t const starting_num = fixture.nwa.sizeCallTrans();

            EXPECT_TRUE(fixture.nwa.addCallTrans(fixture.q0, fixture.zero, fixture.q1));

            EXPECT_EQ(starting_num + 1, fixture.nwa.sizeCallTrans());
        }

        TEST(opennwa$Nwa$addCallTrans, addExistingTransitionDoesntChange)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy = fixture.nwa;

            EXPECT_FALSE(fixture.nwa.addCallTrans(fixture.q0, fixture.call, fixture.q2));

            expect_nwas_are_equal(copy, fixture.nwa);
        }

        TEST(opennwa$Nwa$addCallTrans, addNewTransitionOnNonexistingElements)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy = fixture.nwa;
            SomeElements e;
            
            size_t const starting_num_internals = fixture.nwa.sizeCallTrans();
            size_t const starting_num_states = fixture.nwa.sizeStates();
            size_t const starting_num_symbols = fixture.nwa.sizeSymbols();

            EXPECT_TRUE(fixture.nwa.addCallTrans(e.state, e.symbol, e.state2));

            // Two different ways of checking that the states and symbols
            // were added. First by number, then we check explicitly.
            EXPECT_EQ(starting_num_internals + 1, fixture.nwa.sizeCallTrans());
            EXPECT_EQ(starting_num_states + 2, fixture.nwa.sizeStates());
            EXPECT_EQ(starting_num_symbols + 1, fixture.nwa.sizeSymbols());

            EXPECT_TRUE(fixture.nwa.isState(e.state));
            EXPECT_TRUE(fixture.nwa.isState(e.state2));
            EXPECT_TRUE(fixture.nwa.isSymbol(e.symbol));

            // Now make sure nothing else changed
            expect_nwas_nearly_the_same(fixture.nwa, copy,
                                        false, true, true,   // states
                                        false,               // symbols
                                        true, false, true);  // transitions
        }



        TEST(opennwa$Nwa$addReturnTrans, addingNewTransitionMakesItPresent)
        {
            OddNumEvenGroupsNwa fixture;
            size_t const starting_num = fixture.nwa.sizeReturnTrans();

            EXPECT_TRUE(fixture.nwa.addReturnTrans(fixture.q0, fixture.q1, fixture.zero, fixture.q1));

            EXPECT_EQ(starting_num + 1, fixture.nwa.sizeReturnTrans());
        }

        TEST(opennwa$Nwa$addReturnTrans, addExistingTransitionDoesntChange)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy = fixture.nwa;

            EXPECT_FALSE(fixture.nwa.addReturnTrans(fixture.q3, fixture.q0, fixture.ret, fixture.q1));

            expect_nwas_are_equal(copy, fixture.nwa);
        }

        TEST(opennwa$Nwa$addReturnTrans, addNewTransitionOnNonexistingElements)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy = fixture.nwa;
            SomeElements e;
            
            size_t const starting_num_internals = fixture.nwa.sizeReturnTrans();
            size_t const starting_num_states = fixture.nwa.sizeStates();
            size_t const starting_num_symbols = fixture.nwa.sizeSymbols();

            EXPECT_TRUE(fixture.nwa.addReturnTrans(e.state, e.state3, e.symbol, e.state2));

            // Two different ways of checking that the states and symbols
            // were added. First by number, then we check explicitly.
            EXPECT_EQ(starting_num_internals + 1, fixture.nwa.sizeReturnTrans());
            EXPECT_EQ(starting_num_states + 3, fixture.nwa.sizeStates());
            EXPECT_EQ(starting_num_symbols + 1, fixture.nwa.sizeSymbols());

            EXPECT_TRUE(fixture.nwa.isState(e.state));
            EXPECT_TRUE(fixture.nwa.isState(e.state2));
            EXPECT_TRUE(fixture.nwa.isState(e.state3));
            EXPECT_TRUE(fixture.nwa.isSymbol(e.symbol));

            // Now make sure nothing else changed
            expect_nwas_nearly_the_same(fixture.nwa, copy,
                                        false, true, true,   // states
                                        false,               // symbols
                                        true, true, false);  // transitions
        }
        
        //     - For addYYYTrans, add more than one transition with the given source/sym,
        //       source/tgt, and sym/tgt.
        //



        /// Make sure wali::WALI_BAD_KEY causes an assertion
        TEST(opennwa$Nwa$$isState$and$addState$and$removeState$DeathTest, assertsOnWaliBadKey)
        {
            Nwa nwa;
            EXPECT_DEATH({
                    nwa.addState(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.isState(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeState(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
        }

        TEST(opennwa$Nwa$$isInitialState$and$addInitialState$and$removeInitialState$DeathTest, assertsOnWaliBadKey)
        {
            Nwa nwa;
            EXPECT_DEATH({
                    nwa.addInitialState(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.isInitialState(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeInitialState(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
        }

        TEST(opennwa$Nwa$$isFinalState$and$addFinalState$and$removeFinalState$DeathTest, assertsOnWaliBadKey)
        {
            Nwa nwa;
            EXPECT_DEATH({
                    nwa.addFinalState(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.isFinalState(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeFinalState(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
        }

        TEST(opennwa$Nwa$$isSymbol$and$addSymbol$and$removeSymbol$DeathTest, assertsOnWaliBadKey)
        {
            Nwa nwa;
            EXPECT_DEATH({
                    nwa.addSymbol(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.isSymbol(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeSymbol(wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
        }


        TEST(opennwa$Nwa$$addInternalTrans$and$removeInternalTrans$DeathTest, assertsOnWaliBadKey)
        {
            Nwa nwa;

            // sanity
            nwa.addInternalTrans(0, WILD, 0);
            
            EXPECT_DEATH({
                    nwa.addInternalTrans(wali::WALI_BAD_KEY, WILD, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.addInternalTrans(0, wali::WALI_BAD_KEY, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.addInternalTrans(0, WILD, wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");

            EXPECT_DEATH({
                    nwa.removeInternalTrans(wali::WALI_BAD_KEY, WILD, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeInternalTrans(0, wali::WALI_BAD_KEY, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeInternalTrans(0, WILD, wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
        }
        
        TEST(opennwa$Nwa$$addCallTrans$and$removeCallTrans$DeathTest, assertsOnWaliBadKey)
        {
            Nwa nwa;

            // sanity
            nwa.addCallTrans(0, WILD, 0);
            
            EXPECT_DEATH({
                    nwa.addCallTrans(wali::WALI_BAD_KEY, WILD, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.addCallTrans(0, wali::WALI_BAD_KEY, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.addCallTrans(0, WILD, wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");

            EXPECT_DEATH({
                    nwa.removeCallTrans(wali::WALI_BAD_KEY, WILD, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeCallTrans(0, wali::WALI_BAD_KEY, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeCallTrans(0, WILD, wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
        }

        TEST(opennwa$Nwa$$addReturnTrans$and$removeReturnTrans$DeathTest, assertsOnWaliBadKey)
        {
            Nwa nwa;

            // sanity
            nwa.addReturnTrans(0, 0, WILD, 0);
            
            EXPECT_DEATH({
                    nwa.addReturnTrans(wali::WALI_BAD_KEY, 0, WILD, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.addReturnTrans(0, wali::WALI_BAD_KEY, WILD, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.addReturnTrans(0, 0, wali::WALI_BAD_KEY, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.addReturnTrans(0, 0, WILD, wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");

            EXPECT_DEATH({
                    nwa.removeReturnTrans(wali::WALI_BAD_KEY, 0, WILD, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeReturnTrans(0, wali::WALI_BAD_KEY, WILD, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeReturnTrans(0, 0, wali::WALI_BAD_KEY, 0);
                },
                "< wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.removeReturnTrans(0, 0, WILD, wali::WALI_BAD_KEY);
                },
                "< wali::WALI_BAD_KEY");
        }

}
