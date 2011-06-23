#include "wali/nwa/traverse/depth.hpp"

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/query/internals.hpp"
#include "wali/nwa/query/returns.hpp"
#include "wali/nwa/query/calls.hpp"

namespace wali {
  namespace nwa {
    namespace traverse {
      namespace details {
        typedef ConstCallStringStateFunctor::CallString CallString;

        StateSet extractTargets(std::set<std::pair<Symbol,State> > pairs)
        {
          typedef std::set<std::pair<Symbol,State> >::const_iterator Iterator;
          StateSet targets;

          for (Iterator iter = pairs.begin(); iter != pairs.end(); ++iter) {
            targets.insert(iter->second);
          }

          return targets;
        }
        
        
        /// Performs a depth-first traversal of the transition graph of
        /// 'nwa', starting from 'state'. Returns the set of return nodes
        /// from the current "procedure".
        ///
        /// It calls 'functor' with the given state and call string. When it
        /// reaches a call transition, for the next recursive call it pushes
        /// 'state' onto the call string. When it reaches a return
        /// transition, it compares the call predecessor on that transition
        /// to the actual call site on the top of the call string. If it
        /// matches, then the return site of that transition is added to the
        /// return set. If it does not match, it is discarded.
        StateSet dfsCallStringTraversalHelper(NWA const & nwa,
                                              CallString const & cs,
                                              State state,
                                              StateSet & visited_in_this_procedure,
                                              ConstCallStringStateFunctor & functor)
        {
          // These are the return sites that we have found are reachable
          StateSet return_sites;

          // Make sure we haven't visited this node before.
          if (visited_in_this_procedure.count(state) > 0) {
            return return_sites;
          }

          
          // Call the functor
          functor(nwa, cs, state);

          // Used for all three transitions
          StateSet targets;

          ///////////////
          // Internal transitions.
          targets = extractTargets(query::getTargets(nwa, state));
          for (StateSet::const_iterator iter = targets.begin();
               iter != targets.end(); ++iter)
          {
            State target = *iter;
            
            // For the recursive call, 'nwa' and 'functor' obviously
            // unchanged. 'cs' is unchanged because this is an internal
            // transition. 'visited_in_this_procedure' is the same object
            // because it's an internal transition.
            StateSet new_returns = dfsCallStringTraversalHelper(nwa, cs, target,
                                                                visited_in_this_procedure,
                                                                functor);

            // If we found any return sites reachable from 'target', they
            // are reachable from 'state' as well.
            return_sites.insert(new_returns.begin(), new_returns.end());
          }

          ///////////////
          // Call transitions
          targets = extractTargets(query::getEntries(nwa, state));
          for (StateSet::const_iterator iter = targets.begin();
               iter != targets.end(); ++iter)
          {
            State target = *iter;

            // Now we set up stuff for the recursive call. First, we need a
            // new set since we're starting a new procedure.
            StateSet visited_in_the_next_procedure;

            // Second, we need a new call string with 'state' on it.
            CallString new_cs = cs;
            new_cs.push_back(state);
            
            // Now we can make the recursive call.
            StateSet return_succs = dfsCallStringTraversalHelper(nwa, new_cs, target,
                                                                 visited_in_the_next_procedure,
                                                                 functor);


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
                                                                  functor);

              // If we found any return sites reachable from 'target', they
              // are reachable from 'state' as well.
              return_sites.insert(new_returns.begin(), new_returns.end());
            }

          }

          /////////////////
          // Return transitions
          targets = extractTargets(query::getReturns(nwa, state, cs.back()));

          // Return transitions don't mark anything we need to traverse
          // now. We will traverse those when we return back to the calling
          // function. But we do need to tell it that it's reachable.
          return_sites.insert(targets.begin(), targets.end());

          return return_sites;        
        }
        
      } // namespace details

      void dfsCallStringTraversal(NWA const & nwa, ConstCallStringStateFunctor & functor)
      {
        details::CallString cs;
        cs.push_back(getKey("<below main>"));
        
        StateSet visited_in_main;
        for (NWA::StateIterator initial = nwa.beginInitialStates();
             initial != nwa.endInitialStates(); ++initial)
        {
          details::dfsCallStringTraversalHelper(nwa, cs, *initial, visited_in_main, functor);
        }

      }

      
    } // namespace traverse
  } // namespcae nwa
} // namespace wali



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

