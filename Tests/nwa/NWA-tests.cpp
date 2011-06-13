#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"

namespace wali
{
    namespace nwa
    {

        // NWA()
        //  - Test that a new NWA is empty

        TEST(wali$nwa$NWA$NWA, newNwaIsEmpty)
        {
            NWA empty;
            EXPECT_EQ(0u, empty.sizeStates());
            EXPECT_EQ(0u, empty.sizeInitialStates());
            EXPECT_EQ(0u, empty.sizeFinalStates());
            EXPECT_EQ(0u, empty.sizeSymbols());
            EXPECT_EQ(0u, empty.sizeTrans());
            EXPECT_EQ(0u, empty.sizeInternalTrans());
            EXPECT_EQ(0u, empty.sizeCallTrans());
            EXPECT_EQ(0u, empty.sizeReturnTrans());
        }

        // NWA(NWA const & other)
        //  - Test that a copy of an empty NWA is empty
        //  - Test that a copy of a non-empty NWA has all the same states
        //  - Test that the NWAs do not share structure (pointers are different)
        //  - Test that client information is equal but not shared
        // 
        // operator= (NWA const & other)
        //  - Same questions as NWA(NWA const &)
        //  - Start with non-empty NWA
        //  - Test self assignment
        // 
        // clear ()
        //  - From an NWA that has states, initial states, accepting states, symbols,
        //    and transitions of all three kinds (each of which transitions with a
        //    real symbol, epsilon, and wild), calling clear() removes everything.
        // 
        // getClientInfo()/setClientInfo()
        //  - Check that getting and setting client info returns the same thing as
        //    before.
        //  - Also try adding states and removing unrelated states between the calls.
        // 
        // 
        // For XXX in each of States, InitialStates, FinalStates, InternalTrans,
        // CallTrans, ReturnTrans:
        // 
        //   getXXX()
        //     - I don't think we can really do anything with this separately. This
        //       is tested in concert with other functions except as follows:
        // 
        //     - For getSymbols(), check in an automaton that uses epsilon and wild on
        //       transitions. Make sure that neither is present in the given set.
        //     - For getSymbols(), 
        // 
        //   sizeXXX()
        //     - Make sure this agrees with the size of getXXX()
        // 
        //   isXXX()
        //     - Check if an XXX is a member of the empty NWA
        //     - Add unrelated items of each type; check.
        //     - Add item being checked to an empty NWA; check
        //     - Add both unrelated and checked item, then more unrelated items; check
        //     - For isSymbol(), make sure epsilon and wild return false both when they
        //       are used in transitions and not.
        // 
        //   addXXX()
        //     - Check that adding an XXX adds it to the set returned by getXXX and
        //       returns true
        //     - Check that adding it again leaves the return from getXXX unchanged,
        //       and returns false
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
