#ifndef WALI_TESTS_WALI_NWA_WFA_FIXTURES_HPP
#define WALI_TESTS_WALI_NWA_WFA_FIXTURES_HPP

#include "wali/Key.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/Reach.hpp"

namespace wali {
    namespace wfa {


        struct Letters
        {
            Key a, b, c;

            Letters()
                : a(getKey("a"))
                , b(getKey("b"))
                , c(getKey("c"))
            {}
        };

        struct Words
        {
            typedef WFA::Word Word;

            Word epsilon, a, b, c, aa, ab, ac;

            Words() {
                Letters l;

                a.push_back(l.a);

                b.push_back(l.b);

                c.push_back(l.c);

                aa.push_back(l.a);
                aa.push_back(l.a);

                ab.push_back(l.a);
                ab.push_back(l.b);

                ac.push_back(l.a);
                ac.push_back(l.c);
            }
        };

        struct LoopReject
        {
            WFA wfa;
            Key state;

            LoopReject() {
                sem_elem_t one = Reach(true).one();
                sem_elem_t zero = Reach(true).zero();
                
                Letters l;
                state = getKey("state");

                wfa.addState(state, zero);
                wfa.setInitialState(state);
                wfa.addTrans(state, l.a, state, one);
                wfa.addTrans(state, l.b, state, one);
                wfa.addTrans(state, l.c, state, one);
            }
        };

        struct LoopAccept
        {
            WFA wfa;
            Key state;

            LoopAccept()
                : wfa( LoopReject().wfa )
                , state( LoopReject().state )
            {
                wfa.addFinalState(state);
            }
        };

        struct EvenAsEvenBs
        {
            WFA wfa;

            EvenAsEvenBs() {
                sem_elem_t one = Reach(true).one();
                sem_elem_t zero = Reach(true).zero();
                
                Key even_even = getKey("even_even");
                Key even_odd  = getKey("even_odd");
                Key odd_even  = getKey("odd_even");
                Key odd_odd   = getKey("odd_odd");
                Letters l;

                wfa.addState(even_even, zero);
                wfa.addState(even_odd, zero);
                wfa.addState(odd_even, zero);
                wfa.addState(odd_odd, zero);

                wfa.setInitialState(even_even);
                wfa.addFinalState(even_even);

                wfa.addTrans(even_even, l.a, odd_even,  one);
                wfa.addTrans(even_odd,  l.a, odd_odd,   one);
                wfa.addTrans(odd_even,  l.a, even_even, one);
                wfa.addTrans(odd_odd,   l.a, even_odd,  one);

                wfa.addTrans(even_even, l.b, even_odd,  one);
                wfa.addTrans(even_odd,  l.b, even_even, one);
                wfa.addTrans(odd_even,  l.b, odd_odd,   one);
                wfa.addTrans(odd_odd,   l.b, odd_even,  one);
            }
        };
        

        struct EpsilonTransitionToAccepting
        {
            //     eps
            // -->o-------->(o)
            WFA wfa;

            EpsilonTransitionToAccepting()
            {
                sem_elem_t one = Reach(true).one();
                sem_elem_t zero = Reach(true).zero();
                
                Key start = getKey("start");
                Key accept = getKey("accept");

                wfa.addState(start, zero);
                wfa.addState(accept, zero);

                wfa.setInitialState(start);
                wfa.addFinalState(accept);

                wfa.addTrans(start, WALI_EPSILON, accept, one);
            }
        };

        struct EpsilonTransitionToMiddleToAccepting
        {
            //      eps     a
            // -->o------->o------->(o)
            WFA wfa;

            EpsilonTransitionToMiddleToAccepting() {
                sem_elem_t one = Reach(true).one();
                sem_elem_t zero = Reach(true).zero();
                
                Key start = getKey("start");
                Key middle = getKey("middle");
                Key accept = getKey("accept");

                wfa.addState(start, zero);
                wfa.addState(middle, zero);
                wfa.addState(accept, zero);

                wfa.setInitialState(start);
                wfa.addFinalState(accept);

                wfa.addTrans(start, WALI_EPSILON, middle, one);
                wfa.addTrans(middle, Letters().a, accept, one);
            }
        };

        struct EpsilonTransitionToMiddleToEpsilonToAccepting
        {
            //      eps     a     eps
            // -->o------->o---->o----->(o)
            WFA wfa;

            EpsilonTransitionToMiddleToEpsilonToAccepting() {
                sem_elem_t one = Reach(true).one();
                sem_elem_t zero = Reach(true).zero();
                
                Key start = getKey("start");
                Key middle = getKey("middle");
                Key almost = getKey("almost");
                Key accept = getKey("accept");

                wfa.addState(start, zero);
                wfa.addState(middle, zero);
                wfa.addState(almost, zero);
                wfa.addState(accept, zero);

                wfa.setInitialState(start);
                wfa.addFinalState(accept);

                wfa.addTrans(start, WALI_EPSILON, middle, one);
                wfa.addTrans(middle, Letters().a, almost, one);
                wfa.addTrans(almost, WALI_EPSILON, accept, one);
            }
        };

        struct AcceptAbOrAcNondet
        {
            // -->o--------->o-------->(o)
            //    |      a       b
            //    | a
            //   \/     c
            //    o--------->(o)
            WFA wfa;

            AcceptAbOrAcNondet() {
                sem_elem_t one = Reach(true).one();
                sem_elem_t zero = Reach(true).zero();
                
                Key start = getKey("start");
                Key a_top = getKey("a (top)");
                Key a_left = getKey("a (left)");
                Key ab = getKey("ab");
                Key ac = getKey("ac");

                Letters l;

                wfa.addState(start, zero);
                wfa.addState(a_top, zero);
                wfa.addState(a_left, zero);
                wfa.addState(ab, zero);
                wfa.addState(ac, zero);

                wfa.setInitialState(start);
                wfa.addFinalState(ab);
                wfa.addFinalState(ac);

                wfa.addTrans(start, l.a, a_top, one);
                wfa.addTrans(start, l.a, a_left, one);
                wfa.addTrans(a_top, l.b, ab, one);
                wfa.addTrans(a_left, l.b, ac, one);
            }
        };
        
        }
}


#endif
