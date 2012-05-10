// ::wali::wpds::nwpds
#include "wali/wpds/nwpds/NWPDS.hpp"
// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"
// ::wali::wfa
#include "wali/wfa/WFA.hpp"
// ::wali::domains::binrel
#include "wali/domains/binrel/ProgramBddContext.hpp"
// ::wali::wpds
#include "generateRandomFWPDS.hpp"
#include "wali/wpds/RuleFunctor.hpp"
// ::std
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
// ::wali
#include "wali/KeySpace.hpp"
#include "wali/Key.hpp"
#include "wali/ref_ptr.hpp"
using namespace std;
using namespace wali;
using namespace wali::wfa;
using namespace wali::wpds;
using namespace wali::wpds::ewpds;
using namespace wali::wpds::fwpds;
using namespace wali::wpds::nwpds;
using namespace wali::domains::binrel;

namespace{
  std::string regrDir = "regression_baseline";

  //declarations
  static void writeOutput(
      std::string testname,
      std::string varname, 
      std::stringstream& ss
      );

  //definitions
  static void writeOutput(
      std::string testname,
      std::string varname, 
      std::stringstream& ss
      )
  {
    std::string outpath = testname + "_" + varname + ".output";
    std::fstream fout(outpath.c_str(),std::ios_base::out);
    fout << ss.str();
  };

  bool compareOutput(
      std::string testname,
      std::string varname,
      std::stringstream& outs
      )
  {
    writeOutput(testname, varname, outs);
    std::string inpath = regrDir + "/" + testname + "_" + varname + ".output";
    cout << inpath << endl;
    std::fstream fin(inpath.c_str(),std::ios_base::in);
    std::stringstream ins;
    std::string s;
    char c;
    while(fin.get(c)){
      ins.put(c);
    }
   
    bool res = ins.str() == outs.str();
    if(!res){
      cout << "Difference found!!!\n";
      cout << "ins.str(): " << ins.str() << "\n";;
      cout << "outs.str(): " << outs.str() << "\n";
    }
    return res;
  };


  class MyWtGen : public RandomPdsGen::WtGen
  {
    public:
      MyWtGen(program_bdd_context_t b, unsigned s=0) : 
        WtGen(),
        seed(s),
        brm(b)
      {}

      virtual sem_elem_t operator () () 
      {
        return new BinRel(brm.get_ptr(),brm->tGetRandomTransformer(false,seed));
      }
    private:
      unsigned seed;
      program_bdd_context_t brm;
  };

  typedef ref_ptr< MyWtGen > mywtgen_t;


  class WFACompare : public wali::wfa::ConstTransFunctor
  {
    public:
      struct TransKey {
        wali::Key from;
        wali::Key stack;
        wali::Key to;
        TransKey(wali::Key f,wali::Key s,wali::Key t) :
          from(f),
          stack(s),
          to(t)
        {}
        bool operator == (const TransKey& other) const
        {
          return from == other.from &&  stack == other.stack && to == other.to;
        }
        bool operator < (const TransKey& other) const
        {
          return from < other.from || stack < other.stack || to < other.to;
        }
        ostream& print(ostream& out) const
        {
          out << "[" << from << " -- " << stack << " -> " << to << "]" << std::endl;
          return out;
        }
      };
      typedef enum {READ_FIRST, READ_SECOND, COMPARE} Mode;
      typedef std::map< TransKey, wali::sem_elem_t > DataMap;
      WFACompare(std::string f="FIRST", std::string s="SECOND") :  
        first(f), 
        second(s), 
        cur(READ_FIRST)  
      {}
      virtual void operator() (const ITrans* t)
      {
        if(cur == READ_FIRST)
          firstData[TransKey(t->from(),t->stack(),t->to())] = t->weight();
        else if(cur == READ_SECOND)
          secondData[TransKey(t->from(),t->stack(),t->to())] = t->weight();
        else
          assert(false && "Not in any read mode right now");
      }
      void advance_mode()
      {
        if(cur == READ_FIRST)
          cur = READ_SECOND;
        else if(cur == READ_SECOND)
          cur = COMPARE;
        else
          assert(false && "Where do you want to go dude? You're at the end of the line!");
      }
      bool diff(std::ostream& out) const
      {
        bool diffFound = false;
        assert(cur == COMPARE);
        for(DataMap::const_iterator iter = firstData.begin();
            iter != firstData.end();
            ++iter){
          DataMap::const_iterator iter2 = secondData.find(iter->first);
          if(iter2 == secondData.end()){
            diffFound=true;
            out << "DIFF: Found in " << first << " but not in " << second << ":" << std::endl;
            (iter->first).print(out);
            iter->second->print(out);
            out << std::endl;
          }else{
            if(iter->second != iter2->second){
            diffFound=true;
              out << "DIFF: Found in both but weights differ: " << std::endl;
              (iter->first).print(out);
              out << "[ " << first << " weight]" << std::endl;
              iter->second->print(out);
              out << "[ " << second << " weight]" << std::endl;
              iter2->second->print(out);
              out << std::endl;
            }
          }
        }
        for(DataMap::const_iterator iter = secondData.begin();
            iter != secondData.end();
            ++iter){
          DataMap::const_iterator iter2 = firstData.find(iter->first);
          if(iter2 == firstData.end()){
            diffFound=true;
            out << "DIFF: Found in " << second << " but not in " << first << ":" << std::endl;
            (iter->first).print(out);
            iter->second->print(out);
            out << std::endl;
          }
        }
        return diffFound;
      }
    private:
      string first;
      string second;
      Mode cur;
      DataMap firstData;
      DataMap secondData;
  };
}

int main()
{
  //NEED a CONST VALUE
  unsigned seed = 1;
  program_bdd_context_t bmt = new ProgramBddContext();
  random_pdsgen_t rpt;
  mywtgen_t mwg;
  bmt->addBoolVar("a");
  bmt->addBoolVar("b");
  bmt->addIntVar("p",3);
  bmt->addIntVar("q",3);
  mwg = new MyWtGen(bmt);
  //pds.print(cout);
  WFACompare fac("NEWTON","KLEENE");
  {
    NWPDS npds;
    RandomPdsGen::Names names;
    rpt = new RandomPdsGen(mwg,2,6,10,6,0,0.45,0.45,seed);
    rpt->get(npds,names);
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    for(int i=0;i<300;++i)
      fa.addTrans(names.pdsState,names.exits[i],acc,(*mwg)());
    fa.setInitialState(names.pdsState);
    fa.addFinalState(acc);
    {
      fstream newton_pds("newton_pds.dot", fstream::out);
      RuleDotty rd(newton_pds);
      newton_pds << "digraph{" << endl;
      npds.for_each(rd);
      newton_pds << "}" << endl;
    }
    WFA outfa;
    npds.prestar(fa,outfa);
    outfa.for_each(fac);
    fac.advance_mode();
  }
  {
    FWPDS fpds;
    RandomPdsGen::Names names;
    rpt = new RandomPdsGen(mwg,2,6,10,6,0,0.45,0.45,seed);
    rpt->get(fpds,names);
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    for(int i=0;i<300;++i)
      fa.addTrans(names.pdsState,names.exits[i],acc,(*mwg)());
    fa.setInitialState(names.pdsState);
    fa.addFinalState(acc);
    {
      fstream kleene_pds("kleene_pds.dot", fstream::out);
      RuleDotty rd(kleene_pds);
      kleene_pds << "digraph{" << endl;
      fpds.for_each(rd);
      kleene_pds << "}" << endl;
    }
    WFA outfa;
    fpds.prestar(outfa,fa);
    outfa.for_each(fac);
    fac.advance_mode();
  }
  fstream fadiff("fa_diff",fstream::out);
  fac.diff(fadiff);
  return 0;
}
