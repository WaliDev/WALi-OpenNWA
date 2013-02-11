#ifndef WALI_WFA_DETERMINIZE_WEIGHT_GEN_HPP
#define WALI_WFA_DETERMINIZE_WEIGHT_GEN_HPP

#include "wali/Key.hpp"
#include "wali/SemElem.hpp"

#include <set>
#include <map>

namespace wali {
  namespace wfa {

    class WFA;
    class ITrans;

    class DeterminizeWeightGen
    {
    public:
      typedef std::map<Key, sem_elem_t> AccessibleStateMap;

      /// For each state 's' in 'source' and each 't' in 'target', the
      /// computed weight spec will hold the net weight from 's' to 't'
      /// including the first transition for the symbol in question and any
      /// epsilon paths.
      typedef std::map<Key, AccessibleStateMap> ComputedWeights;
      
      
      virtual ~DeterminizeWeightGen() {}

      /// Returns the weight for the transition from the source to target
      /// state, given as the set of states in the nondeterministic automaton
      /// that correspond. Both the original and partial determinized
      /// automata are provided.
      virtual sem_elem_t getWeight(WFA const & original_wfa,
                                   WFA const & determinized_wfa_so_far,
                                   ComputedWeights const & weight_spec,
                                   std::set<Key> const & source,
                                   Key symbol,
                                   std::set<Key> const & target) const
      = 0;

      virtual sem_elem_t getAcceptWeight(WFA const & original_wfa,
                                         WFA const & determinized_wfa_so_far,
                                         std::set<Key> const & cell) const
      = 0;
    };


    /// Provides a class that computes the weight for a determinizied
    /// transition as follows. Given a transition from S to T (both sets of
    /// states in the nondeterministic WFA), it looks at all edges (in the
    /// nondeterministic machine) from any state in S to any state in T with
    /// the appropriate symbol, collects up all those weights, calls
    /// 'liftWeight' with each of them, and then 'combines' the result.
    class LiftCombineWeightGen
      : public DeterminizeWeightGen
    {
      /// Overload to return the lifted weight for a nondeterministic
      /// transition.
      virtual sem_elem_t liftWeight(WFA const & original_wfa,
                                    Key source,
                                    Key symbol,
                                    Key target,
                                    sem_elem_t weight) const
      = 0;

      virtual sem_elem_t liftAcceptWeight(WFA const & original_wfa,
                                          Key state,
                                          sem_elem_t original_accept_weight) const
      = 0;


      sem_elem_t getWeight(WFA const & original_wfa,
                           WFA const & UNUSED_PARAMETER(determinized_wfa_so_far),
                           ComputedWeights const & weight_spec,
                           std::set<Key> const & sources,
                           Key symbol,
                           std::set<Key> const & targets) const;

      virtual sem_elem_t getAcceptWeight(WFA const & original_wfa,
                                         WFA const & determinized_wfa_so_far,
                                         std::set<Key> const & cell) const;
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
                                   ComputedWeights const & UNUSED_PARAMETER(weight_spec),
                                   std::set<Key> const & UNUSED_PARAMETER(source),
                                   Key UNUSED_PARAMETER(symbol),
                                   std::set<Key> const & UNUSED_PARAMETER(target)) const
      {
        return one;
      }

      virtual sem_elem_t getAcceptWeight(WFA const & UNUSED_PARAMETER(original_wfa),
                                         WFA const & UNUSED_PARAMETER(determinized_wfa_so_far),
                                         std::set<Key> const & UNUSED_PARAMETER(cell)) const
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
