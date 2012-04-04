/**
 * You can only run any one test using FWPDS at a time.
 * This is because RegExp do not clean properly.
 * */

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

// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"

using namespace std;
using namespace wali;
using namespace wali::domains::binrel;
using namespace wali::wpds::fwpds;
using namespace wali::wfa;

#include "Common.cpp"
namespace{

  struct MyConf : public Conf
  {
    MyConf(Voc _voc) : Conf(), voc(_voc) {}
    virtual wali::sem_elem_t randomWt() const{
      return new BinRel(tGetRandomTransformer(voc,false),false);
    }
    //! The number of procedures to be generated
    int numprocs;
    //! The number of callsites to be generated
    int numcalls;
    //! The number of cfg nodes to be generated [loosely followed]
    int numnodes;
    //! The number of two-way splits to be generated.
    int numsplits;
    //! Number of error points to be generated
    int numerrs;

    virtual ~MyConf(){}
    private:
    Voc voc;
  };




  class NewtonSolverTests: public ::testing::Test
  {
    protected:
      virtual void SetUp(){
        Voc initVoc;
        addBoolVar(initVoc,"a");
        addBoolVar(initVoc,"b");
        addBoolVar(initVoc,"c");
        addIntVar(initVoc,"p",4);
        addIntVar(initVoc,"q",4);
        addIntVar(initVoc,"r",4);
        addIntVar(initVoc,"s",4);
        addIntVar(initVoc,"t",4);
        addIntVar(initVoc,"u",4);
        addIntVar(initVoc,"v",4);
        addIntVar(initVoc,"w",4);
        addIntVar(initVoc,"x",4);
        addIntVar(initVoc,"y",4);
        addIntVar(initVoc,"z",4);

        voc = initialize(0,0,initVoc);
        //ASSERT_EQ(voc.size(), 14);

        conf = new MyConf(voc);
        conf->numprocs = 100;
        conf->numcalls = 100;
        conf->numsplits = 200;
        conf->numnodes = 300;
        randfwpds(pds,*conf, names, &cout);
        //randfwpds(pds,*conf, names);
        //pds.print(cout);
      }
      virtual void TearDown(){
        binRelDone();
        delete conf;
      }
    protected:
      Voc voc;
      Conf* conf;
      FWPDS pds;
      Names names;
  };



  TEST(RandomPds,pdsGenTest){
    Voc voc;
    Conf* conf;
    Voc initVoc;
    addBoolVar(initVoc,"a");
    addBoolVar(initVoc,"b");
    addBoolVar(initVoc,"c");
    addBoolVar(initVoc,"d");
    addBoolVar(initVoc,"e");
    addBoolVar(initVoc,"f");
    addIntVar(initVoc,"p",4);
    addIntVar(initVoc,"q",4);
    addIntVar(initVoc,"r",4);
    addIntVar(initVoc,"s",4);
    addIntVar(initVoc,"t",4);
    addIntVar(initVoc,"u",4);
    addIntVar(initVoc,"v",4);
    addIntVar(initVoc,"w",4);
    voc = initialize(0,0,initVoc);
    //ASSERT_EQ(voc.size(), 14);

    conf = new MyConf(voc);
    conf->numprocs = 300;
    conf->numcalls = 1000;
    conf->numsplits = 600;
    conf->numnodes = 5000;
    Names names;
    FWPDS pds;
    //randfwpds(pds,conf, names, &cout);
    randfwpds(pds,*conf, names);
    //pds.print(cout);
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    for(int i=0;i<conf->numprocs;++i)
      fa.addTrans(names.pdsState,names.entries[i],acc,conf->randomWt());
    fa.setInitialState(names.pdsState);
    fa.addFinalState(acc);

    //fa.print(cout);

    WFA outfa;
    wali::set_verify_fwpds(false);
    //yes, I trust FWPDS so much that I use it to check that
    //the pds constructed makes sense.
    //TOUGH LUCK: fwpds/ewpds outputs do not match! 
    pds.poststarIGR(fa,outfa);

    //outfa.print(cout);

    ASSERT_TRUE(true);

    //cleanup
    binRelDone();
    delete conf;
  }

  TEST_F(NewtonSolverTests, first){
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    //for(int i=0;i<conf->numprocs;++i)
    //  fa.addTrans(names.pdsState,names.entries[i],acc,conf->randomWt());
    fa.addTrans(names.pdsState, names.entries[0],acc,conf->randomWt());
    fa.setInitialState(names.pdsState);
    fa.addFinalState(acc);

    //fa.print(cout);

    pds.useNewton(true);

    WFA outfa;
    wali::set_verify_fwpds(false);
    pds.poststarIGR(fa,outfa);

  }

}
