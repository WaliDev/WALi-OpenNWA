#ifndef wali_witness_WITNESS_WRAPPER_GUARD
#define wali_witness_WITNESS_WRAPPER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/MergeFn.hpp"
#include "wali/wpds/Wrapper.hpp"

namespace wali
{
  namespace witness
  {
    /**
     * @class WitnessWrapper
     */
    class WitnessWrapper : public ::wali::wpds::Wrapper
    {
      public:

        WitnessWrapper() {}

        virtual ~WitnessWrapper() {}

        virtual sem_elem_t wrap( wfa::ITrans const & t );

        virtual sem_elem_t wrap( wpds::Rule const & r );

        virtual merge_fn_t wrap( wpds::ewpds::ERule const & r, merge_fn_t user_merge );

        virtual sem_elem_t unwrap( sem_elem_t se );

        virtual merge_fn_t unwrap( merge_fn_t mf );

    }; // namespace WitnessWrapper

  } // namespace witness

} // namespace wali

#endif  // wali_witness_WITNESS_WRAPPER_GUARD

