/**
 * @author Akash Lal
 */

#include "ProgAnalysis.hpp"
#include "Cfg.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/Config.hpp"
#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wpds/fwpds/SWPDS.hpp"

#include "wali/wfa/Trans.hpp"
#include "wali/wfa/State.hpp"

#include "wali/wpds/Wrapper.hpp"
#include "wali/witness/WitnessWrapper.hpp"

#include <iostream>
#include <sstream>

// Constructor that initializes the pds. It takes the following arguments:
// The CFG of the main procedure of the program.
// The type of WPDS class to use for analysis
ProgAnalysis::ProgAnalysis(CFG *main, WpdsType t, bool witnesses) : wpdsType(t), b_witnesses(witnesses) 
{

  assert(main != 0);
  wali::wpds::Wrapper* wrapper = (b_witnesses) ? new wali::witness::WitnessWrapper() : 0;

  // Initialize the PDS
  switch(wpdsType) {

  case USING_WPDS:
    pds = new wali::wpds::WPDS(wrapper);
    break;

  case USING_EWPDS:
    pds = new wali::wpds::ewpds::EWPDS(wrapper);
    break;

  case USING_FWPDS:
    pds = new wali::wpds::fwpds::FWPDS(wrapper);
    break;

  case USING_SWPDS:
    pds = new wali::wpds::fwpds::SWPDS(wrapper);
    break;
  }

  // Create the pds state
  pds_state = wali::getKey("PDS_STATE");

  // Entry point of the program
  main_entry = main->getEntry()->getWpdsKey();

  // Add WPDS rules by traversing the CFG. 

  // The set of all untraversed CFGs
  std::set<CFG *> cfgs;
  // The set of all traversed CFGs
  std::set<CFG *> cfgs_done;

  cfgs.insert(main);

  while(!cfgs.empty()) {
    CFG* cfg = *cfgs.begin();
    cfgs.erase(cfg);
    cfgs_done.insert(cfg);

    traverseCFG(cfg, cfgs, cfgs_done);
  }

  // If SWPDS is being used, then preprocess the WPDS to compute relevant information that will
  // be used later to optimize MOP queries.
  if(wpdsType == USING_SWPDS) {
    wali::wpds::fwpds::SWPDS *spds = static_cast<wali::wpds::fwpds::SWPDS *>(pds);

    // SWPDS needs the set of all possible entry points of the program. The easy thing to do is to
    // just add the entry point of main. SWPDS will itself find all reachable code from main by
    // traversing the CFG structure. The other option is to add the entry points of all the procedures,
    // which can be used when there is code unreachable from main, but one still desires to make MOP
    // queries over that code.

    spds->addEntryPoint(main_entry);
    spds->preprocess();
  }

}

bool ProgAnalysis::isWitnessed() const
{
  return b_witnesses;
}

/*
 * Traverse the CFG cfg and add all its edges as WPDS rules into "ProgAnalysis::pds".
 * The set "cfgs" keeps track of CFGs not seen before and the set "cfgs_done" keeps
 * track of CFGs seen before. Any CFG that is the callee of an edge in "cfg" and is
 * not in the set "cfgs_done" is added to "cfgs".
 */

void ProgAnalysis::traverseCFG(CFG *cfg, std::set<CFG *> &cfgs, std::set<CFG *> &cfgs_done) {

  // Addition of rules only depends of whether the pds
  // is of type wpds::WPDS or not (all others inherit from wpds::ewpds::EWPDS).

  wali::wpds::ewpds::EWPDS *epds = 0;
  if(wpdsType != USING_WPDS) {
    epds = static_cast<wali::wpds::ewpds::EWPDS *>(pds);
  }

  const std::set<CFGEdge *> &edges = cfg->getEdges();
  std::set<CFGEdge *>::const_iterator it;

  for(it = edges.begin(); it != edges.end(); it++) {

    CFGEdge *edge = *it;
    
    if(!edge->isCall()) {
      pds->add_rule(pds_state, edge->getSource()->getWpdsKey(), pds_state, edge->getTarget()->getWpdsKey(), edge->getWeight());
    } else {
      CFG *callee = edge->getCallee();
      if(wpdsType == USING_WPDS) {
	pds->add_rule(pds_state, edge->getSource()->getWpdsKey(), pds_state, callee->getEntry()->getWpdsKey(), edge->getTarget()->getWpdsKey(), 
		      edge->getWeight());
      } else {
	epds->add_rule(pds_state, edge->getSource()->getWpdsKey(), pds_state, callee->getEntry()->getWpdsKey(), edge->getTarget()->getWpdsKey(), 
		       edge->getWeight(), edge->getMergeFn());
      }
      if(cfgs_done.find(callee) == cfgs_done.end()) {
	cfgs.insert(callee);
      }
    }

    // store the weight
    if(!se.is_valid()) {
      se = edge->getWeight();
    }

  }

}

// Calculate MOP(main->entry, n)
wali::sem_elem_t ProgAnalysis::computeMOP(CFGNode *n, SearchDirection d) {

  wali::Key node = n->getWpdsKey();

  wali::wfa::WFA answer;
  wali::sem_elem_t ret;

  if(d == USE_FORWARD_SEARCH) {
    doForwardSearch(answer);

    /* method 1:

    // intersect answer with an automaton that accepts (n \Gamma^*).
    answer.filter(node);
    // Do path summary
    answer.path_summary();
    // Read off the weight on the initial state
    ret = answer.getState(pds_state)->weight();

    */

    // method 2
    answer.path_summary();
    ret = se->zero();

    wali::wfa::TransSet tset;
    wali::wfa::TransSet::iterator titer;

    tset = answer.match(pds_state,node);
    for(titer = tset.begin(); titer != tset.end(); titer++) {
      wali::wfa::ITrans *t = *titer;

      wali::sem_elem_t tmp(answer.getState(t->to())->weight());

      tmp = tmp->extend(t->weight());
      ret = ret->combine(tmp);
    }

  } else {
    doBackwardSearch(n->getWpdsKey(), answer);

    ret = se->zero();

    wali::wfa::TransSet tset;
    wali::wfa::TransSet::iterator titer;

    tset = answer.match(pds_state,main_entry);
    for(titer = tset.begin(); titer != tset.end(); titer++) {
      wali::wfa::ITrans *t = *titer;
      if(!answer.isFinalState(t->to())) continue;

      wali::sem_elem_t tmp(answer.getState(t->to())->weight());
      ret = ret->combine(tmp);
    }

  }

  return ret;

}

// Do forward search from main_entry
void ProgAnalysis::doForwardSearch(wali::wfa::WFA &answer) {
  // Create an automaton to accept the configuration <pds_state, main_entry>
  wali::wfa::WFA query;

  // Create an accepting state for the automaton
  wali::Key accept = wali::getKey("__accept");

  query.addTrans( pds_state, main_entry, accept, se->one() );
  query.set_initial_state( pds_state );
  query.add_final_state( accept );

  pds->poststar(query,answer);
}

// Do Backward search from the set of configurations {n \Gamma^*}
void ProgAnalysis::doBackwardSearch(wali::Key node, wali::wfa::WFA &answer) {
  // Create an automaton to accept the configurations {n \Gamma^*}
  wali::wfa::WFA query;

  // Create an accepting state for the automaton
  wali::Key accept = wali::getKey("__accept");

  // Find the set of all return points
  wali::wpds::WpdsStackSymbols syms;
  pds->for_each(syms);


  query.addTrans( pds_state, node, accept, se->one() );

  set<wali::Key>::iterator it;
  for(it = syms.returnPoints.begin(); it != syms.returnPoints.end(); it++) {
    query.addTrans(accept, *it, accept, se->one());
  }

  query.set_initial_state( pds_state );
  query.add_final_state( accept );

  pds->prestar(query,answer);
}


// Do Backward search from the set of configurations {n \Gamma^*}
void ProgAnalysis::doBackwardSearch(std::vector<wali::Key> &node_stack, wali::wfa::WFA &answer) {
  assert( node_stack.size() > 0 );

  // Create an automaton to accept the configuration <pds_state, node_stack>
  wali::wfa::WFA query;
  wali::Key temp_from = pds_state; 
  wali::Key temp_to = wali::WALI_EPSILON;// add initialization to skip g++ warning. safe b/c of above assertion.

  for(size_t i = 0; i < node_stack.size(); i++) {
    std::stringstream ss;
    ss << "__tmp_state_" << i;
    temp_to = wali::getKey(ss.str());
    query.addTrans( temp_from, node_stack[i], temp_to, se->one() );
    temp_from = temp_to;
  }

  query.set_initial_state( pds_state );
  query.add_final_state( temp_to );

  pds->prestar(query,answer);
}

int main() {
  std::cout << "Program Analysis Example" << std::endl;
  return 0;
}


