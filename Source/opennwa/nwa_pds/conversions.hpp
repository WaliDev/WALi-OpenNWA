#ifndef WALI_NWA_NWA_PDS_nwatopds_HPP
#define WALI_NWA_NWA_PDS_nwatopds_HPP


#include "opennwa/NwaFwd.hpp"
#include "wali/wpds/WPDS.hpp"
#include "opennwa/WeightGen.hpp"

#include "opennwa/deprecate.h"

#include <sstream>

namespace opennwa
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
    extern wali::wpds::WPDS NwaToWpdsReturns( Nwa const & nwa, WeightGen const & wg );


      
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
    wali::wpds::WPDS
    NwaToWpdsCalls( Nwa const & nwa,
                    WeightGen const & wg,
                    ref_ptr<wali::wpds::Wrapper> wrapper );
        
    inline
    wali::wpds::WPDS
    NwaToWpdsCalls( Nwa const & nwa, WeightGen const & wg )
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
    extern wali::wpds::WPDS NwaToBackwardsWpdsReturns( Nwa const & nwa, WeightGen const & wg );


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
    extern wali::wpds::WPDS NwaToBackwardsWpdsCalls( Nwa const & nwa, WeightGen const & wg );


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
      return getKey("program");
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
    getControlLocation( Key otherState )
    {
      //std::stringstream ss;
      //ss << "(key#"  << exit << "," << callSite << "," << returnSite << ")";
      //wali::Key key = getKey(getProgramControlLocation(), getKey(ss.str()));

      State key = getKey(getProgramControlLocation(), otherState);
      return key;
    }



    /**
     *
     * @brief constructs the NWA equivalent to the given PDS
     *
     * This method constructs the NWA that is equivalent to the given
     * PDS. The NWA's state space tracks both the PDS's state and top stack
     * symbol; the NWA's "stack" tracks the remainder of the PDS's stack.
     * The symbol on each transition is the from_stack portion of the
     * corresponding PDS rule.
     *
     *
     * Let the PDS be (P, G, D). The NWA we create is (Q, Q_0, d, Q_f); d
     * is split up into the usual (for NWAs) components d_i, d_c, and d_r.
     *  - Q = P x G    (Tracking the PDS state + top stack symbol)
     *  - d_i = { ((q,a), a, (q',a'))        |  <q, a> -> <q', a'> in D }
     *  - d_c = { ((q,a), a, (q',b))         |  <q, a> -> <q', b a'> in D }
     *  - d_r = { ((q,a), (p,b), a, (q',a')) |  <q, a> -> <q', *> in D  and
     *                                          <p, b> -> <_, _ a'> in D }
     *
     * The last rule is a little funky in a couple ways. First, it has two
     * conditions because the pop rule doesn't let you know what's on the
     * top of the stack, and we need to figure that out from the
     * corresponding push rule.
     *
     * Second, note that the push rules we look at in the condition is
     * totally unconstrained. This is because, with the exception of the
     * "revealed" stack symbol a', everything that the push rule talks
     * about concerns the call predecessor (p,b) and entry node (_,_);
     * nothing the pop rule talks about -- q, a, or q' -- has any relation
     * to those.
     *
     * This last fact means that the NWA we construct can potentially have
     * quadratically-many rules as the input PDS. If the PDS has M push
     * rules and N pop rules, the NWA will have M*N return transitions.
     *
     * @param - pds: the pds to convert 
     * @return the NWA equivalent to the given PDS
     *
     */
    void WpdsToNwa( Nwa & out, const wali::wpds::WPDS & pds ); 

    /**
     *
     * @brief constructs the NWA equivalent to the given PDS
     *
     * This method constructs the NWA that is equivalent to the given PDS.
     *
     * @param - pds: the pds to convert
     * @param - stuck: dummy parameter
     * @return the NWA equivalent to the given PDS
     *
     */
    NwaRefPtr WpdsToNwa( const wali::wpds::WPDS & pds );
      
  } // namespace nwa_pds
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
