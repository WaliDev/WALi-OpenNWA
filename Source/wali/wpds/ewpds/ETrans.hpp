#ifndef wali_wpds_ewpds_ETRANS_GUARD
#define wali_wpds_ewpds_ETRANS_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/wfa/Trans.hpp"
#include "wali/wpds/ewpds/MergeFunction.hpp"

namespace wali {
  namespace wpds {
    namespace ewpds {
      class ETrans : public ::wali::wfa::Trans {

        public:
          ETrans(
              Key from, Key stack, Key to,
              sem_elem_t wAtCall, //!< Weight on path to the call transition
              sem_elem_t wAfterCall, //!< For call rule R, wAtCall->extend(R->weight())
              merge_fn_t mf //!< The merge function
              );

          virtual ~ETrans();

          virtual Trans* copy() const;

          /*!
           * Override to apply the merge function, i.e.,
           * 
           *   >>>   mf->apply_f(wAtCall,se)
           */
          virtual sem_elem_t poststar_eps_closure( sem_elem_t se );

          /*!
           * Override Trans::combineTrans to recover
           * the new value for wAtCall.
           *
           * Note: Trans* tp should <b>always</b> be an ETrans.
           */
          virtual void combineTrans( Trans* tp );

        protected:
          sem_elem_t wAtCall;
          merge_fn_t mf;

      }; // class ETrans
    } // namespace ewpds
  } // namespace wpds
} // namespace wali
#endif  // wali_wpds_ewpds_ETRANS_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 2 ***
   ;;; End: ***
*/

