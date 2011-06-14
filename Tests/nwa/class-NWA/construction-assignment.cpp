#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"

#include "Tests/nwa/fixtures.hpp"
#include "Tests/nwa/class-NWA/supporting.hpp"

namespace wali
{
    namespace nwa
    {
        /////////////////////////////////
        // Now begin the actual tests
        

        // NWA()
        //  - Test that a new NWA is empty
        TEST(wali$nwa$NWA$NWA$$default, newNwaIsEmpty)
        {
            NWA empty;
            expect_nwa_is_empty(empty);
        }

        // NWA(NWA const & other)
        //  - Test that a copy of an empty NWA is empty
        TEST(wali$nwa$NWA$NWA$$copy, copyOfEmptyNwaIsEmpty)
        {
            NWA empty;
            NWA also_empty = empty;

            expect_nwa_is_empty(also_empty);
        }
        
        //  - Test that a copy of a non-empty NWA has all the same states
        TEST(wali$nwa$NWA$NWA$$copy, copyOfNonemptyNwaIsSame)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy = fixture.nwa;

            expect_nwas_are_equal(fixture.nwa, copy);
        }
        
        //  - Test that the NWAs do not share structure (changing one doesn't
        //    change the other)
        TEST(wali$nwa$NWA$NWA$$copy, copyOfNonemptyNwaDoesNotShareState)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy = fixture.nwa;

            expect_nwas_are_physically_different(fixture.nwa, copy);
        }

        //  - Test that client information is equal but not shared
        TEST(wali$nwa$NWA$NWA$$copy, clientInformationIsCloned)
        {
            // TODO: tests (also add to assignment test)
        }
        
        
        // operator= (NWA const & other)
        //  - Same questions as NWA(NWA const &)
        TEST(wali$nwa$NWA$operatorEquals, copyOfEmptyNwaIsEmpty)
        {
            NWA empty, also_empty;
            also_empty = empty;

            expect_nwa_is_empty(also_empty);
        }

        TEST(wali$nwa$NWA$operatorEquals, copyOfNonemptyNwaIsSame)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy;
            copy = fixture.nwa;

            expect_nwas_are_equal(fixture.nwa, copy);
        }
        
        TEST(wali$nwa$NWA$operatorEquals, copyOfNonemptyNwaDoesNotShareState)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy;
            copy = fixture.nwa;

            expect_nwas_are_physically_different(fixture.nwa, copy);
        }

        //  - Test self assignment
        TEST(wali$nwa$NWA$operatorEquals, selfAssignmentDoesNotDestroyNwa)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy = fixture.nwa;

            copy = copy;

            expect_nwas_are_equal(fixture.nwa, copy);
        }

        TEST(wali$nwa$NWA$operatorEquals, selfAssignmentDoesNotCopy)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy = fixture.nwa;

            NWA::StateIterator
                states = copy.beginStates(),
                initials = copy.beginInitialStates(),
                finals = copy.beginFinalStates();

            NWA::SymbolIterator symbols = copy.beginSymbols();

            NWA::InternalIterator internals = copy.beginInternalTrans();
            NWA::CallIterator calls = copy.beginCallTrans();
            NWA::ReturnIterator returns = copy.beginReturnTrans();

            copy = copy;

            EXPECT_EQ(states, copy.beginStates());
            EXPECT_EQ(initials, copy.beginInitialStates());
            EXPECT_EQ(finals, copy.beginFinalStates());
            EXPECT_EQ(symbols, copy.beginSymbols());
            EXPECT_EQ(internals, copy.beginInternalTrans());
            EXPECT_EQ(calls, copy.beginCallTrans());
            EXPECT_EQ(returns, copy.beginReturnTrans());
        }

        
    }
}
