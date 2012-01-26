#ifndef wali_RANKED_WORKLIST_GUARD
#define wali_RANKED_WORKLIST_GUARD 1

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
  class RankedWorklist : public Worklist<wfa::ITrans>
  {
    public:
      typedef std::multimap< int, wfa::ITrans* > pwl_t;

    public:
      RankedWorklist();

      virtual ~RankedWorklist();

      virtual bool put( wfa::ITrans *t );

      virtual wfa::ITrans * get();

      virtual bool empty() const;

      virtual void clear();

      /*
       * Override this to get the rank of a transition
       */
      virtual int doRankOf( sem_elem_t a ) const = 0;

    private:
      pwl_t workset;

      int rankOf( const wfa::ITrans* a ) const;

  }; // class RankedWorklist

} // namespace wali

#endif  // wali_RANKED_WORKLIST_GUARD

