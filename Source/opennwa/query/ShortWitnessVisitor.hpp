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


    class ShortWitnessVisitor
        : public wali::witness::Visitor
    {
    private:
      WitnessRanker const * m_ranker;
      wali::witness::witness_t m_answer;

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
      ShortWitnessVisitor(
          WitnessRanker const * ranker = NULL)
        : m_ranker(ranker)
      {}

      ~ShortWitnessVisitor() {}

      wali::witness::witness_t answer() const {
        return m_answer;
      }

      virtual bool visit(wali::witness::Witness * w);
      virtual bool visitExtend(wali::witness::WitnessExtend * w);
      virtual bool visitCombine(wali::witness::WitnessCombine * w);
      virtual bool visitRule(wali::witness::WitnessRule * w);
      virtual bool visitTrans(wali::witness::WitnessTrans * w);
      virtual bool visitMerge(wali::witness::WitnessMerge * w);

    };
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif  // WALI_SHORT_WITNESS_VISITOR_GUARD
