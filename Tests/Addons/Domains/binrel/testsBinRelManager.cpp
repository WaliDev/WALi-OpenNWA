#include "gtest/gtest.h"
#include "glog/logging.h"
#include "buddy/bdd.h"

// ::std
#include <vector>
#include <utility>
#include <limits>
#include <sstream>

// ::wali::domains::binrel
#include "wali/domains/binrel/BinRelManager.hpp"

using namespace std;
using namespace wali::domains::binrel;
using namespace google;

#include "Common.cpp"
namespace{

  class BinRelManagerTest: public ::testing::Test{
    protected:
      Voc voc;
      virtual void SetUp(){
        Voc initVoc;
        addBoolVar(initVoc,"a");
        addBoolVar(initVoc,"b");
        addBoolVar(initVoc,"c");
        addBoolVar(initVoc,"d");

        addIntVar(initVoc,"p",4);
        addIntVar(initVoc,"q",4);
        addIntVar(initVoc,"r",4);
        addIntVar(initVoc,"s",4);

        voc = initialize(0,0,initVoc);
        ASSERT_EQ(voc.size(), 8);
      }
  };




  // ////////////////////////INIT TESTS////////////////////////////////////
  //We need separate initialization tests becaues the SetUp function in the
  //fixture already initializes.
  TEST(BinRelInitTest, defaultInitEmptyVoc){
    Voc voc;
    Voc retVoc = initialize(0,0,voc);
    ASSERT_EQ(retVoc.size(), 0);
    binRelDone();
  }
  
  TEST(BinRelInitTest, userInit){
    Voc voc;
    addBoolVar(voc,"a");
    addBoolVar(voc,"b");
    addBoolVar(voc,"c");
    addBoolVar(voc,"d");
    addBoolVar(voc,"e");
    addBoolVar(voc,"f");

    addIntVar(voc,"p",32);
    addIntVar(voc,"q",32);
    addIntVar(voc,"r",32);
    addIntVar(voc,"s",32);
    addIntVar(voc,"t",32);
    addIntVar(voc,"u",32);
    addIntVar(voc,"v",32);
    const Voc retVoc = initialize(0,0,voc);
    ASSERT_GT(retVoc.size(),0);
  }


  TEST(BinRelInitTest, multiInitEmptyVoc){
    Voc voc;
    addBoolVar(voc,"a");
    addIntVar(voc,"b",32);
    const Voc ret1  = initialize(0,0,voc);
    EXPECT_EQ(ret1.size(),2);

    Voc voc2;
    addBoolVar(voc,"a");
    addIntVar(voc,"b",32);
    addBoolVar(voc,"c");
    addIntVar(voc,"d",32);
    const Voc ret2 = initialize(10000,20000,voc);
    EXPECT_EQ(ret2.size(),4);
  }


  // /////////////////////////////////////////////
  // BinRelManagerTest Fixture
  // ////////////////////////////////////////////
  TEST_F(BinRelManagerTest, empty){
    std::stringstream ss;
    //yippe SetUp works!
    ASSERT_TRUE(true);
    for(Voc::const_iterator iter = voc.begin();
        iter != voc.end();
        ++iter)
      iter->second->print(ss << iter->first);
    dumpManagerData(ss);
    
    // Uncomment if you want to log the output.
    LOG(INFO) << ss.str();
  }

  TEST_F(BinRelManagerTest, valueConstructors){
    std::stringstream ss;
    ss << "True(): " << fddset << True() << std::endl;
    ss << "False(): " << fddset << False() << std::endl;
    ss << "Const(5): " << fddset << Const(5) << std::endl;
    ASSERT_TRUE(compareOutput("BinRelManagerTest","valueconstructors",ss));
  }

  TEST_F(BinRelManagerTest, from){
    std::stringstream ss;
    bdd a = From("a");
    bdd b = From("b");
    bdd c = From("c");
    bdd d;
    bdd p = From("p");
    bdd q = From("q");
    bdd r = From("r");
    bdd s;

    ss << "\nFrom(a): " << a;
    ss << "\nFrom(b): " << fddset << b;
    ss << "\nFrom(c): " << fddset << c;
    ss << "\nFrom(p): " << fddset << p;
    ss << "\nFrom(q): " << fddset << q;
    ss << "\nFrom(r): " << fddset << r;

    ASSERT_TRUE(compareOutput("BinRelManagerTest","from",ss));
  }

  TEST_F(BinRelManagerTest, boolOps){
    std::stringstream ss;
    bdd a = From("a");
    bdd b = From("b");
    bdd c = From("c");
    bdd d;

    d = And(a,b);
    ss << "\nAnd(a,b): " << fddset << d;
    d = And(b,a);
    ss << "\nAnd(b,a): " << fddset << d;
    d = And(b,c);
    ss << "\nAnd(b,c): " << fddset << d;
    d = Or(a,b);
    ss << "\nOr(a,b): " << fddset << d;
    d = Or(b,a);
    ss << "\nOr(b,a): " << fddset << d;
    d = Or(b,c);
    ss << "\nOr(b,c): " << fddset << d;
    d = Not(a);
    ss << "\nNot(a): " << fddset << d;
    d = Not(b);
    ss << "\nNot(b): " << fddset << d;
    ASSERT_TRUE(compareOutput("BinRelManagerTest","boolOps",ss));
  }

  TEST_F(BinRelManagerTest, intOps){
    std::stringstream ss;
    bdd p = From("p");
    bdd q = From("q");
    bdd r = From("r");
    bdd s;

    s = Plus(p,q);
    ss << "\nPlus(p,q): " << fddset << s;
    s = Plus(q,p);
    ss << "\nPlus(q,p): " << fddset << s;
    s = Plus(q,r);
    ss << "\nPlus(q,r): " << fddset << s;
    s = Minus(p,q);
    ss << "\nMinus(p,q): " << fddset << s;
    s = Minus(q,p);
    ss << "\nMinus(q,p): " << fddset << s;
    s = Minus(q,r);
    ss << "\nMinus(q,r): " << fddset << s;

    s = Times(p,q);
    ss << "\nTimes(p,q): " << fddset << s;
    s = Times(q,p);
    ss << "\nTimes(q,p): " << fddset << s;
    s = Times(q,r);
    ss << "\nTimes(q,r): " << fddset << s;
    s = Div(p,q);
    ss << "\nDiv(p,q): " << fddset << s;
    s = Div(q,p);
    ss << "\nDiv(q,p): " << fddset << s;
    s = Div(q,r);
    ss << "\nDiv(q,r): " << fddset << s;
    ASSERT_TRUE(compareOutput("BinRelManagerTest","intOps",ss));
  }

  TEST_F(BinRelManagerTest, assignNassumeBool){
    std::stringstream ss;

    Voc initVoc;
    addBoolVar(initVoc,"a");
    addBoolVar(initVoc,"b");
    addBoolVar(initVoc,"c");
    voc = initialize(0,0,initVoc);
    ASSERT_EQ(voc.size(), 3);
    bdd a = From("a");
    bdd b = From("b");
    bdd c = From("c");
    bdd d;
    ss << "\nAssign(d,True()): " << fddset << Assign("d",True());
    ss << "\nAssign(b,True()): " << fddset << Assign("b",True());
    ss << "\nAssume(a,b): " << fddset << Assume(a,b);
    d = And(b,a);
    ss << "\nAssign(a,And(b,a))" << fddset << Assign("a",d);
    ss << "\nAssign(a,NonDet())" << fddset << Assign("a",NonDet());

    ASSERT_TRUE(compareOutput("BinRelManagerTest","assignNassumeBool",ss));
  }

  TEST_F(BinRelManagerTest, assignNassumeInt){
    std::stringstream ss;

    Voc initVoc;
    addIntVar(initVoc,"p",4);
    addIntVar(initVoc,"q",4);
    addIntVar(initVoc,"r",4);
    voc = initialize(0,0,initVoc);
    ASSERT_EQ(voc.size(), 3);
    bdd p = From("p");
    bdd q = From("q");
    bdd r = From("r");
    bdd s;
    ss << "\nAssign(r,3): " << fddset << Assign("r",Const(3));
    ss << "\nAssign(r,10): " << fddset << Assign("r",Const(10));
    s = Div(q,r);
    ss << "\nAssign(q,Div(q,r))" << fddset << Assign("q",s);
    ss << "\nAssume(q,r)" << fddset << Assume(q,r);
    ss << "\nAssign(q,NonDet())" << fddset << Assign("q",NonDet());
    ASSERT_TRUE(compareOutput("BinRelManagerTest","assignNassumeInt",ss));
  }

  TEST_F(BinRelManagerTest, logical){
    Voc initVoc;
    addBoolVar(initVoc,"a");
    addBoolVar(initVoc,"b");
    addBoolVar(initVoc,"c");
    addBoolVar(initVoc,"d");

    addIntVar(initVoc,"p",20);
    addIntVar(initVoc,"q",20);
    addIntVar(initVoc,"r",20);
    addIntVar(initVoc,"s",20);

    voc = initialize(0,0,initVoc);
    ASSERT_EQ(voc.size(), 8);


    bdd a = From("a");
    bdd b = From("b");
    bdd c = From("c");
    bdd d;

    bdd p = From("p");
    bdd q = From("q");
    bdd r = From("r");
    bdd s;

    EXPECT_EQ(And(True(),True()), True());
    EXPECT_EQ(Plus(Const(5),Const(5)),Const(10));
    EXPECT_EQ(Times(Const(3),Const(3)),Plus(Const(6),Const(3)));
    EXPECT_EQ(Minus(Const(19),Const(1)),Times(Const(6),Const(3)));
    EXPECT_EQ(Div(Const(15),Const(5)),Const(3));

    s = Assign("s",Plus(Const(5),Const(5)));
    EXPECT_EQ(s,Assign("s", Const(10)));
    EXPECT_NE(s,Assign("s", Const(11)));
  }




}//namespace
