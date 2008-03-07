#ifndef wali_wpds_LAZY_TRANS_GUARD
#define wali_wpds_LAZY_TRANS_GUARD 1

/*!
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/wpds/LinkedTrans.hpp"
#include "wali/graph/InterGraph.hpp"

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

            class LazyTrans : public wali::wpds::LinkedTrans
            {
              public:
                friend class WPDS;
                friend class FWPDS;

              public:
                LazyTrans(  wali_key_t from,
                            wali_key_t stack,
                            wali_key_t to,
                            sem_elem_t  se,
                            Config *config
                            );
                LazyTrans(  wali_key_t from,
                            wali_key_t stack,
                            wali_key_t to,
                            intergraph_t igr,
                            Config *config
                            );
              
                virtual ~LazyTrans();

                const sem_elem_t weight() const throw();
                sem_elem_t weight() throw();              
                void setWeight(sem_elem_t w);
                void combine_weight(sem_elem_t w);
                void setInterGraph(intergraph_t igr);

              private:
                void compute_weight() const;

              private:
                mutable intergraph_t intergr;
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
