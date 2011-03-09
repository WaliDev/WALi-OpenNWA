/**
 * @author Amanda Burton
 */
//#define NWA_TEST


// ::wali
#include "wali/nwa/NWA.hpp"
#include "wali/nwa/OpenFstInterop.hpp"
#include "wali/Key.hpp"
#include "wali/wfa/epr/FunctionalWeight.hpp"
#include "wali/Reach.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/nwa/ClientInfo.hpp"

using wali::getKey;
using wali::Key;
using wali::nwa::NWA;
using wali::nwa::NWARefPtr;
using wali::nwa::fst_to_nwa;

NWARefPtr
buildNwa_Alur_JACM_fig6(std::string const & name_prefix)
{
    // "Suppose Sigma = {0, 1}. Consider the language L of nested words 'n'
    //  such that every subword starting at a call and ending at a matched
    //  return contains an even number of 0-labeled positions.
    //
    //  That is, whenever '1 <= i <= j <= l' and 'i -> j' is a matched
    //  call/return pair, then
    //    | {k | i <= k <= j  and  a_k = 0} |
    //  is even."
    //
    // (Clarification: the whole tagged alphabet is { (0, )0, 0, (1, )1, 1 }
    //
    //                             0
    //               ___           (0 push p0      ___
    //         1    /   \          1) pop p1      /   \  1
    // (1 push p0  |     V         0) pop p      V     | (0 push p1 *
    // )1 pop p0   |     q0  ---------------->  q1     | )0 pop p1 *
    // )1 pop p     \___/    <----------------    \___/  )1 pop p
    //                             0
    //                             (0 push p1
    //                             )0 pop p0
    //                             )0 pop p
    //
    // BOTH q0 AND q1 ARE ACCEPTING
    //
    // * These transitions are marked as '(0 push p1' and ')0 pop p1' in the
    //   figure; I think this is incorrect.
    //
    // "The state q0 means that the number of 0-labeled positions since the
    //  last unmatched call is even, and state q1 means this number is
    //  odd. Upon a call, this information is porpagated along the nesting
    //  edge [in the heirarchical state p#], while the new linear state
    //  reflects the parity count starting at this new call."

    assert(false);
    return NULL;
}



NWARefPtr
buildNwa_1(std::string const & name_prefix = "",
           std::string const & stuck = "[stuck]")
{
    // This is inspired by buildNwa_Alur_JACM_fig6.
    //
    // I want my alphabet to be
    //  - internals  0, 1
    //  - calls      (
    //  -returns     )
    //
    // Between pairs of (, ), there should be an even number of 0s.
    //
    //               ___                           ___
    //          1   /   \        0                /   \
    //  ( push q0  |     V       ) pop q1        V     |  1          
    //  ) pop q0   |     q0  ---------------->  q1     |
    //              \___/    <----------------    \___/ 
    //                           0
    //                           ( push q1  
    //
    // Error transitions are any ) from q1. q0 is accepting.
    
    
    NWARefPtr nwa = new NWA(getKey(stuck));

    Key q0 = getKey(name_prefix + "__q0");
    Key q1 = getKey(name_prefix + "__q1");
    Key zero = getKey("0");
    Key one = getKey("1");
    Key call = getKey("(");
    Key ret = getKey(")");

    nwa->addInitialState(q0);
    nwa->addFinalState(q0);

    nwa->addInternalTrans(q0, zero, q1);
    nwa->addInternalTrans(q1, zero, q0);
    nwa->addInternalTrans(q0, one, q0);
    nwa->addInternalTrans(q1, one, q1);

    nwa->addCallTrans(q0, call, q0);
    nwa->addCallTrans(q1, call, q0);

    nwa->addReturnTrans(q0, q0, ret, q0);
    nwa->addReturnTrans(q0, q1, ret, q1);

    return nwa;
}


NWARefPtr
build_internal_nwa(std::string const & stuck = "[stuck]")
{
    // This is a FSM expressed as an NWA. It accepts strings with an even
    // number of 0s and an odd number of 1s. (EO accepts.)
    //
    //             0
    // --> EE <----------> OE
    //     ^                ^
    //     |                |
    //   1 |                | 1
    //     |                |
    //     V                V
    //    (EO)<----------> OO
    //             0

    NWARefPtr nwa = new NWA(getKey(stuck));

    Key ee = getKey("ee");
    Key oe = getKey("oe");
    Key eo = getKey("eo");
    Key oo = getKey("oo");

    Key zero = getKey("0");
    Key one = getKey("1");

    nwa->addInitialState(ee);
    nwa->addFinalState(eo);

    // Top horizontal arrows
    nwa->addInternalTrans(ee, zero, oe);
    nwa->addInternalTrans(oe, zero, ee);

    // Bottom horizonal arrows
    nwa->addInternalTrans(eo, zero, oo);
    nwa->addInternalTrans(oo, zero, eo);

    // Left vertical arrows
    nwa->addInternalTrans(ee, one, eo);
    nwa->addInternalTrans(eo, one, ee);

    // Right vertical arrows
    nwa->addInternalTrans(oe, one, oo);
    nwa->addInternalTrans(oo, one, oe);

    return nwa;
}


int main()
{
    NWARefPtr nwa1 = buildNwa_1("#");
    NWARefPtr nwa2 = buildNwa_1("@");

    nwa1->combineWith(nwa2);

    //nwa1->print_dot(std::cout, "thingy");


    NWARefPtr eo = build_internal_nwa();
    fst::StdVectorFst fst = internal_only_nwa_to_fst(eo);
    NWARefPtr eo_converted = fst_to_nwa(fst);

    std::cout << "NWA eo:\n";
    eo->print(std::cout);

    std::cout << "\nNWA eo converted to an FST and back:\n";
    eo_converted->print(std::cout);

    assert (eo == eo_converted);
    
#if 0 // this works if you want to uncomment it
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
#endif

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

#if 0 // can uncomment this if you uncomment stuff above
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

#endif
}
