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
// ::wali::Wfa
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/Trans.hpp"
// ::wali
#include "wali/Key.hpp"
#include "wali/Common.hpp"

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
  var2ConstMap.clear();
  savedRules.clear();
}

Key NWPDS::getOldKey(Key newKey)
{
  if(var2ConstMap.find(newKey) == var2ConstMap.end()){
    string oldKeyStr = key2str(newKey) + "_NEWTON_OLDVAL";
    Key oldKey = getKey(oldKeyStr);
    var2ConstMap[newKey] = oldKey;
  }
  return var2ConstMap[newKey];
}

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
  var2ConstMap.clear();
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
    ERule * er = dynamic_cast<ERule*>(r.get_ptr()); //change to static_cast ? 
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

void NWPDS::prestarRestorePds()
{
  //erase all existing delta 2 rules
  WpdsRules wr;
  this->for_each(wr);
  for(std::set<Rule>::iterator it = wr.pushRules.begin();
      it != wr.pushRules.end();
      ++it){
    Rule r = *it;
    erase_rule(r.from_state(), r.from_stack(), r.to_state(),
        r.to_stack1(), r.to_stack2());
  }
  //Now restore the original rules
  for(std::vector<rule_t>::iterator it = savedRules.begin();
      it != savedRules.end();
      ++it){
    ERule * er = dynamic_cast<ERule*>(it->get_ptr());
    add_rule(er->from_state(), er->from_stack(), er->to_state(),
        er->to_stack1(), er->to_stack2(), er->weight(), er->merge_fn());
  }
}

bool NWPDS::prestarUpdateFa(wfa::WFA& f)
{
  UpdateFaFunctor uff(f, var2ConstMap,dbg);
  f.for_each(uff);
  return uff.updated();
}

void NWPDS::prestar(wfa::WFA const & input, wfa::WFA & output)
{
  int i = 0;
  if(&output != &input)
    output = input;
  prestarSetupPds();
  if(dbg)
    output.print(*waliErr << "\n\n\n\n" << "Current Fa on iter[" << i << "]\n") << std::endl;
  do{
    EWPDS::prestar(output,output);
    if(dbg)
      output.print(*waliErr << "\n\n\n\n" << "Current Fa on iter[" << ++i << "]\n") << std::endl;
  }while(prestarUpdateFa(output));
  prestarRestorePds();
  prestarCleanUpFa(output);
}

void NWPDS::prestarCleanUpFa(wfa::WFA& output)
{
  Key2KeyMap old2NewMap;
  for(Key2KeyMap::iterator iter = var2ConstMap.begin();
      iter != var2ConstMap.end();
      ++iter)
    old2NewMap[iter->second] = iter->first;

  RemoveOldTrans rot(old2NewMap);
  output.for_each(rot);
}

NWPDS::UpdateFaFunctor::UpdateFaFunctor(wali::wfa::WFA& f, NWPDS::Key2KeyMap& n2om, bool d) :
  TransFunctor(),
  fa(f),
  new2OldMap(n2om),
  dbg(d)
{
  changed = false;
}

void NWPDS::UpdateFaFunctor::operator () (wali::wfa::ITrans* t)
{
  if(dbg){
    *waliErr << "[UpdateFaFunctor]: " << std::endl << "Processing:" << std::endl;
    t->print(*waliErr) << std::endl;
  }
  Key2KeyMap::const_iterator it = new2OldMap.find(t->stack());
  if(it != new2OldMap.end()){
    if(dbg){
      *waliErr << "Found old stack sym: " << wali::key2str(it->second) << std::endl;
    }
    //We want to track if anything was updated during the
    //functor operation.
    if(!changed){
      wfa::Trans oldt;
      fa.find(t->from(), it->second, t->to(), oldt);
      wali::sem_elem_t newwt = t->weight();
      wali::sem_elem_t oldwt = oldt.weight();
      if(newwt == NULL)
        changed = false;
      else if(oldwt == NULL)
        changed = true;
      else
        changed = !(newwt->equal(oldwt));
    }
    if(dbg){
      wfa::Trans oldt;
      fa.find(t->from(), it->second, t->to(), oldt);
      if(oldt.weight() != NULL)
        (oldt.weight())->print(*waliErr << "NEWTON_CONST_WT" << std::endl) 
          << std::endl;
      if(t->weight() != NULL)
        (t->weight())->print(*waliErr << "NEWTON_VAR_WT" 
            << std::endl) << std::endl;
      if(t->weight() != NULL && oldt.weight() != NULL)
        *waliErr << "The weights are the same (" 
          << (t->weight()->equal(oldt.weight())) << ")" << std::endl;
    }
    wali::wfa::ITrans * oldt;
    oldt = t->copy(t->from(),it->second, t->to());
    fa.addTrans(oldt);
  }
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

RemoveOldTrans::RemoveOldTrans(NWPDS::Key2KeyMap& m) : oldMap(m) {}

void RemoveOldTrans::operator() (wali::wfa::ITrans* t)
{
  if(oldMap.find(t->stack()) != oldMap.end())
    if(t->weight() != NULL)
      t->setWeight(t->weight()->zero());
}


