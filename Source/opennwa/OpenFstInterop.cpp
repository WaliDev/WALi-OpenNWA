#include "OpenFstInterop.hpp"

#include <fst/fstlib.h>

#include <sstream>


using std::set;
using fst::StdVectorFst;
using fst::StdArc;
using fst::StateIterator;
using fst::ArcIterator;
using fst::StdFst;
using fst::StdExpandedFst;
using fst::StdMutableFst;

namespace opennwa {
        
  Key
  get_wali_key(FstKey fstkey, fst_wali_key_maps const & maps)
  {
    assert(maps.first.find(fstkey) != maps.first.end());
    return maps.first.find(fstkey)->second.key;
  }

  StdArc::StateId
  get_fst_key(WaliKey walikey, fst_wali_key_maps const & maps)
  {
    assert(maps.second.find(walikey) != maps.second.end());
    return maps.second.find(walikey)->second.key;
  }

  void
  add_fst_state(StdMutableFst & fst, WaliKey walikey, fst_wali_key_maps & maps)
  {
    assert(maps.second.find(walikey) == maps.second.end());

    // Get a new key
    FstKey fstkey(fst.AddState());

    // Now add it to the maps
    maps.first.insert(std::make_pair(fstkey, walikey));
    maps.second.insert(std::make_pair(walikey, fstkey));
  }

           
  StdVectorFst
  internal_only_nwa_to_fst(NwaRefPtr nwa, fst_wali_key_maps * maps)
  {
    assert(EPSILON == 0);
            
    StdVectorFst retFst;
            
    if (maps == NULL) {
      maps = new fst_wali_key_maps();
    }

    // Add states to the FST
    //
    // We have to set up a mapping for each state from the Wali key
    // to the OpenFST key. That's what this loop does (rather than do
    // it on-the-fly; this way gives me more opportunities to write
    // assert(...)).
    set<State> const & states = nwa->getStates();
    for (set<State>::const_iterator state = states.begin();
         state != states.end(); ++state)
    {
      add_fst_state(retFst, WaliKey(*state), *maps);
    }

    // Set the initial state of the FST
    assert(nwa->sizeInitialStates() == 1);
    set<State> const & initials = nwa->getInitialStates();
    for (set<State>::const_iterator initial = initials.begin();
         initial != initials.end(); ++initial)
    {
      WaliKey wkey(*initial);
      retFst.SetStart(get_fst_key(wkey, *maps));
    }


    // Set the accepting states of the FST
    //assert(nwa->sizeFinalStates() == 1);
    set<State> const & finals = nwa->getFinalStates();
    for (set<State>::const_iterator final = finals.begin();
         final != finals.end(); ++final)
    {
      WaliKey wkey(*final);
      retFst.SetFinal(get_fst_key(wkey, *maps),
                      StdArc::Weight::One());
    }
            

    //Internal Transitions
    for (Nwa::InternalIterator iit = nwa->beginInternalTrans();
         iit != nwa->endInternalTrans(); iit++)
    {
      WaliKey w_start(iit->first);
      WaliKey w_target(iit->third);

      StdArc::StateId fst_start = get_fst_key(w_start, *maps);
      StdArc::StateId fst_target = get_fst_key(w_target, *maps);
                
      retFst.AddArc(fst_start,            // start state
                    StdArc(iit->second,   // input symbol
                           iit->second,   // output symbol (ignored for acceptor)
                           0,             // weight
                           fst_target));  // destination state
    }

    //Call Transitions
    for (Nwa::CallIterator iit = nwa->beginCallTrans();
         iit != nwa->endCallTrans(); iit++)
    {
      assert(false && "Trying to convert an NWA with call transitions to a FSM");
    }

    //Return Transitions
    for (Nwa::ReturnIterator iit = nwa->beginReturnTrans();
         iit != nwa->endReturnTrans(); iit++)
    {
      assert(false && "Trying to convert an NWA with return transitions to a FSM");
    }

    return retFst;
  }


  NwaRefPtr
  fst_to_nwa(StdExpandedFst const & fst,
             fst_wali_key_maps & maps,
             std::string node_prefix)
  {
    // TODO: Because of fun stuff, maps no longer corresponds to the
    // fst. This is not appropriate for the final version: it should
    // provide an option to do this (with a NULL ptr), but not do it
    // by default. -Evan 3/9/11
    maps.first.clear();
    maps.second.clear();
    static int generation = 0;
    for(StdFst::StateId i=0 ; i<fst.NumStates() ; ++i) {
      FstKey fst_key(i);
      std::stringstream ss;
      ss << "__fst_nwa_" << ++generation << "__" << node_prefix << i;
      WaliKey wali_key(getKey(ss.str()));

      maps.first.insert(std::make_pair(fst_key, wali_key));
    }               
            

            
    assert(EPSILON == 0);
            
    NwaRefPtr nwa = new Nwa();
            
    typedef StateIterator<StdFst> StateIter;
    typedef ArcIterator<StdFst> ArcIter;

    // We need to add initial states, final states, and transitions.

    FstKey fst_start(fst.Start());
    nwa->addInitialState(get_wali_key(fst_start, maps));
            
    // OpenFST doesn't let you directly say "give me all
    // transitions"; all you can say is "give me the transitions
    // outgoing from this state." So we have to look at each state in
    // turn, then ask for the transitions.
    //
    // Also, we need to iterate over final states. There's no direct
    // way to do this that I see, so instead, we'd have to iterate
    // over all states, and check if each is final. Since we have to
    // do that anyway, might as well combine those loops.
    for (StateIter state(fst); !state.Done(); state.Next()) {
      FstKey fst_source(state.Value());
      Key source = get_wali_key(fst_source, maps);

      // Add transitions
      for (ArcIter arciter(fst, fst_source.key); !arciter.Done(); arciter.Next()) {
        const StdArc &arc = arciter.Value();
        FstKey target(arc.nextstate);
        Key letter = arc.ilabel;
                    
        // Add 'source -> target'
        nwa->addInternalTrans(source, letter, get_wali_key(target, maps));
      }

      // Add finalness. Aditya says (CashFST.h) "Get state i's
      // final weight; if == Weight::Zero() => non-final" (and I
      // think I maintain that invariant too).
      if (fst.Final(fst_source.key) != StdArc::Weight::Zero()) {
        nwa->addFinalState(source);
      }
    }

    return nwa;
  }



  // Converts the given Nwa (which must have only internal transitions)
  // to an OpenFST acceptor, minimizes that acceptor, then converts it
  // back. (Unfortunately, this will destroy the state names. C'est la
  // vie.)
  NwaRefPtr
  minimize_internal_nwa(NwaRefPtr internal_nwa,
                        std::string node_prefix)
  {
    // Convert to a FSM
    fst_wali_key_maps maps; // this is basically useless at the moment
    StdVectorFst fsm = internal_only_nwa_to_fst(internal_nwa, &maps);
            
    // Minimizing consists of several steps:
    //  o Remove epsilon transitions (during Determinize, epsilon is
    //    treated as any other letter)
    //  o Determinize
    //  o Minimize
    // None of these steps are automatic, and there are a couple
    // styles of interfaces these functions have.
    fst::RmEpsilon(&fsm);
            
    fst::StdVectorFst det_fsm; // make a new one because of bad docs in openfst
    fst::Determinize(fsm, &det_fsm);
            
    fst::Minimize(&det_fsm);
            
    // Now convert back
    NwaRefPtr min_nwa = fst_to_nwa(det_fsm, maps, node_prefix);
            
    // This could be an expensive check, but hopefully not...
    //assert (Nwa::equal(internal_nwa, min_nwa));
            
    return min_nwa;
  }

  // Converts the given Nwa (which must have only internal transitions)
  // to an OpenFST acceptor, minimizes that acceptor, then converts it
  // back. (Unfortunately, this will destroy the state names. C'est la
  // vie.)
  NwaRefPtr
  determinize_internal_nwa(NwaRefPtr internal_nwa,
                           std::string node_prefix)
  {
    // Convert to a FSM
    fst_wali_key_maps maps; // this is basically useless at the moment
    StdVectorFst fsm = internal_only_nwa_to_fst(internal_nwa, &maps);
            
    // Minimizing consists of several steps:
    //  o Remove epsilon transitions (during Determinize, epsilon is
    //    treated as any other letter)
    //  o Determinize
    //  o Minimize
    // None of these steps are automatic, and there are a couple
    // styles of interfaces these functions have.
    fst::RmEpsilon(&fsm);
            
    fst::StdVectorFst det_fsm; // make a new one because of bad docs in openfst
    fst::Determinize(fsm, &det_fsm);
            
    // Now convert back
    NwaRefPtr min_nwa = fst_to_nwa(det_fsm, maps, node_prefix);
            
    // This could be an expensive check, but hopefully not...
    //assert (Nwa::equal(internal_nwa, min_nwa));

    if (internal_nwa->sizeSymbols() != min_nwa->sizeSymbols()) {
      for (Nwa::SymbolIterator sym = internal_nwa->beginSymbols();
           sym != internal_nwa->endSymbols(); ++sym)
      {
        min_nwa->addSymbol(*sym);
      }
    }

    min_nwa->realizeImplicitTrans(getKey("{}"));
            
    return min_nwa;
  }


}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
