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
      binrel_manager_t brm;
      virtual void SetUp(){
        addBoolVar(voc,"a");
        addBoolVar(voc,"b");
        addBoolVar(voc,"c");
        addBoolVar(voc,"d");

        addIntVar(voc,"p",4);
        addIntVar(voc,"q",4);
        addIntVar(voc,"r",4);
        addIntVar(voc,"s",4);

        brm = new BinRelManager(voc);
        ASSERT_EQ(voc.size(), 8);
      }
  };




  // ////////////////////////INIT TESTS////////////////////////////////////
  //We need separate initialization tests becaues the SetUp function in the
  //fixture already initializes.
  TEST(BinRelInitTest, defaultInitEmptyVoc){
    Voc voc;
    binrel_manager_t brm = new BinRelManager(voc);
    //ASSERT_EQ(voc.size(), 0);
    //binRelDone();
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
    binrel_manager_t brm = new BinRelManager(voc);
    //ASSERT_GT(retVoc.size(),0);
  }


  TEST(BinRelInitTest, multiInitEmptyVoc){
    {
    Voc voc;
    addBoolVar(voc,"a");
    addIntVar(voc,"b",32);
    binrel_manager_t brm  = new BinRelManager(voc);
    //EXPECT_EQ(ret1.size(),2);
    }

    {
    Voc voc;
    addBoolVar(voc,"a");
    addIntVar(voc,"b",32);
    addBoolVar(voc,"c");
    addIntVar(voc,"d",32);
    binrel_manager_t brm = new BinRelManager(voc,10000,20000);
    //EXPECT_EQ(ret2.size(),4);
    }
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
    brm->print(ss);
    
    // Uncomment if you want to log the output.
    LOG(INFO) << ss.str();
  }

  TEST_F(BinRelManagerTest, valueConstructors){
    std::stringstream ss;
    ss << "True(): " << fddset << brm->True() << std::endl;
    ss << "False(): " << fddset << brm->False() << std::endl;
    ss << "Const(5): " << fddset << brm->Const(5) << std::endl;
    ASSERT_TRUE(compareOutput("BinRelManagerTest","valueconstructors",ss));
  }

  TEST_F(BinRelManagerTest, from){
    std::stringstream ss;
    bdd a = brm->From("a");
    bdd b = brm->From("b");
    bdd c = brm->From("c");
    bdd d;
    bdd p = brm->From("p");
    bdd q = brm->From("q");
    bdd r = brm->From("r");
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
    bdd a = brm->From("a");
    bdd b = brm->From("b");
    bdd c = brm->From("c");
    bdd d;

    d = brm->And(a,b);
    ss << "\nAnd(a,b): " << fddset << d;
    d = brm->And(b,a);
    ss << "\nAnd(b,a): " << fddset << d;
    d = brm->And(b,c);
    ss << "\nAnd(b,c): " << fddset << d;
    d = brm->Or(a,b);
    ss << "\nOr(a,b): " << fddset << d;
    d = brm->Or(b,a);
    ss << "\nOr(b,a): " << fddset << d;
    d = brm->Or(b,c);
    ss << "\nOr(b,c): " << fddset << d;
    d = brm->Not(a);
    ss << "\nNot(a): " << fddset << d;
    d = brm->Not(b);
    ss << "\nNot(b): " << fddset << d;
    ASSERT_TRUE(compareOutput("BinRelManagerTest","boolOps",ss));
  }

  TEST_F(BinRelManagerTest, intOps){
    std::stringstream ss;
    bdd p = brm->From("p");
    bdd q = brm->From("q");
    bdd r = brm->From("r");
    bdd s;

    s = brm->Plus(p,q);
    ss << "\nPlus(p,q): " << fddset << s;
    s = brm->Plus(q,p);
    ss << "\nPlus(q,p): " << fddset << s;
    s = brm->Plus(q,r);
    ss << "\nPlus(q,r): " << fddset << s;
    s = brm->Minus(p,q);
    ss << "\nMinus(p,q): " << fddset << s;
    s = brm->Minus(q,p);
    ss << "\nMinus(q,p): " << fddset << s;
    s = brm->Minus(q,r);
    ss << "\nMinus(q,r): " << fddset << s;

    s = brm->Times(p,q);
    ss << "\nTimes(p,q): " << fddset << s;
    s = brm->Times(q,p);
    ss << "\nTimes(q,p): " << fddset << s;
    s = brm->Times(q,r);
    ss << "\nTimes(q,r): " << fddset << s;
    s = brm->Div(p,q);
    ss << "\nDiv(p,q): " << fddset << s;
    s = brm->Div(q,p);
    ss << "\nDiv(q,p): " << fddset << s;
    s = brm->Div(q,r);
    ss << "\nDiv(q,r): " << fddset << s;
    ASSERT_TRUE(compareOutput("BinRelManagerTest","intOps",ss));
  }

  TEST_F(BinRelManagerTest, assignNassumeBool){
    std::stringstream ss;

    Voc initVoc;
    addBoolVar(initVoc,"a");
    addBoolVar(initVoc,"b");
    addBoolVar(initVoc,"c");
    brm = NULL;
    brm = new BinRelManager(initVoc);
    bdd a = brm->From("a");
    bdd b = brm->From("b");
    bdd c = brm->From("c");
    bdd d;
    ss << "\nAssign(d,True()): " << fddset << brm->Assign("d",brm->True());
    ss << "\nAssign(b,True()): " << fddset << brm->Assign("b",brm->True());
    ss << "\nAssume(a,b): " << fddset << brm->Assume(a,b);
    d = brm->And(b,a);
    ss << "\nAssign(a,And(b,a))" << fddset << brm->Assign("a",d);
    ss << "\nAssign(a,NonDet())" << fddset << brm->Assign("a",brm->NonDet());

    ASSERT_TRUE(compareOutput("BinRelManagerTest","assignNassumeBool",ss));
  }

  TEST_F(BinRelManagerTest, assignNassumeInt){
    std::stringstream ss;

    Voc initVoc;
    addIntVar(initVoc,"p",4);
    addIntVar(initVoc,"q",4);
    addIntVar(initVoc,"r",4);
    brm = NULL;
    brm = new BinRelManager(initVoc);
    bdd p = brm->From("p");
    bdd q = brm->From("q");
    bdd r = brm->From("r");
    bdd s;
    ss << "\nAssign(r,3): " << fddset << brm->Assign("r",brm->Const(3));
    ss << "\nAssign(r,10): " << fddset << brm->Assign("r",brm->Const(10));
    s = brm->Div(q,r);
    ss << "\nAssign(q,Div(q,r))" << fddset << brm->Assign("q",s);
    ss << "\nAssume(q,r)" << fddset << brm->Assume(q,r);
    ss << "\nAssign(q,NonDet())" << fddset << brm->Assign("q",brm->NonDet());
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
    brm = NULL;
    brm = new BinRelManager(initVoc);

    bdd a = brm->From("a");
    bdd b = brm->From("b");
    bdd c = brm->From("c");
    bdd d;

    bdd p = brm->From("p");
    bdd q = brm->From("q");
    bdd r = brm->From("r");
    bdd s;

    EXPECT_EQ(brm->And(brm->True(),brm->True()), brm->True());
    EXPECT_EQ(brm->Plus(brm->Const(5),brm->Const(5)),brm->Const(10));
    EXPECT_EQ(brm->Times(brm->Const(3),brm->Const(3)),brm->Plus(brm->Const(6),brm->Const(3)));
    EXPECT_EQ(brm->Minus(brm->Const(19),brm->Const(1)),brm->Times(brm->Const(6),brm->Const(3)));
    EXPECT_EQ(brm->Div(brm->Const(15),brm->Const(5)),brm->Const(3));

    s = brm->Assign("s",brm->Plus(brm->Const(5),brm->Const(5)));
    EXPECT_EQ(s,brm->Assign("s", brm->Const(10)));
    EXPECT_NE(s,brm->Assign("s", brm->Const(11)));
  }

  TEST(BinRelManagerCornCases, veryFewVars){
    {
      Voc voc;
      binrel_manager_t brm = new BinRelManager(voc);
      bdd a = brm->True();
      bdd b = brm->Assume(brm->True(), brm->True());
      bdd c = brm->Assume(brm->False(), brm->False());
      ASSERT_EQ(b,c);
    }
    {
      Voc voc;
      addBoolVar(voc, "a");
      binrel_manager_t brm = new BinRelManager(voc);
      ASSERT_EQ(
          brm->Assume(brm->True(),brm->True()),
          brm->Assume(brm->From("a"),brm->From("a")));
    } 
  }



}//namespace
