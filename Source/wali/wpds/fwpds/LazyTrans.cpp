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
                    ) : wfa::Trans(from,stack,to,se)
            {
                setConfig(config);
            }

            LazyTrans::LazyTrans(  wali_key_t from,
                    wali_key_t stack,
                    wali_key_t to,
                    intergraph_t igr,
                    Config *config
                    ) : wfa::Trans(from,stack,to,NULL), intergr(igr) 
            {
                setConfig(config);
            }

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

          void LazyTrans::combineTrans(wfa::Trans* tp) {
            compute_weight();
            Trans::combineTrans(tp);
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
