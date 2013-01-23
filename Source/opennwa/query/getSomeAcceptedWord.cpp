
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
#include "wali/witness/CalculatingVisitor.hpp"


using namespace std;
using namespace wali;

namespace opennwa {
  
  namespace query {

    namespace details {

      class PathVisitor : public witness::CalculatingVisitor<NestedWord> {

      private:
        /// Holds the NWA associated with the given query. We need this so
        /// that when we know that the trace went from state p to q, we can
        /// look up a symbol on that edge.
        Nwa const & nwa;

        /// Holds the *PDS* state and stack symbol corresponding to the
        /// "first half" of a NWA return transition. See visitRule() for
        /// more details.
        map<Key,Key> states;

      public:

        PathVisitor(Nwa const & orig)
          : nwa(orig)
        {}

        ~PathVisitor() {}

        // Concatenates the left and right child.
        NestedWord calculateExtend(witness::WitnessExtend * w, NestedWord& left, NestedWord& right) {
          (void) w;

          NestedWord ret(left);
          for (NestedWord::const_iterator it=right.begin(); it!=right.end(); it++) {
            ret.append(*it);
          }
          return ret;
        }

        // Choses the shortest child.
        NestedWord calculateCombine(witness::WitnessCombine * w, list<NestedWord>& children) {
          (void) w;
          
          size_t min = INT_MAX;
          NestedWord ret;
          for (list<NestedWord>::const_iterator it=children.begin(); it!=children.end(); it++) {
            NestedWord child = *it;
            if (child.size() <= min) {
              min = child.size();
              ret = child;
            }
          }
          
          return ret;
        }

        // Not implemented.
        NestedWord calculateMerge(witness::WitnessMerge* w, NestedWord& callerValue, NestedWord& ruleValue, NestedWord& calleeValue) {
          (void) w;
          (void) callerValue;
          (void) calleeValue;

          assert (0 && "Merge not implemented!");
          return ruleValue;
        }

        /// Identifies the 'letter' represented by the rule.
        NestedWord calculateRule(witness::WitnessRule * w) {
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


          Key from = w->getRuleStub().from_stack();
          Key fromstate = w->getRuleStub().from_state();
          Key to = w->getRuleStub().to_stack1();
          Key to2 = w->getRuleStub().to_stack2();
          Key tostate = w->getRuleStub().to_state();

          //cout << "visitRule(...):\n"
          //          << "  from stack [" << from << "] " << key2str(from) << "\n"
          //          << "  from state [" << fromstate << "] " << key2str(fromstate) << "\n"
          //          << "  to stack1 [" << to << "] " << key2str(to) << "\n"
          //          << "  to state  [" << tostate << "] " << key2str(tostate) << endl;

          //w->print(cerr);

          // Detect a pop rule: this discovers the first half of a return
          // transition. (Case #1 above.)
          if (to == EPSILON) {
            // dealing with first half of return transition
            states[tostate] = from;

            return NestedWord();
          }

          // OK, we're in one of cases #2 or #3 above. Figure out which
          // one, and put the transition type in trans_type, and the symbol
          // in 'sym'.
          Key sym;
          NestedWord::Position::Type trans_type;
          NestedWord ret;

          map<Key,Key>::const_iterator it = states.find(fromstate);
          if (it != states.end()) {
            // Dealing with the second half of a return transition (case #2 above).
            trans_type = NestedWord::Position::ReturnType;
            set<Key> r = query::getReturnSym(nwa, it->second, from, to);
            assert(r.size() > 0);
            sym = *(r.begin());
          }
          else if (to2 != EPSILON) {
            // call (part of case #3)
            trans_type = NestedWord::Position::CallType;
            set<Key> r = query::getCallSym(nwa, from, to);
            assert(r.size() > 0);
            sym = *(r.begin());
          }
          else {
            // internal (part of case #3)
            trans_type = NestedWord::Position::InternalType;
            set<Key> r = query::getInternalSym(nwa, from, to);
            assert(r.size() > 0);
            sym = *(r.begin());
          }

          // If the transition was an epsilon transition, then we don't
          // want to save it since it isn't part of the word.
          if (sym != EPSILON) {
            ret.append(NestedWord::Position(sym, trans_type));
          }

          return ret;
        }

        // Return an empty nested word.
        NestedWord calculateTrans(witness::WitnessTrans * w) {
          (void) w;
          return NestedWord();
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
    getSomeShortestAcceptedWordWithWeights(Nwa const & nwa, WeightGen const & wg)
    {
      return getSomeAcceptedWordInternal(nwa, wg);
    }

      
    ref_ptr<NestedWord>
    getSomeAcceptedWordInternal(Nwa const & nwa, WeightGen const & wg)
    {
      if (nwa.sizeInitialStates() == 0 || nwa.sizeFinalStates() == 0) {
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

      // Intersect path with a new 2-state WFA with a transitioning edge
      // for each final NWA state, and a self-loop from the WFA final
      // state on each non-final NWA state.
      wali::wfa::WFA fa;
      wali::Key init = wali::getKey("__init");
      fa.addState(init, wg.getOne());
      fa.setInitialState(init);
      wali::Key fin = wali::getKey("__final");
      fa.addState(fin, wg.getOne());
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
      
      path.path_summary();

      sem_elem_t se = path.getState(path.getInitialState())->weight();
      witness::Witness *wit =
        dynamic_cast<witness::Witness*>(se.get_ptr());
      assert(wit);

      if (!wit->isZero()) {

        // Find a shortest word through the witness.
        query::details::PathVisitor pv(nwa);
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

