#include "gtest/gtest.h"
#include "buddy/bdd.h"

// ::std
#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <sstream>
#include <string>
#include <cstdlib>
#include <map>

#include <boost/static_assert.hpp>

// ::wali::domains::binrel
#include "wali/domains/binrel/BinRel.hpp"
#include "wali/domains/binrel/ProgramBddContext.hpp"


using namespace std;
using namespace wali;
using namespace wali::domains::binrel;
using namespace wali::domains::binrel::details;

#include "Common.cpp"

namespace{

  struct RawBuddyContext
  {
    RawBuddyContext(int numvars = 4) {
      assert(!bdd_isrunning());
      int err = bdd_init(10000,1000);
      err |= bdd_setvarnum(numvars);
      assert(!err);
    }

    ~RawBuddyContext() {
      bdd_done();
    }
  };


  struct SubsetsOfFour
  {
    bdd b0, b1, b2, b3, b01, b02, b03, b12, b13,
      b23, b012, b013, b023, b123, b0123;
    
    SubsetsOfFour()
    {
      assert(bdd_isrunning());
      if (bdd_varnum() < 2) {
        int err = bdd_setvarnum(2);
        assert(!err);
      }

      bdd zero  = bdd_nithvar(0) & bdd_nithvar(1);
      bdd one   = bdd_nithvar(0) & bdd_ithvar(1);
      bdd two   = bdd_ithvar(0) & bdd_nithvar(1);
      bdd three = bdd_ithvar(0) & bdd_ithvar(1);

      b0 = zero;
      b1 = one;
      b2 = two;
      b3 = three;

      b01 = zero | one;
      b02 = zero | two;
      b03 = zero | three;
      b12 = one | two;
      b13 = one | three;
      b23 = two | three;

      b012 = b01 | two;
      b013 = b01 | three;
      b023 = b02 | three;
      b123 = b12 | three;

      b0123 = b012 | three;
    }
  };
  
  

  TEST(BinRelSelfTest, copyTest){
    ProgramBddContext ctx;
    ctx.addBoolVar("a");

    ProgramBddContext ctx2 = ctx;
  }

  TEST(BinRelSelfTest, repopulateCacheOnCopy){
      ProgramBddContext ctx;
      ctx.addBoolVar("a");

      ProgramBddContext ctx2 = ctx;
      sem_elem_t b = new BinRel(&ctx2, bddtrue);

      // This should just not fail an exception is what I'm checking.
      b->one()->extend(b);
  }

  TEST(BinRelSelfTest, repopulateCacheOnAssign){
      ProgramBddContext ctx;
      ctx.addBoolVar("a");

      ProgramBddContext ctx2;
      ctx2 = ctx;
      sem_elem_t b = new BinRel(&ctx2, bddtrue);

      // This should just not fail an exception is what I'm checking.
      b->one()->extend(b);
  }


  TEST(BinRelSelfTest, creationTest){
    program_bdd_context_t brm = new ProgramBddContext();
    brm->addBoolVar("a");
    brm->addBoolVar("b");
    brm->addBoolVar("c");
    brm->addBoolVar("d");
    brm->addIntVar("p",4);
    brm->addIntVar("q",4);
    brm->addIntVar("r",4);
    brm->addIntVar("s",4);
    ASSERT_EQ(brm->size(), 8u);

    bdd a = brm->From("a");
    bdd p = brm->From("p");

    sem_elem_t se1 = new BinRel(brm.get_ptr(),a,false);
    sem_elem_t se2 = new BinRel(brm.get_ptr(),p,false);

    stringstream ss;
    se1->print(ss << "se1: ") << endl;
    se2->print(ss << "se2: ") << endl;
    ASSERT_TRUE(compareOutput("BinRelSelfTest","creationTest",ss));
  }

  TEST(BinRelSelfTest, selfTest){
    program_bdd_context_t brm = new ProgramBddContext();
    brm->addBoolVar("a");
    brm->addBoolVar("b");
    brm->addIntVar("p",4);

    bdd a = brm->From("a");
    a = brm->Assign("b",a);
    sem_elem_t se1 = new BinRel(brm.get_ptr(),a,false);
    wali::test_semelem_impl(se1);
  }


  class BinRelTestBool: public ::testing::Test{
    protected:
      virtual void SetUp(){
        brm = new ProgramBddContext();
        brm->addBoolVar("a");
        brm->addBoolVar("b");
        brm->addBoolVar("c");
        //ASSERT_EQ(voc.size(), 3);
      }
    protected:
      program_bdd_context_t brm;
  };

  TEST_F(BinRelTestBool,constantTests){
    stringstream ss;
    sem_elem_t se = new BinRel(brm.get_ptr(),brm->False(),false);
    se->one()->print(ss << "one(): ") << endl;
    se->zero()->print(ss << "zero(): ") << endl;
    EXPECT_TRUE(compareOutput("BinRelTestBool","constantTests",ss));
  }

  TEST_F(BinRelTestBool,extendTests){
    stringstream ss;
    bdd a;
    a = brm->Assign("c", brm->And(brm->From("a"),brm->From("b")));
    sem_elem_t se1 = new BinRel(brm.get_ptr(),a,false);
    a = brm->Assume(brm->From("a"), brm->From("b"));
    sem_elem_t se2 = new BinRel(brm.get_ptr(),a,false);
    sem_elem_t one = se1->one();
    sem_elem_t zero = se1->zero();

    EXPECT_TRUE(zero->equal(zero->extend(se1)));
    EXPECT_TRUE(se2->equal(se2->extend(one)));

    se1->print(ss << "se1: ") << endl;
    se2->print(ss << "se2: ") << endl;
    se1->extend(se2)->print(ss << "se1->extend(se2): ") << endl;
    se2->extend(se1)->print(ss << "se2->extend(se1): ") << endl;

    EXPECT_TRUE(compareOutput("BinRelTestBool","extendTests",ss));

    binrel_t be1,be2;
    be1 = dynamic_cast<BinRel*>(se1.get_ptr());
    be2 = dynamic_cast<BinRel*>(se2.get_ptr());

    EXPECT_TRUE(se1->extend(se2.get_ptr())->equal((be1 * be2).get_ptr()));
    EXPECT_TRUE(se2->extend(se1.get_ptr())->equal((be2 * be1).get_ptr()));

    EXPECT_TRUE(compareOutput("BinRelTestBool","extendTests",ss));
  }

  TEST_F(BinRelTestBool,combineTests){
    stringstream ss;
    bdd a;
    a = brm->Assign("c", brm->And(brm->From("a"),brm->From("b")));
    sem_elem_t se1 = new BinRel(brm.get_ptr(),a,false);
    a = brm->Assume(brm->From("a"), brm->From("b"));
    sem_elem_t se2 = new BinRel(brm.get_ptr(),a,false);
    sem_elem_t one = se1->one();
    sem_elem_t zero = se1->zero();

    EXPECT_TRUE(se1->equal(zero->combine(se1)));
    EXPECT_TRUE(one->equal(one->combine(one)));

    se1->print(ss << "se1: ") << endl;
    se2->print(ss << "se2: ") << endl;
    se1->combine(se2)->print(ss << "se1->combine(se2): ") << endl;
    se2->combine(se1)->print(ss << "se2->combine(se1): ") << endl;

    EXPECT_TRUE(compareOutput("BinRelTestBool","combineTests",ss));

    binrel_t be1,be2;
    be1 = dynamic_cast<BinRel*>(se1.get_ptr());
    be2 = dynamic_cast<BinRel*>(se2.get_ptr());
    EXPECT_TRUE(se1->combine(se2.get_ptr())->equal((be1 | be2).get_ptr()));
  }

  TEST_F(BinRelTestBool, transposeTests){
    stringstream ss;
    bdd a;
    a = brm->Assign("c", brm->And(brm->From("a"),brm->From("b")));
    sem_elem_tensor_t se1 = new BinRel(brm.get_ptr(),a,false);
    a = brm->Assume(brm->From("a"), brm->From("b"));
    sem_elem_tensor_t se2 = new BinRel(brm.get_ptr(),a,false);

    se1->print(ss << "se1: ") << endl;
    se1->transpose()->print(ss << "se1: ") << endl;
    se2->print(ss << "se2: ") << endl;
    se2->transpose()->print(ss << "se2: ") << endl;

    EXPECT_TRUE(compareOutput("BinRelTestBool","transposeTests",ss));
  }

  TEST_F(BinRelTestBool, tensorTests){
    brm = new ProgramBddContext();
    brm->addBoolVar("a");
    brm->addBoolVar("b");
    //ASSERT_EQ(voc.size(), 2);

    stringstream ss;
    bdd a;
    a = brm->Assume(brm->From("a"), brm->From("b"));
    sem_elem_tensor_t se1 = new BinRel(brm.get_ptr(),a,false);
    a = brm->Assume(brm->From("a"), brm->Not(brm->From("b")));
    sem_elem_tensor_t se2 = new BinRel(brm.get_ptr(),a,false);
    sem_elem_tensor_t se3 = se1->tensor(se2.get_ptr());

    se1->print(ss << "se1: ") << endl;
    se2->print(ss << "se2: ") << endl;
    se3->print(ss << "se3: ") << endl;

    EXPECT_TRUE(compareOutput("BinRelTestBool","tensorTests",ss));

  }


  TEST_F(BinRelTestBool, detensorTest){
    stringstream ss;
    bdd a;
    a = brm->Assume(brm->From("a"), brm->From("b"));
    sem_elem_tensor_t se1 = new BinRel(brm.get_ptr(),a,false);
    a = brm->Assume(brm->From("b"), brm->From("c"));
    sem_elem_tensor_t se2 = new BinRel(brm.get_ptr(),a,false);
    sem_elem_tensor_t se3 = se1->tensor(se2.get_ptr());
    sem_elem_tensor_t se4 = se3->detensor();
    sem_elem_tensor_t se5 =
      dynamic_cast<SemElemTensor*>(se1->extend(se2.get_ptr()).get_ptr());

    se1->print(ss << "se1: ") << endl;
    se2->print(ss << "se2: ") << endl;
    se3->print(ss << "se3: ") << endl;
    se4->print(ss << "se4: ") << endl;
    se5->print(ss << "se5: ") << endl;

    EXPECT_TRUE(compareOutput("BinRelTestBool","detensorTests",ss));
    EXPECT_TRUE(se5->equal(se4.get_ptr()));
  }

  TEST_F(BinRelTestBool, detensorTransposeTest){
    stringstream ss;
    bdd a;
    a = brm->Assign("a", brm->From("c"));
    sem_elem_tensor_t se1 = new BinRel(brm.get_ptr(),a,false);
    a = brm->Assume(brm->From("b"), brm->From("c"));
    sem_elem_tensor_t se2 = new BinRel(brm.get_ptr(),a,false);
    sem_elem_tensor_t se3 = se1->tensor(se2.get_ptr());
    sem_elem_tensor_t se4 = se3->detensor();
    sem_elem_tensor_t se5 =
      dynamic_cast<SemElemTensor*>(se1->extend(se2.get_ptr()).get_ptr());
    sem_elem_tensor_t se6 = se2->tensor(se1.get_ptr());
    sem_elem_tensor_t se7 = se6->detensorTranspose();
    sem_elem_tensor_t se8 =
      dynamic_cast<SemElemTensor*>(se2->extend(se1.get_ptr()).get_ptr());

    EXPECT_TRUE(se5->equal(se4.get_ptr()));
    EXPECT_TRUE(se8->equal(se7.get_ptr()));
  }

  class BinRelTestInt: public ::testing::Test{
    protected:
      virtual void SetUp(){
        brm = new ProgramBddContext();
        brm->addIntVar("a",4);
        brm->addIntVar("b",4);
        brm->addIntVar("c",4);
        //ASSERT_EQ(voc.size(), 3);
      }
    protected:
      sem_elem_tensor_t screenVar(string var, sem_elem_tensor_t wt);
      program_bdd_context_t brm;
  };

  sem_elem_tensor_t BinRelTestInt::screenVar(string var, sem_elem_tensor_t wt){
    sem_elem_tensor_t screen = new BinRel(brm.get_ptr(),brm->Assume(brm->From(var),brm->Const(0)), false);
    return dynamic_cast<SemElemTensor*>(wt->extend(screen.get_ptr()).get_ptr());
  }

  TEST_F(BinRelTestInt,testScreen){
    stringstream ss;
    sem_elem_tensor_t wt = new BinRel(brm.get_ptr(),brm->Const(0), false);
    wt = dynamic_cast<SemElemTensor*>(wt->one().get_ptr());
    wt->print(ss << "W[1]: ") << endl;
    wt = screenVar("a",wt);
    wt->print(ss << "W[1 & a==0]: ") << endl;
    wt = screenVar("b",wt);
    wt->print(ss << "W[1 && a==0 && b==0]: ") << endl;
    wt = screenVar("c",wt);
    wt->print(ss << "W[1 && a==0 && b==0 && c==0]: ") << endl;
    EXPECT_TRUE(compareOutput("BinRelTestInt","testScreen",ss));
  }

  TEST(BinRelTestRandom, baseDomain){
    // //Maybe this should be done in a more repeat-friendly way// //
    stringstream ss;
    bool failed = false, dump = false;
    program_bdd_context_t brm = new ProgramBddContext();
    brm->addIntVar( "a", 4);
    brm->addBoolVar( "b");
    brm->addIntVar( "c", 4);
    brm->addBoolVar( "d");
    //ASSERT_EQ(voc.size(),4);

    srand((unsigned)time(NULL));
    for(int h=0;h<20;++h){
      bdd r1bdd = brm->tGetRandomTransformer();
      bdd r2bdd = brm->tGetRandomTransformer();
      bdd r3bdd = brm->tGetRandomTransformer();

      sem_elem_t r1 = new BinRel(brm.get_ptr(),r1bdd,false);
      sem_elem_t r2 = new BinRel(brm.get_ptr(),r2bdd,false);
      sem_elem_t r3 = new BinRel(brm.get_ptr(),r3bdd,false);
      sem_elem_t w1,w2,w3,w4,w5;

      w1 = r1->combine(r2);
      w2 = r2->combine(r1);
      if(!w1->equal(w2) || dump){
        ss << "#################################\nr1 | r2 <> r2 | r1\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        w1->print(ss << "r1 | r2: ") << endl;
        w2->print(ss << "r2 | r1: ") << endl;
        failed = true;
      }

      w1 = (r1->extend(r2))->combine(r1->extend(r3));
      w2 = r1->extend(r2->combine(r3));;
      if(!w1->equal(w2) || dump){
        ss << "#################################\nr1 & (r2 | r3) <> r1 & r2 | r1 & r3\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        r3->print(ss << "r3: ") << endl;
        w1->print(ss << "r1 & (r2 | r3): ") << endl;
        w2->print(ss << "r1 & r2 | r1 & r3: ") << endl;
        failed = true;
      }

      w1 = (r1->extend(r3))->combine(r2->extend(r3));
      w2 = (r1->combine(r2))->extend(r3);
      if(!w1->equal(w2) || dump){
        ss << "#################################\n(r1 | r2 ) & r3 <> r1 & r3 | r2 & r3\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        r3->print(ss << "r3: ") << endl;
        w1->print(ss << "(r1 | r2) & r3: ") << endl;
        w2->print(ss << "r1 & r3 | r2 & r3: ") << endl;
        failed = true;
      }
    }

    if(failed){
      cerr << "\n Differences logged in BinRelTestRandom_baseDomain.output\n";
      writeOutput("BinRelTestRandom","baseDomain", ss);
    }
    ASSERT_FALSE(failed);
  }

  TEST(BinRelTestRandom, tensorDomain){
    // //Maybe this should be done in a more repeat-friendly way// //
    stringstream ss;
    bool failed = false;
    bool dump = false;
    program_bdd_context_t brm = new ProgramBddContext();
    brm->addIntVar( "a", 4);
    brm->addBoolVar( "b");
    brm->addIntVar( "c", 4);
    brm->addBoolVar( "d");
    //ASSERT_EQ(voc.size(),4);

    srand((unsigned)time(NULL));
    for(int h=0;h<20;++h){
      bdd r1bdd = brm->tGetRandomTransformer(true);
      bdd r2bdd = brm->tGetRandomTransformer(true);
      bdd r3bdd = brm->tGetRandomTransformer(true);

      sem_elem_t r1 = new BinRel(brm.get_ptr(),r1bdd,true);
      sem_elem_t r2 = new BinRel(brm.get_ptr(),r2bdd,true);
      sem_elem_t r3 = new BinRel(brm.get_ptr(),r3bdd,true);
      sem_elem_t w1,w2,w3,w4,w5;

      w1 = r1->combine(r2);
      w2 = r2->combine(r1);
      if(!w1->equal(w2) || dump){
        ss << "#################################\n[tensor] r1 | r2 <> r2 | r1\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        w1->print(ss << "r1 | r2: ") << endl;
        w2->print(ss << "r2 | r1: ") << endl;
        failed = true;
      }

      w1 = (r1->extend(r2))->combine(r1->extend(r3));
      w2 = r1->extend(r2->combine(r3));;
      if(!w1->equal(w2) || dump){
        ss << "#################################\n[tensor] r1 & (r2 | r3) <> r1 & r2 | r1 & r3\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        r3->print(ss << "r3: ") << endl;
        w1->print(ss << "r1 & (r2 | r3): ") << endl;
        w2->print(ss << "r1 & r2 | r1 & r3: ") << endl;
        failed = true;
      }

      w1 = (r1->extend(r3))->combine(r2->extend(r3));
      w2 = (r1->combine(r2))->extend(r3);
      if(!w1->equal(w2) || dump){
        ss << "#################################\n[tensor] (r1 | r2 ) & r3 <> r1 & r3 | r2 & r3\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        r3->print(ss << "r3: ") << endl;
        w1->print(ss << "(r1 | r2) & r3: ") << endl;
        w2->print(ss << "r1 & r3 | r2 & r3: ") << endl;
        failed = true;
      }
    }

    if(failed){
      cerr << "\n Differences logged in BinRelTestRandom_baseDomain.output\n";
      writeOutput("BinRelTestRandom","tensorDomain", ss);
    }
    ASSERT_FALSE(failed);
  }

  TEST(BinRelTestRandom, baseTensorDomain){
    // //Maybe this should be done in a more repeat-friendly way// //
    stringstream ss;
    bool failed = false;
    bool dump = false;
    program_bdd_context_t brm = new ProgramBddContext();
    brm->addIntVar( "a", 4);
    brm->addBoolVar( "b");
    brm->addIntVar( "c", 4);
    brm->addBoolVar( "d");

    srand(static_cast<unsigned>(time(NULL)));
    //for debugging
    //srand(1);

    for(int h=0;h<20;++h){
      bdd b1bdd = brm->tGetRandomTransformer(false);
      bdd b2bdd = brm->tGetRandomTransformer();
      bdd b3bdd = brm->tGetRandomTransformer();
      bdd b4bdd = brm->tGetRandomTransformer(false);
      bdd t1bdd = brm->tGetRandomTransformer(true);
      bdd t2bdd = brm->tGetRandomTransformer(true);

      sem_elem_tensor_t b1 = new BinRel(brm.get_ptr(),b1bdd,false);
      sem_elem_tensor_t b2 = new BinRel(brm.get_ptr(),b2bdd,false);
      sem_elem_tensor_t b3 = new BinRel(brm.get_ptr(),b3bdd,false);
      sem_elem_tensor_t b4 = new BinRel(brm.get_ptr(),b4bdd,false);
      sem_elem_tensor_t t1 = new BinRel(brm.get_ptr(),t1bdd,true);
      sem_elem_tensor_t t2 = new BinRel(brm.get_ptr(),t2bdd,true);
      sem_elem_tensor_t w1,w2,w3,w4;

      w1 = b1->tensor(b2.get_ptr());
      w2 = b3->tensor(b4.get_ptr());
      w1 = dynamic_cast<SemElemTensor*>(w1->extend(w2.get_ptr()).get_ptr());

      w3 = dynamic_cast<SemElemTensor*>(b1->extend(b3.get_ptr()).get_ptr());
      w4 = dynamic_cast<SemElemTensor*>(b2->extend(b4.get_ptr()).get_ptr());
      w3 = w3->tensor(w4.get_ptr());

      if(!w1->equal(w3) || dump){
        ss << "#################################\n(b1,b2) & (b3,b4) <> (b1 & b3,b2 & b4)\n\n";

        b1->print(ss << "b1: ") << endl;
        b2->print(ss << "b2: ") << endl;
        b3->print(ss << "b3: ") << endl;
        b4->print(ss << "b4: ") << endl;
        w1 = b1->tensor(b2.get_ptr());
        w1->print(ss << "(b1,b2): ") << endl;
        w2 = b3->tensor(b4.get_ptr());
        w2->print(ss << "(b3,b4): ") << endl;
        w1 = dynamic_cast<SemElemTensor*>(w1->extend(w2.get_ptr()).get_ptr());
        w1->print(ss << "(b1,b2) & (b3,b4): ") << endl;


        b1->print(ss << "b1: ") << endl;
        b2->print(ss << "b2: ") << endl;
        b3->print(ss << "b3: ") << endl;
        b4->print(ss << "b4: ") << endl;
        w3 = dynamic_cast<SemElemTensor*>(b1->extend(b3.get_ptr()).get_ptr());
        w3->print(ss << "b1 & b3: ") << endl;
        w4 = dynamic_cast<SemElemTensor*>(b2->extend(b4.get_ptr()).get_ptr());
        w4->print(ss << "b2 & b4: ") << endl;
        w3 = w3->tensor(w4.get_ptr());
        w3->print(ss << "(b1 & b3,b2 & b4): ") << endl;

        failed = true;
      }

      w1 = dynamic_cast<SemElemTensor*>(t1->combine(t2.get_ptr()).get_ptr());
      w1 = w1->detensor();

      w2 = t1->detensor();
      w3 = t2->detensor();
      w2 = dynamic_cast<SemElemTensor*>(w2->combine(w3.get_ptr()).get_ptr());

      if(!w1->equal(w2) || dump){
        ss << "#################################\nD(t1 | t2) <> D(t1) | D(t2)\n\n";
        t1->print(ss << "t1: ") << endl;
        t2->print(ss << "t2: ") << endl;

        w1 = dynamic_cast<SemElemTensor*>(t1->combine(t2.get_ptr()).get_ptr());
        w1->print(ss << "t1 | t2: ") << endl;
        w1 = w1->detensor();
        w1->print(ss << "D(t1 | t2): ") << endl;

        w2 = t1->detensor();
        w2->print(ss << "D(t1): ") << endl;
        w3 = t2->detensor();
        w3->print(ss << "D(t2): ") << endl;
        w2 = dynamic_cast<SemElemTensor*>(w2->combine(w3.get_ptr()).get_ptr());
        w2->print(ss << "D(t1) | D(t2): ") << endl;

        failed = true;
      }

      w1 = b1->tensor(b2.get_ptr());
      w1 = w1->detensor();

      w2 = dynamic_cast<SemElemTensor*>(b1->extend(b2.get_ptr()).get_ptr());

      if(!w1->equal(w2) || dump){
        ss << "#################################\nD((b1, b2)) <> b1 & b2\n\n";
        b1->print(ss << "b1: ") << endl;
        b2->print(ss << "b2: ") << endl;
        w1 = b1->tensor(b2.get_ptr());
        w1->print(ss << "(b1,r2): ") << endl;
        w1 = w1->detensor();
        w1->print(ss << "D((b1,r2)): ") << endl;

        w2 = dynamic_cast<SemElemTensor*>(b1->extend(b2.get_ptr()).get_ptr());
        w2->print(ss << "b1 & b2: ") << endl;

        failed = true;
      }
    }

    if(failed){
      cerr << "\n Differences logged in BinRelTestRandom_baseDomain.output\n";
      writeOutput("BinRelTestRandom","baseTensorDomain", ss);
    }
    ASSERT_FALSE(failed);
  }

  TEST(BinRelTestRandom, tensorDomainSetInt){
    //same as tensorDomain, but uses setIntVars to create the vocabulary.
    // //Maybe this should be done in a more repeat-friendly way// //
    stringstream ss;
    bool failed = false;
    bool dump = false;
    program_bdd_context_t brm = new ProgramBddContext();
    std::map<std::string, int> vars;
    vars["a"] = 4;
    vars["b"] = 2;
    vars["c"] = 4;
    vars["d"] = 2;
    brm->setIntVars(vars);
    //ASSERT_EQ(voc.size(),4);

    srand((unsigned)time(NULL));
    for(int h=0;h<20;++h){
      bdd r1bdd = brm->tGetRandomTransformer(true);
      bdd r2bdd = brm->tGetRandomTransformer(true);
      bdd r3bdd = brm->tGetRandomTransformer(true);

      sem_elem_t r1 = new BinRel(brm.get_ptr(),r1bdd,true);
      sem_elem_t r2 = new BinRel(brm.get_ptr(),r2bdd,true);
      sem_elem_t r3 = new BinRel(brm.get_ptr(),r3bdd,true);
      sem_elem_t w1,w2,w3,w4,w5;

      w1 = r1->combine(r2);
      w2 = r2->combine(r1);
      if(!w1->equal(w2) || dump){
        ss << "#################################\n[tensor] r1 | r2 <> r2 | r1\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        w1->print(ss << "r1 | r2: ") << endl;
        w2->print(ss << "r2 | r1: ") << endl;
        failed = true;
      }

      w1 = (r1->extend(r2))->combine(r1->extend(r3));
      w2 = r1->extend(r2->combine(r3));;
      if(!w1->equal(w2) || dump){
        ss << "#################################\n[tensor] r1 & (r2 | r3) <> r1 & r2 | r1 & r3\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        r3->print(ss << "r3: ") << endl;
        w1->print(ss << "r1 & (r2 | r3): ") << endl;
        w2->print(ss << "r1 & r2 | r1 & r3: ") << endl;
        failed = true;
      }

      w1 = (r1->extend(r3))->combine(r2->extend(r3));
      w2 = (r1->combine(r2))->extend(r3);
      if(!w1->equal(w2) || dump){
        ss << "#################################\n[tensor] (r1 | r2 ) & r3 <> r1 & r3 | r2 & r3\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        r3->print(ss << "r3: ") << endl;
        w1->print(ss << "(r1 | r2) & r3: ") << endl;
        w2->print(ss << "r1 & r3 | r2 & r3: ") << endl;
        failed = true;
      }
    }

    if(failed){
      cerr << "\n Differences logged in BinRelTestRandom_baseDomain.output\n";
      writeOutput("BinRelTestRandom","tensorDomain", ss);
    }
    ASSERT_FALSE(failed);
  }

  TEST(BinRelTestRandom, tensorDomainSetIntTwoSets){
    // Same as tensorDomain, but uses setIntVars to create the vocabulary.
    // Additionally, the variables are grouped into two interleaving sets.
    // //Maybe this should be done in a more repeat-friendly way// //
    stringstream ss;
    bool failed = false;
    bool dump = false;
    program_bdd_context_t brm = new ProgramBddContext();
    std::vector<std::map<std::string, int> > vars;
    std::map<std::string, int> vars1;
    std::map<std::string, int> vars2;
    vars1["a"] = 4;
    vars1["b"] = 2;
    vars2["c"] = 4;
    vars2["d"] = 2;
    vars.push_back(vars1);
    vars.push_back(vars2);
    brm->setIntVars(vars);
    //ASSERT_EQ(voc.size(),4);

    srand((unsigned)time(NULL));
    for(int h=0;h<20;++h){
      bdd r1bdd = brm->tGetRandomTransformer(true);
      bdd r2bdd = brm->tGetRandomTransformer(true);
      bdd r3bdd = brm->tGetRandomTransformer(true);

      sem_elem_t r1 = new BinRel(brm.get_ptr(),r1bdd,true);
      sem_elem_t r2 = new BinRel(brm.get_ptr(),r2bdd,true);
      sem_elem_t r3 = new BinRel(brm.get_ptr(),r3bdd,true);
      sem_elem_t w1,w2,w3,w4,w5;

      w1 = r1->combine(r2);
      w2 = r2->combine(r1);
      if(!w1->equal(w2) || dump){
        ss << "#################################\n[tensor] r1 | r2 <> r2 | r1\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        w1->print(ss << "r1 | r2: ") << endl;
        w2->print(ss << "r2 | r1: ") << endl;
        failed = true;
      }

      w1 = (r1->extend(r2))->combine(r1->extend(r3));
      w2 = r1->extend(r2->combine(r3));;
      if(!w1->equal(w2) || dump){
        ss << "#################################\n[tensor] r1 & (r2 | r3) <> r1 & r2 | r1 & r3\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        r3->print(ss << "r3: ") << endl;
        w1->print(ss << "r1 & (r2 | r3): ") << endl;
        w2->print(ss << "r1 & r2 | r1 & r3: ") << endl;
        failed = true;
      }

      w1 = (r1->extend(r3))->combine(r2->extend(r3));
      w2 = (r1->combine(r2))->extend(r3);
      if(!w1->equal(w2) || dump){
        ss << "#################################\n[tensor] (r1 | r2 ) & r3 <> r1 & r3 | r2 & r3\n\n";
        r1->print(ss << "r1: ") << endl;
        r2->print(ss << "r2: ") << endl;
        r3->print(ss << "r3: ") << endl;
        w1->print(ss << "(r1 | r2) & r3: ") << endl;
        w2->print(ss << "r1 & r3 | r2 & r3: ") << endl;
        failed = true;
      }
    }

    if(failed){
      cerr << "\n Differences logged in BinRelTestRandom_baseDomain.output\n";
      writeOutput("BinRelTestRandom","tensorDomain", ss);
    }
    ASSERT_FALSE(failed);
  }

  
  // The following test just creates some add operations. The point is so
  // that if both the slow and fast adders are turned on, then it will make
  // sure they get the same result.
  TEST(wali$domains$binrel$ProgramBddContext$$bddPlus, slowAndFastAdderAgree)
  {
      for (int i=2; i<10; ++i) {
          ProgramBddContext ctx;
          std::map<std::string, int> vars;
          vars["x"] = i;
          ctx.setIntVars(vars);
          ctx.Plus(ctx.From("x"), ctx.From("x"));
      }
  }

  TEST(wali$domains$binrel$ProgramBddContext$$Assign, canAssignAConstToNotTheBiggestVariable)
  {
      ProgramBddContext ctx;
      std::map<std::string, int> vars;
      vars["x"] = 2;
      vars["y"] = 4;
      ctx.setIntVars(vars);

      // x = 0 should obviously succeed
      bdd assigned = ctx.Assign("x", ctx.Const(0));

      ASSERT_TRUE(assigned != bddfalse);

      // We want to check that there's exactly one satisfying assignment for
      // this (to make sure there's not something strange going
      // on). 'assigned' should be the bdd " x' = 0 " but with some extra
      // stuff tacked on for the auxiliary variables. We will make sure that,
      // if we manually construct that bdd independently, take the
      // complement, and 'and' it with 'assigned', the result is unsat --
      // meaning that every satisfying assignement to 'assigned' has x'=0.
      int x_prime_fdd_num = ctx["x"]->baseRhs;
      bdd x_prime_is_zero = fdd_ithvar(x_prime_fdd_num, 0);

      ASSERT_TRUE(bddfalse == (assigned & !x_prime_is_zero));
  }

#undef SETVARADDVARTESTS
#if defined(SETVARADDVARTESTS)
  /**
   * The following 4 tests try the following configurations:
   *
   * Configuration 1:
   * call 'addIntVar' several times (for variables a-n)
   * construct some weights
   * call 'addIntVar' (for variable w)
   * do some binrel operations
   *
   * Configuration 2:
   * call 'setIntVars' once (for a-n)
   * construct some weights
   * call 'addIntVar' (for variable w)
   * do some binrel operations
   *
   * Configuration 3:
   * call 'setIntVars' once for a-n
   * call 'addIntVar' for w
   * construct some weights
   * do some binrel operations
   *
   * Configuration 4:
   * call 'setIntVars' once for both a-n and w at once
   * construct some weights
   * do some binrel operations
   **/
  TEST(SetVarAddVarTests, one){
    program_bdd_context_t brm = new ProgramBddContext();

    brm->addIntVar("a", 4);
    brm->addIntVar("b", 4);
    brm->addIntVar("c", 4);
    brm->addIntVar("d", 4);
    brm->addIntVar("e", 4);
    brm->addIntVar("f", 4);
    brm->addIntVar("g", 4);
    brm->addIntVar("h", 4);
    brm->addIntVar("i", 4);
    brm->addIntVar("j", 4);
    brm->addIntVar("k", 4);
    brm->addIntVar("l", 4);
    brm->addIntVar("m", 4);
    brm->addIntVar("n", 4);

    binrel_t b1 = new BinRel(brm.get_ptr(), brm->Assign("a", brm->Plus(brm->From("b"), brm->From("c"))));
    binrel_t b2 = new BinRel(brm.get_ptr(), brm->Assign("b", brm->Plus(brm->Const(2), brm->From("d"))));

    brm->addIntVar("w", 15);

    b2 = new BinRel(brm.get_ptr(), brm->Assign("w", brm->Plus(brm->Const(4), brm->From("w"))));
    sem_elem_t b3 = b1->extend(b2.get_ptr());
    b3 = b3->combine(b3);
  }

  TEST(SetVarAddVarTests, two){
    program_bdd_context_t brm = new ProgramBddContext();

    map<string, int> vars;
    vars["a"] = 4;
    vars["b"] = 4;
    vars["c"] = 4;
    vars["d"] = 4;
    vars["e"] = 4;
    vars["f"] = 4;
    vars["g"] = 4;
    vars["h"] = 4;
    vars["i"] = 4;
    vars["j"] = 4;
    vars["k"] = 4;
    vars["l"] = 4;
    vars["m"] = 4;
    vars["n"] = 4;
    brm->setIntVars(vars);

    binrel_t b1 = new BinRel(brm.get_ptr(), brm->Assign("a", brm->Plus(brm->From("b"), brm->From("c"))));
    binrel_t b2 = new BinRel(brm.get_ptr(), brm->Assign("b", brm->Plus(brm->Const(2), brm->From("d"))));

    brm->addIntVar("w", 15);

    b2 = new BinRel(brm.get_ptr(), brm->Assign("w", brm->Plus(brm->Const(4), brm->From("w"))));
    sem_elem_t b3 = b1->extend(b2.get_ptr());
    b3 = b3->combine(b3);
  }

  TEST(SetVarAddVarTests, three){
    program_bdd_context_t brm = new ProgramBddContext();

    map<string, int> vars;
    vars["a"] = 4;
    vars["b"] = 4;
    vars["c"] = 4;
    vars["d"] = 4;
    vars["e"] = 4;
    vars["f"] = 4;
    vars["g"] = 4;
    vars["h"] = 4;
    vars["i"] = 4;
    vars["j"] = 4;
    vars["k"] = 4;
    vars["l"] = 4;
    vars["m"] = 4;
    vars["n"] = 4;
    brm->setIntVars(vars);

    brm->addIntVar("w", 15);

    binrel_t b1 = new BinRel(brm.get_ptr(), brm->Assign("a", brm->Plus(brm->From("b"), brm->From("c"))));
    binrel_t b2 = new BinRel(brm.get_ptr(), brm->Assign("b", brm->Plus(brm->Const(2), brm->From("d"))));


    b2 = new BinRel(brm.get_ptr(), brm->Assign("w", brm->Plus(brm->Const(4), brm->From("w"))));
    sem_elem_t b3 = b1->extend(b2.get_ptr());
    b3 = b3->combine(b3);
  }

  TEST(SetVarAddVarTests, four){
    program_bdd_context_t brm = new ProgramBddContext();

    map<string, int> vars;
    vars["a"] = 4;
    vars["b"] = 4;
    vars["c"] = 4;
    vars["d"] = 4;
    vars["e"] = 4;
    vars["f"] = 4;
    vars["g"] = 4;
    vars["h"] = 4;
    vars["i"] = 4;
    vars["j"] = 4;
    vars["k"] = 4;
    vars["l"] = 4;
    vars["m"] = 4;
    vars["n"] = 4;

    vars["w"] = 15;

    brm->setIntVars(vars);


    binrel_t b1 = new BinRel(brm.get_ptr(), brm->Assign("a", brm->Plus(brm->From("b"), brm->From("c"))));
    binrel_t b2 = new BinRel(brm.get_ptr(), brm->Assign("b", brm->Plus(brm->Const(2), brm->From("d"))));


    b2 = new BinRel(brm.get_ptr(), brm->Assign("w", brm->Plus(brm->Const(4), brm->From("w"))));
    sem_elem_t b3 = b1->extend(b2.get_ptr());
    b3 = b3->combine(b3);
  }
#endif //SETVARADDVARTESTS


#if defined(BINREL_STATS)
  TEST(StatsTests, basicStatCollectionAndPrinting1)
  {
    program_bdd_context_t brm = new ProgramBddContext();
    
    brm->addIntVar("c", 5);
    brm->addIntVar("d", 5);

    sem_elem_t a = new BinRel(brm.get_ptr(), brm->True());
    sem_elem_t b = new BinRel(brm.get_ptr(), brm->Plus(brm->From("c"), brm->From("d")));

    sem_elem_t c;
    c = a->extend(a);
    c = c->extend(c);

    c = c->combine(a);
    c = c->combine(a);


    sem_elem_tensor_t e = dynamic_cast<SemElemTensor*>(a.get_ptr());
    sem_elem_tensor_t f = dynamic_cast<SemElemTensor*>(b.get_ptr());
    sem_elem_tensor_t g,h;

    g = dynamic_cast<SemElemTensor*>(f.get_ptr());
    g = g->transpose();

    g = e->tensor(f.get_ptr());
    h = e->tensor(f.get_ptr());

    g = dynamic_cast<SemElemTensor*>(g->extend(h.get_ptr()).get_ptr());
    g = dynamic_cast<SemElemTensor*>(g->extend(h.get_ptr()).get_ptr());
    g = dynamic_cast<SemElemTensor*>(g->combine(h.get_ptr()).get_ptr());

    c = g->detensor();
    c = h->detensorTranspose();
  
    stringstream ss;

    brm->printStats(ss);
    EXPECT_TRUE(compareOutput("StatTests", "basicStatCollectionAndPrinting1", ss));

  }
#endif

  TEST(Unclassified, dumbTensorTest1)
  {
    program_bdd_context_t brm = new ProgramBddContext();
    brm->addBoolVar("b");

    binrel_t a = new BinRel(brm.get_ptr(), brm->True());
    a = dynamic_cast<BinRel*>(a->one().get_ptr());

    binrel_t b = dynamic_cast<BinRel*>(a->tensor(a.get_ptr()).get_ptr());
    b->print(cout) << std::endl;
    binrel_t wt = dynamic_cast<BinRel*>(b->detensorTranspose().get_ptr());
    if(wt == NULL){
      ASSERT_TRUE(false);
      return;
    }
    cout << wt->count;
    wt->print(cout) << std::endl;
    ASSERT_TRUE(b->detensor()->equal(a));
    ASSERT_TRUE(b->detensorTranspose()->equal(a));

  }

#define STRESS_TEST_CONST1 500
#define STRESS_TEST_CONST2 20000
  TEST(SetVarAddVarTests, DISABLED_stressTest){
    // Try to using setVars vs addVar
    {
      // Using addIntVar
      cout << "Using addIntVar" << endl;
      program_bdd_context_t brm;
      brm = new ProgramBddContext();
      map<string, int> vars;
      for(unsigned i=0; i<STRESS_TEST_CONST1; ++i){
        stringstream ss;
        ss << "var " << i;
        brm->addIntVar(ss.str(), 3);
      }
      cout << "Done adding variables" << endl;
      bdd bdds[STRESS_TEST_CONST1];
      unsigned i = 0;
      for(ProgramBddContext::iterator iter = brm->begin(); iter != brm->end(); ++iter, ++i)
        bdds[i] = brm->Plus(brm->From(iter->first), brm->Const(2));

      sem_elem_t sems[STRESS_TEST_CONST2];
      for(unsigned i=0; i<STRESS_TEST_CONST2; ++i){
        unsigned j = i % 200;
        unsigned k = (int)( i * 4.8) % 200;
        sem_elem_t sem1 = new BinRel(brm.get_ptr(), bdds[j]);
        sem_elem_t sem2 = new BinRel(brm.get_ptr(), bdds[k]);
        sems[i] = sem1->extend(sem2);
      }
    }

    {
      cout << "Using SetIntVar" << endl;
      // Using setIntVar
      program_bdd_context_t brm;
      brm = new ProgramBddContext();
      map<string, int> vars;
      for(unsigned i=0; i<STRESS_TEST_CONST1; ++i){
        stringstream ss;
        ss << "var " << i;
        vars[ss.str()] = 3;
      }
      brm->setIntVars(vars);
      cout << "Done adding variables" << endl;

      bdd bdds[STRESS_TEST_CONST1];
      unsigned i = 0;
      for(ProgramBddContext::iterator iter = brm->begin(); iter != brm->end(); ++iter, ++i)
        bdds[i] = brm->Plus(brm->From(iter->first), brm->Const(2));

      sem_elem_t sems[STRESS_TEST_CONST2];
      for(unsigned i=0; i<STRESS_TEST_CONST2; ++i){
        unsigned j = i % 200;
        unsigned k = (int)( i * 4.8) % 200;
        sem_elem_t sem1 = new BinRel(brm.get_ptr(), bdds[j]);
        sem_elem_t sem2 = new BinRel(brm.get_ptr(), bdds[k]);
        sems[i] = sem1->extend(sem2);
      }
    }
  }

#if defined(TENSOR_MATCHED_PAREN) && defined(NWA_DETENSOR) && 0
  TEST(BddContext, bitLevelInfo)
  {
    program_bdd_context_t con;
    con = new ProgramBddContext();
    map<string, int> vars;
    vars["a"] = 2;
    vars["b"] = 2;
    con->setIntVars(vars);
    
    bddinfo_t a = (*con)["a"];
    bddinfo_t b = (*con)["b"];

    bdd t;

    // Setup Level sets
    con->setupLevelSets();

    /*
     * t looks like:
     * "a".tensor1Lhs          *
     *                       1/ \0
     *              ----------   ---------
     *          bddfalse              bddtrue
     **/
    t = fdd_ithvar(a->tensor1Lhs, 0);
    EXPECT_TRUE(con->isRootInVocTensor1Lhs(t));
    EXPECT_FALSE(con->isRootRelevant(bdd_low(t)));
    EXPECT_FALSE(con->isRootRelevant(bdd_high(t)));
 
    /*
     * t looks like:
     * "a".tensor1Rhs         *
     *                      1/ \0
     *                 ------   \
     * "a".tensor1Lhs  *         \
     *               1/ \         \
     *               /   ----------
     *            bddtrue        bddfalse
     **/
    t = fdd_ithvar(a->tensor1Rhs, 1) & fdd_ithvar(a->tensor1Lhs, 1);
    EXPECT_TRUE(con->isRootInVocTensor1Rhs(t));
    EXPECT_TRUE(con->isRootInVocTensor1Lhs(bdd_high(t)));
    EXPECT_FALSE(con->isRootRelevant(bdd_low(t)));

    /*
     * t looks like:
     * "b".tensor2Lhs         *
     *                      1/ \0
     * "b".tensor2Rhs       /   *
     *                     /  0/ \1
     * "a".tensor2Lhs      |  /   *
     *                     |  | 1/ \0
     * "a".tensor2Rhs      |  |  |  *
     *                     |  |  |0/ \1
     *                  -----------   \
     *                bddfalse       bddtrue
     **/
    t = fdd_ithvar(b->tensor2Lhs, 0) & fdd_ithvar(a->tensor2Lhs, 0) & fdd_ithvar(b->tensor2Rhs, 1) & fdd_ithvar(a->tensor2Rhs, 1);
    EXPECT_TRUE(con->isRootInVocTensor2Lhs(t));
    t = bdd_low(t);
    EXPECT_TRUE(con->isRootInVocTensor2Rhs(t));
    t = bdd_high(t);
    EXPECT_TRUE(con->isRootInVocTensor2Lhs(t));
    t = bdd_low(t);
    EXPECT_TRUE(con->isRootInVocTensor2Rhs(t));
    t = bdd_high(t);
    EXPECT_TRUE(t == bddtrue);

    /*
     * t looks like:
     * "a".tensor1Rhs         *
     *                      1/ \0
     * "a".tensor1Lhs       /   *
     *                     /  0/ \1
     * "b".tensor1Rhs      |  /   *
     *                     |  | 1/ \0
     * "b".tensor1Lhs      |  |  |  *
     *                     |  |  |0/ \1
     *                  -----------   \
     *                bddfalse       bddtrue
     **/
    t = fdd_ithvar(a->tensor1Rhs, 0) & fdd_ithvar(a->tensor1Lhs, 1) & fdd_ithvar(b->tensor1Rhs, 0) & fdd_ithvar(b->tensor1Lhs, 1);
    EXPECT_TRUE(con->isRootInVocTensor1Rhs(t));
    t = bdd_low(t);
    EXPECT_TRUE(con->isRootInVocTensor1Lhs(t));
    t = bdd_high(t);
    EXPECT_TRUE(con->isRootInVocTensor1Rhs(t));
    t = bdd_low(t);
    EXPECT_TRUE(con->isRootInVocTensor1Lhs(t));
    t = bdd_high(t);
    EXPECT_TRUE(t == bddtrue);

  }
#endif //#if defined(TENSOR_MATCHED_PAREN)


  TEST(wali$domains$binrel$details$$bddImplies, trueImpliesTrue)
  {
    RawBuddyContext buddy;
    EXPECT_TRUE(bddImplies(bddtrue, bddtrue));
  }

  TEST(wali$domains$binrel$details$$bddImplies, trueDoesNotSubsumeFalse)
  {
    RawBuddyContext buddy;
    EXPECT_FALSE(bddImplies(bddtrue, bddfalse));
  }

  TEST(wali$domains$binrel$details$$bddImplies, falseImpliesTrue)
  {
    RawBuddyContext buddy;
    EXPECT_TRUE(bddImplies(bddfalse, bddtrue));
  }

  TEST(wali$domains$binrel$details$$bddImplies, falseImpliesFalse)
  {
    RawBuddyContext buddy;
    EXPECT_TRUE(bddImplies(bddfalse, bddfalse));
  }


  TEST(wali$domains$binrel$details$$bddImplies, subsetsOfFourBattery)
  {
    RawBuddyContext buddy(2);
    SubsetsOfFour subsets;

    bdd bdds[] = {
      bddfalse, subsets.b0, subsets.b1, subsets.b2, subsets.b3, subsets.b01, subsets.b02,
      subsets.b03, subsets.b12, subsets.b13, subsets.b23, subsets.b012, subsets.b013,
      subsets.b023, subsets.b123, subsets.b0123
    };

    bool answers[][16] = {
      //            {},    {0}    {1}    {2}    {3}    {01}   {02}   {03}   {12}   {13}   {23}   {012}  {013}  {023}  {123}  {0123}
      /* {}    */ { true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true },

      /* {0}   */ { false, true,  false, false, false, true,  true,  true,  false, false, false, true,  true,  true,  false, true },
      /* {1}   */ { false, false, true,  false, false, true,  false, false, true,  true,  false, true,  true,  false, true,  true },
      /* {2}   */ { false, false, false, true,  false, false, true,  false, true,  false, true,  true,  false, true,  true,  true },
      /* {3}   */ { false, false, false, false, true,  false, false, true,  false, true,  true,  false, true,  true,  true,  true },

      /* {01}  */ { false, false, false, false, false, true,  false, false, false, false, false, true,  true,  false, false, true },
      /* {02}  */ { false, false, false, false, false, false, true,  false, false, false, false, true,  false, true,  false, true },
      /* {03}  */ { false, false, false, false, false, false, false, true,  false, false, false, false, true,  true,  false, true },
      /* {12}  */ { false, false, false, false, false, false, false, false, true,  false, false, true,  false, false, true,  true },
      /* {13}  */ { false, false, false, false, false, false, false, false, false, true,  false, false, true,  false, true,  true },
      /* {23}  */ { false, false, false, false, false, false, false, false, false, false, true,  false, false, true,  true,  true },

      /* {012} */ { false, false, false, false, false, false, false, false, false, false, false, true,  false, false, false, true },
      /* {013} */ { false, false, false, false, false, false, false, false, false, false, false, false, true,  false, false, true },
      /* {023} */ { false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, true },
      /* {123} */ { false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  true },

      /* {0124}*/ { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true }
    };

#define NUM_ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))

    BOOST_STATIC_ASSERT(NUM_ELEMENTS(answers) == NUM_ELEMENTS(bdds));
    BOOST_STATIC_ASSERT(NUM_ELEMENTS(answers[0]) == NUM_ELEMENTS(bdds));

    for (int i=0; i<2; ++i) {
      if (i==1) {
        // reverse the order
        int order[] = {1, 0};
        int old_var_level_0 = bdd_var2level(0);
        ASSERT_EQ(bdd_varnum(), NUM_ELEMENTS(order));
        bdd_setvarorder(order);
        EXPECT_NE(old_var_level_0, bdd_var2level(0));
      }
      
      for (size_t left_no=0; left_no<NUM_ELEMENTS(bdds); ++left_no) {
        for (size_t right_no=0; right_no<NUM_ELEMENTS(bdds); ++right_no) {
          std::stringstream ss;
          ss << "Checking whether BDD " << left_no << " is a subset of " << right_no;
          SCOPED_TRACE(ss.str());
          
          bdd left = bdds[left_no];
          bdd right = bdds[right_no];
          bool expected = answers[left_no][right_no];
          bool actual = bddImplies(left, right);
          bool actual_imp = bddImplies_using_bdd_imp(left, right);
          bool actual_rec = bddImplies_recursive(left, right);
          
          EXPECT_EQ(expected, actual);
          EXPECT_EQ(expected, actual_imp);
          EXPECT_EQ(expected, actual_rec);
        }
      }
    }
  }

  

} //namespace


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
