#ifndef wali_witness_WITNESS_MERGE_FN_GUARD
#define wali_witness_WITNESS_MERGE_FN_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include "wali/MergeFn.hpp"
#include "wali/witness/Witness.hpp"

namespace wali 
{
  namespace witness 
  {
    class WitnessMergeFn;

    typedef ref_ptr<WitnessMergeFn> witness_merge_fn_t;

    class WitnessMergeFn : public MergeFn 
    {
      public:
        WitnessMergeFn( witness_t witness_rule, merge_fn_t user_merge );

        virtual ~WitnessMergeFn();

        virtual sem_elem_t apply_f( sem_elem_t w1, sem_elem_t w2 );

        virtual std::ostream& print( std::ostream& o ) const;

        virtual bool equal(merge_fn_t mf);

        merge_fn_t get_user_merge();

      protected:
        sem_elem_t priv_do_apply( sem_elem_t w1,sem_elem_t w2 );

      protected:
        witness_t witness_rule;
        merge_fn_t user_merge;

    }; // class WitnessMergeFn

  } // namespace witness

} // namespace wali

#endif  // wali_witness_WITNESS_MERGE_FN_GUARD

