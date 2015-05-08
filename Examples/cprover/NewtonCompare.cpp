#include "src/lang/gtr_config.h"
#include "tsl/cir/regexp/conc.1level.cir.hpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_extern_types.hpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_externs.hpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_extern_types.cpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_externs.cpp"
#include "tsl/analysis_components/src/reinterps/emul/concrete_base_type_interp.hpp"
#include "tsl/analysis_components/src/reinterps/emul/concrete_base_type_interp.cpp"
#include "tsl/cir/regexp/conc.1level.cir.cpp"
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
#include "wali/domains/binrel/BinRel.hpp"
#include "../turetsky/svn-repository/NWAOBDDRel.hpp"
#include "NewtonCompare.hpp"
#include "Newton_Compare_NWA.hpp"

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
using namespace wali::domains::nwaobddrel;
using namespace tsl_regexp;

typedef wali::ref_ptr<graph::RegExp> reg_exp_t;
typedef tsl_regexp::Conc1LevelRTG RTG;
typedef tsl_regexp::Conc1LevelCIR CIR;
typedef std::map<int,RTG::regExpRefPtr> tslRegExpMap;
typedef std::map<int,RTG::regExpTRefPtr> tslRegExpTMap;
typedef std::map<int,RTG::regExpTListRefPtr> tslDiffMap;

//#include <signal.h>
#include <boost/cast.hpp>

/*
static pthread_t worker;

extern "C" 
*/

BddContext * con = NULL;
NWAOBDDContext * ncon = NULL;

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
  
  std::set<pair<std::pair<int,int>,int> > globalEpsilon;  //used for debugging Newton
  short dump = false;
  char * mainProc = NULL, * errLbl = NULL;
  string fname;
  prog * pg;
  

  //Calls postar on the pds and wfa, also prints out .dot and .txt files with the initial wfa and final wfa.
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

	//Calls the poststar functions
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
  
  //Computes the path summary to determin if the error label is reachable
  sem_elem_t computePathSummary(WPDS * pds, WFA& outfa)
  {
    cout << "[Newton Compare] Checking error label reachability..." << endl;
    WpdsStackSymbols syms;
    pds->for_each(syms);

	//Creates an errorwfa, that accepts if the error transition is seen and then has a loop to the accept state
	//for all other transitions.
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

	//Creates the interfa, which intersects the given and the error wfa
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

	//If there is a path through the interfa, then there is a legal assignment of variables in the program that reaches
	//an error state.  The weight returned represents that assignment.
    cout << "[Newton Compare] Computing path summary..." << endl;
    interfa.path_summary_iterative_original(outfa.getSomeWeight()->one());

    return interfa.getState(interfa.getInitialState())->weight();
  }

  /*
  *  Determines if an error state in 'pg' is reachable using an npds.
  *
  *  Creates an npds representing the program with bdds as weights.  Runs a newton solver for WPDS queries, 
  *  using EWPDS solvers for the genearted linear equations. The correctness of this solver
  *  is uncertain
  */
  void runNwpds(WFA& outfa, FWPDS * originalPds = NULL)
  { 

    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal III: end-to-end NWPDS run" << endl;
    NWPDS * npds;
    if(originalPds != NULL)
      npds = new NWPDS(false);
    else{
      npds = new NWPDS();
	  con = pds_from_prog(npds, pg);
    }
	freopen("nwaobddOut.txt", "w", stdout);
    npds->print(std::cout) << endl;
    wali::set_verify_fwpds(false);

#if defined(BINREL_STATS)
    con->resetStats(); 
#endif
    wali::util::Timer * t = new wali::util::Timer("NWPDS poststar",cout);
    t->measureAndReport =false;
    doPostStar(npds, outfa);
    sem_elem_t wt = computePathSummary(npds, outfa);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] NWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] NWPDS ==> error reachable" << endl;
    }
    t->print(std::cout << "[Newton Compare] Time taken by NWPDS poststar: ") << endl;
    delete t;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif
  }

  /*
  *  Determines if an error state in 'pg' is reachable using an npds.
  *
  *  Creates an fpds representing the program with bdds (or nwaobdds) as weights.  Runs poststar and computes a
  *  path summary to find the results.
  */
  double runFwpds(WFA& outfa, FWPDS * originalPds = NULL)
  { 
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal IV: end-to-end FWPDS run" << endl;
    FWPDS * fpds;
    if(originalPds != NULL)
      fpds = new FWPDS(*originalPds);
    else{
      fpds = new FWPDS();
      //con = pds_from_prog(fpds, pg);
	  ncon = pds_from_prog_nwa(fpds, pg);
    }

	//freopen("nwaobddOut.txt", "w", stdout);
	//fpds->print(std::cout) << endl;
    wali::set_verify_fwpds(false);
    fpds->useNewton(false);

#if defined(BINREL_STATS)
  //  con->resetStats(); 
#endif
    wali::util::GoodTimer * t = new wali::util::GoodTimer("FWPDS poststar");
    //t->measureAndReport =false;
    doPostStar(fpds, outfa);
    //outfa.path_summary_iterative_original(outfa.getSomeWeight()->one());
    sem_elem_t wt = computePathSummary(fpds, outfa);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] FWPDS ==> error reachable" << endl;
    }
	t->stop();
	double totTime = t->total_time();
	std::cout << "[Newton Compare] Time taken by FWPDS poststar: fwpds: ";
	cout << totTime << endl;
    delete t;
    /*t->stop();
    double totTime = t->total_time();
    delete t;
    return totTime;*/

#if defined(BINREL_STATS)
 //   con->printStats(cout);
#endif //if defined(BINREL_STATS)
    //delete fpds;
	return 0;//return totTime; 
  }
  
  /*
  *  Determines if an error state in 'pg' is reachable using an epds.
  *
  *  Creates an fpds representing the program with bdds as weights.  Runs poststar and computes a
  *  path summary to find the results.
  */
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
   // con->resetStats();
#endif
    wali::util::GoodTimer * t = new wali::util::GoodTimer("EWPDS poststar");
    //t->measureAndReport =false;
    doPostStar(pds, outfa);
    //outfa.path_summary_iterative_original(outfa.getSomeWeight()->one());
    sem_elem_t wt = computePathSummary(pds, outfa);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] EWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] EWPDS ==> error reachable" << endl;
    }
    //t->print(std::cout << "[Newton Compare] Time taken by EWPDS poststar: ") << endl;
    
    //delete t;
    t->stop();
#if defined(BINREL_STATS)
  //  con->printStats(cout);
#endif //if defined(BINREL_STATS)
    double totTime = t->total_time();
    cout << totTime;
    delete t;
    delete pds;
    return totTime;
  }

  /*
  *  Determines if an error state in 'pg' is reachable using a wpds.
  *
  *  Creates an fpds representing the program with bdds as weights.  Runs poststar and computes a
  *  path summary to find the results.
  */
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

  /*
  * Create the differentials from the regular expressions of nodes
  * with epsilon transitions
  */
  void createDifferentials(tslRegExpMap eMap, tslDiffMap & DMap){
    for (tslRegExpMap::iterator it = eMap.begin(); it!=eMap.end(); ++it)
    {
       RTG::regExpTListRefPtr rtList = CIR::TDifferential(it->second);
       DMap[it->first] = rtList;
    }
  }


  /*
  *  Convert a wali NameWeight regexp to a tensored regular expressions
  *
  *  The wali regexp represents a differentiated value.  The weight represents a regexp in eMap
  *  differentiated with respect to a given variable (or not differentiated, depending) or is
  *  the representation of "one"
  */
  RTG::regExpTRefPtr convertToRegExpT(reg_exp_t exp, tslRegExpMap eMap, tslDiffMap DMap)
  {
	//If the expressions is a constant, then it has a name weight type
    if(exp->isConstant())
    {
      NameWeight * nw = static_cast<wali::domains::name_weight::NameWeight*>(exp->get_weight().get_ptr());
      int var = nw->getName1();
      int reID = nw->getName2();
      if (var == -1){
        if(reID == -1){
			return RTG::OneT::make();  //The value isn't a variable, so make it a tensored one
		} else {
			RTG::regExpRefPtr d = eMap[reID];  //If the first is -1, then this is not a differentiated value, so just use the regexp
			return RTG::TensorTranspose::make(RTG::One::make(),d);  //Create a tensored weight with the regexp
		}
      }
      else {
		RTG::regExpTRefPtr d = CIR::getTFromRegList(DMap[reID],var); //the NameWeight represents reID differentiated with respect to var
        return d;
      }
    }else if (exp->isUpdatable()){  //This is a Variable - so make a variable with the associated node numbers
      int node_no = exp->updatableNumber();
      return RTG::VarT::make(CBTI::INT32(node_no));
   } else if (exp->isStar()){ //A Star operation only has one child in the children list
        list<reg_exp_t> children = exp->getChildren();
        RTG::regExpTRefPtr c = convertToRegExpT(children.front(), eMap, DMap);
        return RTG::KleeneT::make(c);
    } else if (exp->isExtend()){ //Convert an extend to a Dot regExpT using recursive calls and the left and right children of the regexp
        list<reg_exp_t> children = exp->getChildren();
        list<reg_exp_t>::iterator ch = children.begin();
        RTG::regExpTRefPtr lch = convertToRegExpT(*ch, eMap, DMap);
        ch++;
        RTG::regExpTRefPtr rch = convertToRegExpT(*ch, eMap, DMap);
        return RTG::DotT::make(lch,rch);
    } else if (exp->isCombine()){ //Convert an combine to a Plus regExpT using recursive calls and the left and right children of the regexp
        list<reg_exp_t> children = exp->getChildren();
        list<reg_exp_t>::iterator ch = children.begin();
        RTG::regExpTRefPtr lch = convertToRegExpT(*ch, eMap, DMap);
        ch++;
        RTG::regExpTRefPtr rch = convertToRegExpT(*ch, eMap, DMap);
        return RTG::PlusT::make(lch,rch);
    }
  }
  
  /*
  *  Converts from a reg_exp_t to a RTG::regExpRefPtr
  *
  *  exp - the expression to be converted
  *  uMap - a map from updateable node to the node number it depends on (that node number is still associated with the inter graph)
  *  oMap - a map from the intergraph node number to the correct unique number for the tsl regexps
  */
  RTG::regExpRefPtr convertToRegExp(reg_exp_t exp, map<int,int> uMap)
  {
	//If the expression is a constant, return the appropriate constant types
    if(exp->isConstant())
    {
      if(exp->isOne())
        return RTG::One::make();
      else if (exp->isZero())
        return RTG::Zero::make();
      else {  //If the expresssion isn't a simple constant, make an external TSL wrapper around the constant and create a TSL weight
		nwaobdd_t w = dynamic_cast<NWAOBDDRel*>(exp->get_weight().get_ptr());
		EXTERN_TYPES::sem_elem_wrapperRefPtr * wt = new EXTERN_TYPES::sem_elem_wrapper(w);
        return RTG::Weight::make(*wt);
      }
    } 
    else if (exp->isUpdatable())
   {  
		//If the expression is updatable/is a variable
		//Then look up the node it depends on - and make a variable associated with it
        int node_no = exp->updatableNumber();
		int mNum = uMap[node_no];
		return RTG::Var::make(CBTI::INT32(mNum));
    } else if (exp->isStar()){
        list<reg_exp_t> children = exp->getChildren();
        RTG::regExpRefPtr c = convertToRegExp(children.front(),uMap); //A Star operation only has one child in the children list
		return RTG::Kleene::make(c);
    } else if (exp->isExtend()){  //Convert an extend to a Dot regExp using recursive calls and the left and right children of the regexp
        list<reg_exp_t> children = exp->getChildren();
		list<reg_exp_t>::iterator ch = children.begin();
		RTG::regExpRefPtr lch = convertToRegExp(*ch, uMap);
		ch++;
		RTG::regExpRefPtr rch = convertToRegExp(*ch, uMap);
		return RTG::Dot::make(lch,rch);
    } else if (exp->isCombine()){ //Convert a combine to a Plus regExp using recursive calls and the left and right children of the regexp
        list<reg_exp_t> children = exp->getChildren();
        list<reg_exp_t>::iterator ch = children.begin();
        RTG::regExpRefPtr lch = convertToRegExp(*ch, uMap);
        ch++;
        RTG::regExpRefPtr rch = convertToRegExp(*ch, uMap);
        return RTG::Plus::make(lch,rch);
    }
  }


  /*
  *  For each element in the list of regular expressions, convert it to a TSL regular expression and put it in a map from
  *  RegExp ID to TSL regular expressions
  *
  *  updateableMap - A map of nodes and the regExp they depend on
  *  oMap - still unsure about
  */
  void convertToTSLRegExps(std::map<int,reg_exp_t> rList, std::map<int,int> updateableMap, tslRegExpMap & tslExpressionMap)
  {
    for (std::map<int,reg_exp_t>::iterator it=rList.begin(); it!=rList.end(); ++it)
    {
      RTG::regExpRefPtr rExp = convertToRegExp(it->second, updateableMap);
      tslExpressionMap[it->first] = rExp;
    }
  }

  /*
  *    For each regular expression, convert to a tensored tsl regexp
  */
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

  /*
  *  A method to print TSL RegExps
  */
  void printTSLRegExps(tslRegExpMap rMap)
  {
    for (tslRegExpMap::iterator it = rMap.begin(); it!=rMap.end(); ++it)
    {
      cout << it->first << ": ";
      it->second.print(cout);
      cout << "\n";
    }
  }

  /*
  *  A method to print TSL Differentials
  */
  void printTSLDifferentials(tslDiffMap dMap)
  {
    for (tslDiffMap::iterator it = dMap.begin(); it!=dMap.end(); ++it)
    {
      cout << it->first << ": ";
      it->second.print(cout);
      cout << "\n";
    }
  }

  /*
  *  A method to print TSL tensored RegExps
  */
  void printTSLTExp(tslRegExpTMap tMap)
  {
    for (tslRegExpTMap::iterator it = tMap.begin(); it!=tMap.end(); ++it)
    {
      cout << it->first << ": ";
      it->second.print(cout);
      cout << "\n";
    }
  }

  /*
  * Given a list of tensored differentials, return a list of variables
  * used by them
  */
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

  /*
  *  Functions to generate stack and state names
  */
  static wali::Key st1()
  {
    return getKey("Unique State Name");
  }

  static wali::Key stk(int k)
  {
    return getKey(k);
  }

  /*
  *  Creates a new fwpds using differentials generated from regular expressions
  *  
  *  pds - the new fwpds that's being generated
  *  DMap - the map of differentials
  */
  void fwpdsFromDifferential(FWPDS * pds, tslDiffMap DMap)
  {
	//For each differential
    for(tslDiffMap::iterator it = DMap.begin(); it!=DMap.end(); ++it)
    {
      int dummy = -1;
      int tgt = it->first;
	  //Get the list of variable IDs which are depended on by this differential
      set<int> srcList = getVarList(it->second);

	  //For each variable in the differential, create a rule where the from stack is
	  //that variable ID and the to stack is the id representing the regexp that
	  //generated the differential
	  //
	  //The weight on the rule is of type NameWeight - (the variable name, the regexp id)
      for(set<int>::iterator i = srcList.begin(); i != srcList.end(); i++)
      {
        int src = *i;
        pds->add_rule(st1(), stk(src),st1(),stk(tgt), new NameWeight(src,tgt));
      }
	  //Also add a rule a dummy rule
      pds->add_rule(st1(), stk(dummy), st1(), stk(tgt), new NameWeight(dummy,tgt));
    }

  }

  /*
  *  Runs the Newton method on on the various maps until we have reached a fix point
  *  Mmap - a list of tensored regular expressions
  *  D - a list of differentials
  *  aList - an initial assignments for variables
  */
  void runNewton(tslRegExpTMap Mmap, tslDiffMap D, RTG::assignmentRefPtr & aList)
  {
    bool newton = true;
	// A map of dependencies
    std::map<int,std::set<int> > dependenceMap;
    std::set<int> changedVars; //The set of possible dirty regexps IDs
	std::set<int> C;
    RTG::assignmentRefPtr aPrime = aList;
    tslDiffMap::iterator dIt;

	// For each set of differentials, get the list of variables they depend on
    // Put those into dependeceMap
	// dependenceMap - Map from regular expression ID to the set variables that regexps differentials depend on
    for(dIt = D.begin(); dIt != D.end(); dIt++)
    {
      std::set<int> dInt = getVarList(dIt->second);
      dependenceMap[dIt->first] = dInt;
      changedVars.insert(dIt->first); //Insert into retPtSet because at start all variable values are potentially "dirty"
    }
    
	bool first = true; //So we know we are on the first Newton Round

    //While the set of changed variables != empty, continue iterating through NewtonRounds
	//This is the actual workhorse of the Newton Rounds
    while(changedVars.size() != 0){
      std::set<int>::iterator pIt;
	  //For each of the variables whose dependencies changed, get their new assignement from aList
      for(pIt = changedVars.begin(); pIt != changedVars.end(); pIt++)
      {
        aPrime = CIR::updateAssignment(aPrime,CBTI_INT32(*pIt),CIR::getAssignment(CBTI_INT32(*pIt),aList));
      }

	  C = changedVars;
      changedVars.clear();

      tslRegExpTMap::iterator assignIt;
      
	  //For each outgoing regular expression
      for(assignIt = Mmap.begin(); assignIt != Mmap.end(); assignIt++)
      {
		//Get the list of variables this regexp's differentials depend on
        int var = assignIt->first;
		std::set<int> varsUsedInP = dependenceMap[var];

		//Now determine if any of the depended variables have been changed
		bool found = false; //True if a variable has been altered
		std::set<int>::iterator sIt1 = C.begin();
		std::set<int>::iterator sIt2 = varsUsedInP.begin();
		//Check to see if C intersect VarsUsedIn(p) is empty
		if(!first){
			while((sIt1 != C.end()) && (sIt2 != varsUsedInP.end()) && !found)
			{
				if(*sIt1<*sIt2) 
					++sIt1;
				else if(*sIt2<*sIt1) 
					++sIt2;
				else
					found = true;
			}
		}
		else  //It's the first time doing a NewtonRound, so all variables have been altered
		{
			found = true;
		}
		if(found){
			//v = Tdetensor(evalT(Mmap[p],aPrime))
			//The variable representing the regexp is potentially dirty, so reevaluate it using the
			//new assignment in aPrime
			EXTERN_TYPES::sem_elem_wrapperRefPtr newVal = CIR::evalT(assignIt->second, aPrime); 
			//DetensorTranspose the new value
			EXTERN_TYPES::sem_elem_wrapperRefPtr rep = EXTERNS::detensorTranspose(newVal);
			//Check to see if the new value is the same as the previous on
			EXTERN_TYPES::sem_elem_wrapperRefPtr oldVal = CIR::getAssignment(CBTI_INT32(var),aPrime);
			//if A'[p] != v`
			//Add v' to the changedVar set and updated the assignmentList
			if(!(oldVal.v->Equal(rep.v))){
				changedVars.insert(var); //C' = C' union {p}
				//A[p] = v
				aList = CIR::updateAssignment(aList, CBTI_INT32(var), rep);
			}
		}
      }
      first = false;
      RTG::_evalRegExpHash().clear();
    }
  }

  //Evaluate the tslRegExps using a list of variable assignments 'aList' - put the result
  // into finWeights
  void evalRegExps(tslRegExpMap reMap, RTG::assignmentRefPtr aList, map<int,nwaobdd_t> & finWeights)
  {
    for(tslRegExpMap::iterator it = reMap.begin(); it != reMap.end(); it++)
    {
      EXTERN_TYPES::sem_elem_wrapperRefPtr w = CIR::evalRegExp(it->second,aList);
      finWeights[it->first] = w.v;
    }
  }

  /*
  *  This runs a fixed point error finding method using Newton rounds.
  *
  *	 Step 1 - Convert the program 'pg' into an fpds where the weights are nwaobdds.
  *
  *  Step 2 - Perform poststar on the fpds get the regular expressions associated with
  *			  the outgoing nodes in the intra_graph associated with the fwpds
  *
  *	 Step 3 - Convert these regexps into TSL regular expressions and get the differentials
  *			  with respect their variables
  *
  *	 Step 4 - Create a new fwpds using these differentials - run poststar on the fwpds in
  *			  order to get new regular expressions representing the return points for Newton
  *
  *  Step 5 - Use the reg exps representing the differentials, the partial differentials, and the original
  *			  tsl regular expressions to get the tensored regular expressions we need to run the newton rounds
  *
  *  Step 6 - Use newton's method to find the fixed point of the weights
  *
  *  Step 7 - Insert the new weights into the original outfa and perform the iterative path summary in order to
  *			  determine if the error weight is reachable
  */
  double run_newton(WFA& outfa, FWPDS * originalPds = NULL)
  { 
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal VIII: end-to-end newton_merge_notensor_fwpds run" << endl;
    
	//Step 1 - Convert the program 'pg' into an fpds where the weights are nwaobdds.
	FWPDS * fpds;
    if(originalPds != NULL)
      fpds = new FWPDS(*originalPds);
    else{
      fpds = new FWPDS();
      ncon = pds_from_prog_nwa(fpds, pg);
    }

	int dummy = -1;
	map<int, reg_exp_t> outNodeRegExpMap; // A map from a unique id of an outgoing node to the regular expression associated with it
	map<int, reg_exp_t> rNew; // A map from the node ids to the regexps representing the full differentials associated with that node id
	map<int, int> updateableMap;  //A map from an upadateable node number to the id of the node it depends on 
	map<int, pair< pair<int, int>, int> > mapBack;  //A map from the node index to the struct of the <<src,tgt>,stack> - used to insert weights back into wfa
	vector<int> differentiatedList; //A list of nodes with where the differential is to be taken
	tslRegExpMap E;  //The map of all tsl regular expression
	tslRegExpMap diffMap;  //The map of tsl regular expressions to be differentiated (the program return points)
	tslDiffMap differentialMap;  //This is a map from regexp ids to the partial differentials assoicated with them
	tslRegExpTMap tensoredRegExpMap;
	map<int, nwaobdd_t> finWeights;  //The map from node id to the final nwaobdd_t weights


    wali::set_verify_fwpds(false);
    
    fpds->useNewton(false);
    //fpds->add_rule(st1(),getKey(mainProc),st1(),fpds->get_theZero()->one());

    wali::util::GoodTimer * t = new wali::util::GoodTimer("temp");
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, fpds->get_theZero()->one());
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);

	/* Step 2 - Perform poststar on the fpds get the regular expressions associated with
	*			the outgoing nodes in the intra_graph associated with the fwpds
	*/
	//This function performs postar on the fa using the fpds and populations the maps as described above
	//The boolean means this is the first time the functions is called and will generate unique ids as needed
	t->stop();
	double t1 = fpds->getOutRegExps(fa, outfa, outNodeRegExpMap, updateableMap, mapBack, differentiatedList);
    
	/*Step 3 - Convert these regexps into TSL regular expressions and get the partial differentials
	*		   with respect their variables
    */
	convertToTSLRegExps(outNodeRegExpMap,updateableMap,E);

	//Create a map from unique IDs to tsl regular expressions with variables
	//Currently doing this by iterating through the regular expressions and copying
	//the tsl regular expression whose ids match the nodes in the differentiatedMap
	for (vector<int>::iterator eit = differentiatedList.begin(); eit != differentiatedList.end(); eit++)
    {
      diffMap[(*eit)] = E[(*eit)];
	  //This is used  in debugging to compare the epsilon transitions with those generated by non-newton methods
	  //and make sure they match
#if defined(NEWTON_DEBUG)
      int src = mapBack[(*eit)].first.first;
      int tgt = mapBack[(*eit)].first.second;
      int stack = mapBack[(*eit)].second;
      globalEpsilon.insert(std::make_pair(std::make_pair(src,tgt),stack));
#endif
    }
    t->start();

	//Created the differentials
    createDifferentials(diffMap,differentialMap);

	/* Step 4 - Create a new fwpds using these partial differentials - run poststar on the fwpds in
	*	        order to get the full differentials representing the values of the program return points
	*/
	//Now we create new fwpds using these differentials, this fwpds has the weight type of nameweight
    FWPDS * fnew = new FWPDS();
    fwpdsFromDifferential(fnew, differentialMap);
    
	//Now create another finite automata
    WFA fa2;
    wali::Key acc2 = wali::getKeySpace()->getKey("accept2");
    fa2.addTrans(getPdsState(),stk(dummy), acc2, fnew->get_theZero()->one());
    fa2.setInitialState(getPdsState());
	fa2.addFinalState(acc2);
    WFA outfa2;

	//Get the regexps generated by running poststar on the new fwpds, these are the ones we will use in the Newton Round
    t->stop();
    double t2 = fnew->getOutRegExpsSimple(fa2,outfa2,rNew);


	/* Step 5 - Use the reg exps representing the differentials, the partial differentials, and the original
	*			tsl regular expressions to get the tensored regular expressions we need to run the newton rounds
    */
	convertToTSLRegExpsT(rNew,differentialMap,E,tensoredRegExpMap);
    t->start();
    RTG::assignmentRefPtr aList = CIR::initializeAssignment();

	/*Step 6 - Use newton's method to find the fixed point of the weights and then reevaluate the values of all the regexps*/
    runNewton(tensoredRegExpMap,differentialMap,aList);

	//Using the final weights from Newton, evaluate the tslRegExps to get the final weights
    evalRegExps(E,aList,finWeights);

	/* Step 7 - Insert the new weights into the original outfa and perform the iterative path summary in order to
	*			determine if the error weight is reachable
    */
	for(map<int,pair<pair<int,int>,int> >::iterator mbit = mapBack.begin(); mbit != mapBack.end(); mbit++)
    {
      int src = mbit->second.first.first;
      int tgt = mbit->second.first.second;
      int stck = mbit->second.second;
      wali::wfa::ITrans * tt = outfa.find2(src,stck,tgt);
      sem_elem_t w = finWeights[mbit->first];
      tt->setWeight(w);
    }

    outfa.path_summary_iterative_original(outfa.getSomeWeight()->one());

	t->stop();
	double tTime = t->total_time() + t1 + t2;
	delete fpds;
	delete t;
	return tTime;
  }

  /*
  *  A function fo compare ewpds,fwpds, and newton methods.  Currently running without merge functions.
  *
  *  Runs ewpds, fwpds, and newton on the same pds with nwaobdd weights and gets timing for each.
  */
  void compareEwpdsFwpdsNewton()
  {
    FWPDS * originalPds = new FWPDS();
    ncon = pds_from_prog_nwa(originalPds, pg);
    WpdsStackSymbols syms;
    originalPds->for_each(syms);


    wali::util::GoodTimer * eTimer = new wali::util::GoodTimer("EWPDS time");
    WFA outfaEwpds;
    double cEtime = runEwpds(outfaEwpds, originalPds);
    eTimer->stop();

    wali::util::GoodTimer * fTimer = new wali::util::GoodTimer("FWPDS time");
    WFA outfaFwpds;
    double cFtime = runFwpds(outfaFwpds, originalPds);
    fTimer->stop();

    wali::util::GoodTimer * nTimer = new wali::util::GoodTimer("Newton time");
    WFA outfaNewton;
    double cNtime = 0;
    run_newton(outfaNewton, originalPds);
    nTimer->stop();

	// used for debugging newton, a way to determine that the weights originally created for the program return pts match those
	// generated by ewpds & fwpds
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

	//  This is used to determine correctness.  If the newton join map is the same as the ewpds and fwpds join
	//  map, then we know that newton is running correctly.
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
	NWA_OBDD::DisposeOfPairProductMapCaches();
	NWA_OBDD::DisposeOfPairProductCache();
	NWA_OBDD::DisposeOfPathSummaryCache();
	DisposeOfModPathSummaryCache();
	NWA_OBDD::NWAOBDDNodeHandle::DisposeOfReduceCache();
    
  }

}

using namespace goals;

static short goal;

void * work(void *)
{
  int dump;
  //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &dump);

  //switches on which method to run
  WFA outfa;
  switch(goal){
    case 1:
      compareEwpdsFwpdsNewton();
      break;
    case 2:
      runNwpds(outfa);
      break;
    case 3:
      runFwpds(outfa);
      break;
    case 4:
      runWpds(outfa);
      break;
    case 5:
      runEwpds(outfa);
      break;
    case 6:
      run_newton(outfa);
      break;
    default:
      assert(0 && "I don't understand that goal!!!");
  }
  return NULL;
}

/*
*  Runs the Esparza evaluation method.
*
*  The form should be of <.bp file> <int goal>
*/
int main(int argc, char ** argv)
{
  // register signal handler
	std::cerr << "TEST!";
  if(argc < 3){
    cerr 
      << "Usage: " << endl
      << "./NewtonFwpdsCompare input_file goal(1/2) [<0/1> dump] [entry function (default:main)] [error label (default:error)]" << endl
      << "Goal: 1 --> Compare EWPDS, FWPDS, and Newton. Compute poststar and compare the output automata." << endl
      << "Goal: 2 --> Run newton_nomerge_tensor_fwpds end-to-end." << endl
      << "Goal: 3 --> Run kleene_merge_fwpds end-to-end." << endl 
      << "Goal: 4 --> Run kleene_nomerge_wpds end-to-end." << endl
      << "Goal: 5 --> Run kleene_merge_ewpds end-to-end." << endl
      << "Goal: 6 --> Run newton_nomerge_tensor end-to-end." << endl;
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
  int yydebug = 1;
  pg = parse_prog(fname.c_str());

  if(!mainProc) mainProc = strdup("main");
  if(!errLbl) errLbl = strdup("error");
  cout << "[Newton Compare] Post processing parsed program... " << endl;
  make_void_returns_explicit(pg);
  remove_skip(pg);
  instrument_enforce(pg);
  instrument_asserts(pg, errLbl);
  instrument_call_return(pg);

  if(dump){
	  NWA_OBDD::NWAOBDDNodeHandle::InitNoDistinctionTable();
	  NWA_OBDD::NWAOBDDNodeHandle::InitReduceCache();
	  NWA_OBDD::InitPairProductCache();
	  NWA_OBDD::InitPathSummaryCache();
	  NWA_OBDD::InitPairProductMapCaches();
    FWPDS * originalPds = new FWPDS();
	ncon = pds_from_prog_nwa(originalPds, pg);
    cout << "[Newton Compare] Dumping PDS to pds.dot..." << endl;
    fstream pds_stream("pds.dot", fstream::out);
    RuleDotty rd(pds_stream);
    pds_stream << "digraph{" << endl;
    originalPds->for_each(rd);
    pds_stream << "}" << endl;
    delete(originalPds);
	NWA_OBDD::DisposeOfPairProductMapCaches();
	NWA_OBDD::DisposeOfPairProductCache();
	NWA_OBDD::DisposeOfPathSummaryCache();
	NWA_OBDD::NWAOBDDNodeHandle::DisposeOfReduceCache();
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
