/**
 * @author Amanda Burton
 */
//#define NWA_TEST


// ::wali
#include "wali/nwa/NWA.hpp"
#include "wali/Key.hpp"
#include "wali/wfa/epr/FunctionalWeight.hpp"
#include "wali/Reach.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/nwa/ClientInfo.hpp"

int main()
{
  wali::Key stuck = wali::getKey("stuck");
  wali::Key stuck2 = wali::getKey("stuck2");
  wali::ref_ptr<wali::nwa::NWA> myNWA = new wali::nwa::NWA();
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
  
  wali::nwa::NWA::ClientInfoRefPtr info = myNWA->getClientInfo(state);
  myNWA->setClientInfo(state,info);

  std::set<wali::Key> preds = std::set<wali::Key>();
  myNWA->getPredecessors(state,preds);
  myNWA->getPredecessors(epsilon,state,preds);
  myNWA->getSuccessors(state,preds);
  myNWA->getSuccessors(epsilon,state,preds);
  myNWA->getCallPredecessors(ret,preds);
  myNWA->getCallPredecessors(epsilon,ret,preds);
  myNWA->getCallSuccessors(call,preds);
  myNWA->getCallSuccessors(call,epsilon,preds);
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

  myNWA->getSymbols(start,call);
  myNWA->getSymbolsFrom(start);
  myNWA->getSymbolsTo(call);
  myNWA->getPredecessors(call);
  myNWA->getPredecessors(epsilon,call);
  myNWA->getSuccessors(call);
  myNWA->getSuccessors(epsilon,call);
  myNWA->getCallRetSymbols(call,ret);
  myNWA->getCallRetSymbolsFrom(call);
  myNWA->getCallRetSymbolsTo(ret);
  myNWA->getCallPredecessors(ret);
  myNWA->getCallPredecessors(epsilon,ret);
  myNWA->getCallSuccessors(call);
  myNWA->getCallSuccessors(call,epsilon);

  myNWA->getCallSites_Sym(epsilon);
  myNWA->getCallSites(epsilon,entry);
  myNWA->getCallSites(entry);
  myNWA->getCallSites();
  myNWA->getCallSym();
  myNWA->getCallSym(call,entry);
  myNWA->getCallSym_Call(call);
  myNWA->getCallSym_Entry(entry);
  myNWA->getEntries_Sym(epsilon);
  myNWA->getEntries(call,epsilon);
  myNWA->getEntries(call);
  myNWA->getEntries();

  myNWA->getSources_Sym(epsilon);
  myNWA->getSources(epsilon,state);
  myNWA->getSources(state);
  myNWA->getSources();
  myNWA->getInternalSym();
  myNWA->getInternalSym(entry,state);
  myNWA->getInternalSym_Source(entry);
  myNWA->getInternalSym_Target(state);
  myNWA->getTargets_Sym(epsilon);
  myNWA->getTargets(entry,epsilon);
  myNWA->getTargets(entry);
  myNWA->getTargets();

  myNWA->getExits_Sym(epsilon);
  myNWA->getExits(call,epsilon,ret);
  myNWA->getExits(call,ret);
  myNWA->getExits();
  myNWA->getExits_Call(call,epsilon);
  myNWA->getExits_Call(call);
  myNWA->getExits_Ret(epsilon,ret);
  myNWA->getExits_Ret(ret);
  myNWA->getCalls_Sym(epsilon);
  myNWA->getCalls(exit,epsilon,ret);
  myNWA->getCalls(exit,ret);
  myNWA->getCalls();
  myNWA->getCalls_Exit(exit,epsilon);
  myNWA->getCalls_Exit(exit);
  myNWA->getCalls_Ret(epsilon,ret);
  myNWA->getCalls_Ret(ret);
  myNWA->getReturnSym();
  myNWA->getReturnSym(exit,call,ret);
  myNWA->getReturnSym_Exit(exit);
  myNWA->getReturnSym_Call(call);
  myNWA->getReturnSym_Ret(ret);
  myNWA->getReturnSym_ExitCall(exit,call);
  myNWA->getReturnSym_ExitRet(exit,ret);
  myNWA->getReturnSym_CallRet(call,ret);
  myNWA->getReturns_Sym(epsilon);
  myNWA->getReturns(exit,call,epsilon);
  myNWA->getReturns(exit,call);
  myNWA->getReturns();
  myNWA->getReturns_Exit(exit,epsilon);
  myNWA->getReturns_Exit(exit);
  myNWA->getReturns_Call(call,epsilon);
  myNWA->getReturns_Call(call);
  myNWA->getReturnSites(call);

  
  myNWA->print(std::cout);

#if 0
  wali::nwa::ReachGen wg;  
  myNWA->NWAtoPDSreturns(wg);
  myNWA->NWAtoPDScalls(wg);
  myNWA->NWAtoBackwardsPDSreturns(wg);
  myNWA->NWAtoBackwardsPDScalls(wg);
  wali::nwa::NWA::NWAtoPDSreturns(myNWA,wg);
  wali::nwa::NWA::NWAtoPDScalls(myNWA,wg);
  wali::nwa::NWA::NWAtoBackwardsPDSreturns(myNWA,wg);
  wali::nwa::NWA::NWAtoBackwardsPDScalls(myNWA,wg);

  wali::wpds::WPDS wpdsBase;
  myNWA->plusWPDS(wpdsBase);
  wali::nwa::NWA::plusWPDS(wpdsBase,myNWA);
  myNWA->PDStoNWA(wpdsBase);
  wali::nwa::NWA::PDStoNWA(wpdsBase,stuck);

  wali::nwa::NWA otherNWA1(*myNWA);
  wali::nwa::NWA otherNWA2 = otherNWA1;
  wali::ref_ptr<wali::nwa::NWA> otherNWA = myNWA;
  wali::ref_ptr<wali::nwa::NWA> intersectNWA = wali::ref_ptr<wali::nwa::NWA>();
  intersectNWA->setStuckState(stuck2);
  intersectNWA->intersect(myNWA,otherNWA);
  wali::ref_ptr<wali::nwa::NWA> union_NWA = wali::ref_ptr<wali::nwa::NWA>();
  union_NWA->setStuckState(stuck);
  union_NWA->unionNWA(myNWA,intersectNWA);
  wali::ref_ptr<wali::nwa::NWA> concatNWA = wali::ref_ptr<wali::nwa::NWA>();
  concatNWA->setStuckState(stuck);
  concatNWA->concat(myNWA,otherNWA);
  wali::ref_ptr<wali::nwa::NWA> reverseNWA = wali::ref_ptr<wali::nwa::NWA>();
  reverseNWA->setStuckState(stuck);
  reverseNWA->reverse(myNWA);
  wali::ref_ptr<wali::nwa::NWA> starNWA = wali::ref_ptr<wali::nwa::NWA>();
  starNWA->setStuckState(stuck);
  starNWA->star(myNWA);
  wali::ref_ptr<wali::nwa::NWA> complementNWA = wali::ref_ptr<wali::nwa::NWA>();
  complementNWA->setStuckState(stuck);
  complementNWA->complement(myNWA);
  wali::ref_ptr<wali::nwa::NWA> determinizeNWA = wali::ref_ptr<wali::nwa::NWA>();
  determinizeNWA->setStuckState(stuck);
  determinizeNWA->determinize(myNWA);
  determinizeNWA->isDeterministic();

  wali::nwa::NWA::unionNWA(myNWA,intersectNWA,stuck);
  wali::nwa::NWA::intersect(myNWA,otherNWA,stuck);
  wali::nwa::NWA::concat(myNWA,otherNWA,stuck);
  wali::nwa::NWA::reverse(myNWA,stuck);
  wali::nwa::NWA::star(myNWA,stuck);
  wali::nwa::NWA::complement(myNWA,stuck);
  wali::nwa::NWA::determinize(myNWA,stuck);

  wali::wfa::WFA wfa1, wfa2a, wfa2b, wfa3, wfa4a, wfa4b;
  myNWA->isEmpty();
  wali::nwa::NWA::isMember(wali::nws::NWS(),myNWA);
  wali::nwa::NWA::inclusion(myNWA,complementNWA);
  wali::nwa::NWA::equal(otherNWA,myNWA);
  myNWA->prestar(wfa1, wg);
  myNWA->prestar(wfa2a, wfa2b, wg);
  myNWA->poststar(wfa3, wg);
  myNWA->poststar(wfa4a, wfa4b, wg);

  myNWA->print(std::cout);
  myNWA->print_dot(std::cout,"dotfile");
  bool equal = myNWA->operator==(*otherNWA);
#endif
  
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
  
  return 0;
}
