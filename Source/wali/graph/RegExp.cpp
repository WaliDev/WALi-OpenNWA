#include "wali/graph/RegExp.hpp"
#include "wali/graph/GraphCommon.hpp"
#include <math.h>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <sstream>

#if defined(PPP_DBG)
#include "wali/SemElemTensor.hpp"
#endif

namespace wali {

    namespace graph {

      RegExpDag::RegExpDag()
      {
        currentSatProcess = 0;
        extend_backwards = false;
        saturation_complete = false;
        executing_poststar = true;
        initialized = false;
        top_down_eval = true;
      }

      reg_exp_t RegExpDag::updatable(node_no_t nno, sem_elem_t se) 
      {

            if(saturation_complete) {
              cerr << "RegExp: Error: cannot create updatable nodes when saturation is complete\n";
              assert(!initialized);
              assert(0);
            }

            if(updatable_nodes.size() > nno) {
#if defined(PPP_DGB) && PPP_DBG >= 0
              // This node will likely get used now.
              reg_exp_key_t insKey(updatable_nodes[nno]->type, updatable_nodes[nno]);
              rootsInSatProcess.insert(insKey, updatable_nodes[nno]);
#endif
              return updatable_nodes[nno];
            }
            for(size_t i = updatable_nodes.size(); i < nno; i++) {
              // These nodes are being created proactively. They aren't referenced in the 
              // graphs yet, so don't add them to roots.
              RegExp * r = new RegExp(currentSatProcess, this, i,se->zero());
              updatable_nodes.push_back(r);
            }
            // Create the desired updatable node, and add it to roots
            reg_exp_t r = new RegExp(currentSatProcess, this, nno, se);
#if defined(PPP_DGB) && PPP_DBG >= 0
            reg_exp_key_t insKey(r->type, r);
            rootsInSatProcess.insert(insKey, r);
#endif
            updatable_nodes.push_back(r);
            return updatable_nodes[nno];
        }

        int RegExp::updatableNumber() {
            assert(type == Updatable);
            return (int)updatable_node_no;
        }

#if defined(PUSH_EVAL)
        void RegExp::setDirty()
        {
          dirty = true;
          for(tr1::unordered_set<RegExp*>::iterator pit = parents.begin(); pit != parents.end(); ++pit)
            (*pit)->setDirty();
        }
#endif

        // Updates all the updatable edgses given in the list together, so that all of them
        // get the same update_count.
        void RegExpDag::update(std::vector<node_no_t> nnos, std::vector<sem_elem_t> ses)
        {
          //Make sure that correct number of weights were passed in.
          assert(nnos.size() == ses.size() && "[RegExp::update] Sizes of input vectors must match\n");
          if(saturation_complete) {
            cerr << "RegExp: Error: cannot update nodes when saturation is complete\n";
            assert(!initialized);
            assert(0);
          }

          unsigned int &update_count = satProcesses[currentSatProcess].update_count;
          for(unsigned i = 0; i < nnos.size(); ++i){
            node_no_t nno = nnos[i];
            sem_elem_t se = ses[i];
            updatable(nno,se); // make sure that this node exists
            if(!updatable_nodes[nno]->value->equal(se)) {
#ifdef DWPDS
              updatable_nodes[nno]->delta[update_count+1] = se->diff(updatable_nodes[nno]->value);
#endif
              updatable_nodes[nno]->value = se;
              updatable_nodes[nno]->last_change = update_count + 1;
              updatable_nodes[nno]->last_seen = update_count + 1;
#if defined(PUSH_EVAL)
              updatable_nodes[nno]->setDirty();
#endif
              updatable_nodes[nno]->eval_map.clear();
              updatable_nodes[nno]->updates.push_back(update_count);
            }
          }
          update_count = update_count + 1;
        }

        void RegExpDag::update(node_no_t nno, sem_elem_t se) {
          if(saturation_complete) {
            cerr << "RegExp: Error: cannot update nodes when saturation is complete\n";
            assert(!initialized);
            assert(0);
          }

          updatable(nno,se); // make sure that this node exists
          if(!updatable_nodes[nno]->value->equal(se)) {
            unsigned int &update_count = satProcesses[currentSatProcess].update_count;
            updatable_nodes[nno]->updates.push_back(update_count);
#ifdef DWPDS
            updatable_nodes[nno]->delta[update_count+1] = se->diff(updatable_nodes[nno]->value);
#endif
            update_count = update_count + 1;
            updatable_nodes[nno]->value = se;
            updatable_nodes[nno]->last_change = update_count;
            updatable_nodes[nno]->last_seen = update_count;
#if defined(PUSH_EVAL)
            updatable_nodes[nno]->setDirty();
#endif
            updatable_nodes[nno]->eval_map.clear();
            
          }
          //updates.push_back(nno);
        }

        ostream &operator << (ostream &out, const RegExpStats &s) {
          out << "Semiring Extend : " << s.nextend << "\n";
          out << "Semiring Combine : " << s.ncombine << "\n";
          out << "Semiring Star : " << s.nstar << "\n";
          out << "Semiring Ops : " << (s.nstar + s.ncombine + s.nextend) << "\n";
          out << "HashMap hits : "<< s.hashmap_hits << "\n";
          out << "HashMap misses : " << s.hashmap_misses << "\n";
          return out;
        }

        ostream &RegExp::print(ostream &out) {
            switch(type) {
                case Constant: 
                    value->print(out);
                    break;
                case Updatable:
                    out << "U" << updatable_node_no << "(";
                    value->print(out) << ")";
                    break;
                case Star:
                    out << "(";
                    (children.front())->print(out);
                    out << ")*";
                    break;
                case Extend: {
                                 list<reg_exp_t>::iterator it;
                                 it = children.begin();
                                 out << "(";
                                 (*it)->print(out) << ")";
                                 it++;
                                 for(; it != children.end(); it++) {
                                     out << " x (";
                                     (*it)->print(out) << ")";
                                 }
                                 break;
                             }
                case Combine: {
                                  list<reg_exp_t>::iterator it;
                                  it = children.begin();
                                  out << "(";
                                  (*it)->print(out) << ")";
                                  it++;
                                  for(; it != children.end(); it++) {
                                      out << " + (";
                                      (*it)->print(out) << ")";
                                  }
                                  break;
                              }
            }
            return out;
        }

        // nextLbl is the next label *to be used*
        // The returned label is the *last label used* and the label for the *returning node*.
        long RegExp::toDot(ostream& out, std::set<long>& seen, bool printUpdates, bool isRoot)
        {
          hash_sem_elem hse;
          long me;
          bool isZero = false, isOne = false;
          if(value != NULL){
            isZero = value->equal(value->zero());
            isOne = value->equal(value->one());
          }else{
            isZero = true;
          }
          const long myptr = (const long) this;
          stringstream updatess;
          switch(type){
            case Constant:
              me = hse(value);
              if(seen.find(me) != seen.end())
                return me;
              if(isRoot)
                out << "n" << me << "[label = \""
                  << "constant \\n "
                  << "(root)\\n "
                  << "zero: " << isZero << "\\n "
                  << "one: " << isOne << "\\n "
                  << "\" color=red];\n";
              else
                out << "n" << me << "[label = \""
                  << "constant \\n "
                  << "zero: " << isZero << "\\n "
                  << "one: " << isOne << "\\n "
                  << "\"];\n";
              seen.insert(me);
              return me;
            case Updatable:
              //me = hse(value); 
              me = updatable_node_no;
              //updates
              if(printUpdates){
                updatess << "updates: ";
                for(vector<unsigned>::iterator it = updates.begin(); it != updates.end(); ++it)
                  updatess << *it << " ";
              }
              if(seen.find(me) != seen.end())
                return me;
              if(isRoot)
                out << "n" << me << "[label = \""
                  << "updatable\\n " 
                  << "(root) \\n "
                  << "zero: " << isZero << "\\n "
                  << "one: " << isOne << "\\n "
                  << updatess.str() << "\\n"                
                  << "\" color=red];\n";
              else
                out << "n" << me << "[label = \""
                  << "updatable\\n "
                  << "zero: " << isZero << "\\n "
                  << "one: " << isOne << "\\n "
                  << updatess.str() << "\\n"                
                  << "\" color=brown];\n";
              seen.insert(me);
              return me;
            case Combine:
            case Extend:
            case Star:
            default:
              break;
          }

          vector<long int> others;
          reg_exp_key_t rkey(type, this);
          hash_reg_exp_key hrek;
          me = hrek(rkey);
          if(seen.find(me) != seen.end()){
            return me;
          }
          seen.insert(me);

          //evaluations
          stringstream evaluatess;
          if(printUpdates){
            evaluatess << "evaluations: ";
            for(vector<unsigned>::iterator it = evaluations.begin(); it != evaluations.end(); ++it)
              evaluatess << *it << " ";
          }

          for(list<reg_exp_t>::iterator it = children.begin(); it != children.end(); it++)
            others.push_back((*it)->toDot(out, seen, printUpdates));
          switch(type){
            case Constant:
            case Updatable:
              break;
            case Star:
              if(isRoot)
                out << "n" << me << "[label = \""
                  << "*\\n "
                  << "(root)\\n "<< myptr << "\\n "
                  << "zero: " << isZero << "\\n "
                  << "one: " << isOne << "\\n "
                  << evaluatess.str() << "\\n"                
                  << "\" color=red];\n";
              else
                out << "n" << me << "[label = \""
                  << "*\\n " << myptr << "\\n "
                  << "zero: " << isZero << "\\n "
                  << "one: " << isOne << "\\n "
                  << evaluatess.str() << "\\n"                
                  << "\" color=aquamarine];\n";
              break;
            case Combine:
              if(isRoot)
                out << "n" << me << "[label = \""
                  << "+\\n "
                  << "(root)\\n "<< myptr << "\\n "
                  << "zero: " << isZero << "\\n "
                  << "one: " << isOne << "\\n "
                  << evaluatess.str() << "\\n"                
                  << "\" color=red];\n";
              else
                out << "n" << me << "[label = \""
                  << "+\\n " << myptr << "\\n "
                  << "zero: " << isZero << "\\n "
                  << "one: " << isOne << "\\n "
                  << evaluatess.str() << "\\n"                
                  << "\" color=blue];\n";
              break;
            case Extend:
              if(isRoot)
                out << "n" << me << "[label = \""
                  << "x\\n "
                  << "(root)\\n "<< myptr << "\\n "
                  << "zero: " << isZero << "\\n "
                  << "one: " << isOne << "\\n "
                  << evaluatess.str() << "\\n"                
                  << "\" color=red];\n";
              else
                out << "n" << me << "[label = \""
                  << "x\\n " << myptr << "\\n "
                  << "zero: " << isZero << "\\n "
                  << "one: " << isOne << "\\n "
                  << evaluatess.str() << "\\n"                
                  << "\" color=green];\n";
              break;
            default:
              assert(false && "[RegExp::toDot] Unknown RegExp Type.\n");
          }
          for(vector<long>::iterator it = others.begin(); it != others.end(); it++)
            out << "n" << me << " -> n" << *it << "\n";
          return me;
        }


        // need not return an evaluated reg_exp
        reg_exp_t RegExpDag::star(reg_exp_t r) {
            if(r->type == Star) {
                return r;
            }
#ifndef REGEXP_CACHING
            reg_exp_t res;
            if(r->type == Constant && r->value->equal(r->value->zero()))
              res = new RegExp(currentSatProcess, this, r->value->one());
            else 
              res = new RegExp(currentSatProcess, this, Star, r);
#if defined(PPP_DGB) && PPP_DBG >= 0
            // Manipulate the set of root nodes.
            // remove r from roots.
            reg_exp_key_t delkey(r->type, r);
            rootsInSatProcess.erase(delkey);
            // Add the new regexp to roots
            reg_exp_key_t inskey(res->type, res);
            rootsInSatProcess.insert(inskey, res);           
#endif
            return res;
#else // REGEXP_CACHING

            if(r->type == Constant) {
                if(r->value->equal(r->value->one()) || r->value->equal(r->value->zero())) {
                    assert(r == reg_exp_one || r == reg_exp_zero);
                    return reg_exp_one;
                }
            }

            reg_exp_key_t rkey(Star, r);
            reg_exp_hash_t::iterator it = reg_exp_hash.find(rkey);
            if(it == reg_exp_hash.end()) {
                reg_exp_t res = new RegExp(currentSatProcess, this, Star, r);
                reg_exp_hash.insert(rkey, res);

#if defined(PPP_DGB) && PPP_DBG >= 0
                // Manipulate the set of root nodes.
                // remove r from roots.
                reg_exp_key_t delkey(r->type, r);
                rootsInSatProcess.erase(delkey);
                // Add the new regexp to roots
                reg_exp_key_t inskey(res->type, res);
                rootsInSatProcess.insert(inskey, res);           
#endif
                STAT(stats.hashmap_misses++);
                return res;
            }
            STAT(stats.hashmap_hits++);
            return it->second;
#endif // REGEXP_CACHING
        }

        reg_exp_t RegExpDag::combine(std::list<reg_exp_t> & rlist)
        {
           if(rlist.size() == 0)
             return reg_exp_zero;
           if(rlist.size() == 1)
             return *(rlist.begin());

           std::list<reg_exp_t>::iterator it = rlist.begin();
           reg_exp_t res = *it;
           it++;
           for(;it != rlist.end(); ++it)
             res = combine(res, *it);
           return res;
        }

        reg_exp_t RegExpDag::combine(reg_exp_t r1, reg_exp_t r2) {
            if(r1.get_ptr() == r2.get_ptr()) 
                return r1;
            if(r1->type == Constant && r1->value->equal(r1->value->zero())) {
                return r2;
            } else if(r2->type == Constant && r2->value->equal(r2->value->zero())) {
                return r1;
            }
#ifndef REGEXP_CACHING
            reg_exp_t res = new RegExp(currentSatProcess, this, Combine, r1, r2);
#if defined(PPP_DGB) && PPP_DBG >= 0
            // Manipulate the set of root nodes.
            // remove r1,r2 from roots.
            reg_exp_key_t del1key(r1->type, r1);
            rootsInSatProcess.erase(del1key);
            reg_exp_key_t del2key(r2->type, r2);
            rootsInSatProcess.erase(del2key);
            // Add the new regexp to roots
            reg_exp_key_t inskey(res->type, res);
            rootsInSatProcess.insert(inskey, res);           
#endif
            return res;
#else
            reg_exp_key_t rkey1(Combine, r1, r2);
            reg_exp_hash_t::iterator it = reg_exp_hash.find(rkey1);
            if(it == reg_exp_hash.end()) {
                STAT(stats.hashmap_misses++);
                reg_exp_key_t rkey2(Combine, r2, r1);
                it = reg_exp_hash.find(rkey2);
                // NAK - fold this if underneath the upper one
                //     - didn't make sense the other way.
                if(it == reg_exp_hash.end()) {
                    reg_exp_t res = new RegExp(currentSatProcess, this, Combine, r1, r2);
                    reg_exp_hash.insert(rkey2, res);

#if defined(PPP_DGB) && PPP_DBG >= 0
                    // Manipulate the set of root nodes.
                    // remove r1,r2 from roots.
                    reg_exp_key_t del1key(r1->type, r1);
                    rootsInSatProcess.erase(del1key);
                    reg_exp_key_t del2key(r2->type, r2);
                    rootsInSatProcess.erase(del2key);
                    // Add the new regexp to roots
                    reg_exp_key_t inskey(res->type, res);
                    rootsInSatProcess.insert(inskey, res);           
#endif
                    STAT(stats.hashmap_misses++);
                    return res;
                }
            }
            STAT(stats.hashmap_hits++);
            return it->second;
#endif // REGEXP_CACHING
        }

        reg_exp_t RegExpDag::extend(reg_exp_t r1, reg_exp_t r2) {
            if(extend_backwards) {
                reg_exp_t tmp = r1;
                r1 = r2;
                r2 = tmp;
            }
            if(r1->type == Constant && r1->value->equal(r1->value->zero())) {
                return r1;
            } else if(r2->type == Constant && r2->value->equal(r2->value->zero())) {
                return r2;
            } 

#ifndef REGEXP_CACHING
            reg_exp_t res = new RegExp(currentSatProcess, this, Extend, r1, r2);
#if defined(PPP_DGB) && PPP_DBG >= 0
            // Manipulate the set of root nodes.
            // remove r1,r2 from roots.
            reg_exp_key_t del1key(r1->type, r1);
            rootsInSatProcess.erase(del1key);
            reg_exp_key_t del2key(r2->type, r2);
            rootsInSatProcess.erase(del2key);
            // Add the new regexp to roots
            reg_exp_key_t inskey(res->type, res);
            rootsInSatProcess.insert(inskey, res);           
#endif
            return res;
#else
            if(r1->type == Constant && r1->value->equal(r1->value->one())) {
                return r2;
            } else if(r2->type == Constant && r2->value->equal(r2->value->one())) {
                return r1;
            }
            reg_exp_key_t rkey(Extend, r1, r2);
            reg_exp_hash_t::iterator it = reg_exp_hash.find(rkey);
            if(it == reg_exp_hash.end()) {
                reg_exp_t res = new RegExp(currentSatProcess, this, Extend, r1, r2);
                reg_exp_hash.insert(rkey, res);

#if defined(PPP_DGB) && PPP_DBG >= 0
                // Manipulate the set of root nodes.
                // remove r1,r2 from roots.
                reg_exp_key_t del1key(r1->type, r1);
                rootsInSatProcess.erase(del1key);
                reg_exp_key_t del2key(r2->type, r2);
                rootsInSatProcess.erase(del2key);
                // Add the new regexp to roots
                reg_exp_key_t inskey(res->type, res);
                rootsInSatProcess.insert(inskey, res);           
#endif
                STAT(stats.hashmap_misses++);
                return res;
            }
            STAT(stats.hashmap_hits++);
            return it->second;
#endif // REGEXP_CACHING
        }

        reg_exp_t RegExpDag::constant(sem_elem_t se) {
            if(se->equal(se->zero()))
                return reg_exp_zero;
#ifndef REGEXP_CACHING
            reg_exp_t res = new RegExp(currentSatProcess, this, se);
#if defined(PPP_DGB) && PPP_DBG >= 0
            reg_exp_key_t insKey(res->type, res);
            rootsInSatProcess.insert(insKey, res);
#endif
            return res;
#else
            if(se->equal(se->one()))
                return reg_exp_one;

            const_reg_exp_hash_t::iterator it = const_reg_exp_hash.find(se);
            if(it == const_reg_exp_hash.end()) {
                reg_exp_t res = new RegExp(currentSatProcess, this, se);
                const_reg_exp_hash.insert(se, res);
#if defined(PPP_DGB) && PPP_DBG >= 0
                reg_exp_key_t insKey(res->type, res);
                rootsInSatProcess.insert(insKey, res);
#endif
                return res;
            }
            return it->second;
#endif // REGEXP_CACHING
        }

        void RegExpDag::startSatProcess(const sem_elem_t se) {
          if(initialized) {
            cerr << "Error: RegExp initialized twice\n";
            assert(0);
          }
          initialized = true;
          
          satProcesses.push_back(RegExpSatProcess());
          currentSatProcess = satProcesses.size() - 1;
          
          reg_exp_hash.clear();
          const_reg_exp_hash.clear();
#if defined(PPP_DGB) && PPP_DBG >= 0
          // The set of root nodes is cleared between saturation phases.
          // but only after transferring the set to rootsAcrossSatProcesses
          for(reg_exp_hash_t::iterator it = rootsInSatProcess.begin(); it != rootsInSatProcess.end(); ++it)
            rootsAcrossSatProcesses.insert(*it);
          rootsInSatProcess.clear();
#endif
          for(reg_exp_hash_t::iterator it = graphLabelsInSatProcess.begin(); it != graphLabelsInSatProcess.end(); ++it)
            graphLabelsAcrossSatProcesses.insert(*it);
          graphLabelsInSatProcess.clear();
          updatable_nodes.clear();
         
          reg_exp_zero = new RegExp(currentSatProcess, this, se->zero());
          reg_exp_key_t insZeroKey(reg_exp_zero->type, reg_exp_zero);
          reg_exp_one = new RegExp(currentSatProcess, this, se->one());
          reg_exp_key_t insOneKey(reg_exp_one->type, reg_exp_one);
#if defined(PPP_DGB) && PPP_DBG >= 0
          rootsInSatProcess.insert(insZeroKey, reg_exp_zero);
          rootsInSatProcess.insert(insOneKey, reg_exp_one);
#endif
          saturation_complete = false;
          executing_poststar = true;
        }
      
        void RegExpDag::stopSatProcess() {
          if(!initialized) {
            cerr << "Error: RegExp reset twice without being init-ed\n";
            assert(0);
          }
          initialized = false;
          saturation_complete = true;
          updatable_nodes.clear();
          
        }

#if defined(PPP_DGB) && PPP_DBG >= 0
        const reg_exp_hash_t& RegExpDag::getRoots()
        {
          return rootsInSatProcess;
        }
#endif
        // a = a union b
        void my_set_union(std::set<long int> &a, std::set<long int> &b) {
          std::set<long int> c;
          insert_iterator< set<long int> > ii(c, c.begin());
          std::set_union(a.begin(), a.end(), b.begin(), b.end(), ii);
          a = c;
        }

        typedef pair<int, reg_exp_t> heap_t;

        struct cmp_heap_t {
            bool operator() (heap_t e1, heap_t e2) const {
                return (e1.first < e2.first) ? true :
                    (e1.first > e2.first) ? false :
                    (e1.second.get_ptr() < e2.second.get_ptr());
            }
        };

        // This is a wrapper to manipulate the roots data structure only once
        // per call to minimize_height
        reg_exp_t RegExpDag::minimize_height(reg_exp_t r, reg_exp_cache_t& cache) 
        {
          reg_exp_t res = _minimize_height(r,cache);
#if defined(PPP_DGB) && PPP_DBG >= 0
          // If r was a root, replace it with res
          reg_exp_key_t delKey(r->type, r);
          if(rootsInSatProcess.find(delKey) != rootsInSatProcess.end()){
            rootsInSatProcess.erase(delKey);
            reg_exp_key_t insKey(res->type, res);
            rootsInSatProcess.insert(insKey, res);
          }
#endif
          return res;
        }

        reg_exp_t RegExpDag::_minimize_height(reg_exp_t r, reg_exp_cache_t &cache) {
            reg_exp_cache_t::iterator cpos = cache.find(r);
            if(cpos != cache.end()) {
                return cpos->second;
            }

            if(r->type == Constant) {
                return r;
            }

            if(r->type == Updatable) {
                r->outnodes.insert(r->updatable_node_no);
                return r;
            }

            if(r->type == Star || r->type == Combine) {
                reg_exp_t res = new RegExp(currentSatProcess, this, r->value->zero());

                list<reg_exp_t>::iterator it;
                for(it = r->children.begin(); it != r->children.end(); it++) {
                    reg_exp_t temp = minimize_height(*it, cache);
                    res->children.push_back(temp);
                    my_set_union(res->outnodes, temp->outnodes);
                }

                res->type = r->type;
                res->value = r->value;
                res->last_seen = r->last_seen;
                res->last_change = r->last_change;

                cache[r] = res;
                return res;
            }

            // Now r->type == Extend
#define MINIMIZE_HEIGHT 2
#if MINIMIZE_HEIGHT==1 // Commutative Huffman-style tree
            list<reg_exp_t>::iterator it;
            multiset< heap_t, cmp_heap_t > heap;

            for(it = r->children.begin(); it != r->children.end(); it++) {
                reg_exp_t temp = minimize_height(*it, cache);
                heap.insert(heap_t(temp->outnodes.size(), temp));
            }

            while(heap.size() != 1) {
                heap_t e1 = *heap.begin();
                heap.erase(heap.begin());
                heap_t e2 = *heap.begin();
                heap.erase(heap.begin());

                reg_exp_t res = new RegExp(currentSatProcess, this, Extend, e1.second, e2.second);
                res->value = r->value;
                res->last_seen = r->last_seen;
                res->last_change = r->last_change;
                my_set_union(res->outnodes, e1.second->outnodes);
                my_set_union(res->outnodes, e2.second->outnodes);

                heap.insert(heap_t(e1.second->outnodes.size() + e2.second->outnodes.size(), res));
            }

            reg_exp_t ans = (*heap.begin()).second;
#elif MINIMIZE_HEIGHT==2 // Non-Commutative Huffman-style tree
            list<reg_exp_t>::iterator it;
            list<reg_exp_t> heap;

            for(it = r->children.begin(); it != r->children.end(); it++) {
                reg_exp_t temp = minimize_height(*it, cache);
                heap.push_back(temp);
            }

            while(heap.size() != 1) {
                list<reg_exp_t>::iterator min_pos = heap.begin(), next_it;
                size_t min = (*min_pos)->outnodes.size();
                it = heap.begin();
                it++;
                min += (*it)->outnodes.size();
                for(; it != heap.end(); it++) {
                    next_it = it;
                    next_it++;
                    if(next_it == heap.end())
                        break;
                    if( (*it)->outnodes.size() + (*next_it)->outnodes.size() < min) {
                        min_pos = it;
                        min = (*it)->outnodes.size() + (*next_it)->outnodes.size();
                    }
                }
                next_it = min_pos; next_it++;
                reg_exp_t r1 = (*min_pos);
                heap.erase(min_pos);
                min_pos = next_it; next_it++;
                reg_exp_t r2 = (*min_pos);
                heap.erase(min_pos);
                min_pos = next_it;

                reg_exp_t res = new RegExp(currentSatProcess, this, Extend, r1, r2);
                res->value = r->value;
                res->last_seen = r->last_seen;
                res->last_change = r->last_change;
                my_set_union(res->outnodes, r1->outnodes);
                my_set_union(res->outnodes, r2->outnodes);

                heap.insert(min_pos,res);
            }

            reg_exp_t ans = heap.front();
#elif MINIMIZE_HEIGHT==3 // Binary tree
            list<reg_exp_t>::iterator it, next_it;
            list<reg_exp_t> *list1 = new list<reg_exp_t>;
            list<reg_exp_t> *list2 = new list<reg_exp_t>;
            list<reg_exp_t> *temp;

            for(it = r->children.begin(); it != r->children.end(); it++) {
                reg_exp_t temp = minimize_height(*it, cache);
                list1->push_back(temp);
            }
            while(list1->size() != 1) {
                for(it = list1->begin(); it != list1->end(); it++) {
                    next_it = it; next_it++;
                    if(next_it == list1->end()) {
                        list2->push_back(*it);
                    } else {
                        reg_exp_t r1 = *it, r2 = *next_it;
                        reg_exp_t res = new RegExp(currentSatProcess, this, Extend, r1, r2);
                        res->value = r->value;
                        res->last_seen = r->last_seen;
                        res->last_change = r->last_change;
                        list2->push_back(res);
                        it++;
                    }
                }
                list1->clear();
                temp = list1; list1 = list2; list2 = temp;
            }
            reg_exp_t ans = list1->front();
#endif
            cache[r] = ans;
            return ans;

        }

        // This is a wrapper around _compress to manipulate the roots
        // hashmap only once per call to compress
        reg_exp_t RegExpDag::compress(reg_exp_t r, reg_exp_cache_t &cache) 
        {
          reg_exp_t res = _compress(r,cache);
#if defined(PPP_DGB) && PPP_DBG >= 0
          // If r was a root, replace it with res
          reg_exp_key_t delKey(r->type, r);
          if(rootsInSatProcess.find(delKey) != rootsInSatProcess.end()){
            rootsInSatProcess.erase(delKey);
            reg_exp_key_t insKey(res->type, res);
            rootsInSatProcess.insert(insKey, res);
          }
#endif
          return res;
        }

        // precond: Extend and Combine have 2 successsors and Star has 1
        // postcond: r is not changed
        reg_exp_t RegExpDag::_compress(reg_exp_t r, reg_exp_cache_t &cache) {
            reg_exp_cache_t::iterator cpos = cache.find(r);
            if(cpos != cache.end()) {
                return cpos->second;
            }
            if(r->type == Constant || r->type == Updatable) {
                // no need to cache these
                return r;
            }
            if(r->type == Star) {
                reg_exp_t ch = r->children.front();
                ch = compress(ch, cache);
                if(ch->type == Star) {
                    cache[r] = ch;
                    return ch;
                }
                reg_exp_t res = new RegExp(currentSatProcess, this, Star,ch);
                res->last_seen = r->last_seen;
                res->last_change = r->last_change;
                res->value = r->value;
                cache[r] = res;
                return res;
            }
            reg_exp_t res;
            if(r->type == Extend) {
                assert(r->children.size() == 2);
                list<reg_exp_t>::iterator it = r->children.begin();
                reg_exp_t r1 = *it;
                it++;
                reg_exp_t r2 = *it;
                r1 = compress(r1, cache);
                r2 = compress(r2, cache);
                res = compressExtend(r1,r2);
            } else if(r->type == Combine) {
                list<reg_exp_t>::iterator it = r->children.begin();
                reg_exp_t r1 = *it;
                it++;
                reg_exp_t r2 = *it;
                r1 = compress(r1, cache);
                r2 = compress(r2, cache);
                res = compressCombine(r1,r2);
            }
            if(res->type == Constant || res->type == Updatable) {
                res->last_seen = 1;
                res->last_change = 1;
            } else {
                res->last_seen = r->last_seen;
                res->last_change = r->last_change;
                res->value = r->value;
            }
            cache[r] = res;
            return res;
        }

        reg_exp_t RegExpDag::compressCombine(reg_exp_t r1, reg_exp_t r2) {
            if(r1.get_ptr() == r2.get_ptr()) {
                return r1;
            }

            if(r1->type == Constant && r2->type == Constant) {
                reg_exp_t res = new RegExp(currentSatProcess, this, r1->value->combine(r2->value));
                STAT(stats.ncombine++);
                return res;
            }
            reg_exp_t res = new RegExp(currentSatProcess, this, r1->value->zero());
            res->type = Combine;

            if(r1->type == Combine && r2->type == Combine) {

                reg_exp_t fc1 = r1->children.front();
                reg_exp_t fc2 = r2->children.front();
                if(fc1->type == Constant && fc2->type == Constant) {
                    reg_exp_t fc = new RegExp(currentSatProcess, this, fc1->value->combine(fc2->value));
                    STAT(stats.ncombine++);
                    res->children.push_back(fc);
                    res->children.insert(res->children.end(), ++r1->children.begin(), r1->children.end());
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                } else if(fc2->type == Constant) {
                    res->children.push_back(fc2);
                    res->children.insert(res->children.end(), r1->children.begin(), r1->children.end());
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                } else {
                    res->children.insert(res->children.end(), r1->children.begin(), r1->children.end());
                    res->children.insert(res->children.end(), r2->children.begin(), r2->children.end());
                }
                return res;
            }
            if(r1->type == Constant && r2->type == Combine) {

                reg_exp_t fc2 = r2->children.front();
                if(fc2->type == Constant) {
                    reg_exp_t fc = new RegExp(currentSatProcess, this, fc2->value->combine(r1->value));
                    STAT(stats.ncombine++);
                    res->children.push_back(fc);
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                    return res;
                }
            }
            if(r1->type == Combine && r2->type == Constant) {

                reg_exp_t fc1 = r1->children.front();
                if(fc1->type == Constant) {
                    reg_exp_t fc = new RegExp(currentSatProcess, this, fc1->value->combine(r2->value));
                    STAT(stats.ncombine++);
                    res->children.push_back(fc);
                    res->children.insert(res->children.end(), ++r1->children.begin(), r1->children.end());
                    return res;
                }
            }
            if(r1->type == Combine) {
                res->children.insert(res->children.end(), r1->children.begin(), r1->children.end());
                res->children.push_back(r2);
                return res;
            }
            if(r2->type == Combine) {
                res->children.insert(res->children.end(), r2->children.begin(), r2->children.end());
                res->children.push_back(r1);
                return res;
            }
            if(r2->type == Constant) {
                res->children.push_back(r2);
                res->children.push_back(r1);
            } else {
                res->children.push_back(r1);
                res->children.push_back(r2);
            }
            return res;
        }

        reg_exp_t RegExpDag::compressExtend(reg_exp_t r1, reg_exp_t r2) {
#ifndef COMMUTATIVE_EXTEND
            if(r1->type == Constant && r2->type == Constant) {
                reg_exp_t res = new RegExp(currentSatProcess, this, r1->value->extend(r2->value));
                STAT(stats.nextend++);
                return res;
            }
            reg_exp_t res = new RegExp(currentSatProcess, this, r1->value->zero());
            res->type = Extend;

            if(r1->type == Extend && r2->type == Extend) {

                reg_exp_t lc = r1->children.back();
                reg_exp_t fc = r2->children.front();
                if(lc->type == Constant && fc->type == Constant) {
                    reg_exp_t mc = new RegExp(currentSatProcess, this, lc->value->extend(fc->value));
                    STAT(stats.nextend++);
                    res->children.insert(res->children.end(), r1->children.begin(), --r1->children.end());
                    res->children.push_back(mc);
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                } else {
                    res->children.insert(res->children.end(), r1->children.begin(), r1->children.end());
                    res->children.insert(res->children.end(), r2->children.begin(), r2->children.end());
                }
                return res;
            }

            if(r1->type == Constant && r2->type == Extend) {
                reg_exp_t fc = r2->children.front();
                if(fc->type == Constant) {
                    reg_exp_t f = new RegExp(currentSatProcess, this, r1->value->extend(fc->value));
                    STAT(stats.nextend++);
                    res->children.push_back(f);
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                    return res;
                }
            }
            if(r1->type == Extend && r2->type == Constant) {
                reg_exp_t lc = r1->children.back();
                if(lc->type == Constant) {
                    reg_exp_t l = new RegExp(currentSatProcess, this, lc->value->extend(r2->value));
                    STAT(stats.nextend++);
                    res->children.insert(res->children.end(), r1->children.begin(), --r1->children.end());
                    res->children.push_back(l);
                    return res;
                }
            }
            if(r1->type == Extend) {
                res->children.insert(res->children.end(), r1->children.begin(), r1->children.end());
                res->children.push_back(r2);
                return res;
            }
            if(r2->type == Extend) {
                res->children.insert(res->children.end(), r2->children.begin(), r2->children.end());
                res->children.push_front(r1);
                return res;
            }
            res->children.push_back(r1);
            res->children.push_back(r2);
            return res;
#else // COMMUTATIVE_EXTEND

            if(r1->type == Constant && r2->type == Constant) {
                reg_exp_t res = new RegExp(currentSatProcess, this, r1->value->extend(r2->value));
                STAT(stats.nextend++);
                return res;
            }
            reg_exp_t res = new RegExp(currentSatProcess, this, r1->value->zero());
            res->type = Extend;

            if(r1->type == Extend && r2->type == Extend) {

                reg_exp_t fc1 = r1->children.front();
                reg_exp_t fc2 = r2->children.front();
                if(fc1->type == Constant && fc2->type == Constant) {
                    reg_exp_t fc = new RegExp(currentSatProcess, this, fc1->value->extend(fc2->value));
                    STAT(stats.nextend++);
                    res->children.push_back(fc);
                    res->children.insert(res->children.end(), ++r1->children.begin(), r1->children.end());
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                } else if(fc2->type == Constant) {
                    res->children.push_back(fc2);
                    res->children.insert(res->children.end(), r1->children.begin(), r1->children.end());
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                } else {
                    res->children.insert(res->children.end(), r1->children.begin(), r1->children.end());
                    res->children.insert(res->children.end(), r2->children.begin(), r2->children.end());
                }
                return res;
            }
            if(r1->type == Constant && r2->type == Extend) {

                reg_exp_t fc2 = r2->children.front();
                if(fc2->type == Constant) {
                    reg_exp_t fc = new RegExp(currentSatProcess, this, fc2->value->extend(r1->value));
                    STAT(stats.nextend++);
                    res->children.push_back(fc);
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                    return res;
                }
            }
            if(r1->type == Extend && r2->type == Constant) {

                reg_exp_t fc1 = r1->children.front();
                if(fc1->type == Constant) {
                    reg_exp_t fc = new RegExp(currentSatProcess, this, fc1->value->extend(r2->value));
                    STAT(stats.nextend++);
                    res->children.push_back(fc);
                    res->children.insert(res->children.end(), ++r1->children.begin(), r1->children.end());
                    return res;
                }
            }
            if(r1->type == Extend) {
                res->children.insert(res->children.end(), r1->children.begin(), r1->children.end());
                res->children.push_back(r2);
                return res;
            }
            if(r2->type == Extend) {
                res->children.insert(res->children.end(), r2->children.begin(), r2->children.end());
                res->children.push_back(r1);
                return res;
            }
            if(r2->type == Constant) {
                res->children.push_back(r2);
                res->children.push_back(r1);
            } else {
                res->children.push_back(r1);
                res->children.push_back(r2);
            }
            return res;

#endif

        }

        int RegExp::calculate_height(set<RegExp *> &visited, out_node_stat_t &stat_map) {
            assert(stat_map.size() == 0);
            if(visited.find(this) != visited.end()) {
                stat_map = outnode_height;
                out_node_stat_t::iterator it;
                for(it = stat_map.begin(); it != stat_map.end(); it++) {
                    it->second = out_node_height_t(0,0); // reset value because visited=true
                }
                return 0;
            }
            int changestat = samechange; // (samechange - differentchange) + 1;
            visited.insert(this);
            switch(type) {
                case Constant:
                    break;
                case Updatable:
                    stat_map[updatable_node_no] = out_node_height_t(0,0);
                    break;
                case Star: {
                               assert(children.size() == 1);
                               list<reg_exp_t>::iterator ch = children.begin();
                               out_node_stat_t stat_map_ch;
                               changestat += (*ch)->calculate_height(visited,stat_map_ch);
                               out_node_stat_t::iterator it;
                               for(it = stat_map_ch.begin(); it != stat_map_ch.end(); it++) {
                                   stat_map[it->first] = out_node_height_t(it->second.first + 1, it->second.second + 1);
                               }
                               break;
                           }
                case Extend: {
                                 list<reg_exp_t>::iterator ch;
                                 for(ch = children.begin(); ch != children.end(); ch++) {
                                     out_node_stat_t stat_map_ch;
                                     changestat += (*ch)->calculate_height(visited,stat_map_ch);
                                     out_node_stat_t::iterator it;
                                     for(it = stat_map_ch.begin(); it != stat_map_ch.end(); it++) {
                                         if(stat_map.find(it->first) == stat_map.end()) {
                                             stat_map[it->first] = out_node_height_t(it->second.first + (children.size() -1), it->second.second);
                                         } else { 
                                             out_node_height_t h = stat_map[it->first];
                                             stat_map[it->first] = out_node_height_t(h.first + it->second.first, h.second + it->second.second);
                                         }
                                     }
                                 }
                                 break;
                             }
                case Combine: {
                                  list<reg_exp_t>::iterator ch;
                                  for(ch = children.begin(); ch != children.end(); ch++) {
                                      out_node_stat_t stat_map_ch;
                                      changestat += (*ch)->calculate_height(visited,stat_map_ch);
                                      out_node_stat_t::iterator it;
                                      for(it = stat_map_ch.begin(); it != stat_map_ch.end(); it++) {
                                          if(stat_map.find(it->first) == stat_map.end()) {
                                              stat_map[it->first] = out_node_height_t(it->second.first + 1, it->second.second);
                                          } else { 
                                              out_node_height_t h = stat_map[it->first];
                                              stat_map[it->first] = out_node_height_t(h.first + it->second.first, h.second + it->second.second);
                                          }
                                      }
                                  }
                                  break;
                              }
            }
            outnode_height = stat_map;
            return changestat;
        }

        int RegExpDag::out_node_height(set<RegExp *> reg_equations) {
            set<RegExp *> visited;
            out_node_stat_t stat_map;
            set<RegExp *>::iterator it;
            int changestat = 0;
            for(it = reg_equations.begin(); it != reg_equations.end(); it++) {
                visited.clear();
                stat_map.clear();
                changestat += (*it)->calculate_height(visited,stat_map);
                out_node_stat_t::iterator it2;
                for(it2 = stat_map.begin(); it2 != stat_map.end(); it2++) {
                    stats.height += it2->second.first;
                    stats.lnd += it2->second.second;
                    stats.out_nodes++;
                }
            }
            return changestat;
        }

        void RegExpDag::markReachable(reg_exp_t const r)
        { 
          reg_exp_key_t ekey(r->type, r);
          if(visited.find(ekey) != visited.end())
            return;
          visited.insert(ekey, r);
          for(list<reg_exp_t>::iterator it = r->children.begin(); it != r->children.end(); ++it)
            markReachable(*it);
        }

        void RegExpDag::computeMinimalRoots()
        {
          visited.clear();          
          for(reg_exp_hash_t::iterator it = graphLabelsInSatProcess.begin(); it != graphLabelsInSatProcess.end(); ++it){
            reg_exp_t root = it->second;
            for(list<reg_exp_t>::iterator cit = root->children.begin(); cit != root->children.end(); ++cit){
              reg_exp_t child = *cit;
              markReachable(child);
            }
          }

          for(reg_exp_hash_t::iterator it = graphLabelsInSatProcess.begin(); it != graphLabelsInSatProcess.end(); ++it){
            if(visited.find(it->first) == visited.end())
              minimalRoots.insert(*it);
          }
          visited.clear();
        }

        void RegExpDag::evaluateRoots()
        {
          for(reg_exp_hash_t::const_iterator iter = minimalRoots.begin(); iter != minimalRoots.end(); ++iter){
            reg_exp_t regexp = iter->second;
#if defined(PUSH_EVAL)
            if(!regexp->dirty)
              continue;
#else
            if(regexp->last_seen == satProcesses[regexp->satProcess].update_count && regexp->last_change != (unsigned)-1)  // evaluate(w) sets last_change to -1
#endif
              continue;
            if(!top_down_eval || !saturation_complete)
              regexp->evaluate();
            else if(executing_poststar)
              regexp->evaluate(regexp->value->one());
            else 
              // Executing prestar
              regexp->evaluate();
          }
        }

        void RegExp::evaluate_iteratively() {
#if defined(PUSH_EVAL)
          assert(0 && "evaluate_iteratively not implemented for PUSH_EVAL mode");
#endif
          typedef list<reg_exp_t>::iterator iter_t;
          typedef pair<reg_exp_t, iter_t > stack_el;

          if(last_seen == dag->satProcesses[satProcess].update_count)
            return;

          list<stack_el> stack;
          stack.push_front(stack_el(this, children.begin()));

          while(!stack.empty()) {
            stack_el sel = stack.front();
            stack.pop_front();

            reg_exp_t re = sel.first;
            iter_t cit = sel.second;

            while( cit != re->children.end() && (*cit)->last_seen == dag->satProcesses[satProcess].update_count) {
              cit++;
            }

            if(cit == re->children.end()) {
              switch(re->type) {
                case Constant:
                case Updatable:
                  break;
                case Star: {
                             reg_exp_t ch = re->children.front();
                             if(ch->last_change <= re->last_seen) { // child did not change
                               re->last_seen = dag->satProcesses[satProcess].update_count;
                             } else {
                               sem_elem_t w = ch->value->star();
                               STAT(dag->stats.nstar++);
                               re->last_seen = dag->satProcesses[satProcess].update_count;
                               if(!re->value->equal(w)) {
                                 //last_change = update_count;
                                 re->last_change = ch->last_change;
                                 re->value = w;
                               }
                             }
                             break;
                           }
                case Extend: {
                               list<reg_exp_t>::iterator ch;
                               sem_elem_t wnew = re->value->one();
                               bool changed = false;
                               unsigned max = re->last_change;
                               for(ch = re->children.begin(); ch != re->children.end() && !changed; ch++) {
                                 changed = changed | ((*ch)->last_change > re->last_seen);
                               }
                               re->last_seen = dag->satProcesses[satProcess].update_count;
                               if(changed) {
                                 for(ch = re->children.begin(); ch != re->children.end(); ch++) {
                                   wnew = wnew->extend( (*ch)->value);
                                   max = ((*ch)->last_change > max) ? (*ch)->last_change : max;        
                                   STAT(dag->stats.nextend++);
                                 }
                                 if(!re->value->equal(wnew)) {
                                   re->last_change = max;
                                   re->value = wnew;
                                 }
                               }
                               break;
                             }
                case Combine: {
                                list<reg_exp_t>::iterator ch;
                                sem_elem_t wnew = re->value;
                                unsigned max = re->last_change;
                                for(ch = re->children.begin(); ch != re->children.end(); ch++) {
                                  if((*ch)->last_change > re->last_seen) {
                                    wnew = wnew->combine((*ch)->value);
                                    max = ((*ch)->last_change > max) ? (*ch)->last_change : max;
                                    STAT(dag->stats.ncombine++);
                                  }
                                }
                                re->last_seen = dag->satProcesses[satProcess].update_count;
                                if(!re->value->equal(wnew)) {
                                  re->last_change = max;
                                  re->value = wnew;
                                }
                                break;
                              }
              }
              re->last_change = (re->last_change > 1) ? re->last_change : 1;  
            } else { // cit != re->children.end()
              iter_t cit2 = cit;
              stack.push_front(stack_el(re, ++cit2));
              stack.push_front(stack_el(*cit, (*cit)->children.begin()));
            }
          }
        }

        sem_elem_t RegExp::evaluate(sem_elem_t w) {
          map<sem_elem_t, sem_elem_t,sem_elem_less>::iterator it;
          sem_elem_t ret;
#if defined(PUSH_EVAL)
          if(dirty){
            eval_map.clear();
            dirty = false;
          }else{
            it = eval_map.find(w);
            if(it != eval_map.end()){
              return it->second;
            }
          }
#else
          if(last_seen == dag->satProcesses[satProcess].update_count) {
            it = eval_map.find(w);
            if(it != eval_map.end()) {
              return it->second;
            } else { 
#if 0
              if(false && last_change != (unsigned)-1) {// "value" is available
                ret = w->extend(value);
                eval_map[w] = ret;
                return ret;
              }
#endif
            }
          } else {
            eval_map.clear();
          }
#endif //#if defined(PUSH_EVAL)
          unsigned int &update_count = dag->satProcesses[dag->currentSatProcess].update_count;
          evaluations.push_back(update_count);
          switch(type) {
            case Constant:
            case Updatable:
              ret = w->extend(value);
              STAT(dag->stats.nextend++);
              break;
            case Star: {
                         reg_exp_t ch = children.front();
                         sem_elem_t ans = w->zero(), nans = w;
                         sem_elem_t temp = w;
                         while(!ans->equal(nans)) {
                           ans = nans;
                           temp = ch->evaluate(temp);
                           nans = nans->combine(temp);
                           STAT(dag->stats.ncombine++);
                         }
                         /*
                         nans = ch->evaluate(w);
                         while(!ans->equal(nans)) {
                           ans = nans;
                           nans = nans->combine(ans->extend(ans));
                           STAT(dag->stats.ncombine++);
                           STAT(dag->stats.nextend++);
                         }
                         */
                         ret = nans;
                         break;
                       }
            case Extend: {
                           list<reg_exp_t>::iterator ch;
                           sem_elem_t temp = w;
                           for(ch = children.begin(); ch != children.end(); ch++) {
                             temp = (*ch)->evaluate(temp);
                           }
                           ret = temp;
                           break;
                         }
            case Combine: {
                            list<reg_exp_t>::iterator ch;
                            sem_elem_t temp = w->zero();
                            for(ch = children.begin(); ch != children.end(); ch++) {
                              temp = temp->combine((*ch)->evaluate(w));
                              STAT(dag->stats.ncombine++);
                            }
                            ret = temp;
                            break;
                          }
          }
          eval_map[w] = ret;
          last_seen = dag->satProcesses[satProcess].update_count; last_change = (unsigned)-1;
          return ret;
        }

        // Evaluate in reverse
        sem_elem_t RegExp::evaluateRev(sem_elem_t w) {
          map<sem_elem_t, sem_elem_t,sem_elem_less>::iterator it;
          sem_elem_t ret;

          if(last_seen == dag->satProcesses[satProcess].update_count) {
            it = eval_map.find(w);
            if(it != eval_map.end()) {
              return it->second;
            } else { 
#if 0
              if(false && last_change != (unsigned)-1) {// "value" is available
                ret = w->extend(value);
                eval_map[w] = ret;
                return ret;
              }
#endif
            }
          } else {
            eval_map.clear();
          }

          unsigned int &update_count = dag->satProcesses[dag->currentSatProcess].update_count;
          evaluations.push_back(update_count);
          switch(type) {
            case Constant:
            case Updatable:
              ret = value->extend(w);
              STAT(dag->stats.nextend++);
              break;
            case Star: {
                         reg_exp_t ch = children.front();
                         sem_elem_t ans = w->zero(), nans = w;
                         sem_elem_t temp = w;
                         while(!ans->equal(nans)) {
                           ans = nans;
                           temp = ch->evaluateRev(temp);
                           nans = nans->combine(temp);
                           STAT(dag->stats.ncombine++);
                         }
                         ret = nans;
                         break;
                       }
            case Extend: {
                           list<reg_exp_t>::reverse_iterator ch;
                           sem_elem_t temp = w;
                           for(ch = children.rbegin(); ch != children.rend(); ch++) {
                             temp = (*ch)->evaluateRev(temp);
                           }
                           ret = temp;
                           break;
                         }
            case Combine: {
                            list<reg_exp_t>::iterator ch;
                            sem_elem_t temp = w->zero();
                            for(ch = children.begin(); ch != children.end(); ch++) {
                              temp = temp->combine((*ch)->evaluateRev(w));
                              STAT(dag->stats.ncombine++);
                            }
                              ret = temp;
                              break;
                          }
        }
        eval_map[w] = ret;
        last_seen = dag->satProcesses[satProcess].update_count; last_change = (unsigned)-1;
        return ret;
    }

#ifdef DWPDS
    sem_elem_t RegExp::get_delta(unsigned int ls) {
        if(type == Constant) {
            if(ls == 0) {
                return value; // Constants are assumed to get their value at update 1
            } else {
                return value->zero();
            }

        }
        delta_map_t::iterator it = delta.upper_bound(ls);
        sem_elem_t del = value->zero();
        if(it == delta.end()) {
            return del;
        }
        //int cnt=0;
        while(it != delta.end()) {
            del = del->combine(it->second);
            it++;
            //cnt++;
        }
        //if(cnt > 1) {
        //    return value;
        //}
        return del;
    }
#endif

    sem_elem_t RegExp::get_weight() {
        if(last_seen == dag->satProcesses[satProcess].update_count && last_change != (unsigned)-1)  // evaluate(w) sets last_change to -1
            return value;

        if(!dag->top_down_eval || !dag->saturation_complete) {
            evaluate();
            return value;
        }
        if(dag->executing_poststar) {
            return evaluate(value->one());
        }
        // Executing prestar
        evaluate();
        return value;
#if 0
        // EvaluateRev does not seem to do a better job than evaluate()
        return evaluateRev(value->one());
#endif
    }

    void RegExp::evaluate() {
#if defined(PUSH_EVAL)
        if(!dirty){
          last_seen = dag->satProcesses[satProcess].update_count;
          return;
        }
#endif
        if(last_seen == dag->satProcesses[satProcess].update_count) return;
        unsigned int &update_count = dag->satProcesses[dag->currentSatProcess].update_count;
        evaluations.push_back(update_count);
        nevals++;
        switch(type) {
            case Constant: 
            case Updatable: 
                return;
            case Star: {
                           reg_exp_t ch = children.front();
                           ch->evaluate();
                           if(ch->last_change > last_seen) { // child did not change
#ifdef DWPDS
                               sem_elem_t w = value->one(),del = value->one(),temp;
                               while(!del->equal(value->zero())) {
                                   temp = del->extend(ch->value);
                                   del = temp->diff(w);
                                   w = w->combine(temp);
                               }
#else
                               sem_elem_t w = ch->value->star();
#endif
                               STAT(dag->stats.nstar++);

                               if(!value->equal(w)) {
                                   last_change = ch->last_change;
#ifdef DWPDS
                                   sem_elem_t dval = w->diff(value);
                                   delta[last_change] = dval;
#endif
                                   value = w;
                               }
                           }
                           last_seen = dag->satProcesses[satProcess].update_count;
                           break;
                       }
            case Combine: {
                              list<reg_exp_t>::iterator ch;
                              sem_elem_t wnew = value;
                              sem_elem_t wchange = value->zero();
                              unsigned max = last_change;
                              for(ch = children.begin(); ch != children.end(); ch++) {
                                  (*ch)->evaluate();
                                  if((*ch)->last_change > last_seen) {
#ifdef DWPDS
                                      wchange = wchange->combine((*ch)->get_delta(last_seen));
#else
                                      wchange = wchange->combine((*ch)->value);
#endif
                                      max = ((*ch)->last_change > max) ? (*ch)->last_change : max;
                                      STAT(dag->stats.ncombine++);
                                  }
                              }
                              wnew = wnew->combine(wchange);

                              if(!value->equal(wnew)) {
                                  last_change = max;
#ifdef DWPDS
                                  sem_elem_t dval = wchange->diff(value);
                                  delta[last_change] = dval; // wchange->diff(value)
#endif
                                  value = wnew;
                              }
                              last_seen = dag->satProcesses[satProcess].update_count;
                              break;
                          }
            case Extend: {
                             list<reg_exp_t>::iterator ch;
                             sem_elem_t wnew;
                             bool changed = false;
                             unsigned max = last_change;
                             int thechange = 0, cnt=1;
                             /*
                                for(ch = children.begin(); ch != children.end(); ch++) {
                                (*ch)->evaluate();
                                changed = changed | ((*ch)->last_change > last_seen);
                                if((*ch)->last_change > last_seen) thechange += cnt;
                                cnt *= 2;
                                }
                                */
                             list<reg_exp_t>::reverse_iterator rch;
                             for(rch = children.rbegin(); rch != children.rend(); rch++) {
                                 (*rch)->evaluate();
                                 changed = changed | ((*rch)->last_change > last_seen);
                                 if((*rch)->last_change > last_seen) thechange += cnt;
                                 cnt *= 2;
                             }
                             if(changed) {
                                 if(lastchange == -1 || thechange != lastchange) differentchange++;
                                 else samechange++;
                                 lastchange=thechange;
                             }

                             if(changed) {
#ifdef DWPDS
                                 list<reg_exp_t>::iterator sel;
                                 sem_elem_t del;
                                 wnew = value->zero();
                                 for(sel = children.begin(); sel != children.end(); sel++) {
                                     del = value->one();
                                     for(ch = children.begin(); ch != children.end(); ch++) {
                                         if(ch == sel) {
                                             del = del->extend((*ch)->get_delta(last_seen));
                                         } else {
                                             del = del->extend((*ch)->value);
                                         }
                                     }
                                     wnew = wnew->combine(del);
                                     max = ((*sel)->last_change > max) ? (*sel)->last_change : max;
                                 }
                                 del = wnew->diff(value);
                                 wnew = wnew->combine(value);
#else
                                 wnew = value->one();
                                 for(ch = children.begin(); ch != children.end(); ch++) {
                                     wnew = wnew->extend( (*ch)->value);
                                     max = ((*ch)->last_change > max) ? (*ch)->last_change : max;    
                                     STAT(dag->stats.nextend++);
                                 }
#endif
                                 if(!value->equal(wnew)) {
                                     last_change = max;
#ifdef DWPDS
                                     delta[last_change] = wnew->diff(value); // del;
#endif
                                     value = wnew;
                                 }
                             }
                             last_seen = dag->satProcesses[satProcess].update_count;
                             break;
                         }
        }
        last_change = (last_change > 1) ? last_change : 1;
        assert(last_seen == dag->satProcesses[satProcess].update_count);
#if defined(PUSH_EVAL)
        dirty = false;
#endif 
    }

    sem_elem_t RegExp::reevaluate() {
        // reset uptodate
        set<RegExp *> gray;
        set<RegExp *> black;
        dfs(gray,black);
        set<RegExp *>::iterator it;
        vector<sem_elem_t> vals;
        for(it = black.begin(); it != black.end(); it++) {
            (*it)->uptodate = false;
            vals.push_back((*it)->value);
        }

        sem_elem_t ret = reevaluateIter();

        int i=0;
        for(it = black.begin(); it != black.end(); it++) {
            if((*it)->type == Constant || (*it)->type == Updatable) {
                assert((*it)->value->equal(vals[i]));
            }
            (*it)->value = vals[i];
            i++;
        }

        return ret;
    }

    sem_elem_t RegExp::reevaluateIter() {
        if(type == Constant || type == Updatable) {
            return value;
        }
        if(uptodate) {
            return value;
        }
        list<reg_exp_t>::iterator it = children.begin();
        for(; it != children.end(); it++) {
            (*it)->reevaluateIter();
        }
        uptodate = true;
        if(type == Star) {
            it = children.begin();
            value = (*it)->value->star();
            STAT(dag->stats.nstar++);
        } else if(type == Extend) {
            it = children.begin();
            value = value->one();
            for(; it != children.end(); it++) {
                value = value->extend((*it)->value);
                STAT(dag->stats.nextend++);
            }
        } else {
            it = children.begin();
            value = value->zero();
            for(; it != children.end(); it++) {
                value = value->combine((*it)->value);
                STAT(dag->stats.ncombine++);
            }
        }
        return value;
    }

    // for debugging
    bool RegExp::isCyclic() {
        set<RegExp *> gray;
        set<RegExp *> black;
        return dfs(gray,black);
    }

    bool RegExp::dfs(set<RegExp *> &gray, set<RegExp *> &black) {
        if(type == Constant || type == Updatable) {
            black.insert(this);
            return false;
        }
        gray.insert(this);
        list<reg_exp_t>::iterator ch = children.begin();
        for(; ch != children.end(); ch++) {
            set<RegExp *>::iterator it = gray.find((*ch).get_ptr());
            if(it != gray.end()) { // cycle
                return true;
            }
            it = black.find((*ch).get_ptr());
            if(it == black.end()) {
                bool r = (*ch)->dfs(gray,black);
                if(r) return true;
            }
        }
        gray.erase(this);
        black.insert(this);
        return false;
    }

    RegExpDag::~RegExpDag()
    {
        satProcesses.clear();
        currentSatProcess = 0;
        extend_backwards = false;
        reg_exp_hash.clear();
        const_reg_exp_hash.clear();
#if defined(PPP_DGB) && PPP_DBG >= 0
        rootsInSatProcess.clear();
        rootsAcrossSatProcesses.clear();
#endif
        graphLabelsInSatProcess.clear();
        graphLabelsAcrossSatProcesses.clear();
        stats.reset();
        reg_exp_one = NULL;
        reg_exp_zero = NULL;
        saturation_complete = false;
        executing_poststar = true;
        initialized = false;
        top_down_eval = true;
        visited.clear();
    }

#if defined(PPP_DGB) && PPP_DBG >= 0
    void RegExpDag::printStructureInformation()
    {
      long nodes = countTotalNodes();
      long leaves = countTotalLeaves();
      long height = getHeight();
      long splines = countSpline();
      long frontiers = countFrontier();
      long graphls = graphLabelsAcrossSatProcesses.size();
      long roots = rootsAcrossSatProcesses.size();
      long errorls = countLabelsUnderNonLabelRoots();

      // Find roots that are also labels
      reg_exp_hash_t rootsThatAreLabels;
      for(reg_exp_hash_t::iterator it = rootsAcrossSatProcesses.begin();
          it != rootsAcrossSatProcesses.end();
          ++it)
        if(graphLabelsAcrossSatProcesses.find(it->first) != graphLabelsAcrossSatProcesses.end())
          rootsThatAreLabels[it->first] = it->second;
      long rootsNgraphls = rootsThatAreLabels.size();

      cout << "RegExp statistics:" << endl;
      cout << "#Nodes: " << nodes << endl;
      cout << "#Leaves: " << leaves << endl;
      cout << "#Spline: " << splines << endl;
      cout << "#Frontiers: " << frontiers << endl;
      cout << "#Labels: " << graphls << endl;
      cout << "#Labels ^ Roots: " << rootsNgraphls << endl;
      cout << "#Labels under non-label roots: " << errorls << endl;
      if(nodes > 0){
        cout << "Spline/nodes %: " << ((double) splines * 100) / ((double) nodes) << endl;
        cout << "Frontier/nodes %: " << ((double) frontiers * 100) / ((double) nodes) << endl;
        cout << "label nodes/nodes %: " << ((double) graphls * 100) / ((double) nodes) << endl;
        cout << "Height/log(nodes) %: " << ((double) height * 100) * log10(2)  / log10((double) nodes) << endl;
        cout << "roots/nodes %: " << ((double) roots * 100) /((double) nodes) << endl;
        cout << "(roots intersect labels)/nodes %: " << ((double) rootsNgraphls * 100) /((double) nodes) << endl;
        cout << "(labels under non-label roots) / nodes %: " << ((double) errorls * 100) / ((double) nodes) << endl;
      }
    }

    long RegExpDag::countLabelsUnderNonLabelRoots()
    {
      long count = 0;
      visited.clear();
      reg_exp_hash_t rootsThatAreLabels;
      for(reg_exp_hash_t::iterator it = rootsAcrossSatProcesses.begin();
          it != rootsAcrossSatProcesses.end();
          ++it){
        if(graphLabelsAcrossSatProcesses.find(it->first) != graphLabelsAcrossSatProcesses.end()){
          count += countLabels(it->second);
        }
      }
      visited.clear();
      return count;
    }

    long RegExpDag::countLabels(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      reg_exp_hash_t::iterator it = visited.find(ekey);
      if(it != visited.end())
        return 0;
      visited.insert(ekey, e);
      long total = 0;
      for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit)
        total += countLabels(*cit);
      if(graphLabelsAcrossSatProcesses.find(ekey) != graphLabelsAcrossSatProcesses.end())
        total += 1;
      return total;
    }

    long RegExpDag::getHeight()
    {
      long max = 0;
      height.clear();
      for(reg_exp_hash_t::const_iterator rit = rootsAcrossSatProcesses.begin(); rit != rootsAcrossSatProcesses.end(); ++rit){
        long cur = getHeight(rit->second);
        max = cur > max ? cur : max;
      }
      return max;
    }

    // Relies on the dag actually being a dag
    // Will go into an infinite loop otherwise
    long RegExpDag::getHeight(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      wali::HashMap<reg_exp_key_t, long, hash_reg_exp_key, reg_exp_key_t>::iterator it = height.find(ekey);
      if(it != height.end())
        return it->second;
      long max = 0;
      if(e->children.size() == 0){
        max = 1;
      }else{
        for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit){
          long cur = getHeight(*cit);
          max = cur > max ? cur : max;
        }
      }
      height[ekey] = max;
      return max;
    }

    long RegExpDag::countSpline(){
      markSpline();
      return (long) spline.size();
    }
    void RegExpDag::markSpline()
    {
      visited.clear();
      spline.clear();
      for(reg_exp_hash_t::const_iterator rit = rootsAcrossSatProcesses.begin(); rit != rootsAcrossSatProcesses.end(); ++rit)
        markSpline(rit->second);
    }

    // Relies on dag actually being a dag
    // will give incorrect answers (will not mark all) otherwise.
    bool RegExpDag::markSpline(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      reg_exp_hash_t::iterator it = visited.find(ekey);
      if(it != visited.end()){
        return spline.find(ekey) != spline.end();
      }
      visited.insert(ekey, e);
      bool onSpline = false;
      for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit)
        onSpline |= markSpline(*cit);
      if(e->type == Updatable)
       onSpline = true; 
      if(onSpline)
        spline.insert(ekey, e);
      return onSpline;
    }

    long RegExpDag::countFrontier()
    {
      markSpline();
      visited.clear();
      long count = 0;
      for(reg_exp_hash_t::const_iterator rit = rootsAcrossSatProcesses.begin(); rit != rootsAcrossSatProcesses.end(); ++rit)
        count += countFrontier(rit->second);
      return count;
    }

    long RegExpDag::countFrontier(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      reg_exp_hash_t::iterator it = visited.find(ekey);
      if(it != visited.end())
        return 0;
      visited.insert(ekey, e);
      it = spline.find(ekey);
      if(it == spline.end())
        return 0;
      long count = 0;
      for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit){
        reg_exp_key_t ckey((*cit)->type, *cit);
        if(spline.find(ckey) != spline.end()){
          count += countFrontier(*cit);
        }else{
          if(visited.find(ckey) == visited.end()){
            count += 1;
            visited.insert(ckey, *cit);
          }
        }
      }
      return count;
    }

    long RegExpDag::countTotalLeaves()
    {
      long total = 0;
      visited.clear();
      for(reg_exp_hash_t::const_iterator rit = rootsAcrossSatProcesses.begin(); rit != rootsAcrossSatProcesses.end(); ++rit)
        total += countTotalLeaves(rit->second);
      return total;
    }

    long RegExpDag::countTotalLeaves(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      reg_exp_hash_t::iterator it = visited.find(ekey);
      if(it != visited.end())
        return 0;
      visited.insert(ekey, e);
      if(e->children.size() == 0)
        return 1;
      long total = 0;
      for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit)
        total += countTotalLeaves(*cit);
      return total;
    }

    long RegExpDag::countTotalCombines()
    {
      long total = 0;
      visited.clear();
      for(reg_exp_hash_t::const_iterator rit = rootsAcrossSatProcesses.begin(); rit != rootsAcrossSatProcesses.end(); ++rit)
        total += countTotalCombines(rit->second);
      return total;
    }

    long RegExpDag::countTotalCombines(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      reg_exp_hash_t::iterator it = visited.find(ekey);
      if(it != visited.end())
        return 0;
      visited.insert(ekey, e);
      long total = 0;
      if(e->type == wali::graph::Combine)
        ++total;
      for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit)
        total += countTotalCombines(*cit);
      return total;
    }

    long RegExpDag::countTotalExtends()
    {
      long total = 0;
      visited.clear();
      for(reg_exp_hash_t::const_iterator rit = rootsAcrossSatProcesses.begin(); rit != rootsAcrossSatProcesses.end(); ++rit){
        total += countTotalExtends(rit->second);
      }
      return total;
    }

    long RegExpDag::countTotalExtends(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      reg_exp_hash_t::iterator it = visited.find(ekey);
      if(it != visited.end())
        return 0;
      visited.insert(ekey, e);
      long total = 0;
      if(e->type == wali::graph::Extend)
        ++total;
      for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit)
        total += countTotalExtends(*cit);
      return total;
    }

    long RegExpDag::countTotalStars()
    {
      long total = 0;
      visited.clear();
      for(reg_exp_hash_t::const_iterator rit = rootsAcrossSatProcesses.begin(); rit != rootsAcrossSatProcesses.end(); ++rit)
        total += countTotalStars(rit->second);
      return total;
    }

    long RegExpDag::countTotalStars(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      reg_exp_hash_t::iterator it = visited.find(ekey);
      if(it != visited.end())
        return 0;
      visited.insert(ekey, e);
      long total = 0;
      if(e->type == wali::graph::Star)
        ++total;
      for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit)
        total += countTotalStars(*cit);
      return total;
    }

    long RegExpDag::countExcept(std::vector<reg_exp_t>& exceptions)
    {
      visited.clear();
      for(vector<reg_exp_t>::const_iterator cit = exceptions.begin(); cit != exceptions.end(); ++cit)
        excludeFromCountReachable(*cit);
      long total = 0;
      for(reg_exp_hash_t::const_iterator cit = rootsAcrossSatProcesses.begin(); cit != rootsAcrossSatProcesses.end(); ++cit)
        total += countTotalNodes(cit->second);
      return total;
    }

    void RegExpDag::excludeFromCountReachable(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      reg_exp_hash_t::iterator it = visited.find(ekey);
      if(it != visited.end())
        return;
      visited.insert(ekey, e);
      for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit)
        excludeFromCountReachable(*cit);
    }

    long RegExpDag::countTotalNodes()
    {
      long total = 0;
      visited.clear();
      for(reg_exp_hash_t::const_iterator rit = rootsAcrossSatProcesses.begin(); rit != rootsAcrossSatProcesses.end(); ++rit){
        total += countTotalNodes(rit->second);
      }
      return total;
    }

    long RegExpDag::countTotalNodes(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      reg_exp_hash_t::iterator it = visited.find(ekey);
      if(it != visited.end())
        return 0;
      visited.insert(ekey, e);
      long total = 0;
      //if(e->type == Combine || e->type == Extend || e->type == Star)
      ++total;
      for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit)
        total += countTotalNodes(*cit);
      return total;
    }
    

    // Relies on the dag actually being a dag.
    // If not, a circular path has no root.
    void RegExpDag::sanitizeRootsAcrossSatProcesses()
    {
      for(reg_exp_hash_t::const_iterator rit = rootsAcrossSatProcesses.begin(); rit != rootsAcrossSatProcesses.end(); ++rit){
        visited.clear();
        reg_exp_t const e = rit->second;
        reg_exp_key_t ekey(e->type, e);
        visited.insert(ekey, e);
        for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit)
          removeDagFromRoots(*cit);
      }
    }
    
    void RegExpDag::removeDagFromRoots(reg_exp_t const e)
    {
      reg_exp_key_t ekey(e->type, e);
      reg_exp_hash_t::iterator it = visited.find(ekey);
      if(it != visited.end())
        return;
      visited.insert(ekey, e);      
      rootsAcrossSatProcesses.erase(ekey);
      for(list<reg_exp_t>::iterator cit = e->children.begin(); cit != e->children.end(); ++cit)
        removeDagFromRoots(*cit);
    }

#endif //#if defined(PPP_DGB) && PPP_DBG >= 0

    void RegExpDag::markAsLabel(reg_exp_t e)
    {
      reg_exp_key_t ekey(e->type, e);
      graphLabelsInSatProcess.insert(ekey,e);
    }

    } // namespace graph
} // namespace wali
