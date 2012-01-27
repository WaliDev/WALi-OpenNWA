#include "gtest/gtest.h"

#include "opennwa/Nwa.hpp"

#include "supporting.hpp"

namespace opennwa
{
        //////////////////////////////////
        // Supporting stuff

        
        ///
        /// Uses EXPECT macros to make sure 'nwa' is empty
        void expect_nwa_is_empty(Nwa const & nwa)
        {
            EXPECT_EQ(0u, nwa.sizeStates());
            EXPECT_EQ(0u, nwa.sizeInitialStates());
            EXPECT_EQ(0u, nwa.sizeFinalStates());
            EXPECT_EQ(0u, nwa.sizeSymbols());
            EXPECT_EQ(0u, nwa.sizeTrans());
            EXPECT_EQ(0u, nwa.sizeInternalTrans());
            EXPECT_EQ(0u, nwa.sizeCallTrans());
            EXPECT_EQ(0u, nwa.sizeReturnTrans());
        }

        ///
        /// Uses EXPECT macros to make sure NWAs are equal
        void expect_nwas_are_equal(Nwa const & a, Nwa const & b)
        {
            EXPECT_EQ(a, b);

            EXPECT_EQ(a.getStates(), b.getStates());
            EXPECT_EQ(a.getInitialStates(), b.getInitialStates());
            EXPECT_EQ(a.getFinalStates(), b.getFinalStates());
            EXPECT_EQ(a.getSymbols(), b.getSymbols());

            std::vector<Nwa::Internal>
                a_internals(a.beginInternalTrans(), a.endInternalTrans()),
                b_internals(b.beginInternalTrans(), b.endInternalTrans());
            std::vector<Nwa::Call>
                a_calls(a.beginCallTrans(), a.endCallTrans()),
                b_calls(b.beginCallTrans(), b.endCallTrans());
            std::vector<Nwa::Return>
                a_returns(a.beginReturnTrans(), a.endReturnTrans()),
                b_returns(b.beginReturnTrans(), b.endReturnTrans());
            
            EXPECT_EQ(a_internals, b_internals);
            EXPECT_EQ(a_calls, b_calls);
            EXPECT_EQ(a_returns, b_returns);
        }


        /// Adds stuff to 'copy' and makes sure it doesn't change
        /// 'reference'.
        void expect_nwas_are_physically_different(Nwa const & reference, Nwa & copy)
        {
            State state = getKey("additional state");
            State initial = getKey("additional initial state");
            State final = getKey("additional initial state");
            State symbol = getKey("additional symbol");

#define ADD_AND_CHECK_NOT_IN_COPY(kind, item) \
            copy.add##kind(item); \
            EXPECT_FALSE(reference.is##kind(item))

            ADD_AND_CHECK_NOT_IN_COPY(State, state);
            ADD_AND_CHECK_NOT_IN_COPY(InitialState, initial);
            ADD_AND_CHECK_NOT_IN_COPY(FinalState, final);
            ADD_AND_CHECK_NOT_IN_COPY(Symbol, symbol);

#undef ADD_AND_CHECK_NOT_IN_COPY
            
            Nwa::Internal internal(state, symbol, initial);
            Nwa::Call call(state, symbol, initial);
            Nwa::Return ret(state, state, symbol, state);

            copy.addInternalTrans(internal);
            EXPECT_EQ(copy.sizeInternalTrans(), reference.sizeInternalTrans() + 1);
            
            copy.addCallTrans(call);
            EXPECT_EQ(copy.sizeCallTrans(), reference.sizeCallTrans() + 1);
            
            copy.addReturnTrans(ret);
            EXPECT_EQ(copy.sizeReturnTrans(), reference.sizeReturnTrans() + 1);
        }


}
