#include <wali/xfa/Xfa.hpp>
#include <wali/domains/binrel/ProgramBddContext.hpp>

#include <string>

namespace wali {
    namespace xfa {

        using wali::domains::binrel::ProgramBddContext;
        using wali::domains::binrel::BinRel;

        struct Relations {
            BinaryRelation false_
                , true_
                , id
                , set_to_zero
                , set_to_one
                , increment
                , one_to_one
                , zero_to_zero;

            Relations(ProgramBddContext const & voc,
                      std::string var)
            {
                //false_ = new BinRel(&voc, voc.False());
                true_  = new BinRel(&voc, voc.True());
                false_ = dynamic_cast<BinRel*>(true_->zero().get_ptr());
                id     = new BinRel(&voc, voc.Assume(voc.True(), voc.True()));
                set_to_zero  = new BinRel(&voc, voc.Assign(var, voc.Const(0)));
                set_to_one   = new BinRel(&voc, voc.Assign(var, voc.Const(1)));
                increment    = new BinRel(&voc, voc.Assign(var, voc.Plus(voc.From(var), voc.Const(1))));
                one_to_one   = new BinRel(&voc, voc.Assume(voc.From(var), voc.Const(1)));
                zero_to_zero = new BinRel(&voc, voc.Assume(voc.From(var), voc.Const(0)));
            }
        };


        struct XfaContext
        {
            ProgramBddContext voc;
            Relations left, right;

            XfaContext()
                : voc(make_variables())
                , left(voc, "left")
                , right(voc, "right")
            {}


        private:
            static std::map<std::string, int> make_variables() {
                std::map<std::string, int> vars;
                vars["left"] = 2;
                vars["right"] = 2;
                return vars;
            }
        };



        struct Words
        {
            Xfa::Word epsilon, a, aa, aaa, aaaa, a5, a6, a7, a8, b, bb, ab, ba;

            Words() {
                Key A = getKey("a");
                Key B = getKey("b");
                
                a.push_back(A);

                for (int i=0; i<2; ++i) {
                    aa.push_back(A);
                }

                for (int i=0; i<3; ++i) {
                    aaa.push_back(A);
                }

                for (int i=0; i<4; ++i) {
                    aaaa.push_back(A);
                }

                for (int i=0; i<5; ++i) {
                    a5.push_back(A);
                }

                for (int i=0; i<6; ++i) {
                    a6.push_back(A);
                }

                for (int i=0; i<7; ++i) {
                    a7.push_back(A);
                }

                for (int i=0; i<8; ++i) {
                    a8.push_back(A);
                }

                b.push_back(B);

                bb.push_back(B);
                bb.push_back(B);

                ab.push_back(A);
                ab.push_back(B);

                ba.push_back(B);
                ba.push_back(A);
            }
            
        };
        

        // { }
        //
        //
        // --> o
        //
        struct TrivialEmptyXfa {
            Xfa xfa;
            State state;

            TrivialEmptyXfa(BinaryRelation zero)
                : state(getKey("q"))
            {
                xfa.addState(state, zero);
                xfa.setInitialState(state);
            }
        };


        // { eps }
        //
        //
        // --> (o)
        //
        struct AcceptEmptyStringOnly {
            Xfa xfa;
            State state;

            AcceptEmptyStringOnly(BinaryRelation zero)
                : state(getKey("q"))
            {
                xfa.addState(state, zero);
                xfa.setInitialState(state);
                xfa.addFinalState(state);
            }
        };


        //
        // -->o         (o)
        //
        struct EmptyByMissingTransition {
            Xfa xfa;
            State start, accept;

            EmptyByMissingTransition(BinaryRelation zero)
                : start(getKey("p"))
                , accept(getKey("q"))
            {
                xfa.addState(start, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);
            }
        };


        // { }
        //
        //
        //       .  .
        //       .  .   [[false_]]
        //
        // -->o ------> (o)
        //        a
        //
        struct EmptyByImpossibleDataTransition {
            Xfa xfa;
            State start, accept;
            Symbol a;

            EmptyByImpossibleDataTransition(BinaryRelation zero, Relations const & rels)
                : start(getKey("p"))
                , accept(getKey("q"))
                , a(getKey("a"))
            {
                xfa.addState(start, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);

                xfa.addTrans(start, a, accept, rels.false_);
            }
        };
            

        // { a }
        //
        //
        //       . __ .
        //       . __ .   [[id]]
        //
        // -->o ------> (o)
        //        a
        //
        struct SingleSimpleTrans {
            Xfa xfa;
            State start, accept;
            Symbol a;

            SingleSimpleTrans(BinaryRelation zero, Relations const & rels)
                : start(getKey("p"))
                , accept(getKey("q"))
                , a(getKey("a"))
            {
                xfa.addState(start, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);

                xfa.addTrans(start, a, accept, rels.id);
            }
        };
            

        // { aa, aaa, aaa, ... }
        //
        //    _
        //   / \ a
        //   \ /
        // -->o ------> (o)
        //        a
        //
        // All transitions id
        //
        struct BoringCycle {
            Xfa xfa;
            State start, accept;
            Symbol a;

            BoringCycle(BinaryRelation zero, Relations const & rels)
                : start(getKey("p"))
                , accept(getKey("q"))
                , a(getKey("a"))
            {
                xfa.addState(start, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);

                xfa.addTrans(start, a, start, rels.id);
                xfa.addTrans(start, a, accept, rels.id);
            }
        };
            


        // { aa, aaaa, aaaaaa, ..., a^(2n) }
        //
        //
        //             _
        //            / \ a;  \e.e[x->e[x]+1]  (increment)
        //            \ /
        // -->o -----> o ------> (o)
        //        a         a
        //   \e.e[x->0]    \e.(if e[x]=0 then e else {})
        //  (set_to_zero)  (zero_to_zero)
        //
        // All transitions id
        //
        struct DataRestrictedCycle {
            Xfa xfa;
            State start, middle, accept;
            Symbol a;

            DataRestrictedCycle(BinaryRelation zero, Relations const & rels)
                : start(getKey("p"))
                , middle(getKey("middle"))
                , accept(getKey("q"))
                , a(getKey("a"))
            {
                xfa.addState(start, zero);
                xfa.addState(middle, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);

                xfa.addTrans(start, a, middle, rels.set_to_zero);
                xfa.addTrans(middle, a, middle, rels.increment);
                xfa.addTrans(middle, a, accept, rels.zero_to_zero);
            }
        };
            

        // { ab }
        //
        //
        // -->o -----> o ------> (o)
        //        a         b
        //      true      true
        //
        struct SimpleAB {
            Xfa xfa;
            State start, middle, accept;
            Symbol a, b;

            SimpleAB(BinaryRelation zero, Relations const & rels)
                : start(getKey("p"))
                , middle(getKey("middle"))
                , accept(getKey("q"))
                , a(getKey("a"))
                , b(getKey("b"))
            {
                xfa.addState(start, zero);
                xfa.addState(middle, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);

                xfa.addTrans(start, a, middle, rels.true_);
                xfa.addTrans(middle, b, accept, rels.true_);
            }
        };



        // { }
        //
        //      .    .   . __ .
        //      . __ .   .    .
        //
        // -->o -----> o ------> (o)
        //        a         b
        //   (one_to_one) (zero_to_zero)
        //
        struct ImpossibleSequence {
            Xfa xfa;
            State start, middle, accept;
            Symbol a, b;

            ImpossibleSequence(BinaryRelation zero, Relations const & rels)
                : start(getKey("p"))
                , middle(getKey("middle"))
                , accept(getKey("q"))
                , a(getKey("a"))
                , b(getKey("b"))
            {
                xfa.addState(start, zero);
                xfa.addState(middle, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);

                xfa.addTrans(start, a, middle, rels.one_to_one);
                xfa.addTrans(middle, b, accept, rels.zero_to_zero);
            }
        };



        // { a }
        //          a (true)
        //       ______
        //      /     _\|
        // --> o       (o)
        //      \______/|
        //          b (true)
        //
        struct AOrB {
            Xfa xfa;
            State start, accept;
            Symbol a, b;

            AOrB(BinaryRelation zero, Relations const & rels)
                : start(getKey("p"))
                , accept(getKey("q"))
                , a(getKey("a"))
                , b(getKey("b"))
            {
                xfa.addState(start, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);

                xfa.addTrans(start, a, accept, rels.true_);
                xfa.addTrans(start, b, accept, rels.true_);
            }
        };


        // { a }
        //          a (true)
        //       ______
        //      /     _\|
        // --> o       (o)
        //      \______/|
        //          b (false)
        //
        struct AlternativePathImpossible {
            Xfa xfa;
            State start, accept;
            Symbol a, b;

            AlternativePathImpossible(BinaryRelation zero, Relations const & rels)
                : start(getKey("p"))
                , accept(getKey("q"))
                , a(getKey("a"))
                , b(getKey("b"))
            {
                xfa.addState(start, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);

                xfa.addTrans(start, a, accept, rels.true_);
                xfa.addTrans(start, b, accept, rels.false_);
            }
        };
            
        // { }
        //
        //
        //       .  .
        //       .  .   [[false_]]
        //
        // -->o ------> (o)
        //        eps
        //
        struct EmptyByImpossibleDataTransitionEpsilon {
            Xfa xfa;
            State start, accept;
            Symbol epsilon;

            EmptyByImpossibleDataTransitionEpsilon(BinaryRelation zero, Relations const & rels)
                : start(getKey("p"))
                , accept(getKey("q"))
                , epsilon(wali::WALI_EPSILON)
            {
                xfa.addState(start, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);

                xfa.addTrans(start, epsilon, accept, rels.false_);
            }
        };
            

        // { eps }
        //
        //
        //       . __ .
        //       . __ .   [[id]]
        //
        // -->o ------> (o)
        //        eps
        //
        struct SingleSimpleTransEpsilon {
            Xfa xfa;
            State start, accept;
            Symbol epsilon;

            SingleSimpleTransEpsilon(BinaryRelation zero, Relations const & rels)
                : start(getKey("p"))
                , accept(getKey("q"))
                , epsilon(wali::WALI_EPSILON)
            {
                xfa.addState(start, zero);
                xfa.addState(accept, zero);
                xfa.setInitialState(start);
                xfa.addFinalState(accept);

                xfa.addTrans(start, epsilon, accept, rels.id);
            }
        };
    }
}
