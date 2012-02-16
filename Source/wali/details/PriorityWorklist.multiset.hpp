#ifndef wali_MULTISET_PRIORITY_WORKLIST_GUARD
#define wali_MULTISET_PRIORITY_WORKLIST_GUARD 1

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
  namespace details
  {
  
    template<typename Compare>
    class MultisetPriorityWorklist : public Worklist<wfa::ITrans>
    {
    public:
      typedef std::multiset< wfa::ITrans*, Compare > pwl_t;

    public:
      MultisetPriorityWorklist();

      virtual ~MultisetPriorityWorklist();

      virtual bool put( wfa::ITrans *t );

      virtual wfa::ITrans * get();

      virtual bool empty() const;

      virtual void clear();

    protected:
      pwl_t workset;

    }; // class PriorityWorklist

  } // namespace details
} // namespace wali

#include "wali/details/PriorityWorklist.multiset.cpp"

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif  // wali_MULTISET_PRIORITY_WORKLIST_GUARD

