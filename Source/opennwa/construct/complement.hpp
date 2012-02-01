#include "opennwa/NwaFwd.hpp"

namespace opennwa
{
  namespace construct
  {

    /**
     * 
     * @brief constructs the NWA that is the complement of the given NWA
     *
     * This method constructs the complement of the given NWA by determinizing it and
     * then replacing the set of final states with the set of states that are not final
     * Note: The resulting NWA is guaranteed to be deterministic.
     *
     * @param - first: the NWA to perform the complement of
     *
     */
    extern void complement(Nwa & out, Nwa const & source);


    /**
     * 
     * @brief constructs the NWA that is the complement of the given NWA
     *
     * This method constructs the complement of the given NWA by determinizing it and
     * then replacing the set of final states with the set of states that are not final
     * Note: The resulting NWA is guaranteed to be deterministic.
     *
     * @param - first: the NWA to perform the complement of
     * @return the NWA resulting from complementing the given NWA
     *
     */
    extern NwaRefPtr complement( Nwa const & source );

      
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

