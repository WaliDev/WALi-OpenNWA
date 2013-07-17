#ifndef WALI_NWA_QUERY_automaton_HPP
#define WALI_NWA_QUERY_automaton_HPP

#include "opennwa/NwaFwd.hpp"


namespace opennwa {
  namespace query {

      
    /**
     *
     * @brief Tests whether this NWA is deterministic 
     *
     * This method tests whether this NWA is deterministic or
     * nondeterministic. If this NWA is deterministic, true is
     * returned.  Otherwise, false is returned.
     *
     * An NWA that is *incomplete* -- that is, delta(q,a) for some
     * delta (delta_i, delta_c, or delta_r) is the empty set -- is
     * still considered deterministic.
     *
     * @return true if this NWA is deterministic, false otherwise
     *
     */
    bool
    isDeterministic(Nwa const & nwa);


    /**
     * 
     * @brief determines whether there is any overlap in the states of
     * the given NWAs
     *
     * This method tests whether there is any overlap in the states of
     * the given NWAs.
     *
     *   first->states()  intersect  second->states()  = {}
     *
     * @param first: one of the NWAs whose states to compare
     * @param second: one of the NWAs whose states to compare
     * @return true if there is some in both 'first' and 'second';
     *         false otherwise
     *
     */
    bool
    statesOverlap(Nwa const & first, Nwa const & second);

    /// Returns the number of states of 'nwa' that appear in the call
    /// position of a call transition.
    size_t numCallSites(Nwa const & nwa);

    /// Returns the number of states of 'nwa' that appear in the entry
    /// position of a call transition.
    size_t numEntrySites(Nwa const & nwa);    

    /// Returns the number of states of 'nwa' that appear in the exit
    /// position of a return transition.
    size_t numExitSites(Nwa const & nwa);

    /// Returns the number of states of 'nwa' that appear in the return
    /// position of a return transition.
    size_t numReturnSites(Nwa const & nwa);

    wali::wfa::WFA
    prestar(Nwa const & nwa,
	    WeightGen const & wg,
	    ref_ptr<wali::Worklist<wali::wfa::ITrans> > worklist,
	    wali::wfa::WFA const & input);

    /**
     * @brief perform the prestar reachability query defined by the
     * given WFA
     *
     * This method performs the prestar reachability query defined by
     * the given input WFA.
     *
     * @param nwa: the NWA to perform the query on
     * @param input: the starting point of the reachability query
     * @param wg: the functions to use in generating weights
     * @return the WFA resulting from performing the prestar reachability query 
     */
    inline
    wali::wfa::WFA
    prestar(Nwa const & nwa,
	    WeightGen const & wg,
	    wali::wfa::WFA const & input)
    {
      return prestar(nwa, wg, NULL, input);
    }

    void
    prestar(Nwa const & nwa,
	    WeightGen const & wg,
	    ref_ptr< wali::Worklist<wali::wfa::ITrans> > worklist,
	    wali::wfa::WFA const & input,
	    wali::wfa::WFA & output);

    /**
     * @brief perform the prestar reachability query defined by the
     * given WFA
     *
     * This method performs the prestar reachability query defined by
     * the given WFA.  The result of the query is stored in the
     * 'output' parameter.
     *
     * Note: Any transitions in output before the query will be there
     * after the query but will have no effect on the reachability
     * query.
     *
     * @param nwa: the NWA to perform prestar on
     * @param wg: the functions to use in generating weights
     * @param input: the starting point of the reachability query
     * @param ouput: the result of performing the reachability query
     */
    inline
    void
    prestar(Nwa const & nwa,
	    WeightGen const & wg,
	    wali::wfa::WFA const & input,
	    wali::wfa::WFA & output)
    {
      prestar(nwa, wg, NULL, input, output);
    }


    wali::wfa::WFA
    poststar(Nwa const & nwa,
	     WeightGen const & wg,
	     ref_ptr<wali::Worklist<wali::wfa::ITrans> > wl,
	     wali::wfa::WFA const & input);

    /**
     * @brief perform the poststar reachability query defined by the
     * given input WFA
     *
     * This method performs the poststar reachability query defined by
     * the given WFA.
     *
     * @param nwa: the NWA to perform the poststar query on
     * @param input: the starting point of the reachability query
     * @param wg: the functions to use in generating weights
     * @return the WFA resulting from performing the poststar reachability query
     */
    inline
    wali::wfa::WFA
    poststar(Nwa const & nwa,
	     WeightGen const & wg,
	     wali::wfa::WFA const & input)
    {
      return poststar(nwa, wg, NULL, input);
    }


    void
    poststar(Nwa const & nwa,
	     WeightGen const & wg,
	     ref_ptr<wali::Worklist<wali::wfa::ITrans> > worklist,
	     wali::wfa::WFA const & input,
	     wali::wfa::WFA & output);


    /**
     *
     * @brief perform the poststar reachability query defined by the
     * given WFA
     * 
     * This method performs the poststar reachability query defined by
     * the given WFA.  The result of the query is stored in the
     * 'output' parameter.
     *
     * Note: Any transitions in output before the query will be there
     * after the query but will have no effect on the reachability
     * query.
     *
     * @param nwa: the NWA to perform the poststar query on
     * @param input: the starting point of the reachability query
     * @param output: the result of performing the reachability query
     * @param wg: the functions to use in generating weights
     *
     */
    inline
    void
    poststar(Nwa const & nwa,
	     WeightGen const & wg,
	     wali::wfa::WFA const & input,
	     wali::wfa::WFA & output)
    {
      poststar(nwa, wg, NULL, input, output);
    }

    /*
     * @brief Perform poststar from the NWA's initial states
     * 
     * @param nwa: The NWA to perform poststar on
     * @param wg: the functions to use in generating weights
     */
    wali::wfa::WFA
    poststar(Nwa const & nwa,
	     WeightGen const & wg);

    /*
     * @brief "Unpacks" the given automaton to determine the overall
     *        result at each state
     * 
     * This method takes an automaton resulting from a poststar query,
     * and for each state in the NWA returns the combine of all
     * configurations in the automaton where that state appears at the
     * top of the stack.
     *
     * @param nwa: the NWA poststar was performed on
     * @param poststarredWFA: the automaton which is the output of poststar
     * @return the map of states to weights
     */
    std::map<State, sem_elem_t>
    readPoststarResult(Nwa const & nwa,
		       wali::wfa::WFA poststarredFA);

    /*
     * Returns the combine-over-all-valid-paths value from the
     * NWA's initial state to each state
     *
     * Internally, does a poststar query using FWPDS.
     *
     * @param nwa: the NWA to analyze
     * @param wg: the functions to use in generating weights
     * @return the map of states to weights corresponding to each state
     */
    std::map<State, sem_elem_t>
    doForwardAnalysis(Nwa const & nwa,
		      WeightGen const & wg);

    /**
     * @brief Perform a poststar query from the given automaton,
     * returning the result
     *
     * This method performs the poststar reachability query defined by the
     * given WFA by converting the NWA to an FWPDS.
     *
     * @param input: the starting point of the reachability query
     * @param wg: the functions to use in generating weights
     * @return the WFA resulting from performing the poststar reachability query
     */
    wali::wfa::WFA
    poststarViaFwpds(Nwa const & nwa,
                     WeightGen const & wg,
		     wali::wfa::WFA const & input);

    /**
     * @brief Perform a poststar query from the given configurations,
     * storing the result in 'output'
     *
     * This method performs the poststar reachability query defined by the
     * given WFA by converting the NWA to an FWPDS.
     *
     * Any transitions in output before the query will be there after
     * the query but will have no effect on the reachability query.
     *
     * @param input: the starting point of the reachability query
     * @param output: the result of performing the reachability query
     * @param wg: the functions to use in generating weights
     * @return the WFA resulting from performing the poststar reachability query
     */
    void
    poststarViaFwpds(Nwa const & nwa,
                     WeightGen const & wg,
		     wali::wfa::WFA const & input,
                     wali::wfa::WFA & output);

  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

