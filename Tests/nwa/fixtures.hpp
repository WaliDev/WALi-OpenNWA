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
        
    }
}
