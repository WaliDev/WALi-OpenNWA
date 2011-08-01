#include "wali/wfa/State.hpp"
#include "wali/wpds/ewpds/ETrans.hpp"
#include "wali/wpds/fwpds/SWPDS.hpp"
#include "wali/graph/GraphCommon.hpp"

using namespace std;

namespace wali 
{
  using wfa::ITrans;
  using graph::Transition;
  using graph::WTransition;

  namespace wpds 
  {

    namespace ewpds 
    {

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

      const std::string SWPDS::XMLTag("SWPDS");

      SWPDS::SWPDS() : FWPDS(), preprocessed(false), sgr(NULL) 
      { 
      }

      SWPDS::SWPDS(ref_ptr<Wrapper> wr) : 
        FWPDS(wr), preprocessed(false), sgr(NULL) 
      { 
      }

      SWPDS::~SWPDS() 
      {
        if(sgr != NULL) {
          delete sgr;
        }
      }

      void SWPDS::addEntryPoint(Key n) {
        syms.entryPoints.insert(n);
      }

      // Carried over from WPDS to ensure that no rules are added after
      // SWPDS is preprocessed
      bool SWPDS::make_rule(
          Config *f,
          Config *t,
          Key stk2,
	  bool replace_weight,
          rule_t& r ) 
      {

        if(preprocessed) {
          *waliErr << "[ERROR] SWPDS cannot add rules after calling preprocess.\n";
          assert(0);
        }

        return WPDS::make_rule(f,t,stk2,replace_weight,r);
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

      bool SWPDS::multiple_proc(Key k) {
        assert(preprocessed);
        return sgr->multiple_proc(k);
      }

      void SWPDS::poststar(wfa::WFA const & ca_in, wfa::WFA &ca_out) {

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
        ca_out.setQuery(wfa::WFA::REVERSE);
        Key init = ca_in.getInitialState();
        std::set<Key> localF( ca_in.getFinalStates() );

        ca_out.setInitialState( init );
        for (std::set<Key>::iterator cit = localF.begin();
             cit != localF.end() ; cit++)
          {
            Key f = *cit;
            ca_out.addFinalState(f);
          }

      }

      //TODO: Can probably implement this like poststar, where SummaryGraph
      //creates regexp for performing saturation. But this is OK for now.
      void SWPDS::prestar(wfa::WFA const & ca_in, wfa::WFA &ca_out) {
        if(!preprocessed) {
          *waliErr << "SWPDS: Error: Must preprocess before running query\n";
          assert(0);
        }

        if(&ca_out != &ca_in) {
          ca_out.operator=(ca_in);
        }

        Key start_state = *pds_states.begin();

        // Add (init,entry,init) transitions
        set<Key>::iterator it;
        for(it = syms.entryPoints.begin(); it != syms.entryPoints.end(); it++) {
          sem_elem_t se = sgr->popWeight(*it);
          if(!se->equal(se->zero())) {
            ca_out.insert(new ewpds::ETrans(start_state, *it, start_state, 0, se, 0));
          }
        }

        // Run pre* with call and (\y to entry) rules

        // Make WALi non-strict about transitions to a PDS state
        bool strict = is_strict();
        set_strict(false);
        pre_pds.prestar(ca_out, ca_out);
        set_strict(strict);

        // Process Rule1s (IntraQ)
        // For this, we take all transitions (start_state, *, q) for each q (!= start_state)
        // then pass all of these to SummaryGraph, run IntraQ, get updated
        // transitions, and add them to the automaton. If all transitions
        // leading into q are ETrans, then the added transitions are also
        // ETrans. If all are Trans, then added transitions are also Trans.
        // If its a mix of Trans and ETrans, then assert(0)

        std::map<Key, std::set<ITrans *> > state_trans_map;
        std::map<Key, std::set<ITrans* > >::iterator st_map_it;

        std::set<Key> states = ca_out.getStates();
        std::set<Key>::iterator set_it;

        // Build a map: state -> { incoming transition }
        for(set_it = states.begin(); set_it != states.end(); set_it++) {

          wfa::State* state = ca_out.getState(*set_it);          
          wfa::State::iterator tli = state->begin();;

          for( ; tli != state->end(); tli++) {

            ITrans* t = *tli;

            if(t->stack() == WALI_EPSILON)
              continue;

            // only process these transitions
            if(!(t->from() == start_state && t->to() != start_state))
              continue;
 
            // Ignore unreachable code
            if(!sgr->reachable(t->stack())) {
              printKey(*waliErr << "SWPDS: Warning: Unreachable code(", t->stack()) << ")\n";
              continue;
            }

            // pre* cannot yet handle nodes that belong to multiple procedures
            if(sgr->multiple_proc(t->stack())) {
              printKey(*waliErr << "SWPDS: Error: Pre* cannot yet handle nodes (", t->stack()) << ") that belong to multiple procedures\n";
              assert(0);
            }

            state_trans_map[t->to()].insert(t);
          }
        }

        // Iterate over all states
        for(st_map_it = state_trans_map.begin(); st_map_it != state_trans_map.end(); st_map_it++) {
          Key q = st_map_it->first;
          
          std::set<ITrans*> &trans_set = st_map_it->second;
          std::set<ITrans*>::iterator trans_it;
          bool state_has_etrans = false;
          bool  first = true;

          // Pass in the updates, keep track of transition type
          for(trans_it = trans_set.begin(); trans_it != trans_set.end(); trans_it++) {
            
            ITrans *t = *trans_it;
            Transition tt(*t);

            sgr->preAddUpdate(tt, t->weight());

            ewpds::ETrans *et = dynamic_cast<ewpds::ETrans *>(t);
            if(first) {
              state_has_etrans = (et != 0);
              first = false;
            } else {
              assert(state_has_etrans == (et != 0));
            }
          }

          // Get the updated transitions back and add them to ca_out
          list<WTransition> ls;
          list<WTransition>::iterator newit;
          sgr->preGetUpdatedTransitions(ls);

          for(newit = ls.begin(); newit != ls.end(); newit++) {
            ITrans *t;

            // Ignore return transitions created by poststar
            // TODO: there should be a better place for this check
            if(newit->first.src != start_state) continue;

            if(state_has_etrans) {
              t = new ewpds::ETrans(newit->first.src, newit->first.stack, q, 0, newit->second, 0);
            } else {
              t = new wfa::Trans(newit->first.src, newit->first.stack, q, newit->second);
            }

            ca_out.insert(t);
          }

        }

        // Done with IntraQ

        // Add (init, *, init) transitions
        for(it = syms.gamma.begin(); it != syms.gamma.end(); it++) {
          if(syms.entryPoints.find(*it) != syms.entryPoints.end()) continue;
          sem_elem_t se = sgr->popWeight(*it);
          if(!se->equal(se->zero())) {
            ca_out.insert(new ewpds::ETrans(start_state, *it, start_state, 0, se, 0));
          }
        }

        // Not really needed, but if WPDS does it, so should SWPDS
        ca_out.setGeneration(ca_in.getGeneration() + 1);

        // Set other info about the output -- see WPDS::setupOutput
        ca_out.setQuery(wfa::WFA::INORDER);
        Key init = ca_in.getInitialState();
        std::set<Key> localF( ca_in.getFinalStates() );

        ca_out.setInitialState( init );
        for (std::set<Key>::iterator cit = localF.begin();
             cit != localF.end() ; cit++)
          {
            Key f = *cit;
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



