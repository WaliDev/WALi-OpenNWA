
#include "opennwa/query/ShortWitnessVisitor.hpp"

#include <iostream>
using std::cerr;

namespace opennwa {

  namespace query {

    using namespace wali;
    using namespace wali::witness;

    witness_t ShortWitnessVisitor::calculateExtend(WitnessExtend * w, witness_t& left, witness_t& right) {
      (void) w;
      // Extend together the trimmed version of this extend node's
      // original children.
      return left->extend(right.get_ptr());
    }

    // Chooses the shortest child.
    witness_t ShortWitnessVisitor::calculateCombine(WitnessCombine * w, std::list<witness_t>& children) {
      (void) w;

      // Track the minimum length of a child.
      unsigned long min_length = ULONG_MAX;
      unsigned long min_rank = ULONG_MAX;
      witness_t ret;

      std::list<witness_t>::const_iterator wit;
      for (wit=children.begin(); wit!=children.end(); wit++) {
        witness_t ptr = *wit;
        assert (ptr.is_valid() && "Invalid combine child pointer");
        unsigned long len = ptr->getMinimumLength();
        unsigned long rank = get_rank(ptr.get_ptr());

        if (rank < min_rank
            ||
            (rank == min_rank && len < min_length))
        {
          min_length = len;
          min_rank = rank;
          ret = ptr;
        }
      }

      assert (ret.is_valid() && "Invalid result of combine");
      
      return ret;
    }

    // Not implemented.
    witness_t ShortWitnessVisitor::calculateMerge(WitnessMerge* w, witness_t& callerValue, witness_t& ruleValue, witness_t& calleeValue) {
      (void) w;
      (void) callerValue;
      (void) calleeValue;

      assert (0 && "Merge not implemented!");
      return ruleValue;
    }

    witness_t ShortWitnessVisitor::calculateRule(WitnessRule * w) {
      return w;
    }

    witness_t ShortWitnessVisitor::calculateTrans(WitnessTrans * w) {
      return w;
    }

  }

}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

