/**
 * @author Nicholas Kidd
 *
 * @version $Id$
 */

#include "wali/Common.hpp"
#include "wali/wpds/ewpds/ERule.hpp"
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

    // NOTE
    //   The weight r->weight() is already a wrapped
    //   witness, so no need to rewrap it here.
    merge_fn_t WitnessWrapper::wrap( 
        wpds::ewpds::ERule& r, 
        merge_fn_t user_merge )
    {
      sem_elem_t se = r.weight();
      WitnessRule* wr = dynamic_cast<WitnessRule*>(se.get_ptr());
      assert (wr != NULL);
      return new WitnessMergeFn(wr, user_merge);
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

