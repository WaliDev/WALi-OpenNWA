#include "opennwa/Nwa.hpp"
#include "opennwa/construct/determinize.hpp"

namespace opennwa
{
  namespace construct
  {
      
    void determinize(Nwa & out, Nwa const & source)
    {
      out._private_determinize_(source);
    }


    NwaRefPtr determinize( Nwa const & nondet )
    {
      NwaRefPtr nwa( new Nwa());
      determinize(*nwa, nondet);
      return nwa;
    }

      
  } // end 'namespace construct' !!!
    

#if 0
  template< T>
  struct terrible_set_hasher
  {
    size_t operator() (T const & set) const {
      return set.size();
    }
  };

  template< T>
  struct terrible_set_comparer
  {
    bool operator() (T const & lhs, T const & rhs) const {
      if (lhs.size() != rhs.size()) return false;
          
      for ( T::const_iterator li = lhs.begin() ; li != lhs.end() ; ++li) {
        if (rhs.find(*li) == rhs.end()) return false;
      }

      return true;
    }
  };
#endif
      

  /**
   *
   * @brief constructs a deterministic NWA that is equivalent to the given NWA.
   *
   * @param - nondet: the NWA to determinize
   *
   */
    
  void Nwa::_private_determinize_( Nwa const & nondet )
  {
#ifdef USE_BUDDY
    wali::relations::buddyInit();
#endif

#ifdef USE_BUDDY
#  define DECLARE(type, name)  type name(nondet.largestState())
#else
#  define DECLARE(type, name)  type name
#endif
        
    //TODO: ponder the following ...
    //Q: how do we guarantee that the stuck state is not any of the states generated here?
      
    //Q: what should be done with clientInfos here?
    //A: use helper methods similar to the treatment of clientInfos for intersection

    //Clear all states(except the stuck state) and transitions from this machine.
    clear();


    using namespace wali::relations;

    typedef std::set<std::pair<State, State> > SetBinaryRelation;
    typedef  RelationTypedefs<State>::TernaryRelation TernaryRelation;

#if 0
    typedef  std::tr1::unordered_set<BinaryRelation
      , terrible_set_hasher<BinaryRelation>
      //, terrible_set_comparer<BinaryRelation>
      >
      RelationSet;
#endif

#ifdef USE_BUDDY
    typedef VectorSet<BinaryRelation> RelationSet;
#else
    typedef std::set<BinaryRelation> RelationSet;
#endif

    // Construct Id
    DECLARE(BinaryRelation, Id);
    for( StateIterator sit = nondet.beginStates(); sit != nondet.endStates(); sit++ )
    {
      Id.insert(std::pair<State,State>(*sit,*sit));
    }
      
    //Construct the epsilon closure relation for the states in nondet.
    SetBinaryRelation pre_close; //Collapse epsilon transitions.
    SetBinaryRelation Ie;   //Internal transitions with epsilon.
    project_symbol_3<SetBinaryRelation>(Ie,nondet.trans.getInternals(), EPSILON);
#ifdef USE_BUDDY
    transitive_closure(pre_close,Ie);
#else
    transitive_closure<State>(pre_close,Ie);
#endif

    DECLARE(BinaryRelation, bdd_pre_close);
    for( SetBinaryRelation::const_iterator iter = pre_close.begin(); iter != pre_close.end(); ++iter) {
      bdd_pre_close.insert(*iter);
    }

    DECLARE(BinaryRelation, close);
    union_(close, bdd_pre_close, Id);

    // R0 is used later; to avoid recomputation we do it now
    // Epsilon Closure( Q0 x Q0 )
    DECLARE(BinaryRelation, R0);
    DECLARE(BinaryRelation, q0crossQ0);

    for( StateIterator initial1 = nondet.beginInitialStates();
         initial1 != nondet.endInitialStates(); ++initial1 )
    {
      for( StateIterator initial2 = nondet.beginInitialStates();
           initial2 != nondet.endInitialStates(); ++initial2 )
      {
        q0crossQ0.insert(std::make_pair(*initial1, *initial2));
      }
    }
      
#ifdef USE_BUDDY
    compose/*<St>*/(R0,q0crossQ0,close);
#else
    compose<State>(R0,q0crossQ0,close);
#endif

    //Make a key for this state.
    State r0 = makeKey(R0);
      
    //Add the initial state to the deterministic NWA.
    addInitialState(r0);

    //Set the client info for this new state.
    ClientInfoRefPtr CI;
    mergeClientInfo(nondet,R0,r0,CI);
    states.setClientInfo(r0,CI);

    //Put the initial state on the worklist.
    RelationSet wl;
    wl.insert(R0); // or close

    //Keep track of all visited states.
    RelationSet visited;

    // Pre-compute some projections and stuff
    std::map<wali::Key, BinaryRelation> callTransPerSymbol;
    std::map<wali::Key, BinaryRelation> internalTransPerSymbol;
    std::map<wali::Key, TernaryRelation> returnTransPerSymbol;

    for( SymbolIterator it = nondet.beginSymbols(); it != nondet.endSymbols(); it++ ) {
      if (*it == EPSILON) continue;    //Epsilon is handled with closure.
      if (*it == WILD) continue;

#ifdef USE_BUDDY
      internalTransPerSymbol[*it] = BinaryRelation(nondet.largestState());
      callTransPerSymbol[*it] = BinaryRelation(nondet.largestState());
      returnTransPerSymbol[*it] = TernaryRelation(nondet.largestState());
#endif
        
      project_symbol_3<BinaryRelation>(internalTransPerSymbol[*it], nondet.trans.getInternals(), *it);
      project_symbol_3<BinaryRelation>(internalTransPerSymbol[*it], nondet.trans.getInternals(), WILD);

      project_symbol_3<BinaryRelation>(callTransPerSymbol[*it], nondet.trans.getCalls(), *it);
      project_symbol_3<BinaryRelation>(callTransPerSymbol[*it], nondet.trans.getCalls(), WILD);   //Every symbol also matches wild.

      project_symbol_4(returnTransPerSymbol[*it], nondet.trans.getReturns(), *it);
      project_symbol_4(returnTransPerSymbol[*it], nondet.trans.getReturns(),WILD);   //Every symbol also matches wild.
    }

      
    //Process the states on the worklist.
    while(! wl.empty() )
    {
      //Take a state off of the worklist.
      BinaryRelation R = *wl.begin();
      wl.erase(wl.begin());
  
      //Mark this state as visited.
      visited.insert(R);

      //Make a key for this state.
      State r = makeKey(R);


      //Check each symbol individually.
      for( SymbolIterator it = nondet.beginSymbols(); it != nondet.endSymbols(); it++ )
      {
        if (*it == EPSILON) continue;    //Epsilon is handled with closure.
        if (*it == WILD) continue;       //Wild is matched to every symbol as we go.

        //Process internal transitions.
        //Compute the relation.
        DECLARE(BinaryRelation, Ri);
        DECLARE(BinaryRelation, Rtmpi);
        BinaryRelation const & Ii = internalTransPerSymbol[*it];

#if 0
        DECLARE(BinaryRelation, IiOrig);
        project_symbol_3(IiOrig,nondet.trans.getInternals(),*it);   
        project_symbol_3(IiOrig,nondet.trans.getInternals(),WILD);   //Every symbol also matches wild.
          
        if (Ii == IiOrig) {
          std::cout << "Ii == IiOrig holds!\n";
        }
        else {
          std::cout << "***** Ii == IiOrig VIOLATED\n";
          std::cout << "      Ii's size is " << Ii.size() << "\n";
          std::cout << "      Orig's size is " << IiOrig.size() << "\n";
        }
#endif

#ifdef USE_BUDDY
        compose/*<St>*/(Rtmpi,R,Ii);
        compose/*<St>*/(Ri,Rtmpi,close);
#else
        compose<State>(Rtmpi,R,Ii);
        compose<State>(Ri,Rtmpi,close);
#endif
        //Make a key for this state.
        State ri = makeKey(Ri);

        //Add the state to the deterministic NWA.
        addState(ri);
        //Add the transition to the deterministic NWA.
        addInternalTrans(r,*it,ri);

        //Adjust the client info for this state.
        ClientInfoRefPtr riCI;
        mergeClientInfoInternal(nondet,R,Ri,r,*it,ri,riCI);
        states.setClientInfo(ri,riCI);

        //Determine whether to add this state to the worklist.
        if( visited.find(Ri) == visited.end() )
        {
          wl.insert(Ri);
        }

        //Process call transitions.
        //Compute the relation.
        //BinaryRelation IdClose_Delta2;
        DECLARE(BinaryRelation, Rc);
        BinaryRelation const & Ic = callTransPerSymbol[*it];

#if 0
        DECLARE(BinaryRelation, IcOrig);
        project_symbol_3(IcOrig,nondet.trans.getCalls(),*it);  
        project_symbol_3(IcOrig,nondet.trans.getCalls(),WILD);   //Every symbol also matches wild.
          
        if (Ic == IcOrig) {
          std::cout << "Ic == IcOrig holds!\n";
        }
        else {
          std::cout << "***** Ic == IcOrig VIOLATED\n";
        }
#endif

        //compose/*<St>*/(IdClose_Delta2, R0, Ic);
        //compose/*<St>*/(Rc,IdClose_Delta2,close);
#ifdef USE_BUDDY
        compose/*<St>*/(Rc,Ic,close);
#else
        compose<State>(Rc,Ic,close);
#endif
          
        //Make a key for this state.
        State rc = makeKey(Rc);
        //Add the state to the deterministic NWA.
        addState(rc);
        //Add the transition to the deterministic NWA.
        addCallTrans(r,*it,rc);

        //Adjust the client info for this state.
        ClientInfoRefPtr rcCI;
        mergeClientInfoCall(nondet,R,Rc,r,*it,rc,rcCI);
        states.setClientInfo(rc,rcCI);

        //Determine whether to add this state to the worklist.
        if( visited.find(Rc) == visited.end() )
        {
          wl.insert(Rc);
        }

        //Process return transitions.
        TernaryRelation const & Ir = returnTransPerSymbol[*it];

#if 0
        TernaryRelation IrOrig;
        project_symbol_4(IrOrig,nondet.trans.getReturns(),*it);    
        project_symbol_4(IrOrig,nondet.trans.getReturns(),WILD);   //Every symbol also matches wild.

        if (Ir == IrOrig) {
          std::cout << "Ir == IrOrig holds!\n";
        }
        else {
          std::cout << "***** Ir == IrOrig VIOLATED\n";
        }
#endif

        //For each possible call predecessor:
        for(  RelationSet::iterator rit = visited.begin();
              rit != visited.end(); rit++ )
        {
          //Compute the relation.
          DECLARE(BinaryRelation, Rr);
          DECLARE(BinaryRelation, Rtmpr);
#ifdef USE_BUDDY
          merge/*<St>*/(Rtmpr,R,*rit,Ir);
          compose/*<St>*/(Rr,Rtmpr,close);
#else
          merge<State>(Rtmpr,R,*rit,Ir);
          compose<State>(Rr,Rtmpr,close);
#endif
          //Make a key for this state and the call predecessor state.
          State rr = makeKey(Rr);
          State rc2 = makeKey(*rit);
          //Add the state to the deterministic NWA.
          addState(rr);
          //Add the transition to the deterministic NWA.
          addReturnTrans(r,rc2,*it,rr);

          //Adjust the client info for this state.
          ClientInfoRefPtr rrCI;
          mergeClientInfoReturn(nondet,R,*rit,Rr,r,rc2,*it,rr,rrCI);
          states.setClientInfo(rr,rrCI);

          //Determine whether to add this state to the worklist.
          if( visited.find(Rr) == visited.end() )
          {
            wl.insert(Rr);
          }
        }
        //For each possible exit point:
        for(  RelationSet::iterator rit = visited.begin();
              rit != visited.end(); rit++ )
        {
          //Compute the relation.
          DECLARE(BinaryRelation, Rr);
          DECLARE(BinaryRelation, Rtmpr);
#ifdef USE_BUDDY
          merge/*<St>*/(Rtmpr,*rit,R,Ir);
#else
          merge<State>(Rtmpr,*rit,R,Ir);
#endif
#ifdef USE_BUDDY
          compose/*<St>*/(Rr,Rtmpr,close);
#else
          compose<State>(Rr,Rtmpr,close);
#endif
          //Make a key for this state and the exit point state.
          State rr = makeKey(Rr);
          State re = makeKey(*rit);
          //Add the state to the deterministic NWA.
          addState(rr);
          //Add the transition to the deterministic NWA.
          addReturnTrans(re,r,*it,rr);
            
          //Adjust the client info for this state.
          ClientInfoRefPtr rrCI;
          mergeClientInfo(nondet,Rr,rr,rrCI);
          states.setClientInfo(rr,rrCI);

          //Determine whether to add this state to the worklist.
          if( visited.find(Rr) == visited.end() )
          {
            wl.insert(Rr);
          }
        }
      }

      //Deterministic final states.
      //Necessary components for a final state, i.e., 
      //any final state must contain one of the pairs in 
      //{(q,fin) | q is any state and fin is a final state}
      DECLARE(BinaryRelation, Rf);
      for( StateIterator iit = nondet.beginStates();
           iit != nondet.endStates(); iit++ )
      {
        for( StateIterator fit = nondet.beginFinalStates();
             fit != nondet.endFinalStates(); fit++ )
        {
          Rf.insert(std::pair<State,State>(*iit,*fit));
        }
      }
      //For each state in the deterministic maching, check whether it is a final state.
      for(  RelationSet::iterator sit = visited.begin();
            sit != visited.end(); sit++ )
      {
        DECLARE(BinaryRelation, Rtmpf);
        wali::relations::intersect(Rtmpf,Rf,*sit);
        if(! Rtmpf.empty() )
        {
          State f = makeKey(*sit);
          addFinalState(f);
        }
      }
    }
#undef DECLARE
  }

#ifdef USE_BUDDY
    
  State NWA::makeKey(
    wali::relations::RelationTypedefs<State>::BinaryRelation const & R ) const
  {
    std::stringstream ss;
    ss << R.getBdd();
    return getKey(ss.str());
  }
#else
  /**
   *
   * @brief returns the state corresponding to the given binary relation
   *
   * @param - R: the binary relation whose state to access
   * @return the state corresponding to the given binary relation
   *
   */
    
  State Nwa::makeKey(
    wali::relations::RelationTypedefs<State>::BinaryRelation const & R ) const
  {
    // C++ really needs C++0x's 'auto' keyword :-)
    typedef  wali::relations::RelationTypedefs<State>::BinaryRelation::const_iterator Iterator;

    std::stringstream ss;

    ss << "{";
      
    for( Iterator mit = R.begin(); mit != R.end(); mit++ )
    {
#define key2str(a) a
      ss << "<" << key2str(mit->first) << "," << key2str(mit->second) << ">"; // 
#undef key2str
    }

    ss << "}";

    //std::cerr << "makeKey -> " << ss.str().size() << " characters\n";

    return getKey(ss.str());
  }
#endif

      
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

