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


            bool test_memory(bool poststar, bool add_a_rule)
            {
                sem_elem_t zero = Reach(true).zero();

                // Set up automaton:
                //         symbol
                //  start -------> (accept)
                Key start = getKey("start");
                Key accept = getKey("accept");
                Key symbol = getKey("symbol");

                WFA query;
                query.addState(start, zero);
                query.addState(accept, zero);

                query.setInitialState(start);
                query.addFinalState(accept);

                query.addTrans(start, symbol, accept, zero->one());

                // Set up WPDS. I use a pointer so I can control its deletion
                // time easier.
                FWPDS * wpds = new FWPDS();

                if (add_a_rule) {
                    Key p = getKey("p");
                    Key sym2 = getKey("sym2");
                    wpds->add_rule(p, symbol, p, sym2, zero->one());
                }

                // Call poststar. In an old version of WALi, as a side effect
                // this would (1) copy query's one transition into result then
                // (2) put the copy of the transition into the wpds's
                // worklist and leave it there.
                WFA result;
                if (poststar) {
                    result = wpds->poststar(query);
                }
                else {
                    result = wpds->prestar(query);
                }

                // Get a handle to the one transition in the WFA
                TransGetter getter;
                result.for_each(getter);
                ITrans * the_trans = getter.the_trans;
                if (the_trans == NULL) {
                    // A little redundant in some sense, but we'll want to
                    // tell gtest about the failure.
                    std::cerr << "[Test failing because the_trans == NULL]\n";
                    return false;
                }

                // The old version of WALi would, in FWPDS's destructor
                // (indirectly), try to unmark the_trans. Thus we will mark
                // it, destroy the FWPDS, then make sure it is still marked.
                the_trans->mark();
                delete wpds;
                wpds = NULL;

                return the_trans->marked();
            }

            TEST(wali$wpds$fwpds$$fwpds, poststarDestructorDoesNotTrashMemoryEmptyFwpds)
            {
                EXPECT_TRUE(test_memory(true, false));
            }

            TEST(wali$wpds$fwpds$$fwpds, poststarDestructorDoesNotTrashMemoryNonemptyFwpds)
            {
                EXPECT_TRUE(test_memory(true, true));
            }

            TEST(wali$wpds$fwpds$$fwpds, prestarDestructorDoesNotTrashMemoryEmptyFwpds)
            {
                EXPECT_TRUE(test_memory(false, false));
            }

            TEST(wali$wpds$fwpds$$fwpds, prestarDestructorDoesNotTrashMemoryNonemptyFwpds)
            {
                EXPECT_TRUE(test_memory(false, true));
            }
        }
    }
}
