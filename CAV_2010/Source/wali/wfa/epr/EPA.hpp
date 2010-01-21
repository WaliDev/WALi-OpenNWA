#ifndef wali_EPA_GUARD
#define wali_EPA_GUARD 1

/*!
 * @author Nicholas Kidd
 * @author Akash Lal
 */

#include "wali/wfa/WFA.hpp"
#include "wali/wfa/epr/FunctionalWeight.hpp"
#include "wali/wfa/epr/FunctionalWeightMaker.hpp"
#include <map>
#include <list>

namespace wali 
{
  namespace wfa 
  {
    namespace epr 
    {

      /*! 
       * @class EPA
       * @brief Implements the Error Projection Automaton
       */

      class EPA : public WFA {
        private:

          // Store the BFS order for states <-- not updated with transition insertions
          std::map< Key, unsigned int> bfsOrder;

          typedef std::list< std::pair< sem_elem_t, sem_elem_t > > CacheElem;
          std::map< Key, CacheElem > errorProjCache;
          std::map< Key, walienum::ETag > stateTagMap;
          int nCacheHits;

        public:

          EPA( );

          ~EPA();

          void clear();

          int getCacheHits();

          void buildEPA(WFA &post, WFA &pre);

          // Compute the error projection weight for node with bottleneck weight initWeight
          sem_elem_t apply(Key node, sem_elem_t initWeight);

          // Same as above, but increments notfound by 1 if the node was not found in
          // the automaton
          sem_elem_t apply(Key node, sem_elem_t initWeight, int &notfound);

        private:

          // returns invalid sem_elem_t if (q,w) is not found in the cache
          sem_elem_t lookupCache(Key q, sem_elem_t w);

          // Adds ((q,w),res) to the cache
          void addToCache(Key q, sem_elem_t w, sem_elem_t res);

          // Maintain a State Key -> ETag map
          void setStateTag(State *s, walienum::ETag et);
          walienum::ETag getStateTag(State *s);

          // Do a BFS on the automaton and store the BFS number
          // in bfsOrder
          void orderStatesBFS();

      }; // class EPA

    } // namespace epr

  } // namespace wfa

} // namespace wali

#endif  // wali_EPA_GUARD

