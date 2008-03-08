#include "wali/wfa/State.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/epr/EPA.hpp"
#include "wali/wfa/epr/StatePriorityWorklist.hpp"

namespace wali {
  namespace wfa {
    namespace epr {

      EPA::EPA( ) : WFA(), nCacheHits(0) {}

      EPA::~EPA() {}

      void EPA::clear() {
        bfsOrder.clear();
        errorProjCache.clear();
        WFA::clear();
      }

      int EPA::getCacheHits() {
        return nCacheHits;
      }

      void EPA::buildEPA(WFA &post, WFA &pre) {
        FunctionalWeightMaker wmaker;
        post.intersect(wmaker, pre, *this);
        prune();
      }

      sem_elem_t EPA::apply(Key node, sem_elem_t initWeight) {
        int temp;
        return apply(node, initWeight, temp);
      }

      // The weights put on states are not functional
      sem_elem_t EPA::apply(Key node, sem_elem_t initWeight, int &notfound) {
        // Get hold of (non-functional) one and zero
        sem_elem_t ONE = initWeight->one();
        sem_elem_t ZERO = initWeight->zero();

        // The worklist with BFS order on states
        orderStatesBFS();
        StatePriorityWorklist wl(bfsOrder);

        // First, initialize the state weights
        state_map_t::iterator it = state_map.begin();
        state_map_t::iterator itEND = state_map.end();

        for(; it != itEND; it++) {
          State *st = it->second;
          
          st->unmark();
          st->weight() = ZERO;
          st->delta() = ZERO;
          
        }

        State *init = getState( getInitialState() );
        assert(init != 0);
        
        // This code assume that the transition
        // (init,node,init) does not exist
        Trans temp_trans;
        if(find(init->name(), node, init->name(), temp_trans)) {
          // TODO: EPA can handle this case as well
          assert(0);
        }
        
        // Process the outgoing transitions from init
        TransSet tset = match(getInitialState(), node);
        TransSet::iterator transit;
        
        if(tset.empty()) {
          notfound++;
          return ZERO;
        }
        
        int trans_cnt = 0;
        State *init_succ = 0;
        sem_elem_t init_succ_wt;
        for(transit = tset.begin(); transit != tset.end(); transit++, trans_cnt++) {
          Trans *t = (*transit);
          State *q = getState(t->to());
          FunctionalWeight *fw = dynamic_cast<FunctionalWeight *> (t->weight().get_ptr());
          
          if(fw == 0) {
            *waliErr << "Error: Non-FunctionalWeight present in EPA\n";
            assert(0);
          }
          
          assert(q->weight()->equal(ZERO));
          q->weight() = fw->apply(initWeight);
          q->delta() = q->weight();
          
          wl.put(q);
          
          init_succ = q;
          init_succ_wt = q->weight();
        }
        
        // There was only one outgoing transition from init
        // This is an opportunity to look in the cache
        if(trans_cnt == 1) { 
          sem_elem_t res = lookupCache(init_succ->name(), init_succ_wt);
          
          if(res.is_valid()) {
            nCacheHits++;              
            return res;
          }
        }
        
        // Continue processing the worklist
        while(!wl.empty()) {
          
          State *q = wl.get();
          
          sem_elem_t delta = q->delta();
          q->delta() = ZERO;
          
          // propagate weight from q to its successors
          TransSet::iterator it;
          for(it = q->begin() ; it != q->end(); it ++) {
            Trans *t = *it;
            State* qprime = getState(t->to());
            FunctionalWeight *fw = dynamic_cast<FunctionalWeight *> (t->weight().get_ptr());
            
            assert(qprime != 0 && fw != 0);  
            
            // Apply the functional weight
            sem_elem_t newW = fw->apply(delta);
            
            std::pair< sem_elem_t, sem_elem_t > cd =
              newW->delta(qprime->weight());
            
            qprime->weight() = qprime->weight()->combine(cd.first);
            qprime->delta() = qprime->delta()->combine(cd.second);
            
            // Put back into the worklist if necessary
            if( !qprime->delta()->equal(ZERO) ) {
              wl.put(qprime);
            }
          }
        }    
        
        // Gather up the result by taking a combine on the
        // weights of the final states
        sem_elem_t ret = ZERO;
        std::set< Key >::iterator sit;
        for(sit = F.begin(); sit != F.end() ; sit++ ) {
          State *q = getState(*sit);
          ret = ret->combine(q->weight());
        }
        
        // Insert result into the cache
        if(trans_cnt == 1) { 
          addToCache(init_succ->name(), init_succ_wt, ret);
        }
        return ret;
        
      }

      // returns invalid sem_elem_t if (q,w) is not found in the cache
      sem_elem_t EPA::lookupCache(Key q, sem_elem_t w) {
        if(errorProjCache.find(q) == errorProjCache.end()) {
          return sem_elem_t(0);
        }
        CacheElem::iterator it;
        for(it = errorProjCache[q].begin(); it != errorProjCache[q].end(); it++) {
          std::pair< sem_elem_t, sem_elem_t > &ce = (*it);
          if(ce.first == w) {
            return ce.second;
          }
        }
        return sem_elem_t(0);
      }
      
      // Adds ((q,w),res) to the cache
      void EPA::addToCache(Key q, sem_elem_t w, sem_elem_t res) {
        assert(w.is_valid());
        assert(res.is_valid());
        
        errorProjCache[q].push_back(std::pair< sem_elem_t, sem_elem_t >(w,res));
      }
      
      // Do a BFS on the automaton and store the BFS number
      // in bfsOrder
      void EPA::orderStatesBFS() {
        std::list<Key> workset;
        int cnt = 0;
        
        workset.push_back(getInitialState());
        bfsOrder[getInitialState()] = cnt++;
        
        while(!workset.empty()) {
          Key st = workset.front();
          workset.pop_front();
          
          if(bfsOrder.find(st) == bfsOrder.end()) {
            bfsOrder[st] = cnt++;
          }
          
          // insert all children back into the workset
          State *q = getState(st);
          TransSet::iterator it;
          for(it = q->begin() ; it != q->end(); it++ ) { 
            Trans *t = *it;
            if(bfsOrder.find(t->to()) == bfsOrder.end()) {
              workset.push_back(t->to());
            }
          }
        }
      }
      
    } // namespace epr
  } // namespace wfa
} // namespace wali
