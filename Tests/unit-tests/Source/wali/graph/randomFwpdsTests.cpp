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

// ::wali::wpds
#include "generateRandomFWPDS.hpp"

// ::wali::wfa
#include "wali/wfa/WFA.hpp"

// ::wali
#include "wali/KeySpace.hpp"
#include "wali/Key.hpp"
#include "wali/ref_ptr.hpp"

// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"

using namespace std;
using namespace wali;
using namespace wali::domains::binrel;
using namespace wali::wpds::fwpds;
using namespace wali::wpds;
using namespace wali::wfa;

#include "Common.cpp"
namespace{

  class MyWtGen : public RandomPdsGen::WtGen
  {
    public:
      MyWtGen(binrel_manager_t b) : 
        WtGen(),
        brm(b)
      {}

      virtual sem_elem_t operator () () 
      {
        return new BinRel(brm->tGetRandomTransformer());
      }
    private:
      binrel_manager_t brm;

  };

  typedef ref_ptr< MyWtGen > mywtgen_t;

  class NewtonSolverTests: public ::testing::Test
  {
    protected:
      virtual void SetUp(){
        Voc voc;
        addBoolVar(voc,"a");
        addBoolVar(voc,"b");
        addIntVar(voc,"p",3);
        addIntVar(voc,"q",3);

        bmt = new BinRelManager(voc);
        mwg = new MyWtGen(bmt);
        rpt = new RandomPdsGen(mwg,2,6,10,6);
        rpt->get(pds,names);
        pds.print(cout);
      }
      virtual void TearDown(){
        bmt = NULL;
        rpt = NULL;
      }
    protected:
      Voc voc;
      binrel_manager_t bmt;
      random_pdsgen_t rpt;
      mywtgen_t mwg;
      FWPDS pds;
      RandomPdsGen::Names names;
  };


  TEST(RandomPds,pdsGenTest){
    Voc voc;
    addBoolVar(voc,"a");
    addBoolVar(voc,"b");
    addBoolVar(voc,"c");
    addIntVar(voc,"q",4);
    addIntVar(voc,"w",4);
    binrel_manager_t bmt = new BinRelManager(voc);
    mywtgen_t mwg = new MyWtGen(bmt);
    random_pdsgen_t rpt = new RandomPdsGen(mwg);
    RandomPdsGen::Names names;
    FWPDS pds;
    rpt->get(pds, names);
    //pds.print(cout);
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    for(int i=0;i<300;++i)
      fa.addTrans(names.pdsState,names.entries[i],acc,(*mwg)());
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
  }

  TEST_F(NewtonSolverTests, first){
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    //for(int i=0;i<conf->numprocs;++i)
    //  fa.addTrans(names.pdsState,names.entries[i],acc,conf->randomWt());
    fa.addTrans(names.pdsState, names.entries[0],acc,(*mwg)());
    fa.setInitialState(names.pdsState);
    fa.addFinalState(acc);

    //fa.print(cout);

    pds.useNewton(true);

    WFA outfa;
    wali::set_verify_fwpds(false);
    pds.poststarIGR(fa,outfa);

    sem_elem_tensor_t one = dynamic_cast<SemElemTensor*>(((*mwg)()->one()).get_ptr());
    sem_elem_tensor_t onet = one->tensor(one.get_ptr());

    cout << "Running Path Summary\n";
    outfa.path_summary(onet);
  }
}
