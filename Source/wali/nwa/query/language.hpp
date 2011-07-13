#ifndef WALI_NWA_LANGUAGE_HPP
#define WALI_NWA_LANGUAGE_HPP WALI_NWA_LANGUAGE_HPP

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/NestedWord.hpp"

namespace wali {
  namespace nwa {
    namespace query {

      inline
      bool
      languageContains(NWA const & nwa, NestedWord const & word)
      {
        return nwa.isMemberNondet(word);
      }

    }
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
