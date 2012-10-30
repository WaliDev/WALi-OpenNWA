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

} //namespace

