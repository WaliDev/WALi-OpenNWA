
#include <vector>
#include <set>

#include "opennwa/query/returns.hpp"
#include "opennwa/query/calls.hpp"
#include "opennwa/query/internals.hpp"
#include "wali/TotalOrderWorklist.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "opennwa/Nwa.hpp"
#include "opennwa/query/language.hpp"
#include "opennwa/nwa_pds/conversions.hpp"
#include "wali/wpds/WPDS.hpp"
#include "opennwa/ClientInfo.hpp"
#include "wali/witness/WitnessWrapper.hpp"
#include "wali/witness/WitnessLengthWorklist.hpp"
#include "opennwa/query/PathVisitor.hpp"
#include "opennwa/query/ShortWitnessVisitor.hpp"


using namespace wali;

namespace opennwa {
  
  namespace query {


    NestedWordRefPtr
    getSomeAcceptedWordInternal(Nwa const & nwa, WeightGen const & wg);

      
    NestedWordRefPtr
    getSomeAcceptedWord(Nwa const & nwa)
    {
      ReachGen wg;
      return getSomeAcceptedWordInternal(nwa, wg);
    }


    NestedWordRefPtr
    getSomeShortestAcceptedWord(Nwa const & nwa)
    {
      ShortestWordGen wg;
      return getSomeAcceptedWordInternal(nwa, wg);
    }


    NestedWordRefPtr
    getSomeAcceptedWordWithWeights(Nwa const & nwa, WeightGen const & wg)
    {
      return getSomeAcceptedWordInternal(nwa, wg);
    }

    sem_elem_t getWitnessForSomeAcceptedWordWithWeights(Nwa const & nwa, WeightGen const & wg) {
      if (nwa.sizeInitialStates() == 0 || nwa.sizeFinalStates() == 0) {
        return sem_elem_t();
      }
        
      ref_ptr<wpds::Wrapper> wrapper = new witness::WitnessWrapper();
      wpds::WPDS conv = nwa_pds::NwaToWpdsCalls(nwa, wg, wrapper);

      // Set the worklist to determine the order of poststar traversal.
      conv.setWorklist(new witness::WitnessLengthWorklist());

      Key state = nwa_pds::getProgramControlLocation();
      Key accept = getKey("__accept");

      // Construct the poststar query automaton to get all
      // configurations reachable from the initial state.
      wfa::WFA query;
      query.addState(state, wg.getOne()->zero());
      query.setInitialState(state);
      query.addState(accept, wg.getOne()->zero());
      query.addFinalState(accept);
      for( Nwa::StateIterator initial = nwa.beginInitialStates();
           initial != nwa.endInitialStates(); initial++ )
      {
        query.addTrans(state, *initial, accept, wg.getOne());
        query.addTrans(accept, *initial, accept, wg.getOne()); // Accept pending returns
      }

      // Execute the post* query.
      wfa::WFA path = conv.poststar(query);

      // Prune unreachable states.
      path.prune();

      // Intersect path with a new 2-state WFA with a transitioning edge
      // for each final NWA state, and a self-loop from the WFA final
      // state on each non-final NWA state.
      wfa::WFA fa;
      Key init = getKey("__init");
      fa.addState(init, wg.getOne()->zero());
      fa.setInitialState(init);
      Key fin = getKey("__final");
      fa.addState(fin, wg.getOne()->zero());
      fa.addFinalState(fin);

      for( Nwa::StateIterator fit = nwa.beginFinalStates();
        fit != nwa.endFinalStates(); fit++)
      {
        fa.addTrans(init, *fit, fin, wg.getOne());
      }

      for( Nwa::StateIterator sit = nwa.beginStates();
        sit != nwa.endStates(); sit++ )
      {
        fa.addTrans(fin, *sit, fin, wg.getOne());
      }

      wfa::KeepLeft wmaker;

      wfa::WFA ipath;
      path.intersect(wmaker, fa, ipath);

      // Intersect may prune all final states.
      if (ipath.getFinalStates().size() == 0) {
        return sem_elem_t();
      }

      ipath.path_summary_iterative_wpds(wfa::WFA::ComputeInitialState);

      return ipath.getState(ipath.getInitialState())->weight();
    }
      
    NestedWordRefPtr
    getSomeAcceptedWordInternal(Nwa const & nwa, WeightGen const & wg)
    {
      sem_elem_t se = getWitnessForSomeAcceptedWordWithWeights(nwa, wg);
      if (se.is_empty()) return NULL;

      witness::Witness *wit =
        dynamic_cast<witness::Witness*>(se.get_ptr());
      assert(wit);

      if (!wit->isZero()) {
        // Find a shortest single-path witness.
        ShortWitnessVisitor swv;
        wit->accept(swv, false);

        witness::Witness *pathWit = swv.answer().get_ptr();
        assert(pathWit);
        //std::cerr << "##### PATHWIT LENGTH: " << pathWit->getLength() << std::endl;
        //pathWit->reset_marks();

        // Generate a NestedWord from the single-path witness.
        PathVisitor pv(nwa);
        pathWit->accept(pv, false);

        NestedWord *nw = new NestedWord(pv.getPath());
        return NestedWordRefPtr(nw);
      }

      // No word was found.
      return NULL;
    }

  } // namespace query
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

