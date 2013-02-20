/**
 * @author Prathmesh Prabhu
 */

// ::wali::wpds::nwpds
#include "wali/wpds/nwpds/NWPDS.hpp"
// ::wali::wpds
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"
// ::wali::wpds::ewpds
#include "wali/wpds/ewpds/ERule.hpp"
#include "wali/wpds/ewpds/ETrans.hpp"
// ::wali::Wfa
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/ITrans.hpp"
// ::wali
#include "wali/Key.hpp"
#include "wali/Common.hpp"
#include "wali/KeySource.hpp"
#include "wali/KeyPairSource.hpp"
#include "wali/Worklist.hpp"
#include "wali/DefaultWorklist.hpp"
//#include "wali/KeyOrderWorklist.hpp"
#include <boost/cast.hpp>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace wali;
using namespace wali::wpds;
using namespace wali::wpds::ewpds;
using namespace wali::wpds::fwpds;
using namespace wali::wpds::nwpds;

namespace wali 
{
  namespace wpds
  {
    namespace nwpds
    {
      const std::string NWPDS::XMLTag("NWPDS");
    } // namespace nwpds
  } //namespace wpds
} //namewpace wali

// ////////////////////////////////////////////////////////////////
// class NWPDS
// ////////////////////////////////////////////////////////////////
NWPDS::NWPDS(bool b) :
  EWPDS(),
  dbg(b)
{
  //XXX:HACK?
  //we disable wali::strict
  //We allow incoming transitions to query automata. 
  //Two newton steps are like a continued pre/post star query interjected
  //by other stuff. So we want to allow incoming transitions there (I think)
  wali::set_strict(false);
  worklist = new DefaultWorklist<wfa::ITrans>; //new KeyOrderWorklist;
}

NWPDS::NWPDS(ref_ptr<wpds::Wrapper> wrapper, bool b) :
  EWPDS(wrapper),
  dbg(b)
{
  //XXX:HACK?
  //we disable wali::strict
  //We allow incoming transitions to query automata. 
  //Two newton steps are like a continued pre/post star query interjected
  //by other stuff. So we want to allow incoming transitions there (I think)
  wali::set_strict(false);
}

NWPDS::NWPDS(const NWPDS& f) :
  EWPDS(f),
  newtonWl(f.newtonWl),
  dbg(f.dbg)
{
  //XXX:HACK?
  //we disable wali::strict
  //We allow incoming transitions to query automata. 
  //Two newton steps are like a continued pre/post star query interjected
  //by other stuff. So we want to allow incoming transitions there (I think)
  wali::set_strict(false);
}

NWPDS::~NWPDS()
{
  stack2ConstMap.clear();
  savedRules.clear();
}

/**
 * Create a key for the Old values of variables in the equations system
 * if a key does not already exist. Return the existing / created key.
 **/
Key NWPDS::getOldKey(Key newKey)
{
  if(stack2ConstMap.find(newKey) == stack2ConstMap.end()){
    string oldKeyStr = key2str(newKey) + "_NEWTON_OLDVAL";
    Key oldKey = getKey(oldKeyStr);
    stack2ConstMap[newKey] = oldKey;
  }
  return stack2ConstMap[newKey];
}

#if 0
/*
   replace each delta_2 rule <p,y> -> <p',y'y''>
   by two rules:
     <p,y> -> <p',y't''> and <p,y> -> <p't'y''>
     here t' and t'' are new stack symbols that will hold on to the values of y',y'' from last Newton 
     iteration during saturation.
*/
void NWPDS::prestarSetupPds()
{
  if(dbg){
    *waliErr << "NWPDS::prestarSetupPds()\n";
    WpdsRules wr;
    this->for_each(wr);
    *waliErr << "Printing out rules before processing.\n";
    *waliErr << "Delta_1 rules: \n";
    for(std::set<Rule>::iterator it = wr.stepRules.begin();
        it != wr.stepRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
    *waliErr << "\n\nDelta_2 rules: \n";
    for(std::set<Rule>::iterator it = wr.pushRules.begin();
        it != wr.pushRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
    *waliErr << "\n\nDelta_0 rules: \n";
    for(std::set<Rule>::iterator it = wr.popRules.begin();
        it != wr.popRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
  }
  stack2ConstMap.clear();
  savedRules.clear();
  Delta2Rules dr;
  this->for_each(dr);
  for(std::vector<rule_t>::iterator it = dr.rules.begin();
      it != dr.rules.end();
      ++it){
    rule_t r = *it;
    if(dbg){
      *waliErr << "[SetupPDS] Processing Rule:" << std::endl;
      r->print(*waliErr);
      *waliErr << endl;
    }
    //backup the current rule
    savedRules.push_back(r);

    Key p = r->from_state();
    Key y = r->from_stack();
    Key p_prime = r->to_state();
    Key y_prime = r->to_stack1();
    Key y_prime_prime = r->to_stack2();

    assert(y_prime_prime != WALI_EPSILON);
    assert(y_prime != WALI_EPSILON);

    Key t_prime = getOldKey(y_prime);
    Key t_prime_prime = getOldKey(y_prime_prime);

    //erase rule from the WPDS
    erase_rule(p,y,p_prime,y_prime,y_prime_prime);
    ERule * er = boost::polymorphic_downcast<ERule*>(r.get_ptr()); //change to static_cast ? 
    //<p,y> -> <p',y't''>
    add_rule(p,y,p_prime,y_prime,t_prime_prime,er->weight(),er->merge_fn());
    //<p,y> -> <p',t'y''>
    add_rule(p,y,p_prime,t_prime,y_prime_prime,er->weight(),er->merge_fn());
  }
  if(dbg){
    WpdsRules wr;
    this->for_each(wr);
    *waliErr << "Printing out rules after processing.\n";
    *waliErr << "Delta_1 rules: \n";
    for(std::set<Rule>::iterator it = wr.stepRules.begin();
        it != wr.stepRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
    *waliErr << "\n\nDelta_2 rules: \n";
    for(std::set<Rule>::iterator it = wr.pushRules.begin();
        it != wr.pushRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
    *waliErr << "\n\nDelta_0 rules: \n";
    for(std::set<Rule>::iterator it = wr.popRules.begin();
        it != wr.popRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
  }
}
#endif

/*
   replace each delta_2 rule <p,y> -> <p',y'y''>
   by three rules:
    - False call site, real call:
     <p,t> -> <p',y'y''>
    - Real call site, summary call:
     <p,y> -> <p't'y''> and <p't'> -><p',.> (w = 0)
    - False call site, summary call:
     <p,t> -> <p',t'y''> // TODO#1
     here t and t' are new stack symbols that will hold on to the values of y,y' from last Newton 
     iteration during saturation.
*/
void NWPDS::poststarSetupPds()
{
  if(dbg){
    *waliErr << "NWPDS::poststarSetupPds()\n";
    WpdsRules wr;
    this->for_each(wr);
    *waliErr << "Printing out rules before processing.\n";
    *waliErr << "Delta_1 rules: \n";
    for(std::set<Rule>::iterator it = wr.stepRules.begin();
        it != wr.stepRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
    *waliErr << "\n\nDelta_2 rules: \n";
    for(std::set<Rule>::iterator it = wr.pushRules.begin();
        it != wr.pushRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
    *waliErr << "\n\nDelta_0 rules: \n";
    for(std::set<Rule>::iterator it = wr.popRules.begin();
        it != wr.popRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
  }
  stack2ConstMap.clear();
  savedRules.clear();
  oldCallSite.clear();
  oldFuncEntry.clear();
  oldFuncEntryPair.clear();

  // Find the delta2 rules in PDS
  Delta2Rules dr;
  this->for_each(dr);
  for(std::vector<rule_t>::iterator it = dr.rules.begin();
      it != dr.rules.end();
      ++it){
    rule_t r = *it;
    if(dbg){
      *waliErr << "[SetupPDS] Processing Rule:" << std::endl;
      r->print(*waliErr);
      *waliErr << endl;
    }
    // backup the current rule
    savedRules.push_back(r);

    // p, y, p', y', y''
    Key p = r->from_state();
    Key y = r->from_stack();
    Key p_prime = r->to_state();
    Key y_prime = r->to_stack1();
    Key y_prime_prime = r->to_stack2();

    assert(y_prime_prime != WALI_EPSILON);
    assert(y_prime != WALI_EPSILON);

    // t, t' are keys to hold on to values from the last newton round for y and y' respectively.
    Key t = getOldKey(y);
    Key t_prime = getOldKey(y_prime);

    // update maps from new call site and entry function generated state to old ones
    oldCallSite[y] = t;
    oldFuncEntryPair[IntState(p_prime, y_prime)] = IntState(p_prime, t_prime);
    // erase rule from the WPDS
    erase_rule(p,y,p_prime,y_prime,y_prime_prime);
    ERule * er = boost::polymorphic_downcast<ERule*>(r.get_ptr()); //change to static_cast ? 
    //Add: <p,t> -> <p',y'y''>
    add_rule(p,t,p_prime,y_prime,y_prime_prime,er->weight(),er->merge_fn());
    //Add: <p,y> -> <p',t'y''>
    add_rule(p,y,p_prime,t_prime,y_prime_prime,er->weight(),er->merge_fn());
    // <p,t> -> <p',t'y''>
    add_rule(p,t,p_prime,t_prime,y_prime_prime,er->weight(),er->merge_fn());
    //Add: <p',t'> -> <p',.>
    add_rule(p_prime,t_prime,p_prime,er->weight()->zero());
    
  }
  if(dbg){
    WpdsRules wr;
    this->for_each(wr);
    *waliErr << "Printing out rules after processing.\n";
    *waliErr << "Delta_1 rules: \n";
    for(std::set<Rule>::iterator it = wr.stepRules.begin();
        it != wr.stepRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
    *waliErr << "\n\nDelta_2 rules: \n";
    for(std::set<Rule>::iterator it = wr.pushRules.begin();
        it != wr.pushRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
    *waliErr << "\n\nDelta_0 rules: \n";
    for(std::set<Rule>::iterator it = wr.popRules.begin();
        it != wr.popRules.end();
        ++it)
      (*it).print(*waliErr) << std::endl;
  }
}


void NWPDS::restorePds()
{
  //erase all added rules
  for(std::vector<rule_t>::iterator it = addedRules.begin(); it != addedRules.end(); ++it){
    rule_t r = *it;
    erase_rule(r->from_state(), r->from_stack(), r->to_state(), r->to_stack1(), r->to_stack2());
  }
  // empty addedRules, so that we don't hold on to the references
  addedRules.clear();

  //Now restore the original rules
  for(std::vector<rule_t>::iterator it = savedRules.begin(); it != savedRules.end(); ++it){
    ERule * er = boost::polymorphic_downcast<ERule*>(it->get_ptr());
    add_rule(er->from_state(), er->from_stack(), er->to_state(),
        er->to_stack1(), er->to_stack2(), er->weight(), er->merge_fn());
  }
  // Don't unnecessarrily hold on to references
  savedRules.clear();
}

/**
 * Walk all transitions in the WFA that were changed in the last saturation phase.
 * Two types of transitions need attention.
 * Update fake call-site:
 *   (1) For every transition <p,y,p'> where oldCallSite[y] == t, wt(<p,t,p'>) += wt(<p,y,p'>)
 *   (2) For every transition <p,*,(p',y')> where oldFuncEntry[(p',y')] == (p',t'), wt(<p,*,(p',t')>) += wt(<p,*,(p'y')>)
 **/
void NWPDS::poststarUpdateFa(wfa::WFA& fa)
{
  wali::wfa::ITrans *t;
  for(worklist_t::iterator it = newtonWl.begin();
      it != newtonWl.end();
      ++it){
    t = *it;

    if(dbg){
      *waliErr << "[UpdateFa]: " << std::endl << "Processing:" << std::endl;
      t->print(*waliErr) << std::endl;
    }

    // Case 1: 
    if(oldCallSite.find(t->stack()) != oldCallSite.end()){
      Key oldStack = oldCallSite[t->stack()];
      wfa::Trans old;
      sem_elem_t oldWt, newWt;
      bool found = fa.find(t->from(), oldStack, t->to(), old);
      if(!found)
        oldWt = t->weight()->zero();
      else
        oldWt = old.weight();
      newWt = t->weight();
      if(newWt != NULL && ! newWt->equal(oldWt)){
        wali::wfa::ITrans * insertTrans;
        // copy the current trans with old stack symbol
        insertTrans = t->copy(t->from(), oldStack, t->to());
        // Add it to the WFA
        insertTrans = fa.insert(insertTrans);
        Config * cfg = make_config(insertTrans->from(),insertTrans->stack());
        insertTrans->setConfig(cfg);
        // insert it into the worklist for next round. 
        worklist->put(insertTrans);
        if(dbg) {
          insertTrans->print(*waliErr << "Updated fake call-site for ") << endl;
          oldWt->print(*waliErr << "Existing weight on the constant edge: ") << endl;
          newWt->print(*waliErr << "Replaced with: ") << endl;
        }
      }
    }

    // Case 2:
    if(oldFuncEntry.find(t->to()) != oldFuncEntry.end() && t->stack() == WALI_EPSILON){
      Key oldTo = oldFuncEntry[t->to()];
      wfa::Trans old;
      sem_elem_t oldWt, newWt;
      bool found = fa.find(t->from(), WALI_EPSILON, oldTo, old);
      if(!found)
        oldWt = t->weight()->zero();
      else
        oldWt = old.weight();
      newWt = t->weight();
      if(newWt != NULL && ! newWt->equal(oldWt)){
        wali::wfa::ITrans * insertTrans;
        // copy the current trans with old to state
        insertTrans = t->copy(t->from(), WALI_EPSILON, oldTo);
        // insert it in the WFA.
        insertTrans = fa.insert(insertTrans);
        Config * cfg = make_config(insertTrans->from(),insertTrans->stack());
        insertTrans->setConfig(cfg);
        // add it to the worklist for the next round
        worklist->put(insertTrans);        
        if(dbg) {
          insertTrans->print(*waliErr << "Updated function summary for ") << endl;
          oldWt->print(*waliErr << "Existing weight on the constant edge: ") << endl;
          newWt->print(*waliErr << "Replaced with: ") << endl;
        }
      }
    }
  } 
  newtonWl.clear();
}
 
void NWPDS::prestar(wfa::WFA const &, wfa::WFA &)
{
  assert(0);
#if 0
  int newtonSteps = 0;
  genStates.clear();
  savedRules.clear();
  stack2ConstMap.clear();
  state2ConstMap.clear();
  state2StackMap.clear();

  prestarSetupPds();
  EWPDS::addEtrans = true;
  EWPDS::prestarSetupFixpoint(input, output);
  EWPDS::addEtrans = false;

  CreateInitialNewtonWl cinw(newtonWl);
  output.for_each(cinw);
  do{
    EWPDS::prestarComputeFixpoint(output);
    updateFa(output);
    newtonSteps++;
  }while(worklist->empty());
  //if(dbg)
  *waliErr << "Total Newton Steps: " << newtonSteps << std::endl;
  restorePds();
  EWPDS::unlinkOutput(output);
  EWPDS::currentOutputWFA = 0;
  cleanUpFa(output);
#endif
}

/**
 * poststar using newton method
 * This involves --
 *  (1) Change the PDS so that we replace every delta2 rule <p,y> --> <p',y'y''> by 3 rules
 *      (i)   <p,t> --> <p',y'y''> 
 *      (ii)  <p,y> --> <p',t'y''> and <p',t'> --> <p',*> with weight = 0
 *      (iii) <p,t> --> <p',t'y''>  // TODO#1
 *  The idea behind this transformation is as follows:
 *  <p,y> --> <p',y'y''> gives us in the poststar GFA problem, the equation -- 
 *  \Forall{q} (p,y'',q) = (p,y,q) . (p_{y'},*,q)
 *  where p_{y'} is the intermediate state generated during poststar for the entry function y', and * is the epsilon symbol.
 *  Upon linearlzing this equation, we obrain,
 *  \Forall{q} (p,y'',q) = ((p,y,q).(p_{t'},*,q)) + ((p,t,q).(p_{y'},*,q)) + ((p,t,q) . (p_{t'},*,q))
 *  where the values with y (y')replaced by t (t') are constants corresponding to the values obtained in the last newton
 *  round.
 *  We can obtain this linearized GFA problem if we change our PDS as above. Note that (ii) replace the actual function 
 *  body for the call at t' with a 0 weight edge. This ensures that no information flows through the summary call. 
 *
 *  (2) Update the WFA to correctly setup the values on the transitions (p,t,q) and (p_{t'},*,q) between newton rounds.
 *  For this, between newton rounds, we copy values from (p,y,q) to (p,t,q) and (p_{y'},*,q) to (p_{t'},*,q).
 **/
void NWPDS::poststar(wfa::WFA const & input, wfa::WFA & output)
{
  int newtonSteps = 0;
  savedRules.clear();
  stack2ConstMap.clear();
  oldCallSite.clear();
  oldFuncEntry.clear();
  oldFuncEntryPair.clear();

  // Step 1:
  poststarSetupPds();
  // Calls EWPDS::poststarSetupFixpoint, and does some light book keeping.
  poststarSetupFixpoint(input,output);

  // Add all transitions in output to the outer worklist (that goes across linear solves)
  CreateInitialNewtonWl cinw(newtonWl);
  output.for_each(cinw);
  do{
    // linear solve.
    EWPDS::poststarComputeFixpoint(output);
    // Step 2:
    poststarUpdateFa(output);
    if(dbg){
      std::stringstream ss;
      ss << "outfa_int_" << newtonSteps << ".dot";
      fstream outfile(ss.str().c_str(), fstream::out);
      output.print_dot(outfile, true);
      outfile.close();
    }
    newtonSteps++;
  }while(!worklist->empty());
  *waliErr << "Total Newton Steps: " << newtonSteps << std::endl;
  // Fix the delta2 rules to be as before
  restorePds();
  EWPDS::unlinkOutput(output);
  EWPDS::currentOutputWFA = 0;
  // Remove all transitions that use t / t' anywhere.
  cleanUpFa(output);
}

// Invert the callSite and funcEntry maps and then
// use RemoveOldTrans to set all relevant weights to 0.
// Finally, prune to remove all 0 transitions.
void NWPDS::cleanUpFa(wfa::WFA& output)
{
  Key2KeyMap old2NewStackMap;
  Key2KeyMap old2NewStateMap;
  for(Key2KeyMap::iterator iter = oldCallSite.begin(); iter != oldCallSite.end(); ++iter)
    old2NewStackMap[iter->second] = iter->first; 
  for(Key2KeyMap::iterator iter = oldFuncEntry.begin(); iter != oldFuncEntry.end(); ++iter)
    old2NewStateMap[iter->second] = iter->first;
  RemoveOldTrans rot(old2NewStackMap,old2NewStateMap);
  output.for_each(rot);
  output.prune();
}

void NWPDS::poststarSetupFixpoint( WFA const & input, WFA& fa )
{
  EWPDS::poststarSetupFixpoint(input, fa);
  // gen_state can be used now. So create intermediate states from the store pairs
  // oldFuncEntryPair maps pair(p,y') --> pair(p',t')
  // convert this to to map between the corresponding keys.
  for(State2StateMap::iterator it = oldFuncEntryPair.begin(); it != oldFuncEntryPair.end(); ++it)
    oldFuncEntry[gen_state(it->first.first, it->first.second)] = gen_state(it->second.first, it->second.second);
  // We don't need the pair map anymore
  oldFuncEntryPair.clear();
}


void NWPDS::update(Key from, Key stack, Key to, sem_elem_t se, Config * cfg)
{

  wali::wfa::ITrans *t;
  if(addEtrans) {
    t = currentOutputWFA->insert(new wali::wpds::ewpds::ETrans(from, stack, to,
          0, se, 0));
  } else {
    t = currentOutputWFA->insert(new wali::wfa::Trans(from, stack, to, se));
  }

  t->setConfig(cfg);
  if (dbg && t->modified()) {
    t->print(cout << "Modified: ") << endl;
    worklist->put( t );
    newtonWl.push_back( t );
  }
}

wali::wfa::ITrans* NWPDS::update_prime( Key from, wali::wfa::ITrans* call, rule_t r, sem_elem_t delta, sem_elem_t wWithRule)
{
  //
  // !!NOTE!!
  // This code is copied in FWPDS::update_prime.
  // Changes here should be reflected there.
  //
  ERule* er = (ERule*)r.get_ptr();
  wali::wfa::ITrans* tmp = 
    new wali::wpds::ewpds::ETrans(
        from, r->to_stack2(), call->to(),
        delta, wWithRule, er);
  wali::wfa::ITrans* t = currentOutputWFA->insert(tmp);
  if(dbg && t->modified()){
    t->print(cout << "Modified: ") << endl;
    newtonWl.push_back( t );
  }
  return t;
}

/////////////////////////////////////////////////////////////////
// class Dela2Rules
/////////////////////////////////////////////////////////////////

Delta2Rules::Delta2Rules() :
  RuleFunctor()
{}

Delta2Rules::~Delta2Rules() {}

void Delta2Rules::operator() (rule_t & r)
{
  if( r->is_rule2() )
    rules.push_back(r);
}

/////////////////////////////////////////////////////////////////
// class RemoveOldTrans
/////////////////////////////////////////////////////////////////
RemoveOldTrans::RemoveOldTrans(const NWPDS::Key2KeyMap& m, const NWPDS::Key2KeyMap& g) : oldStackMap(m), oldStateMap(g){}

void RemoveOldTrans::operator() (wali::wfa::ITrans* t)
{
  //is stack phony?
  if(oldStackMap.find(t->stack()) != oldStackMap.end())
    if(t->weight() != NULL)
      t->setWeight(t->weight()->zero());
  //is from phony?
  if(oldStateMap.find(t->from()) != oldStateMap.end())
    if(t->weight() != NULL)
      t->setWeight(t->weight()->zero());
  //is to phony?
  if(oldStateMap.find(t->to()) != oldStateMap.end())
    if(t->weight() != NULL)
      t->setWeight(t->weight()->zero());
}


/////////////////////////////////////////////////////////////////
// class CreateInitialNewtonWl
/////////////////////////////////////////////////////////////////
CreateInitialNewtonWl::CreateInitialNewtonWl(NWPDS::worklist_t& w): wl(w) {}
void CreateInitialNewtonWl::operator () (wali::wfa::ITrans* t)
{
  wl.push_back(t);
}
