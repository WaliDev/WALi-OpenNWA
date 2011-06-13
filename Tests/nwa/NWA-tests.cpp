#include "gtest/gtest.h"

#include "wali/nwa/NWA.hpp"

namespace wali
{
    namespace nwa
    {
        //////////////////////////////////
        // Supporting stuff

        class OddNumEvenGroupsNwa
        {
        public:
            NWA nwa; 
            
            OddNumEvenGroupsNwa() {
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

                Key q0 = getKey("q0");
                Key q1 = getKey("q1");
                Key q2 = getKey("q2");
                Key q3 = getKey("q3");
                
                Key zero = getKey("0");
                Key call = getKey("(");
                Key ret = getKey(")");
                
                nwa.addInitialState(q0);
                nwa.addFinalState(q1);
                
                nwa.addInternalTrans(q2, zero, q3);
                nwa.addInternalTrans(q3, zero, q2);
                
                nwa.addCallTrans(q0, call, q2);
                nwa.addCallTrans(q1, call, q2);
                nwa.addReturnTrans(q3, q1, ret, q0);
                nwa.addReturnTrans(q3, q0, ret, q1);

                Key dummy = getKey("dummy");
                nwa.addInternalTrans(q2, WALI_EPSILON, dummy);
                nwa.addInternalTrans(q2, WALI_WILD, dummy);
                nwa.addCallTrans(q2, WALI_WILD, dummy);
                nwa.addReturnTrans(q2, q2, WALI_WILD, dummy);
            }
        };
        
        
        ///
        /// Uses EXPECT macros to make sure 'nwa' is empty
        void expect_nwa_is_empty(NWA const & nwa)
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
        void expect_nwas_are_equal(NWA const & a, NWA const & b)
        {
            EXPECT_EQ(a, b);

            EXPECT_EQ(a.getStates(), b.getStates());
            EXPECT_EQ(a.getInitialStates(), b.getInitialStates());
            EXPECT_EQ(a.getFinalStates(), b.getFinalStates());
            EXPECT_EQ(a.getSymbols(), b.getSymbols());

            std::vector<NWA::Internal>
                a_internals(a.beginInternalTrans(), a.endInternalTrans()),
                b_internals(b.beginInternalTrans(), b.endInternalTrans());
            std::vector<NWA::Call>
                a_calls(a.beginCallTrans(), a.endCallTrans()),
                b_calls(b.beginCallTrans(), b.endCallTrans());
            std::vector<NWA::Return>
                a_returns(a.beginReturnTrans(), a.endReturnTrans()),
                b_returns(b.beginReturnTrans(), b.endReturnTrans());
            
            EXPECT_EQ(a_internals, b_internals);
            EXPECT_EQ(a_calls, b_calls);
            EXPECT_EQ(a_returns, b_returns);
        }


        /// Adds stuff to 'copy' and makes sure it doesn't change
        /// 'reference'.
        void expect_nwas_are_physically_different(NWA const & reference, NWA & copy)
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
            
            NWA::Internal internal(state, symbol, initial);
            NWA::Call call(state, symbol, initial);
            NWA::Return ret(state, state, symbol, state);

            copy.addInternalTrans(internal);
            EXPECT_EQ(copy.sizeInternalTrans(), reference.sizeInternalTrans() + 1);
            
            copy.addCallTrans(call);
            EXPECT_EQ(copy.sizeCallTrans(), reference.sizeCallTrans() + 1);
            
            copy.addReturnTrans(ret);
            EXPECT_EQ(copy.sizeReturnTrans(), reference.sizeReturnTrans() + 1);
        }

        
        /////////////////////////////////
        // Now begin the actual tests
        

        // NWA()
        //  - Test that a new NWA is empty
        TEST(wali$nwa$NWA$NWA$$default, newNwaIsEmpty)
        {
            NWA empty;
            expect_nwa_is_empty(empty);
        }

        // NWA(NWA const & other)
        //  - Test that a copy of an empty NWA is empty
        TEST(wali$nwa$NWA$NWA$$copy, copyOfEmptyNwaIsEmpty)
        {
            NWA empty;
            NWA also_empty = empty;

            expect_nwa_is_empty(also_empty);
        }
        
        //  - Test that a copy of a non-empty NWA has all the same states
        TEST(wali$nwa$NWA$NWA$$copy, copyOfNonemptyNwaIsSame)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy = fixture.nwa;

            expect_nwas_are_equal(fixture.nwa, copy);
        }
        
        //  - Test that the NWAs do not share structure (changing one doesn't
        //    change the other)
        TEST(wali$nwa$NWA$NWA$$copy, copyOfNonemptyNwaDoesNotShareState)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy = fixture.nwa;

            expect_nwas_are_physically_different(fixture.nwa, copy);
        }

        //  - Test that client information is equal but not shared
        TEST(wali$nwa$NWA$NWA$$copy, clientInformationIsCloned)
        {
            // TODO: tests (also add to assignment test)
        }
        
        
        // operator= (NWA const & other)
        //  - Same questions as NWA(NWA const &)
        TEST(wali$nwa$NWA$operatorEquals, copyOfEmptyNwaIsEmpty)
        {
            NWA empty, also_empty;
            also_empty = empty;

            expect_nwa_is_empty(also_empty);
        }

        TEST(wali$nwa$NWA$operatorEquals, copyOfNonemptyNwaIsSame)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy;
            copy = fixture.nwa;

            expect_nwas_are_equal(fixture.nwa, copy);
        }
        
        TEST(wali$nwa$NWA$operatorEquals, copyOfNonemptyNwaDoesNotShareState)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy;
            copy = fixture.nwa;

            expect_nwas_are_physically_different(fixture.nwa, copy);
        }

        //  - Test self assignment
        TEST(wali$nwa$NWA$operatorEquals, selfAssignmentDoesNotDestroyNwa)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy = fixture.nwa;

            copy = copy;

            expect_nwas_are_equal(fixture.nwa, copy);
        }

        TEST(wali$nwa$NWA$operatorEquals, selfAssignmentDoesNotCopy)
        {
            OddNumEvenGroupsNwa fixture;
            NWA copy = fixture.nwa;

            NWA::StateIterator
                states = copy.beginStates(),
                initials = copy.beginInitialStates(),
                finals = copy.beginFinalStates();

            NWA::SymbolIterator symbols = copy.beginSymbols();

            NWA::InternalIterator internals = copy.beginInternalTrans();
            NWA::CallIterator calls = copy.beginCallTrans();
            NWA::ReturnIterator returns = copy.beginReturnTrans();

            copy = copy;

            EXPECT_EQ(states, copy.beginStates());
            EXPECT_EQ(initials, copy.beginInitialStates());
            EXPECT_EQ(finals, copy.beginFinalStates());
            EXPECT_EQ(symbols, copy.beginSymbols());
            EXPECT_EQ(internals, copy.beginInternalTrans());
            EXPECT_EQ(calls, copy.beginCallTrans());
            EXPECT_EQ(returns, copy.beginReturnTrans());
        }

        

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
        
        
        // For XXX in each of States, InitialStates, FinalStates, InternalTrans,
        // CallTrans, ReturnTrans:
        // 
        //   getXXX()
        //     - I don't think we can really do anything with this separately. This
        //       is tested in concert with other functions except as follows:
        // 
        //     - For getSymbols(), check in an automaton that uses epsilon and wild on
        //       transitions. Make sure that neither is present in the given set.
        TEST(wali$nwa$NWA$getSymbols, epsilonAndWildAreNotPresent)
        {
            NWA nwa;
            Key q = getKey('q');
            nwa.addInternalTrans(q, WALI_EPSILON, q);
            nwa.addInternalTrans(q, WALI_WILD, q);

            // Sanity check
            EXPECT_EQ(2u, nwa.sizeInternalTrans());

            // Actual check (see checkSizeOfEmptyIsConsistent below)
            EXPECT_EQ(0u, nwa.sizeSymbols());
        }

        
        //   sizeXXX()
        //     - Make sure this agrees with the size of getXXX()
        void expect_size_consistent_with_range(NWA const & nwa)
        {
#define CHECK_CONSISTENCY(Attribute)                                             \
            EXPECT_EQ(nwa.size##Attribute(),                                     \
                      static_cast<size_t>(std::distance(nwa.begin##Attribute(),  \
                                                        nwa.end##Attribute())))
            
            CHECK_CONSISTENCY(States);
            CHECK_CONSISTENCY(InitialStates);
            CHECK_CONSISTENCY(FinalStates);
            CHECK_CONSISTENCY(Symbols);
            CHECK_CONSISTENCY(InternalTrans);
            CHECK_CONSISTENCY(CallTrans);
            CHECK_CONSISTENCY(ReturnTrans);
#undef CHECK_CONSISTENCY
        }
        
        TEST(wali$nwa$NWA$getXXX, checkSizeConsistentForEmptyNwa)
        {
            NWA empty;
            expect_size_consistent_with_range(empty);
        }

        TEST(wali$nwa$NWA$getXXX, checkSizeConsistentForBigNwa)
        {
            OddNumEvenGroupsNwa fixture;
            expect_size_consistent_with_range(fixture.nwa);
        }


        //   isXXX()
        //     - Check if an XXX is a member of the empty NWA
        //     - Add unrelated items of each type; check.
        //     - Add item being checked to an empty NWA; check
        //     - Add both unrelated and checked item, then more unrelated items; check
        //     - For isSymbol(), make sure epsilon and wild return false both when they
        //       are used in transitions and not.
        // 
        //   addXXX()
        //     - Check that adding an XXX adds it to the set returned by getXXX and
        //       returns true
        //     - Check that adding it again leaves the return from getXXX unchanged,
        //       and returns false
        //     - Check that the sets that should not be modified are not modified.
        // 
        //     - For addInitialState and addFinalState, try from both the situation where
        //       the state was and wasn't present in the NWA at all. If they weren't 
        //       present, check that they are after the addition.
        //     - For addSymbol(), make sure adding epsilon or wild returns false even if
        //       they are the first things done
        //     - For addYYYTrans, try from the situation where the states and symbol are
        //       and are not present. If they were not, check that they are after the
        //       addition.
        //     - For addYYYTrans, add more than one transition with the given source/sym,
        //       source/tgt, and sym/tgt.
        // 
        //   removeXXX()
        //     - Try from empty automaton, one containing given item, one not containing
        //       given item. Check correct value is returned.
        //     - In each case, check that sets that shouldn't change are not modified.
        // 
        //     - For removeState(), try from automaton in which the state is and is not
        //       involved in any transitions. Make sure you get the state in all
        //       applicable coordinates: source/target of internal, call/entry of call,
        //       and exit/pred/return of return.
        //     - For removeState(), try from automaton in which the symbol is and is not
        //       involved in any transtions. Test all three kinds of transitions.
        // 
        //   clearXXX()
        //     - Make sure that the set of XXX is empty after.
        //     - Make sure that sets that shouldn't be different are not different.
        // 
        //     - For clearStates(), make sure initial/accepting states and transitions
        //       have all been cleared too.
        //     - For clearSymbols(), make sure transitions are all clear too.

    }
}
