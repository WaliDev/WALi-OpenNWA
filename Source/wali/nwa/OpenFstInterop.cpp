#include "OpenFstInterop.hpp"

#include <fst/fstlib.h>


using std::set;
using fst::StdVectorFst;
using fst::StdArc;
using fst::StateIterator;
using fst::ArcIterator;
using fst::StdFst;
using fst::StdMutableFst;

namespace wali {
    namespace nwa {

        struct WaliKey {
            Key key;

            WaliKey(Key k) : key(k) {}
            bool operator== (WaliKey rhs) const { key == rhs.key; }
            bool operator<  (WaliKey rhs) const { key <  rhs.key; }
        };

        struct FstKey {
            StdArc::StateId key;

            FstKey(Key k) : key(k) {}
            bool operator== (FstKey rhs) const { key == rhs.key; }
            bool operator<  (FstKey rhs) const { key <  rhs.key; }
        };

        typedef std::map<FstKey, WaliKey> fst_to_wali_key_map;
        typedef std::map<WaliKey, FstKey> wali_to_fst_key_map;
        typedef std::pair<fst_to_wali_key_map, wali_to_fst_key_map> fst_wali_key_maps;

        
        WaliKey
        get_wali_key(FstKey fstkey, fst_wali_key_maps const & maps)
        {
            assert(maps.first.find(fstkey) != maps.first.end());
            return maps.first.find(fstkey)->second;
        }

        FstKey
        get_fst_key(WaliKey walikey, fst_wali_key_maps const & maps)
        {
            assert(maps.second.find(walikey) != maps.second.end());
            return maps.second.find(walikey)->second;
        }

        FstKey
        get_new_fst_key(StdMutableFst & fst, WaliKey walikey, fst_wali_key_maps & maps)
        {
            assert(maps.second.find(walikey) != maps.second.end());

            // Get a new key
            FstKey fstkey = fst.AddState();

            // Now add it to the maps
            maps.first.insert(std::make_pair(fstkey, walikey));
            maps.second.insert(std::make_pair(walikey, fstkey));
            
            return fstkey;
        }

           
        StdVectorFst
        internal_only_nwa_to_fst(NWARefPtr nwa)
        {
            StdVectorFst retFst;
            
            StdArc::StateId initial = retFst.AddState();
            StdArc::StateId final   = retFst.AddState();

            assert(nwa->sizeInitialStates() == 1);
            set<St> const & initials = nwa->getInitialStates();
            for (set<St>::const_iterator initial = initials.begin();
                 initial != initials.end(); ++initial)
            {
                retFst.SetStart(*initial);
            }


            assert(nwa->sizeFinalStates() == 1);
            set<St> const & finals = nwa->getFinalStates();
            for (set<St>::const_iterator final = finals.begin();
                 final != finals.end(); ++final)
            {
                cerr << "*final is " << *final << "\n";
                retFst.SetFinal(*final, StdArc::Weight::One());
            }
            

            //Internal Transitions
            for (NWA::internalIterator iit = nwa->beginInternalTrans();
                 iit != nwa->endInternalTrans(); iit++)
            {
                retFst.AddArc(iit->first,           // start state
                              StdArc(iit->second,   // input symbol
                                     iit->second,   // output symbol (ignored for acceptor)
                                     0,             // weight
                                     iit->second)); // destination state
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
        fst_to_nwa(StdFst const & fst, Key stuck)
        {
            NWARefPtr nwa = new NWA(stuck);
            
            typedef StateIterator<StdFst> StateIter;
            typedef ArcIterator<StdFst> ArcIter;

            // We need to add initial states, final states, and transitions.
            
            nwa->addInitialState(fst.Start());
            
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
                Key source = state.Value();

                // Add transitions
                for (ArcIter arciter(fst, source); !arciter.Done(); arciter.Next()) {
                    const StdArc &arc = arciter.Value();
                    Key target = arc.nextstate;
                    Key letter = arc.ilabel;
                    
                    // Add 'source -> target'
                    nwa->addInternalTrans(source, letter, target);
                }

                // Add finalness. Aditya says (CashFST.h) "Get state i's
                // final weight; if == Weight::Zero() => non-final" (and I
                // think I maintain that invariant too).
                if (fst.Final(source) == StdArc::Weight::Zero()) {
                    nwa->addFinalState(source);
                }
            }

            return nwa;
        }

    }
}
