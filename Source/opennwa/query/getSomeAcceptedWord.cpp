
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "wali/TotalOrderWorklist.hpp"
#include "wali/KeyOrderWorklist.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "opennwa/Nwa.hpp"
#include "opennwa/query/returns.hpp"
#include "opennwa/query/transitions.hpp"
#include "opennwa/query/calls.hpp"
#include "opennwa/query/internals.hpp"
#include "opennwa/query/language.hpp"
#include "opennwa/nwa_pds/conversions.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/Key.hpp"
#include "wali/wfa/epr/FunctionalWeight.hpp"
#include "wali/Reach.hpp"
#include "wali/ref_ptr.hpp"
#include "opennwa/ClientInfo.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessWrapper.hpp"
#include "wali/witness/VisitorDot.hpp"
#include "wali/witness/VisitorPrinter.hpp"
#include "wali/witness/WitnessRule.hpp"


using namespace std;
using namespace wali;

namespace opennwa {
  
  namespace query {

    namespace details {
		
      class PathVisitor : public wali::witness::Visitor {
          
      private:
        /// Holds the NWA associated with the given query. We need this so
        /// that when we know that the trace went from state p to q, we can
        /// look up a symbol on that edge.
        Nwa const & nwa;

        /// This is the word that corresponds to the trace; built up as
        /// visitRule() is called.
        NestedWord word;

        /// Holds the *PDS* state and stack symbol corresponding to the
        /// "first half" of a NWA return transition. See visitRule() for
        /// more details.
        vector<wali::Key> symbs;
        vector<wali::Key> states;
          
			
      public:
				
        PathVisitor(Nwa const & orig)
          : nwa(orig)
        {}
				
        ~PathVisitor() {}

        bool visit( wali::witness::Witness * w ) {
          (void) w;
          return true;
        }
		
        bool visitExtend( wali::witness::WitnessExtend * w ) {
          (void) w;
          return true;
        }
		
        bool visitCombine( wali::witness::WitnessCombine * w ) {
          (void) w;
          return true;
        }

        /// This is basically called once for each WPDS rule taken on the
        /// PDS's "accepting" path. Builds up 'word' so it contains the
        /// actual NWA word.
        bool visitRule( wali::witness::WitnessRule * w ) {
          // There are four kinds of WPDS rules that we need to
          // handle. Internal NWA transitions correspond to (some -- see
          // in a bit) internal PDS transitions. Call NWA transitions
          // correspond to push WPDS rules. Return NWA transitions
          // correspond to a pair of WPDS rules: a pop, then an
          // internal. We will call these two WPDS rules the "first half"
          // and "second half" of the NWA transition, respectively.
          //
          // Here's how we figure each of these things out. Our goal in
          // each case is basically to figure out the source and target NWA
          // states and the type of transition, then retrieve a symbol that
          // appears on an edge meeting those descriptions. (Then we stick
          // that onto the end of 'word'.)
          //
          // 1. We detect the first half of a return transition easily --
          //    it's just a pop (delta_0) rule. When we see this, we know
          //    that the second half is coming up in a moment. We record
          //    the exit site in the stack 'symbs' (recall that the exit
          //    site -- an NWA state -- is a PDS stack symbol).
          //
          //    When we see one, we don't do anything else. (We can't: we
          //    need to know the target state first, and we won't know that
          //    until the second half.)
          //
          // 2. We detect the second half of a return transition based on
          //    whether there is a currently-recorded exit site in
          //    'symbs'. If there is, now we have the information we need
          //    to append to 'word'.
          //
          // 3. If neither of these cases applies, then we're in an "easy"
          //    case: either an NWA internal or call transition. Figuring
          //    out which simply means checking whether the WPDS rule is an
          //    internal (delta_1) rule or a push (delta_2) rule. Both of
          //    those have all the information we need to get the symbol.
          //
          // There are some sanity checks throughout... for instance, in
          // addition to recording the exit site in 'symbs', we also record
          // the WPDS state that is the target of the first half of the
          // transition, and make sure it's the same as the source node of
          // the second half of the transition.


          wali::Key from = w->getRuleStub().from_stack();
          wali::Key fromstate = w->getRuleStub().from_state();
          wali::Key to = w->getRuleStub().to_stack1();
          wali::Key to2 = w->getRuleStub().to_stack2();
          wali::Key tostate = w->getRuleStub().to_state();

          //std::cout << "visitRule(...):\n"
          //          << "  from stack [" << from << "] " << key2str(from) << "\n"
          //          << "  from state [" << fromstate << "] " << key2str(fromstate) << "\n"
          //          << "  to stack1 [" << to << "] " << key2str(to) << "\n"
          //          << "  to state  [" << tostate << "] " << key2str(tostate) << std::endl;


          // Detect a pop rule: this discovers the first half of a return
          // transition. (Case #1 above.)
          if( to == EPSILON) {
            // dealing with first half of return transition
            assert(symbs.size() == 0);
            assert(states.size() == 0);
              
            symbs.push_back(from);
            states.push_back(tostate);

            return true;
          }

          // OK, we're in one of cases #2 or q3 above. Figure out which
          // one, and put the transition type in trans_type, and the symbol
          // in 'sym'.
          wali::Key sym;
          NestedWord::Position::Type trans_type;
	
          if(states.size() > 0) {
            // Dealing with the second half of a return transition (case #2 above).
            assert(fromstate == states.back());
              
            trans_type = NestedWord::Position::ReturnType;
            set<wali::Key> r = query::getReturnSym(nwa, symbs.back(), from, to);
            assert(r.size() > 0);
            sym = *(r.begin());

            states.pop_back();
            symbs.pop_back();
          }
          else if (to2 != EPSILON) {
            // call (part of case #3)
            trans_type = NestedWord::Position::CallType;
            set<wali::Key> r = query::getCallSym(nwa, from, to);
            assert(r.size() > 0);
            sym = *(r.begin());
          }
          else {
            // internal (part of case #3)
            trans_type = NestedWord::Position::InternalType;
            set<wali::Key> r = query::getInternalSym(nwa, from, to);
            assert(r.size() > 0);
            sym = *(r.begin());
          }

          // If the transition was an epsilon transition, then we don't
          // want to save it since it isn't part of the word.
          if(sym != EPSILON) {
            word.append(NestedWord::Position(sym, trans_type));
          }
					
          return true;
        }

          
        NestedWord getPath() {return word;}

        bool visitTrans( wali::witness::WitnessTrans * w ) {
          (void) w;
          return true;
        }
		
        bool visitMerge( wali::witness::WitnessMerge * w ) {
          (void) w;
          return true;
        }
		
      };

    } // namespace query::details


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
    getSomeAcceptedWordInternal(Nwa const & nwa, WeightGen const & wg)
    {
      if (nwa.sizeInitialStates() == 0) {
        return NULL;
      }
        
      wali::wfa::WFA query;
      ref_ptr<wali::wpds::Wrapper> wrapper = new wali::witness::WitnessWrapper();
      wali::wpds::WPDS conv = nwa_pds::NwaToWpdsCalls(nwa, wg, wrapper);

      conv.setWorklist(new TotalOrderWorklist());

      wali::Key state = nwa_pds::getProgramControlLocation();
      wali::Key accept = wali::getKey("__accept");

      // Construct the query automaton
      query.set_initial_state(state);
      query.add_final_state(accept);
      for( Nwa::StateIterator initial = nwa.beginInitialStates();
           initial != nwa.endInitialStates(); initial++ )
      {
        query.addTrans(state, *initial, accept, wg.getOne());
        query.addTrans(accept, *initial, accept, wg.getOne()); // Accept pending returns
      }
	
      // Execute the post* query
      wali::wfa::WFA path = conv.poststar(query);
      path.path_summary();

      for(Nwa::StateIterator final = nwa.beginFinalStates();
          final != nwa.endFinalStates(); final++)
      {
        // See if there are any transitions to the current final state
        wali::wfa::TransSet t = path.match(state, *final);
            
        // Collect the transitions
        for(wali::wfa::TransSet::iterator trans = t.begin();
            trans != t.end(); trans++)
        {
          sem_elem_t se =
            path.getState((*trans)->to())
            ->weight()
            ->extend((*trans)->weight());
          if(se->equal(se->zero())) {
            // Doesn't make it to the final state. Try again.
            continue;
          }

          // We found an actual path to the final state, so figure out
          // how it did that.
          details::PathVisitor v(nwa);					
          wali::witness::Witness* wit =
            dynamic_cast<wali::witness::Witness*>(se.get_ptr());
          assert(wit);
          wit->accept(v);
          return new NestedWord(v.getPath());
        }
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

