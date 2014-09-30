// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"
// ::wali::wpds::ewpds
#include "wali/wpds/ewpds/EWPDS.hpp"
// ::wali::wpds
#include "wali/wpds/WPDS.hpp"
// ::wali::wpds::nwpds
#include "wali/wpds/nwpds/NWPDS.hpp"
#if defined(USE_AKASH_EWPDS) || defined(USING_AKASH_FWPDS)
#include "wali/wpds/ewpds/ERule.hpp"
#endif
// ::wali::wfa
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/State.hpp"
// ::wali::wpds
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
// ::wali::util
#include "wali/util/Timer.hpp"
// ::wali::cprover
#include "BplToPds.hpp"
#include "wali/domains/name_weight/nameWeight.hpp"
//#include "PtoICFG.hpp"
// TSL
#include "gtr/src/lang/gtr_config.h"
#include "tsl/cir/regexp/conc.1level.cir.hpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_extern_types.hpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_externs.hpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_extern_types.cpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_externs.cpp"
#include "tsl/analysis_components/src/reinterps/emul/concrete_base_type_interp.hpp"
#include "tsl/analysis_components/src/reinterps/emul/concrete_base_type_interp.cpp"
#include "tsl/cir/regexp/conc.1level.cir.cpp"
#include "wali/domains/binrel/BinRel.hpp"
#include "NewtonCompare.hpp"

using namespace std;
using namespace wali;
using namespace wali::wfa;
using namespace wali::wpds;
using namespace wali::wpds::ewpds;
using namespace wali::wpds::fwpds;
using namespace wali::wpds::nwpds;
using namespace wali::cprover;
using namespace wali::domains::binrel;
using namespace wali::domains::name_weight;
using namespace tsl_regexp;

typedef wali::ref_ptr<graph::RegExp> reg_exp_t;
typedef tsl_regexp::Conc1LevelRTG RTG;
typedef tsl_regexp::Conc1LevelCIR CIR;
typedef std::map<int,RTG::regExpRefPtr> tslRegExpMap;
typedef std::map<int,RTG::regExpTRefPtr> tslRegExpTMap;
typedef std::map<int,RTG::regExpTListRefPtr> tslDiffMap;

#include <pthread.h>
#include <signal.h>
#include <boost/cast.hpp>

static pthread_t worker;

extern "C" 
{
  void handle_sighup(int signum)
  {
    if(signum == SIGHUP){
        pthread_cancel(worker);
    }
  }
}

BinRelInterface * ICon = NULL;
BddContext * con = NULL;

namespace{

  class WFACompare : public wali::wfa::ConstTransFunctor
  {
    public:
      class TransKey {
        public:
          /*
             wali::Key from;
             wali::Key stack;
             wali::Key to;
           */
          std::string from;
          std::string stack;
          std::string to;
          TransKey(wali::Key f,wali::Key s,wali::Key t) :
            from(wali::key2str(f)),
            stack(wali::key2str(s)),
            to(wali::key2str(t))
        {}
          bool operator < (const TransKey& other) const
          {
            return from < other.from || 
              (from == other.from && ( stack < other.stack || 
                                       (stack == other.stack && to < other.to)));
          }
          ostream& print(ostream& out) const
          {
            //out << "[" << wali::key2str(from) << " -- " << wali::key2str(stack) << " -> " << wali::key2str(to) << "]" << std::endl;
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
        else if(cur == READ_SECOND){
          wali::SemElemTensor * wt = boost::polymorphic_downcast<SemElemTensor*>(t->weight().get_ptr());
          secondData[TransKey(t->from(),t->stack(),t->to())] = wt;//->detensorTranspose();
        }
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
      bool diff(std::ostream * out = 0) 
      {
        bool diffFound = false;
        assert(cur == COMPARE);

        for(DataMap::const_iterator iter = firstData.begin();
            iter != firstData.end();
            ++iter){
          TransKey tk = iter->first;
          DataMap::iterator iter2 = secondData.find(tk);
          if(iter2 == secondData.end()){
            if(!(iter->second == NULL) && !(iter->second->equal(iter->second->zero()))){
              diffFound=true;
              if(out){
                *out << "DIFF: Found in " << first << " but not in " << second << ":" << std::endl;
                (iter->first).print(*out);
                iter->second->print(*out);
                *out << std::endl;
              }
            }
          }else{
            sem_elem_t fwpdswt = iter->second;
            sem_elem_tensor_t nwpdswt = boost::polymorphic_downcast<SemElemTensor*>(iter2->second.get_ptr());
            if(!(iter2->second == NULL)){
              nwpdswt = boost::polymorphic_downcast<SemElemTensor*>(nwpdswt->detensorTranspose().get_ptr())->transpose();
              assert(!(nwpdswt == NULL));
            }
            if((fwpdswt == NULL && nwpdswt != NULL) ||
                (fwpdswt != NULL && nwpdswt == NULL) ||
                !((fwpdswt)->equal(nwpdswt))){
              diffFound=true;
              if(out){
                *out << "DIFF: Found in both but weights differ: " << std::endl;
                (iter->first).print(*out);
                *out << std::endl << "[ " << first << " weight]" << std::endl;
                if(fwpdswt != NULL)
                  fwpdswt->print(*out);
                else
                  *out << "NULL" << std::endl;
                *out << std::endl << "[ " << second << " weight]" << std::endl;
                if(nwpdswt != NULL)
                  nwpdswt->print(*out);
                else
                  *out << "NULL" << std::endl;
                *out << std::endl;
              }
            }else{
              //DEBUGGING
              if(out && 0){
                *out << "Printing anyway:\n";
                (iter->first).print(*out);
                *out << std::endl << "[ " << first << " weight]" << std::endl;
                if(iter->second != NULL)
                  iter->second->print(*out);
                else
                  *out << "NULL" << std::endl;
                *out << std::endl << "[ " << second << " weight]" << std::endl;
                if(iter2->second != NULL)
                  iter2->second->print(*out);
                else
                  *out << "NULL" << std::endl;
                *out << std::endl;
              }
              //DEBUGGING
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
              if(out){
                *out << "DIFF: Found in " << second << " but not in " << first << ":" << std::endl;
                (iter->first).print(*out);
                iter->second->print(*out);
                *out << std::endl;
              }
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
            return from_state < other.from_state || (from_state == other.from_state && (
                  from_stack < other.from_stack || (from_stack == other.from_stack && (
                      to_state < other.to_state || (to_state == other.to_state && (
                          to_stack1 < other.to_stack1 || (to_stack1 == other.to_stack1 && (
                              to_stack2 < other.to_stack2))))))));
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
      bool diff(std::ostream * out = 0) 
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
              if(out){
                *out << "DIFF: Weight found in " << first << " but not in " << second << ":" << std::endl;
                (iter->first).print(*out);
                iter->second->print(*out);
                *out << std::endl;
              }
            }
          }else{
            if((iter->second == NULL && iter2->second != NULL) ||
                (iter->second != NULL && iter2->second == NULL) ||
                !((iter->second)->equal(iter2->second))){
              diffFound=true;
              if(out){
                *out << "DIFF: Found in both but weights differ: " << std::endl;
                (iter->first).print(*out);
                *out << std::endl << "[ " << first << " weight]" << std::endl;
                if(iter->second != NULL)
                  iter->second->print(*out);
                else
                  *out << "NULL" << std::endl;
                *out << std::endl << "[ " << second << " weight]" << std::endl;
                if(iter2->second != NULL)
                  iter2->second->print(*out);
                else
                  *out << "NULL" << std::endl;
                *out << std::endl;
              }
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
              if(out){
                *out << "DIFF: Found in " << second << " but not in " << first << ":" << std::endl;
                (iter->first).print(*out);
                iter->second->print(*out);
                *out << std::endl;
              }
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


namespace goals {
  
  std::set<pair<std::pair<int,int>,int> > globalEpsilon;
  short dump = false;
  char * mainProc = NULL, * errLbl = NULL;
  string fname;
  prog * pg;
  
  void doPostStar(WPDS * pds, WFA& outfa)
  {
    if(dump){
      fstream outfile("init_outfa.dot", fstream::out);
      outfa.print_dot(outfile, true);
      outfile.close();
    }
    if(dump){
      fstream outfile("init_outfa.txt", fstream::out);
      outfa.print(outfile);
      outfile.close();
    }

    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, pds->get_theZero()->one());
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);

   if(dump){
      fstream outfile("init_outfa.dot", fstream::out);
      fa.print_dot(outfile, true);
      outfile.close();
    }
    if(dump){
      fstream outfile("init_outfa.txt", fstream::out);
      fa.print(outfile);
      outfile.close();
    }


    cout << "[Newton Compare] Computing poststar..." << endl;
    FWPDS * fpds = NULL;
    fpds = dynamic_cast<FWPDS*>(pds);
    if(fpds == NULL){
      pds->poststar(fa,outfa); 
    }else{
      fpds->poststarIGR(fa,outfa);
    }
    if(dump){
      cout << "[Newton Compare] Dumping the output automaton in dot format to outfa.dot" << endl;
      fstream outfile("final_outfa.dot", fstream::out);
      outfa.print_dot(outfile, true);
      outfile.close();
    } if(dump){
      cout << "[Newton Compare] Dumping the output automaton to final_outfa.txt" << endl;
      fstream outfile("final_outfa.txt", fstream::out);
      outfa.print(outfile);
      outfile.close();
    }
  }
  
  sem_elem_t computePathSummary(WPDS * pds, WFA& outfa)
  {
    cout << "[Newton Compare] Checking error label reachability..." << endl;
    WpdsStackSymbols syms;
    pds->for_each(syms);

    WFA errfa;
    wali::Key fin = wali::getKeySpace()->getKey("accept");
    std::set<Key>::iterator it;
    errfa.addTrans(getPdsState(), getErrStk(pg), fin, outfa.getSomeWeight());
    for(it = syms.gamma.begin(); it != syms.gamma.end(); it++)
      errfa.addTrans(fin, *it, fin, outfa.getSomeWeight());
    errfa.setInitialState(getPdsState());
    errfa.addFinalState(fin);

    if(dump){
      cout << "[Newton Compare] Dumping the error automaton in dot format to errfa.dot" << endl;
      fstream outfile("errfa.dot", fstream::out);
      errfa.print_dot(outfile, true);
      outfile.close();
    }
    if(dump){
      cout << "[Newton Compare] Dumping the error automaton to errfa.txt" << endl;
      fstream outfile("errfa.txt", fstream::out);
      errfa.print(outfile);
      outfile.close();
    }


    WFA interfa;
    KeepLeft wmaker;
    interfa = outfa.intersect(wmaker, errfa);
    

    if(dump){
      cout << "[Newton Compare] Dumping the intersected automaton in dot format to intfa.dot" << endl;
      fstream outfile("intfa.dot", fstream::out);
      interfa.print_dot(outfile, true);
      outfile.close();
    }
    if(dump){
      cout << "[Newton Compare] Dumping the error automaton to intfa.txt" << endl;
      fstream outfile("intfa.txt", fstream::out);
      interfa.print(outfile);
      outfile.close();
    }


    cout << "[Newton Compare] Computing path summary..." << endl;
    interfa.path_summary_iterative_original(outfa.getSomeWeight()->one());

    return interfa.getState(interfa.getInitialState())->weight();
  }

  void runNwpds(WFA& outfa, FWPDS * originalPds = NULL)
  { 
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal III: end-to-end NWPDS run" << endl;
    FWPDS * npds;
    if(originalPds != NULL)
      npds = new FWPDS(*originalPds);
    else{
      npds = new FWPDS();
      con = pds_from_prog(npds, pg);
    }
    //npds->print(std::cout) << endl;
    wali::set_verify_fwpds(false);
    npds->useNewton(true);

#if defined(BINREL_STATS)
    con->resetStats(); 
#endif
    wali::util::Timer * t = new wali::util::Timer("NWPDS poststar",cout);
    t->measureAndReport =false;
    doPostStar(npds, outfa);
    sem_elem_t wt = computePathSummary(npds, outfa);
    if(npds->isOutputTensored())
      wt = boost::polymorphic_downcast<SemElemTensor*>(wt.get_ptr())->detensorTranspose().get_ptr();
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] NWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] NWPDS ==> error reachable" << endl;
    }
    t->print(std::cout << "[Newton Compare] Time taken by NWPDS poststar: ") << endl;
    delete t;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
    delete npds;
  }

  double runFwpds(WFA& outfa, FWPDS * originalPds = NULL)
  { 
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal IV: end-to-end FWPDS run" << endl;
    FWPDS * fpds;
    if(originalPds != NULL)
      fpds = new FWPDS(*originalPds);
    else{
      fpds = new FWPDS();
      //con = pds_from_prog_with_tensor_merge(fpds, pg);
      con = pds_from_prog(fpds, pg);
    }

    wali::set_verify_fwpds(false);
    fpds->useNewton(false);

#if defined(BINREL_STATS)
    con->resetStats(); 
#endif
    wali::util::GoodTimer * t = new wali::util::GoodTimer("FWPDS poststar");
    //t->measureAndReport =false;
    doPostStar(fpds, outfa);
    outfa.path_summary_iterative_original(outfa.getSomeWeight()->one());
    //sem_elem_t wt = computePathSummary(fpds, outfa);
    /*if(wt->equal(wt->zero()))
      cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] FWPDS ==> error reachable" << endl;
    }*/
    //t->print(std::cout << "[Newton Compare] Time taken by FWPDS poststar: ") << endl;
    //delete t;
    t->stop();
    double totTime = t->total_time();
    delete t;
    return totTime;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
    delete fpds;
    return totTime; 
  }

  double runEwpds(WFA& outfa, EWPDS * originalPds = NULL)
  {
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal VI: end-to-end EWPDS run" << endl;
    EWPDS * pds;
    if(originalPds != NULL)
      pds = new EWPDS(*originalPds);
    else{
      pds = new EWPDS();
      con = pds_from_prog(pds,pg); //_with_meet_merge(pds, pg);
    }

    //pds->print(std::cout) << endl;

#if defined(BINREL_STATS)
    con->resetStats();
#endif
    wali::util::GoodTimer * t = new wali::util::GoodTimer("EWPDS poststar");
    //t->measureAndReport =false;
    doPostStar(pds, outfa);
    outfa.path_summary_iterative_original(outfa.getSomeWeight()->one());
    //sem_elem_t wt = computePathSummary(pds, outfa);
    //wt->print(std::cout);
    /*if(wt->equal(wt->zero()))
      cout << "[Newton Compare] EWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] EWPDS ==> error reachable" << endl;
    }
    t->print(std::cout << "[Newton Compare] Time taken by EWPDS poststar: ") << endl;
    */
    //delete t;
    t->stop();
#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
    double totTime = t->total_time();
    cout << totTime;
    delete t;
    delete pds;
    return totTime;
  }


  void runWpds(WFA& outfa, EWPDS * originalPds = NULL)
  { 
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal VI: end-to-end WPDS run" << endl;
    EWPDS * pds;
    if(originalPds != NULL)
      pds = new EWPDS(*originalPds);
    else{
      pds = new EWPDS();
      con = pds_from_prog(pds, pg);
    }

    //pds->print(std::cout) << endl;

#if defined(BINREL_STATS)
    con->resetStats(); 
#endif
    wali::util::Timer * t = new wali::util::Timer("WPDS poststar",cout);
    t->measureAndReport =false;
    doPostStar(pds, outfa);
    sem_elem_t wt = computePathSummary(pds, outfa);
    wt->print(std::cout);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] WPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] WPDS ==> error reachable" << endl;
    }
    t->print(std::cout << "[Newton Compare] Time taken by WPDS poststar: ") << endl;
    delete t;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
    delete pds;
  }

  void runNewtonDirect(WFA& outfa)
  { 
    assert(mainProc && errLbl);
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal VI: end-to-end NewtonDirect run" << endl;
    NWPDS * npds = new NWPDS(dump);
    con = pds_from_prog_with_meet_merge(npds, pg);

#if defined(BINREL_STATS)
    con->resetStats(); 
#endif
    wali::util::Timer * t = new wali::util::Timer("NewtonDirect poststar",cout);
    t->measureAndReport = false;
    doPostStar(npds, outfa);
    sem_elem_t wt = computePathSummary(npds, outfa);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] NewtonDirect ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] NewtonDirect ==> error reachable" << endl;
    }
    t->print(std::cout << "[Newton Compare] Time taken by NewtonDirect poststar: ") << endl;
    delete t;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
    delete npds;
  }


  void createDifferentials(tslRegExpMap eMap, tslDiffMap & DMap){
    for (tslRegExpMap::iterator it = eMap.begin(); it!=eMap.end(); ++it)
    {
       RTG::regExpTListRefPtr rtList = CIR::TDifferential(it->second);
       DMap[it->first] = rtList;
    }
  }

  RTG::regExpTRefPtr convertToRegExpT(reg_exp_t exp, tslRegExpMap eMap, tslDiffMap DMap)
  {
    if(exp->isConstant())
    {
      NameWeight * nw = static_cast<wali::domains::name_weight::NameWeight*>(exp->get_weight().get_ptr());
      int a = nw->getName1();
      int b = nw->getName2();
      if (a == -1){
        if(b == -1){
	  return RTG::OneT::make();
	} else {
          RTG::regExpRefPtr d = eMap[b];
	  return RTG::TensorTranspose::make(RTG::One::make(),d);
	}
      }
      else {
	RTG::regExpTRefPtr d = CIR::getTFromRegList(DMap[b],a);
        return d;
      }
    }else if (exp->isUpdatable()){
      int node_no = exp->updatableNumber();
      return RTG::VarT::make(CBTI::INT32(node_no));
   } else if (exp->isStar()){
        list<reg_exp_t> children = exp->getChildren();
        RTG::regExpTRefPtr c = convertToRegExpT(children.front(), eMap, DMap);
        return RTG::KleeneT::make(c);
    } else if (exp->isExtend()){
        list<reg_exp_t> children = exp->getChildren();
        list<reg_exp_t>::iterator ch = children.begin();
        RTG::regExpTRefPtr lch = convertToRegExpT(*ch, eMap, DMap);
        ch++;
        RTG::regExpTRefPtr rch = convertToRegExpT(*ch, eMap, DMap);
        return RTG::DotT::make(lch,rch);
    } else if (exp->isCombine()){
        list<reg_exp_t> children = exp->getChildren();
        list<reg_exp_t>::iterator ch = children.begin();
        RTG::regExpTRefPtr lch = convertToRegExpT(*ch, eMap, DMap);
        ch++;
        RTG::regExpTRefPtr rch = convertToRegExpT(*ch, eMap, DMap);
        return RTG::PlusT::make(lch,rch);
    }
  }

  RTG::regExpRefPtr convertToRegExp(reg_exp_t exp, map<int,int> uMap, map<int,int> oMap)
  {
    if(exp->isConstant())
    {
      if(exp->isOne())
        return RTG::One::make();
      else if (exp->isZero())
        return RTG::Zero::make();
      else {
	binrel_t w = dynamic_cast<BinRel*>(exp->get_weight().get_ptr());
	BinRelInterface * bI = new BinRelInterface(w.get_ptr());
	EXTERN_TYPES::sem_elem_wrapperRefPtr * wt = new EXTERN_TYPES::sem_elem_wrapper(bI);
        return RTG::Weight::make(*wt);
      }
    } else if (exp->isUpdatable()){
        int node_no = exp->updatableNumber();
	int mNum = oMap[uMap[node_no]];
	return RTG::Var::make(CBTI::INT32(mNum));
    } else if (exp->isStar()){
        list<reg_exp_t> children = exp->getChildren();
        RTG::regExpRefPtr c = convertToRegExp(children.front(),uMap,oMap);
	return RTG::Kleene::make(c);
    } else if (exp->isExtend()){
        list<reg_exp_t> children = exp->getChildren();
	list<reg_exp_t>::iterator ch = children.begin();
	RTG::regExpRefPtr lch = convertToRegExp(*ch, uMap,oMap);
	ch++;
	RTG::regExpRefPtr rch = convertToRegExp(*ch, uMap,oMap);
	return RTG::Dot::make(lch,rch);
    } else if (exp->isCombine()){
        list<reg_exp_t> children = exp->getChildren();
        list<reg_exp_t>::iterator ch = children.begin();
        RTG::regExpRefPtr lch = convertToRegExp(*ch, uMap,oMap);
        ch++;
        RTG::regExpRefPtr rch = convertToRegExp(*ch, uMap,oMap);
        return RTG::Plus::make(lch,rch);
    }
  }


  void convertToTSLRegExps(std::pair<std::map<int,reg_exp_t>,std::map<int,int> > rPair, std::map<int,int> oMap, tslRegExpMap & EMap)
  {
    std::map<int,reg_exp_t> rList = rPair.first;
    std::map<int,int> uMap = rPair.second;
    for (std::map<int,reg_exp_t>::iterator it=rList.begin(); it!=rList.end(); ++it)
    {
      RTG::regExpRefPtr rExp = convertToRegExp(it->second, uMap, oMap);
      EMap[it->first] = rExp;
    }
  }

  void convertToTSLRegExpsT(std::map<int,reg_exp_t> rList, tslDiffMap DMap, tslRegExpMap EMap, tslRegExpTMap & MMap)
  {
    for (std::map<int,reg_exp_t>::iterator it=rList.begin(); it!=rList.end(); ++it)
    {
      if (it->first != -1){
        RTG::regExpTRefPtr rExp = convertToRegExpT(it->second, EMap, DMap);
        MMap[it->first] = rExp;
      }
    }
  }

  void printTSLRegExps(tslRegExpMap rMap)
  {
    for (tslRegExpMap::iterator it = rMap.begin(); it!=rMap.end(); ++it)
    {
      cout << it->first << ": ";
      it->second.print(cout);
      cout << "\n";
    }
  }

  void printTSLDifferentials(tslDiffMap dMap)
  {
    for (tslDiffMap::iterator it = dMap.begin(); it!=dMap.end(); ++it)
    {
      cout << it->first << ": ";
      it->second.print(cout);
      cout << "\n";
    }
  }

  void printTSLTExp(tslRegExpTMap tMap)
  {
    for (tslRegExpTMap::iterator it = tMap.begin(); it!=tMap.end(); ++it)
    {
      cout << it->first << ": ";
      it->second.print(cout);
      cout << "\n";
    }
  }


  set<int> getVarList(RTG::regExpTListRefPtr tList)
  {
    set<int> srcList;
    int listSize = CIR::getTListLength(tList).get_data();
    for(int i = 0; i < listSize; i++)
    {
      int varNum = CIR::getVarNum(tList, i).get_data();;
      srcList.insert(varNum);
    }
    return srcList;
  }

  static wali::Key st1()
  {
    return getKey("Unique State Name");
  }

  static wali::Key stk(int k)
  {
    return getKey(k);
  }

  void fwpdsFromDifferential(FWPDS * pds, tslDiffMap DMap)
  {
    for(tslDiffMap::iterator it = DMap.begin(); it!=DMap.end(); ++it)
    {
      int dummy = -1;
      int tgt = it->first;
      set<int> srcList = getVarList(it->second);
      for(set<int>::iterator i = srcList.begin(); i != srcList.end(); i++)
      {
        int src = *i;
        pds->add_rule(st1(), stk(src),st1(),stk(tgt), new NameWeight(src,tgt));
      }
      pds->add_rule(st1(), stk(dummy), st1(), stk(tgt), new NameWeight(dummy,tgt));
    }

  }

  void runNewton(tslRegExpTMap Mmap, tslDiffMap D, RTG::assignmentRefPtr & aList)
  {
    bool newton = true;
    std::map<int,std::set<int> > dependenceMap;
    std::set<int> retPtSet; //C'
    std::set<int> C;
    RTG::assignmentRefPtr aPrime = aList;
    tslDiffMap::iterator mIt;
    //C' = ProgRetPts
    //Populate dependence map
    for(mIt = D.begin(); mIt != D.end(); mIt++)
    {
      std::set<int> dInt = getVarList(mIt->second);
      dependenceMap[mIt->first] = dInt;
      retPtSet.insert(mIt->first);
    }
    bool first = true;
    //While C' != empty
    while(retPtSet.size() != 0){
      std::set<int>::iterator pIt;
      //aPrime = A
      for(pIt = retPtSet.begin(); pIt != retPtSet.end(); pIt++)
      {
        aPrime = CIR::updateAssignment(aPrime,CBTI_INT32(*pIt),CIR::getAssignment(CBTI_INT32(*pIt),aList));
      }
      //C = C'
      C = retPtSet;
      //C' = emptyset
      retPtSet.clear();
      tslRegExpTMap::iterator agnIt;
      //For each p in ProgRetPt
      for(agnIt = Mmap.begin(); agnIt != Mmap.end(); agnIt++)
      {
        int var = agnIt->first;
	std::set<int> varsUsedInP = dependenceMap[var];
	bool checkIn = false;
	std::set<int>::iterator sIt1 = C.begin();
	std::set<int>::iterator sIt2 = varsUsedInP.begin();
	//Check to see if C intersect VarsUsedIn(p) is empty
	if(!first){
	  while((sIt1 != C.end()) && (sIt2 != varsUsedInP.end()) && !checkIn)
	  {
	    if(*sIt1<*sIt2) 
	      ++sIt1;
	    else if(*sIt2<*sIt1) 
	      ++sIt2;
	    else
	      checkIn = true;
	  }
	}
	else
	{
	  checkIn = true;
	}
	if(checkIn){
	  //v = Tdetensor(evalT(Mmap[p],aPrime
	  EXTERN_TYPES::sem_elem_wrapperRefPtr newVal = CIR::evalT(agnIt->second, aPrime); 
	  EXTERN_TYPES::sem_elem_wrapperRefPtr rep = EXTERNS::detensorTranspose(newVal);
	  //if A'[p] != v`
	  EXTERN_TYPES::sem_elem_wrapperRefPtr oldVal = CIR::getAssignment(CBTI_INT32(var),aPrime);
	  if(!(oldVal.v->n->Equal(rep.v->n))){
	    retPtSet.insert(var); //C' = C' union {p}
	    //A[p] = v
	    aList = CIR::updateAssignment(aList, CBTI_INT32(var), rep);
	  }
	}
      }
      first = false;
      RTG::_evalRegExpHash().clear();
    }
  }

  void evalRegExps(tslRegExpMap reMap, RTG::assignmentRefPtr aList, map<int,BinRelInterface*> & finWeights)
  {
    for(tslRegExpMap::iterator it = reMap.begin(); it != reMap.end(); it++)
    {
      EXTERN_TYPES::sem_elem_wrapperRefPtr w = CIR::evalRegExp(it->second,aList);
      finWeights[it->first] = w.v;
    }
  }

  double run_newton_merge_notensor_fwpds(WFA& outfa, FWPDS * originalPds = NULL)
  { 
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal VIII: end-to-end newton_merge_notensor_fwpds run" << endl;
    FWPDS * fpds;
    if(originalPds != NULL)
      fpds = new FWPDS(*originalPds);
    else{
      fpds = new FWPDS();
      //con = pds_from_prog_with_tensor_merge(fpds, pg);
      con = pds_from_prog(fpds, pg);
    }

    wali::set_verify_fwpds(false);
    
    fpds->useNewton(false);
    //fpds->add_rule(st1(),getKey(mainProc),st1(),fpds->get_theZero()->one());

    wali::util::GoodTimer * t = new wali::util::GoodTimer("temp");
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, fpds->get_theZero()->one());
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);

    int dummy = -1;
    map<int,reg_exp_t> r2Map;
    map<int,int> uMap;
    map<int,int> oMap;
    map<int,pair< pair<int,int>,int> > mapBack;
    vector<int> eps;
    t->stop();
    double t1 = fpds->getOutRegExps(fa,outfa, r2Map, uMap, oMap, mapBack, eps, true);
    tslRegExpMap E;
    convertToTSLRegExps(std::make_pair(r2Map,uMap),oMap,E);
    tslRegExpMap epsMap;
    for(vector<int>::iterator eit = eps.begin(); eit != eps.end(); eit++)
    {
      epsMap[(*eit)] = E[(*eit)];
      r2Map[(*eit)]->print(cout);
      cout << "\n";
#if defined(NEWTON_DEBUG)
      int src = mapBack[(*eit)].first.first;
      int tgt = mapBack[(*eit)].first.second;
      int stack = mapBack[(*eit)].second;
      globalEpsilon.insert(std::make_pair(std::make_pair(src,tgt),stack));
#endif
    }
    printTSLRegExps(epsMap);
    tslDiffMap D;
    t->start();
    createDifferentials(epsMap,D);
    FWPDS * fnew = new FWPDS();
    fwpdsFromDifferential(fnew, D);
    
    WFA fa2;
    wali::Key acc2 = wali::getKeySpace()->getKey("accept2");
    fa2.addTrans(getPdsState(),stk(dummy), acc2, fnew->get_theZero()->one());
    fa2.setInitialState(getPdsState());
    fa2.addFinalState(acc2);
    WFA outfa2;
    map<int,reg_exp_t> rNew;
    map<int,int> uNew;
    map<int,int> oNew;
    map<int,pair<pair<int,int>,int> > B2;
    t->stop();
    vector<int> eNew;
    double t2 = fnew->getOutRegExps(fa2,outfa2,rNew,uNew,oNew,B2,eNew,false);
    tslRegExpTMap M;
    convertToTSLRegExpsT(rNew,D,E,M);
    t->start();
    RTG::assignmentRefPtr aList = CIR::initializeAssignment();
    runNewton(M,D,aList);
    map<int,BinRelInterface*> finWeights;
    evalRegExps(E,aList,finWeights);

    for(map<int,pair<pair<int,int>,int> >::iterator mbit = mapBack.begin(); mbit != mapBack.end(); mbit++)
    {
      int src = mbit->second.first.first;
      int tgt = mbit->second.first.second;
      int stck = mbit->second.second;
      wali::wfa::ITrans * t = outfa.find2(src,stck,tgt);
      BinRelInterface * w = finWeights[mbit->first];
      t->setWeight(w->n);
    }

    outfa.path_summary_iterative_original(outfa.getSomeWeight()->one());

    t->stop();
   
    double tTime = t->total_time()+t1+t2;
    delete t;
    delete fpds;
    return tTime;
#if defined(BINREL_STATS)
    //con->resetStats(); 
#endif
    //wali::util::Timer * t = new wali::util::Timer("newton_merge_notensor_fwpds poststar",cout);
    //t->measureAndReport =false;
    //doPostStar(fpds, outfa);
    //fpds->poststarIGR(fa,outfa);
    //fpds->getOutRegExps(fa,outfa);
    /*sem_elem_t wt = computePathSummary(fpds, outfa);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] newton_merge_notensor_fwpds ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] newton_merge_notensor_fwpds ==> error reachable" << endl;
    }*/
    //t->print(std::cout << "[Newton Compare] Time taken by newton_merge_notensor_fwpds poststar: ") << endl;
    //delete t;

#if defined(BINREL_STATS)
    //con->printStats(cout);
#endif //if defined(BINREL_STATS)
    //delete fpds;
  }

  void run_newton_merge_tensor_fwpds(WFA& outfa, FWPDS * originalPds = NULL)
  {
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal IX: end-to-end newton_merge_tensor_fwpds run" << endl;
    FWPDS * fpds;
    if(originalPds != NULL)
      fpds = new FWPDS(*originalPds);
    else{
      fpds = new FWPDS();
      con = pds_from_prog_with_tensor_merge(fpds, pg);
    }

    wali::set_verify_fwpds(false);
    fpds->useNewton(true);

#if defined(BINREL_STATS)
    con->resetStats();
#endif
    wali::util::Timer * t = new wali::util::Timer("newton_merge_tensor_fwpds poststar",cout);
    t->measureAndReport =false;
    doPostStar(fpds, outfa);
    sem_elem_t wt = computePathSummary(fpds, outfa);
    if(fpds->isOutputTensored())
      wt = boost::polymorphic_downcast<SemElemTensor*>(wt.get_ptr())->detensorTranspose().get_ptr();
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] newton_merge_tensor_fwpds ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] newton_merge_tensor_fwpds ==> error reachable" << endl;
    }
    t->print(std::cout << "[Newton Compare] Time taken by newton_merge_tensor_fwpds poststar: ") << endl;
    delete t;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
    delete fpds;
  }


  void compareEwpdsFwpdsNewton()
  {
    FWPDS * originalPds = new FWPDS();
    con = pds_from_prog(originalPds, pg);
    WpdsStackSymbols syms;
    originalPds->for_each(syms);


    wali::util::GoodTimer * eTimer = new wali::util::GoodTimer("EWPDS time");
    WFA outfaEwpds;
    double cEtime = runEwpds(outfaEwpds, originalPds);
    eTimer->stop();

    wali::util::GoodTimer * fTimer = new wali::util::GoodTimer("EWPDS time");
    WFA outfaFwpds;
    double cFtime = runFwpds(outfaFwpds, originalPds);
    fTimer->stop();

    wali::util::GoodTimer * nTimer = new wali::util::GoodTimer("Newton time");
    WFA outfaNewton;
    double cNtime = 0;
    run_newton_merge_notensor_fwpds(outfaNewton, originalPds);
    nTimer->stop();

#if defined(NEWTON_DEBUG)
    for(std::set<std::pair<std::pair<int,int>,int> >::iterator git = globalEpsilon.begin(); git != globalEpsilon.end(); git++)
    {
      cout << "tete";
      int src = git->first.first;
      int tgt = git->first.second;
      int stack = git->second;

      wali::wfa::Trans tE;
      wali::wfa::Trans tF;
      wali::wfa::Trans tN;

      outfaEwpds.find(src,stack,tgt,tE);
      outfaFwpds.find(src,stack,tgt,tF);
      outfaNewton.find(src,stack,tgt,tN);

      sem_elem_t wtE = tE.weight();
      sem_elem_t wtF = tF.weight();
      sem_elem_t wtN = tN.weight();

      if(!wtE->equal(wtN))
      {
        cout << "EWPDS Weight: \n";
	wtE->print(cout);
	cout << "\nNewton Weight: \n";
	wtN->print(cout);

	cout << "Src: " << key2str(src) << " Target: " << key2str(tgt) << " Stack: " << key2str(stack) << "\n";
      }

    }
#endif

    nTimer->start();
    std::map<wali::Key, sem_elem_t> newtonJoinMap = outfaNewton.readOutCombineOverAllPathsValues(syms.gamma);
    nTimer->stop();

    eTimer->start();
    std::map<wali::Key, sem_elem_t> ewpdsJoinMap = outfaEwpds.readOutCombineOverAllPathsValues(syms.gamma);
    eTimer->stop();

    fTimer->start();
    std::map<wali::Key, sem_elem_t> fwpdsJoinMap = outfaFwpds.readOutCombineOverAllPathsValues(syms.gamma);
    fTimer->stop();


    delete originalPds;
    
    bool b1 = false;
    bool b2 = false;
    bool b3 = false;
    for(std::set<Key>::iterator ait = syms.gamma.begin(); ait != syms.gamma.end(); ait++)
    {
      sem_elem_t eval = ewpdsJoinMap[*ait];
      sem_elem_t fval = fwpdsJoinMap[*ait];
      sem_elem_t nval = newtonJoinMap[*ait];
      if(!eval->equal(fval))
      {
        b1 = true;
      }
      if(!eval->equal(nval))
      {
        b2 = true; 
      }
      if(!fval->equal(nval))
      {
        b3 = true; 
      }
    }

    if(b1)
    {
      cout << "ERROR: Ewpds and Fwpds don't match\n";
    }
    if(b2)
    {
      cout << "ERROR: Ewpds and Newton don't match\n";
    }
    if(b3)
    {
      cout << "ERROR: Newton and Fwpds don't match\n";
    }

    double totalEtime = eTimer->total_time();
    totalEtime += cEtime;
    double totalFtime = fTimer->total_time();
    totalFtime += cFtime;
    double totalNtime = nTimer->total_time();
    totalNtime += cNtime;

    cout << "EWPDS TIME: " << totalEtime << "\n";
    cout << "FWPDS TIME: " << totalFtime << "\n";
    cout << "Newton Time: " << totalNtime << "\n";
    
  }

  void compareWpdsNwpds()
  { 
    FWPDS * originalPds = new FWPDS();
    con = pds_from_prog_with_meet_merge(originalPds, pg);
    WFACompare fac("WPDS", "NWPDS");
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal I: Check Correctness by comparing the result automaton: WPDS vs NWPDS" << endl;
    
    {
      WFA outfa;
      runWpds(outfa, originalPds);
      outfa.for_each(fac);    
      if(dump){
        cout << "[Newton Compare] Dumping the result automaton for WPDS..." << endl;
        fstream out_fa_stream("wpds_out_fa.dot", fstream::out);
        TransDotty td(out_fa_stream,false, NULL);
        out_fa_stream << "digraph{" << endl;
        outfa.for_each(td);
        out_fa_stream << "}" << endl;
      }
      fac.advance_mode();
    }
    {
      WFA outfa;
      runNwpds(outfa, originalPds);
      outfa.for_each(fac);    
      if(dump){
        cout << "[Newton Compare] Dumping the result automaton for NWPDS..." << endl;
        fstream out_fa_stream("nwpds_out_fa.dot", fstream::out);
        TransDotty td(out_fa_stream,false, NULL);
        out_fa_stream << "digraph{" << endl;
        outfa.for_each(td);
        out_fa_stream << "}" << endl;
      }
      fac.advance_mode();
    }
    {
      fstream fadiff("fa_diff",fstream::out);
      if(dump){
        fac.diff(&fadiff);
      }else{
        bool fadifffound = fac.diff();
        if(fadifffound)
          fadiff << "FA DIFF FOUND!!!" << std::endl;
      }
    }
    delete originalPds;
  }
}

using namespace goals;

static short goal;

void * work(void *)
{
  int dump;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &dump);

  WFA outfa;
  switch(goal){
    case 1:
      compareEwpdsFwpdsNewton();
      break;
    case 2:
      assert(0);
      break;
    case 3:
      runNwpds(outfa);
      break;
    case 4:
      runFwpds(outfa);
      break;
    case 5:
      runNewtonDirect(outfa);
      break;
    case 6:
      runWpds(outfa);
      break;
    case 7:
      runEwpds(outfa);
      break;
    case 8:
      run_newton_merge_notensor_fwpds(outfa);
      break;
    case 9:
      run_newton_merge_tensor_fwpds(outfa);
      break;
    default:
      assert(0 && "I don't understand that goal!!!");
  }
  return NULL;
}

int main(int argc, char ** argv)
{
  // register signal handler
  signal(SIGHUP, handle_sighup);

  if(argc < 3){
    cerr 
      << "Usage: " << endl
      << "./NewtonFwpdsCompare input_file goal(1/2) [<0/1> dump] [entry function (default:main)] [error label (default:error)]" << endl
      << "Goal: 1 --> Compare WPDS & NWPDS. Compute poststar and compare the output automata." << endl
      << "Goal: 2 [NOW DEFUNCT]--> Compare FWPDS & NWPDS. Compute poststar and check if any assertion can fail." << endl
      << "Goal: 3 --> Run newton_nomerge_tensor_fwpds end-to-end." << endl
      << "Goal: 4 --> Run kleene_merge_fwpds end-to-end." << endl 
      << "Goal: 5 --> Run newton_merge_notensor_ewpds end-to-end." << endl
      << "Goal: 6 --> Run kleene_nomerge_wpds end-to-end." << endl
      << "Goal: 7 --> Run kleene_merge_ewpds end-to-end." << endl
      << "Goal: 8 --> Run newton_merge_notensor_fwpds end-to-end." << endl
      << "Goal: 9 --> Run newton_merge_tensor_fwpds end-to-end." << endl;
    return -1;
  }

  int curarg = 2;
  if(argc >= curarg){
    stringstream s;
    s << argv[curarg-1];
    fname = s.str();
  }

  ++curarg;
  if(argc >= curarg){
    stringstream s;
    s << argv[curarg-1];
    s >> goal;
  }

  ++curarg;
  if(argc >= curarg){
    stringstream s;
    s << argv[curarg-1];
    s >> dump;
  }

  ++curarg;
  if(argc >= curarg){
    stringstream s;
    delete mainProc;
    mainProc = argv[curarg-1];
  }

  ++curarg;
  if(argc >= curarg){
    stringstream s;
    delete errLbl;
    errLbl = argv[curarg-1];
  }

  cout << "Verbosity Level: " << dump << std::endl;
  cout << "Goal #: " << goal << std::endl;

  cout << "[NewtonCompare] Parsing Program..." << endl;
  pg = parse_prog(fname.c_str());
  //BPCFG::ICFG * icfg = BPCFG::createICFG(pg);
  //BPCFG::ICFGtoBP(icfg);

  if(!mainProc) mainProc = strdup("main");
  if(!errLbl) errLbl = strdup("error");
  cout << "[Newton Compare] Post processing parsed program... " << endl;
  make_void_returns_explicit(pg);
  remove_skip(pg);
  instrument_enforce(pg);
  instrument_asserts(pg, errLbl);
  instrument_call_return(pg);

  if(dump){
    FWPDS * originalPds = new FWPDS();
    con = pds_from_prog_with_meet_merge(originalPds, pg);
    cout << "[Newton Compare] Dumping PDS to pds.dot..." << endl;
    fstream pds_stream("pds.dot", fstream::out);
    RuleDotty rd(pds_stream);
    pds_stream << "digraph{" << endl;
    originalPds->for_each(rd);
    pds_stream << "}" << endl;
    delete(originalPds);
  }

  //void * dump; 
  //pthread_create(&worker, NULL, &work,  NULL);
  work(NULL);
  //pthread_join(worker, &dump);

  bdd_printstat();
  // Clean Up.
  deep_erase_prog(&pg);
  return 0;
}
