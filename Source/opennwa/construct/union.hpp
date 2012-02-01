#include "opennwa/NwaFwd.hpp"

namespace opennwa
{
  namespace construct
  {
    /**
     *
     * @brief constructs the NWA resulting from the union of the given NWAs 
     *
     * This method constructs the union of the given NWAs by unioning the state sets,
     * symbols, transitions, initial state sets, and final state sets of the two NWAs.
     * Note: The resulting NWA is guaranteed NOT to be deterministic.
     *
     * @param - first: the NWA to union with 'second'
     * @param - second: the NWA to union with 'first'
     *
     */
    extern void unionNwa(Nwa & out, Nwa const & first, Nwa const & second);


    /**
     *
     * @brief constructs the NWA resulting from the union of the given NWAs 
     *
     * This method constructs the union of the given NWAs by unioning the state sets,
     * symbols, transitions, initial state sets, and final state sets of the two NWAs.
     * Note: The resulting NWA is guaranteed NOT to be deterministic.
     *
     * @param - first: the NWA to union with 'second'
     * @param - second: the NWA to union with 'first'
     * @param - stuck: dummy variable
     * @return the NWA resulting from the union of the given NWAs
     *
     */
    extern NwaRefPtr unionNwa( Nwa const & first, Nwa const & second );

      
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

