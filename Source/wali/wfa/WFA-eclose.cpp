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
#include "wali/KeyPairSource.hpp"
#include "wali/wpds/GenKeySource.hpp"
#include "wali/domains/SemElemSet.hpp"

#include "wali/wpds/fwpds/FWPDS.hpp"
#undef COMBINE // grumble grumble swear swear
#undef EXTEND

#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <iterator>

using wali::WALI_EPSILON;
using wali::wfa::WFA;
using wali::wfa::ITrans;
using wali::domains::SemElemSet;

namespace
{
  int
  num_trans(WFA const & wfa)
  {
    wali::wfa::TransCounter ctr;
    wfa.for_each(ctr);
    return ctr.getNumTrans();
  }

  
  bool
  is_epsilon_transition(ITrans const * trans)
  {
    return trans->stack() == WALI_EPSILON;
  }
  

  bool
  is_any_transition(ITrans const * trans)
  {
    return true;
  }


  class SemElemSetLifter : public wali::wfa::ConstTransFunctor
  {
    WFA & target;
    
  public:
    SemElemSetLifter(WFA * output_to_here)
      : target(*output_to_here)
    {}
    
    virtual void operator()(wali::wfa::ITrans const * t) {
      wali::domains::SemElemSet::ElementSet es;
      es.push_back(t->weight());
      
      target.addTrans(t->from(),
                      t->stack(),
                      t->to(),
                      new wali::domains::SemElemSet(t->weight(), es));
    }
  };
  
}    
  


namespace wali
{
  namespace wfa
  {
    //////////////////////////////
    // epsilonCloseCached variants

    WFA::AccessibleStateMap
    WFA::epsilonCloseCached(Key state, WFA::EpsilonCloseCache & cache) const
    {
      return epsilonCloseCached_FwpdsAllMulti(state, cache);
    }
    

    // On-demand variants
    WFA::AccessibleStateMap
    epsilonCloseCached_genericDemand(WFA const & wfa,
                                     Key state,
                                     WFA::EpsilonCloseCache & cache,
                                     WFA::AccessibleStateMap (WFA::* accessor)(Key) const)
    {
      WFA::EpsilonCloseCache::iterator loc = cache.find(state);

      if (loc != cache.end()) {
        return loc->second;
      }
      else {
        WFA::AccessibleStateMap eclose = (wfa.*accessor)(state);
        cache[state] = eclose;
        return eclose;
      }
    }

    
    WFA::AccessibleStateMap
    WFA::epsilonCloseCached_MohriDemand(Key state, WFA::EpsilonCloseCache & cache) const
    {
      return epsilonCloseCached_genericDemand(*this, state, cache,
                                              &wali::wfa::WFA::epsilonClose_Mohri);
    }

    
    WFA::AccessibleStateMap
    WFA::epsilonCloseCached_FwpdsDemand(Key state, WFA::EpsilonCloseCache & cache) const
    {
      return epsilonCloseCached_genericDemand(*this, state, cache,
                                              &wali::wfa::WFA::epsilonClose_Fwpds);
    }


    // "All" variants

    /// Populates 'targets' with every state that is a target of a
    /// non-epsilon transition. (The reason we do this is because algorithms
    /// that require eclose(s) only do so for such states, plus the initial
    /// state.
    struct TransTargetFinder : ConstTransFunctor
    {
      std::set<Key> targets;
    
      virtual void operator()( const wali::wfa::ITrans* t ) {
        if (t->stack() != wali::WALI_EPSILON) {
          targets.insert(t->to());
        }
      }
    };

    
    WFA::AccessibleStateMap
    epsilonCloseCached_genericAll(WFA const & wfa,
                                  Key state,
                                  WFA::EpsilonCloseCache & cache,
                                  WFA::AccessibleStateMap (WFA::* accessor)(Key) const)
    {
      if (cache.size() == 0) {
        TransTargetFinder finder;
        wfa.for_each(finder);
        finder.targets.insert(wfa.getInitialState());

        // A source of an eclose is the target of a non-eps transition
        for (std::set<Key>::const_iterator source = finder.targets.begin();
             source != finder.targets.end(); ++source)
        {
          cache[*source] = (wfa.*accessor)(*source);
        }
      }

      // Return cache[state]
      WFA::EpsilonCloseCache::iterator loc = cache.find(state);
      assert(loc != cache.end());
      return loc->second;
    }

    
    WFA::AccessibleStateMap
    WFA::epsilonCloseCached_MohriAll(Key state, WFA::EpsilonCloseCache & cache) const
    {
      return epsilonCloseCached_genericAll(*this, state, cache,
                                           &wali::wfa::WFA::epsilonClose_Mohri);
    }

    
    WFA::AccessibleStateMap
    WFA::epsilonCloseCached_FwpdsAllSingles(Key state, WFA::EpsilonCloseCache & cache) const
    {
      return epsilonCloseCached_genericAll(*this, state, cache,
                                           &wali::wfa::WFA::epsilonClose_Fwpds);
    }


    WFA::AccessibleStateMap
    WFA::epsilonCloseCached_FwpdsAllMulti(Key source, WFA::EpsilonCloseCache & cache) const
    {
      if (cache.size() == 0) {
        TransTargetFinder finder;
        this->for_each(finder);
        finder.targets.insert(this->getInitialState());

        cache = genericFwpdsPoststar(finder.targets, is_epsilon_transition);
      }
      
      // Return cache[state]
      WFA::EpsilonCloseCache::iterator loc = cache.find(source);
      assert(loc != cache.end());
      return loc->second;
    }
   

    ////////////////////////
    // epsilonClose variants
    
    WFA::AccessibleStateMap
    WFA::epsilonClose(Key state) const
    {
      return this->epsilonClose_Fwpds(state);
    }
    
    
    WFA::AccessibleStateMap
    WFA::epsilonClose_Mohri(Key start) const
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

      // Worklist loop
      while (worklist.size() > 0) {
        Key q = *worklist.begin();
        worklist.erase(worklist.begin());
        sem_elem_t r_q = r[q]; // the change in q's weight since the last time it was visited; just 'r' in the paper
        r[q] = zero;

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

      AccessibleStateMap out;
      for (AccessibleStateMap::const_iterator iter = d.begin();
           iter != d.end(); ++iter)
      {
        if (iter->second != zero) {
          assert(!iter->second->equal(zero));
          out[iter->first] = iter->second;
        }
      }
      
      return out;
    }


    WFA::EpsilonCloseCache
    WFA::genericFwpdsPoststar(std::set<Key> const & sources,
                              boost::function<bool (ITrans const *)> trans_accept) const
    {
      Key p_state = getKey("__p");
      Key query_accept = getKey("__accept");
      Key dummy = getKey("__dummy");
      sem_elem_t zero = getSomeWeight()->zero();

      // Convert this WFA into a WPDS so that we can run poststar
      wali::wpds::fwpds::FWPDS wpds;
      this->toWpds(p_state, &wpds, trans_accept);
      wpds.topDownEval(false);

      // Add an additional dummy node to pull off the multi-source trick
      for (std::set<Key>::const_iterator source = sources.begin();
           source != sources.end(); ++source)
      {
          wpds.add_rule(p_state, dummy,
                        p_state, *source, *source,
                        zero->one());
      }

      // Set up the query automaton:
      //               dummy
      //     p_state -----------> ((query_accept))
      //              weight 1
      WFA query;
      query.addState(p_state, zero);
      query.addState(query_accept, zero);
      query.setInitialState(p_state);
      query.addFinalState(query_accept);
      query.addTrans(p_state, dummy, query_accept, zero->one());

      // Issue poststar. Be careful to not force the output a la what Cindy
      // was doing! (Actually I think we look at all the weights anyway so
      // that shouldn't matter, but whatever.)
      WFA const & result = wpds.poststar(query);

      // The 'result' automaton should be something like
      //
      //               dummy
      //   p_state --------------> ((query_accept))
      //        \  \                 -+
      //         \  \ S             /
      //          \  \             /
      //           \  \           / source
      //            \  \         /
      //            -+ -+       /
      //          (p_state, source)
      //
      // for each 'source' in 'sources' and lots of symbols 'S' -- which are
      // states in 'this' WFA. Each symbol S is in the epsilon close of
      // 'source', so add it.
      //
      // Because of the representation of transitions, we again need to
      // iterate over each (start, sym) pair then over the TransSet.
      WFA::EpsilonCloseCache closures;

      for (kp_map_t::const_iterator kp_iter = result.kpmap.begin();
           kp_iter != result.kpmap.end(); ++kp_iter)
      {
        Key start = kp_iter->first.first;
        Key target = kp_iter->first.second;  // a state in some epsilon closure, maybe
        TransSet const & transitions = kp_iter->second;

        if (start != p_state) {
          // We are on a (p_state, source) ---source---> ((query_accept))
          // transition, which we don't care about
          continue;
        }
        
        for (TransSet::const_iterator trans = transitions.begin();
             trans != transitions.end(); ++trans)
        {
          Key to_state = (*trans)->to();
          if (to_state == query_accept) {
            // We are on the (p_state) ---dummy---> ((query_accept))
            // transition, which we don't care about.
            assert (transitions.size() == 1u);
            continue;
          }

          // to_state better be (p_state, source)_g#; we need to extract
          // 'source'.
          key_src_t to_state_refptr = getKeySource(to_state);
          wpds::GenKeySource const * to_state_gen_source =
            dynamic_cast<wpds::GenKeySource const *>(to_state_refptr.get_ptr());
          assert(to_state_gen_source);

          key_src_t state_pair = getKeySource(to_state_gen_source->getKey());
          KeyPairSource const * to_state_pair =
            dynamic_cast<KeyPairSource const *>(state_pair.get_ptr());
          assert(to_state_pair);

          Key source = to_state_pair->second();
          assert(this->getState(source) != NULL);
          
          // Now get the weight. That's the net weight from 'source' to
          // 'target', where 'target' is actually a state in 'this' WFA.
          closures[source][target] = (*trans)->weight();
        }
      }
      
      return closures;
    }


    WFA::AccessibleStateMap
    WFA::epsilonClose_Fwpds(Key source) const
    {
      std::set<Key> sources;
      sources.insert(source);
      EpsilonCloseCache const & cache = genericFwpdsPoststar(sources, is_epsilon_transition);
      EpsilonCloseCache::const_iterator loc = cache.find(source);
      assert(loc != cache.end());
      return loc->second;
    }



    WFA::AccessibleStateSetMap
    WFA::computeAllReachingWeights() const
    {
      // Lift weights to the sets
      WFA lifted;
      sem_elem_t lifted_zero = new SemElemSet(this->getSomeWeight()->zero());
      SemElemSetLifter lifter(&lifted);
      for (std::set<Key>::const_iterator q = Q.begin(); q != Q.end(); ++q) {
        lifted.addState(*q, lifted_zero);
      }
      this->for_each(lifter);
      lifted.setInitialState(this->getInitialState());
      // finals don't matter

      assert(num_trans(*this) == num_trans(lifted));

      // Issue poststar from the initial state in the lifted automaton.
      std::set<Key> sources;
      sources.insert(lifted.getInitialState());
      EpsilonCloseCache const & poststar_answer = lifted.genericFwpdsPoststar(sources, is_any_transition);
      EpsilonCloseCache::const_iterator loc = poststar_answer.find(lifted.getInitialState());
      assert(loc != poststar_answer.end());
      assert(1u == poststar_answer.size());
      AccessibleStateMap const & set_weights = loc->second;      

      // set_weights is almost what we want, except we don't want to make the
      // user downcast everything to get the actual result.
      AccessibleStateSetMap result;

      for (AccessibleStateMap::const_iterator state_weight_pair = set_weights.begin();
           state_weight_pair != set_weights.end(); ++state_weight_pair)
      {
        SemElemSet * set_weight = dynamic_cast<SemElemSet *>(state_weight_pair->second.get_ptr());
        
        assert(result.find(state_weight_pair->first) == result.end());
        result[state_weight_pair->first] = set_weight->getElements();
      }

      return result;
    }

  } // namespace wfa

} // namespace wali

// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:

