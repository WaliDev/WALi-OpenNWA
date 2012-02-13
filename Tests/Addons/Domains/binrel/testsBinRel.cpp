#include "gtest/gtest.h"
#include "buddy/bdd.h"

// ::std
#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <sstream>
#include <string>

// ::wali::domains::binrel
#include "wali/domains/binrel/BinRel.hpp"
#include "wali/domains/binrel/BinRelManager.hpp"


using namespace std;
using namespace wali;
using namespace wali::domains::binrel;

#include "Common.cpp"

namespace{

  TEST(BinRelSelfTest, creationTest){
    Voc initVoc;
    addBoolVar(initVoc,"a");
    addBoolVar(initVoc,"b");
    addBoolVar(initVoc,"c");
    addBoolVar(initVoc,"d");

    addIntVar(initVoc,"p",4);
    addIntVar(initVoc,"q",4);
    addIntVar(initVoc,"r",4);
    addIntVar(initVoc,"s",4);

    Voc voc = initialize(0,0,initVoc);
    ASSERT_EQ(voc.size(), 8);

    bdd a = From("a");
    bdd p = From("p");

    sem_elem_t se1 = new BinRel(a,false);
    sem_elem_t se2 = new BinRel(p,false);

    stringstream ss;
    se1->print(ss << "se1: ") << endl;
    se2->print(ss << "se2: ") << endl;
    ASSERT_TRUE(compareOutput("BinRelSelfTest","creationTest",ss));
  }

  TEST(BinRelSelfTest, selfTest){
    Voc initVoc;
    addBoolVar(initVoc,"a");
    addBoolVar(initVoc,"b");

    addIntVar(initVoc,"p",4);

    Voc voc = initialize(0,0,initVoc);
    ASSERT_EQ(voc.size(), 3);
    bdd a = From("a");
    a = Assign("b",a);
    sem_elem_t se1 = new BinRel(a,false);
    wali::test_semelem_impl(se1);
  }


  class BinRelTestBool: public ::testing::Test{
    protected:
      virtual void SetUp(){
        Voc initVoc;
        addBoolVar(initVoc,"a");
        addBoolVar(initVoc,"b");
        addBoolVar(initVoc,"c");
        voc = initialize(0,0,initVoc);
        ASSERT_EQ(voc.size(), 3);
      }
    protected:
      Voc voc;
  };

  TEST_F(BinRelTestBool,constantTests){
    stringstream ss;
    sem_elem_t se = new BinRel(False(),false);
    se->one()->print(ss << "one(): ") << endl;
    se->zero()->print(ss << "zero(): ") << endl;
    EXPECT_TRUE(compareOutput("BinRelTestBool","constantTests",ss));
  }

  TEST_F(BinRelTestBool,extendTests){
    stringstream ss;
    bdd a;
    a = Assign("c", And(From("a"),From("b")));
    sem_elem_t se1 = new BinRel(a,false);
    a = Assume(From("a"), From("b"));
    sem_elem_t se2 = new BinRel(a,false);
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
    a = Assign("c", And(From("a"),From("b")));
    sem_elem_t se1 = new BinRel(a,false);
    a = Assume(From("a"), From("b"));
    sem_elem_t se2 = new BinRel(a,false);
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
    a = Assign("c", And(From("a"),From("b")));
    sem_elem_tensor_t se1 = new BinRel(a,false);
    a = Assume(From("a"), From("b"));
    sem_elem_tensor_t se2 = new BinRel(a,false);

    se1->print(ss << "se1: ") << endl;
    se1->transpose()->print(ss << "se1: ") << endl;
    se2->print(ss << "se2: ") << endl;
    se2->transpose()->print(ss << "se2: ") << endl;

    EXPECT_TRUE(compareOutput("BinRelTestBool","transposeTests",ss));
  }

  TEST_F(BinRelTestBool, tensorTests){
    Voc initVoc;
    addBoolVar(initVoc,"a");
    addBoolVar(initVoc,"b");
    voc = initialize(0,0,initVoc);
    ASSERT_EQ(voc.size(), 2);

    stringstream ss;
    bdd a;
    a = Assume(From("a"), From("b"));
    sem_elem_tensor_t se1 = new BinRel(a,false);
    a = Assume(From("a"), Not(From("b")));
    sem_elem_tensor_t se2 = new BinRel(a,false);
    sem_elem_tensor_t se3 = se1->tensor(se2.get_ptr());

    se1->print(ss << "se1: ") << endl;
    se2->print(ss << "se2: ") << endl;
    se3->print(ss << "se3: ") << endl;

    EXPECT_TRUE(compareOutput("BinRelTestBool","tensorTests",ss));

  }


  TEST_F(BinRelTestBool, detensorTest){
    stringstream ss;
    bdd a;
    a = Assume(From("a"), From("b"));
    sem_elem_tensor_t se1 = new BinRel(a,false);
    a = Assume(From("b"), From("c"));
    sem_elem_tensor_t se2 = new BinRel(a,false);
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
    a = Assign("a", From("c"));
    sem_elem_tensor_t se1 = new BinRel(a,false);
    a = Assume(From("b"), From("c"));
    sem_elem_tensor_t se2 = new BinRel(a,false);
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
        Voc initVoc;
        addIntVar(initVoc,"a",4);
        addIntVar(initVoc,"b",4);
        addIntVar(initVoc,"c",4);
        voc = initialize(0,0,initVoc);
        ASSERT_EQ(voc.size(), 3);
      }
    protected:
      sem_elem_tensor_t screenVar(string var, sem_elem_tensor_t wt);
      Voc voc;
  };

  sem_elem_tensor_t BinRelTestInt::screenVar(string var, sem_elem_tensor_t wt)
  {
    sem_elem_tensor_t screen = new BinRel(Assume(From(var),Const(0)), false);
    return dynamic_cast<SemElemTensor*>(wt->extend(screen.get_ptr()).get_ptr());
  }

  TEST_F(BinRelTestInt,testScreen){
    stringstream ss;
    sem_elem_tensor_t wt = new BinRel(Const(0), false);
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

} //namespace

