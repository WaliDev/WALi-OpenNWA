#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"

#include "Tests/nwa/Source/fixtures.hpp"
#include "Tests/nwa/Source/class-NWA/supporting.hpp"
#include "Tests/nwa/Source/int-client-info.hpp"

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
            OddNumEvenGroupsNwa fixture;

            ref_ptr<IntClientInfo> q0_ci = new IntClientInfo(0);
            ref_ptr<IntClientInfo> q1_ci = new IntClientInfo(1);
            ref_ptr<IntClientInfo> q2_ci = new IntClientInfo(2);
            ref_ptr<IntClientInfo> q3_ci = new IntClientInfo(3);
                        
            fixture.nwa.setClientInfo(fixture.q0, q0_ci);
            fixture.nwa.setClientInfo(fixture.q1, q1_ci);
            fixture.nwa.setClientInfo(fixture.q2, q2_ci);
            fixture.nwa.setClientInfo(fixture.q3, q3_ci);

            NWA copy(fixture.nwa);
            
#define EXPECT_CLIENT_INFO_IS(expect, nwa, state)                       \
            do {                                                        \
                ClientInfo * ci = nwa.getClientInfo(state).get_ptr();   \
                IntClientInfo * ici = dynamic_cast<IntClientInfo *>(ci);\
                ASSERT_TRUE(ici != NULL);                               \
                EXPECT_EQ(expect, ici->n);                              \
            } while (0)

            // Check that it makes it over
            EXPECT_CLIENT_INFO_IS(0, copy, fixture.q0);
            EXPECT_CLIENT_INFO_IS(1, copy, fixture.q1);
            EXPECT_CLIENT_INFO_IS(2, copy, fixture.q2);
            EXPECT_CLIENT_INFO_IS(3, copy, fixture.q3);
            EXPECT_TRUE(copy.getClientInfo(fixture.dummy) == NULL);


            // Check that it isn't changed
            q0_ci->n = 10;
            q1_ci->n = 11;
            q2_ci->n = 12;
            q3_ci->n = 13;

            EXPECT_CLIENT_INFO_IS(0, copy, fixture.q0);
            EXPECT_CLIENT_INFO_IS(1, copy, fixture.q1);
            EXPECT_CLIENT_INFO_IS(2, copy, fixture.q2);
            EXPECT_CLIENT_INFO_IS(3, copy, fixture.q3);
            EXPECT_TRUE(copy.getClientInfo(fixture.dummy) == NULL);
#undef EXPECT_CLIENT_INFO_IS
        }


        // Test that aliased ClientInfos "separate" when copied
        TEST(wali$nwa$NWA$NWA$$copy, aliasedClientInfosSeparate)
        {
            OddNumEvenGroupsNwa fixture;
            ref_ptr<ClientInfo> ci = new IntClientInfo(0);

            fixture.nwa.setClientInfo(fixture.q0, ci);
            fixture.nwa.setClientInfo(fixture.q1, ci);
            ASSERT_EQ(fixture.nwa.getClientInfo(fixture.q0),
                      fixture.nwa.getClientInfo(fixture.q1));

            // Now copy it and demonstrate that the copies of q0 and q1 have
            // different client info (after sanity checks that they differ
            // from the original)
            NWA copy = fixture.nwa;

            ASSERT_NE(ci, copy.getClientInfo(fixture.q0));
            ASSERT_NE(ci, copy.getClientInfo(fixture.q1));

            EXPECT_NE(copy.getClientInfo(fixture.q0), copy.getClientInfo(fixture.q1));
        }

        TEST(wali$nwa$NWA$NWA$$operatorEquals, aliasedClientInfosSeparate)
        {
            OddNumEvenGroupsNwa fixture;
            ref_ptr<ClientInfo> ci = new IntClientInfo(0);

            fixture.nwa.setClientInfo(fixture.q0, ci);
            fixture.nwa.setClientInfo(fixture.q1, ci);
            ASSERT_EQ(fixture.nwa.getClientInfo(fixture.q0),
                      fixture.nwa.getClientInfo(fixture.q1));

            // Now copy it and demonstrate that the copies of q0 and q1 have
            // different client info (after sanity checks that they differ
            // from the original)
            NWA copy;
            copy = fixture.nwa;

            ASSERT_NE(ci, copy.getClientInfo(fixture.q0));
            ASSERT_NE(ci, copy.getClientInfo(fixture.q1));

            EXPECT_NE(copy.getClientInfo(fixture.q0), copy.getClientInfo(fixture.q1));
        }

        
        // operator= (NWA const & other)
        //  - Same questions as NWA(NWA const &)
        TEST(wali$nwa$NWA$$operatorEquals, copyOfEmptyNwaIsEmpty)
        {
            NWA empty, also_empty;
            also_empty = empty;

            expect_nwa_is_empty(also_empty);
        }

        TEST(wali$nwa$NWA$$operatorEquals, copyOfNonemptyNwaIsSame)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy;
            copy = fixture.nwa;

            expect_nwas_are_equal(fixture.nwa, copy);
        }
        
        TEST(wali$nwa$NWA$$operatorEquals, copyOfNonemptyNwaDoesNotShareState)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy;
            copy = fixture.nwa;

            expect_nwas_are_physically_different(fixture.nwa, copy);
        }

        TEST(wali$nwa$NWA$$operatorEquals, doesOperatorEqualsCompareClientInfo)
        {
            ClientInfoRefPtr ci = new IntClientInfo(1);
            
            OddNumEvenGroupsNwa fixture;
            fixture.nwa.setClientInfo(fixture.q0, ci);
            
            NWA copy = fixture.nwa;

            ASSERT_NE(ci, copy.getClientInfo(fixture.q0));

            EXPECT_EQ(fixture.nwa, copy);
        }

        //  - Test self assignment
        TEST(wali$nwa$NWA$$operatorEquals, selfAssignmentDoesNotDestroyNwa)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy = fixture.nwa;

            copy = copy;

            expect_nwas_are_equal(fixture.nwa, copy);
        }

        TEST(wali$nwa$NWA$$operatorEquals, selfAssignmentDoesNotCopy)
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

        TEST(wali$nwa$NWA$$operatorEquals, selfAssignmentDoesNotDestroyClientInfo)
        {
            OddNumEvenGroupsNwa fixture;

            fixture.nwa.setClientInfo(fixture.q0, new IntClientInfo(0));
            fixture.nwa.setClientInfo(fixture.q1, new IntClientInfo(1));
            fixture.nwa.setClientInfo(fixture.q2, new IntClientInfo(2));
            fixture.nwa.setClientInfo(fixture.q3, new IntClientInfo(3));
            
            NWA copy = fixture.nwa;

            copy = copy;
            
#define EXPECT_CLIENT_INFO_IS(expect, nwa, state)                       \
            do {                                                        \
                ClientInfo * ci = nwa.getClientInfo(state).get_ptr();   \
                IntClientInfo * ici = dynamic_cast<IntClientInfo *>(ci);\
                ASSERT_TRUE(ici != NULL);                               \
                EXPECT_EQ(expect, ici->n);                              \
            } while (0)

            // Check that it makes it over
            EXPECT_CLIENT_INFO_IS(0, copy, fixture.q0);
            EXPECT_CLIENT_INFO_IS(1, copy, fixture.q1);
            EXPECT_CLIENT_INFO_IS(2, copy, fixture.q2);
            EXPECT_CLIENT_INFO_IS(3, copy, fixture.q3);
            EXPECT_TRUE(copy.getClientInfo(fixture.dummy) == NULL);
#undef EXPECT_CLIENT_INFO_IS
        }

        //  - Test that client information is copied but not shared
        TEST(wali$nwa$NWA$NWA$$operatorEquals, clientInformationIsCloned)
        {
            OddNumEvenGroupsNwa fixture;

            ref_ptr<IntClientInfo> q0_ci = new IntClientInfo(0);
            ref_ptr<IntClientInfo> q1_ci = new IntClientInfo(1);
            ref_ptr<IntClientInfo> q2_ci = new IntClientInfo(2);
            ref_ptr<IntClientInfo> q3_ci = new IntClientInfo(3);
                        
            fixture.nwa.setClientInfo(fixture.q0, q0_ci);
            fixture.nwa.setClientInfo(fixture.q1, q1_ci);
            fixture.nwa.setClientInfo(fixture.q2, q2_ci);
            fixture.nwa.setClientInfo(fixture.q3, q3_ci);

            NWA copy;
            copy = fixture.nwa;
            
#define EXPECT_CLIENT_INFO_IS(expect, nwa, state)                       \
            do {                                                        \
                ClientInfo * ci = nwa.getClientInfo(state).get_ptr();   \
                IntClientInfo * ici = dynamic_cast<IntClientInfo *>(ci);\
                ASSERT_TRUE(ici != NULL);                               \
                EXPECT_EQ(expect, ici->n);                              \
            } while (0)

            // Check that it makes it over
            EXPECT_CLIENT_INFO_IS(0, copy, fixture.q0);
            EXPECT_CLIENT_INFO_IS(1, copy, fixture.q1);
            EXPECT_CLIENT_INFO_IS(2, copy, fixture.q2);
            EXPECT_CLIENT_INFO_IS(3, copy, fixture.q3);
            EXPECT_TRUE(copy.getClientInfo(fixture.dummy) == NULL);


            // Check that it isn't changed
            q0_ci->n = 10;
            q1_ci->n = 11;
            q2_ci->n = 12;
            q3_ci->n = 13;

            EXPECT_CLIENT_INFO_IS(0, copy, fixture.q0);
            EXPECT_CLIENT_INFO_IS(1, copy, fixture.q1);
            EXPECT_CLIENT_INFO_IS(2, copy, fixture.q2);
            EXPECT_CLIENT_INFO_IS(3, copy, fixture.q3);
            EXPECT_TRUE(copy.getClientInfo(fixture.dummy) == NULL);
#undef EXPECT_CLIENT_INFO_IS
        }


        ///////////////////////////
        // clear ()
        //  - From an NWA that has states, initial states, accepting states, symbols,
        //    and transitions of all three kinds (each of which transitions with a
        //    real symbol, epsilon, and wild), calling clear() removes everything.
        TEST(wali$nwa$NWA$$clear, clearingEmptyGivesEmpty)
        {
            NWA empty;
            empty.clear();

            expect_nwa_is_empty(empty);
        }

        TEST(wali$nwa$NWA$$clear, clearingNonEmptyGivesEmpty)
        {
            OddNumEvenGroupsNwa fixture;
            fixture.nwa.clear();

            expect_nwa_is_empty(fixture.nwa);
        }


        ////////////////////////////////
        // getClientInfo(), setClientInfo()
        //  - some prerequsites for the tests above to be meaningful
        TEST(wali$nwa$NWA$NWA$$getClientInfo$and$setClientInfo, clientInfoStartsOffNull)
        {
            OddNumEvenGroupsNwa fixture;

            EXPECT_TRUE(fixture.nwa.getClientInfo(fixture.q0) == NULL);
            EXPECT_TRUE(fixture.nwa.getClientInfo(fixture.dummy) == NULL);
        }
        
        
        TEST(wali$nwa$NWA$NWA$$getClientInfo$and$setClientInfo, getAfterSetWorks)
        {
            OddNumEvenGroupsNwa fixture;

            ref_ptr<ClientInfo> q0_ci = new IntClientInfo(0);
            ref_ptr<ClientInfo> q1_ci = new IntClientInfo(1);
            ref_ptr<ClientInfo> q2_ci = new IntClientInfo(2);
            ref_ptr<ClientInfo> q3_ci = new IntClientInfo(3);

            // Set some info, then retrieve it to make sure it's the same
            fixture.nwa.setClientInfo(fixture.q0, q0_ci);
            fixture.nwa.setClientInfo(fixture.q1, q1_ci);

            EXPECT_EQ(q0_ci, fixture.nwa.getClientInfo(fixture.q0));
            EXPECT_EQ(q1_ci, fixture.nwa.getClientInfo(fixture.q1));
        }

        TEST(wali$nwa$NWA$NWA$$getClientInfo$and$setClientInfo, canSetClientInfoToNull)
        {
            OddNumEvenGroupsNwa fixture;

            ref_ptr<ClientInfo> q0_ci = new IntClientInfo(0);
            fixture.nwa.setClientInfo(fixture.q0, q0_ci);
            ASSERT_EQ(q0_ci, fixture.nwa.getClientInfo(fixture.q0));
            
            // Now set it back to null, and make sure it is
            fixture.nwa.setClientInfo(fixture.q0, NULL);
            
            EXPECT_TRUE(fixture.nwa.getClientInfo(fixture.q0) == NULL);
        }

    }
}
