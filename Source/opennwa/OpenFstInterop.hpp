#ifndef OPENFST_INTEROP_HPP
#define OPENFST_INTEROP_HPP

#include <fst/fstlib.h>

#include "opennwa/Nwa.hpp"

namespace opennwa {

  /// Structure for holding Wali keys. (Introduced to increase type
  /// safety between Wali and OpenFST keys, to decrease the chance
  /// of me making a mistake.)
  struct WaliKey {
    Key key;

    explicit WaliKey(Key k) : key(k) {}
    bool operator== (WaliKey rhs) const { return key == rhs.key; }
    bool operator<  (WaliKey rhs) const { return key <  rhs.key; }
  };

  /// Structure for holding OpenFST keys. (Introduced to increase type
  /// safety between Wali and OpenFST keys, to decrease the chance
  /// of me making a mistake.)
  struct FstKey {
    fst::StdArc::StateId key;

    explicit FstKey(Key k) : key(k) {}
    bool operator== (FstKey rhs) const { return key == rhs.key; }
    bool operator<  (FstKey rhs) const { return key <  rhs.key; }
  };


  /// Maps a known OpenFST key to the corresponding Wali key.
  ///
  typedef std::map<FstKey, WaliKey> fst_to_wali_key_map;
  /// Maps a known Wali key to the corresponding OpenFST key.
  ///
  typedef std::map<WaliKey, FstKey> wali_to_fst_key_map;
  /// Holds bidirectional mappings between Wali and OpenFST keys, to
  /// allow bidirectional lookup.
  typedef std::pair<fst_to_wali_key_map, wali_to_fst_key_map> fst_wali_key_maps;


  /// Given an NWA 'nwa' with no call or return transitions,
  /// converts 'nwa' to an OpenFST acceptor. If 'fst_wali_key_maps'
  /// is non-null, stores the mapping between Wali and OpenFST keys
  /// (state IDs) at that location.
  fst::StdVectorFst
  internal_only_nwa_to_fst(NwaRefPtr nwa, fst_wali_key_maps * maps = NULL);


  /// Converts an OpenFST acceptor to an NWA. 'maps' could specify
  /// the mapping to use for the conversion, but the code is broken
  /// right now; instead, it is just an output parameter.
  /// 'node_prefix' is prepended to the OpenFST key of FST states,
  /// and provdies a way to make, say, state 1 of two different FSTs
  /// produce different Wali states.
  NwaRefPtr
  fst_to_nwa(fst::StdExpandedFst const & fst, fst_wali_key_maps & maps, std::string node_prefix = "");

  /// Given an NWA 'internal_nwa' without call or return
  /// transitions, treat the NWA as a standard FA and minimize
  /// it. (It does this by converting it to an OpenFST acceptor,
  /// minimizing that, then converting it back.)
  NwaRefPtr
  minimize_internal_nwa(NwaRefPtr internal_nwa, std::string node_prefix = "");

  /// Given an NWA 'internal_nwa' without call or return transitions, treat
  /// the NWA as a standard FA and determinize it using OpenFST.  (It does
  /// this by converting it to an OpenFST acceptor, minimizing that, then
  /// converting it back.)
  NwaRefPtr
  determinize_internal_nwa(NwaRefPtr internal_nwa, std::string node_prefix = "");

}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
