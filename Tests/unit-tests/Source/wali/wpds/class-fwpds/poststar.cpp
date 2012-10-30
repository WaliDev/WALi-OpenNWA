#include <gtest/gtest.h>

#include <wali/wpds/fwpds/FWPDS.hpp>
#include <wali/wfa/WFA.hpp>
#include <wali/Reach.hpp>

namespace wali {
    namespace wpds {
        namespace fwpds {

            using namespace wali::wfa;

            struct TransGetter : TransFunctor
            {
                ITrans * the_trans;

                TransGetter()
                    : the_trans(NULL)
                {}
                
                virtual void operator() (ITrans * trans) {
                    // THERE SHALL BE ONLY ONE!
                    assert(the_trans == NULL);

                    the_trans = trans;
                }
            };


            TEST(wali$wpds$fwpds$$fwpds, poststarDestructorDoesNotTrashMemory)
            {
                sem_elem_t zero = Reach(true).zero();

                // Set up automaton:
                //         symbol
                //  start -------> (accept)
                Key start = getKey("start");
                Key accept = getKey("accept");
                Key symbol = getKey("symbol");

                WFA wfa;
                wfa.addState(start, zero);
                wfa.addState(accept, zero);

                wfa.setInitialState(start);
                wfa.addFinalState(accept);

                wfa.addTrans(start, symbol, accept, zero->one());

                // Set up WPDS. I use a pointer so I can control its deletion
                // time easier.
                FWPDS * wpds = new FWPDS();

                // Call poststar. In an old version of WALi, as a side effect
                // this would (1) copy wfa's one transition into result then
                // (2) put the copy of the transition into the wpds's
                // worklist and leave it there.
                WFA result = wpds->poststar(wfa);

                // Get a handle to the one transition in the WFA
                TransGetter getter;
                result.for_each(getter);
                ITrans * the_trans = getter.the_trans;
                ASSERT_TRUE(the_trans != NULL);

                // The old version of WALi would, in FWPDS's destructor
                // (indirectly), try to unmark the_trans. Thus we will mark
                // it, destroy the FWPDS, then make sure it is still marked.
                the_trans->mark();
                delete wpds;
                wpds = NULL;

                EXPECT_TRUE(the_trans->marked());
            }

        }
    }
}
