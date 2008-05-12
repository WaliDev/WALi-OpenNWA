#ifndef wali_witness_WITNESS_WRAPPER_GUARD
#define wali_witness_WITNESS_WRAPPER_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/MergeFn.hpp"
#include "wali/wpds/Wrapper.hpp"

namespace wali
{
  namespace witness
  {
    /*!
     * @class WitnessWrapper
     */
    class WitnessWrapper : public ::wali::wpds::Wrapper
    {
      public:

        WitnessWrapper() {}

        virtual ~WitnessWrapper() {}

        virtual sem_elem_t wrap( wfa::ITrans& t );

        virtual sem_elem_t wrap( wpds::Rule& r );

        virtual merge_fn_t wrap( merge_fn_t );

        virtual sem_elem_t unwrap( sem_elem_t se );

    }; // namespace WitnessWrapper

  } // namespace witness

} // namespace wali

#endif  // wali_witness_WITNESS_WRAPPER_GUARD

