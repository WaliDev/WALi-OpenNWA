#include "gtest/gtest.h"

#include "wali/nwa/NestedWord.hpp"


namespace wali {
    namespace nwa {

        typedef NestedWord::Position Position;
        typedef NestedWord::const_iterator Iterator;
        
        Position::Type InternalType = Position::InternalType;
        Position::Type CallType = Position::CallType;
        Position::Type ReturnType = Position::ReturnType;


        TEST(wali$nwa$NestedWord, newNestedWordIsEmpty)
        {
            NestedWord w;

            EXPECT_EQ(w.begin(), w.end());
        }


        TEST(wali$nwa$NestedWord, oneCharacterWords)
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


        TEST(wali$nwa$NestedWord, longerWord)
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
    }
}

    
