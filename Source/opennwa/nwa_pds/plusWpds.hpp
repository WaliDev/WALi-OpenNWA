#ifndef WALI_NWA_NWA_PDS_plusWpds_HPP
#define WALI_NWA_NWA_PDS_plusWpds_HPP


#include "opennwa/NwaFwd.hpp"
#include "wali/wpds/WPDS.hpp"

namespace opennwa
{
  namespace nwa_pds
  {

    /**
     * 
     * @brief constructs the WPDS which is the result of the explicit NWA plus WPDS 
     *        construction from Advanced Querying for Property Checking
     *
     * This method constructs the WPDS which allows WPDS reachability to be used to 
     * perform property checking using this NWA and the given WPDS.
     *
     * @param - base: the WPDS that forms the basis for the constructed WPDS
     * @param - nwa: the NWA to process
     * @return the WPDS which can be used to perform property checking using PDS 
     *          reachability
     */
    wali::wpds::WPDS plusWpds( Nwa const & nwa, const wali::wpds::WPDS & base ); 


  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
