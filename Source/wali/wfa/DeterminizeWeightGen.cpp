#include "wali/wfa/DeterminizeWeightGen.hpp"
#include "wali/wfa/WFA.hpp"

namespace wali
{
  namespace wfa
  {

    sem_elem_t
    LiftCombineWeightGen::getWeight(WFA const & original_wfa,
                                    WFA const & UNUSED_PARAMETER(determinized_wfa_so_far),
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
          Trans trans;
          if (original_wfa.find(*source, symbol, *target, trans)) {
            sem_elem_t lifted = liftWeight(original_wfa, &trans);
            
            if (total_weight == NULL) {
              total_weight = lifted->zero();
            }
            
            total_weight = total_weight->combine(lifted);
          }
        } // for each target state
      } // for each source state
      
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
