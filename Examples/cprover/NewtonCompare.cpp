// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"
// ::wali::wpds::ewpds
#include "wali/wpds/ewpds/EWPDS.hpp"
// ::wali::wpds
#include "wali/wpds/WPDS.hpp"
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

using namespace std;
using namespace wali;
using namespace wali::wfa;
using namespace wali::wpds;
using namespace wali::wpds::ewpds;
using namespace wali::wpds::fwpds;
using namespace wali::cprover;
using namespace wali::domains::binrel;

#include <pthread.h>
#include <signal.h>

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
          wali::SemElemTensor * wt = dynamic_cast<SemElemTensor*>(t->weight().get_ptr());
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
        {//DEBUG
          /*
             if(out){
           *out << "firstData:" << std::endl;
           for(DataMap::const_iterator iter = firstData.begin();
           iter != firstData.end();
           iter++)
           (iter->first).print(*out);
           *out << "secondData:" << std::endl;
           for(DataMap::const_iterator iter = secondData.begin();
           iter != secondData.end();
           iter++)
           (iter->first).print(*out);
           }
           */
        }//DEBUG


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
  FWPDS * originalPds = NULL;
  BddContext * con = NULL;

  void deepCompare()
  {
    assert(originalPds && con && mainProc && errLbl);
    WFACompare fac("FWPDS", "NEWTON");
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal I: Check Correctness by comparing the result automaton: WPDS vs NWPDS" << endl;
    {
      cout << "///////////////// WPDS ///////////////////" << endl;
      WPDS * pds = new WPDS(*originalPds);

      WFA fa;
      wali::Key acc = wali::getKeySpace()->getKey("accept");
      fa.setInitialState(getPdsState());
      fa.addFinalState(acc);
      fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, pds->get_theZero()->one());
      if(dump){
        cout << "[Newton Compare] Dumping the query automaton for WPDS" << endl;
        fstream in_fa_stream("wpds_in_fa.dot", fstream::out);
        TransDotty td(in_fa_stream,false, NULL);      
        in_fa_stream << "digraph{" << endl;
        fa.for_each(td);
        in_fa_stream << "}" << endl;
      }

      WFA outfa;
      {
        cout << "[Newton Compare] Computing WPDS poststar..." << endl;
        wali::util::Timer * t3 = new wali::util::Timer("FWPDS poststar",cout);
        pds->poststar(fa,outfa); 
        t3->print(std::cout << "[Newton Compare] Time taken by WPDS poststar: ") << endl;
        delete t3;
      }
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
      delete pds;
    }

    {
      cout << "///////////////// NWPDS ///////////////////" << endl;
      FWPDS * npds = new FWPDS(*originalPds);
      wali::set_verify_fwpds(false);
      npds->useNewton(true);

      WFA fa;
      wali::Key acc = wali::getKeySpace()->getKey("accept");
      fa.setInitialState(getPdsState());
      fa.addFinalState(acc);
      fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, npds->get_theZero()->one());
      if(dump){
        cout << "[Newton Compare] Dumping the query automaton for NWPDS" << endl;
        fstream in_fa_stream("nwpds_in_fa.dot", fstream::out);
        TransDotty td(in_fa_stream,false, NULL);
        in_fa_stream << "digraph{" << endl;
        fa.for_each(td);
        in_fa_stream << "}" << endl;
      }

      WFA outfa;
      {
        cout << "[Newton Compare] Computing NWPDS poststar..." << endl; 
        wali::util::Timer * t2 = new wali::util::Timer("NWPDS poststar",cout);
        cout << "[NWPDS poststar]\n";
        npds->poststar(fa,outfa);
        t2->print(std::cout << "[Newton Compare] Time take by NWPDS poststar: ") << endl;
        delete t2;
      }
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
      delete npds;
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
  }

  void runNewton()
  { 
    assert(originalPds && con && mainProc && errLbl);
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal III: end-to-end NWPDS run" << endl;
    FWPDS * npds = new FWPDS(*originalPds);
    wali::set_verify_fwpds(false);
    npds->useNewton(true);

    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);
    fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, npds->get_theZero()->one());

    WFA outfa;
    cout << "[Newton Compare] Computing NWPDS poststar..." << endl;
    wali::util::Timer * t3 = new wali::util::Timer("NWPDS poststar",cout);
    t3->measureAndReport = false;
#if defined(BINREL_STATS)
    con->resetStats(); 
#endif
    npds->poststarIGR(fa,outfa); 

    cout << "[Newton Compare] Computing path summary..." << endl;
    outfa.path_summary(npds->get_theZero()->one());

    cout << "[Newton Compare] Checking error label reachability..." << endl;
    TransSet ts = outfa.match(getPdsState(), getErrStk(pg));
    sem_elem_t wt = outfa.getSomeWeight()->zero();
    for(TransSet::const_iterator it = ts.begin(); it != ts.end(); ++it){
        Key toKey = (*it)->to();
        State * toState = outfa.getState(toKey);
        wt = wt->combine(toState->weight());
    }
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] NWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] NWPDS ==> error reachable" << endl;
    }

    t3->print(std::cout << "[Newton Compare] Time taken by NWPDS poststar: ") << endl;
    delete t3;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
  }

  void runFwpds()
  { 
    assert(originalPds && con && mainProc && errLbl);
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal IV: end-to-end FWPDS run" << endl;
    FWPDS * npds = new FWPDS(*originalPds);
    wali::set_verify_fwpds(false);
    npds->useNewton(false);

    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);
    fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, npds->get_theZero()->one());

    WFA outfa;
    cout << "[Newton Compare] Computing FWPDS poststar..." << endl;
    wali::util::Timer * t3 = new wali::util::Timer("NWPDS poststar",cout);
    t3->measureAndReport = false;
#if defined(BINREL_STATS)
    con->resetStats(); 
#endif
    npds->poststarIGR(fa,outfa); 

    cout << "[Newton Compare] Computing path summary..." << endl;
    outfa.path_summary(npds->get_theZero()->one());

    cout << "[Newton Compare] Checking error label reachability..." << endl;
    TransSet ts = outfa.match(getPdsState(), getErrStk(pg));
    sem_elem_t wt = outfa.getSomeWeight()->zero();
    for(TransSet::const_iterator it = ts.begin(); it != ts.end(); ++it){
        Key toKey = (*it)->to();
        State * toState = outfa.getState(toKey);
        wt = wt->combine(toState->weight());
    }
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] FWPDS ==> error reachable" << endl;
    }

    t3->print(std::cout << "[Newton Compare] Time taken by FWPDS poststar: ") << endl;
    delete t3;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
  }


  void endToEndCompareFwpdsRun()
  {
    cout << "///////////////// FWPDS ///////////////////" << endl;
    FWPDS * fpds = new FWPDS(*originalPds);
    wali::set_verify_fwpds(false);
    fpds->useNewton(false);

    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);
    fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, fpds->get_theZero()->one());

    WFA outfa;
    cout << "[Newton Compare] Computing FWPDS poststar..." << endl;
    wali::util::Timer * t3 = new wali::util::Timer("FWPDS poststar",cout);
    t3->measureAndReport = false;
#if defined(BINREL_STATS)
    con->resetStats(); 
#endif
    fpds->poststarIGR(fa,outfa); 

    cout << "[Newton Compare] Computing path summary..." << endl;
    outfa.path_summary(fpds->get_theZero()->one());

    cout << "[Newton Compare] Checking error label reachability..." << endl;
    TransSet ts = outfa.match(getPdsState(), getErrStk(pg));
    sem_elem_t wt = outfa.getSomeWeight()->zero();
    for(TransSet::const_iterator it = ts.begin(); it != ts.end(); ++it){
      Key toKey = (*it)->to();
      State * toState = outfa.getState(toKey);
      wt = wt->combine(toState->weight());
    }
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
    else
      cout << "[Newton Compare] FWPDS ==> error reachable" << endl;

    t3->print(std::cout << "[Newton Compare] Time taken by FWPDS poststar: ") << endl;
    delete t3;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
  }

  void endToEndCompareNewtonRun()
  {
    cout << "///////////////// NWPDS ///////////////////" << endl;
    FWPDS * npds = new FWPDS(*originalPds);
    wali::set_verify_fwpds(false);
    npds->useNewton(true);

    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);
    fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, npds->get_theZero()->one());

    WFA outfa;
    cout << "[Newton Compare] Computing NWPDS poststar..." << endl;
    wali::util::Timer * t3 = new wali::util::Timer("FWPDS poststar",cout);
    t3->measureAndReport = false;
#if defined(BINREL_STATS)
    con->resetStats(); 
#endif
    npds->poststarIGR(fa,outfa); 

    cout << "[Newton Compare] Computing path summary..." << endl;
    outfa.path_summary(npds->get_theZero()->one());

    cout << "[Newton Compare] Checking error label reachability..." << endl;
    
    WpdsStackSymbols syms;
    npds->for_each(syms);


    WFA errfa;

    wali::Key init = wali::getKey("__init");
    errfa.addState(getPdsState(),outfa.getSomeWeight());
    errfa.setInitialState(init);
    wali::Key fin = wali::getKey("__final");
    errfa.addState(fin);
    errfa.addFinalState(fin);

    std::set<Key>::iterator it;
    errfa.addTrans(init, getErrStk(pg), fin, outfa.getSomeWeight());

    for(it = syms.gamma.begin(); it != syms.gamma.end(); it++)
    {
      errfa.addTrans(fin, *it, fin, outfa.getSomeWeight());
    }

    WFA interfa;
    KeepRight wmaker;
    interfa = errfa.intersect(wmaker, outfa);


    TransSet ts = interfa.match(getPdsState(), getErrStk(pg));
    sem_elem_t wt = interfa.getSomeWeight()->zero();
    for(TransSet::const_iterator it = ts.begin(); it != ts.end(); ++it){
      Key toKey = (*it)->to();
      State * toState = interfa.getState(toKey);
      wt = wt->combine(toState->weight());
    }
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] NWPDS ==> error not reachable" << endl;
    else
      cout << "[Newton Compare] NWPDS ==> error reachable" << endl;

    t3->print(std::cout << "[Newton Compare] Time taken by NWPDS poststar: ") << endl;
    delete t3;

#if defined(BINREL_STATS)
    con->printStats(cout);
#endif //if defined(BINREL_STATS)
  }

  void endToEndCompare()
  {
    assert(originalPds && con && mainProc && errLbl);
    cout << "#################################################" << endl;
    cout << "[Newton Compare] Goal II: end-to-end time comparison: FWPDS vs NWPDS" << endl;
    endToEndCompareFwpdsRun();
    endToEndCompareNewtonRun();
  }

}

using namespace goals;

static short goal;

void * work(void *)
{
  int dump;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &dump);


  switch(goal){
    case 1:
      deepCompare();
      break;
    case 2:
      endToEndCompare();
      break;
    case 3:
      runNewton();
      break;
    case 4:
      runFwpds();
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
      << "Goal: 2 --> Compare FWPDS & NWPDS. Compute poststar and check if any assertion can fail." << endl
      << "Goal: 3 --> Simply run NWPDS end-to-end." << endl
      << "Goal: 4 --> Simply run FWPDS end-to-end. You may need it some time!" << endl; 
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

  if(!mainProc) mainProc = strdup("main");
  if(!errLbl) errLbl = strdup("error");
  cout << "[Newton Compare] Post processing parsed program... " << endl;
  make_void_returns_explicit(pg);
  instrument_enforce(pg);
  instrument_asserts(pg, errLbl);
  instrument_call_return(pg);

  cout << "[Newton Compare] Obtaining PDS..." << endl;
  originalPds = new FWPDS();
  con = pds_from_prog(originalPds, pg);
  if(dump){
    cout << "[Newton Compare] Dumping PDS to pds.dot..." << endl;
    fstream pds_stream("pds.dot", fstream::out);
    RuleDotty rd(pds_stream);
    pds_stream << "digraph{" << endl;
    originalPds->for_each(rd);
    pds_stream << "}" << endl;
  }


  pthread_create(&worker, NULL, &work,  NULL);

  void * dump;
  pthread_join(worker, &dump);

  // Clean Up.
  delete originalPds;
  deep_erase_prog(&pg);
  return 0;
}
