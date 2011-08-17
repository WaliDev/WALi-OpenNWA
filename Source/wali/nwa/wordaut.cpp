
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/returns.hpp"
#include "wali/nwa/query/transitions.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/Key.hpp"
#include "wali/wfa/epr/FunctionalWeight.hpp"
#include "wali/Reach.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/nwa/ClientInfo.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessWrapper.hpp"
#include "wali/witness/VisitorDot.hpp"
#include "wali/witness/VisitorPrinter.hpp"
#include "wali/witness/WitnessRule.hpp"

using namespace std;
using namespace wali;
using namespace wali::nwa;

// This is exactly like NWAtoPDScalls, except when the return WPDS is
// constructed, a new WitnessWrapper is passed to it.
// This is needed for our PathVisitor class, which collects transition symbols.
//
// NOTE: This should be placed in NWA.hpp
//
wpds::WPDS
NWA::_private_NWAtoPDScallsWitness( WeightGen & wg ) const
{
  //TODO: beware the implicit transitions
  //Q: do we need to make all transitions explicit in order to make this correct?
  
  wali::witness::WitnessWrapper* wrapper = new wali::witness::WitnessWrapper();
  wpds::WPDS result = wpds::WPDS(wrapper);
  
  Key program = getProgramControlLocation();  // = wali::getKey("program");
  
  wali::sem_elem_t wgt;
  
  //Internal Transitions
  for( InternalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
    {  
      // (q,sigma,q') in delta_i goes to <p,q> -w-> <p,q'> in delta_1
      // where the weight w depends on sigma
      
      State src = Trans::getSource(*iit);
      State tgt = Trans::getTarget(*iit);
      
      if( Trans::getInternalSym(*iit) == WALI_WILD )
	wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
      else
	wgt = wg.getWeight(src, getClientInfo(src),
			   Trans::getInternalSym(*iit),
			   WeightGen::INTRA,
			   tgt, getClientInfo(tgt));           // w
      
      result.add_rule(program,                                //from_state (p)
		      src,                                    //from_stack (q)
		      program,                                //to_state (p)
		      tgt,                                    //to_stack1 (q')
		      wgt);                                   //weight (w)      
    }
  
  //Call Transitions
  for( CallIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
    {           
      // (q_c,sigma,q_e) in delta_c goes to
      // <p,q_c> -w-> <p,q_e q_c> in delta_2 
      // and the weight w depends on sigma
      
      State src = Trans::getCallSite(*cit);
      State tgt = Trans::getEntry(*cit);
      
      if( Trans::getCallSym(*cit) == WALI_WILD )
	wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); // w
      else
	wgt = wg.getWeight(src, getClientInfo(src),
			   Trans::getCallSym(*cit),
			   WeightGen::CALL_TO_ENTRY,
			   tgt, getClientInfo(tgt));          // w
      
      result.add_rule(program,                                //from_state (p)
		      src,                                    //from_stack (q_c)
		      program,                                //to_state (p)
		      Trans::getEntry(*cit),                  //to_stack1 (q_e)
		      src,                                    //to_stack2 (q_c)
		      wgt);                                   //weight (w)  
    } 
  
  //Return Transitions
  int r_count = 0;
  for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
    {
      ++r_count;
      
      //std::cerr << "Return transition #" << r_count << " of " << trans.size() << "\n";
      // (q_x,q_c,sigma,q_r) in delta_r goes to 
      // <p,q_x> -w-> <p_q_xcr,epsilon> in delta_0
      // and <p_q_xcr,q_c> -1-> <p,q_r> in delta_1
      // where p_q_xcr = (p,q_x,q_c,q_r), and w depends on sigma
      
      State src = Trans::getExit(*rit);
      State tgt = Trans::getReturnSite(*rit);
      
      if( Trans::getReturnSym(*rit) == WALI_WILD )
	wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w 
      else
	wgt = wg.getWeight(src, getClientInfo(src), 
			   Trans::getReturnSym(*rit),
			   WeightGen::EXIT_TO_RET,  
			   tgt, getClientInfo(tgt));    // w     
      
      //Note: if you change this, make sure you modify the code in NWPForest.createCA()
      Key rstate = getControlLocation(src,Trans::getCallSite(*rit),tgt);  //p_q_xcr
      
      result.add_rule(program,                              //from_state (p)
		      src,                                  //from_stack (q_x)
		      rstate,                               //to_state (p_q_xcr == (p,q_x,q_c,q_r))
		      wgt);                                 //weight (w)
      
      
      wgt = wg.getOne();                                    // 1                      
      
      result.add_rule(rstate,                               //from_state (p_q_xcr == (p,q_x,q_c,q_r))
		      Trans::getCallSite(*rit),             //from_stack (q_c)
		      program,                              //to_state (p)
		      tgt,                                  //to_stack (q_r)
		      wgt);                                 //weight (1)
    }
  
  return result;
}

map<wali::Key, string> state_preds;

void Tokenize(const string& str,
	      vector<string>& tokens,
	      const string& delimiters = " ")
{    
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos)
    {
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      lastPos = str.find_first_not_of(delimiters, pos);
      pos = str.find_first_of(delimiters, lastPos);
    }
}

pair<wali::Key, wali::Key> dissectIntersectState(string statelabel) {
    //std::cout << "dissectIntersectState(" << statelabel << ")\n";
  
  statelabel.replace(statelabel.find("("), 1, "");
  statelabel.replace(statelabel.find(")"), 1, "");

  vector<string> tokens1;
  vector<string> tokens2;  
  Tokenize(statelabel,tokens1,"#");
  Tokenize(tokens1[1],tokens2,",");

  int first = atoi(tokens2[0].c_str());
  int second = atoi(tokens2[1].c_str());

  return pair<wali::Key, wali::Key>((wali::Key)first, (wali::Key)second);
}

class STR
{
public:
  STR() { };
  STR( std::string st ) : str(st) { }
  
  size_t hash( ) const
  {
    return str.length();
  } 
  
  std::ostream& print( std::ostream & o ) const
  {
    return o << str;
  }
  
  bool operator==( const STR & other ) const
  {
    return str == other.str;
  }
  
  bool operator<( const STR & other ) const
  {
    return str < other.str;
  }
  
private:
  std::string str;
};

namespace wali {
  
  namespace witness {
		
    class PathVisitor : public wali::witness::Visitor {
    protected:
				
      wali::nwa::NWA *o;
      vector<string> path;
      vector<string> pathPreds;
      vector<wali::Key> states;
      vector<wali::Key> symbs;
      map<wali::Key, string> stateLabels;
			
    public:
				
      PathVisitor(wali::nwa::NWA *orig) {
	o = orig;

        //std::cout << "PathVisitor(...):\n";

	set<wali::Key> ostates = o->getStates();
	for(set<wali::Key>::iterator kit = ostates.begin(); kit != ostates.end(); kit++) {

	  stringstream ss(stringstream::in | stringstream::out);

	  printKey(ss, *kit);
	  ss.flush();
	  stateLabels.insert(pair<wali::Key, string>(*kit, ss.str()));

          //std::cout<< "  stateLabels maps [" << *kit << "] to [" << ss.str() << "]\n";
	}
						
      }
				
      ~PathVisitor() {}

      bool visit( wali::witness::Witness * w ) {
          (void) w;
          return true;
      }
		
      bool visitExtend( wali::witness::WitnessExtend * w ) {
          (void) w;
          return true;
      }
		
      bool visitCombine( wali::witness::WitnessCombine * w ) {
          (void) w;
          return true;
      }

      // Keeps track of everything needed to 
      bool visitRule( wali::witness::WitnessRule * w ) {
								
	wali::Key from = w->getRuleStub().from_stack();
	wali::Key fromstate = w->getRuleStub().from_state();
	wali::Key to = w->getRuleStub().to_stack1();
	wali::Key tostate = w->getRuleStub().to_state();
	wali::Key sym;

        //std::cout << "visitRule(...):\n"
        //          << "  from stack [" << from << "] " << key2str(from) << "\n"
        //          << "  from state [" << fromstate << "] " << key2str(fromstate) << "\n"
        //          << "  to stack1 [" << to << "] " << key2str(to) << "\n"
        //          << "  to state  [" << tostate << "] " << key2str(tostate) << std::endl;

	if( to == wali::WALI_EPSILON) {
	  symbs.push_back(from);
	  states.push_back(tostate);

	  return true;
	}

	bool found;
	
	if(states.size() > 0 && fromstate == states.back()) {
          set<wali::Key> r = query::getReturnSym(*o, symbs.back(), from, to);
	  sym = *(r.begin());

	  states.pop_back();
	  symbs.pop_back();

	  if(r.size() > 0) found = true;
	} else
          found = query::getSymbol(*o,from,to,sym);

	if(!found) return true;
					
	string symstr = key2str(sym);

	if(symstr != "") {
          pathPreds.push_back(state_preds[to]);
	  path.push_back(symstr);
        }
					
	return true;
      }
				
      vector<string> getPath() {return path;}

      vector<string> getPathPreds() {return pathPreds;}
		
      bool visitTrans( wali::witness::WitnessTrans * w ) {
          (void) w;
          return true;
      }
		
      bool visitMerge( wali::witness::WitnessMerge * w ) {
          (void) w;
          return true;
      }
		
    };
  }
}

vector<string> getWord(wali::nwa::NWA *aut) {
  
  vector<string> ret;

  if(!query::languageIsEmpty(*aut)) {
    
    wali::nwa::ReachGen wg;
    wali::wfa::WFA query;
    wali::wpds::WPDS conv = aut->_private_NWAtoPDScallsWitness(wg);
    
    set<wali::Key> wpstates = conv.get_states();
    set<wali::Key>::iterator bi = wpstates.begin();
    wali::Key state = *bi;		
    
    set<wali::Key>::iterator i, j;
    set<wali::Key> inits = aut->getInitialStates();
    set<wali::Key> finals = aut->getFinalStates();
    wali::Key accept = wali::getKey("__accept");

    // Try to find a path from some initial state to some final state
    for(j = inits.begin(); j != inits.end(); j++) {
      for(i = finals.begin(); i != finals.end(); i++) {

	// Construct the query automaton
	query.addTrans(state, *j, accept, wg.getOne());
	query.set_initial_state(state);
	query.add_final_state(accept);
	
	// Execute the post* query
	wali::wfa::WFA path = conv.poststar(query);
	path.path_summary();

	// Find a path to the final state
	//wali::Key start = path.getInitialState();	
	wali::wfa::TransSet t = path.match(state,*i);

	// Collect the transitions
	for(wali::wfa::TransSet::iterator itt = t.begin(); itt != t.end(); itt++) {
	  sem_elem_t se = path.getState((*itt)->to())->weight()->extend((*itt)->weight());
	  if(se->equal(se->zero())) continue;
	  wali::witness::PathVisitor v(aut);					
	  wali::witness::Witness* wit = dynamic_cast<wali::witness::Witness*>(se.get_ptr());		
	  if( 0 != wit ) {
	    wit->accept(v);
	    vector<string>::iterator it;
	    vector<string> p = v.getPath();
	    vector<string> preds = v.getPathPreds();
	    for(it = p.begin(); it != p.end(); it++) {
	      ret.push_back(*it);
	    }
	  }
	}
      }
    }
  }

  return ret;
}
