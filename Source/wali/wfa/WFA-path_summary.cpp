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
#include "wali/wpds/fwpds/LazyTrans.hpp"
#include "wali/graph/RegExp.hpp"
#include "wali/util/ConfigurationVar.hpp"
#include "wali/graph/GraphCommon.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/domains/ReversedSemElem.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <iterator>
#include <fstream>

using namespace wali::witness;
using namespace wali::wpds;

namespace wali
{
  namespace wfa
  {
    WFA::PathSummaryImplementation
      WFA::globalDefaultPathSummaryImplementation
      = wali::util::ConfigurationVar<WFA::PathSummaryImplementation>(
          "WALI_WFA_PATH_SUMMARY_IMPLEMENTATION",
          WFA::IterativeWpds
        )
        ("IterativeOriginal", WFA::IterativeOriginal)
        ("IterativeWpds",     WFA::IterativeWpds)
        ("TarjanFwpds",       WFA::TarjanFwpds)
        ("CrossCheckAll",     WFA::CrosscheckAll);

    bool WFA::globalDefaultPathSummaryFwpdsTopDown
      = wali::util::ConfigurationVar<bool>(
          "WALI_WFA_PATH_SUMMARY_FWPDS_EVAL",
          true
        )
        ("TopDown", true)
        ("BottomUp", false);

    //
    // Calls path_summary with default Worklist
    //
    void
    WFA::path_summary_iterative_original()
    {
      DefaultWorklist<State> wl;
      path_summary_iterative_original(wl);
    }

    //
    // Calls path_summary_iterative_original with default Worklist
    //
    void
    WFA::path_summary_iterative_original(sem_elem_t wt)
    {
      DefaultWorklist<State> wl;
      path_summary_iterative_original(wl, wt);
    }

    //
    // Computes path_summary_iterative_original 
    //
    void
    WFA::path_summary_iterative_original(Worklist<State> &wl)
    {
      sem_elem_t nullwt; // treated as ONE
      path_summary_iterative_original(wl, nullwt);
    }

    //
    // Computes path_summary_iterative_original
    //
    void
    WFA::path_summary_iterative_original(Worklist<State>& wl, sem_elem_t wt)
    {
      // BEGIN DEBUGGING
      //int numPops = 0;
      // END DEBUGGING
      IncomingTransMap_t preds;
      setupFixpoint(wl, &preds, NULL, wt);
      while (!wl.empty()) {
        State* q = wl.get();
        sem_elem_t the_delta = q->delta();
        q->delta() = the_delta->zero();

        { // BEGIN DEBUGGING
          //numPops++;
          //q->print(*waliErr << "  Popped: ") << std::endl;
        } // END DEBUGGING

        // Get a handle on ZERO b/c we use it alot
        sem_elem_t ZERO = q->weight()->zero();

        // Find predecessor set
        IncomingTransMap_t::iterator incomingTransIt = preds.find(q->name());

        // Some states may have no predecessors, like
        // the initial state
        if (incomingTransIt == preds.end()) {
          continue;
        }

        // Tell predecessors we have changed
        std::vector<ITrans*> & incoming = incomingTransIt->second;

        std::vector<ITrans*>::iterator transit = incoming.begin();
        for ( ; transit != incoming.end() ; ++transit)
        {
          ITrans* t = *transit;
          
          // We are looking at a transition (q', _, q)
          State* qprime = state_map[t->from()];

          sem_elem_t newW = qprime->weight()->zero();

          { // BEGIN DEBUGGING
            //t->print(*waliErr << "\t++ Popped ") << std::endl;
          } // END DEBUGGING

          assert(t->to() == q->name());

          sem_elem_t extended;
          if (query == INORDER) {
            extended = t->weight()->extend(the_delta);
          }
          else {
            extended = the_delta->extend(t->weight());
          }
          newW = newW->combine(extended);

          // delta => (w+se,w-se)
          // Use extended->delta b/c we want the diff b/w the new
          // weight (extended) and what was there before
          std::pair<sem_elem_t,sem_elem_t> p = newW->delta(qprime->weight());

          { // BEGIN DEBUGGING
            //qprime->weight()->print(*waliErr << "   oldW " << key2str(qprime->name())) << std::endl;
            //newW->print(*waliErr << "   newW " << key2str(qprime->name())) << std::endl;
            //p.first->print(*waliErr << "\t++ p.first ") << std::endl;
            //p.second->print(*waliErr << "\t++ p.second ") << std::endl;
          } // END DEBUGGING

          // Sets qprime's new weight
          // p.first == (l(t) X the_delta) + W(qprime)
          qprime->weight() = p.first;

          // on the worklist?
          if (qprime->marked()) {
            qprime->delta() = qprime->delta()->combine(p.second);
          }
          else {
            // not on the worklist means its delta is zero
            qprime->delta() = p.second;

            // add to worklist if not zero
            if (!qprime->delta()->equal(ZERO)) {
              wl.put(qprime);
            }
          }
        }
        if (progress.is_valid()) {
            progress->tick();
        }
      }
      { // BEGIN DEBUGGING
        //*waliErr << "\n --- WFA::path_summary_iterative_original needed " << numPops << " pops\n";
        //*waliErr << "WFA state labels:\n";
        //FOR_EACH_STATE(st) {
        //    *waliErr << "\t" << key2str(st->name()) << ": ";
        //    st->weight()->print(*waliErr) << std::endl;
        //}
      } // END DEBUGGING
    }

    namespace details
    {
      class WitnessChecker : public TransFunctor
      {
        bool found_any;

      public:
        WitnessChecker()
          : found_any(false)
        {}

        bool
        foundAny() const {
          return found_any;
        }

        virtual void operator()(ITrans * t)
        {
          SemElem * weight = t->weight().get_ptr();
          found_any |= (dynamic_cast<Witness*>(weight) != NULL);
        }
      };
    }

    void
    WFA::path_summary_tarjan_fwpds()
    {
      path_summary_tarjan_fwpds(defaultPathSummaryFwpdsTopDown);
    }
    
    void
    WFA::path_summary_tarjan_fwpds(bool top_down)
    {
#if defined(REGEXP_CACHING) // TODO: && CHECKED_LEVEL >= 2
      // If REGEXP_CACHING is on, there is a gotcha while using the
      // FWPDS-based path_summary. If your weights have the property
      // that you can have two weights W1 and W2 that compare equal
      // (in the sense that W1->equal(W2) is true) but are not
      // *really* equal, then regexp node caching can cause WALi to
      // conflate two sort-of-the-same-but-sort-of-different weights
      // and produce the wrong answer.
      //
      // Witnesse weights have this property, but they are the only
      // ones I know about currently. (Witness weights are
      // conceptually a pair <weight, witness>, and two witness
      // weights compare equal if the underlying 'weight' portions are
      // the same. However, the witnesses can differ, and this can
      // lead to incorrect witnesses.)
      //
      // As a result, here we check that the user is not performing
      // path_summary on a WFA that has witness weights using FWPDS
      // while REGEXP_CACHING is on.
      //
      // This test is not complete -- there could be other weights
      // that have the poorly-behaved property described above -- but
      // I don't know what they are so can't check for them. At the
      // same time, turning off REGEXP_CACHING really hurts the
      // performance of FWPDS, but FWPDS-based path_summary could
      // really be useful, so we want to keep the option around to
      // allow path_summary_tarjan_fwpds() with REGEXP_CACHING on for
      // the common case where weights behave "properly."
      details::WitnessChecker checker;
      for_each(checker);
      fast_assert(!checker.foundAny());
#endif

      fwpds::FWPDS pds;
      pds.topDownEval(top_down);
      pds.useNewton(false);
      path_summary_via_wpds(pds);
    }

    void
    WFA::path_summary_iterative_wpds()
    {
      WPDS pds;
      path_summary_via_wpds(pds);
    }

    void
    WFA::path_summary_crosscheck_all()
    {
      WFA copy1 = *this;
      WFA copy2 = *this;
      WFA copy3 = *this;

      path_summary_iterative_original();
      copy1.path_summary_iterative_wpds();
      copy2.path_summary_tarjan_fwpds(true);
      copy3.path_summary_tarjan_fwpds(false);

      assert(this->equal(copy1)); // TODO: slow_assert
      assert(this->equal(copy2));
      assert(this->equal(copy3));
    }

    void
    WFA::path_summary()
    {
      switch (defaultPathSummaryImplementation)
      {
      case IterativeOriginal:
        path_summary_iterative_original();
        break;

      case IterativeWpds:
        path_summary_iterative_wpds();
        break;

      case TarjanFwpds:
        path_summary_tarjan_fwpds();
        break;

      case CrosscheckAll:
        path_summary_crosscheck_all();
        break;
      }
    }

    void
    WFA::path_summary_via_wpds(WPDS & pds) {
      if (this->getFinalStates().size() == 0u) {
        return;
      }

      sem_elem_t wt = getSomeWeight()->one();
      Key pkey = getKey("__pstate");

      if (getQuery() == INORDER) {
          this->toWpds(pkey, &pds, is_any_transition, true, wali::domains::wrapToReversedSemElem);
      }
      else {
          this->toWpds(pkey, &pds, is_any_transition, true);
      }

#ifdef JAMDEBUG
      std::cerr << "##### FWPDS" << std::endl;
      pds.print(std::cerr);
#endif

      WFA query(INORDER, progress);
      query.addState(pkey, wt->zero());
      query.setInitialState(pkey);
      Key fin = getKey("__done");
      query.addState(fin, wt->zero());
      query.addFinalState(fin);

      sem_elem_t one = wt->one();
      if (getQuery() == INORDER) {
        one = new domains::ReversedSemElem(one);
      }
      
      for (std::set<Key>::const_iterator fit = getFinalStates().begin();
        fit!=getFinalStates().end(); fit++)
      {
        Key fkey = *fit;
        query.addTrans(pkey, fkey, fin, one);
      }

#ifdef JAMDEBUG
      std::cerr << "##### QUERY2" << std::endl;
      query.print(std::cerr);
#endif

      WFA ans = pds.poststar(query);

#ifdef JAMDEBUG
      fstream foo;
      foo.open("regexp_prestar.dot", fstream::out);
      const wali::graph::reg_exp_hash_t& roots = wali::graph::RegExp::getRoots();
      foo << "digraph {\n";
      std::set<long> seen;
      for (wali::graph::reg_exp_hash_t::const_iterator iter = roots.begin();
           iter != roots.end();
           ++iter)
      {
        (iter->second)->toDot(foo, seen, true, true);
      }
      foo << "}\n";
      foo.close();
      
      std::cerr << "##### ANS" << std::endl;
      ans.print(std::cerr);
#endif


      for (state_map_t::const_iterator smit=state_map.begin();
           smit!=state_map.end(); smit++)
      {
        Key stkey = smit->first;

        Key initkey = ans.init_state;
        Key finkey = *ans.getFinalStates().begin();

        State *st = smit->second;
        ITrans *trans = ans.find(initkey, stkey, finkey);
        sem_elem_t weight;
        if (trans != NULL) {
          weight = trans->weight();
        } else {
          if (getQuery() == INORDER) {
            weight = new domains::ReversedSemElem(wt->zero());
          }
          else {
            weight = wt->zero();
          }
        }

        if (getQuery() == INORDER) {
          domains::ReversedSemElem * rw = dynamic_cast<domains::ReversedSemElem*>(weight.get_ptr());
          assert(rw);
          weight = rw->backingSemElem();
        }

        st->weight() = weight;
      }
    }


    std::map<Key, sem_elem_t>
    WFA::readOutCombineOverAllPathsValues()
    {
      return readOutCombineOverAllPathsValues(alphabet());
    }

    std::map<Key, sem_elem_t>
    WFA::readOutCombineOverAllPathsValues(std::set<Key> const & alpha)
    {
      Key const init = getInitialState();
      sem_elem_t weight = getSomeWeight()->zero();

      std::map<Key, sem_elem_t> values_map;

      for (std::set<Key>::const_iterator iter = alpha.begin();
           iter != alpha.end(); iter++)
      {
        // Compute Combine_{t = (init,*iter,to)} (weight(t) extend weight(to))
        // or      Combine_{t = (init,*iter,to)} (weight(to) extend weight(t)),
        // depending on whether the WFA holds a prestar or poststar
        // answer.
        //
        // Get the set of transitions in the WFA that are of the
        // form (init,*iter,?)
        TransSet transitionSet = match(init, *iter);
        
        for (TransSet::iterator tsiter = transitionSet.begin();
             tsiter != transitionSet.end() ; tsiter++)
        {
          ITrans * t = *tsiter;

          // Set tmp to (weight(to) extend weight(t)) or
          // (weight(t) extend weight(to)),
          // depending on whether the WFA holds a prestar or
          // poststar answer.
          sem_elem_t tmp;
          sem_elem_t target_weight = getState(t->to())->weight();
          sem_elem_t trans_weight = t->weight();
          if (getQuery() == WFA::INORDER) {
            tmp = trans_weight->extend(target_weight);
          }
          else {
            tmp = target_weight->extend(trans_weight);
          }

          // Combine the value to the accumulated weight of all
          // "to" states 
          weight = weight->combine(tmp);
        }

        values_map[*iter] = weight;
      }
      
      return values_map;
    }

  }
}
