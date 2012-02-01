#include "opennwa/NwaFwd.hpp"

namespace opennwa
{
  namespace construct
  {

    /**
     *
     * @brief constructs the NWA resulting from the concatenation of the given NWAs
     *
     * This method constructs the concatenation of the given NWAs by adding epsilon 
     * transitions from all final states of 'first' to all initial states of 'second'.  
     * Note: The resulting NWA is NOT guaranteed to be deterministic.
     *
     * @param - first: the NWA to which 'second' should be concatenated
     * @param - second: the NWA to concatenate onto the end of 'first'
     *
     */
    extern void concat(Nwa & out, Nwa const & first, Nwa const & second);


    /**
     *
     * @brief constructs the NWA resulting from the concatenation of the given NWAs
     *
     * This method constructs the concatenation of the given NWAs by adding epsilon 
     * transitions from all final states of 'first' to all initial states of 'second'.  
     * Note: The resulting NWA is NOT guaranteed to be deterministic.
     *
     * @param - first: the NWA to which 'second' should be concatenated
     * @param - second: the NWA to concatenate onto the end of 'first'
     * @param - stuck: dummy parameter
     * @return the NWA resulting from the concatenation of the given NWAs
     *
     */
    extern NwaRefPtr concat( Nwa const & first, Nwa const & second );

      
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

