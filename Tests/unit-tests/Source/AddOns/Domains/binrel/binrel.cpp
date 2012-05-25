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

// ::wali::domains::binrel
#include "wali/domains/binrel/BinRel.hpp"
#include "wali/domains/binrel/ProgramBddContext.hpp"


using namespace std;
using namespace wali;
using namespace wali::domains::binrel;

#include "Common.cpp"

namespace{

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
    ASSERT_EQ(brm->size(), 8);

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

    srand(time(NULL));
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



} //namespace

