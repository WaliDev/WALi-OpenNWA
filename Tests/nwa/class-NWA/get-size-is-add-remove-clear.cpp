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

        
        //     - Check that the sets that should not be modified are not modified.
        // 
        //     - For addInitialState and addFinalState, try from both the situation where
        //       the state was and wasn't present in the NWA at all. If they weren't 
        //       present, check that they are after the addition.
        //     - For addSymbol(), make sure adding epsilon or wild returns false even if
        //       they are the first things done
        //     - For addYYYTrans, try from the situation where the states and symbol are
        //       and are not present. If they were not, check that they are after the
        //       addition.
        //     - For addYYYTrans, add more than one transition with the given source/sym,
        //       source/tgt, and sym/tgt.
        // 
        //   removeXXX()
        //     - Try from empty automaton, one containing given item, one not containing
        //       given item. Check correct value is returned.
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

    }
}
