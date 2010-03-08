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

class ReachGen : public wali::nwa::WeightGen<>
{
  public:
    enum Kind {INTRA, CALL_TO_ENTRY, EXIT_TO_RET, CALL_TO_RET};
    typedef wali::Key St;
    typedef wali::Key Sym;
     
    //
    // Methods
    //
      
  public:
    //Constructors and Destructor
    ReachGen( ) { }
        
    sem_elem_t getOne( )
    {
      wali::ref_ptr<Reach> r = new Reach(true);
      return r->one();
    }

    sem_elem_t getWeight( const St & src, const Sym & inst, Kind k, const St & tgt )
    {
      return getOne();
    }
       
    sem_elem_t getWildWeight( const St & src, const St & tgt )
    { //TODO: want the default here to be bottom
      return getOne();
    }
};

int main()
{

  wali::ref_ptr<wali::nwa::NWA<>> myNWA;
  
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

  myNWA->getPredecessorNames(state);
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
  
  ReachGen wg;  
  myNWA->NWAtoPDSreturns(wg);
  myNWA->NWAtoPDScalls(wg);
  myNWA->NWAtoBackwardsPDSreturns(wg);
  myNWA->NWAtoBackwardsPDScalls(wg);
  //myNWA->plusWPDS(wpdsBase);
  //myNWA->PDStoNWA(wpdsBase);

  wali::nwa::NWA<> otherNWA1 = wali::nwa::NWA<>(*myNWA);
  wali::nwa::NWA<> otherNWA2 = otherNWA1;
  wali::ref_ptr<wali::nwa::NWA<>> otherNWA = myNWA;
  wali::ref_ptr<wali::nwa::NWA<>> intersectNWA;
  intersectNWA->intersect(myNWA,otherNWA);
  wali::ref_ptr<wali::nwa::NWA<>> union_NWA;
  union_NWA->unionNWA(myNWA,otherNWA);
  wali::ref_ptr<wali::nwa::NWA<>> concatNWA;
  concatNWA->concat(myNWA,otherNWA);
  wali::ref_ptr<wali::nwa::NWA<>> reverseNWA;
  reverseNWA->reverse(myNWA);
  wali::ref_ptr<wali::nwa::NWA<>> starNWA;
  starNWA->star(myNWA);
  wali::ref_ptr<wali::nwa::NWA<>> complementNWA;
  complementNWA->complement(myNWA);
  wali::ref_ptr<wali::nwa::NWA<>> determinizeNWA;
  determinizeNWA->determinize(myNWA);
  determinizeNWA->isDeterministic();
  
  myNWA->isEmpty();
  myNWA->isMember(wali::nws::NWS());
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
