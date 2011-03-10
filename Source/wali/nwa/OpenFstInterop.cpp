#include "OpenFstInterop.hpp"

#include <fst/fstlib.h>


using std::set;
using fst::StdVectorFst;
using fst::StdArc;
using fst::StateIterator;
using fst::ArcIterator;
using fst::StdFst;
using fst::StdExpandedFst;
using fst::StdMutableFst;

namespace wali {
    namespace nwa {
        
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
        internal_only_nwa_to_fst(NWARefPtr nwa, fst_wali_key_maps * maps)
        {
            assert(NWA::getEpsilon() == 0);
            
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
            set<St> const & states = nwa->getStates();
            for (set<St>::const_iterator state = states.begin();
                 state != states.end(); ++state)
            {
                add_fst_state(retFst, WaliKey(*state), *maps);
            }

            // Set the initial state of the FST
            assert(nwa->sizeInitialStates() == 1);
            set<St> const & initials = nwa->getInitialStates();
            for (set<St>::const_iterator initial = initials.begin();
                 initial != initials.end(); ++initial)
            {
                WaliKey wkey(*initial);
                retFst.SetStart(get_fst_key(wkey, *maps));
            }


            // Set the accepting states of the FST
            assert(nwa->sizeFinalStates() == 1);
            set<St> const & finals = nwa->getFinalStates();
            for (set<St>::const_iterator final = finals.begin();
                 final != finals.end(); ++final)
            {
                WaliKey wkey(*final);
                retFst.SetFinal(get_fst_key(wkey, *maps),
                                StdArc::Weight::One());
            }
            

            //Internal Transitions
            for (NWA::internalIterator iit = nwa->beginInternalTrans();
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
            for (NWA::callIterator iit = nwa->beginCallTrans();
                 iit != nwa->endCallTrans(); iit++)
            {
                assert(false && "Trying to convert an NWA with call transitions to a FSM");
            }

            //Return Transitions
            for (NWA::returnIterator iit = nwa->beginReturnTrans();
                 iit != nwa->endReturnTrans(); iit++)
            {
                assert(false && "Trying to convert an NWA with return transitions to a FSM");
            }

            return retFst;
        }


        NWARefPtr
        fst_to_nwa(StdExpandedFst const & fst, Key stuck, fst_wali_key_maps const & maps)
        {
            assert(NWA::getEpsilon() == 0);
            
            NWARefPtr nwa = new NWA(stuck);
            
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

    }
}
