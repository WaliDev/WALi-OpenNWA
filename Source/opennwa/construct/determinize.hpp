#include "opennwa/NWAFwd.hpp"

namespace wali
{
  namespace nwa
  {
    namespace construct
    {

      extern void determinize(NWA & out, NWA const & source);


      /**
       *
       * @brief constructs a deterministic NWA that is equivalent to the given NWA.
       *
       * This method constructs a deterministic NWA that is equivalent to the given NWA.
       * Note: The resulting NWA is guaranteed to be deterministic.
       *
       * @param - nondet: the NWA to determinize
       * @param - stuck: dummy parameter
       * @return the NWA resulting from determinizing the given NWA
       *
       */
      extern NWARefPtr determinize( NWA const & nondet );

      
    }
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

