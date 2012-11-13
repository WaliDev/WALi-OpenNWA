#include "wali/wfa/DeterminizeWeightGen.hpp"
#include "wali/wfa/WFA.hpp"

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
    

  }
}



// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
