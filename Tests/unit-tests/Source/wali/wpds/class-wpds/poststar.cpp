#include "gtest/gtest.h"

#include "wali/Reach.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wfa/TransFunctor.hpp"

using namespace wali;
using namespace wali::wpds;
using namespace wali::wfa;

namespace {
    struct SimpleQuery
    {
        Key start, accept, symbol;
        WFA wfa;
        sem_elem_t one, zero;

        SimpleQuery()
            : start(getKey("start"))
            , accept(getKey("accept"))
            , symbol(getKey("symbol"))
            , one(Reach(true).one())
            , zero(Reach(true).zero())
        {
            wfa.addState(start, zero);
            wfa.addState(accept, zero);
            wfa.setInitialState(start);
            wfa.addFinalState(accept);
            wfa.addTrans(start, symbol, accept, one);
        }
    };
}

TEST(wali$wpds$WPDS$prestar, canTakePrestarOfEmptyWpds)
{
    WPDS empty;
    SimpleQuery query;
    WFA result = empty.prestar(query.wfa);
    TransCounter counter;
    result.for_each(counter);
    EXPECT_EQ(2u, result.numStates());
    EXPECT_EQ(1u, counter.getNumTrans());
}

TEST(wali$wpds$WPDS$poststar, canTakePoststarOfEmptyWpds)
{
    WPDS empty;
    SimpleQuery query;
    WFA result = empty.poststar(query.wfa);
    TransCounter counter;
    result.for_each(counter);
    EXPECT_EQ(2u, result.numStates());
    EXPECT_EQ(1u, counter.getNumTrans());
}
