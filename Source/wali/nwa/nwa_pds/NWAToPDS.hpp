#include "wali/nwa/NWAFwd.hpp"

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
      extern wpds::WPDS NWAToPDSReturns( NWA const & nwa, WeightGen & wg );

      
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
      extern wpds::WPDS NWAToPDSCalls( NWA const & nwa, WeightGen & wg );


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
      extern wpds::WPDS NWAToBackwardsPDSReturns( NWA const & nwa, WeightGen & wg );

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
      extern wpds::WPDS NWAToBackwardsPDSCalls( NWA const & nwa, WeightGen & wg );

    }
  }
}
