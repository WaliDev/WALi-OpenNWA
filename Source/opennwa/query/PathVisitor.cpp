
#include "opennwa/query/returns.hpp"
#include "opennwa/query/calls.hpp"
#include "opennwa/query/internals.hpp"
#include "opennwa/query/PathVisitor.hpp"


namespace opennwa {

  namespace query {

    using namespace wali;

    bool PathVisitor::visitRule( witness::WitnessRule * w ) {
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
        states[tostate] = from;

        return true;
      }

      // OK, we're in one of cases #2 or q3 above. Figure out which
      // one, and put the transition type in trans_type, and the symbol
      // in 'sym'.
      wali::Key sym;
      NestedWord::Position::Type trans_type;

      std::map<Key,Key>::const_iterator it = states.find(fromstate);
      if (it != states.end()) {
        // Dealing with the second half of a return transition (case
        // #2 above).
        trans_type = NestedWord::Position::ReturnType;
        std::set<wali::Key> r = query::getReturnSym(nwa, it->second, from, to);
        assert(r.size() > 0);
        sym = *(r.begin());
      }
      else if (to2 != EPSILON) {
        // call (part of case #3)
        trans_type = NestedWord::Position::CallType;
        std::set<wali::Key> r = query::getCallSym(nwa, from, to);
        assert(r.size() > 0);
        sym = *(r.begin());
      }
      else {
        // internal (part of case #3)
        trans_type = NestedWord::Position::InternalType;
        std::set<wali::Key> r = query::getInternalSym(nwa, from, to);
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

  }

}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

