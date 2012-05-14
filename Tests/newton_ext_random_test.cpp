// ::wali::wpds::nwpds
#include "wali/wpds/nwpds/NWPDS.hpp"
// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"
// ::wali::wpds::ewpds
#include "wali/wpds/ewpds/EWPDS.hpp"
#if defined(USE_AKASH_EWPDS) || defined(USING_AKASH_FWPDS)
#include "wali/wpds/ewpds/ERule.hpp"
#endif
// ::wali::wfa
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/TransFunctor.hpp"
// ::wali::domains::binrel
#include "wali/domains/binrel/ProgramBddContext.hpp"
// ::wali::wpds
#include "generateRandomFWPDS.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"
// ::std
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
// ::wali
#include "wali/KeySpace.hpp"
#include "wali/Key.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/MergeFn.hpp"
// ::wali::util
#include "wali/util/Timer.hpp"
using namespace std;
using namespace wali;
using namespace wali::wfa;
using namespace wali::wpds;
using namespace wali::wpds::ewpds;
using namespace wali::wpds::fwpds;
using namespace wali::wpds::nwpds;
using namespace wali::domains::binrel;

namespace{
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
      class TransKey {
        public:
        wali::Key from;
        wali::Key stack;
        wali::Key to;
        TransKey(wali::Key f,wali::Key s,wali::Key t) :
          from(f),
          stack(s),
          to(t)
        {}
        bool operator < (const TransKey& other) const
        {
          return from < other.from || from == other.from && (
                  stack < other.stack || stack == other.stack && (
                    to < other.to));
        }
        ostream& print(ostream& out) const
        {
          out << "[" << wali::key2str(from) << " -- " << wali::key2str(stack) << " -> " << wali::key2str(to) << "]" << std::endl;
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
      bool diff(std::ostream& out) 
      {
        bool diffFound = false;
        assert(cur == COMPARE);

        /*
        {//DEBUG
          out << "firstData:" << std::endl;
          for(DataMap::const_iterator iter = firstData.begin();
              iter != firstData.end();
              iter++)
            (iter->first).print(out);
          out << "secondData:" << std::endl;
          for(DataMap::const_iterator iter = secondData.begin();
              iter != secondData.end();
              iter++)
            (iter->first).print(out);
        }//DEBUG
        */

        for(DataMap::const_iterator iter = firstData.begin();
            iter != firstData.end();
            ++iter){
          TransKey tk = iter->first;
          DataMap::iterator iter2 = secondData.find(tk);
          if(iter2 == secondData.end()){
            if(!(iter->second == NULL) && !(iter->second->equal(iter->second->zero()))){
              diffFound=true;
              out << "DIFF: Found in " << first << " but not in " << second << ":" << std::endl;
              (iter->first).print(out);
              iter->second->print(out);
              out << std::endl;
            }
          }else{
            if((iter->second == NULL && iter2->second != NULL) ||
               (iter->second != NULL && iter2->second == NULL) ||
               !((iter->second)->equal(iter2->second))){
              diffFound=true;
              out << "DIFF: Found in both but weights differ: " << std::endl;
              (iter->first).print(out);
              out << std::endl << "[ " << first << " weight]" << std::endl;
              if(iter->second != NULL)
                iter->second->print(out);
              else
                out << "NULL" << std::endl;
              out << std::endl << "[ " << second << " weight]" << std::endl;
              if(iter2->second != NULL)
                iter2->second->print(out);
              else
                out << "NULL" << std::endl;
              out << std::endl;
            }
          }
        }
        for(DataMap::const_iterator iter = secondData.begin();
            iter != secondData.end();
            ++iter){
          DataMap::iterator iter2 = firstData.find(iter->first);
          if(iter2 == firstData.end()){
            if(!(iter->second == NULL) && !(iter->second->equal(iter->second->zero()))){
              diffFound=true;
              out << "DIFF: Found in " << second << " but not in " << first << ":" << std::endl;
              (iter->first).print(out);
              iter->second->print(out);
              out << std::endl;
            }
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

  class PDSCompare : public wali::wpds::ConstRuleFunctor
  {
    public:
      class RuleKey {
        public:
          wali::Key from_state;
          wali::Key from_stack;
          wali::Key to_state;
          wali::Key to_stack1;
          wali::Key to_stack2;

          RuleKey(wali::Key fstate,wali::Key fstack,wali::Key tstate, wali::Key tstack1, wali::Key tstack2) :
            from_state(fstate),
            from_stack(fstack),
            to_state(tstate),
            to_stack1(tstack1),
            to_stack2(tstack2)
        {}
          bool operator < (const RuleKey& other) const
          {
            return from_state < other.from_state || from_state == other.from_state && (
                from_stack < other.from_stack || from_stack == other.from_stack && (
                  to_state < other.to_state || to_state == other.to_state && (
                    to_stack1 < other.to_stack1 || to_stack1 == other.to_stack1 && (
                      to_stack2 < other.to_stack2))));
          }
          ostream& print(ostream& out) const
          {
            out << "[<" << wali::key2str(from_state) << " , " << wali::key2str(from_stack) <<
              " > --> < " << wali::key2str(to_state) << " , " << wali::key2str(to_stack1) << " " << wali::key2str(to_stack2) << " >]" << std::endl;
            return out;
          }
      };
      typedef enum {READ_FIRST, READ_SECOND, COMPARE} Mode;
      typedef std::map< RuleKey, wali::sem_elem_t > WeightMap;
#if defined(USING_AKASH_EWPDS) || defined(USING_AKASH_FWPDS)
      typedef std::map< RuleKey, wali::merge_fn_t > MergeFnMap;
#endif
      PDSCompare(std::string f="FIRST", std::string s="SECOND") :  
        first(f), 
        second(s), 
        cur(READ_FIRST)  
    {}
      virtual void operator() (const rule_t& t)
      {
        if(cur == READ_FIRST){
          firstWts[RuleKey(t->from_state(),t->from_stack(),t->to_state(),t->to_stack1(),t->to_stack2())] = t->weight();
        }else if(cur == READ_SECOND){
          secondWts[RuleKey(t->from_state(),t->from_stack(),t->to_state(),t->to_stack1(),t->to_stack2())] = t->weight();
        }else{
          assert(false && "Not in any read mode right now");
        }
#if defined(USING_AKASH_EWPDS) || defined(USING_AKASH_FWPDS)
        if(cur == READ_FIRST){
          firstMergeFns[RuleKey(t->from_state(),t->from_stack(),t->to_state(),t->to_stack1(),t->to_stack2())] = t->merge_fn();
        }else if(cur == READ_SECOND){
          secondMergeFns[RuleKey(t->from_state(),t->from_stack(),t->to_state(),t=>to_stack1(),t->to_stack2())] = t->merge_fn();
        }else{
          assert(false && "Not in any read mode right now");
        }
#endif

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
      bool diff(std::ostream& out) 
      {
        bool diffFound = false;
        assert(cur == COMPARE);

        for(WeightMap::const_iterator iter = firstWts.begin();
            iter != firstWts.end();
            ++iter){
          RuleKey tk = iter->first;
          WeightMap::iterator iter2 = secondWts.find(tk);
          if(iter2 == secondWts.end()){
            if(!(iter->second == NULL) && !(iter->second->equal(iter->second->zero()))){
              diffFound=true;
              out << "DIFF: Weight found in " << first << " but not in " << second << ":" << std::endl;
              (iter->first).print(out);
              iter->second->print(out);
              out << std::endl;
            }
          }else{
            if((iter->second == NULL && iter2->second != NULL) ||
                (iter->second != NULL && iter2->second == NULL) ||
                !((iter->second)->equal(iter2->second))){
              diffFound=true;
              out << "DIFF: Found in both but weights differ: " << std::endl;
              (iter->first).print(out);
              out << std::endl << "[ " << first << " weight]" << std::endl;
              if(iter->second != NULL)
                iter->second->print(out);
              else
                out << "NULL" << std::endl;
              out << std::endl << "[ " << second << " weight]" << std::endl;
              if(iter2->second != NULL)
                iter2->second->print(out);
              else
                out << "NULL" << std::endl;
              out << std::endl;
            }
          }
        }
        for(WeightMap::const_iterator iter = secondWts.begin();
            iter != secondWts.end();
            ++iter){
          WeightMap::iterator iter2 = firstWts.find(iter->first);
          if(iter2 == firstWts.end()){
            if(!(iter->second == NULL) && !(iter->second->equal(iter->second->zero()))){
              diffFound=true;
              out << "DIFF: Found in " << second << " but not in " << first << ":" << std::endl;
              (iter->first).print(out);
              iter->second->print(out);
              out << std::endl;
            }
          }
        }
        return diffFound;
      }
    private:
      string first;
      string second;
      Mode cur;
      WeightMap firstWts;
      WeightMap secondWts;
#if defined(USING_AKASH_EWPDS) || defined(USING_AKASH_FWPDS)
      MergeFnMap firstMergeFns;
      MergeFnMap secondMergeFns;
#endif
  };

}

int main()
{
  //NEED a CONST VALUE
  unsigned seed = time(NULL);
  //unsigned seed = 111;
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
  PDSCompare pac("NEWTON","KLEENE");
  {
    NWPDS npds(false);
    RandomPdsGen::Names names;
    {
      wali::util::Timer * t1 = new wali::util::Timer("Generating Random PDS");
      // rpt = new RandomPdsGen(mwg,2,6,10,6,0,0.45,0.45,seed);
      rpt = new RandomPdsGen(mwg,400,6000,10000,2000,0,0.45,0.45,seed);
      rpt->get(npds,names);
      delete t1;
    }
    npds.for_each(pac);
    pac.advance_mode();
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    for(RandomPdsGen::Names::KeyVector::iterator iter =  names.exits.begin();
        iter != names.exits.end();
        ++iter
       )
      fa.addTrans(names.pdsState,*iter,acc,(*mwg)());
    fa.setInitialState(names.pdsState);
    fa.addFinalState(acc);
    {
      fstream newton_pds("newton_pds.dot", fstream::out);
      RuleDotty rd(newton_pds);
      newton_pds << "digraph{" << endl;
      npds.for_each(rd);
      newton_pds << "}" << endl;
    }
    {
      fstream innfa("newton_in_fa.dot", fstream::out);
      TransDotty td(innfa,false);
      innfa << "digraph{" << endl;
      fa.for_each(td);
      innfa << "}" << endl;
    }
    WFA outfa;
    {
      wali::util::Timer * t2 = new wali::util::Timer("NWPDS prestar",cout);
      cout << "[NWPDS prestar]\n";
      npds.poststar(fa,outfa);
      delete t2;
    }
    outfa.for_each(fac);
    {
      fstream outfaf("newton_out_fa.dot", fstream::out);
      TransDotty td(outfaf,false);
      outfaf << "digraph{" << endl;
      outfa.for_each(td);
      outfaf << "}" << endl;
    }
    fac.advance_mode();
  }
  {
    EWPDS fpds;
    RandomPdsGen::Names names;
    //rpt = new RandomPdsGen(mwg,2,6,10,6,0,0.45,0.45,seed);
    rpt = new RandomPdsGen(mwg,400,6000,10000,2000,0,0.45,0.45,seed);

    rpt->get(fpds,names);
    fpds.for_each(pac);
    pac.advance_mode();
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    for(RandomPdsGen::Names::KeyVector::iterator iter =  names.exits.begin();
        iter != names.exits.end();
        ++iter
       )
      fa.addTrans(names.pdsState,*iter,acc,(*mwg)());
    fa.setInitialState(names.pdsState);
    fa.addFinalState(acc);
    {
      fstream kleene_pds("kleene_pds.dot", fstream::out);
      RuleDotty rd(kleene_pds);
      kleene_pds << "digraph{" << endl;
      fpds.for_each(rd);
      kleene_pds << "}" << endl;
    }
    {
      fstream innfa("kleene_in_fa.dot", fstream::out);
      TransDotty td(innfa,false);
      innfa << "digraph{" << endl;
      fa.for_each(td);
      innfa << "}" << endl;
    }
    WFA outfa;
    {
      wali::util::Timer * t3 = new wali::util::Timer("EWPDS prestar",cout);
      cout << "[EWPDS prestar]\n";
      fpds.poststar(fa,outfa);
      delete t3;
    }
    outfa.for_each(fac);
    {
      fstream outfaf("kleene_out_fa.dot", fstream::out);
      TransDotty td(outfaf,false);
      outfaf << "digraph{" << endl;
      outfa.for_each(td);
      outfaf << "}" << endl;
    }
    fac.advance_mode();
  }
  fstream fadiff("fa_diff",fstream::out);
  fac.diff(fadiff);
  fstream pdsdiff("pds_diff",fstream::out);
  pac.diff(pdsdiff);
  return 0;
}
