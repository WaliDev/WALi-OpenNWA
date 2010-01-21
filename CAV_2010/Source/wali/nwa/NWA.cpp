/**
 * @author Amanda Burton
 */
//#define NWA_TEST

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

template<typename St, typename Sym>
class ReachGen : public wali::nwa::WeightGen<St,Sym>
{
  public:
    enum Kind {INTRA, CALL_TO_ENTRY, EXIT_TO_RET, CALL_TO_RET};
     
    //
    // Methods
    //
      
  public:
    //Constructors and Destructor
    ReachGen( ) { }
        
    sem_elem_t getOne( )
    {
      wali::ref_ptr<Reach> r;
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
#ifndef KEY
  wali::nwa::NWA<wali::nwa::State<std::string>,std::string,wali::nwa::Symbol<STR>> myNWA = wali::nwa::NWA<wali::nwa::State<std::string>,std::string,wali::nwa::Symbol<STR>>( );
  
  wali::nwa::State<std::string> start = wali::nwa::State<std::string>("start");
  wali::nwa::State<std::string> state = wali::nwa::State<std::string>("state");
  wali::nwa::State<std::string> call = wali::nwa::State<std::string>("call");
  wali::nwa::State<std::string> call2 = wali::nwa::State<std::string>("call2");
  wali::nwa::State<std::string>  entry = wali::nwa::State<std::string>("entry");
  wali::nwa::State<std::string>  exit = wali::nwa::State<std::string>("exit");
  wali::nwa::State<std::string>  ret = wali::nwa::State<std::string>("return");
  wali::nwa::State<std::string>  end = wali::nwa::State<std::string>("end");
  
  wali::nwa::Symbol<STR> epsilon = wali::nwa::Symbol<STR>::getEpsilon();
  wali::nwa::Symbol<STR> callInst = wali::nwa::Symbol<STR>("callInst");
  wali::nwa::Symbol<STR> intraInst = wali::nwa::Symbol<STR>("intraInst");
  wali::nwa::Symbol<STR> retInst = wali::nwa::Symbol<STR>("retInst");
  wali::nwa::Symbol<STR> sym;
  
  wali::Triple<std::string,wali::nwa::Symbol<STR>,std::string > callTrans = 
                wali::Triple<std::string ,wali::nwa::Symbol<STR>,std::string >("call",callInst,"entry");
  wali::Triple<std::string ,wali::nwa::Symbol<STR>,
        std::string > internalTrans = 
                wali::Triple<std::string ,wali::nwa::Symbol<STR>,
                        std::string >("entry",intraInst,"exit");
  wali::Quad<std::string,std::string,
      wali::nwa::Symbol<STR>,std::string > returnTrans = 
                wali::Quad<std::string,std::string,
                      wali::nwa::Symbol<STR>,std::string >("exit","call",retInst,"ret");
  
  myNWA.isAbsentAccept();
  myNWA.acceptAbsent();
  myNWA.rejectAbsent();
  
  myNWA.addInitialState(&start);
  myNWA.addState(&state);
  myNWA.addState(&call);
  myNWA.addState(&entry);
  myNWA.addState(&exit);
  myNWA.addState(&ret);
  myNWA.addFinalState(&end);
  
  myNWA.getState("call");
  myNWA.getInitialState();
  myNWA.getFinalStates();
  myNWA.getStates();
  myNWA.getStateNames();
  myNWA.getPredecessorNames( "ret" );
  myNWA.duplicateState("call","call2");

  assert(start.isInitial());
  assert(!end.isInitial());

  myNWA.setClientInfo( "state", wali::ref_ptr<wali::nwa::ClientInfo>() );
  wali::ref_ptr<wali::nwa::ClientInfo> info = myNWA.getClientInfo( "state" );
  
  //myNWA.addAllStates(wali::nwa::StateSet<wali::nwa::State ,std::string>());
  //myNWA.addAllInitialStates(wali::nwa::StateSet<wali::nwa::State ,std::string>());
  //myNWA.addAllFinalStates(wali::nwa::StateSet<wali::nwa::State ,std::string>(),false);
  myNWA.isState(&state);
  myNWA.isInitialState(&start);
  myNWA.isFinalState(&end);
  //myNWA.beginStates();
  //myNWA.endStates();
  //myNWA.beginInitialStates();
  //myNWA.endInitialStates();
  //myNWA.beginFinalStates();
  //myNWA.endFinalStates();  
  myNWA.sizeStates();
  myNWA.sizeInitialStates();
  myNWA.sizeFinalStates();
  
  //myNWA.getSymbols();
  myNWA.isSymbol(epsilon);
  myNWA.addSymbol(epsilon);
 //myNWA.addAllSymbols(std::set<wali::nwa::Symbol *>());  
  //myNWA.beginSymbols();
  //myNWA.endSymbols();
  myNWA.sizeSymbols();
    
  myNWA.addInternalTrans(&start,epsilon,&call);
  myNWA.addInternalTrans("start",epsilon,"call");
  myNWA.addCallTrans(callTrans);
  myNWA.addCallTrans(&call,callInst,&entry);
  myNWA.addCallTrans("call",callInst,"entry");
  myNWA.addInternalTrans(internalTrans);
  myNWA.addInternalTrans(&entry,intraInst,&state);
  myNWA.addInternalTrans("entry",intraInst,"state");
  myNWA.addInternalTrans(&state,intraInst,&exit);
  myNWA.addInternalTrans("state",intraInst,"exit");
  myNWA.addReturnTrans(returnTrans);
  myNWA.addReturnTrans(&exit,&call,retInst,&ret);
  myNWA.addReturnTrans("exit","call",retInst,"ret");
  myNWA.addInternalTrans(&ret,epsilon,&end);
  myNWA.addInternalTrans("ret",epsilon,"end");
  //myNWA.beginCallTrans();
  //myNWA.endCallTrans();
  //myNWA.beginInternalTrans();
  //myNWA.endInternalTrans();
  //myNWA.beginReturnTrans();
  //myNWA.endReturnTrans();
  
  myNWA.getSymbol("call","entry",sym);
  myNWA.findTrans("call",sym,"entry");
  myNWA.getReturnSites("call");

  myNWA.getTargets("state");
  myNWA.getEntries("call");
  myNWA.getReturns("exit","call");
  
  myNWA.print(std::cout);
  
  ReachGen<wali::nwa::State<std::string> ,wali::nwa::Symbol<STR>> wg;  
  myNWA.NWAtoPDS(wg);
  myNWA.NWAtoBackwardsPDS(wg);

  wali::nwa::NWA<wali::nwa::State<std::string>,std::string,wali::nwa::Symbol<STR>> otherNWA = wali::nwa::NWA<wali::nwa::State<std::string>,std::string,wali::nwa::Symbol<STR>>(myNWA);
  wali::nwa::NWA<wali::nwa::State<std::string>,std::string,wali::nwa::Symbol<STR>> intersectNWA;
  myNWA.intersect(otherNWA,intersectNWA);
  otherNWA = intersectNWA;
  otherNWA.isDeterministic();
  otherNWA.print(std::cout);
  otherNWA.print_dot(std::cout,"NWAdotfile");

  bool equal = myNWA.operator==(otherNWA);
  //myNWA.numStates();
  //myNWA.numTrans();

  myNWA.removeInternalTrans(&start,epsilon,&call);
  myNWA.removeInternalTrans("start",epsilon,"call");
  myNWA.removeCallTrans(callTrans);
  myNWA.removeCallTrans(&call,callInst,&entry);
  myNWA.removeCallTrans("call",callInst,"entry");
  myNWA.removeInternalTrans(internalTrans);
  myNWA.removeInternalTrans(&entry,intraInst,&state);
  myNWA.removeInternalTrans("entry",intraInst,"state");
  myNWA.removeInternalTrans(&state,intraInst,&exit);
  myNWA.removeInternalTrans("state",intraInst,"exit");
  myNWA.removeReturnTrans(returnTrans);
  myNWA.removeReturnTrans(&exit,&call,retInst,&ret);
  myNWA.removeReturnTrans("exit","call",retInst,"ret");
  myNWA.removeInternalTrans(&ret,epsilon,&end);
  myNWA.removeInternalTrans("ret",epsilon,"end");
  myNWA.removeReturnTrans("exit",retInst,"ret");
  myNWA.removeReturnTrans(&exit,retInst,&ret);

  myNWA.removeState(&state);
  myNWA.removeInitialState(&start);
  myNWA.removeFinalState(&end);
  myNWA.clearStates();
  myNWA.clearInitialStates();
  myNWA.clearFinalStates();
  
  myNWA.removeSymbol(epsilon);
  myNWA.clearSymbols();
  
  myNWA.clearTrans();
  otherNWA.~NWA();
#else
  wali::nwa::NWA<> myNWA;
  
  wali::nwa::State<> start = wali::nwa::State<>(wali::getKey("start"));
  wali::nwa::State<> state = wali::nwa::State<>(wali::getKey("state"));
  wali::nwa::State<> call = wali::nwa::State<>(wali::getKey("call"));
  wali::nwa::State<> call2 = wali::nwa::State<>(wali::getKey("call2"));
  wali::nwa::State<>  entry = wali::nwa::State<>(wali::getKey("entry"));
  wali::nwa::State<>  exit = wali::nwa::State<>(wali::getKey("exit"));
  wali::nwa::State<>  ret = wali::nwa::State<>(wali::getKey("return"));
  wali::nwa::State<>  end = wali::nwa::State<>(wali::getKey("end"));
  
  wali::nwa::Symbol<> epsilon = wali::nwa::Symbol<>::getEpsilon();
  wali::nwa::Symbol<> callInst = wali::nwa::Symbol<>(wali::getKey("callInst"));
  wali::nwa::Symbol<> intraInst = wali::nwa::Symbol<>(wali::getKey("intraInst"));
  wali::nwa::Symbol<> retInst = wali::nwa::Symbol<>(wali::getKey("retInst"));
  wali::nwa::Symbol<> sym;
  
  wali::Triple<wali::nwa::State<>,wali::nwa::Symbol<>,wali::nwa::State<> > callTrans = 
                wali::Triple<wali::nwa::State<> ,wali::nwa::Symbol<>,wali::nwa::State<> >(call,callInst,entry);
  wali::Triple<wali::nwa::State<> ,wali::nwa::Symbol<>,
        wali::nwa::State<> > internalTrans = 
                wali::Triple<wali::nwa::State<> ,wali::nwa::Symbol<>,
                        wali::nwa::State<> >(entry,intraInst,exit);
  wali::Quad<wali::nwa::State<> ,wali::nwa::State<> ,
      wali::nwa::Symbol<>,wali::nwa::State<> > returnTrans = 
                wali::Quad<wali::nwa::State<> ,wali::nwa::State<> ,
                      wali::nwa::Symbol<>,wali::nwa::State<> >(exit,call,retInst,ret);
  
  myNWA.isAbsentAccept();
  myNWA.acceptAbsent();
  myNWA.rejectAbsent();
  
  myNWA.addInitialState(&start);
  myNWA.addState(&state);
  myNWA.addState(&call);
  myNWA.addState(&entry);
  myNWA.addState(&exit);
  myNWA.addState(&ret);
  myNWA.addFinalState(&end);
  
  myNWA.getState(wali::getKey("call"));
  myNWA.getStates();
  myNWA.getStateNames();
  myNWA.duplicateState(wali::getKey("call"),wali::getKey("call2"));
  
  //myNWA.addAllStates(wali::nwa::StateSet<wali::nwa::State ,std::string>());
  //myNWA.addAllInitialStates(wali::nwa::StateSet<wali::nwa::State ,std::string>());
  //myNWA.addAllFinalStates(wali::nwa::StateSet<wali::nwa::State ,std::string>(),false);
  myNWA.isState(&state);
  myNWA.isInitialState(&start);
  myNWA.isFinalState(&end);
  //myNWA.beginStates();
  //myNWA.endStates();
  //myNWA.beginInitialStates();
  //myNWA.endInitialStates();
  //myNWA.beginFinalStates();
  //myNWA.endFinalStates();  
  myNWA.sizeStates();
  myNWA.sizeInitialStates();
  myNWA.sizeFinalStates();
  
  //myNWA.getSymbols();
  myNWA.isSymbol(epsilon);
  myNWA.addSymbol(epsilon);
 //myNWA.addAllSymbols(std::set<wali::nwa::Symbol *>());  
  //myNWA.beginSymbols();
  //myNWA.endSymbols();
  myNWA.sizeSymbols();
    
  myNWA.addInternalTrans(&start,epsilon,&call);
  myNWA.addInternalTrans(wali::getKey("start"),epsilon,wali::getKey("call"));
  myNWA.addCallTrans(callTrans);
  myNWA.addCallTrans(&call,callInst,&entry);
  myNWA.addCallTrans(wali::getKey("call"),callInst,wali::getKey("entry"));
  myNWA.addInternalTrans(internalTrans);
  myNWA.addInternalTrans(&entry,intraInst,&state);
  myNWA.addInternalTrans(wali::getKey("entry"),intraInst,wali::getKey("state"));
  myNWA.addInternalTrans(&state,intraInst,&exit);
  myNWA.addInternalTrans(wali::getKey("state"),intraInst,wali::getKey("exit"));
  myNWA.addReturnTrans(returnTrans);
  myNWA.addReturnTrans(&exit,&call,retInst,&ret);
  myNWA.addReturnTrans(wali::getKey("exit"),wali::getKey("call"),retInst,wali::getKey("ret"));
  myNWA.addInternalTrans(&ret,epsilon,&end);
  myNWA.addInternalTrans(wali::getKey("ret"),epsilon,wali::getKey("end"));
  //myNWA.beginCallTrans();
  //myNWA.endCallTrans();
  //myNWA.beginInternalTrans();
  //myNWA.endInternalTrans();
  //myNWA.beginReturnTrans();
  //myNWA.endReturnTrans();
  
  myNWA.getSymbol(wali::getKey("call"),wali::getKey("entry"),sym);
  myNWA.getReturnSites(wali::getKey("call"));
  
  myNWA.print(std::cout);
  
  ReachGen<wali::nwa::State<> ,wali::nwa::Symbol<>> wg;  
  myNWA.NWAtoPDS(wg);
  myNWA.NWAtoBackwardsPDS(wg);

  wali::nwa::NWA<> otherNWA;
  wali::nwa::NWA<> intersectNWA;
  myNWA.intersect(otherNWA,intersectNWA);

  bool equal = myNWA.operator==(otherNWA);
  //myNWA.numStates();
  //myNWA.numTrans();

  myNWA.removeInternalTrans(&start,epsilon,&call);
  myNWA.removeInternalTrans(wali::getKey("start"),epsilon,wali::getKey("call"));
  myNWA.removeCallTrans(callTrans);
  myNWA.removeCallTrans(&call,callInst,&entry);
  myNWA.removeCallTrans(wali::getKey("call"),callInst,wali::getKey("entry"));
  myNWA.removeInternalTrans(internalTrans);
  myNWA.removeInternalTrans(&entry,intraInst,&state);
  myNWA.removeInternalTrans(wali::getKey("entry"),intraInst,wali::getKey("state"));
  myNWA.removeInternalTrans(&state,intraInst,&exit);
  myNWA.removeInternalTrans(wali::getKey("state"),intraInst,wali::getKey("exit"));
  myNWA.removeReturnTrans(returnTrans);
  myNWA.removeReturnTrans(&exit,&call,retInst,&ret);
  myNWA.removeReturnTrans(wali::getKey("exit"),wali::getKey("call"),retInst,wali::getKey("ret"));
  myNWA.removeInternalTrans(&ret,epsilon,&end);
  myNWA.removeInternalTrans(wali::getKey("ret"),epsilon,wali::getKey("end"));
  myNWA.removeReturnTrans(wali::getKey("exit"),retInst,wali::getKey("ret"));
  myNWA.removeReturnTrans(&exit,retInst,&ret);

  myNWA.removeState(&state);
  myNWA.removeInitialState(&start);
  myNWA.removeFinalState(&end);
  myNWA.clearStates();
  myNWA.clearInitialStates();
  myNWA.clearFinalStates();
  
  myNWA.removeSymbol(epsilon);
  myNWA.clearSymbols();
  
  myNWA.clearTrans();
#endif

  return 0;
}
#endif
