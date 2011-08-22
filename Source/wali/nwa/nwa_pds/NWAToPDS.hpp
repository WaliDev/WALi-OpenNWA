#ifndef WALI_NWA_NWA_PDS_nwatopds_HPP
#define WALI_NWA_NWA_PDS_nwatopds_HPP


#include "wali/nwa/NWAFwd.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/nwa/WeightGen.hpp"

#include "wali/nwa/deprecate.h"

#include <sstream>

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


      /**
       *  
       * @brief returns the default program control location for PDSs
       *
       * This method provides access to the default program control location for PDSs.
       * 
       * @return the default program control location for PDSs
       *
       */
      inline
      wali::Key
      getProgramControlLocation( )
      {
        static Key key = getKey("program");
        return key;
      }

      /**
       *  
       * @brief returns the program control location corresponding to the given states
       *
       * This method provides access to the program control location corresponding to
       * the given exit point/call site/return site triple.
       *
       * @param - exit: the exit point corresponding to this control location
       * @param - callSite: the call site corresponding to this control location
       * @param - returnSite: the return site corresponding to this control location
       * @return the program control location corresponding to the given states
       *
       */
      inline
      wali::Key
      getControlLocation( Key exit, Key callSite, Key returnSite )
      {
        std::stringstream ss;
        ss << "(key#"  << exit << "," << callSite << "," << returnSite << ")";
        wali::Key key = getKey(getProgramControlLocation(), getKey(ss.str()));
        return key;
      }

    } // namespace nwa_pds


    DEPRECATE("Use version of this function inside wali::nwa::nwa_pds")
    inline
    wali::Key
    getProgramControlLocation( )
    {
      return nwa_pds::getProgramControlLocation();
    }

    DEPRECATE("Use version of this function inside wali::nwa::nwa_pds")    
    inline
    wali::Key
    getControlLocation( Key exit, Key callSite, Key returnSite )
    {
      return nwa_pds::getControlLocation(exit, callSite, returnSite);
    }


  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
