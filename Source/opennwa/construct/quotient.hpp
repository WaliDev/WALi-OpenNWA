#include "opennwa/NwaFwd.hpp"
#include <wali/util/DisjointSets.hpp>

namespace opennwa
{
  namespace construct
  {

    /**
     *
     * @brief constructs the NWA which is the result of applying the quotient operation on the given NWA	
     *
	 * @param - out: the quotient NWA of the given NWA
     * @param - nwa: the NWA on which to apply quotient operation
     * @param - quotientMap: the disjoint set which specifies the equivalence relation on the NWA    
     *	
     */
    extern void quotient( Nwa & out, Nwa const & nwa, wali::util::DisjointSets<State> partitionMap );


    /**
     *
     * @brief constructs the NWA which is the result of applying the quotient operation on the given NWA
     *
     * @param - nwa: the NWA on which to apply quotient operation
     * @param - quotientMap: the disjoint set which specifies the equivalence relation on the NWA
     * @return - the quotient NWA of the given NWA
     *	
     */
    extern NwaRefPtr quotient( Nwa const & nwa, wali::util::DisjointSets<State> partitionMap );

    extern void printPartitionMap(wali::util::DisjointSets<State> const & partitionMap);  
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

