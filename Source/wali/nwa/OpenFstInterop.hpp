#ifndef OPENFST_INTEROP_HPP
#define OPENFST_INTEROP_HPP

#include <fst/fstlib.h>

#include "wali/nwa/NWA.hpp"

namespace wali {
    namespace nwa {

        fst::StdVectorFst
        internal_only_nwa_to_fst(NWARefPtr nwa);

        NWARefPtr
        fst_to_nwa(fst::StdFst const & fst, Key stuck = getKey("[stuck]"));
    }
}

#endif
