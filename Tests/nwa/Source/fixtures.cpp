#include "gtest/gtest.h"

#include "fixtures.hpp"

namespace wali
{
    namespace nwa
    {
        //////////////////////////////////
        // Supporting stuff

        SomeElements::SomeElements()
            : state(getKey("some state"))
            , state2(getKey("another state"))
            , state3(getKey("a third state"))
            , symbol(getKey("a symbol!"))
            , internal(state, symbol, state2)
            , call(state, symbol, state2)
            , ret(state, state3, symbol, state2)
        {}

        void
        SomeElements::add_to_nwa(NWA * nwa)
        {
            SomeElements elements;
                
            nwa->addInitialState(elements.state);
            nwa->addFinalState(elements.state2);
                
            nwa->addInternalTrans(elements.internal);
            nwa->addCallTrans(elements.call);
            nwa->addReturnTrans(elements.ret);
        }

        void
        SomeElements::expect_not_present(NWA const & nwa)
        {
            SomeElements elements;
                
            EXPECT_FALSE(nwa.isState(elements.state));
            EXPECT_FALSE(nwa.isInitialState(elements.state));
            EXPECT_FALSE(nwa.isFinalState(elements.state));
            EXPECT_FALSE(nwa.isSymbol(elements.state));
                
            // Should always be true
            EXPECT_FALSE(nwa.isSymbol(WALI_EPSILON));
            EXPECT_FALSE(nwa.isSymbol(WALI_WILD));
        }

        void
        expect_present(NWA const & nwa)
        {
            SomeElements elements;
                
            EXPECT_TRUE(nwa.isState(elements.state));
            EXPECT_TRUE(nwa.isInitialState(elements.state));
            EXPECT_TRUE(nwa.isFinalState(elements.state));
            EXPECT_TRUE(nwa.isSymbol(elements.state));
                
            // Should always be true
            EXPECT_FALSE(nwa.isSymbol(WALI_EPSILON));
            EXPECT_FALSE(nwa.isSymbol(WALI_WILD));
        }

        

        OddNumEvenGroupsNwa::OddNumEvenGroupsNwa()
        {
            build_nwa(&nwa);
        }
            
        void
        OddNumEvenGroupsNwa::build_nwa(NWA * nwa)
        {
            // From NWA-tests.cpp in Source/opennwa if you want to trace
            // history.
                
            // Accepts NWs with an odd number of () groups, each of which
            // has an even number of 0s.
            //
            // q1 accepts; there is a self loop on each of q0 and q1 on 0.
            // 
            //
            //            ,---.                       ,---.
            //           /     \                     //```\ \.
            //     ---> (  q0   ) XXXXX             (( q1  ))
            //           \     / XX                  \\___//
            //            `---'  X XX               ,'`---'
            //             :     X   xx          ,-'    X
            //             :        ) pop q1   ,'      XXX
            //  ( push q0  :             xx  ,'       X X X
            //             :               XX           X
            //             :            ,-'  XX         X  ) pop q0
            //             :      ( push q1    XX       X
            //             :   :    ,'           XX     X
            //           : : ; : ,-'               XX   X
            //            ';'  ----                  XX X
            //           ,---.                    \    ,---.
            //          /     \   -----------------/  /     \.
            //         (  q2   )  /     0     0   /  (   q3  )
            //          \     /  X___________________ \     /
            //           `---'    \                    `---'
            //             :
            //           : : :  epsilon, wild (internal, call, and return)
            //            ':'
            //           ,---.
            //          /     \.
            //         ( dummy )
            //          \     /
            //           `---'

            nwa->addInitialState(q0);
            nwa->addFinalState(q1);
                
            nwa->addInternalTrans(q2, zero, q3);
            nwa->addInternalTrans(q3, zero, q2);
                
            nwa->addCallTrans(q0, call, q2);
            nwa->addCallTrans(q1, call, q2);
            nwa->addReturnTrans(q3, q1, ret, q0);
            nwa->addReturnTrans(q3, q0, ret, q1);


            nwa->addInternalTrans(q2, WALI_EPSILON, dummy);
            nwa->addInternalTrans(q2, WALI_WILD, dummy);
            nwa->addCallTrans(q2, WALI_WILD, dummy);
            nwa->addReturnTrans(q2, q1, WALI_WILD, dummy);
        }
        
        const State OddNumEvenGroupsNwa::q0 = getKey("q0");
        const State OddNumEvenGroupsNwa::q1 = getKey("q1");
        const State OddNumEvenGroupsNwa::q2 = getKey("q2");
        const State OddNumEvenGroupsNwa::q3 = getKey("q3");
        const State OddNumEvenGroupsNwa::dummy = getKey("dummy");
        
        const Symbol OddNumEvenGroupsNwa::zero = getKey("0");
        const Symbol OddNumEvenGroupsNwa::call = getKey("(");
        const Symbol OddNumEvenGroupsNwa::ret = getKey(")");
        
    }
}
