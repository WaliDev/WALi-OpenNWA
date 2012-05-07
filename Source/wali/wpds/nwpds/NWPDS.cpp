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

using namespace wali;
using namespace std;
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
  FWPDS(),
  dbg(b)
{}

NWPDS::NWPDS(ref_ptr<Wrapper> wrapper, bool b) :
  FWPDS(wrapper),
  dbg(b)
{}

NWPDS::NWPDS(const NWPDS& f) :
  FWPDS(f),
  dbg(f.dbg)
{}

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
      (*it).print(*waliErr);
    *waliErr << "Delta_2 rules: \n";
    for(std::set<Rule>::iterator it = wr.pushRules.begin();
        it != wr.pushRules.end();
        ++it)
      (*it).print(*waliErr);
    *waliErr << "Delta_0 rules: \n";
    for(std::set<Rule>::iterator it = wr.popRules.begin();
        it != wr.popRules.end();
        ++it)
      (*it).print(*waliErr);
  }
  var2ConstMap.clear();
  savedRules.clear();
  Delta2Rules dr;
  this->for_each(dr);
  for(std::vector<rule_t>::iterator it = dr.rules.begin();
      it != dr.rules.end();
      ++it){
    rule_t r = *it;
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
      (*it).print(*waliErr);
    *waliErr << "Delta_2 rules: \n";
    for(std::set<Rule>::iterator it = wr.pushRules.begin();
        it != wr.pushRules.end();
        ++it)
      (*it).print(*waliErr);
    *waliErr << "Delta_0 rules: \n";
    for(std::set<Rule>::iterator it = wr.popRules.begin();
        it != wr.popRules.end();
        ++it)
      (*it).print(*waliErr);
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
  UpdateFaFunctor uff(f, var2ConstMap);
  f.for_each(uff);
  return uff.updated();
}

void NWPDS::prestar(wfa::WFA const & input, wfa::WFA & output)
{
  output = input;
  prestarSetupPds();
  do{
    FWPDS::prestar(output,output);
  }while(prestarUpdateFa(output));
  prestarRestorePds();
}


NWPDS::UpdateFaFunctor::UpdateFaFunctor(wali::wfa::WFA& f, NWPDS::Key2KeyMap& n2om) :
  TransFunctor(),
  fa(f),
  new2OldMap(n2om)
{
  changed = false;
}

void NWPDS::UpdateFaFunctor::operator () (wali::wfa::ITrans* t)
{
  wfa::Trans old;
  Key2KeyMap::const_iterator it = new2OldMap.find(t->stack());
  if(it != new2OldMap.end()){
    //We want to track if anything was updated during the
    //functor operation.
    if(!changed){
      wfa::Trans oldt;
      fa.find(t->from(), it->second, t->to(), oldt);
      changed = (t->weight() == oldt.weight());
    }
    fa.addTrans(t->from(), it->second, t->to(), t->weight());
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




