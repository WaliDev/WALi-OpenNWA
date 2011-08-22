#include "wali/nwa/NWAFwd.hpp"
#include "wali/nwa/deprecate.h"

namespace wali
{
  namespace nwa
  {
    namespace nwa_pds
    {

      /**
       *
       * @brief constructs the PDS equivalent to this NWA
       *
       * This method constructs the PDS that is equivalent to this NWA.
       * Note: This version keeps returns on the stack.
       *
       * @param - wg: the functions to use in generating weights
       * @return the PDS equivalent to this NWA
       *
       */ 
      extern wpds::WPDS NwaToWpdsReturns( NWA const & nwa, WeightGen const & wg );

      DEPRECATE("Use NwaToWpdsReturns()")
      inline
      wpds::WPDS
      NWAToPDSReturns( NWA const & nwa, WeightGen const & wg )
      {
        return NwaToWpdsReturns(nwa, wg);
      }

      
      /**
       *
       * @brief constructs the backwards PDS equivalent to this NWA
       *
       * This method constructs the backwards PDS that is equivalent to this NWA.
       * Note: This version keeps returns on the stack.
       *
       * @param - wg: the functions to use in generating weights
       * @return the backwards PDS equivalent to this NWA
       *
       */ 
      extern
      wpds::WPDS
      NwaToWpdsCalls( NWA const & nwa,
                     WeightGen const & wg,
                     ref_ptr<wali::wpds::Wrapper> wrapper );
        
      inline
      wpds::WPDS
      NwaToWpdsCalls( NWA const & nwa, WeightGen const & wg )
      {
        return NwaToWpdsCalls(nwa, wg, NULL);
      }


      DEPRECATE("Use NwaToWpdsCalls()")
      inline
      wpds::WPDS
      NWAToPDSCalls( NWA const & nwa, WeightGen const & wg )
      {
        return NwaToWpdsCalls(nwa, wg, NULL);
      }


      /**
       *
       * @brief constructs the PDS equivalent to this NWA
       *
       * This method constructs the PDS that is equivalent to this NWA.
       * Note: This version keeps calls on the stack.
       *
       * @param - wg: the functions to use in generating weights
       * @return the PDS equivalent to this NWA
       *
       */ 
      extern wpds::WPDS NwaToBackwardsWpdsReturns( NWA const & nwa, WeightGen const & wg );


      DEPRECATE("Use NwaToBackwardsWpdsReturns()")
      inline
      wpds::WPDS
      NWAToBackwardsPDSReturns( NWA const & nwa, WeightGen const & wg )
      {
        return NwaToBackwardsWpdsReturns(nwa, wg);
      }


      /**
       *
       * @brief constructs the backwards PDS equivalent to this NWA
       *
       * This method constructs the backwards PDS that is equivalent to this NWA.
       * Note: This version keeps calls on the stack.
       *
       * @param - wg: the functions to use in generating weights
       * @return the backwards PDS equivalent to this NWA
       *
       */ 
      extern wpds::WPDS NwaToBackwardsWpdsCalls( NWA const & nwa, WeightGen const & wg );

      DEPRECATE("Use NwaToBackwardsWpdsCalls()")
      inline
      wpds::WPDS
      NWAToBackwardsPDSCalls( NWA const & nwa, WeightGen const & wg )
      {
        return NwaToBackwardsWpdsCalls(nwa, wg);
      }

    }
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

