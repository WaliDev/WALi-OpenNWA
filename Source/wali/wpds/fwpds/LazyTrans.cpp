/*!
 * @author Akash Lal
 * @author Nicholas Kidd
 * $Id: LazyTrans.cpp 1099 2011-06-07 22:22:04Z driscoll $
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

      LazyTrans::LazyTrans(  wali_key_t the_from,
          wali_key_t the_stack,
          wali_key_t the_to,
          sem_elem_t se,
          Config *config
          )
        : wfa::DecoratorTrans(new wfa::Trans(the_from,the_stack,the_to,se))
      {
        setConfig(config);
        is_etrans = false;
      }

      LazyTrans::LazyTrans( wfa::ITrans* the_delegate )
        : DecoratorTrans(the_delegate), intergr(NULL)
      {
        ewpds::ETrans *et = dynamic_cast<ewpds::ETrans *>(the_delegate);
        if(et != 0) {
          is_etrans = true;
        } else {
          is_etrans = false;
        }

      }

      LazyTrans::LazyTrans( wfa::ITrans* the_delegate, graph::InterGraphPtr g )
        : DecoratorTrans(the_delegate)
      {
        setInterGraph(g);

        ewpds::ETrans *et = dynamic_cast<ewpds::ETrans *>(the_delegate);
        if(et != 0) {
          is_etrans = true;
        } else {
          is_etrans = false;
        }

      }

      ewpds::ETrans *LazyTrans::getETrans() {
        if(is_etrans) {
          compute_weight();
          return static_cast<ewpds::ETrans *> (getDelegate());
        }
        return 0;
      }

      wfa::ITrans* LazyTrans::copy() const {
        compute_weight();

        return getDelegate()->copy();
      }

      wfa::ITrans* LazyTrans::copy(Key f, Key s, Key t) const {
        compute_weight();

        return getDelegate()->copy(f,s,t);
      }

      void LazyTrans::compute_weight() const {
        if(!getDelegate()->weight().is_valid()) {
          sem_elem_t val = intergr->get_weight(wali::graph::Transition(*this));

          if(is_etrans) {
            // Should also set the weight at call site
            sem_elem_t wt = intergr->get_call_weight(wali::graph::Transition(*this));
            ((ewpds::ETrans *) getDelegate())->setWeightAtCall(wt);
          }

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

      std::ostream &LazyTrans::print(std::ostream &o) const {
        compute_weight();
        return getDelegate()->print(o);
      }

      void LazyTrans::setWeight(sem_elem_t w) {
        if(is_etrans && intergr.is_valid()) {
          // Ensure that 
          // etrans information inside intergr is propagated
          sem_elem_t wt = intergr->get_call_weight(wali::graph::Transition(*this));
          ((ewpds::ETrans *) getDelegate())->setWeightAtCall(wt);
        }
        intergr = NULL;
        getDelegate()->setWeight(w);
      }

      void LazyTrans::setInterGraph(graph::InterGraphPtr igr) {
        setWeight(NULL);
        intergr = igr;
      }

      void LazyTrans::combineTrans(wfa::ITrans* tp) {
        compute_weight();
        getDelegate()->combineTrans(tp);
      }

      TaggedWeight LazyTrans::apply_post( TaggedWeight tw) const {
        compute_weight();
        return getDelegate()->apply_post(tw);
      }

      TaggedWeight LazyTrans::apply_pre( TaggedWeight tw) const {
        compute_weight();
        return getDelegate()->apply_pre(tw);
      }

      void LazyTrans::applyWeightChanger( util::WeightChanger &wc) {
        compute_weight();
        getDelegate()->applyWeightChanger(wc);
      }

      LazyTrans::~LazyTrans() {}

    } // namespace fwpds

  } // namespace wpds

} // namespace wali

