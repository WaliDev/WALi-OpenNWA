#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"
#include "Tests/unit-tests/Source/opennwa/class-NWA/supporting.hpp"
#include "Tests/unit-tests/Source/opennwa/int-client-info.hpp"

namespace opennwa
{
        /////////////////////////////////
        // Now begin the actual tests
        

        // Nwa()
        //  - Test that a new NWA is empty
        TEST(opennwa$Nwa$Nwa$$default, newNwaIsEmpty)
        {
            Nwa empty;
            expect_nwa_is_empty(empty);
        }

        // Nwa(Nwa const & other)
        //  - Test that a copy of an empty NWA is empty
        TEST(opennwa$Nwa$Nwa$$copy, copyOfEmptyNwaIsEmpty)
        {
            Nwa empty;
            Nwa also_empty = empty;

            expect_nwa_is_empty(also_empty);
        }
        
        //  - Test that a copy of a non-empty NWA has all the same states
        TEST(opennwa$Nwa$Nwa$$copy, copyOfNonemptyNwaIsSame)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy = fixture.nwa;

            expect_nwas_are_equal(fixture.nwa, copy);
        }
        
        //  - Test that the NWAs do not share structure (changing one doesn't
        //    change the other)
        TEST(opennwa$Nwa$Nwa$$copy, copyOfNonemptyNwaDoesNotShareState)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy = fixture.nwa;

            expect_nwas_are_physically_different(fixture.nwa, copy);
        }

        //  - Test that client information is equal but not shared
        TEST(opennwa$Nwa$Nwa$$copy, clientInformationIsCloned)
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

            Nwa copy(fixture.nwa);
            
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
        TEST(opennwa$Nwa$Nwa$$copy, aliasedClientInfosSeparate)
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
            Nwa copy = fixture.nwa;

            ASSERT_NE(ci, copy.getClientInfo(fixture.q0));
            ASSERT_NE(ci, copy.getClientInfo(fixture.q1));

            EXPECT_NE(copy.getClientInfo(fixture.q0), copy.getClientInfo(fixture.q1));
        }

        TEST(opennwa$Nwa$Nwa$$operatorEquals, aliasedClientInfosSeparate)
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
            Nwa copy;
            copy = fixture.nwa;

            ASSERT_NE(ci, copy.getClientInfo(fixture.q0));
            ASSERT_NE(ci, copy.getClientInfo(fixture.q1));

            EXPECT_NE(copy.getClientInfo(fixture.q0), copy.getClientInfo(fixture.q1));
        }

        
        // operator= (Nwa const & other)
        //  - Same questions as Nwa(Nwa const &)
        TEST(opennwa$Nwa$$operatorEquals, copyOfEmptyNwaIsEmpty)
        {
            Nwa empty, also_empty;
            also_empty = empty;

            expect_nwa_is_empty(also_empty);
        }

        TEST(opennwa$Nwa$$operatorEquals, copyOfNonemptyNwaIsSame)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy;
            copy = fixture.nwa;

            expect_nwas_are_equal(fixture.nwa, copy);
        }
        
        TEST(opennwa$Nwa$$operatorEquals, copyOfNonemptyNwaDoesNotShareState)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy;
            copy = fixture.nwa;

            expect_nwas_are_physically_different(fixture.nwa, copy);
        }

        TEST(opennwa$Nwa$$operatorEquals, doesOperatorEqualsCompareClientInfo)
        {
            ClientInfoRefPtr ci = new IntClientInfo(1);
            
            OddNumEvenGroupsNwa fixture;
            fixture.nwa.setClientInfo(fixture.q0, ci);
            
            Nwa copy = fixture.nwa;

            ASSERT_NE(ci, copy.getClientInfo(fixture.q0));

            EXPECT_EQ(fixture.nwa, copy);
        }

        //  - Test self assignment
        TEST(opennwa$Nwa$$operatorEquals, selfAssignmentDoesNotDestroyNwa)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy = fixture.nwa;

            copy = copy;

            expect_nwas_are_equal(fixture.nwa, copy);
        }

        TEST(opennwa$Nwa$$operatorEquals, selfAssignmentDoesNotCopy)
        {
            OddNumEvenGroupsNwa fixture;
            Nwa copy = fixture.nwa;

            Nwa::StateIterator
                states = copy.beginStates(),
                initials = copy.beginInitialStates(),
                finals = copy.beginFinalStates();

            Nwa::SymbolIterator symbols = copy.beginSymbols();

            Nwa::InternalIterator internals = copy.beginInternalTrans();
            Nwa::CallIterator calls = copy.beginCallTrans();
            Nwa::ReturnIterator returns = copy.beginReturnTrans();

            copy = copy;

            EXPECT_EQ(states, copy.beginStates());
            EXPECT_EQ(initials, copy.beginInitialStates());
            EXPECT_EQ(finals, copy.beginFinalStates());
            EXPECT_EQ(symbols, copy.beginSymbols());
            EXPECT_EQ(internals, copy.beginInternalTrans());
            EXPECT_EQ(calls, copy.beginCallTrans());
            EXPECT_EQ(returns, copy.beginReturnTrans());
        }

        TEST(opennwa$Nwa$$operatorEquals, selfAssignmentDoesNotDestroyClientInfo)
        {
            OddNumEvenGroupsNwa fixture;

            fixture.nwa.setClientInfo(fixture.q0, new IntClientInfo(0));
            fixture.nwa.setClientInfo(fixture.q1, new IntClientInfo(1));
            fixture.nwa.setClientInfo(fixture.q2, new IntClientInfo(2));
            fixture.nwa.setClientInfo(fixture.q3, new IntClientInfo(3));
            
            Nwa copy = fixture.nwa;

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
        TEST(opennwa$Nwa$Nwa$$operatorEquals, clientInformationIsCloned)
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

            Nwa copy;
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
        TEST(opennwa$Nwa$$clear, clearingEmptyGivesEmpty)
        {
            Nwa empty;
            empty.clear();

            expect_nwa_is_empty(empty);
        }

        TEST(opennwa$Nwa$$clear, clearingNonEmptyGivesEmpty)
        {
            OddNumEvenGroupsNwa fixture;
            fixture.nwa.clear();

            expect_nwa_is_empty(fixture.nwa);
        }


        ////////////////////////////////
        // getClientInfo(), setClientInfo()
        //  - some prerequsites for the tests above to be meaningful        
        TEST(opennwa$Nwa$$getClientInfo$and$setClientInfo, clientInfoStartsOffNull)
        {
            OddNumEvenGroupsNwa fixture;

            EXPECT_TRUE(fixture.nwa.getClientInfo(fixture.q0) == NULL);
            EXPECT_TRUE(fixture.nwa.getClientInfo(fixture.dummy) == NULL);
        }
        
        
        TEST(opennwa$Nwa$$getClientInfo$and$setClientInfo, getAfterSetWorks)
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

        TEST(opennwa$Nwa$$getClientInfo$and$setClientInfo, canSetClientInfoToNull)
        {
            OddNumEvenGroupsNwa fixture;

            ref_ptr<ClientInfo> q0_ci = new IntClientInfo(0);
            fixture.nwa.setClientInfo(fixture.q0, q0_ci);
            ASSERT_EQ(q0_ci, fixture.nwa.getClientInfo(fixture.q0));
            
            // Now set it back to null, and make sure it is
            fixture.nwa.setClientInfo(fixture.q0, NULL);
            
            EXPECT_TRUE(fixture.nwa.getClientInfo(fixture.q0) == NULL);
        }


        ///////////////////////////
        TEST(opennwa$Nwa$$getClientInfo$and$setClientInfo$DeathTest, waliBadKeyAsserts)
        {
            Nwa nwa;
            EXPECT_DEATH({
                    nwa.getClientInfo(wali::WALI_BAD_KEY);
                },
                "state < wali::WALI_BAD_KEY");
            EXPECT_DEATH({
                    nwa.setClientInfo(wali::WALI_BAD_KEY, NULL);
                },
                "state < wali::WALI_BAD_KEY");
        }

}
