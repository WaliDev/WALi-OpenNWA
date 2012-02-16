#ifndef wali_BOOST_HEAP_PRIORITY_WORKLIST_GUARD
#define wali_BOOST_HEAP_PRIORITY_WORKLIST_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Worklist.hpp"
#include "wali/wfa/Trans.hpp"
#include <map>
#include <set>
#include <functional>

namespace wali
{
  namespace details
  {
  
  template<typename Compare>
  class BoostHeapPriorityWorklist : public Worklist<wfa::ITrans>
  {
      // Boost implements max-heaps, not min-heaps. We want to hide this
      // pecularity, so negate the comparison functor.
      typedef std::binary_negate<Compare> NotCompare;
      
    public:
      typedef std::multiset< wfa::ITrans*, NotCompare > pwl_t;

    public:
      BoostHeapPriorityWorklist();

      virtual ~BoostHeapPriorityWorklist();

      virtual bool put( wfa::ITrans *t );

      virtual wfa::ITrans * get();

      virtual bool empty() const;

      virtual void clear();

    protected:
      pwl_t workset;

  }; // class BoostHeapPriorityWorklist


  } // namespace details
} // namespace wali

#include "wali/details/PriorityWorklist.boost_heap.cpp"

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:


#endif  // wali_BOOST_HEAP_PRIORITY_WORKLIST_GUARD

