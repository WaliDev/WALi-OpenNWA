#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "opennwa/Nwa.hpp"
#include "opennwa/query/automaton.hpp"
#include "opennwa/nwa_pds/conversions.hpp"

namespace opennwa {
  namespace query {

    using wali::wpds::WPDS;
    using wali::wfa::WFA;

    wali::wfa::WFA
    prestar(Nwa const & nwa,
            WeightGen const & wg,
            ref_ptr< wali::Worklist<wali::wfa::ITrans> > worklist,
            wali::wfa::WFA const & input)
    {
      WPDS wpds = nwa_pds::NwaToWpdsCalls(nwa, wg);
      if (worklist != NULL) {
        wpds.setWorklist(worklist);
      }
      return wpds.prestar(input);
    }

    
    void
    prestar(Nwa const & nwa,
            WeightGen const & wg,
            ref_ptr< wali::Worklist<wali::wfa::ITrans> > worklist,
            wali::wfa::WFA const & input,
            wali::wfa::WFA & output)
    {
      WPDS wpds = nwa_pds::NwaToWpdsCalls(nwa, wg);
      if (worklist != NULL) {
        wpds.setWorklist(worklist);
      }
      wpds.prestar(input, output);
    }

    
    wali::wfa::WFA
    poststar(Nwa const & nwa,
             WeightGen const & wg,
             ref_ptr< wali::Worklist<wali::wfa::ITrans> > worklist,
             wali::wfa::WFA const & input)
    {
      WPDS wpds = nwa_pds::NwaToWpdsCalls(nwa, wg);
      if (worklist != NULL) {
        wpds.setWorklist(worklist);
      }
      return wpds.poststar(input);
    }

    
    void
    poststar(Nwa const & nwa,
             WeightGen const & wg,
             ref_ptr< wali::Worklist<wali::wfa::ITrans> > worklist,
             wali::wfa::WFA const & input,
             wali::wfa::WFA & output)
    {
      WPDS wpds = nwa_pds::NwaToWpdsCalls(nwa, wg);
      if (worklist != NULL) {
        wpds.setWorklist(worklist);
      }
      wpds.poststar(input, output);
    }


    wali::wfa::WFA
    poststarViaFwpds(Nwa const & nwa,
                     WeightGen const & wg,
                     wali::wfa::WFA const & input)
    {
      wali::wpds::fwpds::FWPDS pds;
      nwa_pds::NwaToWpdsCalls(nwa, wg, pds);
      return pds.poststar(input);
    }


    void
    poststarViaFwpds(Nwa const & nwa,
                     WeightGen const & wg,
                     wali::wfa::WFA const & input,
                     wali::wfa::WFA & output)
    {
      wali::wpds::fwpds::FWPDS pds;
      nwa_pds::NwaToWpdsCalls(nwa, wg, pds);
      pds.poststar(input, output);
    }

    wali::wfa::WFA
    poststar(Nwa const & nwa,
             WeightGen const & wg)
    {
      //create query automata

      State program = opennwa::nwa_pds::getProgramControlLocation();
      State accept = wali::getKey("accept");

      wali::wfa::WFA faIn;         // Query automata
      faIn.addState(program, wg.getOne()->zero());
      faIn.addState(accept, wg.getOne()->zero());

      faIn.setInitialState(program);
      faIn.addFinalState(accept);

      for (opennwa::Nwa::StateIterator sit = nwa.beginInitialStates();
           sit != nwa.endInitialStates(); sit++)
      {
        faIn.addTrans(program, *sit, accept, wg.getOne());
      }

      // run poststar on the query automata
      wali::wfa::WFA output;
      poststar(nwa, wg, faIn, output);

      return output;
    }

    std::map<State, sem_elem_t>
    readResult(Nwa const & nwa,
               wali::wfa::WFA wfa)
    {
      wali::Key const program = opennwa::nwa_pds::getProgramControlLocation();
      wfa.path_summary();
      std::map<State, sem_elem_t> stateWeightMap;

      for (opennwa::Nwa::StateIterator sit = nwa.beginStates();
           sit != nwa.endStates(); sit++)
      {
        // get the set of transitions matching (program,state,?) in the WFA
        wali::wfa::TransSet transitionSet = wfa.match(program, *sit);
        
        sem_elem_t weight = wfa.getSomeWeight()->zero();

        for (wali::wfa::TransSet::iterator tsiter = transitionSet.begin();
             tsiter != transitionSet.end() ; tsiter++)
        {
          wali::wfa::ITrans * t = *tsiter;

          // This is different according to either post or pre
          sem_elem_t tmp;
          sem_elem_t target_weight = wfa.getState(t->to())->weight();
          sem_elem_t trans_weight = t->weight();

          if (wfa.getQuery() == WFA::INORDER) {
            tmp = trans_weight->extend(target_weight);
          }
          else {
            tmp = target_weight->extend(trans_weight);
          }

          // combine the weights of all "to" states 
          weight = weight->combine(tmp);
        }

        stateWeightMap[*sit] = weight;
      }
    
      return stateWeightMap;
    }

    std::map<State, sem_elem_t>
    doForwardAnalysis(Nwa const & nwa, WeightGen const &wg)
    {   
      // poststar can't be performed if the NWA has no initial states,
      // in which case return a map with all states mapped to zero
      if (nwa.sizeInitialStates() == 0u) {
        std::map<State, sem_elem_t> all_zero_map;

        for (opennwa::Nwa::StateIterator sit = nwa.beginStates();
             sit != nwa.endStates(); sit++)
        {
          all_zero_map[*sit] = wg.getOne()->zero();
        }
        return all_zero_map;
      }

      return readResult(nwa, poststar(nwa, wg));
    }


  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
