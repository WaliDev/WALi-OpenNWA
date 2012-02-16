#ifndef wali_PRIORITY_WORKLIST_GUARD
#define wali_PRIORITY_WORKLIST_GUARD 1

#include <boost/version.hpp>

#if defined(BOOST_VERSION) && BOOST_VERSION >= 104900
#    define WALI_HAS_BOOST_HEAP 1
#else
#    define WALI_HAS_BOOST_HEAP 0
#endif


#include "wali/Common.hpp"
#include "wali/Worklist.hpp"
#include "wali/wfa/Trans.hpp"



#if WALI_HAS_BOOST_HEAP

#    include "wali/details/PriorityWorklist.boost_heap.hpp"

namespace wali
{
  template<typename Compare>
  class PriorityWorklist : public wali::details::BoostHeapPriorityWorklist<Compare>
  {};
}

#else
#    if HAS_PRAGMA_MESSAGE
#        pragma message ("NOTE: Proper priority queues require Boost 1.49 or higher (with the Heap library). Disabling these and replacing with the default, unordered worklist.")
#    endif

#    include "wali/DefaultWorklist.hpp"

namespace wali
{
  template<typename Compare>
  class PriorityWorklist : public DefaultWorklist<wfa::ITrans>
  {};
}

#endif



#include "wali/details/PriorityWorklist.multiset.hpp"

namespace wali
{
  template<typename Compare>
  class FixedPriorityWorklist : public wali::details::MultisetPriorityWorklist<Compare>
  {};
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif  // wali_PRIORITY_WORKLIST_GUARD

