#include "wali/wfa/DeterminizeWeightGen.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/domains/KeyedSemElemSet.hpp"

using wali::domains::KeyedSemElemSet;
using wali::domains::PositionKey;

namespace wali
{
  namespace wfa
  {

    sem_elem_t
    LiftCombineWeightGen::getWeight(WFA const & original_wfa,
                                    WFA const & UNUSED_PARAMETER(determinized_wfa_so_far),
                                    ComputedWeights const & weight_spec,
                                    std::set<Key> const & sources,
                                    Key symbol,
                                    std::set<Key> const & targets) const
    {
      sem_elem_t total_weight = NULL;
      
      for (std::set<Key>::const_iterator source = sources.begin();
           source != sources.end(); ++source)
      {
        for (std::set<Key>::const_iterator target = targets.begin();
             target != targets.end(); ++target)
        {
          ComputedWeights::const_iterator weights_from_source = weight_spec.find(*source);
          if (weights_from_source == weight_spec.end()) {
            continue;
          }
          AccessibleStateMap::const_iterator weight_to_target =
            weights_from_source->second.find(*target);
          if (weight_to_target == weights_from_source->second.end()) {
            continue;
          }

          sem_elem_t orig_weight = weight_to_target->second;
          sem_elem_t lifted = liftWeight(original_wfa, *source, symbol, *target, orig_weight);
            
          if (total_weight == NULL) {
            total_weight = lifted->zero();
          }
            
          total_weight = total_weight->combine(lifted);

        } // for each target state
      } // for each source state

      assert(total_weight != NULL);
      return total_weight;
    }


    sem_elem_t
    LiftCombineWeightGen::getAcceptWeight(WFA const & original_wfa,
                                          WFA const & UNUSED_PARAMETER(determinized_wfa_so_far),
                                          std::set<Key> const & cell) const
    {
      sem_elem_t total_weight = NULL;
      
      for (std::set<Key>::const_iterator state = cell.begin();
           state != cell.end(); ++state)
      {
        sem_elem_t orig_accept_weight = original_wfa.getState(*state)->acceptWeight();
        sem_elem_t lifted = liftAcceptWeight(original_wfa, *state, orig_accept_weight);

        if (total_weight == NULL) {
          total_weight = lifted->zero();
        }

        total_weight = total_weight->combine(lifted);
      }

      assert(total_weight != NULL);
      return total_weight;
    }
    

    ////////////////////////////////////

    sem_elem_t
    CreateKeyedSet::getWeight(WFA const & original_wfa,
                              WFA const & UNUSED_PARAMETER(determinized_wfa_so_far),
                              ComputedWeights const & weight_spec,
                              std::set<Key> const & sources,
                              Key symbol,
                              std::set<Key> const & targets) const
    {
      KeyedSemElemSet::BackingMap map;
      
      for (std::set<Key>::const_iterator source = sources.begin();
           source != sources.end(); ++source)
      {
        for (std::set<Key>::const_iterator target = targets.begin();
             target != targets.end(); ++target)
        {
          ComputedWeights::const_iterator weights_from_source = weight_spec.find(*source);
          if (weights_from_source == weight_spec.end()) {
            continue;
          }
          AccessibleStateMap::const_iterator weight_to_target =
            weights_from_source->second.find(*target);
          if (weight_to_target == weights_from_source->second.end()) {
            continue;
          }

          sem_elem_t weight = weight_to_target->second;
          sem_elem_t guard = new PositionKey<Key>(*source, *target);
          map[guard].insert(weight);
        } // for each target state
      } // for each source state

      assert(map.size() > 0u);
      return new KeyedSemElemSet(map);
    }


    sem_elem_t
    CreateKeyedSet::getAcceptWeight(WFA const & original_wfa,
                                          WFA const & UNUSED_PARAMETER(determinized_wfa_so_far),
                                          std::set<Key> const & cell) const
    {
      KeyedSemElemSet::BackingMap map;
      
      for (std::set<Key>::const_iterator state = cell.begin();
           state != cell.end(); ++state)
      {
        sem_elem_t accept_weight = original_wfa.getState(*state)->acceptWeight();
        sem_elem_t guard = new PositionKey<Key>(*state, *state);
        map[guard].insert(accept_weight);
      }

      assert(map.size() > 0u);
      return new KeyedSemElemSet(map);
    }
    
    

  }
}



// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
