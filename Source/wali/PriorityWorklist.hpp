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

namespace wali
{
  template<typename Compare>
  class PriorityWorklist : public Worklist<wfa::ITrans>
  {
    public:
      typedef std::multiset< wfa::ITrans*, Compare > pwl_t;

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

