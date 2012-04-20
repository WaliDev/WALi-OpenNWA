#ifndef WALI_WFA_DETERMINIZE_WEIGHT_GEN_HPP
#define WALI_WFA_DETERMINIZE_WEIGHT_GEN_HPP

#include "wali/Key.hpp"
#include "wali/SemElem.hpp"

#include <set>

namespace wali {
  namespace wfa {

    class WFA;

    struct DeterminizeWeightGen
    {
      virtual ~DeterminizeWeightGen() {}

      /// Returns the weight for the transition from the source to target
      /// state, given as the set of states in the nondeterministic automaton
      /// that correspond. Both the original and partial determinized
      /// automata are provided.
      virtual sem_elem_t getWeight(WFA const & original_wfa,
                                   WFA const & determinized_wfa_so_far,
                                   std::set<Key> const & source,
                                   Key symbol,
                                   std::set<Key> const & target)
        = 0;
    };


    class AlwaysReturnOneWeightGen
      : public DeterminizeWeightGen
    {
      sem_elem_t const one;

    public:
      AlwaysReturnOneWeightGen(sem_elem_t some_weight)
        : one(some_weight->one())
      {}

      virtual sem_elem_t getWeight(WFA const & UNUSED_PARAMETER(original_wfa),
                                   WFA const & UNUSED_PARAMETER(determinized_wfa_so_far),
                                   std::set<Key> const & UNUSED_PARAMETER(source),
                                   Key UNUSED_PARAMETER(symbol),
                                   std::set<Key> const & UNUSED_PARAMETER(target))
      {
        return one;
      }
    };

  }
}

// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:

#endif
