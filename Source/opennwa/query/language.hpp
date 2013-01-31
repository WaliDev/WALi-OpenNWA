#ifndef WALI_NWA_QUERY_LANGUAGE_HPP
#define WALI_NWA_QUERY_LANGUAGE_HPP

#include <map>
#include <list>

#include "opennwa/NwaFwd.hpp"
#include "opennwa/NestedWord.hpp"
#include "opennwa/WeightGen.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/CalculatingVisitor.hpp"

namespace opennwa {
  namespace query {
    namespace details {
      class PathVisitor : public wali::witness::CalculatingVisitor<NestedWord> {
      private:
        /// Holds the NWA associated with the given query. We need this so
        /// that when we know that the trace went from state p to q, we can
        /// look up a symbol on that edge.
        Nwa const & nwa;

        /// Holds the *PDS* state and stack symbol corresponding to the
        /// "first half" of a NWA return transition. See visitRule() for
        /// more details.
        // %%% Fix comment, possibly make this more efficient.
        std::map<wali::Key,wali::Key> states;

      public:

        PathVisitor(Nwa const & orig)
          : nwa(orig)
        {}

        ~PathVisitor() {}

        NestedWord calculateExtend(wali::witness::WitnessExtend * w, NestedWord& left, NestedWord& right);
        NestedWord calculateCombine(wali::witness::WitnessCombine * w, std::list<NestedWord>& children);
        NestedWord calculateMerge(wali::witness::WitnessMerge* w, NestedWord& callerValue, NestedWord& ruleValue, NestedWord& calleeValue);
        NestedWord calculateRule(wali::witness::WitnessRule * w);
        NestedWord calculateTrans(wali::witness::WitnessTrans * w);
      };
        
    } // namespace details


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
      
    extern
    ref_ptr<NestedWord>
    getSomeAcceptedWordWithWeights(Nwa const & nwa, WeightGen const & wg);

    extern
    sem_elem_t
    getWitnessForSomeAcceptedWordWithWeights(Nwa const & nwa, WeightGen const & wg);

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
