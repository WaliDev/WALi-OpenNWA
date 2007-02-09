#ifndef __SWPDS_H_
#define __SWPDS_H_

#include <map>
#include "../ewpds/EWPDS.h"
#include "InterGraph.h"
#include "SummaryGraph.h"

using namespace wpds;
using namespace fwpds;
using namespace std;

template< typename T > struct CompareCA : wpds::util::TransActionFunctor<T>
{
  GEN_WPDS_TYPEDEFS(T);

  CA<T>& rhs;
  bool isEqual;
  catrans_t badT;

  CompareCA( CA<T>& r ) : rhs(r),isEqual(true) {}

  virtual ~CompareCA() {}

  virtual void operator()( const catrans_t& t )
  {
    if(t->stack() == WPDS_EPSILON)
      return;
    // guard so that as soon as we are no longer equal
    // we skip doing anymore work
    if( isEqual ) {
      catrans_t rhsT;
      if( rhs.find( t->from_state(), t->stack(), t->to_state(), rhsT ) ) {
        if( !t->weight()->equal( rhsT->weight().get_ptr() ) ) {
          badT = rhsT;
          badT->print(std::cerr << "rhsT: ") << std::endl;
          //badT->witness()->print( std::cerr << "\nbadT Witness ---------------------------------\n" ) << std::endl;
          t->print(std::cerr << "t: ") << std::endl;
          //t->witness()->print( std::cerr << "\n   T Witness ---------------------------------\n" ) << std::endl;
          isEqual = false;
        }
      }
      else {
        if(!t->weight()->equal( t->weight()->zero() )) {
          t->print(cerr << "t: ") << "\n";
          badT = t;
          isEqual = false;
        }
      }
    }
  }
};

template< typename T > struct Step1 : wpds::util::TransActionFunctor<T>
{
  GEN_WPDS_TYPEDEFS(T);
  
  WPDS<T>& pds;
  
  Step1( WPDS<T>& pds_) : pds(pds_) {}
  
  virtual ~Step1() {}
  
  virtual void operator()( const catrans_t& t )
  {
    pds.add_rule( t->from_state(), t->stack(), t->to_state(), t->weight().get_ptr() );
  }
};

template< typename T > struct PopPhaseWorklistConstructor : wpds::util::TransActionFunctor<T> {
  GEN_WPDS_TYPEDEFS(T);
  list< CATransition<T> *> &worklist;
  wpds_key_t start_state;

  PopPhaseWorklistConstructor(list< CATransition<T> *> &w, wpds_key_t ss) : worklist(w), start_state(ss) { }

  virtual void operator() (const catrans_t &t) {
    if(t->from_state() == start_state) {
      worklist.push_back(t.get_ptr());
      t->is_in_workset(true);
    }
  }
};

template< typename T> struct PopPhase;

template< typename T > struct CommutativePopPhaseHelper : wpds::util::TransActionFunctor<T> {
  GEN_WPDS_TYPEDEFS(T);
  CA<T> ca;
  Semiring<T> &sem;
  PopPhase<T> &popPhase;
  CA<T> &ca_out;
  wpds_key_t start_state;
  bool initialized;

  CommutativePopPhaseHelper(Semiring<T> &s, PopPhase<T> &p, 
                            CA<T> &ca_, wpds_key_t ss) 
    : ca(s)
    , sem(s), popPhase(p), ca_out(ca_)
    , start_state(ss), initialized(false) { }

  virtual void operator()(const catrans_t &t) {
    if(!initialized) {
      // Build a reverse automaton because path_summary starts at final states and propagates backwards
      sem_elem_t wt = t->semiring_element()->extend(popPhase.popWeight(t->stack()).get_ptr());
      ca.add(t->to_state(), WPDS_EPSILON, t->from_state(), wt.get_ptr());
    } else {
      sem_elem_t wt = stateWeight(t->from_state());
      if(!wt->equal(sem.zero().get_ptr())) {
        // Add transitions to output automaton
        wt = t->semiring_element()->extend(wt.get_ptr());
        ca_out.add(start_state, t->stack(), t->to_state(), wt);
      }
    }
  }
  void setup() {
    ca.set_query(Query::poststar());
    ca.add_final_state(start_state);
    ca.path_summary();
    initialized = true;
  }
  sem_elem_t stateWeight(wpds_key_t tst) {
    return ca.state_weight(tst);
  }
};

template< typename T > struct PopPhase : wpds::util::TransActionFunctor<T> {
  GEN_WPDS_TYPEDEFS(T);
  Semiring<T> &sem;
  wpds_key_t start_state;
  map<wpds_key_t, sem_elem_t> pop_weight;

  typedef list< CATransition<T>* > Worklist_t;
  Worklist_t worklist;

  PopPhase(Semiring<T> &s_, wpds_key_t ss) : sem(s_), start_state(ss) { }

  // For building pop_weight
  virtual void operator()(const catrans_t &t) {
    assert(pop_weight.find(t->stack()) == pop_weight.end());
    pop_weight[t->stack()] = t->semiring_element();
  }

  sem_elem_t popWeight(wpds_key_t stk) {
    if(pop_weight.find(stk) == pop_weight.end()) {
      return sem.zero();
    }
    return pop_weight[stk];
  }

  void commutativeRun(CA<T> &ca_in, CA<T> &ca_out) {
    CommutativePopPhaseHelper<T> hlp(sem, *this, ca_out, start_state);
    ca_in.for_each(hlp);
    hlp.setup();
    ca_in.for_each(hlp);
  }

  void run(CA<T> &ca) {
    //cout << "Before: "<< ca << "\n";

#ifdef DWPDS
    assert(0); // the saturation procedure is not designed for DWPDS, yet.
#endif
    // The saturation will proceed by first applying pop-rules on all transitions in the worklist.
    // Then, eps-contraction on the new eps-transition produced, and then repeat. The idea is to delay
    // eps-contraction for as long as possible.

    // Put all initial transitions into the worklist -- can be optimized
    PopPhaseWorklistConstructor<T> wc(worklist,start_state);
    ca.for_each(wc);

    typedef int TransPtr; // to stand for CATransition<T> *. This hack was necessary to make it compile under cygwin
    set< TransPtr > epstrans;
    catrans_t t, teps, tprime;
    sem_elem_t wt;

    while(!worklist.empty()) {

      // Apply all pop-rules
      while(!worklist.empty()) {
        // Get a transition from the worklist
        t = worklist.front();
        worklist.pop_front();
        t->is_in_workset(false);
        
        // compute its weight and insert the new transition
        wt = popWeight(t->stack());
        teps = ca.add(t->from_state(), WPDS_EPSILON, t->to_state(), t->semiring_element()->extend(wt.get_ptr()));
        if(teps->is_modified()) {
          epstrans.insert((TransPtr)teps.get_ptr());
        }
      }

      //cout << ca << "\n";
      // Do eps-contraction
      set< TransPtr > :: iterator it;
      for(it = epstrans.begin(); it != epstrans.end(); it++) {
        teps = (CATransition<T> *)(*it);
        
        TransListIterPair tIterPair;
        TransListIter titer;
        tIterPair = ca.match( teps->to_state() );
        for( titer = tIterPair.first;
             titer != tIterPair.second; titer++ ) {
          
          tprime = *titer;
          
          wt = tprime->semiring_element()->extend(teps->semiring_element().get_ptr());
          
          t = ca.add(teps->from_state(), tprime->stack(), tprime->to_state(), wt.get_ptr());
          if(t->is_modified() && !t->is_in_workset()) {
            worklist.push_back(t.get_ptr());
            t->is_in_workset(true);
          }
        }
      }
      epstrans.clear();
      //cout << ca << "\n";
    }
  }

  void eagerContraction(CA<T> &ca) {
    //cout << "Before: "<< ca << "\n";

#ifdef DWPDS
    assert(0); // the saturation procedure is not designed for DWPDS, yet.
#endif
    // The saturation will proceed by first applying a pop-rule and immediately doing eps-contraction
    // on the resulting eps-transition. The worklist is processed in a depth-first order (LIFO)

    // Put all initial transitions into the worklist -- can be optimized
    PopPhaseWorklistConstructor<T> wc(worklist,start_state);
    ca.for_each(wc);

    typedef int TransPtr; // to stand for CATransition<T> *. This hack was necessary to make it compile under cygwin
    set< TransPtr > epstrans;
    catrans_t t, teps, tprime;
    sem_elem_t wt;

    while(!worklist.empty()) {
      // Get a transition from the worklist
      t = worklist.front();
      worklist.pop_front();
      t->is_in_workset(false);
      
      // compute its weight 
      wt = popWeight(t->stack());
      teps = ca.add(t->from_state(), WPDS_EPSILON, t->to_state(), t->semiring_element()->extend(wt.get_ptr()));
      if(teps->is_modified()) {
        // Do eps-copntraction
        TransListIterPair tIterPair;
        TransListIter titer;
        tIterPair = ca.match( teps->to_state() );
        for( titer = tIterPair.first;
             titer != tIterPair.second; titer++ ) {
          
          tprime = *titer;
          
          wt = tprime->semiring_element()->extend(teps->semiring_element().get_ptr());
          
          t = ca.add(teps->from_state(), tprime->stack(), tprime->to_state(), wt.get_ptr());
          if(t->is_modified() && !t->is_in_workset()) {
            worklist.push_back(t.get_ptr());
            t->is_in_workset(true);
          }
        }
      }
    }      
  }
};

template< typename T > struct ApplyGrow : wpds::util::TransActionFunctor<T>
{
  GEN_WPDS_TYPEDEFS(T);

  wpds_key_t start_state;  
  SummaryGraph *gr;
  CA<T> &ca;

  // FIXME: For some reason list<catrans_t> wasn't compiling under cygwin
  typedef map<wpds_key_t, list<WTransition> > transMap_t;
  transMap_t transMap;

  ApplyGrow(wpds_key_t ss, SummaryGraph *gr_, CA<T> &ca_) : start_state(ss), gr(gr_), ca(ca_) {}
  
  virtual ~ApplyGrow() {}
  
  virtual void operator()( const catrans_t& t )
  {
    if(t->from_state() == start_state) {
      Transition tt(t->from_state(), t->stack(), t->to_state());
      transMap[t->to_state()].push_back(WTransition(tt, t->weight().get_ptr()) );
    }
  }
  void run() {
    transMap_t::iterator it;
    list<WTransition> newtrans;
    list<WTransition>::iterator trans_it;

    clock_t t1=0,t2=0,t3=0,t4=0,t5=0,start,end;

    for(it = transMap.begin(); it != transMap.end(); it++) {
      list<WTransition> &ls = it->second;
      list<WTransition>::iterator it2;
      start = clock();
      // Pass weights of initial transitions
      for(it2 = ls.begin(); it2 != ls.end(); it2++) {
        WTransition &t = *it2;
        if(t.first.stack != (int)WPDS_EPSILON)
          gr->addUpdate(t.first, t.second.get_ptr());
      }

      end = clock();
      t1 += (end-start);
      start = end;

      // Get new transitions to add
      gr->getUpdatedTransitions(newtrans);

      end = clock();
      t2 += (end-start);
      start = end;

      // Add them to the automaton
      for(trans_it = newtrans.begin(); trans_it != newtrans.end(); trans_it++) {
        ca.add(trans_it->first.src, trans_it->first.stack, it->first, (T *)trans_it->second.get_ptr());
      }
      newtrans.clear();

      end = clock();
      t3 += (end-start);
      start = end;
    }
    cout << "SWPDS: States = " << transMap.size() << "\n";
    start = clock();
    // Now get the middle transitions to add
    gr->getMiddleTransitions(newtrans);
    end = clock();
    t4 += (end - start);
    start = end;
    // Add them to the automaton
    for(trans_it = newtrans.begin(); trans_it != newtrans.end(); trans_it++) {
      ca.add(trans_it->first.src, trans_it->first.stack, trans_it->first.tgt, (T *)trans_it->second.get_ptr());
    }
    end = clock();
    t5 += (end - start);
    Timer::printTime(cout << "t1 = ", t1) << "\n";
    Timer::printTime(cout << "t2 = ", t2) << "\n";
    Timer::printTime(cout << "t3 = ", t3) << "\n";
    Timer::printTime(cout << "t4 = ", t4) << "\n";
    Timer::printTime(cout << "t5 = ", t5) << "\n";
  }
};

template< typename T >
struct EwpdsAgrowGenerator : wpds::util::RuleActionFunctor< util::SemiringElemPair<T,T> >
{
  GEN_WPDS_TYPEDEFS(T);

  CA<T>& Agrow;
  wpds_key_t init;
  const Semiring<T>& s;
  std::set< wpds_key_t > returnPoints;

  EwpdsAgrowGenerator( CA<T>& ca
                       , wpds_key_t initState) :
    Agrow(ca)
    , init(initState)
    , s( ca.semiring() )
  {
  }

  virtual ~EwpdsAgrowGenerator() {}

  virtual void operator()( const Rule< SemiringElemPair<T,T> >* r )
  {
    wpds_key_t k1 = create_key( new key_pair_src(init, r->to_stack1() ) );
    Agrow.add( init, r->to_stack1(), k1, s.one().get_ptr() );

    // save return points
    returnPoints.insert( r->to_stack2() );
  }
};

template< typename T >
struct EwpdsSymbols : wpds::util::RuleActionFunctor< util::SemiringElemPair<T,T> >
{
  GEN_WPDS_TYPEDEFS(T);

  std::set< wpds_key_t > gamma;
  std::set< wpds_key_t > returnPoints;

  EwpdsSymbols() { }

  virtual ~EwpdsSymbols() {}

  virtual void operator()( const Rule< SemiringElemPair<T,T> >* r )
  {
    addPoint(r->from_stack(), gamma);
    addPoint(r->to_stack1(), gamma);
    addPoint(r->to_stack2(), gamma);
    addPoint(r->to_stack2(), returnPoints); 
  }

  void addPoint(wpds_key_t k, set<wpds_key_t> &s) {
    if(k != WPDS_EPSILON)
      s.insert(k);
  }

};

template <typename T>
class SWPDS {
  EWPDS<T> &pds;
  wpds_key_t start_state;
  Semiring<T> sem;
  WPDS<T> Apop;
  PopPhase<T> popPhase;
  SummaryGraph *sgr;
  EwpdsSymbols<T> syms;
  void (*flush_cache)();
 public:
  SWPDS(EWPDS<T> &ewpds_,
        wpds_key_t ss,
        Semiring<T> &s_) : 
    pds(ewpds_), start_state(ss), sem(s_)
    , Apop(s_), popPhase(s_,ss), sgr(NULL) {
    flush_cache = &no_op;
  }
    ~SWPDS() {
      if(sgr != NULL)
        delete sgr;
    }
    void initialize(wpds_key_t start_node);
    void summaryPoststar(CA<T> &ca_in, CA<T> &ca_out);
    void genCA( CA<T>& ca, CA<T>& Source);
    void set(void (*f)()) {
      flush_cache = f;
    }
 private:
    static void no_op() { }    
};

template <typename T>
void SWPDS<T>::initialize(wpds_key_t start_node) {
  // Get all EWPDS symbols
  pds.for_each_rule(syms);
  // Run prestar to get all the summaries
  CA<T> prestarCA(sem);
  prestar<T>(pds, prestarCA, sem, prestarCA);
  Step1<T> step1(Apop); // Apop is the WPDS containing all the pop rules
  prestarCA.for_each(step1);
  prestarCA.for_each(popPhase);
  prestarCA.clear();

  // Now create the (multiple-final-state) automaton for running post*
  CA<T> tmpGrow(sem);
  EwpdsAgrowGenerator<T> growGen(tmpGrow, start_state);
  pds.for_each_rule2(growGen);
  // Add transition for start_node too
  tmpGrow.add(start_state, start_node, create_key(new key_pair_src(start_state, start_node)), sem.one().get_ptr());

  // Now run post* with swpdsTracking
  //gr = new InterGraph(sem.get_data(), true, false);
  InterGraph *gr = FWPDSpoststar<T>(pds, tmpGrow, sem, tmpGrow);
  //gr->print(cout, (InterGraph::PRINT_OP)printkey) << "\n";
  gr->print_stats(cout) << "\n";

  // pre-processing
  sgr = new SummaryGraph(gr, start_state, (InterGraph::PRINT_OP)printkey );
  //cout << "SWPDS: Graphs = " << gr->nGraphs() << "\n";
  //gr->print(cout, (swpds::InterGraph::PRINT_OP)printkey) << "\n";
  RegExp::reset();
}

//#define COMMUTATIVE_EXTEND

template <typename T>
void SWPDS<T>::summaryPoststar(CA<T> &ca_in, CA<T> &ca_out) {
  //CA<T> ca_pop(ca_in);
  if(&ca_out != &ca_in) {
    ca_out.clear();
  }

  CA<fwpds::SemElem> *ca1 = (CA<fwpds::SemElem> *)(&ca_in);
  CA<fwpds::SemElem> *ca2 = (CA<fwpds::SemElem> *)(&ca_out);
  sgr->summaryPoststar(*ca1, *ca2);
  return;

  // pop-phase: Saturate with pop-rules
  
  {
    Timer timer("Pop Phase");

#define USE_OWN_SATURATION

#ifdef USE_OWN_SATURATION 

#ifdef COMMUTATIVE_EXTEND
    if(&ca_out == &ca_in) {
      CA<T> ca_tmp(ca_in);
      popPhase.commutativeRun(ca_tmp, ca_out);
    } else {
      ca_out.operator=(ca_in);
      popPhase.commutativeRun(ca_in, ca_out);
    }
#else // !COMMUTATIVE_EXTEND
    if(&ca_out != &ca_in) {
      ca_out.operator=(ca_in);
    }
    popPhase.run(ca_out);
    //ca_pop = ca_out;
    //popPhase.eagerContraction(ca_out);
#endif // COMMUTATIVE_EXTEND

#else // !USE_OWN_SATURATION

    // pop-phase: use FWPDS
    poststar(Apop, ca_in, sem, ca_out);
    //ca_out = ca_in;

#endif // USE_OWN_SATURATION
  }
  flush_cache();

  // push-phase: grow the automaton
  {
    Timer timer("Push Phase");
    ApplyGrow<T> applyGrow(start_state, sgr, ca_out);
    ca_out.for_each(applyGrow);
    applyGrow.run();
  }
  sgr->printStats(cout) << "\n";
}

// Create a random source configuration automaton
template <typename T>
void SWPDS<T>::genCA( CA<T>& ca, CA<T>& Source)
{
  Source.clear();

  wpds_key_t Sf = str2key("S.final");
  Source.add_initial_state( ca.initial_state() );
  Source.add_final_state( Sf );

  std::set< wpds_key_t >::iterator it = syms.returnPoints.begin();
  std::set< wpds_key_t >::iterator itEND = syms.returnPoints.end();

  // (Return point)*
  for( ; it != itEND ; it++ ) {
    Source.add( Sf, *it, Sf, sem.one().get_ptr() );
  }

  int r = (int)(syms.gamma.size() * (rand()/(1.0+RAND_MAX)));
  it = syms.gamma.begin();
  while(r > 0) {
    it++; r--;
  }
  Source.add(Source.initial_state(), *it, Sf, sem.one().get_ptr());
}

#endif
