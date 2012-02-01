#include "opennwa/NwaFwd.hpp"

namespace opennwa
{
  namespace construct
  {

    /**
     *
     * @brief constructs the NWA which is the intersection of the given NWAs
     *
     * This method constructs the NWA which accepts only nested words that are accepted 
     * by both 'first' and 'second'.  
     * Note: The resulting NWA is NOT guaranteed to be deterministic.	
     *
     * @param - first: the NWA to intersect with 'second'
     * @param - second: the NWA to intersect with 'first'
     *	
     */
    extern void intersect(Nwa & out, Nwa const & first, Nwa const & second);


    /**
     *
     * @brief constructs the NWA which is the intersection of the given NWAs
     *
     * This method constructs the NWA which accepts only nested words that are accepted 
     * by both 'first' and 'second'.  
     * Note: The resulting NWA is NOT guaranteed to be deterministic.	
     *
     * @param - first: the NWA to intersect with 'second'
     * @param - second: the NWA to intersect with 'first'
     * @return the NWA resulting from the intersection of the given NWAs
     *	
     */
    extern NwaRefPtr intersect( Nwa const & first, Nwa const & second );

      
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

