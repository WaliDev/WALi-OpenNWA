/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/WitnessWrapper.hpp"
#include "wali/witness/WitnessTrans.hpp"
#include "wali/witness/WitnessRule.hpp"
#include "wali/witness/WitnessMergeFn.hpp"

namespace wali
{
  namespace witness
  {
    sem_elem_t WitnessWrapper::wrap( wfa::ITrans& t )
    {
      return new WitnessTrans(t);
    }

    sem_elem_t WitnessWrapper::wrap( wpds::Rule& r )
    {
      return new WitnessRule(r);
    }

    merge_fn_t WitnessWrapper::wrap( merge_fn_t user_merge )
    {
      return new WitnessMergeFn(user_merge);
    }

    sem_elem_t WitnessWrapper::unwrap( sem_elem_t se )
    {
      Witness* wit = dynamic_cast< Witness* >(se.get_ptr());
      if( 0 != wit ) {
        return wit->weight();
      }
      else {
        // TODO: throw an error
        *waliErr << "[ERROR] Unwrap called on non Witness weight.\n";
        assert(0);
        return 0;
      }
    }

  } // namespace witness

} // namespace wali

/*
 * $Id$
 */
