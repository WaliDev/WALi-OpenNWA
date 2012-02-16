#ifndef wali_PRIORITY_WORKLIST_GUARD
#define wali_PRIORITY_WORKLIST_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Worklist.hpp"
#include "wali/wfa/Trans.hpp"
#include <map>
#include <set>
#include <functional>

#include <boost/version.hpp>

#if !defined(BOOST_VERSION) || BOOST_VERSION < 104900
#    pragma message ("NOTE: This file requires Boost 1.49 or higher (with the Heap library). The next lines are likely to be a missing header error.")
#endif

#include <boost/heap/binomial_heap.hpp>

namespace wali
{
  template<typename Compare>
  class PriorityWorklist : public Worklist<wfa::ITrans>
  {
      // Boost implements max-heaps, not min-heaps. We want to hide this
      // pecularity, so negate the comparison functor.
      typedef std::binary_negate<Compare> NotCompare;
      
    public:
      typedef std::multiset< wfa::ITrans*, NotCompare > pwl_t;

    public:
      PriorityWorklist();

      virtual ~PriorityWorklist();

      virtual bool put( wfa::ITrans *t );

      virtual wfa::ITrans * get();

      virtual bool empty() const;

      virtual void clear();

    protected:
      pwl_t workset;

  }; // class PriorityWorklist

} // namespace wali

#include "wali/PriorityWorklist.cpp"

#endif  // wali_PRIORITY_WORKLIST_GUARD

