/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/nwa/NWA.hpp"
#include "wali/nwa/State.hpp"
#include "wali/nwa/Symbol.hpp"
#include "wali/Key.hpp"

int main()
{

  wali::nwa::NWA<wali::nwa::State<std::string>,wali::nwa::Symbol<std::string>> myNWA;
  
  wali::nwa::State<std::string> start = wali::nwa::State<std::string>("start");
  wali::nwa::State<std::string> state = wali::nwa::State<std::string>("state");
  wali::nwa::State<std::string> call = wali::nwa::State<std::string>("call");
  wali::nwa::State<std::string> entry = wali::nwa::State<std::string>("entry");
  wali::nwa::State<std::string> exit = wali::nwa::State<std::string>("exit");
  wali::nwa::State<std::string> ret = wali::nwa::State<std::string>("return");
  wali::nwa::State<std::string> end = wali::nwa::State<std::string>("end");
  
  wali::nwa::Symbol<std::string> epsilon = wali::nwa::Symbol<std::string>::getEpsilon();
  wali::nwa::Symbol<std::string> callInst = wali::nwa::Symbol<std::string>("callInst");
  wali::nwa::Symbol<std::string> intraInst = wali::nwa::Symbol<std::string>("intraInst");
  wali::nwa::Symbol<std::string> retInst = wali::nwa::Symbol<std::string>("retInst");
  
  wali::Triple<wali::nwa::State<std::string>,wali::nwa::Symbol<std::string>,
        wali::nwa::State<std::string>> callTrans = 
                wali::Triple<wali::nwa::State<std::string>,wali::nwa::Symbol<std::string>,
                        wali::nwa::State<std::string>>(call,callInst,entry);
  wali::Triple<wali::nwa::State<std::string>,wali::nwa::Symbol<std::string>,
        wali::nwa::State<std::string>> internalTrans = 
                wali::Triple<wali::nwa::State<std::string>,wali::nwa::Symbol<std::string>,
                        wali::nwa::State<std::string>>(entry,intraInst,exit);
  wali::Quad<wali::nwa::State<std::string>,wali::nwa::State<std::string>,
      wali::nwa::Symbol<std::string>,wali::nwa::State<std::string>> returnTrans = 
                wali::Quad<wali::nwa::State<std::string>,wali::nwa::State<std::string>,
                      wali::nwa::Symbol<std::string>,wali::nwa::State<std::string>>(exit,call,retInst,ret);
  
  myNWA.isAbsentAccept();
  myNWA.acceptAbsent();
  myNWA.rejectAbsent();
  
  myNWA.addInitialState(start);
  myNWA.addState(state);
  myNWA.addState(call);
  myNWA.addState(entry);
  myNWA.addState(exit);
  myNWA.addState(ret);
  myNWA.addFinalState(end);
  
  myNWA.getStates();
  myNWA.getInitialState();
  myNWA.getFinalStates();
  myNWA.addAllStates(wali::nwa::StateSet<wali::nwa::State<std::string>>());
  myNWA.addAllInitialStates(wali::nwa::StateSet<wali::nwa::State<std::string>>());
  myNWA.addAllFinalStates(wali::nwa::StateSet<wali::nwa::State<std::string>>(),false);
  myNWA.isState(state);
  myNWA.isInitialState(start);
  myNWA.isFinalState(end);
  myNWA.beginStates();
  myNWA.endStates();
  myNWA.beginInitialStates();
  myNWA.endInitialStates();
  myNWA.beginFinalStates();
  myNWA.endFinalStates();  
  myNWA.sizeStates();
  myNWA.sizeInitialStates();
  myNWA.sizeFinalStates();
  
  myNWA.getSymbols();
  myNWA.isSymbol(epsilon);
  myNWA.addSymbol(epsilon);
  myNWA.addAllSymbols(std::set<wali::nwa::Symbol<std::string>>());  
  myNWA.beginSymbols();
  myNWA.endSymbols();
  myNWA.sizeSymbols();
    
  myNWA.addInternalTrans(start,epsilon,call);
  myNWA.addCallTrans(call,callInst,entry);
  myNWA.addInternalTrans(entry,intraInst,state);
  myNWA.addInternalTrans(state,intraInst,exit);
  myNWA.addReturnTrans(exit,call,retInst,ret);
  myNWA.addInternalTrans(ret,epsilon,end);
  
  myNWA.getCallTrans();
  
  myNWA.print(std::cout);
  
  wali::nwa::WeightGen<wali::nwa::State<std::string>,wali::nwa::Symbol<std::string>> wg;  
  wali::nwa::NWA<wali::nwa::State<std::string>,wali::nwa::Symbol<std::string>>::NWAtoPDS(myNWA,wg);

  myNWA.removeState(state);
  myNWA.removeInitialState(start);
  myNWA.removeFinalState(end);
  myNWA.clearStates();
  myNWA.clearInitialStates();
  myNWA.clearFinalStates();
  
  myNWA.removeSymbol(epsilon);
  myNWA.clearSymbols();
  
  myNWA.clearTrans();

  return 0;
}