#ifndef wali_SHORTEST_PATH_WORKLIST_GUARD
#define wali_SHORTEST_PATH_WORKLIST_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/RankedWorklist.hpp"

namespace wali
{
  class ShortestPathWorklist : public RankedWorklist
  {
    public:
      ShortestPathWorklist();
      virtual ~ShortestPathWorklist();

      /*!
       * Returns -1 if a's weight is less than b's weight.
       */
      virtual int doRankOf( sem_elem_t a ) const;

  }; // class PriorityWorklist

} // namespace wali

#endif  // wali_SHORTEST_PATH_WORKLIST_GUARD

