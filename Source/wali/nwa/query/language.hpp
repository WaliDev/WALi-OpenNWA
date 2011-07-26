#ifndef WALI_NWA_QUERY_LANGUAGE_HPP
#define WALI_NWA_QUERY_LANGUAGE_HPP

#include "wali/nwa/NWAFwd.hpp"
#include "wali/nwa/NestedWord.hpp"

namespace wali {
  namespace nwa {
    namespace query {

      /// @brief Determines whether word is in the language of the given NWA.
      ///
      /// @returns true if 'word' is in L(this), and false otherwise.
      bool
      languageContains(NWA const & nwa, NestedWord const & word);


      /**
       * @brief tests whether the language of the first NWA is included in the language of 
       *        the second NWA
       *
       * This method tests whether the language of the first NWA is included in the language
       * of the second NWA.
       *
       * @param - first: the proposed subset
       * @param - second: the proposed superset
       * @return true if the language of the first NWA is included in the language of the 
       *          second NWA, false otherwise
       *
       */
      bool
      languageSubsetEq(NWA const & left, NWA const & right);

    }
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
