#include "gtr/src/lang/gtr_config.h"
#include "tsl/cir/regexp/conc.1level.cir.hpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_extern_types.hpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_externs.hpp"
////// The following includes of CPP files were removed by Jason Breck
//////
//////   #include "tsl/analysis_components/src/reinterps/emul/conc_extern_types.cpp"
//////   #include "tsl/analysis_components/src/reinterps/emul/conc_externs.cpp"
#include "tsl/analysis_components/src/reinterps/emul/concrete_base_type_interp.hpp"
//////   #include "tsl/analysis_components/src/reinterps/emul/concrete_base_type_interp.cpp"
//////   #include "tsl/cir/regexp/conc.1level.cir.cpp"
//
//
////// Jason Breck added the following lines (up to //////////////////////////)
typedef CBTI BASETYPE;
#include "tsl/analysis_components/src/utils/prettyprint/RegExpPrettyPrint.hpp"
/////////////////////////////////////////
// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"
// ::wali::wpds::ewpds
#include "wali/wpds/ewpds/EWPDS.hpp"
#include "wali/graph/RegExp.hpp"
// ::wali::wpds
#include "wali/wpds/WPDS.hpp"
// ::wali::wpds::nwpds
#include "wali/wpds/nwpds/NWPDS.hpp"
#if defined(USE_AKASH_EWPDS) || defined(USING_AKASH_FWPDS)
#include "wali/wpds/ewpds/ERule.hpp"
#endif
// ::wali::wfa
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/State.hpp"
// ::wali::wpds
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/GenKeySource.hpp"
// ::std
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <stack>
#include <getopt.h>
// ::wali
#include "wali/KeySpace.hpp"
#include "wali/Key.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/IntSource.hpp"
// ::wali::util
#include "wali/util/Timer.hpp"
// ::wali::cprover
#include "BplToPds.hpp"
#include "wali/domains/name_weight/nameWeight.hpp"
#include "wali/domains/tsl_weight/TSLWeight.hpp"
#include <boost/unordered_map.hpp>
//#include "PtoICFG.hpp"
// TSL
#ifdef USE_NWAOBDD
#include "../turetsky/svn-repository/NWAOBDDRel.hpp"
#include "../turetsky/svn-repository/NWAOBDDRelMergeFns.hpp"
#elif USE_DUET
#include "wali/domains/duet/DuetRel.hpp"
#include "NewtonOcamlInterface.hpp"
extern "C" {
#include <caml/mlvalues.h>
#include <caml/callback.h>
}
#else
#include "wali/domains/binrel/BinRel.hpp"
#include "wali/domains/binrel/BinRelMergeFns.hpp"
#endif
#include "NewtonCompare.hpp"

using namespace std;
using namespace wali;
using namespace wali::wfa;
using namespace wali::wpds;
using namespace wali::wpds::ewpds;
using namespace wali::wpds::fwpds;
using namespace wali::wpds::nwpds;
using namespace wali::cprover;
using namespace wali::domains::name_weight;
using namespace wali::domains::tsl_weight;
#ifdef USE_NWAOBDD
using namespace wali::domains::nwaobddrel;
#elif USE_DUET
using namespace wali::domains::duetrel;
#else
using namespace wali::domains::binrel;
#endif
using namespace tsl_regexp;

typedef wali::ref_ptr<graph::RegExp> reg_exp_t;
typedef tsl_regexp::Conc1LevelRTG RTG;
typedef tsl_regexp::Conc1LevelCIR CIR;
typedef std::map<int,RTG::regExpRefPtr> tslRegExpMap;
typedef std::map<int,RTG::regExpTRefPtr> tslRegExpTMap;
typedef std::map<int,RTG::regExpTListRefPtr> tslDiffMap;
typedef std::map<int, RTG::regExpListRefPtr> tslUntensoredDiffMap;

#ifdef USE_NWAOBDD
typedef nwaobdd_t domain_t;
typedef NWAOBDDRel Relation;
#elif USE_DUET
typedef duetrel_t domain_t;
typedef DuetRel Relation;
#else
typedef binrel_t domain_t;
typedef BinRel Relation;
#endif

//#include <signal.h>
#include <boost/cast.hpp>

/*
static pthread_t worker;

extern "C" 
*/

bool nonRec, pNonLin;

#ifdef USE_NWAOBDD
NWAOBDDContext * con = NULL;
NWAOBDDContext * dCon = con;
#else
BddContext * con = NULL;
BddContext * dCon = con;
#endif


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


	typedef boost::unordered_map<
		MemoCacheKey1<RTG::regExpRefPtr >,
		ConcTSLInterface::regExpTListRefPtr,
		boost::hash<MemoCacheKey1<RTG::regExpRefPtr > >,
		std::equal_to<MemoCacheKey1<RTG::regExpRefPtr > >,
		std::allocator<std::pair<const MemoCacheKey1<RTG::regExpRefPtr >, ConcTSLInterface::regExpTListRefPtr> > >
		TDiffHashMap;

	TDiffHashMap TdiffMap;

	typedef boost::unordered_map<
		MemoCacheKey1<RTG::regExpRefPtr >,
		RTG::regExpListRefPtr,
		boost::hash<MemoCacheKey1<RTG::regExpRefPtr > >,
		std::equal_to<MemoCacheKey1<RTG::regExpRefPtr > >,
		std::allocator<std::pair<const MemoCacheKey1<RTG::regExpRefPtr >, RTG::regExpListRefPtr> > >
		DiffHashMap;

	DiffHashMap diffHMap;

	typedef boost::unordered_map<
		MemoCacheKey1<RTG::regExpRefPtr >,
		EXTERN_TYPES::sem_elem_wrapperRefPtr,
		boost::hash<MemoCacheKey1<RTG::regExpRefPtr > >,
		std::equal_to<MemoCacheKey1<RTG::regExpRefPtr > >,
		std::allocator<std::pair<const MemoCacheKey1<RTG::regExpRefPtr >, EXTERN_TYPES::sem_elem_wrapperRefPtr> > >
		EvalMap;

	EvalMap EvalMap0;

	// typedef boost::unordered_map<
	// 	MemoCacheKey2<RTG::regExpRefPtr, ConcTSLInterface::assignmentRefPtr >,
	// 	EXTERN_TYPES::sem_elem_wrapperRefPtr,
	// 	boost::hash<MemoCacheKey2<RTG::regExpRefPtr, ConcTSLInterface::assignmentRefPtr > >,
	// 	std::equal_to<MemoCacheKey2<RTG::regExpRefPtr, ConcTSLInterface::assignmentRefPtr > >,
	// 	std::allocator<std::pair<const MemoCacheKey2<RTG::regExpRefPtr, ConcTSLInterface::assignmentRefPtr>, EXTERN_TYPES::sem_elem_wrapperRefPtr> > >
	typedef boost::unordered_map<
		MemoCacheKey1<RTG::regExpRefPtr >,
		EXTERN_TYPES::sem_elem_wrapperRefPtr,
		boost::hash<MemoCacheKey1<RTG::regExpRefPtr > >,
		std::equal_to<MemoCacheKey1<RTG::regExpRefPtr > >,
		std::allocator<std::pair<const MemoCacheKey1<RTG::regExpRefPtr >, EXTERN_TYPES::sem_elem_wrapperRefPtr> > >
		EvalRMap;

	EvalRMap EvalMap2;

	// typedef boost::unordered_map<
	// 	MemoCacheKey2<RTG::regExpTRefPtr, ConcTSLInterface::assignmentRefPtr >,
	// 	EXTERN_TYPES::sem_elem_wrapperRefPtr,
	// 	boost::hash<MemoCacheKey2<RTG::regExpTRefPtr, ConcTSLInterface::assignmentRefPtr > >,
	// 	std::equal_to<MemoCacheKey2<RTG::regExpTRefPtr, ConcTSLInterface::assignmentRefPtr > >,
	// 	std::allocator<std::pair<const MemoCacheKey2<RTG::regExpTRefPtr, ConcTSLInterface::assignmentRefPtr >, EXTERN_TYPES::sem_elem_wrapperRefPtr> > >
	typedef boost::unordered_map<
		MemoCacheKey1<RTG::regExpTRefPtr >,
		EXTERN_TYPES::sem_elem_wrapperRefPtr,
		boost::hash<MemoCacheKey1<RTG::regExpTRefPtr > >,
		std::equal_to<MemoCacheKey1<RTG::regExpTRefPtr > >,
		std::allocator<std::pair<const MemoCacheKey1<RTG::regExpTRefPtr >, EXTERN_TYPES::sem_elem_wrapperRefPtr> > >
		EvalTMap;
	
	EvalTMap EvalMapT;

	typedef boost::unordered_map<
		MemoCacheKey1<RTG::regExpRefPtr >,
		EXTERN_TYPES::sem_elem_wrapperRefPtr,
		boost::hash<MemoCacheKey1<RTG::regExpRefPtr > >,
		std::equal_to<MemoCacheKey1<RTG::regExpRefPtr > >,
		std::allocator<std::pair<const MemoCacheKey1<RTG::regExpRefPtr >, EXTERN_TYPES::sem_elem_wrapperRefPtr> > >
		StarMap;

	typedef boost::unordered_map<
		MemoCacheKey1<RTG::regExpTRefPtr >,
		EXTERN_TYPES::sem_elem_wrapperRefPtr,
		boost::hash<MemoCacheKey1<RTG::regExpTRefPtr > >,
		std::equal_to<MemoCacheKey1<RTG::regExpTRefPtr > >,
		std::allocator<std::pair<const MemoCacheKey1<RTG::regExpTRefPtr >, EXTERN_TYPES::sem_elem_wrapperRefPtr> > >
		StarMapT;

	StarMap newStarVal;
	StarMapT newStarValT;
	
	

	// Stack frames needed by the non-recursive versions of convertToTSL and Eval and EvalT

	//A Stack frame which takes a regexp_t and has pointers to it's left and right children, used from
	//Converting to a TSLRegExp
	struct cFrame
	{
		cFrame(reg_exp_t & e) : is_new(true), e(e), op(), left(), right() {}

		bool is_new;
		reg_exp_t e;
		int op;
		reg_exp_t left;
		reg_exp_t right;
	};

	//A Stack frame which takes a TSL regExpRefPtr and has pointers to it's left and right children
	//Used for the differential function and evalRegExpAt0 and evalRegExp
	struct dFrame
	{
		dFrame(RTG::regExpRefPtr & e) : is_new(true), e(e), op(), left(), right(){}
		bool is_new;
		RTG::regExpRefPtr e;
		int op;
		RTG::regExpRefPtr left;
		RTG::regExpRefPtr right;
	};

	//A Stack frame which takes a TSL regExpTRefPtr and has pointers to it's left and right children, used for evalT
	struct sFrame
	{
		sFrame(RTG::regExpTRefPtr & e) : is_new(true), e(e), op(), left(), right(){}
		bool is_new;
		RTG::regExpTRefPtr e;
		int op;
		RTG::regExpTRefPtr left;
		RTG::regExpTRefPtr right;
	};

  short dump = false;
  char * mainProc = NULL, * errLbl = NULL;
  string fname;
  prog * pg;


  //this is used for the testing suite
  bool testMode = false;
  string testFileName;
  

  //Calls postar on the pds and wfa, also prints out .dot and .txt files with the initial wfa and final wfa.
  //Author: Prathmesh Prahbu
  void doPostStar(WPDS * pds, WFA& outfa)
  {
	//Create the initial wfa - with only the start and the accept states
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    fa.addTrans(getPdsState(),getEntryStk(pg, mainProc), acc, pds->get_theZero()->one());
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);

	//Dump the initial wfa to a dot and txt file
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
    if(fpds == NULL){ //We are using an ewpds, so no lazy transitions and therefore no IGR
      pds->poststar(fa,outfa); 
    }else{
      fpds->poststarIGR(fa,outfa);
    }

	//Dump the resultant wfa to dot and txt files
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
  
  /*
  *  Takes the final automata after poststar (or the newton solver) -- ETTODO - check this
  *  and returns the resultant path summary from a pruned version of the wfa with a new
  *  initial state that represents the error location.
  *
  *  The resultant weight represents all possible paths from the error state to the accept state
  *  in the wfa.
  *  
  *  If the weight is not null, then the error state is reachable in the program.
  *
  *  @param: out
  *  Input:  outfa - wfa representing the boolean program
  *  Return:  Weight representing all possible paths to the error location in the program.  Zero/False
			  if there is not path.
  *  Author: Emma Turetsky
  */
  sem_elem_t computePathSummary(WFA& outfa)
  {
    cout << "[Newton Compare] Checking error label reachability..." << endl;

	//Get the new initial key for the wfa - using what we know about how that state is labeled
	//That key will be stored in fKey
	Key errKey = wali::getKeySpace()->getKey("error");
	Key iKey = wali::getKeySpace()->getKey("Unique State Name");
	Key pErrKey = wali::getKeySpace()->getKey(iKey, errKey);
	GenKeySource * t = new wali::wpds::GenKeySource(1, pErrKey);
	Key fKey = wali::getKeySpace()->getKey(t);

    cout << "[Newton Compare] Computing path summary..." << endl;

	//First check to be sure the error state even exits (if it doesn't, error state is not reachable
	if (outfa.getState(fKey) != NULL)
	{
		//Prune outfa to be start at the error state and then call path_summary _tarjan_fwpds
		outfa.setInitialState(fKey);
		outfa.prune();
		outfa.path_summary_tarjan_fwpds(true, false);
		sem_elem_t wt2;
		//If the weight from the path summary isn't null, then return that value, otherwise return the False/Zero weight
		if (outfa.getState(outfa.getInitialState()) != NULL)
		{
			wt2 = outfa.getState(outfa.getInitialState())->weight();
		}
		else
		{
			wt2 = con->getBaseZero();
		}
		return wt2;
	}
	else //The error state doesn't exist
	{
		return con->getBaseZero();
	}
  }

  /*
  *  Determines if an error state in 'pg' is reachable using an npds.
  *
  *  Creates an npds representing the program with bdds as weights.  Runs a newton solver for WPDS queries, 
  *  using EWPDS solvers for the genearted linear equations. The correctness of this solver
  *  is uncertain
  *
  *  Author:  Prathmesh Prahbu
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
#if USE_NWAOBDD
	  con = pds_from_prog_nwa(npds, pg, true);
#else
	  con = pds_from_prog(npds, pg);
#endif
    }
	//freopen("nwaobddOut.txt", "w", stdout);
    //npds->print(std::cout) << endl;
    wali::set_verify_fwpds(false);


    wali::util::Timer * t = new wali::util::Timer("NWPDS poststar",cout);
    t->measureAndReport =false;
    doPostStar(npds, outfa);
    sem_elem_t wt = computePathSummary(outfa);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] NWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] NWPDS ==> error reachable" << endl;
    }
    t->print(std::cout << "[Newton Compare] Time taken by NWPDS poststar: ") << endl;
    delete t;

  }

/******
*
*
*  DuetTest
*
*/

  double duetTest(DuetRel* a, DuetRel * b)
{
  duetrel_t c = a->Compose(b);    // FIXME: Compose badly named: Compose should be Extend
  c->print(std::cout);
  return 0;
}
  /*
  *  Determines if an error state in 'pg' is reachable using an npds.
  *
  *  Creates an fpds representing the program with bdds (or nwaobdds) as weights.  Runs poststar and computes a
  *  path summary to find the results.
  *
  *  Author:  Prathmesh Prahbu
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
#if USE_NWAOBDD
	  con = pds_from_prog_with_meet_merge_nwa(fpds, pg);
#else
	  con = pds_from_prog_with_meet_merge(fpds, pg);
#endif
    }

    wali::set_verify_fwpds(false);
    fpds->useNewton(false);
	
	//freopen("testingLocalVars.txt", "w", stdout);
	//fpds->print(std::cout) << std::endl;
	double curTime;
    wali::util::GoodTimer * t = new wali::util::GoodTimer("FWPDS poststar");
    //t->measureAndReport =false;
    doPostStar(fpds, outfa);
    sem_elem_t wt = computePathSummary(outfa);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] FWPDS ==> error reachable" << endl;
    }
	t->stop();
	double totTime = t->total_time();
	std::cout << "[Newton Compare] Time taken by: FWPDS: ";
	cout << totTime << endl;

    return totTime;
  }
  
  /*
  *  Determines if an error state in 'pg' is reachable using an epds.
  *
  *  Creates an fpds representing the program with bdds as weights.  Runs poststar and computes a
  *  path summary to find the results.
  *
  *  Author: Prathmesh Prahbu
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
#if USE_NWAOBDD
	  con = pds_from_prog_with_meet_merge_nwa(pds, pg);
#else
	  con = pds_from_prog_with_meet_merge(pds, pg);//_with_meet_merge(pds, pg);
#endif
    }

	//freopen("testingEWPDS.txt", "w", stdout);
    //pds->print(std::cout) << endl;

    wali::util::GoodTimer * t = new wali::util::GoodTimer("EWPDS poststar");
    //t->measureAndReport =false;
    doPostStar(pds, outfa);
    //outfa.path_summary_iterative_original(outfa.getSomeWeight()->one());
    sem_elem_t wt = computePathSummary(outfa);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] EWPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] EWPDS ==> error reachable" << endl;
    }
	std::cout << "[Newton Compare] Time taken by: EWPDS: ";
    
    //delete t;
    t->stop();
    double totTime = t->total_time();
    cout << totTime << std::endl;
    return totTime;
  }

  /*
  *  Determines if an error state in 'pg' is reachable using a wpds.
  *
  *  Creates an fpds representing the program with bdds as weights.  Runs poststar and computes a
  *  path summary to find the results.
  *
  *  Author: Prathmesh Prahbu
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
#if USE_NWAOBDD
	  con = pds_from_prog_nwa(pds, pg, true);
#else
      con = pds_from_prog(pds, pg);
#endif
    }

    //pds->print(std::cout) << endl;


    wali::util::Timer * t = new wali::util::Timer("WPDS poststar",cout);
    t->measureAndReport =false;
    doPostStar(pds, outfa);
    sem_elem_t wt = computePathSummary(outfa);
    wt->print(std::cout);
    if(wt->equal(wt->zero()))
      cout << "[Newton Compare] WPDS ==> error not reachable" << endl;
    else{
      cout << "[Newton Compare] WPDS ==> error reachable" << endl;
    }
    t->print(std::cout << "[Newton Compare] Time taken by WPDS poststar: ") << endl;
    delete t;

    delete pds;
  }


  /*
  *  A non recursive version of the Differential function that uses a stack
  *  It takes in a TSL regular expression and returns a Tensored differential
  *
  *  This is based on the recursive TDifferential in regExp.tsl and uses that function's hashtable
  *
  *  @param:  RTG::regExpRefPtr exp - The top level regular expression to be differentiated
  *  @return:  RTG::regExpTListRefPtr - A list of the tensored differentials with respect to
  *									    all the variables in exp
  *
  *  Author: Emma Turetsky
  */
 RTG::regExpTListRefPtr nonRecTDiff(RTG::regExpRefPtr exp)
  {
	  //return CIR::TDifferential(exp);
	  //This stack represents the worklist, as long as it's not empty, there are still
	  //TSL regular expressions to be differentiated
	  std::stack<dFrame> todo;
	  std::map<RTG::regExpRefPtr, RTG::regExpTListRefPtr>::iterator it;

	  //Push the top frame onto the stack
	  todo.push(dFrame(exp));

	  //While there are still 
	  while (!todo.empty())
	  {
		  //Get the top stack frame
		  dFrame & frame = todo.top();
		  //If this is the first time we've seen this frame
		  if (frame.is_new){  //First check if this regularexpression has been differentiated before, if so then no need to differentiate again
							  //Just return the differentiated regular expression and pop the frame off the stack
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHash(frame.e);
			  TDiffHashMap::const_iterator TDifferential___it = TdiffMap.find(lookupKeyForTDifferentialHash);
			  if (TDifferential___it != TdiffMap.end()) {
				  todo.pop();
				  continue;
			  }
			  frame.is_new = false;

			  //Now determine what type of TSLRegExp this is - either a one with children (Kleene,Plus,Dot) or a leaf
			  //If it's not a leaf, determine if the children have been differentiated before.  If they haven't, push them
			  //on the stack.  If they have, differentiate the TSLRegExp and pop it from the stack
			  if (CIR::isKleene(frame.e).get_data())
			  {
				  frame.op = 0;
				  //Kleene only has one child
				  RTG::regExpRefPtr child = CIR::getLChild(frame.e);
				  frame.left = child;
				  //Look it up in the hash table
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHashL(child);
				  TDiffHashMap::const_iterator TDifferential___it = TdiffMap.find(lookupKeyForTDifferentialHashL);
				  //If it has been seen before
				  if (TDifferential___it != TdiffMap.end())
				  {
					  //Create the tensored differential for frame.e
					  RTG::regExpTListRefPtr ret;
					  RTG::regExpTRefPtr tmp = CIR::mkTensorTranspose(frame.e, frame.e);
					  ret = CIR::mapDotTOnRight(TDifferential___it->second, tmp);
					  //Add the resultant value to the hashmap and pop the stack frame
					  TdiffMap.insert(std::make_pair(lookupKeyForTDifferentialHash, ret));
					  todo.pop();
					  continue;
				  }
				  else
				  {
					  //Push the child TSL RegExp onto the stackframe
					  todo.push(dFrame(child));
					  continue;
				  }
			  }
			  else if (CIR::isDot(frame.e).get_data())
			  {
				  frame.op = 1;
				  //Dot has a left and right child
				  frame.left = CIR::getLChild(frame.e);
				  frame.right = CIR::getRChild(frame.e);
				  //Look up the left child in the hash table
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHashL(frame.left);
				  TDiffHashMap::const_iterator TDifferential___it = TdiffMap.find(lookupKeyForTDifferentialHashL);
				  //The left child has been differentiated before
				  if (TDifferential___it != TdiffMap.end())
				  {
					  RTG::regExpTListRefPtr lch = TDifferential___it->second;
					  //Look up the right child in the hash table
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHashR(frame.right);
					  TDiffHashMap::const_iterator TDifferential___it = TdiffMap.find(lookupKeyForTDifferentialHashR);
					  //If the right child has been differentiated before
					  if (TDifferential___it != TdiffMap.end())
					  {
						  //Create the tensored differential for frame.e
						  RTG::regExpTListRefPtr rch = TDifferential___it->second;
						  RTG::regExpTListRefPtr ret;
						  RTG::regExpTListRefPtr mma = CIR::mapDotTOnRight(lch, CIR::mkTensorTranspose(RTG::One::make(), frame.right));
						  RTG::regExpTListRefPtr mmb = CIR::mapDotTOnRight(rch, CIR::mkTensorTranspose(frame.left, RTG::One::make()));
						  ret = CIR::mapPlusT(mma, mmb);
						  //Add the resultant value to the hashmap and pop the stack frame
						  TdiffMap.insert(std::make_pair(lookupKeyForTDifferentialHash, ret));
						  todo.pop();
						  continue;
					  }
					  else  //The right child must be differentiated, so push it on to the stack frame
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
				  else //The left child must be differentiated, so push it on to the stack frame
				  {
					  todo.push(dFrame(frame.left));
					  //Look up the right child in the hash table
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHashR(frame.right);
					  TDiffHashMap::const_iterator TDifferential___it = TdiffMap.find(lookupKeyForTDifferentialHashR);
					  //If the right child also must be differentiated, push it on to the stack frame
					  if (TDifferential___it == TdiffMap.end())
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else if (CIR::isPlus(frame.e).get_data())
			  {
				  frame.op = 2;
				  //Plus has a left and right child
				  frame.left = CIR::getLChild(frame.e);
				  frame.right = CIR::getRChild(frame.e);
				  //Look up the left child in the hash table
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHashL(frame.left);
				  TDiffHashMap::const_iterator TDifferential___it = TdiffMap.find(lookupKeyForTDifferentialHashL);
				  //The left child has been differentiated before
				  if (TDifferential___it != TdiffMap.end())
				  {
					  RTG::regExpTListRefPtr lch = TDifferential___it->second;
					  //Look up the right child in the hash table
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHashR(frame.right);
					  TDiffHashMap::const_iterator TDifferential___it = TdiffMap.find(lookupKeyForTDifferentialHashR);
					  //If the right child has been differentiated before
					  if (TDifferential___it != TdiffMap.end())
					  {
						  //Create the tensored differential for frame.e
						  RTG::regExpTListRefPtr rch = TDifferential___it->second;
						  RTG::regExpTListRefPtr ret;
						  ret = CIR::mapPlusT(lch, rch);
						  //Add the resultant value to the hashmap and pop the stack frame
						  TdiffMap.insert(std::make_pair(lookupKeyForTDifferentialHash, ret));
						  todo.pop();
						  continue;
					  }
					  else //The right child must be differentiated, so push it on to the stack frame
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
				  else //The left child must be differentiated, so push it on to the stack frame
				  {
					  todo.push(dFrame(frame.left));
					  //Look up the right child in the hash table
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHashR(frame.right);
					  TDiffHashMap::const_iterator TDifferential___it = TdiffMap.find(lookupKeyForTDifferentialHashR);
					  //If the right child also must be differentiated, push it on to the stack frame
					  if (TDifferential___it == TdiffMap.end())
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else { 
				  ConcTSLInterface::regExpTListRefPtr T_a1;
				  switch (frame.e->GetClassId()) {
				  case RTG::TSL_ID_Var: {
					  RTG::VarRefPtr t_T_c1_scast__1 = static_cast<RTG::Var*>(frame.e.get_ptr());
					  BASETYPE::INT32 e_Var_1 = t_T_c1_scast__1->Get_V();
					  RTG::regExpTRefPtr T_c4 = RTG::OneT::make(); // TSL-spec: line 825 of "regExp.tsl"
					  ConcTSLInterface::regExpTListRefPtr T_c5 = RTG::RegExpTListNull::make(); // TSL-spec: line 825 of "regExp.tsl"
					  ConcTSLInterface::regExpTListRefPtr T_c1 = RTG::RegExpTListCons::make(e_Var_1, T_c4, T_c5); // TSL-spec: line 825 of "regExp.tsl"
					  T_a1 = T_c1;
					  TdiffMap.insert(std::make_pair(lookupKeyForTDifferentialHash, T_a1));
				  }
					  break;
				  case RTG::TSL_ID_Zero: {
					  ConcTSLInterface::regExpTListRefPtr T_c1 = RTG::RegExpTListNull::make(); // TSL-spec: line 826 of "regExp.tsl"
					  T_a1 = T_c1;
					  TdiffMap.insert(std::make_pair(lookupKeyForTDifferentialHash, T_a1));
				  }
					  break;
				  case RTG::TSL_ID_One: {
					  ConcTSLInterface::regExpTListRefPtr T_c1 = RTG::RegExpTListNull::make(); // TSL-spec: line 827 of "regExp.tsl"
					  T_a1 = T_c1;
					  TdiffMap.insert(std::make_pair(lookupKeyForTDifferentialHash, T_a1));
				  }
					  break;
				  case RTG::TSL_ID_Weight: {
					  RTG::WeightRefPtr t_T_c1_scast__1 = static_cast<RTG::Weight*>(frame.e.get_ptr());
					  EXTERN_TYPES::sem_elem_wrapperRefPtr e_Weight_1 = t_T_c1_scast__1->Get_weight();
					  ConcTSLInterface::regExpTListRefPtr T_c1 = RTG::RegExpTListNull::make(); // TSL-spec: line 828 of "regExp.tsl"
					  T_a1 = T_c1;
					  TdiffMap.insert(std::make_pair(lookupKeyForTDifferentialHash, T_a1));
				  }
					  break;
				  case RTG::TSL_ID_Project: {
					  RTG::ProjectRefPtr t_T_c1_scast__1 = static_cast<RTG::Project*>(frame.e.get_ptr());
					  BASETYPE::INT32 e_Project_1 = t_T_c1_scast__1->Get_MS();
					  BASETYPE::INT32 e_Project_2 = t_T_c1_scast__1->Get_MT();
					  RTG::regExpRefPtr e_Project_3 = t_T_c1_scast__1->Get_child();
					  frame.op = 3;
					  frame.left = e_Project_3;
					  todo.push(dFrame(frame.left));
					  continue;
				  }
				  }
				  todo.pop();
				  continue;
			  }
		  }
		  else  //This stack fram has been seen before, this means it's children will have been differentiated already and is not a leaf
		  {
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHash(frame.e);
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHashL(frame.left);
			  //Get the left child differentiated value (as all non leaves have a left child) and assign it to lch
			  TDiffHashMap::const_iterator TDifferential___it = TdiffMap.find(lookupKeyForTDifferentialHashL);
			  RTG::regExpTListRefPtr lch = TDifferential___it->second;
			  RTG::regExpTListRefPtr ret;
			  if (frame.op == 0) //Kleene
			  {
				  //Create the tensored differential and insert it into the hash_map
				  RTG::regExpTRefPtr tmp = CIR::mkTensorTranspose(frame.e, frame.e);
				  ret = CIR::mapDotTOnRight(lch, tmp);
				  TdiffMap.insert(std::make_pair(lookupKeyForTDifferentialHash, ret));
				  todo.pop();
				  continue;
			  }
			  else if (frame.op == 3) //Project
			  {
				  TdiffMap.insert(std::make_pair(lookupKeyForTDifferentialHash, lch));
				  todo.pop();
				  continue;
			  }
			  else  //Dot or Plus
			  {
				  //Both Dot and Plus have a right child, so look it up and assign it to rch
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForTDifferentialHashR(frame.right);
				  //frame.right->print(std::cout) << std::endl;
				  TDiffHashMap::const_iterator TDifferential___it = TdiffMap.find(lookupKeyForTDifferentialHashR);

				  RTG::regExpTListRefPtr rch = TDifferential___it->second;
				  RTG::regExpTListRefPtr ret;
				  if (frame.op == 1) //Dot
				  {
					  //Create the tensored Differential Value for Dot
					  RTG::regExpTListRefPtr mma = CIR::mapDotTOnRight(lch, CIR::mkTensorTranspose(RTG::One::make(), frame.right));
					  RTG::regExpTListRefPtr mmb = CIR::mapDotTOnRight(rch, CIR::mkTensorTranspose(frame.left, RTG::One::make()));
					  ret = CIR::mapPlusT(mma, mmb);
				  }
				  else
				  {
					  //Create the tensored Differential Value for Plus
					  ret = CIR::mapPlusT(lch, rch);
				  }
				  //Insert the resultant value into the hashmap
				  TdiffMap.insert(std::make_pair(lookupKeyForTDifferentialHash, ret));
				  todo.pop();
				  continue;
			  }
		  }
	  }

	  //This will start with a lookup on the hash_map for exp, which has been evaluated in the loop above, so will not re-evaluate using TDifferential
	  MemoCacheKey1<RTG::regExpRefPtr > fin(exp);
	  return TdiffMap[fin];
  }

  /*
  *  A non recursive version of the Differential function that uses a stack
  *  It takes in a TSL regular expression and returns a non-tensored differential
  *
  *  This is based on the recursive Differential in regExp.tsl and uses that function's hashtable
  *
  *  @param:  RTG::regExpRefPtr exp - The top level regular expression to be differentiated
  *  @return:  RTG::regExpListRefPtr - A list of the non-tensored differentials with respect to
  *									    all the variables in exp
  *
  *  Author: Emma Turetsky
  */
  RTG::regExpListRefPtr nonRecUntensoredDiff(RTG::regExpRefPtr exp)
  {
	  //return CIR::Differential(exp);
	  //std::cout << hits << std::endl;
	  //This stack represents the worklist, as long as it's not empty, there are still
	  //TSL regular expressions to be differentiated
	  std::stack<dFrame> todo;
	  std::map<RTG::regExpRefPtr, RTG::regExpListRefPtr>::iterator it;

	  //Push the top frame onto the stack
	  todo.push(dFrame(exp));


	  while (!todo.empty())
	  {
		  //Get the top stack frame
		  dFrame & frame = todo.top();
		  //If this is the first time we've seen this frame
		  if (frame.is_new){ //First check if this regularexpression has been differentiated before, if so then no need to differentiate again
							  //Just return the differentiated regular expression and pop the frame off the stack
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHash(frame.e);
			  DiffHashMap::const_iterator Differential___it = diffHMap.find(lookupKeyForDifferentialHash);
			  if (Differential___it != diffHMap.end()) {
				  todo.pop();
				  continue;
			  }
			  frame.is_new = false;

			  //Now determine what type of TSLRegExp this is - either a one with children (Kleene,Plus,Dot) or a leaf
			  //If it's not a leaf, determine if the children have been differentiated before.  If they haven't, push them
			  //on the stack.  If they have, differentiate the TSLRegExp and pop it from the stack
			  if (CIR::isKleene(frame.e).get_data())
			  {
				  frame.op = 0;
				  //Kleene only has one child
				  RTG::regExpRefPtr child = CIR::getLChild(frame.e);
				  frame.left = child;
				  //Look it up in the hash table
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHashL(child);
				  DiffHashMap::const_iterator Differential___it = diffHMap.find(lookupKeyForDifferentialHashL);
				  //If it has been seen before
				  if (Differential___it != diffHMap.end())
				  {
					  //Create the tensored differential for frame.e
					  RTG::regExpListRefPtr ret;
					  ret = CIR::mapDotBothSides(frame.e, Differential___it->second);
					  //Add the resultant value to the hashmap and pop the stack frame
					  diffHMap.insert(std::make_pair(lookupKeyForDifferentialHash, ret));
					  todo.pop();
					  continue;
				  }
				  else //Push the child TSL RegExp onto the stackframe
				  {
					  todo.push(dFrame(child));
					  continue;
				  }
			  }
			  else if (CIR::isDot(frame.e).get_data())
			  {
				  frame.op = 1;
				  //Dot has a left and right child
				  frame.left = CIR::getLChild(frame.e);
				  frame.right = CIR::getRChild(frame.e);
				  //Look up the left child in the hash table
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHashL(frame.left);
				  DiffHashMap::const_iterator Differential___it = diffHMap.find(lookupKeyForDifferentialHashL);
				  //If the left child has been differentiated before
				  if (Differential___it != diffHMap.end())
				  {
					  RTG::regExpListRefPtr lch = Differential___it->second;
					  //Look up the right child in the hash table
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHashR(frame.right);
					  DiffHashMap::const_iterator Differential___it = diffHMap.find(lookupKeyForDifferentialHashR);
					  //If the right child has been differentiated before
					  if (Differential___it != diffHMap.end())
					  {
						  //Create the tensored differential for frame.e
						  RTG::regExpListRefPtr rch = Differential___it->second;
						  RTG::regExpListRefPtr ret;
						  RTG::regExpListRefPtr mma = CIR::mapDotOnRight(lch, frame.right);
						  RTG::regExpListRefPtr mmb = CIR::mapDotOnLeft(frame.left, rch);
						  ret = CIR::mapPlus(mma, mmb);
						  //Add the resultant value to the hashmap and pop the stack frame
						  diffHMap.insert(std::make_pair(lookupKeyForDifferentialHash, ret));
						  todo.pop();
						  continue;
					  }
					  else  //The right child must be differentiated, so push it on to the stack frame
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
				  else //The left child must be differentiated, so push it on to the stack frame
				  {
					  todo.push(dFrame(frame.left));
					  //Look up the right child in the hash table
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHashR(frame.right);
					  DiffHashMap::const_iterator Differential___it = diffHMap.find(lookupKeyForDifferentialHashR);
					  //If the right child also must be differentiated, push it on to the stack frame
					  if (Differential___it == diffHMap.end())
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else if (CIR::isPlus(frame.e).get_data())
			  {
				  frame.op = 2;
				  //Plus has a left and right child
				  frame.left = CIR::getLChild(frame.e);
				  frame.right = CIR::getRChild(frame.e);
				  //Look up the left child in the hash table
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHashL(frame.left);
				  DiffHashMap::const_iterator Differential___it = diffHMap.find(lookupKeyForDifferentialHashL);
				  //If the left child has been differentiated before
				  if (Differential___it != diffHMap.end())
				  {
					  RTG::regExpListRefPtr lch = Differential___it->second;
					  //Look up the right child in the hash table
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHashR(frame.right);
					  DiffHashMap::const_iterator Differential___it = diffHMap.find(lookupKeyForDifferentialHashR);
					  //If the right child has been differentiated before
					  if (Differential___it != diffHMap.end())
					  {
						  //Create the tensored differential for frame.e
						  RTG::regExpListRefPtr rch = Differential___it->second;
						  RTG::regExpListRefPtr ret;
						  ret = CIR::mapPlus(lch, rch);
						  //Add the resultant value to the hashmap and pop the stack frame
						  diffHMap.insert(std::make_pair(lookupKeyForDifferentialHash, ret));
						  todo.pop();
						  continue;
					  }
					  else //The right child must be differentiated, so push it on to the stack frame
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
				  else //The left child must be differentiated, so push it on to the stack frame
				  {
					  todo.push(dFrame(frame.left));
					  //Look up the right child in the hash table
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHashR(frame.right);
					  DiffHashMap::const_iterator Differential___it = diffHMap.find(lookupKeyForDifferentialHashR);
					  //If the right child also must be differentiated, push it on to the stack frame
					  if (Differential___it == diffHMap.end())
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else {
				  //The value is a leaf, cheat and just call Differential.  This will add it to DifferentialHash (the hash map) and leverages
				  //the existing TSL spec.
				  //Pop the stack frame when done.
				  RTG::regExpListRefPtr T_a1;

				  switch (frame.e->GetClassId()) {
				  case RTG::TSL_ID_Var: {
					  RTG::VarRefPtr t_T_c1_scast__1 = static_cast<RTG::Var*>(frame.e.get_ptr());
					  BASETYPE::INT32 e_Var_1 = t_T_c1_scast__1->Get_V();
					  RTG::regExpRefPtr T_c5 = RTG::One::make(); // TSL-spec: line 1018 of "regExp.tsl"
					  RTG::regExpRefPtr T_c6 = RTG::One::make(); // TSL-spec: line 1018 of "regExp.tsl"
					  RTG::rePairListRefPtr T_c7 = RTG::RePairListNull::make(); // TSL-spec: line 1018 of "regExp.tsl"
					  RTG::rePairListRefPtr T_c4 = RTG::RePairListCons::make(T_c5, T_c6, T_c7); // TSL-spec: line 1018 of "regExp.tsl"
					  RTG::regExpListRefPtr T_c8 = RTG::RegExpListNull::make(); // TSL-spec: line 1018 of "regExp.tsl"
					  RTG::regExpListRefPtr T_c1 = RTG::RegExpListCons::make(e_Var_1, T_c4, T_c8); // TSL-spec: line 1018 of "regExp.tsl"
					  T_a1 = T_c1;
					  diffHMap.insert(std::make_pair(lookupKeyForDifferentialHash, T_a1));
				  }
					  break;
				  case RTG::TSL_ID_Zero: {
					  RTG::regExpListRefPtr T_c1 = RTG::RegExpListNull::make(); // TSL-spec: line 1019 of "regExp.tsl"
					  T_a1 = T_c1;
					  diffHMap.insert(std::make_pair(lookupKeyForDifferentialHash, T_a1));
				  }
					  break;
				  case RTG::TSL_ID_One: {
					  RTG::regExpListRefPtr T_c1 = RTG::RegExpListNull::make(); // TSL-spec: line 1020 of "regExp.tsl"
					  T_a1 = T_c1;
					  diffHMap.insert(std::make_pair(lookupKeyForDifferentialHash, T_a1));
				  }
					  break;
				  case RTG::TSL_ID_Weight: {
					  RTG::WeightRefPtr t_T_c1_scast__1 = static_cast<RTG::Weight*>(frame.e.get_ptr());
					  EXTERN_TYPES::sem_elem_wrapperRefPtr e_Weight_1 = t_T_c1_scast__1->Get_weight();
					  RTG::regExpListRefPtr T_c1 = RTG::RegExpListNull::make(); // TSL-spec: line 1021 of "regExp.tsl"
					  T_a1 = T_c1;
					  diffHMap.insert(std::make_pair(lookupKeyForDifferentialHash, T_a1));
				  }
					  break;
				  case RTG::TSL_ID_Project: {
					  RTG::ProjectRefPtr t_T_c1_scast__1 = static_cast<RTG::Project*>(frame.e.get_ptr());
					  RTG::regExpRefPtr e_Project_3 = t_T_c1_scast__1->Get_child();
					  frame.op = 3;
					  frame.left = e_Project_3;
					  todo.push(dFrame(frame.left));
					  continue;
				  }
				  }
					  todo.pop();
					  continue;
			  }
		  }
		  else //This stack fram has been seen before, this means it's children will have been differentiated already and is not a leaf
		  {
			  //Get the left child differentiated value (as all non leaves have a left child) and assign it to lch
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHash(frame.e);
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHashL(frame.left);
			  DiffHashMap::const_iterator Differential___it = diffHMap.find(lookupKeyForDifferentialHashL);
			  RTG::regExpListRefPtr lch = Differential___it->second;
			  RTG::regExpListRefPtr ret;
			  if (frame.op == 0) //Kleene
			  {
				  //Create the tensored differential and insert it into the hash_map
				  ret = CIR::mapDotBothSides(frame.e, lch);
				  diffHMap.insert(std::make_pair(lookupKeyForDifferentialHash, ret));
				  todo.pop();
				  continue;
			  }
			  else if (frame.op == 3)
			  {
				  diffHMap.insert(std::make_pair(lookupKeyForDifferentialHash, lch));
				  todo.pop();
				  continue;
			  }
			  else //Dot
			  {
				  //Both Dot and Plus have a right child, so look it up and assign it to rch
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForDifferentialHashR(frame.right);
				  //frame.right->print(std::cout) << std::endl;
				  DiffHashMap::const_iterator Differential___it = diffHMap.find(lookupKeyForDifferentialHashR);

				  RTG::regExpListRefPtr rch = Differential___it->second;
				  RTG::regExpListRefPtr ret;
				  if (frame.op == 1) //Dot
				  {
					  //Create the tensored Differential Value for Dot
					  RTG::regExpListRefPtr mma = CIR::mapDotOnRight(lch, frame.right);
					  RTG::regExpListRefPtr mmb = CIR::mapDotOnLeft(frame.left, rch);
					  ret = CIR::mapPlus(mma, mmb);
				  }
				  else
				  {
					  //Create the tensored Differential Value for Plus
					  ret = CIR::mapPlus(lch, rch);
				  }
				  //Insert the resultant value into the hashmap
				  diffHMap.insert(std::make_pair(lookupKeyForDifferentialHash, ret));
				  todo.pop();
				  continue;
			  }
		  }
	  }

	  //This will start with a lookup on the hash_map for exp, which has been evaluated in the loop above, so will not re-evaluate using Differential
	  MemoCacheKey1<RTG::regExpRefPtr > fin(exp);
	  return diffHMap[fin];
  }


  /*
  *  A non recursive version of evalAt0 used to eval TSLRegExp assuming variables are the zero sem_elem. (f(0)).
  *
  *  This is based on the recursive evalAt0 in regExp.tsl and uses that function's hastable
  *
  *  @param:  RTG::regExpRefPtr exp - The top level regular expression to be evaluated
  *  @return:  EXTERN_TYPES::sem_elem_wrapperRefPtr - A sem_elem wrapper around a sem_elem wt
  *
  *  Author: Emma Turetsky
  *
  */
   EXTERN_TYPES::sem_elem_wrapperRefPtr evalNonRecAt0(RTG::regExpRefPtr exp)
  {
	  //std::cout << hits << std::endl;
	  std::stack<dFrame> todo;
	  std::map<RTG::regExpRefPtr, EXTERN_TYPES::sem_elem_wrapperRefPtr>::iterator it;

	  todo.push(dFrame(exp));  //Push the expression on the stack as our starter frame

	  while (!todo.empty())
	  {
		  dFrame & frame = todo.top();
		  if (frame.is_new){  //See if we've seen this frame before
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0Hash(frame.e);  //See if we've evaluated this regexp already, if so, return evaluated value
			  EvalMap::const_iterator evalRegExpAt0___it = EvalMap0.find(lookupKeyForevalRegExpAt0Hash);
			  if (evalRegExpAt0___it != EvalMap0.end()) {
				  todo.pop();
				  continue;
			  }
			  frame.is_new = false;
			  //If the regexp is of type Kleene star
			  if (CIR::isKleene(frame.e).get_data())
			  {
				  frame.op = 0;
				  //Determine if the child has been seen before
				  RTG::regExpRefPtr child = CIR::getLChild(frame.e);
				  frame.left = child;
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0HashL(child);
				  EvalMap::const_iterator evalRegExpAt0___it = EvalMap0.find(lookupKeyForevalRegExpAt0HashL);
				  if (evalRegExpAt0___it != EvalMap0.end())
				  {
					  //If the child expression has been evaluated before, performe the Kleene Star operation and insert the resultant
					  //value into the hash table and pop the stack frame
					  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
					  ret = EXTERNS::evalKleeneSemElem(evalRegExpAt0___it->second);
					  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, ret));
					  todo.pop();
					  continue;
				  }
				  else
				  {
					  //Otherwise push the child onto the stack
					  todo.push(dFrame(child));
					  continue;
				  }
			  }
			  else if (CIR::isDot(frame.e).get_data())  //If the regexp is of type Dot
			  {
				  frame.op = 1;
				  //Determine if either child has been seen before
				  frame.left = CIR::getLChild(frame.e);
				  frame.right = CIR::getRChild(frame.e);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0HashL(frame.left);
				  EvalMap::const_iterator evalRegExpAt0___it = EvalMap0.find(lookupKeyForevalRegExpAt0HashL);
				  if (evalRegExpAt0___it != EvalMap0.end())  //The left child has been evaluated before
				  {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalRegExpAt0___it->second;
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0HashR(frame.right);
					  EvalMap::const_iterator evalRegExpAt0___it = EvalMap0.find(lookupKeyForevalRegExpAt0HashR);
					  //The right child has been evaluated before
					  if (evalRegExpAt0___it != EvalMap0.end())
					  {
						  //Both children have been evaluated at 0 already, so evaluate the Dot expression and put the value into the hash table
						  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalRegExpAt0___it->second;
						  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
						  ret = EXTERNS::evalDotSemElem(lch, rch);
						  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, ret));
						  todo.pop();
						  continue;
					  }
					  else  //Push the unevaluated right expression onto the stack
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
				  else
				  {
					  todo.push(dFrame(frame.left)); //Push the unevaluated left expression onto the stack
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0HashR(frame.right);
					  EvalMap::const_iterator evalRegExpAt0___it = EvalMap0.find(lookupKeyForevalRegExpAt0HashR);
					  //If the right child has not been seen before
					  if (evalRegExpAt0___it == EvalMap0.end())
					  {
						  //Push the unevaluated right expression onto the stack
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else if (CIR::isPlus(frame.e).get_data())
			  {
				  frame.op = 2;
				  //Determine if either child has been seen before
				  frame.left = CIR::getLChild(frame.e);
				  frame.right = CIR::getRChild(frame.e);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0HashL(frame.left);
				  EvalMap::const_iterator evalRegExpAt0___it = EvalMap0.find(lookupKeyForevalRegExpAt0HashL);
				  if (evalRegExpAt0___it != EvalMap0.end()) //The left child has been evaluated before
				  {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalRegExpAt0___it->second;
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0HashR(frame.right);
					  EvalMap::const_iterator evalRegExpAt0___it = EvalMap0.find(lookupKeyForevalRegExpAt0HashR);
					  if (evalRegExpAt0___it != EvalMap0.end())  //The right child has been evaluated before
					  {
						  //Both children have been evaluated at 0 already, so evaluate the Plus expression and put the value into the hash table
						  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalRegExpAt0___it->second;
						  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
						  ret = EXTERNS::evalPlusSemElem(lch, rch);
						  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, ret));
						  todo.pop();
						  continue;
					  }
					  else //Push the unevaluated right expression onto the stack
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
				  else
				  {
					  todo.push(dFrame(frame.left)); //Push the unevaluated left expression onto the stack
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0HashR(frame.right);
					  EvalMap::const_iterator evalRegExpAt0___it = EvalMap0.find(lookupKeyForevalRegExpAt0HashR);
					  //If the right child has not been seen before
					  if (evalRegExpAt0___it == EvalMap0.end())
					  {
						  //Push the unevaluated right expression onto the stack
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else {
				  switch (frame.e->GetClassId()) {
				  case RTG::TSL_ID_Var: {
					  RTG::VarRefPtr t_T_c1_scast__1 = static_cast<RTG::Var*>(frame.e.get_ptr());
					  BASETYPE::INT32 e_Var_1 = t_T_c1_scast__1->Get_V();
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = EXTERNS::getZeroWt(); // TSL-spec: line 680 of "regExp.tsl"
					  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_Zero: {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = EXTERNS::getZeroWt(); // TSL-spec: line 681 of "regExp.tsl"
					  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_One: {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = EXTERNS::getOneWt(); // TSL-spec: line 682 of "regExp.tsl"
					  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_Weight: {
					  RTG::WeightRefPtr t_e_Weight_1_scast__1 = static_cast<RTG::Weight*>(frame.e.get_ptr());
					  EXTERN_TYPES::sem_elem_wrapperRefPtr e_Weight_1 = t_e_Weight_1_scast__1->Get_weight();
					  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, e_Weight_1));
				  }
					  break;
				  case RTG::TSL_ID_Project: {
					  RTG::ProjectRefPtr t_T_c1_scast__1 = static_cast<RTG::Project*>(frame.e.get_ptr());
					  BASETYPE::INT32 e_Project_1 = t_T_c1_scast__1->Get_MS();
					  BASETYPE::INT32 e_Project_2 = t_T_c1_scast__1->Get_MT();
					  RTG::regExpRefPtr e_Project_3 = t_T_c1_scast__1->Get_child();
					  frame.op = 3;
					  frame.left = e_Project_3;
					  todo.push(dFrame(frame.left));
					  continue;
				  }
				  }
				  //The value is a leaf, cheat and just call evalRegExpAt0.  This will add it to evalRegExpAt0Hash (the hash map) and leverages
				  //the existing TSL spec.
				  //Pop the stack frame when done.
				  todo.pop();
				  continue;
			  }
		  }
		  else  //This is the second time we've popped this stack frame, so it's children must have been evaluated
		  {
			  //Look up the left child value (this is not a leaf regExp, so it must have a left child)
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0Hash(frame.e);
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0HashL(frame.left);
			  EvalMap::const_iterator evalRegExpAt0___it = EvalMap0.find(lookupKeyForevalRegExpAt0HashL);
			  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalRegExpAt0___it->second;
			  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
			  if (frame.op == 0) //Kleene
			  {
				  //Evaluate and insert into the hashmap
				  ret = EXTERNS::evalKleeneSemElem(lch);
				  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, ret));
				  todo.pop();
				  continue;
			  }
			  else if (frame.op == 3)
			  {
				  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, lch));
				  todo.pop();
				  continue;
			  }
			  else  //Dot or Plus
			  {
				  //Look up the value for the right child
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpAt0HashR(frame.right);
				  //frame.right->print(std::cout) << std::endl;
				  EvalMap::const_iterator evalRegExpAt0___it = EvalMap0.find(lookupKeyForevalRegExpAt0HashR);

				  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalRegExpAt0___it->second;
				  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
				  if (frame.op == 1) //Dot
				  {
					  //Evaluate and insert into the hashmap
					  ret = EXTERNS::evalDotSemElem(lch, rch);
					  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, ret));
					  todo.pop();
					  continue;
				  }
				  else //Plus
				  {
					  //Evaluate and insert into the hashmap
					  ret = EXTERNS::evalPlusSemElem(lch, rch);
					  EvalMap0.insert(std::make_pair(lookupKeyForevalRegExpAt0Hash, ret));
					  todo.pop();
					  continue;
				  }
			  }
		  }
	  }

	  //This will start with a lookup on the hash_map for exp, which has been evaluated in the loop above, so will not re-evaluate using evalRegExpAt0
	  MemoCacheKey1<RTG::regExpRefPtr > fin(exp);
	  return EvalMap0[fin];
  }

  /*
  *  A non recursive version of evalRegExpFin used to eval TSLRegExps given the final assignment a.
  *
  *  This is based on the recursive evalRegExpFin in regExp.tsl and uses that function's hastable
  *
  *  @param:  RTG::regExpRefPtr exp - The top level regular expression to be evaluated
  *			  RTG::assignmentRefPtr a - The final values for the variables in the regExp
  *  @return:  EXTERN_TYPES::sem_elem_wrapperRefPtr - A sem_elem wrapper around a sem_elem wt
  *
  *  Author: Emma Turetsky
  */
 EXTERN_TYPES::sem_elem_wrapperRefPtr evalRegExpFinNonRec(RTG::regExpRefPtr exp, RTG::assignmentRefPtr a)
  {
	  //std::cout << hits << std::endl;
	  std::stack<dFrame> todo;
	  std::map<RTG::regExpRefPtr, EXTERN_TYPES::sem_elem_wrapperRefPtr>::iterator it;

	  todo.push(dFrame(exp));
	  while (!todo.empty())
	  {
		  dFrame & frame = todo.top();
		  if (frame.is_new){ //See if we've seen this frame before
			  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHash(frame.e, a); //See if we've evaluated this regexp already, if so, return evaluated value
			  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHash);
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHash(frame.e); //See if we've evaluated this regexp already, if so, return evaluated value
			  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHash);
			  if (evalRegExpFin___it != EvalMap2.end()) {
				  todo.pop();
				  continue;
			  }
			  frame.is_new = false;
			  //If the regexp is of type Kleene star
			  if (CIR::isKleene(frame.e).get_data())
			  {
				  frame.op = 0;
				  //Determine if the child has been seen before
				  RTG::regExpRefPtr child = CIR::getLChild(frame.e);
				  frame.left = child;
				  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHashL(child, a);
				  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashL);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHashL(child);
				  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashL);
				  if (evalRegExpFin___it != EvalMap2.end())
				  {
					  //If the child expression has been evaluated before, performe the Kleene Star operation and insert the resultant
					  //value into the hash table and pop the stack frame
					  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
					  ret = EXTERNS::evalKleeneSemElem(evalRegExpFin___it->second);
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, ret));
					  todo.pop();
					  continue;
				  }
				  else
				  {
					  //Otherwise push the child onto the stack
					  todo.push(dFrame(child));
					  continue;
				  }
			  }
			  else if (CIR::isDot(frame.e).get_data())  //If the regexp is of type Dot
			  {
				  frame.op = 1;
				  //Determine if either child has been seen before
				  frame.left = CIR::getLChild(frame.e);
				  frame.right = CIR::getRChild(frame.e);
				  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHashL(frame.left, a);
				  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashL);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHashL(frame.left);
				  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashL);
				  if (evalRegExpFin___it != EvalMap2.end()) //The left child has been evaluated before
				  {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalRegExpFin___it->second;
					  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHashR(frame.right, a);
					  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashR);
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHashR(frame.right);
					  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashR);
					  if (evalRegExpFin___it != EvalMap2.end()) //The right child has been evaluated before
					  {
						  //Both children have been evaluated already, so evaluate the Dot expression and put the value into the hash table
						  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalRegExpFin___it->second;
						  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
						  ret = EXTERNS::evalDotSemElem(lch, rch);
						  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, ret));
						  todo.pop();
						  continue;
					  }
					  else //Push the unevaluated right expression onto the stack
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
				  else
				  {
					  todo.push(dFrame(frame.left)); //Push the unevaluated left expression onto the stack
					  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHashR(frame.right, a);
					  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashR);
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHashR(frame.right);
					  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashR);
					  if (evalRegExpFin___it == EvalMap2.end())
					  {
						  //Push the unevaluated right expression onto the stack
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else if (CIR::isPlus(frame.e).get_data())
			  {
				  frame.op = 2;
				  //Determine if either child has been seen before
				  frame.left = CIR::getLChild(frame.e);
				  frame.right = CIR::getRChild(frame.e);
				  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHashL(frame.left, a);
				  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashL);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHashL(frame.left);
				  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashL);
				  if (evalRegExpFin___it != EvalMap2.end()) //The left child has been evaluated before
				  {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalRegExpFin___it->second;
					  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHashR(frame.right, a);
					  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashR);
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHashR(frame.right);
					  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashR);
					  if (evalRegExpFin___it != EvalMap2.end()) //The right child has been evaluated before
					  {
						  //Both children have been evaluated already, so evaluate the Plus expression and put the value into the hash table
						  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalRegExpFin___it->second;
						  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
						  ret = EXTERNS::evalPlusSemElem(lch, rch);
						  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, ret));
						  todo.pop();
						  continue;
					  }
					  else
					  {
						  //Push the unevaluated right expression onto the stack
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
				  else
				  {
					  todo.push(dFrame(frame.left)); //Push the unevaluated left expression onto the stack
					  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHashR(frame.right, a);
					  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashR);
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHashR(frame.right);
					  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashR);
					  if (evalRegExpFin___it == EvalMap2.end())
					  {
						  //Push the unevaluated right expression onto the stack
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else if (CIR::isProject(frame.e).get_data())
			  {
				  //Get the children for project (MT = merge Target, MS = merge Src)
				  frame.op = 3;
				  RTG::ProjectRefPtr t_T_c1_scast__1 = static_cast<RTG::Project*>(frame.e.get_ptr());
				  BASETYPE::INT32 e_Project_1 = t_T_c1_scast__1->Get_MS();
				  BASETYPE::INT32 e_Project_2 = t_T_c1_scast__1->Get_MT();
				  RTG::regExpRefPtr e_Project_3 = t_T_c1_scast__1->Get_child();
				  frame.left = e_Project_3;
				  //if the child has been evaluated before
				  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHashL(frame.left, a);
				  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashL);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHashL(frame.left);
				  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashL);
				  if (evalRegExpFin___it != EvalMap2.end())
				  {
					  //Evaluate the projection and insert it into the hash table
					  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
					  ret = EXTERNS::evalProjectSemElem(e_Project_1, e_Project_2,evalRegExpFin___it->second);
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, ret));
					  todo.pop();
					  continue;
				  }
				  else
				  {
					  todo.push(dFrame(frame.left)); //Push the unevaluated child expression onto the stack
					  continue;
				  }
			  }
			  else {
				  //The value is a leaf, cheat and just call evalRegExpFin.  This will add it to evalRegExpFinHash (the hash map) and leverages
				  //the existing TSL spec.
				  //Pop the stack frame when done.
				  switch (frame.e->GetClassId()) {
				  case RTG::TSL_ID_Var: {
					  RTG::VarRefPtr t_T_c1_scast__1 = static_cast<RTG::Var*>(frame.e.get_ptr());
					  BASETYPE::INT32 e_Var_1 = t_T_c1_scast__1->Get_V();
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = CIR::getAssignment(e_Var_1, a); // TSL-spec: line 701 of "regExp.tsl"
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_Zero: {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = EXTERNS::getZeroWt(); // TSL-spec: line 702 of "regExp.tsl"
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_One: {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = EXTERNS::getOneWt(); // TSL-spec: line 703 of "regExp.tsl"
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_Weight: {
					  RTG::WeightRefPtr t_e_Weight_1_scast__1 = static_cast<RTG::Weight*>(frame.e.get_ptr());
					  EXTERN_TYPES::sem_elem_wrapperRefPtr e_Weight_1 = t_e_Weight_1_scast__1->Get_weight();
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, e_Weight_1));
				  }
				  }
				  todo.pop();
				  continue;
			  }
		  }
		  else  //This is the second time we've seen this value, so the children must have been evaluated at a, already
		  {
			  //Look up the children's evaluated values
			  //Determine the op code
			  //Call the appropriate evaluattion function depending on frame.op
			  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHash(frame.e, a);
			  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHashL(frame.left, a);
			  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashL);
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHash(frame.e);
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHashL(frame.left);
			  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashL);
			  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalRegExpFin___it->second;
			  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
			  if (frame.op == 0) //Kleene
			  {
				  ret = EXTERNS::evalKleeneSemElem(lch);
				  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, ret));
				  todo.pop();
				  continue;
			  }
			  else if (frame.op == 3)  //Project
			  {
				  RTG::ProjectRefPtr t_T_c1_scast__1 = static_cast<RTG::Project*>(frame. e.get_ptr());
				  BASETYPE::INT32 e_Project_1 = t_T_c1_scast__1->Get_MS();
				  BASETYPE::INT32 e_Project_2 = t_T_c1_scast__1->Get_MT();
				  ret = EXTERNS::evalProjectSemElem(e_Project_1, e_Project_2, lch);
				  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, ret));
				  todo.pop();
				  continue;
			  }
			  else //Dot or Plus
			  {
				  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpFinHashR(frame.right, a);
				  // //frame.right->print(std::cout) << std::endl;
				  // EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashR);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpFinHashR(frame.right);
				  //frame.right->print(std::cout) << std::endl;
				  EvalRMap::const_iterator evalRegExpFin___it = EvalMap2.find(lookupKeyForevalRegExpFinHashR);

				  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalRegExpFin___it->second;
				  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
				  if (frame.op == 1)  //Dot
				  {
					  ret = EXTERNS::evalDotSemElem(lch, rch);
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, ret));
					  todo.pop();
					  continue;
				  }
				  else  //Plus
				  {
					  ret = EXTERNS::evalPlusSemElem(lch, rch);
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpFinHash, ret));
					  todo.pop();
					  continue;
				  }
			  }
		  }
	  }

	  //This will start with a lookup on the hash_map for exp, which has been evaluated in the loop above, so will not re-evaluate using evalRegExpFin
	  //MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > fin(exp, a);
	  MemoCacheKey1<RTG::regExpRefPtr > fin(exp);
	  
	  return EvalMap2[fin];
  }

  /*
  *  A non recursive version of evalRegExp used to eval TSLRegExps given the assignment a.
  *
  *  This is based on the recursive evalRegExp in regExp.tsl and uses that function's hashtable
  *
  *  @param:  RTG::regExpRefPtr exp - The top level regular expression to be evaluated
  *           RTG::assignmentRefPtr a - The final values for the variables in the regExp
  *  @return:  EXTERN_TYPES::sem_elem_wrapperRefPtr - A sem_elem wrapper around a sem_elem wt
  *
  *  Author: Emma Turetsky
  */
  EXTERN_TYPES::sem_elem_wrapperRefPtr evalRegExpNonRec(RTG::regExpRefPtr exp, RTG::assignmentRefPtr a)
  {
	  //std::cout << hits << std::endl;
	  std::stack<dFrame> todo;
	  std::map<RTG::regExpRefPtr, EXTERN_TYPES::sem_elem_wrapperRefPtr>::iterator it;

	  todo.push(dFrame(exp));
	  while (!todo.empty())
	  {
		  dFrame & frame = todo.top();

		  //Determine if this frame has been seen before
		  if (frame.is_new){
			  //The frame has not been seen before, but frame.e may be pointing to a regexp that's been previously evaluated.  If so, return that result
			  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHash(frame.e, a);
			  // EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHash);
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHash(frame.e);
			  EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHash);
			  if (evalRegExp___it != EvalMap2.end()) {
				  todo.pop();
				  continue;
			  }
			  //frame.e has not been evaluated before, so lookup the children in the hash map.  If all the children of the frame.e have been evaluated, then evaluate frame.e, otherwise
			  //push the unevaluated children onto the stack
			  frame.is_new = false;
			  if (CIR::isKleene(frame.e).get_data())
			  {
				  frame.op = 0;
				  RTG::regExpRefPtr child = CIR::getLChild(frame.e);
				  frame.left = child;
				  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHashL(child, a);
				  // EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashL);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHashL(child);
				  EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashL);
				  if (evalRegExp___it != EvalMap2.end())
				  {
					  //EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
					  //ret = EXTERNS::evalKleeneSemElem(evalRegExp___it->second);
					  //EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, ret));
					  //todo.pop();
					  //std::cout << "New code! (U)" << std::endl;
					  duetrelpair_t ret;
					  ret = ((evalRegExp___it->second.v))->alphaHatStar();
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, ret->second));
				          MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForStar(child);
					  //std::cout << "  Hash key is: " << hash_value(lookupKeyForStar) << std::endl;
					  newStarVal.insert(std::make_pair(lookupKeyForStar, ret->first));
					  todo.pop();
					  continue;
				  }
				  else
				  {
					  todo.push(dFrame(child));
					  continue;
				  }
			  }
			  else if (CIR::isDot(frame.e).get_data())
			  {
				  frame.op = 1;
				  frame.left = CIR::getLChild(frame.e);
				  frame.right = CIR::getRChild(frame.e);
				  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHashL(frame.left, a);
				  // EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashL);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHashL(frame.left);
				  EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashL);
				  if (evalRegExp___it != EvalMap2.end())
				  {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalRegExp___it->second;
					  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHashR(frame.right, a);
					  // EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashR);
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHashR(frame.right);
					  EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashR);
					  if (evalRegExp___it != EvalMap2.end())
					  {
						  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalRegExp___it->second;
						  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
						  ret = EXTERNS::evalDotSemElem(lch, rch);
						  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, ret));
						  todo.pop();
						  continue;
					  }
					  else
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
				  else
				  {
					  todo.push(dFrame(frame.left));
					  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHashR(frame.right, a);
					  // EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashR);
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHashR(frame.right);
					  EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashR);
					  if (evalRegExp___it == EvalMap2.end())
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else if (CIR::isPlus(frame.e).get_data())
			  {
				  frame.op = 2;
				  frame.left = CIR::getLChild(frame.e);
				  frame.right = CIR::getRChild(frame.e);
				  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHashL(frame.left, a);
				  // EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashL);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHashL(frame.left);
				  EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashL);
				  if (evalRegExp___it != EvalMap2.end())
				  {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalRegExp___it->second;
					  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHashR(frame.right, a);
					  // EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashR);
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHashR(frame.right);
					  EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashR);
					  if (evalRegExp___it != EvalMap2.end())
					  {
						  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalRegExp___it->second;
						  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
						  ret = EXTERNS::evalPlusSemElem(lch, rch);
						  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, ret));
						  todo.pop();
						  continue;
					  }
					  else
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
				  else
				  {
					  todo.push(dFrame(frame.left));
					  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHashR(frame.right, a);
					  // EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashR);
					  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHashR(frame.right);
					  EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashR);
					  if (evalRegExp___it == EvalMap2.end())
					  {
						  todo.push(dFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else {  //frame.e is a leaf, so just leverage the TSL function as it's a base case (non recursive)
				  switch (frame.e->GetClassId()) {
				  case RTG::TSL_ID_Var: {
					  RTG::VarRefPtr t_T_c1_scast__1 = static_cast<RTG::Var*>(frame.e.get_ptr());
					  BASETYPE::INT32 e_Var_1 = t_T_c1_scast__1->Get_V();
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = CIR::getAssignment(e_Var_1, a); // TSL-spec: line 724 of "regExp.tsl"
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_Zero: {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = EXTERNS::getZeroWt(); // TSL-spec: line 725 of "regExp.tsl"
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_One: {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = EXTERNS::getOneWt(); // TSL-spec: line 726 of "regExp.tsl"
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_Weight: {
					  RTG::WeightRefPtr t_e_Weight_1_scast__1 = static_cast<RTG::Weight*>(frame.e.get_ptr());
					  EXTERN_TYPES::sem_elem_wrapperRefPtr e_Weight_1 = t_e_Weight_1_scast__1->Get_weight();
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, e_Weight_1));
				  }
					  break;
				  case RTG::TSL_ID_Project: {
					  RTG::ProjectRefPtr t_T_c1_scast__1 = static_cast<RTG::Project*>(frame.e.get_ptr());
					  BASETYPE::INT32 e_Project_1 = t_T_c1_scast__1->Get_MS();
					  BASETYPE::INT32 e_Project_2 = t_T_c1_scast__1->Get_MT();
					  RTG::regExpRefPtr e_Project_3 = t_T_c1_scast__1->Get_child();
					  frame.op = 3;
					  frame.left = e_Project_3;
					  todo.push(dFrame(frame.left));
					  continue;
				  }
				  }
				  todo.pop();
				  continue;
			  }
		  }
		  else
		  {
			  //This frame has been seen before, so it's children have all previously been evaluated.  So lookup the children in the hashtable and evaluate
			  //frame.e as appropriate based on what type of operation it is (determined in frame.op)
			  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHash(frame.e, a);
			  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHashL(frame.left, a);
			  // EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashL);
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHash(frame.e);
			  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHashL(frame.left);
			  EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashL);
			  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalRegExp___it->second;
			  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
			  if (frame.op == 0) //Kleene
			  {
				  //ret = EXTERNS::evalKleeneSemElem(lch);
				  //EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, ret));
				  //todo.pop();
				  //std::cout << "New code! (U)" << std::endl;
				  duetrelpair_t ret;
				  ret = ((evalRegExp___it->second.v))->alphaHatStar();
				  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, ret->second));
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForStar(frame.e);
				  //std::cout << "  Hash key is: " << hash_value(lookupKeyForStar) << std::endl;
				  newStarVal.insert(std::make_pair(lookupKeyForStar, ret->first));
                                  todo.pop();
				  continue;
			  }
			  else if (frame.op == 3)
			  {
				  RTG::ProjectRefPtr t_T_c1_scast__1 = static_cast<RTG::Project*>(frame.e.get_ptr());
				  BASETYPE::INT32 e_Project_1 = t_T_c1_scast__1->Get_MS();
				  BASETYPE::INT32 e_Project_2 = t_T_c1_scast__1->Get_MT();
				  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c7 = EXTERNS::evalProjectSemElem(e_Project_1, e_Project_2, lch);
				  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, T_c7));
				  todo.pop();
				  continue;
			  }
			  else
			  {
				  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > lookupKeyForevalRegExpHashR(frame.right, a);
				  // //frame.right->print(std::cout) << std::endl;
				  // EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashR);
				  MemoCacheKey1<RTG::regExpRefPtr > lookupKeyForevalRegExpHashR(frame.right);
				  //frame.right->print(std::cout) << std::endl;
				  EvalRMap::const_iterator evalRegExp___it = EvalMap2.find(lookupKeyForevalRegExpHashR);

				  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalRegExp___it->second;
				  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
				  if (frame.op == 1) //Dot
				  {
					  ret = EXTERNS::evalDotSemElem(lch, rch);
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, ret));
					  todo.pop();
					  continue;
				  }
				  else //Plus
				  {
					  ret = EXTERNS::evalPlusSemElem(lch, rch);
					  EvalMap2.insert(std::make_pair(lookupKeyForevalRegExpHash, ret));
					  todo.pop();
					  continue;
				  }
			  }
		  }
	  }

	  // MemoCacheKey2<RTG::regExpRefPtr, RTG::assignmentRefPtr > fin(exp, a);
	  MemoCacheKey1<RTG::regExpRefPtr > fin(exp);

	  //This will start with a lookup on the hash_map for exp, which has been evaluated in the loop above, so will not re-evaluate using evalRegExp
	  return EvalMap2[fin];
  }

  /*
  *  A non recursive test to check the linearity of the untensored portiona of a differential
  *
  *  @param:  RTG::regExpRefPtr rExp - The top level regular expression to be looked at
  *  @return:  bool linear - returns true if the elements of tList pass the test
  *
  *  Author: Emma Turetsky
  */
  bool checkLinearityHelperUntensored(RTG::regExpRefPtr rExp)
  {
	  std::stack<RTG::regExpRefPtr> workList;
	  bool linearity = true;

	  workList.push(rExp);
	  while (!workList.empty() && linearity)
	  {
		  RTG::regExpRefPtr currentRE = workList.top();
		  workList.pop();
		  if (CIR::isKleene(currentRE).get_data())
		  {
			  RTG::regExpRefPtr child = CIR::getLChild(currentRE);
			  workList.push(child);
		  }
		  else if (CIR::isPlus(currentRE).get_data())
		  {
			  RTG::regExpRefPtr left = CIR::getLChild(currentRE);
			  RTG::regExpRefPtr right = CIR::getRChild(currentRE);
			  workList.push(left);
			  workList.push(right);
		  }
		  else if (CIR::isDot(currentRE).get_data())
		  {
			  RTG::regExpRefPtr left = CIR::getLChild(currentRE);
			  RTG::regExpRefPtr right = CIR::getRChild(currentRE);
			  workList.push(left);
			  workList.push(right);
		  }
		  else if (CIR::isProject(currentRE).get_data())
		  {
			  RTG::regExpRefPtr child = CIR::getLChild(currentRE);
			  workList.push(child);
		  }
		  else if (CIR::isVar(currentRE).get_data())
		  {
			  linearity = false;
		  }
	  }
	  return linearity;

  }

  /*
  *  A non recursive test to check the linearity of tensored partial differential
  *
  *  @param:  RTG::regExpTRefPtr rExpT - The top level regular expression to be looked at
  *  @return:  bool linear - returns true if the elements of tList pass the test
  *
  *  Author: Emma Turetsky
  */
  bool checkLinearityHelper(RTG::regExpTRefPtr rExpT)
  {
	  std::stack<RTG::regExpTRefPtr> workList;
	  bool linearity = true;

	  workList.push(rExpT);
	  while (!workList.empty() && linearity)
	  {
		  RTG::regExpTRefPtr currentRET = workList.top();
		  workList.pop();
		  if (CIR::isKleeneT(currentRET).get_data())
		  {
			  RTG::regExpTRefPtr child = CIR::getLChildT(currentRET);
			  workList.push(child);
		  }
		  else if (CIR::isDotT(currentRET).get_data())
		  {
			  RTG::regExpTRefPtr left = CIR::getLChildT(currentRET);
			  RTG::regExpTRefPtr right = CIR::getRChildT(currentRET);
			  workList.push(left);
			  workList.push(right);
		  }
		  else if (CIR::isPlusT(currentRET).get_data())
		  {
			  RTG::regExpTRefPtr left = CIR::getLChildT(currentRET);
			  RTG::regExpTRefPtr right = CIR::getRChildT(currentRET);
			  workList.push(left);
			  workList.push(right);
		  }
		  else if (CIR::isProjectT(currentRET).get_data()){
			  RTG::regExpTRefPtr child = CIR::getLChildT(currentRET);
			  workList.push(child);
		  }
		  else if (CIR::isTensorTranspose(currentRET).get_data())
		  {
			  RTG::TensorTransposeRefPtr t_T_c1_scast__1 = static_cast<RTG::TensorTranspose*>(currentRET.get_ptr());
			  RTG::regExpRefPtr lch = t_T_c1_scast__1->Get_lChild();
			  RTG::regExpRefPtr rch = t_T_c1_scast__1->Get_rChild();
			  bool linL = checkLinearityHelperUntensored(lch);
			  if (linL)
			  {
				  linearity = checkLinearityHelperUntensored(rch);
			  }
			  else
			  {
				  linearity = false;
			  }
		  }
	  }
	  return linearity;
  }

  /*
  *  A non recursive test to check the linearity of tensored differential
  *
  *  @param:  RTG::regExpTListRefPtr tList - The top level regularexpression list to be looked at
  *  @return:  bool linear - returns true if the elements of tList pass the test
  *
  *  Author: Emma Turetsky
  */
  bool checkLinearity(RTG::regExpTListRefPtr tList)
  {
	  RTG::regExpTListRefPtr nextTList = tList;
	  bool linearity = true;
	  bool sentinal = true;
	  while (sentinal && linearity)  //Run until we've failed the test or reach the end of the tList
	  {
		  RTG::regExpTListRefPtr currentTList = nextTList;
		  if (CIR::isNullTList(currentTList).get_data()){ //Check to see if we've reached the end of the tList
			  sentinal = false;
		  }
		  else
		  {
			  RTG::regExpTRefPtr rExpT = CIR::getRegExpTFromTList(tList);
			  linearity = checkLinearityHelper(rExpT);  //check the linearity of the tensored regular expression
		  }
		  nextTList = CIR::getNextTList(currentTList);
	  }
	  
	  return linearity;
  }


  /*
  *  A non recursive version of evalT used to eval TSLRegExps representing tensored weights
  *  given the assignment a.
  *
  *  This is based on the recursive evalT in regExp.tsl and uses that function's hashtable
  *
  *  @param:  RTG::regExpTRefPtr exp - The top level tensored regular expression to be evaluated
  *           RTG::assignmentRefPtr a - The final values for the variables in the regExp
  *  @return:  EXTERN_TYPES::sem_elem_wrapperRefPtr - A sem_elem wrapper around a tensored sem_elem wt
  *
  *  Author: Emma Turetsky
  */
  EXTERN_TYPES::sem_elem_wrapperRefPtr evalTNonRec(RTG::regExpTRefPtr exp, RTG::assignmentRefPtr a)
  {
	  //std::cout << hits << std::endl;
	  std::stack<sFrame> todo;
	  std::map<RTG::regExpTRefPtr, EXTERN_TYPES::sem_elem_wrapperRefPtr>::iterator it;

	  todo.push(sFrame(exp));
	  while (!todo.empty())
	  {
		  sFrame & frame = todo.top();
		  if (frame.is_new){  //Determine if this is the first time looking at this frame
			  //Check frame.e see if it's been evaluated before.
			  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHash(frame.e, a);
			  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHash);
			  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHash(frame.e);
			  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHash);
			  if (evalT___it != EvalMapT.end()) {
				  todo.pop();
				  continue;
			  }
			  frame.is_new = false;
			  //frame.e has not been evaluated before - check to see if the children have been seen before
			  //If so, evaluate frame.e as appropriate, otherwise push the children on to the stack
			  if (CIR::isKleeneT(frame.e).get_data())
			  {
				  frame.op = 0;
				  RTG::regExpTRefPtr child = CIR::getLChildT(frame.e);
				  frame.left = child;
				  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHashL(child, a);
				  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashL);
				  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHashL(child);
				  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashL);
				  if (evalT___it != EvalMapT.end())
				  {
				  	  //std::cout << "New code! (T1)" << std::endl;
					  duetrelpair_t ret;
					  ret = ((evalT___it->second.v))->alphaHatStar();
					  // std::cout << "  Body value is: " << std::endl;
					  // evalT___it->second.v->print(std::cout);
					  // std::cout << std::endl;
					  // std::cout << "  Linearized value is: " << std::endl;
					  // ret->first->print(std::cout);
					  // std::cout << std::endl;
					  // std::cout << "  Star value is: " << std::endl;
					  // ret->second->print(std::cout);
					  // std::cout << std::endl;

					  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret->second));
				  	  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForStar(child);
					  newStarValT.insert(std::make_pair(lookupKeyForStar, ret->first));
					  todo.pop();
					  continue;
				  }
				  else
				  {
					  todo.push(sFrame(child));
					  continue;
				  }
			  }
			  else if (CIR::isDotT(frame.e).get_data())
			  {
				  frame.op = 1;
				  frame.left = CIR::getLChildT(frame.e);
				  frame.right = CIR::getRChildT(frame.e);
				  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHashL(frame.left, a);
				  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashL);
				  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHashL(frame.left);
				  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashL);
				  if (evalT___it != EvalMapT.end())
				  {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalT___it->second;
					  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHashR(frame.right, a);
					  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashR);
					  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHashR(frame.right);
					  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashR);
					  if (evalT___it != EvalMapT.end())
					  {
						  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalT___it->second;
						  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
						  ret = EXTERNS::evalDotSemElemT(lch, rch);
						  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret));
						  todo.pop();
						  continue;
					  }
					  else
					  {
						  todo.push(sFrame(frame.right));
						  continue;
					  }
				  }
				  else
				  {
					  todo.push(sFrame(frame.left));
					  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHashR(frame.right, a);
					  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashR);
					  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHashR(frame.right);
					  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashR);
					  if (evalT___it == EvalMapT.end())
					  {
						  todo.push(sFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else if (CIR::isPlusT(frame.e).get_data())
			  {
				  frame.op = 2;
				  frame.left = CIR::getLChildT(frame.e);
				  frame.right = CIR::getRChildT(frame.e);
				  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHashL(frame.left, a);
				  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashL);
				  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHashL(frame.left);
				  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashL);
				  if (evalT___it != EvalMapT.end())
				  {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalT___it->second;
					  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHashR(frame.right, a);
					  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashR);
					  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHashR(frame.right);
					  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashR);
					  if (evalT___it != EvalMapT.end())
					  {
						  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalT___it->second;
						  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
						  ret = EXTERNS::evalPlusSemElemT(lch, rch);
						  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret));
						  todo.pop();
						  continue;
					  }
					  else
					  {
						  todo.push(sFrame(frame.right));
						  continue;
					  }
				  }
				  else
				  {
					  todo.push(sFrame(frame.left));
					  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHashR(frame.right, a);
					  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashR);
					  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHashR(frame.right);
					  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashR);
					  if (evalT___it == EvalMapT.end())
					  {
						  todo.push(sFrame(frame.right));
						  continue;
					  }
				  }
			  }
			  else if (CIR::isProjectT(frame.e).get_data()){
					  frame.op = 3;
					  RTG::ProjectTRefPtr t_T_b1_scast__1 = static_cast<RTG::ProjectT*>(frame.e.get_ptr());
					  RTG::regExpTRefPtr e_ProjectT_3 = t_T_b1_scast__1->Get_child();
					  BASETYPE::INT32 e_ProjectT_1 = t_T_b1_scast__1->Get_MS();
					  BASETYPE::INT32 e_ProjectT_2 = t_T_b1_scast__1->Get_MT();
					  frame.left = e_ProjectT_3;
					  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHashL(frame.left, a);
					  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashL);
					  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHashL(frame.left);
					  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashL);
					  if (evalT___it != EvalMapT.end())
					  {
						  // std::cout << "This value appears inside a ProjectT:" << std::endl;
						  // evalT___it->second.print(std::cout);
						  // std::cout << std::endl;
						  EXTERN_TYPES::sem_elem_wrapperRefPtr ret = EXTERNS::evalProjectSemElemT(e_ProjectT_1, e_ProjectT_2, evalT___it->second);
						  // std::cout << "Evaluating ProjectT gave us:" << std::endl;
						  // ret.print(std::cout);
						  // std::cout << std::endl;
						  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret));
						  todo.pop();
						  continue;
					  }
					  /*
					 else if (CIR::isDotT(e_ProjectT_3).get_data())
					  {
						  RTG::regExpTRefPtr aDot = CIR::getLChildT(e_ProjectT_3);
						  RTG::regExpTRefPtr bDot = CIR::getRChildT(e_ProjectT_3);
						  if (CIR::isTensorTranspose(aDot).get_data())
						  {
							  if (CIR::isTensorTranspose(bDot).get_data())
							  {
								  RTG::TensorTransposeRefPtr t_T_c1_scast__1 = static_cast<RTG::TensorTranspose*>(aDot.get_ptr());
								  RTG::regExpRefPtr w = t_T_c1_scast__1->Get_lChild();
								  RTG::regExpRefPtr x = t_T_c1_scast__1->Get_rChild();
								  RTG::TensorTransposeRefPtr t_T_c1_scast__2 = static_cast<RTG::TensorTranspose*>(bDot.get_ptr());
								  RTG::regExpRefPtr y = t_T_c1_scast__2->Get_lChild();
								  RTG::regExpRefPtr z = t_T_c1_scast__2->Get_rChild();
								  RTG::regExpRefPtr lch = CIR::mkDot(y, w);
								  RTG::regExpRefPtr rch = CIR::mkDot(x, z);
								  EXTERN_TYPES::sem_elem_wrapperRefPtr mVal = evalRegExpNonRec(rch, a);
								  EXTERN_TYPES::sem_elem_wrapperRefPtr fVal = evalRegExpNonRec(lch, a);
								  EXTERN_TYPES::sem_elem_wrapperRefPtr mergedVal = EXTERNS::evalProjectSemElem(e_ProjectT_1, e_ProjectT_2, mVal);
								  EXTERN_TYPES::sem_elem_wrapperRefPtr eVal = EXTERNS::evalDotSemElem(fVal, mergedVal);
								  EXTERN_TYPES::sem_elem_wrapperRefPtr ret = EXTERNS::evalTensorTranspose(EXTERNS::getOneWt(), eVal);
								  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret));
								  todo.pop();
								  continue;
							  }
						  }
					  }
					  */
					  todo.push(frame.left);
					  continue;
				  }
			  else if (CIR::isTensorTranspose(frame.e).get_data())
				  {
					  RTG::TensorTransposeRefPtr t_T_c1_scast__1 = static_cast<RTG::TensorTranspose*>(frame.e.get_ptr());
					  RTG::regExpRefPtr lch = t_T_c1_scast__1->Get_lChild();
					  RTG::regExpRefPtr rch = t_T_c1_scast__1->Get_rChild();
					  EXTERN_TYPES::sem_elem_wrapperRefPtr lVal = evalRegExpNonRec(lch, a);
					  EXTERN_TYPES::sem_elem_wrapperRefPtr rVal = evalRegExpNonRec(rch, a);
					  EXTERN_TYPES::sem_elem_wrapperRefPtr ret = EXTERNS::evalTensorTranspose(lVal, rVal);
					  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret));
					  todo.pop();
					  continue;
				  }
			  else  //Leaf tensored regexp, so call evalT as it's a base case
			  {
				  switch (frame.e->GetClassId()) {
				  case RTG::TSL_ID_VarT: {
					  RTG::VarTRefPtr t_T_c1_scast__1 = static_cast<RTG::VarT*>(frame.e.get_ptr());
					  BASETYPE::INT32 e_VarT_1 = t_T_c1_scast__1->Get_VT();
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = EXTERNS::getZeroTWt(); // TSL-spec: line 751 of "regExp.tsl"
					  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_ZeroT: {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = EXTERNS::getZeroTWt(); // TSL-spec: line 752 of "regExp.tsl"
					  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, T_c1));
				  }
					  break;
				  case RTG::TSL_ID_OneT: {
					  EXTERN_TYPES::sem_elem_wrapperRefPtr T_c1 = EXTERNS::getOneTWt(); // TSL-spec: line 753 of "regExp.tsl"
					  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, T_c1));
				  }
				  }
				  todo.pop();
				  continue;

			  }
		  }
		  else  //We've seen this stack frame before
		  {
			  //All the children of frame.e have been evaluated, so evaluate frame.e as appropriate
			  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHash(frame.e, a);
			  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHashL(frame.left, a);
			  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashL);
			  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHash(frame.e);
			  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHashL(frame.left);
			  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashL);
			  EXTERN_TYPES::sem_elem_wrapperRefPtr lch = evalT___it->second;
			  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
			  if (frame.op == 0) //Kleene
			  {
			  	  //std::cout << "New code! (T2)" << std::endl;
				  duetrelpair_t ret;
				  ret = ((evalT___it->second.v))->alphaHatStar();
				  // std::cout << "  Body value is: " << std::endl;
				  // evalT___it->second.v->print(std::cout);
				  // std::cout << std::endl;
				  // std::cout << "  Linearized value is: " << std::endl;
				  // ret->first->print(std::cout);
				  // std::cout << std::endl;
				  // std::cout << "  Star value is: " << std::endl;
				  // ret->second->print(std::cout);
				  // std::cout << std::endl;
				  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret->second));
				  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForStar(frame.e); // FIXME this variable is now redundant with lookupKeyForevalTHash
				  newStarValT.insert(std::make_pair(lookupKeyForStar, ret->first));
				  //ret = EXTERNS::evalKleeneSemElemT(lch);
				  //EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret));
				  todo.pop();
				  continue;
			  }
			  else if (frame.op == 3) //Project
			  {
				  RTG::ProjectTRefPtr t_T_b1_scast__1 = static_cast<RTG::ProjectT*>(frame.e.get_ptr());
				  BASETYPE::INT32 e_ProjectT_1 = t_T_b1_scast__1->Get_MS();
				  BASETYPE::INT32 e_ProjectT_2 = t_T_b1_scast__1->Get_MT();
				  // std::cout << "This value appears inside a ProjectT:" << std::endl;
				  // lch.print(std::cout);
				  // std::cout << std::endl;
				  ret = EXTERNS::evalProjectSemElemT(e_ProjectT_1, e_ProjectT_2, lch);
				  // std::cout << "Evaluating ProjectT gave us:" << std::endl;
				  // ret.print(std::cout);
				  // std::cout << std::endl;
				  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret));
				  todo.pop();
				  continue;
			  }
			  else //Dot or Plust
			  {
				  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > lookupKeyForevalTHashR(frame.right, a);
				  // //frame.right->print(std::cout) << std::endl;
				  // EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashR);
				  MemoCacheKey1<RTG::regExpTRefPtr > lookupKeyForevalTHashR(frame.right);
				  //frame.right->print(std::cout) << std::endl;
				  EvalTMap::const_iterator evalT___it = EvalMapT.find(lookupKeyForevalTHashR);

				  EXTERN_TYPES::sem_elem_wrapperRefPtr rch = evalT___it->second;
				  EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
				  if (frame.op == 1)  //Dot
				  {
					  ret = EXTERNS::evalDotSemElemT(lch, rch);
					  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret));
					  todo.pop();
					  continue;
				  }
				  else  //Plus
				  {
					  ret = EXTERNS::evalPlusSemElemT(lch, rch);
					  EvalMapT.insert(std::make_pair(lookupKeyForevalTHash, ret));
					  todo.pop();
					  continue;
				  }
			  }
		  }
	  }

	  // MemoCacheKey2<RTG::regExpTRefPtr, RTG::assignmentRefPtr > fin(exp, a);
	  MemoCacheKey1<RTG::regExpTRefPtr > fin(exp);
	  return EvalMapT[fin];  //Grabs exp evaluated at assignment a from the hash_map
  }

  /*
  * Create the tensored differentials from the regular expressions of nodes
  * with epsilon transitions and populate the map
  *
  * @params:  tslRegExpMap & diffMap - reference to map from varID to it's corresponding TSLRegExp
  *			 tslDiffMap & differentialMap - reference to map from varID to it's list of TSL tensoredDifferentials
  *
  * @author:  Emma Turetsky 
  */
  bool createDifferentials(tslRegExpMap & diffMap, tslDiffMap & differentialMap){
	  bool linear = true;
	  for (tslRegExpMap::iterator it = diffMap.begin(); it != diffMap.end(); ++it)
	  {
		 // std::cout << "EMap: " << it->first << ": ";
		 // it->second.print(std::cout);
		 // std::cout << std::endl;
		  RTG::regExpTListRefPtr rtList = nonRecTDiff(it->second);
		  if (linear)
		  {
			  linear = checkLinearity(rtList);
		  }
		  differentialMap[it->first] = rtList;
		 // std::cout << "Diff: " << it->first << ": ";
		 // rtList->print(std::cout);
		 // std::cout << std::endl;
	  }
	  return linear;
  }


  /*
  * Create the differentials from the regular expressions of nodes
  * with epsilon transitions and populate the map
  *
  * @params:  tslRegExpMap & diffMap - reference to map from varID to it's corresponding TSLRegExp
  *			  tslUntensoredDiffMap & differentialMap - reference to map from varID to it's list of TSL non-Differentials
  *
  * @author:  Emma Turetsky
  */
  void createUntensoredDifferentials(tslRegExpMap & diffMap, tslUntensoredDiffMap & untensoredDifferentialMap) {
	  //Also, for each Differential, create a enter and exit state in the keyspace for the differential
	  for (tslRegExpMap::iterator it = diffMap.begin(); it != diffMap.end(); ++it)
	  {
		  //std::cout << it->first << ": ";
		 // it->second.print(std::cout) << endl;
		  RTG::regExpListRefPtr rList = nonRecUntensoredDiff(it->second);
		  untensoredDifferentialMap[it->first] = rList;
		 // rList.print(std::cout) << endl;
	  }
  }


 /*
  *  convertToRegExpT
  *
  *  Convert a WALi NameWeight regexp to a tensored regular expression
  *
  *  This procedure implements Step (5) of Algorithm NPA-TP from TR-1825 (page 33)
  *  http://research.cs.wisc.edu/wpis/papers/tr1825.pdf
  *
  *  In the terms of that paper, this procedure is used to create the
  *  set of regular expressions that are used to evaluate the
  *  variables Z_i in the left-linear equation system over the vector
  *  of variables Z:
  *
  *    Z = (1^t tensor f(0)) combine_T D_T f|_nu(Z).       (*)
  *
  *  At top-level, the value of parameter "exp" is a WALi regular
  *  expression over constants of type NameWeight (for some variable
  *  Z_i). It will be converted into a tensored regular expression
  *  that expresses how to compute the solution for Z_i on a given
  *  Newton round.
  *
  *  Method. Recursively traverse exp:
  *
  *   o convert each regular-expression operator into the analogous
  *     tensored operator
  *
  *   o convert each leaf NameWeight(j,i) into the tensored regular
  *     expression RE_T[j,i], where RE_T[j,i] is the tensored regular
  *     expression in the i-th equation of equation-system (*)
  *
  *       Z_i = (1^t tensor f_i(0)) combine_T ... combine_T (Z_j *_T RE_T[j,i]) combine_T ...
  *
  *  params: reg_exp_t exp - a WALi NameWeight regular expression
  *          tslRegExpMap & regExpMap - map from varId to the TSL regExp representing it
  *          tslDiffMap & differentialMap - map from varId to list of partial differentials
  *                                         representing varId differentiated with
  *                                         respect to different variables
  *          mapBack - a map for associating different variables with the
  *                    appropriate variableID, needed because of the different node names
  *                    in the fwpds IGR - should try to eliminate this need if possible
  *
  *  return: RTG::regExpTRefPtr - the tensored TSLRegexp that
  *           represents exp converted in the manner described
  *           above. The return value at top-level expresses how to
  *           compute the solution for some variable Z_i on a given
  *           Newton round.
  *
  *  Author:  Emma Turetsky
  */
  RTG::regExpTRefPtr convertToRegExpT(reg_exp_t exp, tslRegExpMap & regExpMap, tslDiffMap & differentialMap, std::map<int, std::pair<std::pair<int, int>, int> > & mapBack, map<std::pair<int, int>, std::pair<int, int> >  & mergeSrcMap)
  {
	  if (exp->isConstant()) // Encountered a NameWeight
                                // Create an appropriate tensored regular expression that reflects the NameWeight's
                                // role in the Z equation system
	  {
		  NameWeight * nw = static_cast<wali::domains::name_weight::NameWeight*>(exp->get_weight().get_ptr());
		  int var = nw->getName1();
		  int reID = nw->getName2();
		  //std::cout << "Looking Up: " << reID << "," << var << std::endl;

		  RTG::regExpTRefPtr d;
                // The Z equation-system has an equation of the form
                //    Z_reID = f_reId(0) combine ... combine_T (Z_var *_T RE_T[var,reID]) combine_T ...
		  if (var == -1){ // Create f_reId(0): NameWeight(dummy, reID) stands for f_reId(0)
                                // in the FWPDS encoding of the Z equation-system's dependence graph
			  if (reID == -1){  // Should never occur; return (1^t tensor 1)
				  return RTG::OneT::make();
			  }
			  else { // From the regular expression r for reID, create (1^t tensor r(0))
                               // Look up the appropriate regexp for reID and evaluate at 0
				  RTG::regExpRefPtr r = regExpMap[reID];
				  EXTERN_TYPES::sem_elem_wrapperRefPtr newVal = evalNonRecAt0(r);
                  //std::cout << "***Output with evalNonRecAt0:" << std::endl;
			      //newVal.v->print(std::cout);
                  //std::cout << std::endl;
				  //EXTERN_TYPES::sem_elem_wrapperRefPtr temp = CIR::evalRegExpAt0(r);
                  //std::cout << "***Output with evalRegExpAt0:" << std::endl;
      			  //temp.v->print(std::cout);
                  //std::cout << std::endl;
				  d = CIR::mkTensorTranspose(RTG::One::make(), RTG::Weight::make(newVal));
			  }
		  }
		  else { // Create the tensored regular expression RE_T[var,reID]
                       // that corresponds to the dependence of reID on var in
                       // the Z equation-system. That is, NameWeight(var,reID)
                       // represents RE_T[var,reID] in an equation in the Z equation-system
                       // of the form
                       //    Z_reID = f_reId(0) combine ... combine_T (Z_var *_T RE_T[var,reID]) combine_T ...
			  d = CIR::getTFromRegList(differentialMap[reID], var);
		  }

		  // Create and return ProjectT(_, _, d) (with appropriate first and second components)
		  int t1 = mapBack[reID].first.second;
		  int t2 = mapBack[var].first.second;
		  //std::cout << "newMerge: " << t1 << "," << t2 << std::endl;
		  std::pair<int, int> mergePair = mergeSrcMap[std::pair<int, int>(t1, t2)];
		  return RTG::ProjectT::make(CBTI::INT32(mergePair.first), CBTI::INT32(mergePair.second), d);
	  }
	  else if (exp->isUpdatable()){  // This case is a placeholder; it will never occur due to
                                        // linear nature of the Z equation-system
                assert(false && "A representation of a tensored variable occurred in a nameweight\n");
		  int node_no = exp->updatableNumber();
		  return RTG::VarT::make(CBTI::INT32(node_no));
	  }
	  else if (exp->isStar()){ // Convert a Kleene node to a KleeneT node
		  list<reg_exp_t> children = exp->getChildren(); // The list of children of a Star operation has a only a single element
		  RTG::regExpTRefPtr c = convertToRegExpT(children.front(), regExpMap, differentialMap, mapBack, mergeSrcMap);
		  return CIR::mkKleeneT(c);
	  }
	  else if (exp->isExtend()){ // Convert a Dot node to a DotT node
		  list<reg_exp_t> children = exp->getChildren();
		  list<reg_exp_t>::iterator ch = children.begin();
		  RTG::regExpTRefPtr lch = convertToRegExpT(*ch, regExpMap, differentialMap, mapBack, mergeSrcMap);
		  ch++;
		  RTG::regExpTRefPtr rch = convertToRegExpT(*ch, regExpMap, differentialMap, mapBack, mergeSrcMap);
		  return CIR::mkDotT(lch, rch);
	  }
	  else if (exp->isCombine()){ // Convert a Plus node to a PlusT node
		  list<reg_exp_t> children = exp->getChildren();
		  list<reg_exp_t>::iterator ch = children.begin();
		  RTG::regExpTRefPtr lch = convertToRegExpT(*ch, regExpMap, differentialMap, mapBack, mergeSrcMap);
		  ch++;
		  RTG::regExpTRefPtr rch = convertToRegExpT(*ch, regExpMap, differentialMap, mapBack, mergeSrcMap);
		  return CIR::mkPlusT(lch, rch);
	  }
  }	

 /*
  *  convertToRegExp
  *
  *  Convert regular expression reID to a TSL regular expression (RTG::regExpRefPtr)
  *
  *  @param: reID - the varID of the regular expressiosn
  *          exp - the expression to be converted
  *          varDependencies - a map from reID to the set of variabels it depends on that will be populated as we convert, used for the newton round later
  *			 updateableMap - a map from updateable node to the intergraph node number it depends on (that node number is still associated with the inter graph)
  *			 oMap - a map from the intergraph node number to the correct unique number for the tsl regexps
  *			 mapBack - a map for associating different variables with the appropriate variableID, needed because of the different node names
  *                             in the fwpds IGR - should try to eliminate this need if possible]
  *          elapsedTime - Total time done on possible calculations in the conversion (due to simplification in MkDot
  *
  *   Author: Emma Turetsky
  */
  RTG::regExpRefPtr convertToRegExp(int reID, reg_exp_t exp, std::map<int, reg_exp_t> & outNodeRegExpsMap, std::map<int, std::set<int> > & varDependencies, std::map<int, int> & updateableMap, std::map<int, int> & oMap, std::map<int, std::pair<std::pair<int, int>, int> > & mapBack, std::map<std::pair<int, int>, std::pair<int, int> > & mergeSrcMap, std::vector<int> & wl, std::set<int> & vl, double * elapsedTime, bool insertProjects = true)
  {
	  std::stack<cFrame> todo;
	  std::map<reg_exp_t, RTG::regExpRefPtr> seen; //map of regExps that have already been seen (should change to an unordered_map for speedup)
	  std::map<reg_exp_t, RTG::regExpRefPtr>::iterator it;
	  wali::util::GoodTimer * tC = new wali::util::GoodTimer("temp");
	  tC->stop();
	  double extraTime = tC->total_time();

	  reg_exp_t simpl_exp = exp;
	  todo.push(cFrame(simpl_exp));
	  std::set<int> vDep;
	  while (!todo.empty())
	  {
		  cFrame & frame = todo.top();
		  if (frame.is_new){ //We haven't seen the frame before
			  it = seen.find(frame.e);
			  if (it != seen.end())
			  {
				  todo.pop();
				  continue;
			  }
			  frame.is_new = false;
			  if (frame.e->isConstant())
			  {
				  if (frame.e->isOne())
					  seen[frame.e] = RTG::One::make();
				  else if (frame.e->isZero())
					  seen[frame.e] = RTG::Zero::make();
				  else {  //If the expresssion isn't a simple constant, make an external TSL wrapper around the constant and create a TSL weight
					  domain_t w = dynamic_cast<Relation*>(frame.e->get_weight().get_ptr());
					  EXTERN_TYPES::sem_elem_wrapperRefPtr wt = EXTERN_TYPES::sem_elem_wrapper(w);
					  seen[frame.e] = RTG::Weight::make(wt);
				  }
				  todo.pop();
				  continue;
			  }
			  else if (frame.e->isUpdatable())  //This means it's a call
			  {
				  //If the expression is updatable/is a variable
				  //Then look up the node it depends on - and make a variable associated with it
				  int node_no = frame.e->updatableNumber();
				  //std::cout << "node_no: " << node_no;
				  int mNum = oMap[updateableMap[node_no]];
				  //std::cout << " mNum: " << mNum << std::endl;
				  int t1 = mapBack[reID].first.second;
				  int t2 = mapBack[mNum].first.second;
				  std::pair<int, int> mergePair = mergeSrcMap[std::pair<int, int>(t1, t2)];
				  if (vl.find(mNum) == vl.end())
				  {
					  wl.push_back(mNum);
					  vl.insert(mNum);
				  }
				  vDep.insert(mNum);  //This regExp is dependent on the regExp represented by mNum
				  seen[frame.e] = insertProjects
				                      ? RTG::Project::make(CBTI::INT32(mergePair.first), CBTI::INT32(mergePair.second), RTG::Var::make(CBTI::INT32(mNum)))
				                      : RTG::Var::make(CBTI::INT32(mNum));
				  todo.pop();
				  continue;
			  }
			  //Determine if the children of this reg_exp_t have been seen before.  If so convert to the TSLRegExp, otherwise
			  //push the children reg_exp_t onto the stack
			  else if (frame.e->isStar())
			  {
				  frame.op = 0;
				  reg_exp_t child = frame.e->getChildren().front();
				  frame.left = child;
				  it = seen.find(child);
				  if (it != seen.end())
				  {
					  tC->start();
					  RTG::regExpRefPtr ret = CIR::mkKleene(it->second);
					  tC->stop();
					  seen[frame.e] = ret;
					  todo.pop();
					  continue;
				  }
				  else
				  {
					  todo.push(cFrame(child));
					  continue;
				  }
			  }
			  else
			  {
				  if (frame.e->isExtend())
					  frame.op = 1;
				  else
					  frame.op = 2;

				  list<reg_exp_t>::iterator ch = frame.e->children.begin();
				  frame.left = *ch;
				  ch++;
				  frame.right = *ch;
				  it = seen.find(frame.left);
				  if (it != seen.end())
				  {
					  RTG::regExpRefPtr lch = it->second;
					  it = seen.find(frame.right);
					  if (it != seen.end())
					  {
						  RTG::regExpRefPtr rch = it->second;
						  RTG::regExpRefPtr ret;
						  tC->start();
						  if (frame.op == 1)
							  ret = CIR::mkDot(lch, rch);
						  else
							  ret = CIR::mkPlus(lch, rch);
						  tC->stop();
						  seen[frame.e] = ret;
						  todo.pop();
						  continue;
					  }
					  else
					  {
						  todo.push(cFrame(frame.right));
						  continue;
					  }
				  }
				  else
				  {
					  todo.push(cFrame(frame.left));
					  if (seen.find(frame.right) == seen.end())
					  {
						  todo.push(cFrame(frame.right));
						  continue;
					  }
				  }
			  }
		  }
		  else  //We've seen this reg_exp_t before, so it's children have been converted
		  {
			  //Look up the children and call the appropriate constructor for TSL
			  RTG::regExpRefPtr lch = seen[frame.left];
			  RTG::regExpRefPtr ret;
			  if (frame.op == 0)  //Kleene
			  {
				  tC->start();
				  ret = CIR::mkKleene(lch);
				  tC->stop();
			  }
			  else  //Dot or Plus
			  {
				  RTG::regExpRefPtr rch = seen[frame.right];
				  if (frame.op == 1)  //Dot
				  {
					  tC->start();
					  ret = CIR::mkDot(lch, rch);
					  tC->stop();
				  }
				  else  //Plus
				  {
					  tC->start();
					  ret = CIR::mkPlus(lch, rch);
					  tC->stop();
				  }
			  }
			  seen[frame.e] = ret;
			  todo.pop();
		  }
	  }

	  varDependencies[reID] = vDep;   //Put the dependant variable set into varDependencies
	  *elapsedTime = tC->total_time() - extraTime;
	  return seen[simpl_exp];
  }
	  
 /*
  *  convertToTSLRegExps
  *
  *  Convert regular expression reg to a TSL regular expression, and
  *  put it in the map regExpMap, which maps from RegExp IDs to TSL
  *  regular expressions
  *
  *  outNodeRegExpMap - A map from reID to the wali reg_exp_t that represents it
  *  updateableMap - A map of updateable node numbers to the intergraph node they depend on
  *  oMap - a map of intergraph nodes to the ReID that represents them
  *  varDependencies - a map from reID to the set of variabels it depends on that will be populated as we convert, used for the newton round later
  *	 mapBack - a map for associating different variables with the appropriate variableID, needed because of the different node names
  *
  *  Author:  Emma Turetsky
  */
  double convertToTSLRegExps(int reg, std::map<int, reg_exp_t> & outNodeRegExpMap, tslRegExpMap & regExpMap, std::map<int, std::set<int> > & varDependencies, std::map<int, int> & updateableMap, std::map<int, int> & oMap, std::map<int, std::pair<std::pair<int, int>, int> > & mapBack, std::map<std::pair<int, int>, std::pair<int, int > >  & mergeSrcMap, std::vector<int> & wl, std::set<int> & vl
  , bool insertProjects = true)
  {
	  //std::cout << "REID: " << reg << endl;
	  //outNodeRegExpMap[reg]->print(std::cout) << std::endl;
	  double evalTime = 0;
	  RTG::regExpRefPtr rExp = convertToRegExp(reg, outNodeRegExpMap[reg], outNodeRegExpMap, varDependencies, updateableMap, oMap, mapBack, mergeSrcMap, wl, vl, &evalTime, insertProjects);
	  //rExp->print(std::cout) << std::endl;
	  //std::cout << "src: " << mapBack[reg].first.first << "tgt: " << mapBack[reg].first.second << "stack: " << mapBack[reg].second << endl << endl;
	  regExpMap[reg] = rExp;
	  return evalTime;
  }


 /*
  *  convertToTSLRegExpsT
  *
  *  For each WALi NameWeight regexp in rList, convert it to a tensored regular expression
  *
  *  The wali regexp represents a differentiated value.  The weight represents a regexp in eMap
  *  differentiated with respect to a given variable (or not differentiated, depending) or is
  *  the representation of "one"
  *
  *
  *  params:  rList -  a map from reID to the appropriate NameWeight regExp
  *           tslRegExpMap & regExpMap - map from varId to the TSL regExpRepresenting it
  *           tslDiffMap & differentialMap - map from varId to list of partial differentials representing varId
  *			  differentiated with respect to different variables
  *	       mapBack - a map for associating different variables with the appropriate variableID, needed because of the different node names
  *                     in the fwpds IGR - should try to eliminate this need if possible
  *
  *  Author:  Emma Turetsky
  */
  void convertToTSLRegExpsT(std::map<int, reg_exp_t> & rList, tslRegExpMap & regExpMap, tslRegExpTMap & tensoredRegExpMap, tslDiffMap & differentialMap, std::map<int, std::pair<std::pair<int, int>, int> > & mapBack, std::map<std::pair<int, int>, std::pair<int, int > >  & mergeSrcMap)
  {
	  for (std::map<int, reg_exp_t>::iterator it = rList.begin(); it != rList.end(); ++it)
	  {
		  if (it->first != -1){
			  // std::cout << "Converting RegExp: " << it->first << std::endl;
			  // it->second->print(std::cout);
			  // std::cout << std::endl;
			  RTG::regExpTRefPtr rExp = convertToRegExpT(it->second, regExpMap, differentialMap, mapBack, mergeSrcMap);
			  //rExp->print(std::cout) << endl;
			  tensoredRegExpMap[it->first] = rExp;
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
      int varNum = CIR::getVarNum(tList, i).get_data();
      srcList.insert(varNum);
    }
    return srcList;
  }

  /*
  * Given a list of non-differentials, return a list of variables
  * used by them
  */
  set<int> getDVarList(RTG::regExpListRefPtr dList)
  {
	  set<int> srcList;
	  int listSize = CIR::getDListLength(dList).get_data();
	  for (int i = 0; i < listSize; i++)
	  {
		  int varNum = CIR::getDVarNum(dList, i).get_data();
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
  *  fwpdsFromDifferential
  *
  *  pds - the new fwpds that is being generated
  *  differentialMap - a map of differentials
  *  varDependencies - a map from an reID (a variable) to the set of
  *                    variables it depends on
  *
  *  This procedure implements Step (3) of Algorithm NPA-TP from TR-1825 (page 33)
  *  http://research.cs.wisc.edu/wpis/papers/tr1825.pdf
  *
  *  In the terms of that paper, we are creating the dependence graph
  *  G for the left-linear equation system over the vector Z:
  *
  *    Z = (1^t tensor f(0)) combine_T D_T f|_nu(Z).       (*)
  *
  *  In terms of the implementation, the procedure is really taking
  *  one encoding of G (namely, varDependencies + differentialMap),
  *  and converting it into another encoding of G. Procedure
  *  fwpdsFromDifferential returns an encoding of G as an FWPDS, which
  *  allows us to use the FWPDS operations that implement Tarjan's
  *  path-expression method to create an appropriate set of tensored
  *  regular expressions for the nodes of G (i.e., for the variables
  *  {Z_i}, where each Z_i corresponds to some procedure i).
  *
  *  However, the encoding is slightly indirect because we have not
  *  yet actually created the tensored regular expressions of the
  *  Z-equations.  (We have various auxiliary maps from which the
  *  appropriate tensored regular expressions will be created later.)
  *  Instead, we just create an FWPDS that represents the dependences
  *  of the Z system, along with "NameWeights" that stand for the
  *  (so-far still missing) tensored regular expressions.  (The
  *  NameWeights have enough information in them so that, using the
  *  aforementioned auxiliary data structures, the correct tensored
  *  regular expressions can be constructed later; see procedure
  *  convertToRegExpT.)
  *
  *  Because the Z system is left-linear, and hence regular,
  *  dependence graph G is a normal graph, and hence we only use
  *  Delta-1 rules.  In particular, if the equation system contains
  *  Z_i = ... combine_T (Z_j *_T RE_T[j,i]) combine_T ...,
  *
  *   o variable Z_i (corresponding to some procedure i) is represented by i
  *
  *   o the dependence of i on j is encoded by the Delta-1 rule
  *     (st1, j) -> (st1, i)
  *
  *   o the weight on such a rule is NameWeight(j,i), which stands
  *     for the tensored regular expression RE_T[j,i] that contributes
  *     to the i-th equation of equation-system (*)
  *
  *  In addition, G has additional Delta-1 rules of the form
  *
  *     (st1, dummy) -> (st1, i), with weight NameWeight(dummy,i).
  *
  *  Such a rule represents the equation with the constant right-hand side
  *
  *     Z_i = (1^t tensor f_i(0))
  *
  *  that appears in the i-th component of equation-system (*).
  *  
  *  Author: Emma Turetsky
  */
void fwpdsFromDifferential(FWPDS * pds, tslDiffMap & differentialMap, std::map<int, std::set<int> > & varDependencies)
{
    // For each component of the equation system
    for(tslDiffMap::iterator it = differentialMap.begin(); it!=differentialMap.end(); ++it)
    {
        int dummy = -1;
        int tgt = it->first;
        // Get the list of variable IDs on which this component of the equation system depends
        set<int> srcList = varDependencies[it->first];

        // For each variable src in srcList, create a Delta-1 rule (st1, src) -> (st1, tgt)
        // with weight NameWeight(src,tgt)
        for(set<int>::iterator i = srcList.begin(); i != srcList.end(); i++)
        {
            int src = *i;
            pds->add_rule(st1(), stk(src),st1(),stk(tgt), new NameWeight(src,tgt));
        }
        // Add the Delta-1 rule (st1, dummy) -> (st1, tgt), with weight NameWeight(dummy,tgt)
        pds->add_rule(st1(), stk(dummy), st1(), stk(tgt), new NameWeight(dummy,tgt));
    }
}

 /*
  *  runNewton
  *
  *  This procedure implements Steps (6)-(8) of Algorithm NPA-TP from TR-1825 (page 33)
  *  http://research.cs.wisc.edu/wpis/papers/tr1825.pdf
  *
  *  Perform Newton rounds until a fixed-point is reached
  *
  *  differentialMap - a map of differentials
  *  aList - an initial assignment for the variables
  *  varDependencies - a map from a variable (an reID) to the set of variables it depends on
  *  tensoredRegExpMap - a map from a variable (an reID) to the tensored regular expression
  *                      that defines its value
  *
  *  Author:  Emma Turetsky
  *  
  *  
  *  
  *  newVal = lambda var.bottom
  *  newStarval = empty map
  *  while true {
  *     NEWROUND:
  *	oldval = newval
  *	clear (newval)
  *	oldstarval = newstarval
  *	clear (newstarval)
  *	
  *	for each variable v { 
  *		evaluete tensored re for v with the side effect of tabulating in newstarval the linear abstraction of each star body
  *	}
  *	for each star location s {
  *		if oldstarval(s) != newstarval(s)
  *			goto NEWROUND
  *	}
  *	break
  *  }
  */
#define MAX_ROUNDS 50
  void runNewton(RTG::assignmentRefPtr & newVal, tslDiffMap & differentialMap, std::map<int, std::set<int> > & varDependencies, tslRegExpTMap & tensoredRegExpMap, bool linear)
  {
	int rnd = 0;
        bool newton = true;
	// A map of dependencies
        RTG::assignmentRefPtr oldVal;
        tslDiffMap::iterator dIt;
	int pV = 0;
	tslRegExpTMap::iterator assignIt;
	
	StarMap oldStarVal;
	StarMapT oldStarValT;
	
	newStarVal.clear();
	newStarValT.clear();

	
	// Perform Newton rounds until convergence, with a maximum of MAX_ROUNDS rounds
	while (true){
NEWROUND:
		if (rnd >= MAX_ROUNDS) {
			std::cout << "Maximum number of rounds reached. ------------------------------------------" << std::endl;
			break;
		}
		std::cout << "-------------------------------------------------------------------------------" << std::endl;
		std::cout << "Round " << rnd << ":" << std::endl;
		rnd++;
			
			
		oldVal = newVal;
		newVal = CIR::initializeAssignment();
			
		oldStarVal = newStarVal;
		oldStarValT = newStarValT;
		newStarVal.clear();
		newStarValT.clear();

		// For each variable in the equation system, evaluate its regular expression
		for (assignIt = tensoredRegExpMap.begin(); assignIt != tensoredRegExpMap.end(); assignIt++)
		{
			int var = assignIt->first;
			
			// Reevaluate the regular expression associated with variable var,
			//    using oldVal for the quantity \vec{nu} of Algorithm NPA-TP, and
			//    apply detensorTranspose
			//v = Tdetensor(evalT(Mmap[p],oldVal))
			
			std::cout << "Eval: " << assignIt->first << std::endl;
			//assignIt->second.print(std::cout);
            tsl_regexp::regExpTPrettyPrint(assignIt->second, std::cout);
			std::cout << std::endl;
			
			EXTERN_TYPES::sem_elem_wrapperRefPtr newValue = evalTNonRec(assignIt->second, oldVal);
			
			std::cout << std::endl << "Tensored Value: ";
			newValue.v->print(std::cout);
			
			EXTERN_TYPES::sem_elem_wrapperRefPtr rep = EXTERNS::detensorTranspose(newValue);
			
			std::cout << std::endl << "Detensored Val: ";
			rep.v->print(std::cout);
			std::cout << std::endl;
			
			/*EXTERN_TYPES::sem_elem_wrapperRefPtr ret;
			
			ret = EXTERNS::evalDotSemElem(rep, EXTERNS::getOneWt());
			std::cout << std::endl << "After extend one: " << std::endl;
			ret.v->print(std::cout);
			std::cout << std::endl;*/

			// Insert <var,rep> into newVal
			newVal = CIR::updateAssignment(newVal, CBTI_INT32(var), rep);

		}
		
		EvalMap2.clear();
		EvalMapT.clear();

		// std::cout << "Beginning main-loop exit test:" << std::endl;
		// std::cout << "Old untensored star keys are:" << std::endl;
                // for(StarMap::const_iterator oldStar_it = oldStarVal.begin(); oldStar_it != oldStarVal.end(); ++oldStar_it)
		// {
		// 	std::cout << "  Hash Key: " << hash_value((*oldStar_it).first) << std::endl;
		// 	//std::cout << (*oldStar_it).second << std::endl; // segfault!
		// }
		// std::cout << "New untensored star keys are:" << std::endl;
                // for(StarMap::const_iterator newStar_it = newStarVal.begin(); newStar_it != newStarVal.end(); ++newStar_it)
		// {
		// 	std::cout << "  Hash Key: " << hash_value((*newStar_it).first) << std::endl;
		// 	//std::cout << (*oldStar_it).second << std::endl; // segfault!
		// }
		// std::cout << "End of untensored map keys." << std::endl;
		// Test to see whether all abstracted Kleene star bodies have converged
		for(StarMap::const_iterator newStar_it = newStarVal.begin(); newStar_it != newStarVal.end(); ++newStar_it) 
		{
			// std::cout << "  Checking an untensored Kleene star." << std::endl;
			StarMap::const_iterator oldStarValue = oldStarVal.find(newStar_it->first);
			if (oldStarValue == oldStarVal.end()) {
				// On the first round, we have no previous value to compare against
				// std::cout << "    Nothing to compare against: continuing loop." << std::endl;
				goto NEWROUND;
			}
			if (!newStar_it->second.v->Equivalent(oldStarValue->second.v)) {
				// std::cout << "    Inequivalent: continuing loop." << std::endl;
				goto NEWROUND;
			}
			// std::cout << "  Equivalent." << std::endl;
		}
		for(StarMapT::const_iterator newStar_it = newStarValT.begin(); newStar_it != newStarValT.end(); ++newStar_it) 
		{
			// std::cout << "  Checking a tensored Kleene star." << std::endl;
			StarMapT::const_iterator oldStarValue = oldStarValT.find(newStar_it->first);
			if (oldStarValue == oldStarValT.end()) {
				// On the first round, we have no previous value to compare against
				// std::cout << "    Nothing to compare against: continuing loop." << std::endl;
				goto NEWROUND;
			}
			if (!newStar_it->second.v->Equivalent(oldStarValue->second.v)) {
				// std::cout << "    Inequivalent: continuing loop." << std::endl;
				goto NEWROUND;
			}
			// std::cout << "  Equivalent." << std::endl;
		}
		// std::cout << "  All stars are equivalent: exiting loop." << std::endl;
		break; // Exit the Newton loop because all abstracted Kleene star bodies have converged
	}
		
	std::cout << std::endl << "NumRnds: " << rnd << std::endl;

	if (testMode) {
		std::fstream testFile(testFileName.c_str(), std::fstream::out | std::fstream::app);
		testFile << "__NUMRNDS " << rnd << std::endl;
		testFile.close();
	}
	
	/*else
	{
		rnd++;
		for (assignIt = tensoredRegExpMap.begin(); assignIt != tensoredRegExpMap.end(); assignIt++)
		{
			    int var = assignIt->first;
				//v = Tdetensor(evalT(Mmap[p],oldVal))
				//The variable representing the regexp is potentially dirty, so reevaluate it using the
				//new assignment in oldVal
				//std::cout << "Eval: " << assignIt->first << std::endl;
				//assignIt->second.print(std::cout);
				//std::cout << std::endl;
				EXTERN_TYPES::sem_elem_wrapperRefPtr newValue = evalTNonRec(assignIt->second, oldVal);
				//DetensorTranspose the new value
				//std::cout << std::endl << "Result: ";
				//newVal.v->print(std::cout);
				EXTERN_TYPES::sem_elem_wrapperRefPtr rep = EXTERNS::detensorTranspose(newVal);
				//std::cout << std::endl << "Detensored Val: ";
				//rep.v->print(std::cout);
				//Perform the computation needed for merge functions by computing rep = M(1,rep)
				//FIXME - what is applied here has to be a global merge function that applies at all procedure exits
				//		- we don't have the information to perform exit-specific merge functions
				//Check to see if the new value is the same as the previous one
				//if A'[p] != v`
				//Add v' to the changedVar set and updated the assignmentList
				newVal = CIR::updateAssignment(newVal, CBTI_INT32(var), rep);
		}
		EvalMap2.clear();
		EvalMapT.clear();
	std::cout << std::endl << "NumRnds: " << rnd << std::endl;
	}*/
  }

 /*
  *  run_newton
  *
  *  This procedure implements Algorithm NPA-TP from TR-1825 (page 33)
  *  http://research.cs.wisc.edu/wpis/papers/tr1825.pdf
  *
  *  Step 1 - Convert the program 'pg' into an fpds, whose rules
  *           encode the edges of the program's ICFG in the standard manner, e.g., see
  *           Fig. 3 of http://research.cs.wisc.edu/wpis/papers/fsttcs07.invited.pdf.
  *           A rule's weight represents the abstract transformer (in some abstract domain)
  *           of the corresponding ICFG edge.  The merge function on a call-rule (a.k.a.
  *           Delta-2 rule) must obey the properties given in Section 8 of TR-1825.
  *
  *  Step 2 - Apply Tarjan's path-expression algorithm to the CFG of each procedure of pg.
  *           Method: Perform poststar on the fpds to obtain the regular expressions associated with
  *           the outgoing nodes in the intra_graph associated with the fwpds.
  *           Each such node represents one procedure of pg.  Taken together, the
  *           regular expressions represent the equation system E = {Xj = Rhs_j(vec{X}) | Xj in vec{X}}.
  *
  *  Step 3 - Convert these regexps into TSL regular expressions and obtain the differentials
  *           with respect to the variables
  *
  *  Step 4 - Create a new fwpds using these differentials - run poststar on the fwpds in
  *           order to get new regular expressions representing the return points for Newton
  *
  *  Step 5 - Use the reg exps representing the differentials, the partial differentials, and the original
  *           tsl regular expressions to get the tensored regular expressions we need to run the newton rounds
  *
  *  Step 6 - Perform Newton rounds until a fixed-point is reached
  *
  *  Step 7 - Insert the new weights into the original outfa and perform the iterative path summary
  *           to determine if the error weight is reachable [Vestigial/optional]
  *
  *  Author:  Emma Turetsky
  */
  
  double run_newton(WFA& outfa, wali::Key entry_key, FWPDS * originalPds = NULL, bool canPrune = true)
  { 
    cout << "#################################################" << endl;
    //cout << "[Newton Compare] Goal VIII: end-to-end newton_merge_notensor_fwpds run" << endl;

	nonRec = false;
	pNonLin = false;
    
    cout << "Step 1: =========================================================" << endl;
	//Step 1 - Convert the program 'pg' into an fpds where the weights are nwaobdds.
	FWPDS * fpds;
    if(originalPds != NULL)
      fpds = new FWPDS(*originalPds);
    else{
      fpds = new FWPDS();
	  con = pds_from_prog_with_newton_merge(fpds, pg);// ncon = pds_from_prog_nwa(fpds, pg);
    }

    //freopen("newtonMinTest.txt", "w", stdout);
	//fpds->print(std::cout) << endl;

	int dummy = -1;
	map<int, reg_exp_t> outNodeRegExpMap; // A map from a unique id of an outgoing node to the regular expression associated with it
	map<int, reg_exp_t> rNew; // A map from the node ids to the regexps representing the full differentials associated with that node id
	map<int, int> updateableMap;  //A map from an updateable node number to the id of the node it depends on 
	map<int, int> oMap;
	map<int, pair< pair<int, int>, int> > mapBack;  //A map from the node index to the struct of the <<src,tgt>,stack> - used to insert weights back into wfa
	map<pair<pair<int, int>, int>, int> transMap;
	vector<int> differentiatedList; //A list of nodes with where the differential is to be taken
	tslRegExpMap regExpMap;  //The map of all tsl regular expression
	tslRegExpMap diffMap;  //The map of tsl regular expressions to be differentiated (the program return points)
	tslDiffMap differentialMap;  //This is a map from regexp ids to the partial differentials assoicated with them
	tslRegExpTMap tensoredRegExpMap;  //A map from the regexpId to the tsl tensored differential representing it
	map<int, domain_t> finWeights;  //The map from node id to the final relational weights
	map<std::pair<int, int>, std::pair<int, int> > mergeSrcMap; //The map that keeps track of the src of calls on call instructions
	std::vector<int> wl;
	std::set<int> vl;
	double baseEvalTime = 0;
	map<int, std::set<int> > varDependencies;
	TdiffMap = TDiffHashMap();
	
	RTG::assignmentRefPtr aList;


    wali::set_verify_fwpds(false);
    
    fpds->useNewton(false);
    //fpds->add_rule(st1(),getKey(mainProc),st1(),fpds->get_theZero()->one());


    wali::util::GoodTimer * t = new wali::util::GoodTimer("temp");
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    sem_elem_t temp = fpds->get_theZero()->one();
    fa.addTrans(getPdsState(),entry_key, acc, fpds->get_theZero()->one());
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);


	if (dump){
		fstream outfile("init_fa.dot", fstream::out);
		fa.print_dot(outfile, true);
		outfile.close();
	}
	if (dump){
		fstream outfile("init_fa.txt", fstream::out);
		fa.print(outfile);
		outfile.close();
	}

    cout << "Step 2: =========================================================" << endl;
	/* Step 2 - Perform poststar on the fpds get the regular expressions associated with
	*			the outgoing nodes in the intra_graph associated with the fwpds
	*/
	// This function performs poststar on fpds w.r.t. fa, and populates the maps as described above
	// The Boolean indicates whether this is the first time the function is called and will generate unique ids as needed
	double curTime = t->total_time();
	t->stop();
	double t1 = fpds->getOutRegExps(fa, outfa, outNodeRegExpMap, updateableMap, oMap, mapBack, transMap, differentiatedList, mergeSrcMap);
	t->start();
	int testSize = mapBack.size();
	int testSize2 = transMap.size();

	// Find the key to the error state
	Key errKey = wali::getKeySpace()->getKey("error");
	Key iKey = wali::getKeySpace()->getKey("Unique State Name");
	Key pErrKey = wali::getKeySpace()->getKey(iKey, errKey);
	GenKeySource * esrc = new wali::wpds::GenKeySource(1, pErrKey);
	Key fKey = wali::getKeySpace()->getKey(esrc);
	bool prune = false;

	// If the error state is in the wfa, then prune, otherwise the error is not reachable  -- ETTODO (prune check is redundant)
	if (outfa.getState(fKey) != NULL)
	{
		prune = true;
	}
	if (prune || (!canPrune))
	{
                if(prune)
                {
		//Set the initial state to be the error state in the wfa and then prune
		outfa.setInitialState(fKey);
		outfa.prune();
		}
		t->stop();
		std::set<Key> faStates = outfa.getStates();
		std::set<Key>::iterator stateIter;
		int numTrans = 0;

		//Get the set of transitions on the remaining states and put that into the worklist
		for (stateIter = faStates.begin(); stateIter != faStates.end(); stateIter++)
		{
			TransSet & transSet = outfa.getState(*stateIter)->getTransSet();
			TransSet::iterator transIt;
			for (transIt = transSet.begin(); transIt != transSet.end(); transIt++)
			{
				ITrans * tr = *transIt;
				int tSrc = tr->from();
				int tTgt = tr->to();
				int tStack = tr->stack();
				int transReg = transMap[std::make_pair(std::make_pair(tSrc, tTgt), tStack)];
				wl.push_back(transReg);
				vl.insert(transReg);
				numTrans++;
			}
		}
		if (dump){
			cout << "[Newton Compare] Dumping the output automaton in dot format to outfa.dot" << endl;
			fstream outfile("inter_outfa.dot", fstream::out);
			outfa.print_dot(outfile, true);
			outfile.close();
		} if (dump){
			cout << "[Newton Compare] Dumping the output automaton to final_outfa.txt" << endl;
			fstream outfile("inter_outfa.txt", fstream::out);
			outfa.print(outfile);
			outfile.close();
		}

        cout << "Step 3: =========================================================" << endl;
		/*Step 3 - Convert these regexps into TSL regular expressions and get the partial differentials
		*		   with respect their variables
		*/

		cout << "[Newton Compare] converting to TSL" << endl;
		while (!wl.empty())
		{
			int wlSzie = wl.size();
			//convertToTSLRegExp
			//if in conversion an updatable node is hit
			//add that regexp to wl if not already in visitedList
			int rToConvert = wl.back();
			//std::cout << rToConvert << endl;
			wl.pop_back();
			baseEvalTime += convertToTSLRegExps(rToConvert, outNodeRegExpMap, regExpMap, varDependencies, updateableMap, oMap, mapBack, mergeSrcMap, wl, vl, false);
		}
		//std::cout << "ESIZE: " << E.size() << std::endl;
		//std::cout << "DSIZE: " << differentiatedList.size() << std::endl;

		// Create a map from unique IDs to tsl regular expressions with variables
		// Currently doing this by iterating through the regular expressions and copying
		// the tsl regular expression whose ids match the nodes in the differentiatedMap
		for (vector<int>::iterator eit = differentiatedList.begin(); eit != differentiatedList.end(); eit++)
		{
			// std::cout << "D: " << *eit << endl;
				RTG::regExpRefPtr tmpReg = regExpMap[(*eit)];
				if (tmpReg != NULL)
				{
					diffMap[(*eit)] = regExpMap[(*eit)];
                    //tsl_regexp::regExpPrettyPrint(tmpReg, std::cout);
				}
			//  std::cout << "RE: " << *eit;
			//  std::cout << " ";
			//  E[(*eit)].print(std::cout) << std::endl;
			// This is used in debugging to compare the epsilon transitions with those generated by non-newton methods
			// to make sure they match
#if defined(NEWTON_DEBUG)
			int src = mapBack[(*eit)].first.first;
			int tgt = mapBack[(*eit)].first.second;
			int stack = mapBack[(*eit)].second;
			globalEpsilon.insert(std::make_pair(std::make_pair(src,tgt),stack));
#endif
		}
		t->start();

		cout << std::endl << "[Newton Compare] Creating Differentials" << std::endl;
		//Created the differentials
		double t2 = 0;
		if (diffMap.size() != 0)
		{
			bool linear = createDifferentials(diffMap, differentialMap);
			if (linear)
			{
				std::cout << "linear";
				std::cout << std::endl;
			}
			else
			{
				std::cout << "NonLin";
				std::cout << std::endl;
			}
			tslDiffMap::iterator it3;

            cout << "Step 4: =========================================================" << endl;
			/* Step 4 - Create a new fwpds using these partial differentials - run poststar on the fwpds in
			*	        order to get the full differentials representing the values of the program return points
			*/
			//Now we create new fwpds using these differentials, this fwpds has the weight type of NameWeight
			FWPDS * fnew = new FWPDS();
			fwpdsFromDifferential(fnew, differentialMap, varDependencies);

			//Now create another finite automaton
			WFA fa2;
			wali::Key acc2 = wali::getKeySpace()->getKey("accept2");
			fa2.addTrans(getPdsState(), stk(dummy), acc2, fnew->get_theZero()->one());
			fa2.setInitialState(getPdsState());
			fa2.addFinalState(acc2);
			WFA outfa2;

			//Get the regexps generated by running poststar on the new fwpds, these are the ones we will use in the Newton Round
			t->stop();
			t2 = fnew->getOutRegExpsSimple(fa2, outfa2, rNew);
			t->start();
			if (dump){
				cout << "[Newton Compare] Dumping the output automaton in dot format to outfa.dot" << endl;
				fstream outfile("inter_outfa2.dot", fstream::out);
				outfa2.print_dot(outfile, true);
				outfile.close();
			} if (dump){
				cout << "[Newton Compare] Dumping the output automaton to final_outfa.txt" << endl;
				fstream outfile("inter_outfa2.txt", fstream::out);
				outfa2.print(outfile);
				outfile.close();
			}

			if (dump){
				cout << "[Newton Compare] Dumping the output automaton in dot format to outfa.dot" << endl;
				fstream outfile("fa2.dot", fstream::out);
				fa2.print_dot(outfile, true);
				outfile.close();
			} if (dump){
				cout << "[Newton Compare] Dumping the output automaton to final_outfa.txt" << endl;
				fstream outfile("fa2.txt", fstream::out);
				fa2.print(outfile);
				outfile.close();
			}

            cout << "Step 5: =========================================================" << endl;
			/* Step 5 - Use the reg exps representing the differentials, the partial differentials, and the original
			*			tsl regular expressions to get the tensored regular expressions we need to run the newton rounds
			*/
			convertToTSLRegExpsT(rNew, regExpMap, tensoredRegExpMap, differentialMap, mapBack, mergeSrcMap);
			aList = CIR::initializeAssignment();
			
			for (tslRegExpMap::iterator it = regExpMap.begin(); it != regExpMap.end(); ++it)
			{
				RTG::regExpRefPtr r = it->second;
				EXTERN_TYPES::sem_elem_wrapperRefPtr newVal = evalNonRecAt0(r);

				// Insert <it->first,newVal> into aList
				aList = CIR::updateAssignment(aList, CBTI_INT32(it->first), newVal);
			}


            cout << "Step 6: =========================================================" << endl;
			/* Step 6 - Perform Newton rounds until a fixed-point is reached */
			cout << "[Newton Compare] Running Newton" << endl;
			runNewton(aList, differentialMap, varDependencies, tensoredRegExpMap,linear);

			//Using the final weights from Newton, evaluate the tslRegExps to get the final weights
			//evalRegExps(aList);


			/* Step 7 - Insert the new weights into the original outfa and perform the iterative path summary in order to
			*			determine if the error weight is reachable
			*/
		}
		else  //There are no variables to be differentiated
		{
			nonRec = true;
			std::cout << "NonRec";
			std::cout << std::endl;
			aList = CIR::initializeAssignment();
			//evalRegExps(aList);
			t2 = 0;
		}
		t->stop();

        cout << "Step 7: =========================================================" << endl;
        //Map the evaluated weights back to the transitions the regexps came from
		for (stateIter = faStates.begin(); stateIter != faStates.end(); stateIter++)
		{
			TransSet & transSet = outfa.getState(*stateIter)->getTransSet();
			TransSet::iterator transIt;
			for (transIt = transSet.begin(); transIt != transSet.end(); transIt++)
			{
				ITrans * tt = *transIt;
				int tSrc = tt->from();
				int tTgt = tt->to();
				int tStack = tt->stack();
				int transReg = transMap[std::make_pair(std::make_pair(tSrc, tTgt), tStack)];
				t->start();
				EXTERN_TYPES::sem_elem_wrapperRefPtr w = evalRegExpFinNonRec(regExpMap[transReg], aList);
				t->stop();
				//std::cout << "OutWeight: " << tSrc << "," << tTgt << "," << tStack << ":";
				//w.v->print(std::cout) << endl;
				tt->setWeight(w.v);
			}
		}
		/*cout << "[Newton Compare] Dumping the output automaton in dot format to outfa.dot" << endl;
		fstream outfile("newton_outfa.dot", fstream::out);
		outfa.print_dot(outfile, true);
		outfile.close();*/

		//Perform the final path summary
		t->start();
		outfa.path_summary_tarjan_fwpds(true, true);
		//State * initS = outfa.getState(outfa.getInitialState());
		//if (initS == NULL)
		//{
		//	cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
		//}
		//else
		//{
		//	sem_elem_t fWt = outfa.getState(outfa.getInitialState())->weight();
		//	if (fWt->equal(fWt->zero()))
		//	{
		//		cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
		//	}
		//	else{
		//		cout << "[Newton Compare] FWPDS ==> error reachable" << endl;
		//	}
		//}

		t->stop();
		double tTime = t->total_time() + t1 + t2 + baseEvalTime;
		std::cout << "[Newton Compare] Time taken by: Newton: " << std::endl;
		cout << tTime << endl;
		
		if (testMode) {
			std::fstream testFile(testFileName.c_str(), std::fstream::out | std::fstream::app);
			testFile << "__TIME " << tTime << std::endl;
			testFile.close();
		}
		
		return tTime;
	}
	else  //There is no error state
	{
		cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
		double tTime = t->total_time() + t1;;
		std::cout << "[Newton Compare] Time taken by: Newton: ";
		cout << tTime << endl;
		std::cout << "NonRec";
		std::cout << std::endl;
		
		if (testMode) {
			std::fstream testFile(testFileName.c_str(), std::fstream::out | std::fstream::app);
			testFile << "__TIME " << tTime << std::endl;
			testFile.close();
		}
		
		return tTime;
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
  *  Step 4 -
  */
  double run_newton_noTensor(WFA& outfa, FWPDS * originalPds = NULL)
  {
	  cout << "#################################################" << endl;

	  //Step 1 - Convert the program 'pg' into an fpds where the weights are nwaobdds.
	  FWPDS * fpds;
	  if (originalPds != NULL)
		  fpds = new FWPDS(*originalPds);
	  else{
		  fpds = new FWPDS();
#if USE_NWAOBDD
		  con = pds_from_prog_with_newton_merge_nwa(fpds, pg, true);
#else
		  con = pds_from_prog_with_newton_merge(fpds, pg);
#endif
	  }

	  //freopen("newtonOutTest.txt", "w", stdout);
	  //fpds->print(std::cout) << endl;


	  map<int, reg_exp_t> outNodeRegExpMap; // A map from a unique id of an outgoing node to the regular expression associated with it
	  map<int, int> updateableMap;  //A map from an upadateable node number to the id of the node it depends on 
	  map<int, int> oMap;
	  map<int, pair< pair<int, int>, int> > mapBack;  //A map from the node index to the struct of the <<src,tgt>,stack> - used to insert weights back into wfa
	  map<pair<pair<int, int>, int>, int> transMap;
	  vector<int> differentiatedList; //A list of nodes with where the differential is to be taken
	  tslRegExpMap regExpMap;  //The map of all tsl regular expression
	  tslRegExpMap diffMap;  //The map of tsl regular expressions to be differentiated (the program return points)
	  tslUntensoredDiffMap untensoredDifferentialMap;
	  map<int, domain_t> finWeights;  //The map from node id to the final relational weights
	  map<std::pair<int, int>, std::pair<int, int> > mergeSrcMap; //The map that keeps track of the src of calls on call instructions
	  std::vector<int> wl;
	  std::set<int> vl;
	  double baseEvalTime = 0;
	  map<int, std::set<int> > varDependencies;

	  std::set<pair<std::pair<int, int>, int> > globalEpsilon;  //used for debugging Newton
	  int dummy = -1;


	  wali::set_verify_fwpds(false);

	  fpds->useNewton(false);
	  //fpds->add_rule(st1(),getKey(mainProc),st1(),fpds->get_theZero()->one());


	  wali::util::GoodTimer * t = new wali::util::GoodTimer("temp");
	  WFA fa;
	  wali::Key acc = wali::getKeySpace()->getKey("accept");
	  fa.addTrans(getPdsState(), getEntryStk(pg, mainProc), acc, fpds->get_theZero()->one());
	  fa.setInitialState(getPdsState());
	  fa.addFinalState(acc);


	  if (dump){
		  fstream outfile("init_fa.dot", fstream::out);
		  fa.print_dot(outfile, true);
		  outfile.close();
	  }
	  if (dump){
		  fstream outfile("init_fa.txt", fstream::out);
		  fa.print(outfile);
		  outfile.close();
	  }

	  /* Step 2 - Perform poststar on the fpds get the regular expressions associated with
	  *			the outgoing nodes in the intra_graph associated with the fwpds
	  */
	  //This function performs postar on the fa using the fpds and populations the maps as described above
	  //The boolean means this is the first time the functions is called and will generate unique ids as needed
	  double curTime = t->total_time();
	  t->stop();
	  double t1 = fpds->getOutRegExps(fa, outfa, outNodeRegExpMap, updateableMap, oMap, mapBack, transMap, differentiatedList, mergeSrcMap);
	  t->start();

	  //Get the key to the error state
	  Key errKey = wali::getKeySpace()->getKey("error");
	  Key iKey = wali::getKeySpace()->getKey("Unique State Name");
	  Key pErrKey = wali::getKeySpace()->getKey(iKey, errKey);
	  GenKeySource * esrc = new wali::wpds::GenKeySource(1, pErrKey);
	  Key fKey = wali::getKeySpace()->getKey(esrc);
	  bool prune = false;
	  //Set the initial state to the error state, if the error state exists and prune the wfa
	  if (outfa.getState(fKey) != NULL)
	  {
		  prune = true;
	  }
	  if (prune)
	  {
		  outfa.setInitialState(fKey);
		  outfa.prune();
		  t->stop();
		  std::set<Key> faStates = outfa.getStates();
		  std::set<Key>::iterator stateIter;
		  int numTrans = 0;
		  for (stateIter = faStates.begin(); stateIter != faStates.end(); stateIter++)
		  {
			  //Get the regular expressions on each remaining transition
			  TransSet & transSet = outfa.getState(*stateIter)->getTransSet();
			  TransSet::iterator transIt;
			  for (transIt = transSet.begin(); transIt != transSet.end(); transIt++)
			  {
				  ITrans * tr = *transIt;
				  int tSrc = tr->from();
				  int tTgt = tr->to();
				  int tStack = tr->stack();
				  int transReg = transMap[std::make_pair(std::make_pair(tSrc, tTgt), tStack)];
				  wl.push_back(transReg);
				  vl.insert(transReg);
				  numTrans++;
			  }
		  }
		  if (dump){
			  cout << "[Newton Compare] Dumping the output automaton in dot format to outfa.dot" << endl;
			  fstream outfile("inter_outfa.dot", fstream::out);
			  outfa.print_dot(outfile, true);
			  outfile.close();
		  } if (dump){
			  cout << "[Newton Compare] Dumping the output automaton to final_outfa.txt" << endl;
			  fstream outfile("inter_outfa.txt", fstream::out);
			  outfa.print(outfile);
			  outfile.close();
		  }

		 /*Step 3 - Convert these regexps into TSL regular expressions and get the partial differentials
		  *		   with respect their variables
		  */
		  cout << "[Newton Compare] converting to TSL" << endl;
		  while (!wl.empty())
		  {
			  int wlSzie = wl.size();
			  //convertToTSLRegExp
			  //if in convertion an updatable node is hit
			  //add that regexp to wl if not already in visitedList
			  int rToConvert = wl.back();
			  //std::cout << rToConvert << endl;
			  wl.pop_back();
			  baseEvalTime += convertToTSLRegExps(rToConvert, outNodeRegExpMap, regExpMap, varDependencies, updateableMap, oMap, mapBack, mergeSrcMap, wl, vl);
		  }


		  int entry = 0;
		  bool first = true;
		  for (vector<int>::iterator eit = differentiatedList.begin(); eit != differentiatedList.end(); eit++)
		  {
			  //std::cout << "D: " << *eit << endl;
			  RTG::regExpRefPtr tmpReg = regExpMap[(*eit)];
			  if (tmpReg != NULL) //That updateable node may have been pruned away
			  {
				  if (first)
				  {
					  entry = (*eit);
					  first = false;
				  }
				  diffMap[(*eit)] = regExpMap[(*eit)];
			  }
			  //std::cout << "RE: " << *eit;
			  //std::cout << " ";
			  //E[(*eit)].print(std::cout) << std::endl;
			  //This is used  in debugging to compare the epsilon transitions with those generated by non-newton methods
			  //and make sure they match
#if defined(NEWTON_DEBUG)
			  int src = mapBack[(*eit)].first.first;
			  int tgt = mapBack[(*eit)].first.second;
			  int stack = mapBack[(*eit)].second;
			  globalEpsilon.insert(std::make_pair(std::make_pair(src, tgt), stack));
#endif
		  }
		  t->start();
		  RTG::assignmentRefPtr aList;
		  if (diffMap.size() != 0)
		  {
			  std::cout << "[Newton Compare] Creating Differentials" << std::endl;
			  //Created the differentials
			  createUntensoredDifferentials(diffMap,untensoredDifferentialMap);

			  //For each Differential
			  //Create the rules for the TSLDifferentialPDS
			  //We do this once

			  std::map<int, Key> exitKeyMap;
			  std::map<std::pair<std::pair<Key, Key>, Key>, merge_fn_t> mfMap;
			  tslUntensoredDiffMap::iterator rePIt;
			  //For each Differential e
			  wali::KeySpace * ks = wali::getKeySpace();
			  Key pKey = wali::getKeySpace()->getKey("Unique State Name");
			  FWPDS * tslFwpds = new FWPDS;

			  //Creat an main node this is the start of the fwpds - it has transitions to all the differential nodes
			  ostringstream MEntry;
			  MEntry << "Main_Entry_NODE";
			  Key mEntryKey = ks->getKey(MEntry.str());
			  for (rePIt = untensoredDifferentialMap.begin(); rePIt != untensoredDifferentialMap.end(); rePIt++)
			  {
				  //For the current RegExp
				  int wID = rePIt->first;
				  ostringstream keyStringEntry;
				  ostringstream keyStringExit;
				  //Get the key for the entry and exit pt for that regExp
				  keyStringEntry << wID << "_entry";
				  keyStringExit << wID << "_exit";
				  Key KeyEntry = ks->getKey(keyStringEntry.str());
				  Key KeyExit = ks->getKey(keyStringExit.str());
				  exitKeyMap[wID] = KeyExit;
				  //Add a rule from the main entry to this entry stack with weight one
				  tslFwpds->add_rule(st1(), stk(mEntryKey), st1(), stk(KeyEntry), new TSLWeight(RTG::One::make()));
				  //std::cout << "KeyEntry: " << keyStringEntry.str() << " ----> " << KeyEntry << endl;
				  //std::cout << "KeyExit: " << keyStringExit.str() << " ----> " << KeyExit << endl;

				  //Create a delta 1 fpds rule from e_entry to e_ext with reg_exp e at 0
				  EXTERN_TYPES::sem_elem_wrapperRefPtr w0 = evalNonRecAt0(regExpMap[wID]);
				  RTG::regExpRefPtr w = RTG::Weight::make(w0);
				  tslFwpds->add_rule(st1(), stk(KeyEntry), st1(), stk(KeyExit), new TSLWeight(w));
				  set<int> srcList = varDependencies[rePIt->first];

				  //Create the return rule for this pt
				  tslFwpds->add_rule(st1(), stk(KeyExit), st1(), new TSLWeight(RTG::One::make()));
				  //For each Var i in the differential
				  for (set<int>::iterator srcit = srcList.begin(); srcit != srcList.end(); srcit++)
				  {
					  int y_i = *srcit;
					  //Get the pairList associated with y_i
					  RTG::rePairListRefPtr pList = CIR::getPFromRegList(rePIt->second, y_i);
					  int numPairs = CIR::getNumPairs(pList).get_data();
					  //For each Pair k in the var
					  for (int pair_k = 0; pair_k < numPairs; pair_k++)
					  {
						  //Get the Left and Right sides of the pair
						  RTG::regExpRefPtr L_k = CIR::getPairL(pList, 0);
						  RTG::regExpRefPtr R_K = CIR::getPairR(pList, 0);
						  pList = CIR::getNextPList(pList);
						  ostringstream keyStringCallSite;
						  ostringstream keyStringRetSite;
						  //Create call and return keys
						  keyStringCallSite << wID << "_" << y_i << "_" << pair_k << "Call";
						  keyStringRetSite << wID << "_" << y_i << "_" << pair_k << "Ret";
						  Key KeyCall = ks->getKey(keyStringCallSite.str());
						  Key KeyRet = ks->getKey(keyStringRetSite.str());
						  //std::cout << "KeyCall: " << keyStringCallSite.str() << " ----> " << KeyCall << endl;
						  //std::cout << "KeyRet: " << keyStringRetSite.str() << " ----> " << KeyRet << endl;

						  //Create a delta 1 fpds rule from e_entry to e_call_i_k with weight L_i_k
						  //Create a delta 1 fpds rule from e_entry to e_return_i_k with weight L_r_k
						  tslFwpds->add_rule(st1(), stk(KeyEntry), st1(), stk(KeyCall), new TSLWeight(L_k));
						  tslFwpds->add_rule(st1(), stk(KeyRet), st1(), stk(KeyExit), new TSLWeight(R_K));
						  ostringstream keyStringCalleeEntry;
						  ostringstream keyStringCalleeExit;
						  keyStringCalleeEntry << y_i << "_entry";
						  keyStringCalleeExit << y_i << "_exit";
						  Key KeyCalleeEntry = ks->getKey(keyStringCalleeEntry.str());
						  //std::cout << "KeyCalleeEntry: " << keyStringCalleeEntry.str() << " ----> " << KeyCalleeEntry << endl;
						  //std::cout << "KeyCaleeExit: " << keyStringCalleeExit.str() << " ----> " << KeyCalleeExit << endl;
						  //Create a delta 2 fpds rule from e_call_i_k to e_entry_i_k e_return_i_k with a merge function on it
						  //std::cout << "Looking Up: " << reID << "," << var << std::endl;
						  int t1 = mapBack[wID].first.second;
						  int t2 = mapBack[y_i].first.second;
						  std::pair<int, int> mergePair = mergeSrcMap[std::pair<int, int>(t1, t2)];
						  merge_fn_t merge = con->getLocalVars(mergePair).first;
						  mfMap[make_pair(make_pair(KeyCall, KeyCalleeEntry), KeyRet)] = merge;
						  tslFwpds->add_rule(st1(), stk(KeyCall), st1(), stk(KeyCalleeEntry), stk(KeyRet), new TSLWeight(RTG::One::make()), merge);
					  }
				  }
			  }


			  /*std::map<std::pair<std::pair<Key, Key>, Key>, merge_fn_t>::iterator mfmIt;
			  for (mfmIt = mfMap.begin(); mfmIt != mfMap.end(); mfmIt++)
			  {
			  std::cout << "Key: (" << mfmIt->first.first.first << "," << mfmIt->first.first.second << "," << mfmIt->first.second << ")" << std::endl;
			  mfmIt->second->print(std::cout);
			  std::cout << std::endl;
			  }*/

			 std::cout << "Creating Domain FWPDS" << std::endl;
			  //tslFwpds->print(std::cout);

			  //Create an assignmentlist of 0
			  aList = CIR::initializeAssignment();

			  //THIS IS THE FIRST NEWTON ROUND
			  FWPDS * domainFWPDS = new FWPDS();
			  //For each pds rule - create a domain_t rule (evaluate the rule with respect to the assignment list and create a new pds
			  WpdsRules rules;
			  tslFwpds->for_each(rules);
			  std::set< Rule >::iterator ruleIt;
			  for (ruleIt = rules.popRules.begin(); ruleIt != rules.popRules.end(); ruleIt++)
			  {
				  Key fromSt = ruleIt->from_state();
				  Key toSt = ruleIt->to_state();
				  Key fromStk = ruleIt->from_stack();
				  TSLWeight * ruleWeight = static_cast<wali::domains::tsl_weight::TSLWeight*>(ruleIt->weight().get_ptr());
				  EXTERN_TYPES::sem_elem_wrapperRefPtr binWeight = evalRegExpNonRec(ruleWeight->getWeight(), aList);
				  domainFWPDS->add_rule(fromSt, fromStk, toSt, binWeight.v);
			  }
			  for (ruleIt = rules.stepRules.begin(); ruleIt != rules.stepRules.end(); ruleIt++)
			  {
				  Key fromSt = ruleIt->from_state();
				  Key toSt = ruleIt->to_state();
				  Key fromStk = ruleIt->from_stack();
				  Key toStk = ruleIt->to_stack1();
				  TSLWeight * ruleWeight = static_cast<wali::domains::tsl_weight::TSLWeight*>(ruleIt->weight().get_ptr());
				  EXTERN_TYPES::sem_elem_wrapperRefPtr binWeight = evalRegExpNonRec(ruleWeight->getWeight(), aList);
				  domainFWPDS->add_rule(fromSt, fromStk, toSt, toStk, binWeight.v);
			  }
			  for (ruleIt = rules.pushRules.begin(); ruleIt != rules.pushRules.end(); ruleIt++)
			  {
				  Key fromSt = ruleIt->from_state();
				  Key toSt = ruleIt->to_state();
				  Key fromStk = ruleIt->from_stack();
				  Key toStk = ruleIt->to_stack1();
				  Key toStk2 = ruleIt->to_stack2();
				  wali::IntSource * fStkSrc = static_cast<wali::IntSource*>(ks->getKeySource(fromStk).get_ptr());
				  wali::IntSource * tStkSrc = static_cast<wali::IntSource*>(ks->getKeySource(toStk).get_ptr());
				  wali::IntSource * tStkSrc2 = static_cast<wali::IntSource*>(ks->getKeySource(toStk2).get_ptr());
				  //std::cout << "Key: (" << fStkSrc->getInt() << "," << tStkSrc->getInt() << "," << tStkSrc2->getInt() << ")" << std::endl;
				  merge_fn_t mf = mfMap[make_pair(make_pair(fStkSrc->getInt(), tStkSrc->getInt()), tStkSrc2->getInt())];
				  TSLWeight * ruleWeight = static_cast<wali::domains::tsl_weight::TSLWeight*>(ruleIt->weight().get_ptr());
				  EXTERN_TYPES::sem_elem_wrapperRefPtr binWeight = evalRegExpNonRec(ruleWeight->getWeight(), aList);
				  domainFWPDS->add_rule(fromSt, fromStk, toSt, toStk, toStk2, binWeight.v, mf);
			  }

			  //std::cout << std::endl;
			  //domainFWPDS->print(std::cout);

			  //Create an automata and perform poststar on the domain_t pds
			  ostringstream keyStringEntry;
			  keyStringEntry << entry << "_entry";
			  Key WFAEntry = ks->getKey(keyStringEntry.str());
			  //std::cout << "WFA StackStart: " << WFAEntry << std::endl;

			  WFA fa2;
			  wali::Key acc2 = wali::getKeySpace()->getKey("accept2");
			  fa2.addTrans(getPdsState(), stk(mEntryKey), acc2, domainFWPDS->get_theZero()->one());
			  fa2.setInitialState(getPdsState());
			  fa2.addFinalState(acc2);
			  WFA outfa2;
			  //fa2.print(std::cout);
			  domainFWPDS->poststarIGR(fa2, outfa2);
			  //outfa2.print(std::cout);

			  // Call pathsummarytarjan on the automata and get the result(sum of weights on trans of exit stack symbol * w on state it points to
			  //on the new path summary automata
			  WFA ansFA;

			  outfa2.path_summary_tarjan_fwpds(true, ansFA, false);

			  //ansFA.print(std::cout) << std::endl;
			  // outfa2.print(std::cout) << std::endl;

			  std::set<int> changedVars; //The set of possible dirty regexps IDs
			  for (std::map<int, Key>::iterator exitIt = exitKeyMap.begin(); exitIt != exitKeyMap.end(); exitIt++)
			  {
				  std::vector<ITrans const *>  trans;
				  TransStackGetter stackGetter(exitIt->second, trans);
				  outfa2.for_each(stackGetter);
				  sem_elem_t wt = con->getBaseZero();
				  for (std::vector<ITrans const *>::iterator transSIter = trans.begin(); transSIter != trans.end(); transSIter++)
				  {
					  //std::cout << "Key: " << exitIt->second << std::endl;
					  //(*transSIter)->print(std::cout) << std::endl;
					  wt = wt->combine((*transSIter)->weight());
				  }
				  //Compare with the new assignment
				  EXTERN_TYPES::sem_elem_wrapperRefPtr oldVal = CIR::getAssignment(CBTI_INT32(exitIt->first), aList);
				  //oldVal.v->print(std::cout) << std::endl;
				  //wt->print(std::cout) << std::endl;
				  if (wt != oldVal.v)
				  {
					  int rId = exitIt->first;
					  changedVars.insert(rId);
					  //std::cout << rId << std::endl;
					  domain_t w = dynamic_cast<Relation*>(wt.get_ptr());
					  EXTERN_TYPES::sem_elem_wrapperRefPtr rep = EXTERN_TYPES::sem_elem_wrapper(w);
					  aList = CIR::updateAssignment(aList, CBTI_INT32(rId), rep);
				  }
			  }

			  int rnd = 1;
			  // A map of dependencies
			  std::map<int, std::set<int> > dependenceMap;
			  std::set<int> C;
			  RTG::assignmentRefPtr aPrime = aList;
			  std::cout << "Running Newton";
			  //FOR EACH NEWTON ROUND
			  // For each set of differentials, get the list of variables they depend on
			  // Put those into dependeceMap
			  // dependenceMap - Map from regular expression ID to the set variables that regexps differentials depend on
			  while (changedVars.size() != 0)
			  {
				  EvalMap2.clear();
				  rnd++;
				  std::set<int>::iterator pIt;
				  //For each of the variables whose dependencies changed, get their new assignement from aList
				  for (pIt = changedVars.begin(); pIt != changedVars.end(); pIt++)
				  {
					  aPrime = CIR::updateAssignment(aPrime, CBTI_INT32(*pIt), CIR::getAssignment(CBTI_INT32(*pIt), aList));
				  }
				  C = changedVars;
				  changedVars.clear();

				  //Now rebuild the domain fwpds
				  for (ruleIt = rules.stepRules.begin(); ruleIt != rules.stepRules.end(); ruleIt++)
				  {
					  Key fromSt = ruleIt->from_state();
					  Key toSt = ruleIt->to_state();
					  Key fromStk = ruleIt->from_stack();
					  Key toStk = ruleIt->to_stack1();
					  wali::IntSource * fStkSrc = static_cast<wali::IntSource*>(ks->getKeySource(fromStk).get_ptr());
					  wali::IntSource * tStkSrc = static_cast<wali::IntSource*>(ks->getKeySource(toStk).get_ptr());
					  //std::cout << "Key: (" << fStkSrc->getInt() << "," << tStkSrc->getInt() << ")" << std::endl;
					  TSLWeight * ruleWeight = static_cast<wali::domains::tsl_weight::TSLWeight*>(ruleIt->weight().get_ptr());
					  //ruleWeight->getWeight().print(std::cout) << endl;
					  EXTERN_TYPES::sem_elem_wrapperRefPtr binWeight = evalRegExpNonRec(ruleWeight->getWeight(), aPrime);
					  //binWeight.v->print(std::cout) << endl;
					  domainFWPDS->replace_rule(fromSt, fromStk, toSt, toStk, binWeight.v);
				  }

				  //domainFWPDS->print(std::cout);
				  WFA fa3;
				  wali::Key acc2 = wali::getKeySpace()->getKey("accept2");
				  fa3.addTrans(getPdsState(), stk(mEntryKey), acc2, domainFWPDS->get_theZero()->one());
				  fa3.setInitialState(getPdsState());
				  fa3.addFinalState(acc2);
				  WFA outfa3;
				  //fa3.print(std::cout);
				  domainFWPDS->poststarIGR(fa3, outfa3);
				  //outfa3.print(std::cout);

				  // Call pathsummarytarjan on the automata and get the result(sum of weights on trans of exit stack symbol * w on state it points to
				  //on the new path summary automata
				  WFA ansFA;

				  outfa3.path_summary_tarjan_fwpds(true, ansFA, false);

				  //outfa3.print(std::cout) << std::endl;

				  for (std::map<int, Key>::iterator exitIt = exitKeyMap.begin(); exitIt != exitKeyMap.end(); exitIt++)
				  {
					  std::vector<ITrans const *>  trans;
					  TransStackGetter stackGetter(exitIt->second, trans);
					  outfa3.for_each(stackGetter);
					  sem_elem_t wt = con->getBaseZero();
					  for (std::vector<ITrans const *>::iterator transSIter = trans.begin(); transSIter != trans.end(); transSIter++)
					  {
						  //std::cout << "Key: " << exitIt->second << std::endl;
						  //(*transSIter)->print(std::cout) << std::endl;
						  wt = wt->combine((*transSIter)->weight());
					  }
					  //Compare with the new assignment
					  EXTERN_TYPES::sem_elem_wrapperRefPtr oldVal = CIR::getAssignment(CBTI_INT32(exitIt->first), aPrime);
					  //std::cout << "NewWeight: ";
					  //wt->print(std::cout) << std::endl;
					  //std::cout << "OldWeight: ";
					  //oldVal.v->print(std::cout) << std::endl;
					  domain_t w = dynamic_cast<Relation*>(wt.get_ptr());
					  domain_t oldV = oldVal.v;
					  if (!(w.get_ptr()->equal(oldV.get_ptr())))
					  {
						  int rId = exitIt->first;
						  changedVars.insert(rId);
						  //std::cout << rId << std::endl;
						  EXTERN_TYPES::sem_elem_wrapperRefPtr rep = EXTERN_TYPES::sem_elem_wrapper(w);
						  aList = CIR::updateAssignment(aList, CBTI_INT32(rId), rep);
					  }
				  }
			  }
			  std::cout << std::endl << "NumRnds: " << rnd << std::endl;
		  }
		  else
		  {
			  aList = CIR::initializeAssignment();
			  std::cout << std::endl << "NumRnds: " << 0 << std::endl;
		  }
		  //evalRegExps(aList);
		  t->stop();
		  for (stateIter = faStates.begin(); stateIter != faStates.end(); stateIter++)
		  {
			  TransSet & transSet = outfa.getState(*stateIter)->getTransSet();
			  TransSet::iterator transIt;
			  for (transIt = transSet.begin(); transIt != transSet.end(); transIt++)
			  {
				  ITrans * tt = *transIt;
				  int tSrc = tt->from();
				  int tTgt = tt->to();
				  int tStack = tt->stack();
				  int transReg = transMap[std::make_pair(std::make_pair(tSrc, tTgt), tStack)];
				  t->start();
				  EXTERN_TYPES::sem_elem_wrapperRefPtr w = evalRegExpFinNonRec(regExpMap[transReg], aList);
				  t->stop();
				  //std::cout << "OutWeight: " << tSrc << "," << tTgt << "," << tStack << ":";
				  //w.v->print(std::cout) << endl;
				  tt->setWeight(w.v);
			  }
		  }
		  /*for(map<int,pair<pair<int,int>,int> >::iterator mbit = mapBack.begin(); mbit != mapBack.end(); mbit++)
		  {
		  int src = mbit->second.first.first;
		  int tgt = mbit->second.first.second;
		  int stck = mbit->second.second;
		  //  cout << endl << endl;
		  //cout << "Src: " << key2str(src) << " Target: " << key2str(tgt) << " Stack: " << key2str(stck) << "\n";
		  wali::wfa::ITrans * tt = outfa.find2(src,stck,tgt);
		  sem_elem_t w = finWeights[mbit->first];
		  if (w != NULL && tt != NULL)
		  {
		  //  cout << endl;
		  // w->print(std::cout);
		  tt->setWeight(w);
		  }
		  }*/
		  t->start();
		  //sem_elem_t fWt = computePathSummary(fpds, outfa);
		  outfa.path_summary_tarjan_fwpds(true, false);

          State * initS = outfa.getState(outfa.getInitialState());
		  if (initS == NULL)
		  {
		      cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
		  }
		  else
		  {
		      sem_elem_t fWt = outfa.getState(outfa.getInitialState())->weight();
		      if (fWt->equal(fWt->zero()))
		      {
		    	  cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
		      }
		      else{
		    	  cout << "[Newton Compare] FWPDS ==> error reachable" << endl;
		      }
		  }

		  //cout << "[Newton Compare] Dumping the output automaton in dot format to outfa.dot" << endl;
		  //fstream outfile("nnt_outfa.dot", fstream::out);
		  //outfa.print_dot(outfile, true);
		  //outfile.close();
		  t->stop();
		  double tTime = t->total_time() + t1 + baseEvalTime;
		  std::cout << "[Newton Compare] Time taken by: Newton: ";
		  cout << tTime << endl;
		  return tTime;
	  }
	  else
	  {
		  cout << "[Newton Compare] FWPDS ==> error not reachable" << endl;
		  double tTime = t->total_time() + t1;
		  std::cout << "[Newton Compare] Time taken by: Newton: ";
		  cout << tTime << endl;
		  return tTime;
	  }
	  //While the set of changed variables != empty, continue iterating through NewtonRounds
	  //This is the actual workhorse of the Newton Rounds

	  //FOR EACH Zi that depends on the changed variables
	  //RE-EVALUATE THE tslFPDS RULE and create a new weight of the domain_t pds rule*/
  }

#ifdef USE_DUET
std::vector<caml_rule> ruleHolder;
std::vector<caml_call_rule> callRuleHolder;
std::vector<caml_epsilon_rule> epsilonRuleHolder;
std::vector<caml_error_rule> errorRuleHolder;
wali::Key entry_key;
wali::Key exit_key;
duetrel_t compareWeight;

CAMLprim value compare_weights(Trans t) {
    CAMLparam0();
    CAMLlocal4(bval, nval, nCVal, sval);
    value * eq_func = caml_named_value("eq_callback");
    value * p_func = caml_named_value("print_callback");
    value * n_func = caml_named_value("normalize_callback");
    
    // not normalized (WALI value)
    nval = ((DuetRel*)(t.weight().get_ptr()))->getValue();	
    sval = caml_callback(*p_func, nval);
    std::cout << std::endl << "WALI Weight (Not Normalized): " << std::endl;
    std::cout << String_val(sval) << std::endl;
    
    //not normalized (Duet value)
    nCVal = compareWeight->getValue();	
    sval = caml_callback(*p_func, nCVal);
    std::cout << std::endl << "Duet Weight (Not Normalized): " << std::endl;
    std::cout << String_val(sval) << std::endl; 
    
    // normalize WALI value
    nval = caml_callback(*n_func, nval);
    sval = caml_callback(*p_func, nval);
    std::cout << std::endl << "WALI Weight (Normalized): " << std::endl;
    std::cout << String_val(sval) << std::endl;
    
    // normalize Duet value
    nCVal = caml_callback(*n_func, nCVal);   
    sval = caml_callback(*p_func, nCVal);
    std::cout << std::endl << "Duet Weight: " << std::endl;
    std::cout << String_val(sval) << std::endl;

    //bval = caml_callback2(*eq_func, nval, nCVal);


    //if(Bool_val(bval))
    //{
	//std::cout << "Correct!" << std::endl;
    //}
    //else 
    //{
	//std::cout << "Failure!" << std::endl;
    //}
    CAMLreturn(Val_unit);
}

#endif

void runFwpds(FWPDS * pds)
{
  #if ET_DBG == 1
    pds->print(std::cout);
#endif
    //reachPds->print(std::cout);
    WFA outfa;
    WFA outfa2;
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    sem_elem_t x = pds->get_theZero();
    fa.addTrans(st1(),entry_key, acc, pds->get_theZero()->one());
    fa.setInitialState(st1());
    fa.addFinalState(acc);

#if ET_DBG == 1
	map<int, reg_exp_t> outNodeRegExpMap; // A map from a unique id of an outgoing node to the regular expression associated with it
	map<int, int> updateableMap;  //A map from an upadateable node number to the id of the node it depends on 
	map<int, int> oMap;
	map<int, pair< pair<int, int>, int> > mapBack;  //A map from the node index to the struct of the <<src,tgt>,stack> - used to insert weights back into wfa
	map<pair<pair<int, int>, int>, int> transMap;
	vector<int> differentiatedList; //A list of nodes with where the differential is to be taken
	map<std::pair<int, int>, std::pair<int, int> > mergeSrcMap; //The map that keeps track of the src of calls on call instructions
#endif
	
    //Calls the poststar functions
    cout << "[Newton Compare] Computing poststar..." << endl;
#if ET_DBG == 1
      pds->getOutRegExps(fa, outfa, outNodeRegExpMap, updateableMap, oMap, mapBack, transMap, differentiatedList, mergeSrcMap);
#endif
      pds->poststarIGR(fa,outfa);
    //static value * start_anal = NULL;
    //start_anal = caml_named_value("init_analysis");
    //caml_callback(*start_anal, Val_unit);

#if ET_DBG == 1
    cout << "Print Regular Expressions: " << endl;
    map<int, reg_exp_t>::iterator regExpIt = outNodeRegExpMap.begin();
    for (regExpIt; regExpIt != outNodeRegExpMap.end(); regExpIt++)
    {
 	regExpIt->second->print(std::cout);
	std::cout << std::endl << std::endl;
    }

#endif

    cout << "Finishing poststar: " << endl;
    outfa.path_summary_iterative_original(outfa.getSomeWeight()->one());

    fstream outfile("outfa.txt", fstream::out);
		  outfa.print(outfile);
		  outfile.close();

    Trans t;
    outfa.find(st1(), exit_key, acc, t);

    compare_weights(t);


    #undef flush
    std::cout << "Finished Printing" << std::endl << std::flush;
}
// Jason is commenting out what he believes to be a dangling
//   preprocessor directive here:
//#endif

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
      run_newton(outfa, getEntryStk(pg, mainProc));
      break;
    case 7:
	run_newton_noTensor(outfa);
	break;
    default:
      assert(0 && "I don't understand that goal!!!");
  }
  return NULL;
}


#ifdef USE_DUET

int runBasicNewtonFromBelow(char **args)
{
    caml_startup(args);
    FWPDS * pds = new FWPDS();

    for (std::vector<caml_rule>::iterator it = ruleHolder.begin(); it != ruleHolder.end(); it++)
    {
	pds->add_rule(st1(), it->first.first, st1(), it->first.second, it->second);
    }

    for (std::vector<caml_call_rule>::iterator itc = callRuleHolder.begin(); itc != callRuleHolder.end(); itc++)
    {
	pds->add_rule(st1(), itc->first.first, st1(), itc->first.second.first, itc->first.second.second, itc->second);
    }

    for (std::vector<caml_epsilon_rule>::iterator ite = epsilonRuleHolder.begin(); ite != epsilonRuleHolder.end(); ite++)
    {
	pds->add_rule(st1(), ite->first, st1(), ite->second);
    }
    
    pds->print(std::cout);

    WFA outfaNewton;
    goals::run_newton(outfaNewton, entry_key, pds, false);

    wali::Key acc = wali::getKeySpace()->getKey("accept");

    Trans t;
    outfaNewton.find(st1(), exit_key, acc, t);

    compare_weights(t);

    // Set exit_transitions to the set of all transitions in outfaNewton
    // of the form (st1,WALI_EPSILON,<st1,e>), where e is an entry node
    wali::wfa::TransSet exit_transitions;
    exit_transitions = outfaNewton.match(st1(), WALI_EPSILON);
    for(wali::wfa::TransSet::iterator tsit = exit_transitions.begin(); tsit != exit_transitions.end(); tsit++)
    {
        std::cout << "Procedure summary for (some as yet unidentified -- FIXME) procedure" << std::endl;
        duetrel_t nval = ((DuetRel*)((*tsit)->weight().get_ptr()));
        //DuetRel *nval = ((DuetRel*)((*tsit)->weight().get_ptr()));
        //DuetRel *nval = ((DuetRel*)((*tsit)->weight().get_ptr()))->getValue();
        nval->print(std::cout);
        std::cout << std::endl << std::endl;
    }
    
    std::cout << "================================================" << std::endl;
    std::cout << "Assertion Checkings at Error Points" << std::endl << std::endl;
    
    // Check the assertion at each error point
    for (std::vector<caml_error_rule>::iterator it = errorRuleHolder.begin(); it != errorRuleHolder.end(); it++)
    {
        wali::wfa::TransSet error_transitions;
        error_transitions = outfaNewton.match(st1(), it->first);
        for(wali::wfa::TransSet::iterator tsit = error_transitions.begin(); tsit != error_transitions.end(); tsit++)
        {
            std::cout << "Checking assertion at vertex " << it->first <<  std::endl << std::endl;

			// Check if is_sat ( (it->second) extend (*tsit)->weight() )

            duetrel_t negatedAssertionWeight = ((DuetRel*)(it->second.get_ptr()));   // Negated assertion condition
            negatedAssertionWeight->print(std::cout);
            std::cout << std::endl << std::endl;
            
            duetrel_t intraprocWeight = ((DuetRel*)((*tsit)->weight().get_ptr()));  // Weight from containing procedure's entry to assertion pt
            duetrel_t contextWeight = ((DuetRel*)(outfaNewton.getState((*tsit)->to())->weight().get_ptr()));  // Weight of calling context

            duetrel_t composedWeight = intraprocWeight->Compose(negatedAssertionWeight).get_ptr();    // FIXME: Compose badly named: Compose should be Extend
            duetrel_t finalWeight = contextWeight->Compose(composedWeight).get_ptr();    // FIXME: Compose badly named: Compose should be Extend

            bool isSat = finalWeight->IsSat();

			if (isSat) {
				std::cout << "Is SAT! (Assertion Failed)" << std::endl ;	
			
				// This print statement causes seg fault (not sure why) -Ashkan		
				//extval->print(std::cout);
				//std::cout << std::endl;
			}
			else {
				std::cout << "Is not SAT! (Assertion Passed)" << std::endl;
			}
	
			if (testMode) {
				std::fstream testFile(testFileName.c_str(), std::fstream::out | std::fstream::app);
				testFile << "__ASSERTION " << (isSat ? "FAIL" : "PASS") << std::endl;
				testFile.close();
			}

			std::cout << "---------------------------------------------" << std::endl << std::endl;
        }
    }

    
  /*  if(dump){
        FWPDS * originalPds = new FWPDS();
	con = pds_from_prog(originalPds, pg);
    cout << "[Newton Compare] Dumping PDS to pds.dot..." << endl;
    fstream pds_stream("pds.dot", fstream::out);
    RuleDotty rd(pds_stream);
    pds_stream << "digraph{" << endl;
    originalPds->for_each(rd);
    pds_stream << "}" << endl;
    delete(originalPds);
    }*/


    #undef flush
    std::cout << "Finished Printing" << std::endl << std::flush;

    return 0;
}	

void printUsageInstr() {
	std::cout << "Newton Duet" << std::endl;
	std::cout << std::endl << "Correct usage:\tNewtonOcaml running_mode [options] input_file [duet options]" << std::endl;
	std::cout << std::endl << "Running modes:" << std::endl;
	std::cout << "\t-cra_newton_basic\t[currently, this mode is the only one working]" << std::endl;
	std::cout << "\t-cra_newton_star" << std::endl;
	std::cout << "\t-cra_newton_above" << std::endl;
	std::cout << std::endl << "Available options:" << std::endl;
	std::cout << "\t-D,\t--dump" << std::endl;
	std::cout << "\t-H,\t--help" << std::endl;
	std::cout << "\t-P,\t--no_simplify_on_print" << std::endl;
	std::cout << "\t-S,\t--simplify" << std::endl;
}

void printHelp() {
	printUsageInstr();	
	std::cout << "\n\nDuet Options:\n\n";
	char **ocamlArgs = new char *[3];
	ocamlArgs[0] = "";
	ocamlArgs[1] = "-help";
	ocamlArgs[2] = 0;
	caml_startup(ocamlArgs);
}

int main(int argc, char **argv)
{
    int runningMode = 0;
	std::vector <char *> unrecognizedArgs;
    static struct option long_options[] = {
        {"cra_newton_basic", no_argument,       &runningMode,  1  },
        {"cra_newton_star",  no_argument,       &runningMode,  2  },
        {"cra_newton_above", no_argument,       &runningMode,  3  },
        {"simplify",         no_argument,       0,            'S' },
        {"no_simplify_on_print",no_argument,    0,            'P' },
        {"help",	         no_argument,       0,            'H' },
        {"dump",             no_argument,       0,            'D' },
        {"test",             required_argument, 0,            'T' },
//      {"verbosity",        required_argument, 0,            'V' },
        {0,                  0,                 0,             0  }
    };

    int long_index = 0, opt = 0;	
    while ((opt = getopt_long_only(argc, argv, "SPDT:H", 
                   long_options, &long_index )) != -1) {
    	switch (opt) {
			case 0:
				break;
    		case 'P':
    			DuetRel::simplifyOnPrint = false;
    			break;
    		case 'S':
    			DuetRel::simplify = true;
    			break;
    		case 'D':
    			dump = true;
    			break;
			case 'H':
				printHelp();
				return 0;
			case 'T':
				testMode = true;
				testFileName = optarg;
				break;
			// unrecognized option, currently we just pass it to duet
			case '?':		    			
				std::cout << "Passing command-line option " << 	argv[optind - 1] << " to duet." << std::endl;
				unrecognizedArgs.push_back(argv[optind - 1]);
				break;	
		}	
    }
    if (runningMode == 0) {
        std::cerr << "Error : Running mode not set!" << std::endl;
		printUsageInstr();
    }
    else if (optind == argc) {
		std::cout << "Error : No input file detected!" << std::endl;
		printUsageInstr();
    }
	
    else {
    	if (runningMode == 1) {
			if (testMode) {
				std::fstream testFile(testFileName.c_str(), std::fstream::out | std::fstream::app);
				testFile << "__FILENAME " << argv[optind] << std::endl;
				testFile.close();
			}
			char **ocamlArgs = new char *[3 + unrecognizedArgs.size() + argc - optind];
			ocamlArgs[0] = argv[0];
			ocamlArgs[1] = "-cra_newton_basic";
			ocamlArgs[2] = argv[optind];
			for (int i = 0; i < unrecognizedArgs.size(); i++) {
				ocamlArgs[3 + i] = unrecognizedArgs[i];
			}
			for (int i = 0; i < argc - optind; i++) {
				ocamlArgs[3 + unrecognizedArgs.size() + i] = argv[optind + i + 1];
			}
			runBasicNewtonFromBelow(ocamlArgs);
    	}
    	else if (runningMode == 2) {
    	    std::cout << "Newton from below, with equivalence checks extracted from Kleene stars" << std::endl;
    	    std::cout << "Not implemented yet." << std::endl;
    	}
    	else if (runningMode == 3) {
    	    std::cout << "Newton from above." << std::endl;
    	    std::cout << "Not implemented yet." << std::endl;
    	}
    }   

    //cout << "Printing stats" << std::endl;
    //value * stats = caml_named_value("print_stats_callback");
    //caml_callback(*stats, 0);
}



#else

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
    FWPDS * originalPds = new FWPDS();
	con = pds_from_prog(originalPds, pg);
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

#endif
