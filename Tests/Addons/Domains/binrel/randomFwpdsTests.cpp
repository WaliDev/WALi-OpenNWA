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
#include "wali/domains/binrel/BinRelManager.hpp"

#include "generateRandomFWPDS.hpp"

using namespace std;
using namespace wali;
using namespace wali::domains::binrel;
using namespace wali::wpds::fwpds;

#include "Common.cpp"
namespace{
  // ***flinch***
  static Voc __voc;
  static sem_elem_t randomWt(){
    return new BinRel(tGetRandomTransformer(__voc,false),false);
  }


  class RandomFWPDSTest: public ::testing::Test{
    protected:
      virtual void SetUp(){
        Voc initVoc;
        addBoolVar(initVoc,"a");
        addBoolVar(initVoc,"b");
        addBoolVar(initVoc,"c");
        voc = initialize(0,0,initVoc);
        ASSERT_EQ(voc.size(), 3);

        __voc = voc;
      }

    protected:
      Voc voc;
  };



  TEST_F(RandomFWPDSTest,genTest){
    Conf conf;
    conf.randomWt = &randomWt;
    conf.numprocs = 5;
    conf.numcalls = 45;
    conf.numsplits = 40;
    conf.numnodes = 300;
    Names names;
    FWPDS pds;
    randfwpds(pds,conf, names,&cout);
    //pds.print(cout);
    ASSERT_TRUE(true);
  }
}
