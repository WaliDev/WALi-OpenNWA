/*!
 * @author Akash Lal
 */

#include "wali/wpds/fwpds/LazyTrans.hpp"
#include "wali/wpds/Config.hpp"

namespace wali
{
    namespace wpds
    {
        namespace fwpds
        {

          LazyTrans::LazyTrans(  wali_key_t from,
                                 wali_key_t stack,
                                 wali_key_t to,
                                 sem_elem_t se,
                                 Config *config
                                 ) : LinkedTrans(from,stack,to,se,config) {}

          LazyTrans::LazyTrans(  wali_key_t from,
                                 wali_key_t stack,
                                 wali_key_t to,
                                 intergraph_t igr,
                                 Config *config
                                 ) : LinkedTrans(from,stack,to,NULL,config), intergr(igr) {}

          void LazyTrans::compute_weight() const {
            if(!se.is_valid()) {
              se = intergr->get_weight(wali::graph::Transition(*this));
              intergr = NULL;
            }
          }

          const sem_elem_t LazyTrans::weight() const throw() {
            compute_weight();
            return Trans::weight();
          }

          sem_elem_t LazyTrans::weight() throw() {
            compute_weight();
            return Trans::weight();
          }

          void LazyTrans::setWeight(sem_elem_t w) {
            intergr = NULL;
            Trans::setWeight(w);
          }

          void LazyTrans::setInterGraph(intergraph_t igr) {
            se = NULL;
            intergr = igr;
          }

          void LazyTrans::combine_weight(sem_elem_t wnew) {
            compute_weight();
            Trans::combine_weight(wnew);
          }

          LazyTrans::~LazyTrans() {}

        } // namespace fwpds

    } // namespace wpds

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
