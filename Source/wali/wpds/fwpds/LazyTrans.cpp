/*!
 * @author Akash Lal
 * @author Nicholas Kidd
 * $Id$
 */

// ::wali::wfa
#include "wali/wfa/Trans.hpp"

// ::wali::wpds
#include "wali/wpds/Config.hpp"

// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/LazyTrans.hpp"

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
          ) : wfa::DecoratorTrans(new wfa::Trans(from,stack,to,se))
      {
        setConfig(config);
      }

      LazyTrans::LazyTrans( wfa::ITrans* delegate )
        : DecoratorTrans(delegate), intergr(NULL)
      {
      }

      LazyTrans::LazyTrans( wfa::ITrans* delegate, intergraph_t g )
        : DecoratorTrans(delegate)
      {
        setInterGraph(g);
      }

      wfa::ITrans* LazyTrans::copy() const {
        if (intergr.is_valid())
          return new LazyTrans(getDelegate()->copy(),intergr);
        else
          return new LazyTrans(getDelegate()->copy());
      }

      void LazyTrans::compute_weight() const {
        if(!getDelegate()->weight().is_valid()) {
          sem_elem_t val = intergr->get_weight(wali::graph::Transition(*this));
          // This cast is b/c of the const qualifier
          // The const qualifier is necessary b/c it is called
          // by other const methods, of which they really should
          // be const. E.g., ITrans::print(...)
          ((LazyTrans*)this)->setWeight(val);
        }
      }

      const sem_elem_t LazyTrans::weight() const throw() {
        compute_weight();
        return getDelegate()->weight();
      }

      sem_elem_t LazyTrans::weight() throw() {
        compute_weight();
        return getDelegate()->weight();
      }

      void LazyTrans::setWeight(sem_elem_t w) {
        intergr = NULL;
        getDelegate()->setWeight(w);
      }

      void LazyTrans::setInterGraph(intergraph_t igr) {
        setWeight(NULL);
        intergr = igr;
      }

      void LazyTrans::combineTrans(wfa::ITrans* tp) {
        compute_weight();
        getDelegate()->combineTrans(tp);
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
