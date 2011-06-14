#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"

#include "fixtures.hpp"
#include "class-NWA/supporting.hpp"

namespace wali
{
    namespace nwa
    {
        /////////////////////////////////
        // Now begin the actual tests
        

        // clear ()
        //  - From an NWA that has states, initial states, accepting states, symbols,
        //    and transitions of all three kinds (each of which transitions with a
        //    real symbol, epsilon, and wild), calling clear() removes everything.
        TEST(wali$nwa$NWA$clear, clearingEmptyGivesEmpty)
        {
            NWA empty;
            empty.clear();

            expect_nwa_is_empty(empty);
        }

        TEST(wali$nwa$NWA$clear, clearingNonEmptyGivesEmpty)
        {
            OddNumEvenGroupsNwa fixture;
            fixture.nwa.clear();

            expect_nwa_is_empty(fixture.nwa);
        }


        // getClientInfo()/setClientInfo()
        //  - Check that getting and setting client info returns the same
        //    thing as before.
        //  - Also try adding states and removing unrelated states between
        //  - the calls.

        // TODO: write tests
        
        
    }
}
