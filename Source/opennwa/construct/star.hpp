#include "opennwa/NwaFwd.hpp"

namespace opennwa
{
  namespace construct
  {

    /**
     *
     * @brief constructs the NWA resulting from performing Kleene-* on the given NWA
     *
     * This method constructs the Kleene-* of the given NWA by adding epsilon transitions 
     * from all final states of the NWA to all initial states of the NWA.  
     * Note: The resulting NWA is NOT guaranteed to be deterministic.
     *
     * @param - first: the NWA to perform the Kleene-* of
     *
     */
    extern void star(Nwa & out, Nwa const & source);


    /**
     *
     * @brief constructs the NWA resulting from performing Kleene-* on the given NWA
     *
     * This method constructs the Kleene-* of the given NWA by adding epsilon transitions 
     * from all final states of the NWA to all initial states of the NWA.  
     * Note: The resulting NWA is NOT guaranteed to be deterministic.
     *
     * @param - first: the NWA to perform the Kleene-* of
     * @param - stuck: dummy parameter
     * @return the NWA resulting from performing Kleene-* on the given NWA
     *
     */
    extern NwaRefPtr star( Nwa const & nondet );

      
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

