#include <map>
#include <list>

#include "opennwa/NwaFwd.hpp"
#include "opennwa/NestedWord.hpp"
#include "opennwa/WeightGen.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/CalculatingVisitor.hpp"

namespace opennwa {
  namespace query {
    class ShortWitnessVisitor : public wali::witness::CalculatingVisitor<wali::witness::witness_t> {

    public:
      ShortWitnessVisitor() {}

      ~ShortWitnessVisitor() {}

      virtual wali::witness::witness_t calculateExtend(wali::witness::WitnessExtend * w, wali::witness::witness_t& left, wali::witness::witness_t& right);
      virtual wali::witness::witness_t calculateCombine(wali::witness::WitnessCombine * w, std::list<wali::witness::witness_t>& children);
      virtual wali::witness::witness_t calculateMerge(wali::witness::WitnessMerge* w, wali::witness::witness_t& callerValue, wali::witness::witness_t& ruleValue, wali::witness::witness_t& calleeValue);
      virtual wali::witness::witness_t calculateRule(wali::witness::WitnessRule * w);
      virtual wali::witness::witness_t calculateTrans(wali::witness::WitnessTrans * w);
    };
  }
}
