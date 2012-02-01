#ifndef WALI_NWA_QUERY_LANGUAGE_HPP
#define WALI_NWA_QUERY_LANGUAGE_HPP

#include "opennwa/NwaFwd.hpp"
#include "opennwa/NestedWord.hpp"

namespace opennwa {
  namespace query {

    /// @brief Determines whether word is in the language of the given NWA.
    ///
    /// @returns true if 'word' is in L(this), and false otherwise.
    bool
    languageContains(Nwa const & nwa, NestedWord const & word);


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
    languageSubsetEq(Nwa const & left, Nwa const & right);


    /**
     *
     * @brief tests whether the language accepted by this NWA is empty
     *
     * This method tests whether the language accepted by this NWA is empty.
     *
     * @return true if the language accepted by this NWA is empty
     *
     */
    bool
    languageIsEmpty(Nwa const & nwa);


    /**
     *
     * @brief Returns some word accepted by 'nwa', or NULL if there isn't one.
     *
     * @return A word accepted by 'nwa', or NULL if there isn't one
     *
     */
    extern
    ref_ptr<NestedWord>
    getSomeAcceptedWord(Nwa const & nwa);

    extern
    ref_ptr<NestedWord>
    getSomeShortestAcceptedWord(Nwa const & nwa);
      

    /**
     *
     * @brief tests whether the languages of the given NWAs are equal
     *
     * This method tests the equivalence of the languages accepted by the given NWAs.
     *
     * @param - first: one of the NWAs whose language to test
     * @param - second: one of the NWAs whose language to test
     * @return true if the languages accepted by the given NWAs are equal, false otherwise
     *
     */
    bool
    languageEquals(Nwa const & first, Nwa const & second);
      
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
