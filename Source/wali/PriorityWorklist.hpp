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
  class PriorityWorklist : public Worklist<wfa::ITrans>
  {
    private:
      struct LessThan {
        LessThan( PriorityWorklist& pw ) : pWorklist(pw) {}

        PriorityWorklist& pWorklist;

        bool operator()( const wfa::ITrans* a, const wfa::ITrans* b ) const
        {
          return pWorklist.compareTo(a,b) < 0;
        }

      };

    public:
      typedef std::multiset< wfa::ITrans*, LessThan > pwl_t;

    public:
      PriorityWorklist();

      virtual ~PriorityWorklist();

      virtual bool put( wfa::ITrans *t );

      virtual wfa::ITrans * get();

      virtual bool empty() const;

      virtual void clear();

      /*
       * Override this for custom sorting. Default is to perform a
       * comparison on the Trans's stack key.
       */
      virtual int compareTo( const wfa::ITrans* a, const wfa::ITrans* b ) const;

    protected:
      pwl_t workset;

  }; // class PriorityWorklist

} // namespace wali

#endif  // wali_PRIORITY_WORKLIST_GUARD

