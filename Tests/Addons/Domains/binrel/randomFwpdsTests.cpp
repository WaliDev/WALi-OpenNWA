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
  static Voc *__voc;
  static sem_elem_t randomWt(){
    return new BinRel(tGetRandomTransformer(*__voc,false),false);
  }


  class RandomFWPDSTest: public ::testing::Test{
    protected:
      virtual void SetUp(){
        Voc initVoc;
        addBoolVar(initVoc,"a");
        addBoolVar(initVoc,"b");
        addBoolVar(initVoc,"c");
        addIntVar(initVoc,"p",4);
        addIntVar(initVoc,"q",4);
        /*
        addIntVar(initVoc,"r",4);
        addIntVar(initVoc,"s",4);
        addIntVar(initVoc,"t",4);
        addIntVar(initVoc,"u",4);
        addIntVar(initVoc,"v",4);
        addIntVar(initVoc,"w",4);
        addIntVar(initVoc,"x",4);
        addIntVar(initVoc,"y",4);
        addIntVar(initVoc,"z",4);
        */
        voc = initialize(0,0,initVoc);
        //ASSERT_EQ(voc.size(), 14);

        __voc = &voc;
      }

      virtual void TearDown(){
        binRelDone();
        __voc = NULL;
      }
    protected:
      Voc voc;
  };



  TEST_F(RandomFWPDSTest,genTest){
    Conf conf;
    conf.randomWt = &randomWt;
    conf.numprocs = 300;
    conf.numcalls = 1000;
    conf.numsplits = 600;
    conf.numnodes = 5000;
    Names names;
    FWPDS pds;
    //randfwpds(pds,conf, names, &cout);
    randfwpds(pds,conf, names);
    //pds.print(cout);
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    for(int i=i;i<conf.numprocs;++i)
      fa.addTrans(names.pdsState,names.entries[i],acc,(*randomWt)());
    fa.setInitialState(names.pdsState);
    fa.addFinalState(acc);
  
    //fa.print(cout);

    WFA outfa;
    wali::set_verify_fwpds(true);
    pds.poststarIGR(fa,outfa);

    //outfa.print(cout);

    ASSERT_TRUE(true);
  }
}
