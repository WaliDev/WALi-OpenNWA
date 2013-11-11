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
  
  short dump = false;
  char * mainProc = NULL, * errLbl = NULL;
  string fname;
  prog * pg;
  BddContext * con = NULL;

  
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

  void runFwpds(WFA& outfa, FWPDS * originalPds = NULL)
  { 
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal IV: end-to-end FWPDS run" << endl;
    FWPDS * fpds;
    if(originalPds != NULL)
      fpds = new FWPDS(*originalPds);
    else{
      fpds = new FWPDS();
      //con = pds_from_prog_with_tensor_merge(fpds, pg);
      con = pds_from_prog_with_meet_merge(fpds, pg);
    }

    wali::set_verify_fwpds(false);
    fpds->useNewton(false);

#if defined(BINREL_STATS)
    con->resetStats(); 
#endif
    wali::util::Timer * t = new wali::util::Timer("FWPDS poststar",cout);
    t->measureAndReport =false;
    doPostStar(fpds, outfa);
    sem_elem_t wt = computePathSummary(fpds, outfa);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] FWPDS ==> error reachable" << endl;
    }
    t->print(std::cout << "[Newton Compare] Time taken by FWPDS poststar: ") << endl;
    delete t;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
    delete fpds;
  }

  void runEwpds(WFA& outfa, EWPDS * originalPds = NULL)
  {
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal VI: end-to-end EWPDS run" << endl;
    EWPDS * pds;
    if(originalPds != NULL)
      pds = new EWPDS(*originalPds);
    else{
      pds = new EWPDS();
      con = pds_from_prog_with_meet_merge(pds, pg);
    }

    //pds->print(std::cout) << endl;

#if defined(BINREL_STATS)
    con->resetStats();
#endif
    wali::util::Timer * t = new wali::util::Timer("EWPDS poststar",cout);
    t->measureAndReport =false;
    doPostStar(pds, outfa);
    sem_elem_t wt = computePathSummary(pds, outfa);
    //wt->print(std::cout);

    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] EWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] EWPDS ==> error reachable" << endl;
    }
    t->print(std::cout << "[Newton Compare] Time taken by EWPDS poststar: ") << endl;
    delete t;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
    delete pds;
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
      RTG::regExpTRefPtr d = RTG::OneT::make(); //CIR::getTRegFromList(DMapp[b],a);
      return RTG::PlusT::make(d, RTG::TensorTranspose::make(RTG::One::make(),eMap[a]));
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
        RTG::regExpTRefPtr rch = convertToRegExpT(*ch, eMap, DMap);
        ch++;
        RTG::regExpTRefPtr lch = convertToRegExpT(*ch, eMap, DMap);
        return RTG::DotT::make(lch,rch);
    } else if (exp->isCombine()){
        list<reg_exp_t> children = exp->getChildren();
        list<reg_exp_t>::iterator ch = children.begin();
        RTG::regExpTRefPtr rch = convertToRegExpT(*ch, eMap, DMap);
        ch++;
        RTG::regExpTRefPtr lch = convertToRegExpT(*ch, eMap, DMap);
        return RTG::PlusT::make(lch,rch);
    }
  }

  RTG::regExpRefPtr convertToRegExp(reg_exp_t exp)
  {
    if(exp->isConstant())
    {
      if(exp->isOne())
        return RTG::One::make();
      else if (exp->isZero())
        return RTG::Zero::make();
      else {
	sem_elem_t w = exp->get_weight();
	CONC_EXTERN_TYPES::sem_elem_wrapperRefPtr * wt = new CONC_EXTERN_TYPES::sem_elem_wrapper(w);
        return RTG::Weight::make(*wt);
      }
    } else if (exp->isUpdatable()){
        int node_no = exp->updatableNumber();
        return RTG::Var::make(CBTI::INT32(node_no));
    } else if (exp->isStar()){
        list<reg_exp_t> children = exp->getChildren();
        RTG::regExpRefPtr c = convertToRegExp(children.front());
	return RTG::Kleene::make(c);
    } else if (exp->isExtend()){
        list<reg_exp_t> children = exp->getChildren();
	list<reg_exp_t>::iterator ch = children.begin();
	RTG::regExpRefPtr rch = convertToRegExp(*ch);
	ch++;
	RTG::regExpRefPtr lch = convertToRegExp(*ch);
	return RTG::Dot::make(lch,rch);
    } else if (exp->isCombine()){
        list<reg_exp_t> children = exp->getChildren();
        list<reg_exp_t>::iterator ch = children.begin();
        RTG::regExpRefPtr rch = convertToRegExp(*ch);
        ch++;
        RTG::regExpRefPtr lch = convertToRegExp(*ch);
        return RTG::Plus::make(lch,rch);
    }
  }


  void convertToTSLRegExps(std::map<int,reg_exp_t> rList, tslRegExpMap & EMap)
  {
    for (std::map<int,reg_exp_t>::iterator it=rList.begin(); it!=rList.end(); ++it)
    {
      RTG::regExpRefPtr rExp = convertToRegExp(it->second);
      EMap[it->first] = rExp;
    }
  }

  void convertToTSLRegExpsT(std::map<int,reg_exp_t> rList, tslDiffMap DMap, tslRegExpMap EMap, tslRegExpTMap & MMap)
  {
    for (std::map<int,reg_exp_t>::iterator it=rList.begin(); it!=rList.end(); ++it)
    {
      RTG::regExpTRefPtr rExp = convertToRegExpT(it->second, EMap, DMap);
      MMap[it->first] = rExp;
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

  vector<int> getVarList(RTG::regExpTListRefPtr tList)
  {
    vector<int> srcList;
    int listSize = 0; // CIR::getTListLength(tList);
    for (int i = 0; i < listSize; i++)
    {
      int varNum = 0; //CIR::getVar(tList, i);
      srcList.push_back(varNum);
    }
    return srcList;
  }

  static wali::Key stt()
  {
    return getKey("Unique State Name 2");
  }

  static wali::Key stk(int k)
  {
    return getKey(k);
  }

  void fwpdsFromDifferential(FWPDS * pds, tslDiffMap DMap)
  {
    for(tslDiffMap::iterator it = DMap.begin(); it!=DMap.end(); ++it)
    {
      int dummy;
      int tgt = it->first;
      vector<int> srcList = getVarList(it->second);
      for(int i = 0; i != srcList.size(); i++)
      {
        int src = srcList.at(i);
        pds->add_rule(stt(), stk(src),stt(),stk(tgt), new NameWeight(src,tgt));
      }
      pds->add_rule(stt(), stk(dummy), stt(), stk(tgt), new NameWeight(dummy,tgt));
    }

  }

  void runNewton(tslRegExpTMap Mmap, RTG::assignmentListRefPtr aList)
  {
    bool newton = true;
    while(newton == true){
      RTG::assignmentListRefPtr aOld = aList;
      tslRegExpTMap::iterator agnIt;
      for(agnIt = Mmap.begin(); agnIt != Mmap.end(); agnIt++)
      {
        int var = agnIt->first;
        CONC_EXTERN_TYPES::sem_elem_wrapperRefPtr newVal = CIR::evalT(agnIt->second, aOld);
        //aList = CIR::updateAssignmentList(aList, CBTI_INT32(var), newVal);
      }
      RTG::_evalRegExpHash().clear();
      //if(CIR::checkEqual(aOld,aList))
      //{
        newton = false;
      //}
    }
  }

  void run_newton_merge_notensor_fwpds(WFA& outfa, FWPDS * originalPds = NULL)
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
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, fpds->get_theZero()->one());
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);

    int dummy = -1;
    cout << "Step 0 \n";
    map<int,reg_exp_t> rMap = fpds->getOutRegExps(fa,outfa,true);
    cout << "Step 1 \n";
    vector<reg_exp_t>::iterator ait;
    tslRegExpMap E;
    convertToTSLRegExps(rMap,E);
    printTSLRegExps(E);
    cout << "Step 2 \n";
    tslDiffMap D;
    createDifferentials(E,D);
    FWPDS * fnew = new FWPDS();
    fwpdsFromDifferential(fnew, D);
    WFA fa2;
    wali::Key acc2 = wali::getKeySpace()->getKey("accept2");
    fa2.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc2, fnew->get_theZero()->one());
    fa2.setInitialState(getPdsState());
    fa2.addFinalState(acc2);
    WFA outfa2;
    map<int,reg_exp_t> rNew = fnew->getOutRegExps(fa2,outfa2,false);
    tslRegExpTMap M;
    convertToTSLRegExpsT(rNew,D,E,M);
    RTG::assignmentListRefPtr aList = RTG::aListNull::make();
    for (tslRegExpTMap::iterator mIt = M.begin(); mIt != M.end(); mIt++)
    {
      //RTG::assignmentListRefPtr aList = initAssignmentList(CBTI::INT32(mIt->first),aList);
    }
    runNewton(M,aList);

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
      compareWpdsNwpds();
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
