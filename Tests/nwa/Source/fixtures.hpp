#include "wali/nwa/NWA.hpp"

namespace wali
{
    namespace nwa
    {
        //////////////////////////////////
        // Supporting stuff

        class SomeElements
        {
        public:
            State state, state2, state3;
            Symbol symbol;
            NWA::Internal internal;
            NWA::Call call;
            NWA::Return ret;

            SomeElements();

            static void add_to_nwa(NWA * nwa);

            static void expect_not_present(NWA const & nwa);

            static void expect_present(NWA const & nwa);
        };
        

        class OddNumEvenGroupsNwa
        {
        public:
            NWA nwa;

            OddNumEvenGroupsNwa();
            
            static void build_nwa(NWA * nwa);

            static const State q0, q1, q2, q3, dummy;
            static const Symbol call, ret, zero;
            
            // From NWA-tests.cpp in Source/wali/nwa if you want to trace
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
        };


        ///////////////////////////////////////////////////////////////////
        // The following are parenthesis languages with 0s allowed anywhere

        // Accepts:
        //      S -> S S
        //         | ( S )
        //         | 0
        //         | epsilon
        class AcceptsBalancedOnly
        {
        public:
            //            ( push q0
            //        /-------------->  /---\.
            // -->((q0))              q1    |  ( push q1
            //    / /\<-------------/  /\   |  ) pop q1
            //   /  |    ) pop q0       |   |  0
            //   |  |                    \_/
            //   \_/
            //    0

            NWA nwa;
            State const q0, q1;
            Symbol const zero, call, ret;

            AcceptsBalancedOnly()
                : q0(getKey("q0")), q1(getKey("q1"))
                , zero(getKey("0")), call(getKey("(")), ret(getKey(")"))
            {
                nwa.addInitialState(q0);
                nwa.addFinalState(q0);
                
                nwa.addInternalTrans(q0, zero, q0);
                nwa.addInternalTrans(q1, zero, q1);
                nwa.addCallTrans(q0, call, q1);
                nwa.addCallTrans(q1, call, q1);
                nwa.addReturnTrans(q1, q1, ret, q1);
                nwa.addReturnTrans(q1, q0, ret, q0);
            }
        };
        

        // Accepts  <balanced> (+
        class AcceptsStrictlyUnbalancedLeft
        {
        public:
            //            ( push q0
            //        /-------------->  /---\.
            // -->  q0              ((q1))  |  ( push q1
            //    / /\<-------------/  /\   |  ) pop q1
            //   /  |    ) pop q0       |   |  0
            //   |  |                    \_/
            //   \_/
            //    0
            //
            // Just like AcceptsBalancedOnly except with q1 as the accepting
            // state instead of q0.

            NWA nwa;
            State const q0, q1;
            Symbol const zero, call, ret;

            AcceptsStrictlyUnbalancedLeft()
                : q0(getKey("q0")), q1(getKey("q1"))
                , zero(getKey("0")), call(getKey("(")), ret(getKey(")"))
            {
                nwa.addInitialState(q0);
                nwa.addFinalState(q1);
                
                nwa.addInternalTrans(q0, zero, q0);
                nwa.addInternalTrans(q1, zero, q1);
                nwa.addCallTrans(q0, call, q1);
                nwa.addCallTrans(q1, call, q1);
                nwa.addReturnTrans(q1, q1, ret, q1);
                nwa.addReturnTrans(q1, q0, ret, q0);
            }

        };


        // Accepts  <balanced> (*
        class AcceptsPosssiblyUnbalancedLeft
        {
        public:
            //            ( push q0
            //        /-------------->  /---\.
            // -->((q0))            ((q1))  |  ( push q1
            //    / /\<-------------/  /\   |  ) pop q1
            //   /  |    ) pop q0       |   |  0
            //   |  |                    \_/
            //   \_/
            //    0
            //
            // Just like AcceptsBalancedOnly except with both q0 and q1
            // accepting.

            NWA nwa;
            State const q0, q1;
            Symbol const zero, call, ret;

            AcceptsPosssiblyUnbalancedLeft()
                : q0(getKey("q0")), q1(getKey("q1"))
                , zero(getKey("0")), call(getKey("(")), ret(getKey(")"))
            {
                nwa.addInitialState(q0);
                nwa.addFinalState(q0);
                nwa.addFinalState(q1);
                
                nwa.addInternalTrans(q0, zero, q0);
                nwa.addInternalTrans(q1, zero, q1);
                nwa.addCallTrans(q0, call, q1);
                nwa.addCallTrans(q1, call, q1);
                nwa.addReturnTrans(q1, q1, ret, q1);
                nwa.addReturnTrans(q1, q0, ret, q0);
            }

        };


        // Accepts )* <balanced>
        class AcceptsPossiblyUnbalancedRight
        {
        public:
            //            ( push q0
            //        /-------------->  /---\.
            // -->((q0))              q1    |  ( push q1
            //    / /\<-------------/  /\   |  ) pop q1
            //   /  |    ) pop q0       |   |  0
            //   |  |                    \_/
            //   \_/
            //    0
            //    ) pop q0
            //
            // Just like AcceptsBalancedOnly except with extra q0->q0 self
            // loop on ") pop q0".

            NWA nwa;
            State const q0, q1;
            Symbol const zero, call, ret;

            AcceptsPossiblyUnbalancedRight()
                : q0(getKey("q0")), q1(getKey("q1"))
                , zero(getKey("0")), call(getKey("(")), ret(getKey(")"))
            {
                nwa.addInitialState(q0);
                nwa.addFinalState(q0);
                
                nwa.addInternalTrans(q0, zero, q0);
                nwa.addInternalTrans(q1, zero, q1);
                nwa.addCallTrans(q0, call, q1);
                nwa.addCallTrans(q1, call, q1);
                nwa.addReturnTrans(q1, q1, ret, q1);
                nwa.addReturnTrans(q1, q0, ret, q0);
                nwa.addReturnTrans(q0, q0, ret, q0);
            }

        };

        // Accepts )* <balanced> (*
        class AcceptsPositionallyConsistentString
        {
        public:
            //            ( push q0
            //        /-------------->  /---\.
            // -->((q0))            ((q1))  |  ( push q1
            //    / /\<-------------/  /\   |  ) pop q1
            //   /  |    ) pop q0       |   |  0
            //   |  |                    \_/
            //   \_/
            //    0
            //    ) pop q0
            //
            // Just like AcceptsBalancedOnly except with q1 accepting too and
            // the extra q0->q0 self loop on ") pop q0"

            NWA nwa;
            State const q0, q1;
            Symbol const zero, call, ret;

            AcceptsPositionallyConsistentString()
                : q0(getKey("q0")), q1(getKey("q1"))
                , zero(getKey("0")), call(getKey("(")), ret(getKey(")"))
            {
                nwa.addInitialState(q0);
                nwa.addFinalState(q0);
                nwa.addFinalState(q1);
                
                nwa.addInternalTrans(q0, zero, q0);
                nwa.addInternalTrans(q1, zero, q1);
                nwa.addCallTrans(q0, call, q1);
                nwa.addCallTrans(q1, call, q1);
                nwa.addReturnTrans(q1, q1, ret, q1);
                nwa.addReturnTrans(q1, q0, ret, q0);
                nwa.addReturnTrans(q0, q0, ret, q0);
            }

        };

        // Accepts )+ <balanced>
        class AcceptsStrictlyUnbalancedRight
        {
        public:
            //            ( push q0
            //        /-------------->  /---\.
            // -->  q0                q1    |  ( push q1
            //    / /\<-------------/  /\   |  ) pop q1
            //   /  |    ) pop q0       |   |  0
            //  /|  |                    \_/
            // / \_/
            // |  0
            // |
            // |) pop q0
            // \          ( push q2
            // _\|    /-------------->  /---\.
            //    ((q2))              q3    |  ( push q3
            //    / /\<-------------/  /\   |  ) pop q3
            //   /  |    ) pop q2       |   |  0
            //   |  |                    \_/
            //   \_/
            //    0
            //
            // This has two "copies" of AcceptsBalancedOnly, with the extra
            // transition between them and the limited accept states.

            NWA nwa;
            State const q0, q1, q2, q3;
            Symbol const zero, call, ret;

            AcceptsStrictlyUnbalancedRight()
                : q0(getKey("q0")), q1(getKey("q1"))
                , q2(getKey("q2")), q3(getKey("q3"))
                , zero(getKey("0")), call(getKey("(")), ret(getKey(")"))
            {
                nwa.addInitialState(q0);
                nwa.addFinalState(q2);

                // Copy 0
                nwa.addInternalTrans(q0, zero, q0);
                nwa.addInternalTrans(q1, zero, q1);
                nwa.addCallTrans(q0, call, q1);
                nwa.addCallTrans(q1, call, q1);
                nwa.addReturnTrans(q1, q1, ret, q1);
                nwa.addReturnTrans(q1, q0, ret, q0);

                // Copy 1
                nwa.addInternalTrans(q2, zero, q2);
                nwa.addInternalTrans(q3, zero, q3);
                nwa.addCallTrans(q2, call, q3);
                nwa.addCallTrans(q3, call, q3);
                nwa.addReturnTrans(q3, q3, ret, q3);
                nwa.addReturnTrans(q3, q2, ret, q2);

                // Connector
                nwa.addReturnTrans(q0, q0, ret, q2);
            }

        };
    }
}
