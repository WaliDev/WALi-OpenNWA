#define EPSILON_CLOSE_WITH_FWPDS 1

/*!
 * @author Evan Driscoll
 */

#include "wali/Common.hpp"
#include "wali/DefaultWorklist.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/WeightMaker.hpp"
#include "wali/regex/AllRegex.hpp"
#include "wali/wpds/GenKeySource.hpp"
#include "wali/wfa/DeterminizeWeightGen.hpp"
#include "wali/wpds/WPDS.hpp"

#include "wali/wpds/fwpds/FWPDS.hpp"
#undef COMBINE // grumble grumble swear swear
#undef EXTEND

#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <iterator>


namespace wali
{
  namespace wfa
  {
    WFA::AccessibleStateMap
    WFA::epsilonCloseCached(Key state, WFA::EpsilonCloseCache & cache) const
    {
      EpsilonCloseCache::iterator loc = cache.find(state);

      if (loc != cache.end()) {
        return loc->second;
      }
      else {
        AccessibleStateMap eclose = this->epsilonClose(state);
        cache[state] = eclose;
        return eclose;
      }
    }

    WFA::AccessibleStateMap
    WFA::epsilonClose(Key state) const
    {
      return this->epsilonCloseFwpds(state);
    }
    
    
    bool
    is_epsilon_trans(ITrans const * trans)
    {
      return trans->stack() == WALI_EPSILON;
    }

      
    WFA::AccessibleStateMap
    WFA::epsilonCloseFwpds(Key source) const
    {
      Key p_state = getKey("__p");
      Key query_accept = getKey("__accept");
      sem_elem_t zero = getSomeWeight()->zero();

      // Convert this WFA into a WPDS so that we can run poststar
      wali::wpds::fwpds::FWPDS wpds;
      this->toWpds(p_state, &wpds, is_epsilon_trans);
      wpds.topDownEval(false);

      // Set up the query automaton:
      //                    source
      //     p_state -----------> ((query_accept))
      //                   weight 1
      //
      // source is what we are doing the epsilon closure from.
      WFA query;
      query.addState(p_state, zero);
      query.addState(query_accept, zero);
      query.setInitialState(p_state);
      query.addFinalState(query_accept);
      query.addTrans(p_state, source, query_accept, zero->one());

      // Issue poststar. Be careful to not force the output a la what Cindy
      // was doing! (Actually I think we look at all the weights anyway so
      // that shouldn't matter, but whatever.)
      WFA const & result = wpds.poststar(query);

#if 0
      std::cout << "\n";
      std::cout << "=====================================\n";
      std::cout << "WPDS:\n";
      wpds.print(std::cout);
      std::cout << "======================================\n";
      std::cout << "query automaton:\n";
      query.print(std::cout);
      std::cout << "======================================\n";
      std::cout << "result automaton:\n";
      result.print(std::cout);
      std::cout << "=====================================\n";
      std::cout << "\n";
#endif

      // The 'result' automaton should be something like
      //
      //           -------------->
      //   p_state --------------> ((query_accept))
      //           -------------->
      //
      // for lots of symbols S -- which are states in 'this' WFA. Each symbol
      // S is in the epsilon close of 'source', so add it.
      //
      // Because of the representation of transitions, we again need to
      // iterate over each (start, sym) pair then over the TransSet -- but in
      // this case, there should only ever be one source, which is
      // p_state, and for each (start, sym) there should only be one
      // transition, to query_accept.
      WFA::AccessibleStateMap accessible;

      for (kp_map_t::const_iterator kp_iter = result.kpmap.begin();
           kp_iter != result.kpmap.end(); ++kp_iter)
      {
        Key start = kp_iter->first.first; // should be p_state
        Key sym = kp_iter->first.second;  // a state in eclose(source)
        TransSet const & transitions = kp_iter->second;
        
        assert(start == p_state);
        assert(transitions.size() == 1u);

        // Pull out the one transition; do sanity checks
        ITrans * trans = *(transitions.begin());
        assert(trans->to() == query_accept);
        //assert(!trans->weight()->equal(zero)); // actually this should be fine to remove

        // Now get the weight. That's the net weight from 'source' to 'sym',
        // where 'sym' is actually a state in 'this' WFA.
        accessible[sym] = trans->weight();
      }

      return accessible;
    }

    
    WFA::AccessibleStateMap
    WFA::epsilonCloseMohri(Key start) const
    {
      std::set<Key> worklist;

      // Follows Fig 3 from 'Generic epsilon-Removal Algorithm for Weighted
      // Automata" by Mohri.
      std::map<Key, sem_elem_t> d; // estimates the shortest distance from start to key
      std::map<Key, sem_elem_t> r; // weight added to d[q] since last time q was visited

      sem_elem_t zero = getSomeWeight()->zero();

      // Inialization
      for (std::set<Key>::const_iterator p = Q.begin(); p != Q.end(); ++p) {
        d[*p] = r[*p] = zero;
      }
      d[start] = r[start] = getSomeWeight()->one();
      worklist.insert(start);


      //std::cout << "--- epsilonClose(" << key2str(start) << ")\n";

      // Worklist loop
      while (worklist.size() > 0) {
        Key q = *worklist.begin();
        worklist.erase(worklist.begin());
        sem_elem_t r_q = r[q]; // the change in q's weight since the last time it was visited; just 'r' in the paper
        r[q] = zero;

        //std::cout << "---   dequeued " << key2str(q) << "\n";

        // so we want to look at all outgoing transitions from 'q'.
        // kp_map_t maps from (source * stack) -> {trans}. Furthermore,
        // this is a hash map so is not in any particular order, which
        // means we have to loop over the whole dang thing. (If we had an
        // explicit list of symbols we could do (for every symbol) but we
        // can't.)
        //
        // The next three indentation levels are like 'for each e in E[q]' in
        // the paper except that 'e' is called 'transition'
        for(kp_map_t::const_iterator group = kpmap.begin();
            group != kpmap.end(); ++group)
        {
          if (group->first.first == q
              && group->first.second == WALI_EPSILON)
          {
            //std::cout << "---     found some epsilon transitions\n";
            TransSet const & transitions = group->second;
            for (TransSet::const_iterator transition = transitions.begin();
                 transition != transitions.end(); ++transition)
            {             
              // 'for each transition in E[q]'. Now we have one. The
              // following variables aren't explicitly named in the paper,
              // but I do here because either my names are too long (for
              // 'next') or to reduce caching.
              Key next = (*transition)->to();                // 'n[e]'
              sem_elem_t w_e = (*transition)->weight();      // 'w[e]'
              sem_elem_t delta = r_q->extend(w_e);           // 'r*w[e]'
              sem_elem_t new_d_n = d[next]->combine(delta); // 'd[n[e]] + (r*w[e])'

              // std::cout << "---     found transition to " << key2str(next) << "\n";
              // w_e->print(std::cout << "---         the edge weight is ") << "\n";
              // r_q->print(std::cout << "---         and the r weight is ") << "\n";
              // delta->print(std::cout << "---         so delta is ") << ";\n";
              // d[next]->print(std::cout << "---         the old weight on " << key2str(next) << " was ") << "\n";
              //new_d_n->print(std::cout << "---         so the new weight would be ") << "\n";
              
              if (!new_d_n->equal(d[next])) {
                //std::cout << "---       so I updated it's reachability amount and re-enqueued\n";
                d[next] = new_d_n;
                r[next] = r[next]->combine(delta);
                worklist.insert(next);
              }
              
            } // for each outgoing transition from q...
          }  // ..
        } // ...for each outgoing transition from q
      } // while (!worklist.empty())

      //d[start] = zero->one();

      //std::cout << "---   done finding state weights\n";
      //std::cout << "---   now removing nonzero states\n";

      AccessibleStateMap out;
      for (AccessibleStateMap::const_iterator iter = d.begin();
           iter != d.end(); ++iter)
      {
        if (iter->second != zero) {
          //iter->second->print(std::cout << "---     found state " << key2str(iter->first) << " with weight ") << "\n";
          assert(!iter->second->equal(zero));
          out[iter->first] = iter->second;
        }
      }
      
      return out;
    }
    
  } // namespace wfa

} // namespace wali

// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
