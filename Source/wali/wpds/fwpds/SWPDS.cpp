
#include "wali/wpds/fwpds/SWPDS.hpp"
using namespace std;

namespace wali {
  namespace wpds {

    namespace ewpds {

      /////////////////////////////////////////////////////////////////
      // class CopyCallRules
      /////////////////////////////////////////////////////////////////
      
      CopyCallRules::CopyCallRules( EWPDS &w ) : pds(w) {}
      
      CopyCallRules::~CopyCallRules() {}
      
      void CopyCallRules::operator()( rule_t & r )
      {
        ref_ptr<ERule> er = dynamic_cast<ERule *> (r.get_ptr());
        assert(er != 0);
        
        if(r->to_stack2() != WALI_EPSILON) {
          pds.add_rule(r->from_state(), r->from_stack(), r->to_state(), r->to_stack1(), r->to_stack2(), r->weight(), er->merge_fn());
        }
      }
    } // namespace ewpds
    

    namespace fwpds {

      SWPDS::SWPDS() : FWPDS(), preprocessed(false), sgr(NULL) { }

      SWPDS::SWPDS(Wrapper *wrapper) : FWPDS(wrapper), preprocessed(false), sgr(NULL) { }

      SWPDS::~SWPDS() {
        if(sgr != NULL) {
          delete sgr;
        }
      }

      void SWPDS::addEntryPoint(Key n) {
        syms.entryPoints.insert(n);
      }

      void SWPDS::prestar(wfa::WFA &input, wfa::WFA &output) {
        if(!preprocessed) {
          *waliErr << "SWPDS: Error: Must preprocess before running query\n";
          assert(0);
        }

        *waliErr << "Error: SWPDS::prestar not yet supported\n";
        assert(0);
      }

      void SWPDS::preprocess() {
        assert(!preprocessed);
        assert(theZero.is_valid());

        if(pds_states.size() != 1) {
          *waliErr << "Error: Currently SWPDS can only handle single-state PDSs\n";
          assert(0);
        }

        Key start_state = *pds_states.begin();

        // Get all EWPDS symbols
        for_each(syms);

        // Create an automaton Agrow with transitions (start_state, e, <start_state,e>)
        // for each entry point node e.
        wfa::WFA Agrow;
        std::set<Key>::iterator it; 

        // Need to get the WFA::generation correct for the mid-states so that
        // new mid-states are not created while running poststar        
        Agrow.setGeneration(Agrow.getGeneration() + 1);
        currentOutputWFA = &Agrow; 

        for(it = syms.entryPoints.begin(); it != syms.entryPoints.end(); it++) {
          Key entry = *it;
          Agrow.addTrans(start_state, entry, gen_state(start_state, entry), theZero->one());
        }
        Agrow.setInitialState(start_state);

        // Set the generation back to original value
        Agrow.setGeneration(Agrow.getGeneration() - 1);
        currentOutputWFA = 0;

        cout << "Entry points found: " << syms.entryPoints.size() << "\n";
        
        // Then run FWPDS post* on Agrow and get the InterGraph that it creates
        wfa::WFA postAgrow;
        poststarIGR(Agrow, postAgrow);
        interGr->update_all_weights();
        
        // Create SummaryGraph from the InterGraph
        sgr = new graph::SummaryGraph(interGr.get_ptr(), start_state, syms.entryPoints, postAgrow, (graph::InterGraph::PRINT_OP)printKey);
        
        // Now do pre-processing for pre*
        
        // Add (entry -> \y) rules
        for(it = syms.gamma.begin(); it != syms.gamma.end(); it++) {
          sem_elem_t se = sgr->pushWeight(*it);
          Key entry = sgr->getEntry(*it);
          
          if(entry == WALI_EPSILON || !se.is_valid() || se->equal(se->zero())) {
            // unreachable code
            continue;
          }
          
          pre_pds.add_rule(start_state, entry, start_state, *it, se.get_ptr());
        }
        
        // Add call rules
        ewpds::CopyCallRules cr(pre_pds);
        for_each(cr);
        
        preprocessed = true;
      }

      bool SWPDS::reachable(Key k) {
        assert(preprocessed);
        return sgr->reachable(k);
      }

      void SWPDS::poststar(wfa::WFA &ca_in, wfa::WFA &ca_out) {

        if(!preprocessed) {
          *waliErr << "SWPDS: Error: Must preprocess before running query\n";
          assert(0);
        }

        if(&ca_out != &ca_in) {
          ca_out.clear();
        }

        // Needed for creating mid-states -- set before summaryPoststar
        ca_out.setGeneration(ca_in.getGeneration() + 1);

        sgr->summaryPoststar(ca_in, ca_out);

        // Set other info about the output -- see WPDS::setupOutput
        Key init = ca_in.getInitialState();
        std::set<Key> localF( ca_in.getFinalStates() );

        ca_out.addState(init,theZero);
        ca_out.setInitialState( init );
        for (std::set<Key>::iterator cit = localF.begin();
             cit != localF.end() ; cit++)
          {
            Key f = *cit;
            ca_out.addState(f,theZero);
            ca_out.addFinalState(f);
          }

      }

      void SWPDS::nonSummaryPoststar(wfa::WFA &ca_in, wfa::WFA &ca_out) {
        FWPDS::poststar(ca_in, ca_out);
      }

      void SWPDS::nonSummaryPrestar(wfa::WFA &ca_in, wfa::WFA &ca_out) {
        FWPDS::prestar(ca_in, ca_out);
      }


    } // namespace fwpds

  } // namespace wpds

} // namespace wali



