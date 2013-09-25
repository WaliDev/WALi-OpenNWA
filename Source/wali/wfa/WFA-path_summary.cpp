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

#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <iterator>
#include <fstream>

//#define USE_FWPDS 1
//#define JAMDEBUG 1


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
        ("CrossCheckAll",     WFA::CrossCheckAll);

    //
    // Calls path_summary with default Worklist
    //
    void WFA::path_summary()
    {
      DefaultWorklist<State> wl;
      path_summary(wl);
    }

    //
    // Calls path_summary with default Worklist
    //
    void WFA::path_summary(sem_elem_t wt)
    {
      DefaultWorklist<State> wl;
      path_summary(wl, wt);
    }

    //
    // Computes path_summary 
    //
    void WFA::path_summary(Worklist<State> &wl)
    {
      sem_elem_t nullwt; // treated as ONE
      path_summary(wl, nullwt);
    }

    //
    // Computes path_summary
    //
    void WFA::path_summary( Worklist<State>& wl, sem_elem_t wt )
    {
      // BEGIN DEBUGGING
      //int numPops = 0;
      // END DEBUGGING
      IncomingTransMap_t preds;
      setupFixpoint( wl, &preds, NULL, wt );
      while( !wl.empty() )
      {
        State* q = wl.get();
        sem_elem_t the_delta = q->delta();
        q->delta() = the_delta->zero();

        { // BEGIN DEBUGGING
          //numPops++;
          //q->print( *waliErr << "  Popped: " ) << std::endl;
        } // END DEBUGGING

        // Get a handle on ZERO b/c we use it alot
        sem_elem_t ZERO = q->weight()->zero();

        // Find predecessor set
        IncomingTransMap_t::iterator incomingTransIt = preds.find(q->name());

        // Some states may have no predecessors, like
        // the initial state
        if(  incomingTransIt == preds.end() )
        {
          continue;
        }

        // Tell predecessors we have changed
        std::vector<ITrans*> & incoming = incomingTransIt->second;

        std::vector<ITrans*>::iterator transit = incoming.begin();
        for( ; transit != incoming.end() ; ++transit )
        {
          ITrans* t = *transit;
          
          // We are looking at a transition (q', _, q)
          State* qprime = state_map[t->from()];

          sem_elem_t newW = qprime->weight()->zero();

          { // BEGIN DEBUGGING
            //t->print( *waliErr << "\t++ Popped " ) << std::endl;
          } // END DEBUGGING

          assert(t->to() == q->name());

          sem_elem_t extended;
          if( query == INORDER )
            extended = t->weight()->extend( the_delta );
          else
            extended = the_delta->extend( t->weight() );
          newW = newW->combine(extended);

          // delta => (w+se,w-se)
          // Use extended->delta b/c we want the diff b/w the new
          // weight (extended) and what was there before
          std::pair< sem_elem_t,sem_elem_t > p =
            newW->delta( qprime->weight() );

          { // BEGIN DEBUGGING
            //qprime->weight()->print( *waliErr << "   oldW " << key2str(qprime->name()) ) << std::endl;
            //newW->print( *waliErr << "   newW " << key2str(qprime->name()) ) << std::endl;
            //p.first->print( *waliErr << "\t++ p.first " ) << std::endl;
            //p.second->print( *waliErr << "\t++ p.second " ) << std::endl;
          } // END DEBUGGING

          // Sets qprime's new weight
          // p.first == (l(t) X the_delta) + W(qprime)
          qprime->weight() = p.first;

          // on the worklist?
          if( qprime->marked() ) {
            qprime->delta() = qprime->delta()->combine(p.second);
          }
          else {
            // not on the worklist means its delta is zero
            qprime->delta() = p.second;

            // add to worklist if not zero
            if( !qprime->delta()->equal(ZERO) ) {
              wl.put(qprime);
            }
          }
        }
        if( progress.is_valid() )
            progress->tick();
      }
      { // BEGIN DEBUGGING
        //*waliErr << "\n --- WFA::path_summary needed " << numPops << " pops\n";
        //*waliErr << "WFA state labels:\n";
        //FOR_EACH_STATE( st ) {
        //    *waliErr << "\t" << key2str(st->name()) << ": ";
        //    st->weight()->print( *waliErr ) << std::endl;
        //}
      } // END DEBUGGING
    }

    void WFA::path_summary_tarjan() {
      sem_elem_t wt = getSomeWeight()->one();
      Key pkey = getKey("__pstate");
#ifdef USE_FWPDS
      wpds::fwpds::FWPDS pds;
#else
      wpds::WPDS pds;
#endif
      //pds.useNewton(false);
      //wpds::fwpds::FWPDS::topDownEval(false);
      this->toWpds(pkey, &pds, is_any_transition, true);

#ifdef JAMDEBUG
      std::cerr << "##### FWPDS" << std::endl;
      pds.print(std::cerr);
#endif

      WFA query;
      query.addState(pkey, wt->zero());
      query.setInitialState(pkey);
      Key fin = getKey("__done");
      query.addState(fin, wt->zero());
      query.addFinalState(fin);
      
      for (std::set<Key>::const_iterator fit = getFinalStates().begin();
        fit!=getFinalStates().end(); fit++)
      {
        Key fkey = *fit;
        query.addTrans(pkey, fkey, fin, wt->one());
      }

#ifdef JAMDEBUG
      std::cerr << "##### QUERY2" << std::endl;
      query.print(std::cerr);
#endif

      WFA ans;
      pds.poststar(query, ans);

#ifdef JAMDEBUG
      fstream foo;
      foo.open("regexp_prestar.dot", fstream::out);
      const wali::graph::reg_exp_hash_t& roots = wali::graph::RegExp::getRoots();
      foo << "digraph {\n";
      std::set<long> seen;
      for(wali::graph::reg_exp_hash_t::const_iterator iter = roots.begin();
          iter != roots.end();
          ++iter){
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
        if (trans != NULL) {
          //wpds::fwpds::LazyTrans *ltrans = dynamic_cast<wpds::fwpds::LazyTrans*>(trans);
          //st->se = ltrans->se;
          st->weight() = trans->weight();
        } else {
          //assert (found && "Cannot find transition associated with state.");
          st->weight() = wt->zero();
        }
      }
    }


  }
}
