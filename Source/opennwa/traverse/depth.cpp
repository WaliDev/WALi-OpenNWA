#include "opennwa/traverse/depth.hpp"

#include "opennwa/Nwa.hpp"
#include "opennwa/query/internals.hpp"
#include "opennwa/query/returns.hpp"
#include "opennwa/query/calls.hpp"

namespace opennwa {
  namespace traverse {
    namespace details {
      typedef ConstCallStringStateFunctor::CallString CallString;
      typedef std::set<std::pair<Symbol,State> > SymbolStatePairSet;


      void
      handle_internals(Nwa const & nwa, CallString const & cs, State state,
                       StateSet & visited, ConstCallStringStateFunctor * state_functor,
                       ConstCallStringTransitionFunctor * trans_functor,
                       StateSet & return_sites);
      void
      handle_calls(Nwa const & nwa, CallString const & cs, State state,
                   StateSet & visited, ConstCallStringStateFunctor * state_functor,
                   ConstCallStringTransitionFunctor * trans_functor,
                   StateSet & return_sites);

      void
      handle_returns(Nwa const & nwa, CallString const & cs, State state,
                     ConstCallStringTransitionFunctor * trans_functor,
                     StateSet & return_sites);

        
      /// Performs a depth-first traversal of the transition graph of
      /// 'nwa', starting from 'state'. Returns the set of return nodes
      /// from the current "procedure".
      ///
      /// When it reaches a call transition, for the next recursive call it
      /// pushes 'state' onto the call string. When it reaches a return
      /// transition, it compares the call predecessor on that transition
      /// to the actual call site on the top of the call string. If it
      /// matches, then the return site of that transition is added to the
      /// return set. If it does not match, it is discarded.
      ///
      /// If 'state_functor' is non-null, calls it for each new state. If
      /// 'trans_functor' is set, calls it for each transition in the
      /// expanded nw.
      StateSet
      dfsCallStringTraversalHelper(Nwa const & nwa,
                                   CallString const & cs,
                                   State state,
                                   StateSet & visited_in_this_procedure,
                                   ConstCallStringStateFunctor * state_functor,
                                   ConstCallStringTransitionFunctor * trans_functor)
      {
        // These are the return sites that we have found are reachable
        StateSet return_sites;

        // Make sure we haven't visited this node before.
        if (visited_in_this_procedure.count(state) > 0) {
          return return_sites;
        }

          
        // Call the state functor
        if (state_functor) {
          (*state_functor)(nwa, cs, state);
        }

        // Call helper functions to do the actual traversal
        handle_internals(nwa, cs, state, visited_in_this_procedure,
                         state_functor, trans_functor, return_sites);
        handle_calls(nwa, cs, state, visited_in_this_procedure,
                     state_functor, trans_functor, return_sites);
        handle_returns(nwa, cs, state, trans_functor, return_sites);

        return return_sites;        
      }


      ///////////////
      // Internal transitions.
      void
      handle_internals(Nwa const & nwa, CallString const & cs, State state,
                       StateSet & visited, ConstCallStringStateFunctor * state_functor,
                       ConstCallStringTransitionFunctor * trans_functor,
                       StateSet & return_sites)
      {
        SymbolStatePairSet targets = query::getTargets(nwa, state);
          
        for (SymbolStatePairSet::const_iterator iter = targets.begin();
             iter != targets.end(); ++iter)
        {
          Symbol symbol = iter->first;
          State target = iter->second;

          // If the user requested a trans_functor, call it
          if (trans_functor) {
            trans_functor->doInternal(nwa, cs, state, symbol, target);
          }
            
          // For the recursive call, 'nwa' and 'functor' obviously
          // unchanged. 'cs' is unchanged because this is an internal
          // transition. 'visited_in_this_procedure' is the same object
          // because it's an internal transition.
          StateSet new_returns = dfsCallStringTraversalHelper(nwa, cs, target,
                                                              visited,
                                                              state_functor,
                                                              trans_functor);

          // If we found any return sites reachable from 'target', they
          // are reachable from 'state' as well.
          return_sites.insert(new_returns.begin(), new_returns.end());
        }
      }

        
      ///////////////
      // Call transitions
      void
      handle_calls(Nwa const & nwa, CallString const & cs, State state,
                   StateSet & visited_in_this_procedure,
                   ConstCallStringStateFunctor * state_functor,
                   ConstCallStringTransitionFunctor * trans_functor,
                   StateSet & return_sites)
      {
        SymbolStatePairSet targets = query::getEntries(nwa, state);
          
        for (SymbolStatePairSet::const_iterator iter = targets.begin();
             iter != targets.end(); ++iter)
        {
          Symbol symbol = iter->first;
          State target = iter->second;

          // If the user gave us a trans_functor, call it
          if (trans_functor) {
            trans_functor->doCall(nwa, cs, state, symbol, target);
          }            

            
          // Now we set up stuff for the recursive call. First, we need a
          // new set since we're starting a new procedure.
          StateSet visited_in_the_next_procedure;

          // Second, we need a new call string with 'state' on it.
          CallString new_cs = cs;
          new_cs.push_back(state);
            
          // Now we can make the recursive call.
          StateSet return_succs = dfsCallStringTraversalHelper(nwa, new_cs, target,
                                                               visited_in_the_next_procedure,
                                                               state_functor,
                                                               trans_functor);


          // But we're not done. What that recursive call returns is the
          // set of nodes in THIS procedure that match this call site (if
          // any). Those are also all reachable. (In actuality, they are
          // reachable by a path through the called
          // procedure. Conceptually, it's reachable by a "virtual"
          // internal edge from the call site to the return site. We just
          // can't figure out what the return sites are easily.)
          //
          // In any case, we need to do the same thing as the internal
          // transitions above.
          for (StateSet::const_iterator return_iter = return_succs.begin();
               return_iter != return_succs.end(); ++return_iter)
          {
            StateSet new_returns = dfsCallStringTraversalHelper(nwa, cs, *return_iter,
                                                                visited_in_this_procedure,
                                                                state_functor,
                                                                trans_functor);

            // If we found any return sites reachable from 'target', they
            // are reachable from 'state' as well.
            return_sites.insert(new_returns.begin(), new_returns.end());
          }
        } // for each transition (sym/tgt pair)
      }


      /////////////////
      // Return transitions
      void
      handle_returns(Nwa const & nwa, CallString const & cs, State state,
                     ConstCallStringTransitionFunctor * trans_functor,
                     StateSet & return_sites)
      {
        State call_pred = cs.back();
          
        SymbolStatePairSet targets = query::getReturns(nwa, state, call_pred);

        for (SymbolStatePairSet::const_iterator iter = targets.begin();
             iter != targets.end(); ++iter)
        {
          State symbol = iter->first;
          State target = iter->second;

          // If the user gave us a trans_functor, use it
          if (trans_functor) {
            CallString cs_copy = cs;
            cs_copy.pop_back();
            trans_functor->doReturn(nwa, cs_copy, state, call_pred, symbol, target);
          }

          // Return transitions don't mark anything we need to traverse
          // now. We will traverse those when we return back to the calling
          // function. But we do need to tell it that it's reachable.
          return_sites.insert(target);
        }
      }

    } // namespace details

        
    void dfsCallStringTraversal(Nwa const & nwa,
                                ConstCallStringStateFunctor * state_functor,
                                ConstCallStringTransitionFunctor * trans_functor)
    {
      details::CallString cs;
      cs.push_back(getKey("<below main>"));
        
      StateSet visited_in_main;
      for (Nwa::StateIterator initial = nwa.beginInitialStates();
           initial != nwa.endInitialStates(); ++initial)
      {
        details::dfsCallStringTraversalHelper(nwa, cs, *initial, visited_in_main,
                                              state_functor, trans_functor);
      }

    }

      
  } // namespace traverse
} // namespace opennwa



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

