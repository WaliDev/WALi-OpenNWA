#ifndef WALI_SHORT_WITNESS_VISITOR_GUARD
#define WALI_SHORT_WITNESS_VISITOR_GUARD 1

/*!
 * @author Rich Joiner
 */

#include <list>

#include "opennwa/NwaFwd.hpp"
#include "opennwa/NestedWord.hpp"
#include "opennwa/WeightGen.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/CalculatingVisitor.hpp"

namespace opennwa {
  namespace query {

    struct WitnessRanker
    {
      // Lower rank = has priority
      virtual unsigned long
      get_rank(wali::witness::Witness * w) const = 0;
    };


    class ShortWitnessVisitor : public wali::witness::CalculatingVisitor<wali::witness::witness_t> {
    private:
      WitnessRanker const * m_ranker;

      unsigned long get_rank(wali::witness::Witness * w) const
      {
        if (m_ranker == NULL) {
          return 0;
        }
        else {
          return m_ranker->get_rank(w);
        }
      }

    public:
      ShortWitnessVisitor(WitnessRanker const * ranker = NULL)
        : m_ranker(ranker)
      {}

      ~ShortWitnessVisitor() {}

      virtual wali::witness::witness_t calculateExtend(wali::witness::WitnessExtend * w, wali::witness::witness_t& left, wali::witness::witness_t& right);
      virtual wali::witness::witness_t calculateCombine(wali::witness::WitnessCombine * w, std::list<wali::witness::witness_t>& children);
      virtual wali::witness::witness_t calculateMerge(wali::witness::WitnessMerge* w, wali::witness::witness_t& callerValue, wali::witness::witness_t& ruleValue, wali::witness::witness_t& calleeValue);
      virtual wali::witness::witness_t calculateRule(wali::witness::WitnessRule * w);
      virtual wali::witness::witness_t calculateTrans(wali::witness::WitnessTrans * w);
    };
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif  // WALI_SHORT_WITNESS_VISITOR_GUARD
