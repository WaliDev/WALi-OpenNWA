#ifndef OPENFST_INTEROP_HPP
#define OPENFST_INTEROP_HPP

#include <fst/fstlib.h>

#include "wali/nwa/NWA.hpp"

namespace wali {
    namespace nwa {

        struct WaliKey {
            Key key;

            explicit WaliKey(Key k) : key(k) {}
            bool operator== (WaliKey rhs) const { return key == rhs.key; }
            bool operator<  (WaliKey rhs) const { return key <  rhs.key; }
        };

        struct FstKey {
            fst::StdArc::StateId key;

            explicit FstKey(Key k) : key(k) {}
            bool operator== (FstKey rhs) const { return key == rhs.key; }
            bool operator<  (FstKey rhs) const { return key <  rhs.key; }
        };

        typedef std::map<FstKey, WaliKey> fst_to_wali_key_map;
        typedef std::map<WaliKey, FstKey> wali_to_fst_key_map;
        typedef std::pair<fst_to_wali_key_map, wali_to_fst_key_map> fst_wali_key_maps;


        fst::StdVectorFst
        internal_only_nwa_to_fst(NWARefPtr nwa, fst_wali_key_maps * maps = NULL);

        NWARefPtr
        fst_to_nwa(fst::StdExpandedFst const & fst, Key stuck, fst_wali_key_maps const & maps);
    }
}

#endif
