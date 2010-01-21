#ifndef wali_MERGE_FN_FACTORY_GUARD
#define wali_MERGE_FN_FACTORY_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/MergeFn.hpp"
#include <string>

namespace wali
{
  class MergeFnFactory
  {
    public:
      MergeFnFactory();

      virtual ~MergeFnFactory();

      virtual wali::merge_fn_t getMergeFn( std::string s ) = 0;

  }; // class MergeFnFactory

} // namespace wali

#endif  // wali_MERGE_FN_FACTORY_GUARD

