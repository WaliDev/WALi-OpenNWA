
#include "wali/TotalOrderWorklist.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "opennwa/Nwa.hpp"
#include "opennwa/query/language.hpp"
#include "opennwa/nwa_pds/conversions.hpp"
#include "wali/wpds/WPDS.hpp"
#include "opennwa/ClientInfo.hpp"
#include "wali/witness/WitnessWrapper.hpp"

//#define JAMDEBUG 1

using namespace wali;

namespace opennwa {
  
  namespace query {


    ref_ptr<NestedWord>
    getSomeAcceptedWordInternal(Nwa const & nwa, WeightGen const & wg);

      
    ref_ptr<NestedWord>
    getSomeAcceptedWord(Nwa const & nwa)
    {
      ReachGen wg;
      return getSomeAcceptedWordInternal(nwa, wg);
    }


    ref_ptr<NestedWord>
    getSomeShortestAcceptedWord(Nwa const & nwa)
    {
      ShortestWordGen wg;
      return getSomeAcceptedWordInternal(nwa, wg);
    }


    ref_ptr<NestedWord>
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

#ifdef JAMDEBUG
      std::cerr << "##### WPDS" << std::endl;
      conv.print(std::cerr);
#endif

      conv.setWorklist(new TotalOrderWorklist());

      Key state = nwa_pds::getProgramControlLocation();
      Key accept = getKey("__accept");

      // Construct the query automaton
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
#ifdef JAMDEBUG
      std::cerr << "##### QUERY1" << std::endl;
      query.print(std::cerr);
#endif

	
      // Execute the post* query
      wfa::WFA path = conv.poststar(query);
#ifdef JAMDEBUG
      std::cerr << "##### POSTSTAR" << std::endl;
      path.print(std::cerr);
#endif

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
      path = path.intersect(wmaker, fa);
      
#ifdef JAMDEBUG
      std::cerr << "##### INTERSECTION" << std::endl;
      path.print(std::cerr);
#endif

      path.path_summary_tarjan();

#ifdef JAMDEBUG
      std::cerr << "##### SUMMARY" << std::endl;
      path.print(std::cerr);
#endif

      return path.getState(path.getInitialState())->weight();
    }
      
    ref_ptr<NestedWord>
    getSomeAcceptedWordInternal(Nwa const & nwa, WeightGen const & wg)
    {
      sem_elem_t se = getWitnessForSomeAcceptedWordWithWeights(nwa, wg);
      if (se.is_empty()) return ref_ptr<NestedWord>();

      witness::Witness *wit =
        dynamic_cast<witness::Witness*>(se.get_ptr());
      assert(wit);
#ifdef JAMDEBUG
      std::cerr << "WITNESS DEPTH: " << wit->getLength() << std::endl;
#endif

      if (!wit->isZero()) {

        // Find a shortest word through the witness.
        details::PathVisitor pv(nwa);
        wit->accept(pv);

        return new NestedWord(pv.answer());
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

