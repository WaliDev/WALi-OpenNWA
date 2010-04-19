/**
 * @author Amanda Burton
 */
#define NWA_TEST

#ifdef NWA_TEST
// ::wali
#include "wali/nwa/NWA.hpp"
#include "wali/nwa/State.hpp"
#include "wali/nwa/Symbol.hpp"
#include "wali/Key.hpp"
#include "wali/wfa/epr/FunctionalWeight.hpp"
#include "wali/Reach.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/nwa/ClientInfo.hpp"


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

int main()
{
  wali::Key stuck = wali::getKey("stuck");
  wali::Key stuck2 = wali::getKey("stuck2");
  wali::ref_ptr<wali::nwa::NWA<>> myNWA = wali::ref_ptr<wali::nwa::NWA<>>();
  myNWA->setStuckState(stuck);
  
  wali::Key start = wali::getKey("start");
  wali::Key state = wali::getKey("state");
  wali::Key call = wali::getKey("call");
  wali::Key call2 = wali::getKey("call2");
  wali::Key  entry = wali::getKey("entry");
  wali::Key  exit = wali::getKey("exit");
  wali::Key  ret = wali::getKey("return");
  wali::Key  end = wali::getKey("end");
  
  wali::Key epsilon = wali::nwa::SymbolSet::getEpsilon();
  wali::Key callInst = wali::getKey("callInst");
  wali::Key intraInst = wali::getKey("intraInst");
  wali::Key retInst = wali::getKey("retInst");
  wali::Key sym;
  
  wali::Triple<wali::Key,wali::Key,wali::Key > callTrans = 
                wali::Triple<wali::Key ,wali::Key,wali::Key >(call,callInst,entry);
  wali::Triple<wali::Key ,wali::Key,
        wali::Key > internalTrans = 
                wali::Triple<wali::Key ,wali::Key,
                        wali::Key >(entry,intraInst,exit);
  wali::Quad<wali::Key ,wali::Key ,
      wali::Key,wali::Key > returnTrans = 
                wali::Quad<wali::Key ,wali::Key ,
                      wali::Key,wali::Key >(exit,call,retInst,ret);
  
  myNWA->addInitialState(start);
  myNWA->addState(state);
  myNWA->addState(call);
  myNWA->addState(entry);
  myNWA->addState(exit);
  myNWA->addState(ret);
  myNWA->addFinalState(end);
  
  myNWA->getStates();
  myNWA->get_states();
  myNWA->getInitialStates();
  myNWA->getFinalStates();

  myNWA->isState(state);
  myNWA->is_nwa_state(state);
  myNWA->isInitialState(start);
  myNWA->isFinalState(end);
  myNWA->sizeStates();
  myNWA->num_nwa_states();
  myNWA->sizeInitialStates();
  myNWA->sizeFinalStates();
  
  wali::nwa::NWA<>::ClientInfoRefPtr info = myNWA->getClientInfo(state);
  myNWA->setClientInfo(state,info);

  std::set<wali::Key> preds = std::set<wali::Key>();
  myNWA->getPredecessors(state,preds);
  wali::Key state1 = wali::getKey("state1");
  myNWA->duplicateStateOutgoing(state,state1);
  wali::Key state2 = wali::getKey("state2");
  myNWA->duplicateState(state,state2);
  
  myNWA->getSymbols();
  myNWA->isSymbol(epsilon);
  myNWA->addSymbol(epsilon);  
  myNWA->sizeSymbols();
    
  myNWA->addInternalTrans(start,epsilon,call);
  myNWA->addInternalTrans(wali::getKey("start"),epsilon,wali::getKey("call"));
  myNWA->addCallTrans(callTrans);
  myNWA->addCallTrans(call,callInst,entry);
  myNWA->addCallTrans(wali::getKey("call"),callInst,wali::getKey("entry"));
  myNWA->addInternalTrans(internalTrans);
  myNWA->addInternalTrans(entry,intraInst,state);
  myNWA->addInternalTrans(wali::getKey("entry"),intraInst,wali::getKey("state"));
  myNWA->addInternalTrans(state,intraInst,exit);
  myNWA->addInternalTrans(wali::getKey("state"),intraInst,wali::getKey("exit"));
  myNWA->addReturnTrans(returnTrans);
  myNWA->addReturnTrans(exit,call,retInst,ret);
  myNWA->addReturnTrans(wali::getKey("exit"),wali::getKey("call"),retInst,wali::getKey("ret"));
  myNWA->addInternalTrans(ret,epsilon,end);
  myNWA->addInternalTrans(wali::getKey("ret"),epsilon,wali::getKey("end"));
  
  myNWA->getSymbol(wali::getKey("call"),wali::getKey("entry"),sym);
  myNWA->findTrans(start,epsilon,call);
  myNWA->findTrans(start,epsilon,state);
  myNWA->getReturnSites(wali::getKey("call"));

  myNWA->getEntries(call);
  myNWA->getTargets(state);
  myNWA->getReturnSites(call);
  myNWA->getReturns(exit,call);
  
  myNWA->print(std::cout);
  
  wali::nwa::ReachGen<> wg;  
  myNWA->NWAtoPDSreturns(wg);
  myNWA->NWAtoPDScalls(wg);
  myNWA->NWAtoBackwardsPDSreturns(wg);
  myNWA->NWAtoBackwardsPDScalls(wg);
  wali::wpds::WPDS wpdsBase;
  myNWA->plusWPDS(wpdsBase);
  //myNWA->PDStoNWA(wpdsBase);

  wali::nwa::NWA<> otherNWA1 = wali::nwa::NWA<>(*myNWA);
  wali::nwa::NWA<> otherNWA2 = otherNWA1;
  wali::ref_ptr<wali::nwa::NWA<>> otherNWA = myNWA;
  wali::ref_ptr<wali::nwa::NWA<>> intersectNWA = wali::ref_ptr<wali::nwa::NWA<>>();
  intersectNWA->setStuckState(stuck2);
  intersectNWA->intersect(myNWA,otherNWA);
  wali::ref_ptr<wali::nwa::NWA<>> union_NWA = wali::ref_ptr<wali::nwa::NWA<>>();
  union_NWA->setStuckState(stuck);
  union_NWA->unionNWA(myNWA,intersectNWA);
  wali::ref_ptr<wali::nwa::NWA<>> concatNWA = wali::ref_ptr<wali::nwa::NWA<>>();
  concatNWA->setStuckState(stuck);
  concatNWA->concat(myNWA,otherNWA);
  wali::ref_ptr<wali::nwa::NWA<>> reverseNWA = wali::ref_ptr<wali::nwa::NWA<>>();
  reverseNWA->setStuckState(stuck);
  reverseNWA->reverse(myNWA);
  wali::ref_ptr<wali::nwa::NWA<>> starNWA = wali::ref_ptr<wali::nwa::NWA<>>();
  starNWA->setStuckState(stuck);
  starNWA->star(myNWA);
  wali::ref_ptr<wali::nwa::NWA<>> complementNWA = wali::ref_ptr<wali::nwa::NWA<>>();
  complementNWA->setStuckState(stuck);
  complementNWA->complement(myNWA);
  wali::ref_ptr<wali::nwa::NWA<>> determinizeNWA = wali::ref_ptr<wali::nwa::NWA<>>();
  determinizeNWA->setStuckState(stuck);
  determinizeNWA->determinize(myNWA);
  determinizeNWA->isDeterministic();
  
  myNWA->isEmpty();
  wali::nwa::NWA<>::isMember(wali::nws::NWS(),myNWA);
  wali::nwa::NWA<>::inclusion(myNWA,complementNWA);
  wali::nwa::NWA<>::equal(otherNWA,myNWA);
  myNWA->prestar(wali::wfa::WFA(),wg);
  myNWA->prestar(wali::wfa::WFA(),wali::wfa::WFA(),wg);
  myNWA->poststar(wali::wfa::WFA(),wg);
  myNWA->poststar(wali::wfa::WFA(),wali::wfa::WFA(),wg);

  myNWA->print(std::cout);
  myNWA->print_dot(std::cout,"dotfile");
  bool equal = myNWA->operator==(*otherNWA);

  myNWA->sizeStates();
  myNWA->sizeTrans();
  myNWA->count_rules();
  myNWA->sizeCallTrans();
  myNWA->sizeInternalTrans();
  myNWA->sizeReturnTrans();

  myNWA->removeInternalTrans(start,epsilon,call);
  myNWA->removeInternalTrans(wali::getKey("start"),epsilon,wali::getKey("call"));
  myNWA->removeCallTrans(callTrans);
  myNWA->removeCallTrans(call,callInst,entry);
  myNWA->removeCallTrans(wali::getKey("call"),callInst,wali::getKey("entry"));
  myNWA->removeInternalTrans(internalTrans);
  myNWA->removeInternalTrans(entry,intraInst,state);
  myNWA->removeInternalTrans(wali::getKey("entry"),intraInst,wali::getKey("state"));
  myNWA->removeInternalTrans(state,intraInst,exit);
  myNWA->removeInternalTrans(wali::getKey("state"),intraInst,wali::getKey("exit"));
  myNWA->removeReturnTrans(returnTrans);
  myNWA->removeReturnTrans(exit,call,retInst,ret);
  myNWA->removeReturnTrans(wali::getKey("exit"),wali::getKey("call"),retInst,wali::getKey("ret"));
  myNWA->removeInternalTrans(ret,epsilon,end);
  myNWA->removeInternalTrans(wali::getKey("ret"),epsilon,wali::getKey("end"));
  myNWA->removeReturnTrans(wali::getKey("exit"),retInst,wali::getKey("ret"));
  myNWA->removeReturnTrans(exit,retInst,ret);

  myNWA->clearTrans();

  myNWA->removeState(state);
  myNWA->removeInitialState(start);
  myNWA->removeFinalState(end);
  myNWA->clearStates();
  myNWA->clearInitialStates();
  myNWA->clearFinalStates();
  
  myNWA->removeSymbol(epsilon);
  myNWA->clearSymbols();
  
  
  otherNWA->clear();
  return 0;
}
#endif
