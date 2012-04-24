#include "gtest/gtest.h"

#include "opennwa/NestedWord.hpp"

#include "Tests/unit-tests/Source/opennwa/fixtures.hpp"


namespace opennwa {

        typedef NestedWord::Position Position;
        typedef NestedWord::const_iterator Iterator;
        
        Position::Type InternalType = Position::InternalType;
        Position::Type CallType = Position::CallType;
        Position::Type ReturnType = Position::ReturnType;


        TEST(opennwa$NestedWord, newNestedWordIsEmpty)
        {
            NestedWord w;

            EXPECT_EQ(0u, w.size());
            EXPECT_EQ(w.begin(), w.end());
        }


        TEST(opennwa$NestedWord, oneCharacterWords)
        {
            // wa(i,c,r) = word with "appendInternal", "appendCall", "appendReturn"
            // wapi(i,c,r) = word with "append", appending internal, call, or return
            NestedWord wai, wac, war, wapi, wapc, wapr;

            Symbol const s = getKey("a");

            wai.appendInternal(s);
            wac.appendCall(s);
            war.appendReturn(s);

            wapi.append(Position(s, InternalType));
            wapc.append(Position(s, CallType));
            wapr.append(Position(s, ReturnType));

            // Make sure the size of each is 1
            EXPECT_EQ(1u, wai.size());
            EXPECT_EQ(1u, wac.size());
            EXPECT_EQ(1u, war.size());
            EXPECT_EQ(1u, wapi.size());
            EXPECT_EQ(1u, wapc.size());
            EXPECT_EQ(1u, wapr.size());

            // Make sure the symbol of each is s
            EXPECT_EQ(s, wai.begin()->symbol);
            EXPECT_EQ(s, wac.begin()->symbol);
            EXPECT_EQ(s, war.begin()->symbol);
            EXPECT_EQ(s, wapi.begin()->symbol);
            EXPECT_EQ(s, wapc.begin()->symbol);
            EXPECT_EQ(s, wapr.begin()->symbol);

            // Make sure that the type of each thing is right
            EXPECT_EQ(InternalType, wai.begin()->type);
            EXPECT_EQ(CallType,     wac.begin()->type);
            EXPECT_EQ(ReturnType,   war.begin()->type);
            EXPECT_EQ(InternalType, wapi.begin()->type);
            EXPECT_EQ(CallType,     wapc.begin()->type);
            EXPECT_EQ(ReturnType,   wapr.begin()->type);
        }


        TEST(opennwa$NestedWord, longerWord)
        {
            Symbol const a = getKey("a");
            Symbol const b = getKey("b");
            Symbol const c = getKey("c");
            
            NestedWord w;
            w.appendReturn(a);
            w.appendInternal(b);
            w.appendReturn(c);
            w.appendCall(a);
            w.appendInternal(b);
            w.appendReturn(c);
            w.appendCall(a);

            EXPECT_EQ(7u, w.size());

            Iterator i = w.begin();

            ASSERT_NE(i, w.end());
            EXPECT_EQ(a, i->symbol);
            EXPECT_EQ(ReturnType, i->type);

            ++i;
            ASSERT_NE(i, w.end());
            EXPECT_EQ(b, i->symbol);
            EXPECT_EQ(InternalType, i->type);

            ++i;
            ASSERT_NE(i, w.end());
            EXPECT_EQ(c, i->symbol);
            EXPECT_EQ(ReturnType, i->type);

            ++i;
            ASSERT_NE(i, w.end());
            EXPECT_EQ(a, i->symbol);
            EXPECT_EQ(CallType, i->type);

            ++i;
            ASSERT_NE(i, w.end());
            EXPECT_EQ(b, i->symbol);
            EXPECT_EQ(InternalType, i->type);

            ++i;
            ASSERT_NE(i, w.end());
            EXPECT_EQ(c, i->symbol);
            EXPECT_EQ(ReturnType, i->type);

            ++i;
            ASSERT_NE(i, w.end());
            EXPECT_EQ(a, i->symbol);
            EXPECT_EQ(CallType, i->type);

            ++i;
            EXPECT_EQ(i, w.end());
        }

        TEST(opennwa$NestedWord$$operatorEquals, equalWordsEqual)
        {
            NestedWord word1, word2;
            SomeElements e;
            
            EXPECT_EQ(word1, word2);

            word1.appendInternal(e.symbol);
            word2.appendInternal(e.symbol);

            EXPECT_EQ(word1, word2);

            word1.appendCall(e.symbol);
            word2.appendCall(e.symbol);

            EXPECT_EQ(word1, word2);

            word1.appendReturn(e.symbol);
            word2.appendReturn(e.symbol);

            EXPECT_EQ(word1, word2);
        }

        TEST(opennwa$NestedWord$$operatorEquals, differentLengthsUnequal)
        {
            NestedWord word1, word2;
            SomeElements e;
            word1.appendInternal(e.symbol);

            EXPECT_NE(word1, word2);
            EXPECT_NE(word2, word1);
        }

        TEST(opennwa$NestedWord$$operatorEquals, differentLettersUnequal)
        {
            NestedWord word1, word2;
            SomeElements e;

            word1.appendInternal(e.state);
            word2.appendInternal(e.symbol);

            EXPECT_NE(word1, word2);
            EXPECT_NE(word2, word1);
        }

        TEST(opennwa$NestedWord$$operatorEquals, differentKindsUnequal)
        {
            NestedWord word1, word2, word3, word4, word5, word6;
            SomeElements e;

            // internal-call
            word1.appendInternal(e.symbol);
            word2.appendCall(e.symbol);

            EXPECT_NE(word1, word2);
            EXPECT_NE(word2, word1);

            // internal-return
            word3.appendInternal(e.symbol);
            word4.appendReturn(e.symbol);

            EXPECT_NE(word3, word4);
            EXPECT_NE(word4, word3);

            // call-return
            word5.appendCall(e.symbol);
            word6.appendReturn(e.symbol);

            EXPECT_NE(word5, word6);
            EXPECT_NE(word6, word5);
        }

}

    
