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

// ::wali::wfa
#include "wali/wfa/WFA.hpp"

// ::wali
#include "wali/KeySpace.hpp"
#include "wali/Key.hpp"

using namespace std;
using namespace wali;
using namespace wali::domains::binrel;
using namespace wali::wpds::fwpds;
using namespace wali::wfa;

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
    conf.numprocs = 50;
    conf.numcalls = 450;
    conf.numsplits = 400;
    conf.numnodes = 9000;
    Names names;
    FWPDS pds;
    randfwpds(pds,conf, names, &cout);
    pds.print(cout);
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    for(int i=i;i<conf.numprocs;++i)
      fa.addTrans(names.pdsState,names.entries[i],acc,(*randomWt)());
    fa.setInitialState(names.pdsState);
    fa.addFinalState(acc);
  
    //fa.print(cout);

    WFA outfa;
    pds.poststarIGR(fa,outfa);

    //outfa.print(cout);

    ASSERT_TRUE(true);
  }
}
