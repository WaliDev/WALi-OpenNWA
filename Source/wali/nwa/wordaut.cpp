
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/returns.hpp"
#include "wali/nwa/query/transitions.hpp"
#include "wali/nwa/query/calls.hpp"
#include "wali/nwa/query/internals.hpp"
#include "wali/nwa/query/language.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/Key.hpp"
#include "wali/wfa/epr/FunctionalWeight.hpp"
#include "wali/Reach.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/nwa/ClientInfo.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessWrapper.hpp"
#include "wali/witness/VisitorDot.hpp"
#include "wali/witness/VisitorPrinter.hpp"
#include "wali/witness/WitnessRule.hpp"

using namespace std;
using namespace wali;
using namespace wali::nwa;

// This is exactly like NWAtoPDScalls, except when the return WPDS is
// constructed, a new WitnessWrapper is passed to it.
// This is needed for our PathVisitor class, which collects transition symbols.
//
// NOTE: This should be placed in NWA.hpp
//
wpds::WPDS
NWA::_private_NWAtoPDScallsWitness( WeightGen & wg ) const
{
  //TODO: beware the implicit transitions
  //Q: do we need to make all transitions explicit in order to make this correct?
  
  wali::witness::WitnessWrapper* wrapper = new wali::witness::WitnessWrapper();
  wpds::WPDS result = wpds::WPDS(wrapper);
  
  Key program = getProgramControlLocation();  // = wali::getKey("program");
  
  wali::sem_elem_t wgt;
  
  //Internal Transitions
  for( InternalIterator iit = trans.beginInternal(); iit != trans.endInternal(); iit++ )
  {  
    // (q,sigma,q') in delta_i goes to <p,q> -w-> <p,q'> in delta_1
    // where the weight w depends on sigma
      
    State src = Trans::getSource(*iit);
    State tgt = Trans::getTarget(*iit);
      
    if( Trans::getInternalSym(*iit) == WALI_WILD )
      wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w
    else
      wgt = wg.getWeight(src, getClientInfo(src),
                         Trans::getInternalSym(*iit),
                         WeightGen::INTRA,
                         tgt, getClientInfo(tgt));           // w
      
    result.add_rule(program,                                //from_state (p)
                    src,                                    //from_stack (q)
                    program,                                //to_state (p)
                    tgt,                                    //to_stack1 (q')
                    wgt);                                   //weight (w)      
  }
  
  //Call Transitions
  for( CallIterator cit = trans.beginCall(); cit != trans.endCall(); cit++ )
  {           
    // (q_c,sigma,q_e) in delta_c goes to
    // <p,q_c> -w-> <p,q_e q_c> in delta_2 
    // and the weight w depends on sigma
      
    State src = Trans::getCallSite(*cit);
    State tgt = Trans::getEntry(*cit);
      
    if( Trans::getCallSym(*cit) == WALI_WILD )
      wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt)); // w
    else
      wgt = wg.getWeight(src, getClientInfo(src),
                         Trans::getCallSym(*cit),
                         WeightGen::CALL_TO_ENTRY,
                         tgt, getClientInfo(tgt));          // w
      
    result.add_rule(program,                                //from_state (p)
                    src,                                    //from_stack (q_c)
                    program,                                //to_state (p)
                    Trans::getEntry(*cit),                  //to_stack1 (q_e)
                    src,                                    //to_stack2 (q_c)
                    wgt);                                   //weight (w)  
  } 
  
  //Return Transitions
  int r_count = 0;
  for( ReturnIterator rit = trans.beginReturn(); rit != trans.endReturn(); rit++ )
  {
    ++r_count;
      
    //std::cerr << "Return transition #" << r_count << " of " << trans.size() << "\n";
    // (q_x,q_c,sigma,q_r) in delta_r goes to 
    // <p,q_x> -w-> <p_q_xcr,epsilon> in delta_0
    // and <p_q_xcr,q_c> -1-> <p,q_r> in delta_1
    // where p_q_xcr = (p,q_x,q_c,q_r), and w depends on sigma
      
    State src = Trans::getExit(*rit);
    State tgt = Trans::getReturnSite(*rit);
      
    if( Trans::getReturnSym(*rit) == WALI_WILD )
      wgt = wg.getWildWeight(src,getClientInfo(src),tgt,getClientInfo(tgt));  // w 
    else
      wgt = wg.getWeight(src, getClientInfo(src), 
                         Trans::getReturnSym(*rit),
                         WeightGen::EXIT_TO_RET,  
                         tgt, getClientInfo(tgt));    // w     
      
    //Note: if you change this, make sure you modify the code in NWPForest.createCA()
    Key rstate = getControlLocation(src,Trans::getCallSite(*rit),tgt);  //p_q_xcr
      
    result.add_rule(program,                              //from_state (p)
                    src,                                  //from_stack (q_x)
                    rstate,                               //to_state (p_q_xcr == (p,q_x,q_c,q_r))
                    wgt);                                 //weight (w)
      
      
    wgt = wg.getOne();                                    // 1                      
      
    result.add_rule(rstate,                               //from_state (p_q_xcr == (p,q_x,q_c,q_r))
                    Trans::getCallSite(*rit),             //from_stack (q_c)
                    program,                              //to_state (p)
                    tgt,                                  //to_stack (q_r)
                    wgt);                                 //weight (1)
  }
  
  return result;
}



namespace wali {
  
  namespace nwa {

    namespace query {

      namespace details {
		
        class PathVisitor : public wali::witness::Visitor {
          
        private:
          /// Holds the NWA associated with the given query. We need this so
          /// that when we know that the trace went from state p to q, we can
          /// look up a symbol on that edge.
          wali::nwa::NWA const * o;

          /// This is the word that corresponds to the trace; built up as
          /// visitRule() is called.
          NestedWord word;

          /// Holds the *PDS* state and stack symbol corresponding to the
          /// "first half" of a NWA return transition. See visitRule() for
          /// more details.
          vector<wali::Key> states;
          vector<wali::Key> symbs;
			
        public:
				
          PathVisitor(wali::nwa::NWA const * orig)
            : o(orig)
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
            if( to == wali::WALI_EPSILON) {
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
              set<wali::Key> r = query::getReturnSym(*o, symbs.back(), from, to);
              assert(r.size() > 0);
              sym = *(r.begin());

              states.pop_back();
              symbs.pop_back();
            }
            else if (to2 != WALI_EPSILON) {
              // call (part of case #3)
              trans_type = NestedWord::Position::CallType;
              set<wali::Key> r = query::getCallSym(*o,from,to);
              assert(r.size() > 0);
              sym = *(r.begin());
            }
            else {
              // internal (part of case #3)
              trans_type = NestedWord::Position::InternalType;
              set<wali::Key> r = query::getInternalSym(*o,from,to);
              assert(r.size() > 0);
              sym = *(r.begin());
            }

            // If the transition was an epsilon transition, then we don't
            // want to save it since it isn't part of the word.
            if(sym != WALI_EPSILON) {
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

      } // namespace wali::nwa::query::details


      /// Given an automaton 'aut', return some word in its language.
      ///
      /// If L(aut)={}, right now there is no way to distinguish that case
      /// from the case where L(aut) contains epsilon, and that's what the
      /// function returns. (You can, of course, call languageContains to see
      /// if epsilon is in the language.)
      NestedWord getWord(wali::nwa::NWA const * aut) {
  
        if(!query::languageIsEmpty(*aut)) {
    
          wali::nwa::ReachGen wg;
          wali::wfa::WFA query;
          wali::wpds::WPDS conv = aut->_private_NWAtoPDScallsWitness(wg);
    
          wali::Key state = getProgramControlLocation();
          wali::Key accept = wali::getKey("__accept");

          // Construct the query automaton
          query.set_initial_state(state);
          query.add_final_state(accept);
          for( NWA::StateIterator initial = aut->beginInitialStates();
               initial != aut->endInitialStates(); initial++ )
          {
            query.addTrans(state, *initial, accept, wg.getOne());
            query.addTrans(accept, *initial, accept, wg.getOne()); // Accept pending returns
          }
	
          // Execute the post* query
          wali::wfa::WFA path = conv.poststar(query);
          path.path_summary();
          
          for(NWA::StateIterator final = aut->beginFinalStates();
              final != aut->endFinalStates(); final++)
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
              details::PathVisitor v(aut);					
              wali::witness::Witness* wit =
                dynamic_cast<wali::witness::Witness*>(se.get_ptr());
              assert(wit);
              wit->accept(v);
              return v.getPath();
            }
          }
        }

        return NestedWord();
      }

    } // namespace wali::nwa::query
  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

