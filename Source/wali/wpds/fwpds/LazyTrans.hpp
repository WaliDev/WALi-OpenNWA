#ifndef wali_wpds_LAZY_TRANS_GUARD
#define wali_wpds_LAZY_TRANS_GUARD 1

/*!
 * @author Akash Lal
 * @author Nicholas Kidd
 *
 * $Id$
 */

#include "wali/Common.hpp"

#include "wali/wfa/DecoratorTrans.hpp"

#include "wali/graph/InterGraph.hpp"

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
      typedef ref_ptr<wali::graph::InterGraph> intergraph_t;


      class LazyTrans : public wfa::DecoratorTrans
      {
        public:
          friend class WPDS;
          friend class FWPDS;
          friend struct FWPDSSourceFunctor;
 
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
          LazyTrans( wfa::ITrans* delegate, intergraph_t igr );

          virtual ~LazyTrans();

          virtual wfa::ITrans* copy() const;

          virtual const sem_elem_t weight() const throw();

          virtual sem_elem_t weight() throw();              

          virtual void setWeight(sem_elem_t w);

          virtual void combineTrans( wfa::ITrans* tp );

          void setInterGraph(intergraph_t igr);

        private:
          void compute_weight() const;

        private:
          intergraph_t intergr;
      };
    }

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_LAZY_TRANS_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */
