#ifndef wali_wpds_LAZY_TRANS_GUARD
#define wali_wpds_LAZY_TRANS_GUARD 1

/*!
 * @author Akash Lal
 * @author Nicholas Kidd
 *
 */

#include "wali/Common.hpp"

#include "wali/wfa/DecoratorTrans.hpp"

#include "wali/graph/InterGraph.hpp"

#include "wali/wpds/ewpds/ETrans.hpp"

struct FWPDSSourceFunctor;

namespace wali
{
  namespace wpds
  {
    class WPDS;
    class Config;

    namespace fwpds
    {
      class FWPDS;

      class LazyTrans : public wfa::DecoratorTrans
      {
        public:
          friend class WPDS;

        public:
          LazyTrans(  wali_key_t from,
              wali_key_t stack,
              wali_key_t to,
              sem_elem_t  se,
              Config *config
              );

          /*! TODO comments */
          LazyTrans( wfa::ITrans* delegate );

          /*! TODO comments */
          LazyTrans( wfa::ITrans* delegate, graph::InterGraphPtr igr );

          virtual ~LazyTrans();

          virtual wali::wpds::ewpds::ETrans *getETrans();

          virtual wfa::ITrans* copy() const;
          virtual wfa::ITrans* copy(Key f, Key s, Key t) const;

          virtual const sem_elem_t weight() const throw();

          virtual sem_elem_t weight() throw();              

          virtual void setWeight(sem_elem_t w);

          virtual void combineTrans( wfa::ITrans* tp );

          virtual TaggedWeight apply_post( TaggedWeight tw) const;
          virtual TaggedWeight apply_pre( TaggedWeight tw) const;
          virtual void applyWeightChanger(util::WeightChanger &wc);

          void setInterGraph(graph::InterGraphPtr igr);

          virtual std::ostream &print(std::ostream &o) const;

        private:
          void compute_weight() const;

        private:
          //is the delegate an ETrans?
          bool is_etrans; 

          graph::InterGraphPtr intergr;
      };
    }

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_LAZY_TRANS_GUARD

