#include "gtest/gtest.h"
#include "glog/logging.h"
#include "buddy/bdd.h"

// ::std
#include <vector>
#include <utility>
#include <limits>
#include <sstream>

// ::wali::domains::binrel
#include "wali/domains/binrel/ProgramBddContext.hpp"

using namespace std;
using namespace wali::domains::binrel;
using namespace google;

#include "Common.cpp"
namespace{

  class ProgramBddContextTest: public ::testing::Test{
    protected:
      program_bdd_context_t brm;
      virtual void SetUp(){
        brm = new ProgramBddContext();
        brm->addBoolVar("a");
        brm->addBoolVar("b");
        brm->addBoolVar("c");
        brm->addBoolVar("d");

        brm->addIntVar("p",4);
        brm->addIntVar("q",4);
        brm->addIntVar("r",4);
        brm->addIntVar("s",4);
        ASSERT_EQ(brm->size(), 8);
      }
  };




  // ////////////////////////INIT TESTS////////////////////////////////////
  //We need separate initialization tests becaues the SetUp function in the
  //fixture already initializes.
  TEST(BinRelInitTest, defaultInitEmptyBddContext){
    program_bdd_context_t brm = new ProgramBddContext();
  }
  
  TEST(BinRelInitTest, userInit){
    program_bdd_context_t brm = new ProgramBddContext();
    brm->addBoolVar("a");
    brm->addBoolVar("b");
    brm->addBoolVar("c");
    brm->addBoolVar("d");
    brm->addBoolVar("e");
    brm->addBoolVar("f");

    brm->addIntVar("p",32);
    brm->addIntVar("q",32);
    brm->addIntVar("r",32);
    brm->addIntVar("s",32);
    brm->addIntVar("t",32);
    brm->addIntVar("u",32);
    brm->addIntVar("v",32);
    //ASSERT_GT(retBddContext.size(),0);
  }


  TEST(BinRelInitTest, multiInitEmptyBddContext){
    {
    program_bdd_context_t brm  = new ProgramBddContext();
    brm->addBoolVar("a");
    brm->addIntVar("b",32);
    EXPECT_EQ(brm->size(),2);
    }

    {
    program_bdd_context_t brm = new ProgramBddContext(10000,20000);
    brm->addBoolVar("a");
    brm->addIntVar("b",32);
    brm->addBoolVar("c");
    brm->addIntVar("d",32);
    EXPECT_EQ(brm->size(),4);
    }
  }


  // /////////////////////////////////////////////
  // ProgramBddContextTest Fixture
  // ////////////////////////////////////////////
  TEST_F(ProgramBddContextTest, empty){
    std::stringstream ss;
    //yippe SetUp works!
    ASSERT_TRUE(true);
    for(ProgramBddContext::const_iterator iter = brm->begin();
        iter != brm->end();
        ++iter)
      iter->second->print(ss << iter->first);
    brm->print(ss); 
    // Uncomment if you want to log the output.
    LOG(INFO) << ss.str();
  }

  TEST_F(ProgramBddContextTest, valueConstructors){
    std::stringstream ss;
    ss << "True(): " << fddset << brm->True() << std::endl;
    ss << "False(): " << fddset << brm->False() << std::endl;
    ss << "Const(5): " << fddset << brm->Const(5) << std::endl;
    ASSERT_TRUE(compareOutput("ProgramBddContextTest","valueconstructors",ss));
  }

  TEST_F(ProgramBddContextTest, DISABLED_from){
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

    ASSERT_TRUE(compareOutput("ProgramBddContextTest","from",ss));
  }

  TEST_F(ProgramBddContextTest, boolOps){
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
    ASSERT_TRUE(compareOutput("ProgramBddContextTest","boolOps",ss));
  }

  TEST_F(ProgramBddContextTest, intOps){
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
    ASSERT_TRUE(compareOutput("ProgramBddContextTest","intOps",ss));
  }

  TEST_F(ProgramBddContextTest, assignNassumeBool){
    std::stringstream ss;

    brm = new ProgramBddContext();
    brm->addBoolVar("a");
    brm->addBoolVar("b");
    brm->addBoolVar("c");
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

    ASSERT_TRUE(compareOutput("ProgramBddContextTest","assignNassumeBool",ss));
  }

  TEST_F(ProgramBddContextTest, assignNassumeInt){
    std::stringstream ss;

    brm = new ProgramBddContext();
    brm->addIntVar("p",4);
    brm->addIntVar("q",4);
    brm->addIntVar("r",4);
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
    ASSERT_TRUE(compareOutput("ProgramBddContextTest","assignNassumeInt",ss));
  }

  TEST_F(ProgramBddContextTest, logical){
    brm = new ProgramBddContext();
    brm->addBoolVar("a");
    brm->addBoolVar("b");
    brm->addBoolVar("c");
    brm->addBoolVar("d");
    brm->addIntVar("p",20);
    brm->addIntVar("q",20);
    brm->addIntVar("r",20);
    brm->addIntVar("s",20);

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

  TEST(ProgramBddContextCornCases, DISABLED_veryFewVars){
    {
      program_bdd_context_t brm = new ProgramBddContext();
      bdd a = brm->True();
      bdd b = brm->Assume(brm->True(), brm->True());
      bdd c = brm->Assume(brm->False(), brm->False());
      ASSERT_EQ(b,c);
    }
    {
      program_bdd_context_t brm = new ProgramBddContext();
      brm->addBoolVar( "a");
      ASSERT_EQ(
          brm->Assume(brm->True(),brm->True()),
          brm->Assume(brm->From("a"),brm->From("a")));
    } 
  }



}//namespace
