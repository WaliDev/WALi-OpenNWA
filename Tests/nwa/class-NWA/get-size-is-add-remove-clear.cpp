#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"

#include "Tests/nwa/fixtures.hpp"
#include "Tests/nwa/class-NWA/supporting.hpp"

namespace wali
{
    namespace nwa
    {
        //////////////////////////////////
        // Supporting stuff

        
        /// All of these functions add 'state' to 'nwa', and checks that it
        /// is present in the appropriate sets and only those sets. (Symbol
        /// does the same with 'symbol'.) 'expect_addition' controls whether
        /// the state should be added.

        void
        add_to_states_and_check(NWA * nwa, State state, bool expect_addition)
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
        add_to_initial_states_and_check(NWA * nwa, State state, bool expect_addition)
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
        add_to_final_states_and_check(NWA * nwa, State state, bool expect_addition)
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
        add_to_symbols_and_check(NWA * nwa, Symbol symbol, bool expect_addition)
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
        void expect_size_consistent_with_range(NWA const & nwa)
        {
#define CHECK_CONSISTENCY(Attribute)                                             \
            EXPECT_EQ(nwa.size##Attribute(),                                     \
                      static_cast<size_t>(std::distance(nwa.begin##Attribute(),  \
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
        TEST(wali$nwa$NWA$getSymbols, epsilonAndWildAreNotPresent)
        {
            NWA nwa;
            Key q = getKey('q');
            nwa.addInternalTrans(q, WALI_EPSILON, q);
            nwa.addInternalTrans(q, WALI_WILD, q);

            // Sanity check
            EXPECT_EQ(2u, nwa.sizeInternalTrans());

            // Actual check (see checkSizeOfEmptyIsConsistent below)
            EXPECT_EQ(0u, nwa.sizeSymbols());
        }


        //////////////////
        //   sizeXXX()
        //     - Make sure this agrees with the size of getXXX()       
        TEST(wali$nwa$NWA$getXXX, checkSizeConsistentForEmptyNwa)
        {
            NWA empty;
            expect_size_consistent_with_range(empty);
        }

        TEST(wali$nwa$NWA$getXXX, checkSizeConsistentForBigNwa)
        {
            OddNumEvenGroupsNwa fixture;
            expect_size_consistent_with_range(fixture.nwa);
        }


        //////////////////
        //   isXXX() and addXXX() for states and symbols
        //
        //     - Check if an XXX is a member of the empty NWA       
        TEST(wali$nwa$NWA$isXXX, checkNothingMemberOfEmpty)
        {
            NWA empty;
            SomeElements::expect_not_present(empty);
        }

        //     - Add unrelated items of each type; check.
        TEST(wali$nwa$NWA$isXXX, checkNothingMemberOfStuff)
        {
            OddNumEvenGroupsNwa fixture;
            SomeElements::expect_not_present(fixture.nwa);
        }

        
        //     - Add item being checked to an empty NWA; check
        //     - Check that adding an XXX adds it to the set returned by
        //       getXXX and returns true       
        TEST(wali$nwa$NWA$isState$$and$addState, addToEmptyAndCheck)
        {
            NWA empty;
            SomeElements e;
            add_to_states_and_check(&empty, e.state, true);
        }
        
        TEST(wali$nwa$NWA$isInitialState$$and$addInitialState, addToEmptyAndCheck)
        {
            NWA empty;
            SomeElements e;
            add_to_initial_states_and_check(&empty, e.state, true);
        }

        TEST(wali$nwa$NWA$isFinalState$$and$addFinalState, addToEmptyAndCheck)
        {
            NWA empty;
            SomeElements e;
            add_to_final_states_and_check(&empty, e.state, true);
        }

        TEST(wali$nwa$NWA$isSymbol$$and$addSymbol, addToEmptyAndCheck)
        {
            NWA empty;
            SomeElements e;
            add_to_symbols_and_check(&empty, e.symbol, true);
        }

        
        //     - Add both unrelated and checked item, then more unrelated
        //       items; check. (I'm not doing the "then more
        //       unrelated". TODO?)
        TEST(wali$nwa$NWA$isState$$and$addState, addToBusyAndCheck)
        {
            OddNumEvenGroupsNwa fixture;
            SomeElements e;
            add_to_states_and_check(&fixture.nwa, e.state, true);
        }
        
        TEST(wali$nwa$NWA$isInitialState$$and$addInitialState, addToBusyAndCheck)
        {
            OddNumEvenGroupsNwa fixture;
            SomeElements e;
            add_to_initial_states_and_check(&fixture.nwa, e.state, true);
        }

        TEST(wali$nwa$NWA$isFinalState$$and$addFinalState, addToBusyAndCheck)
        {
            OddNumEvenGroupsNwa fixture;
            SomeElements e;
            add_to_final_states_and_check(&fixture.nwa, e.state, true);
        }

        TEST(wali$nwa$NWA$isSymbol$$and$addSymbol, addToBusyAndCheck)
        {
            OddNumEvenGroupsNwa fixture;
            SomeElements e;
            add_to_symbols_and_check(&fixture.nwa, e.symbol, true);
        }

        
        //     - For isSymbol(), make sure epsilon and wild return false both when they
        //       are used in transitions and not.
        // This is done inline with the above
        

        //   addXXX()
        //     - Check that adding it again leaves the return from getXXX
        //       unchanged, and returns false
        TEST(wali$nwa$NWA$addState, addTwiceToEmptyAndCheck)
        {
            NWA nwa;
            SomeElements e;
            add_to_states_and_check(&nwa, e.state, true);

            NWA copy = nwa;
            add_to_states_and_check(&nwa, e.state, false);
            
            expect_nwas_are_equal(nwa, copy);
        }
        
        TEST(wali$nwa$NWA$addInitialState, addTwiceToEmptyAndCheck)
        {
            NWA nwa;
            SomeElements e;
            add_to_initial_states_and_check(&nwa, e.state, true);

            NWA copy = nwa;
            add_to_initial_states_and_check(&nwa, e.state, false);

            expect_nwas_are_equal(nwa, copy);
        }

        TEST(wali$nwa$NWA$addFinalState, addTwiceToNwaAndCheck)
        {
            NWA nwa;
            SomeElements e;
            add_to_final_states_and_check(&nwa, e.state, true);

            NWA copy = nwa;
            add_to_final_states_and_check(&nwa, e.state, false);

            expect_nwas_are_equal(nwa, copy);
        }

        TEST(wali$nwa$NWA$addSymbol, addTwiceToEmptyAndCheck)
        {
            NWA nwa;
            SomeElements e;
            add_to_symbols_and_check(&nwa, e.symbol, true);

            NWA copy = nwa;
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
        TEST(wali$nwa$NWA$addInitialState, addInitialStateOfAStateAlreadyPresent)
        {
            NWA nwa;
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
        TEST(wali$nwa$NWA$addSymbol, addingEpsilonOrWildShouldFail)
        {
            NWA nwa;

            EXPECT_FALSE(nwa.addSymbol(WALI_EPSILON));
            EXPECT_FALSE(nwa.addSymbol(WALI_WILD));
        }

        
        //   removeXXX()
        //     - Try from empty automaton, one containing given item, one not
        //       containing given item. Check correct value is returned.
        TEST(wali$nwa$NWA$removeXXX, removingFromEmptyShouldFail)
        {
            NWA nwa;
            SomeElements e;

            EXPECT_FALSE(nwa.removeState(e.state));
            EXPECT_FALSE(nwa.removeInitialState(e.state));
            EXPECT_FALSE(nwa.removeFinalState(e.state));
            EXPECT_FALSE(nwa.removeSymbol(e.symbol));
            EXPECT_FALSE(nwa.removeInternalTrans(e.internal));
            EXPECT_FALSE(nwa.removeCallTrans(e.call));
            EXPECT_FALSE(nwa.removeReturnTrans(e.ret));
        }

        TEST(wali$nwa$NWA$removeXXX, removingSingleItemsShouldLeaveEmpty)
        {
            NWA nwa;
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

        TEST(wali$nwa$NWA$removeXXX, removingItemNotInNwaShouldLeaveItUnchanged)
        {
            OddNumEvenGroupsNwa fixture;
            NWA nwa = fixture.nwa;
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

        TEST(wali$nwa$NWA$removeXXX, removingItemWhenThereIsMoreThanOneOfThatKindShouldLeaveTheOthers)
        {
            // TODO: this test doesn't really test that the right transitions
            // were removed.
            
            typedef OddNumEvenGroupsNwa TheNwa;
            OddNumEvenGroupsNwa fixture;
            NWA nwa = fixture.nwa;

            // We'll add a couple more for good fun
            ASSERT_TRUE(nwa.addInitialState(TheNwa::q2));
            ASSERT_TRUE(nwa.addFinalState(TheNwa::q0));

            // TheNwa transitions first
            EXPECT_EQ(4u, nwa.sizeInternalTrans());
            EXPECT_TRUE(nwa.removeInternalTrans(TheNwa::q2, TheNwa::zero, TheNwa::q3));
            EXPECT_EQ(3u, nwa.sizeInternalTrans());
            EXPECT_TRUE(nwa.removeInternalTrans(TheNwa::q2, WALI_EPSILON, TheNwa::dummy));
            EXPECT_EQ(2u, nwa.sizeInternalTrans());

            EXPECT_EQ(3u, nwa.sizeCallTrans());
            EXPECT_TRUE(nwa.removeCallTrans(TheNwa::q0, TheNwa::call, TheNwa::q2));
            EXPECT_EQ(2u, nwa.sizeCallTrans());

            EXPECT_EQ(3u, nwa.sizeReturnTrans());
            EXPECT_TRUE(nwa.removeReturnTrans(TheNwa::q3, TheNwa::q1, TheNwa::ret, TheNwa::q0));
            EXPECT_EQ(2u, nwa.sizeReturnTrans());

            // Symbols
            EXPECT_EQ(3u, nwa.sizeSymbols());
            EXPECT_TRUE(nwa.removeSymbol(TheNwa::zero)); // **
            EXPECT_EQ(2u, nwa.sizeSymbols());
            EXPECT_TRUE(nwa.removeSymbol(TheNwa::call)); // **
            EXPECT_EQ(1u, nwa.sizeSymbols());

            EXPECT_FALSE(nwa.isSymbol(TheNwa::zero));
            EXPECT_FALSE(nwa.isSymbol(TheNwa::call));
            EXPECT_TRUE(nwa.isSymbol(TheNwa::ret));

            // Initial states
            EXPECT_EQ(2u, nwa.sizeInitialStates());
            EXPECT_EQ(2u, nwa.sizeFinalStates());
            
            EXPECT_TRUE(nwa.removeInitialState(TheNwa::q0)); // **
            
            EXPECT_EQ(1u, nwa.sizeInitialStates());
            EXPECT_EQ(2u, nwa.sizeFinalStates());

            // Final states          
            EXPECT_TRUE(nwa.removeFinalState(TheNwa::q1)); // **
            EXPECT_EQ(1u, nwa.sizeInitialStates());
            EXPECT_EQ(1u, nwa.sizeFinalStates());

            EXPECT_TRUE(nwa.isInitialState(TheNwa::q2));
            EXPECT_TRUE(nwa.isFinalState(TheNwa::q0));

            // States
            EXPECT_EQ(5u, nwa.sizeStates());
            EXPECT_TRUE(nwa.removeState(TheNwa::dummy)); // **
            EXPECT_EQ(4u, nwa.sizeStates());
            EXPECT_FALSE(nwa.isState(TheNwa::dummy));
        }
        
        //     - In each case, check that sets that shouldn't change are not modified.
        // 
        //     - For removeState(), try from automaton in which the state is and is not
        //       involved in any transitions. Make sure you get the state in all
        //       applicable coordinates: source/target of internal, call/entry of call,
        //       and exit/pred/return of return.
        //     - For removeState(), try from automaton in which the symbol is and is not
        //       involved in any transtions. Test all three kinds of transitions.
        // 
        //   clearXXX()
        //     - Make sure that the set of XXX is empty after.
        //     - Make sure that sets that shouldn't be different are not different.
        // 
        //     - For clearStates(), make sure initial/accepting states and transitions
        //       have all been cleared too.
        //     - For clearSymbols(), make sure transitions are all clear too.

        
        // TODO: transition functions. In particular:
        //
        //     - For addYYYTrans, try from the situation where the states and symbol are
        //       and are not present. If they were not, check that they are after the
        //       addition.
        //     - For addYYYTrans, add more than one transition with the given source/sym,
        //       source/tgt, and sym/tgt.
        // 
    }
}
