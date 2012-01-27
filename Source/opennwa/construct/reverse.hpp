#include "opennwa/NWAFwd.hpp"

namespace opennwa
{
    namespace construct
    {

      /**
       *
       * @brief constructs the NWA which is the reverse of the given NWA
       *
       * This method constructs the NWA which is the reverse of the given NWA. It reverses 
       * internal transitions and switches call and return transitions.
       * Note: the resulting NWA is NOT guaranteed to be deterministic.
       *
       * @param - first: the NWA to reverse
       * 
       */
      extern void reverse(NWA & out, NWA const & source);


      /**
       *
       * @brief constructs the NWA which is the reverse of the given NWA
       *
       * This method constructs the NWA which is the reverse of the given NWA. It reverses 
       * internal transitions and switches call and return transitions.
       * Note: the resulting NWA is NOT guaranteed to be deterministic.
       *
       * @param - first: the NWA to reverse
       * @param - stuck: dummy parameter
       * @return the NWA resulting from reversing the given NWA
       * 
       */
      extern NWARefPtr reverse( NWA const & source );

      
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

