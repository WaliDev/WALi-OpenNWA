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
        sem_elem_t WitnessWrapper::wrap( wfa::Trans& t )
        {
            return new WitnessTrans(t);
        }

        sem_elem_t WitnessWrapper::wrap( wpds::Rule& r )
        {
            return new WitnessRule(r);
        }

        wpds::Wrapper::merge_fn_t WitnessWrapper::wrap( merge_fn_t user_merge )
        {
            if( user_merge.is_valid() ) {
                return new WitnessMergeFn(user_merge);
            }
            else
                return 0;
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

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/
