#include "wali/graph/RegExp.hpp"
#include "wali/graph/GraphCommon.hpp"
#include <algorithm>
#include <iterator>

namespace wali {

    namespace graph {

        vector<reg_exp_t> RegExp::updatable_nodes;
        
        vector<RegExpSatProcess> RegExp::satProcesses;
        unsigned long int RegExp::currentSatProcess = 0;

        bool RegExp::extend_backwards = false;
        reg_exp_hash_t RegExp::reg_exp_hash;
        const_reg_exp_hash_t RegExp::const_reg_exp_hash;
        RegExpStats RegExp::stats;
        reg_exp_t RegExp::reg_exp_one;
        reg_exp_t RegExp::reg_exp_zero;
        bool RegExp::saturation_complete = false;
        bool RegExp::executing_poststar = true;
        bool RegExp::initialized = false;
        bool RegExp::top_down_eval = true;

        reg_exp_t RegExp::updatable(node_no_t nno, sem_elem_t se) {

            if(saturation_complete) {
              cerr << "RegExp: Error: cannot create updatable nodes when saturation is complete\n";
              assert(!initialized);
              assert(0);
            }

            if(updatable_nodes.size() > nno) {
              return updatable_nodes[nno];
            }
            for(size_t i = updatable_nodes.size(); i < nno; i++) {
              updatable_nodes.push_back(new RegExp(i,se->zero()));
            }
            updatable_nodes.push_back(new RegExp(nno, se));
            return updatable_nodes[nno];
        }

        int RegExp::updatableNumber() {
            assert(type == Updatable);
            return (int)updatable_node_no;
        }

        void RegExp::update(node_no_t nno, sem_elem_t se) {
            if(saturation_complete) {
              cerr << "RegExp: Error: cannot update nodes when saturation is complete\n";
              assert(!initialized);
              assert(0);
            }

            updatable(nno,se); // make sure that this node exists
            if(!updatable_nodes[nno]->value->equal(se)) {
                unsigned int &update_count = satProcesses[currentSatProcess].update_count;

#ifdef DWPDS
                updatable_nodes[nno]->delta[update_count+1] = se->diff(updatable_nodes[nno]->value);
#endif
                update_count = update_count + 1;
                updatable_nodes[nno]->value = se;
                updatable_nodes[nno]->last_change = update_count;
                updatable_nodes[nno]->last_seen = update_count;
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

        // need not return an evaluated reg_exp
        reg_exp_t RegExp::star(reg_exp_t r) {
            if(r->type == Star) {
                return r;
            }
            if(r->type == Constant) {
                if(r->value->equal(r->value->zero())) {
                    return new RegExp(r->value->one());
                }
            }
#ifndef REGEXP_CACHING
            return new RegExp(Star, r);
#else // REGEXP_CACHING

            if(r->type == Constant) {
                if(r->value->equal(r->value->one())) {
                    return r;
                }
            }

            reg_exp_key_t rkey(Star, r);
            reg_exp_hash_t::iterator it = reg_exp_hash.find(rkey);
            if(it == reg_exp_hash.end()) {
                reg_exp_t res = new RegExp(Star, r);
                reg_exp_hash.insert(rkey, res);
                STAT(stats.hashmap_misses++);
                return res;
            }
            STAT(stats.hashmap_hits++);
            return it->second;
#endif // REGEXP_CACHING
        }

        reg_exp_t RegExp::combine(reg_exp_t r1, reg_exp_t r2) {
            if(r1.get_ptr() == r2.get_ptr()) 
                return r1;
            if(r1->type == Constant && r1->value->equal(r1->value->zero())) {
                return r2;
            } else if(r2->type == Constant && r2->value->equal(r2->value->zero())) {
                return r1;
            }
#ifndef REGEXP_CACHING
            return new RegExp(Combine, r1, r2);
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
                    reg_exp_t res = new RegExp(Combine, r1, r2);
                    reg_exp_hash.insert(rkey2, res);
                    STAT(stats.hashmap_misses++);
                    return res;
                }
            }
            STAT(stats.hashmap_hits++);
            return it->second;
#endif // REGEXP_CACHING
        }

        reg_exp_t RegExp::combine(list<reg_exp_t> &ls) {
          if(ls.size() == 0)
            return reg_exp_zero;
          if(ls.size() == 1)
            return *ls.begin();
          
          reg_exp_t reg = new RegExp(reg_exp_zero->value);
          reg->type = Combine;
          reg->children = ls;
          return reg;
        }
      

        reg_exp_t RegExp::extend(reg_exp_t r1, reg_exp_t r2) {
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
            return new RegExp(Extend, r1, r2);
#else
            if(r1->type == Constant && r1->value->equal(r1->value->one())) {
                return r2;
            } else if(r2->type == Constant && r2->value->equal(r2->value->one())) {
                return r1;
            }
            reg_exp_key_t rkey(Extend, r1, r2);
            reg_exp_hash_t::iterator it = reg_exp_hash.find(rkey);
            if(it == reg_exp_hash.end()) {
                reg_exp_t res = new RegExp(Extend, r1, r2);
                reg_exp_hash.insert(rkey, res);
                STAT(stats.hashmap_misses++);
                return res;
            }
            STAT(stats.hashmap_hits++);
            return it->second;
#endif // REGEXP_CACHING
        }

        reg_exp_t RegExp::constant(sem_elem_t se) {
            if(se->equal(se->zero()))
                return reg_exp_zero;
#ifndef REGEXP_CACHING
            return new RegExp(se);
#else
            if(se->equal(se->one()))
                return reg_exp_one;

            const_reg_exp_hash_t::iterator it = const_reg_exp_hash.find(se);
            if(it == const_reg_exp_hash.end()) {
                reg_exp_t res = new RegExp(se);
                const_reg_exp_hash.insert(se, res);
                return res;
            }
            return it->second;
#endif // REGEXP_CACHING
        }

        void RegExp::startSatProcess(const sem_elem_t se) {
          if(initialized) {
            cerr << "Error: RegExp initialized twice\n";
            assert(0);
          }
          initialized = true;
          
          satProcesses.push_back(RegExpSatProcess());
          currentSatProcess = satProcesses.size() - 1;
          
          reg_exp_hash.clear();
          const_reg_exp_hash.clear();
          updatable_nodes.clear();
          
          reg_exp_zero = new RegExp(se->zero());
          reg_exp_one = new RegExp(se->one());
          saturation_complete = false;
          executing_poststar = true;
        }
      
        void RegExp::stopSatProcess() {
          if(!initialized) {
            cerr << "Error: RegExp reset twice without being init-ed\n";
            assert(0);
          }
          initialized = false;
          saturation_complete = true;
          updatable_nodes.clear();
          
        }


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

        reg_exp_t RegExp::minimize_height(reg_exp_t r, reg_exp_cache_t &cache) {
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
                reg_exp_t res = new RegExp(r->value->zero());

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

                reg_exp_t res = new RegExp(Extend, e1.second, e2.second);
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

                reg_exp_t res = new RegExp(Extend, r1, r2);
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
                        reg_exp_t res = new RegExp(Extend, r1, r2);
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

        // precond: Extend and Combine have 2 successsors and Star has 1
        // postcond: r is not changed
        reg_exp_t RegExp::compress(reg_exp_t r, reg_exp_cache_t &cache) {
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
                reg_exp_t res = new RegExp(Star,ch);
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

        reg_exp_t RegExp::compressCombine(reg_exp_t r1, reg_exp_t r2) {
            if(r1.get_ptr() == r2.get_ptr()) {
                return r1;
            }

            if(r1->type == Constant && r2->type == Constant) {
                reg_exp_t res = new RegExp(r1->value->combine(r2->value));
                STAT(stats.ncombine++);
                return res;
            }
            reg_exp_t res = new RegExp(r1->value->zero());
            res->type = Combine;

            if(r1->type == Combine && r2->type == Combine) {

                reg_exp_t fc1 = r1->children.front();
                reg_exp_t fc2 = r2->children.front();
                if(fc1->type == Constant && fc2->type == Constant) {
                    reg_exp_t fc = new RegExp(fc1->value->combine(fc2->value));
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
                    reg_exp_t fc = new RegExp(fc2->value->combine(r1->value));
                    STAT(stats.ncombine++);
                    res->children.push_back(fc);
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                    return res;
                }
            }
            if(r1->type == Combine && r2->type == Constant) {

                reg_exp_t fc1 = r1->children.front();
                if(fc1->type == Constant) {
                    reg_exp_t fc = new RegExp(fc1->value->combine(r2->value));
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

        reg_exp_t RegExp::compressExtend(reg_exp_t r1, reg_exp_t r2) {
#ifndef COMMUTATIVE_EXTEND
            if(r1->type == Constant && r2->type == Constant) {
                reg_exp_t res = new RegExp(r1->value->extend(r2->value));
                STAT(stats.nextend++);
                return res;
            }
            reg_exp_t res = new RegExp(r1->value->zero());
            res->type = Extend;

            if(r1->type == Extend && r2->type == Extend) {

                reg_exp_t lc = r1->children.back();
                reg_exp_t fc = r2->children.front();
                if(lc->type == Constant && fc->type == Constant) {
                    reg_exp_t mc = new RegExp(lc->value->extend(fc->value));
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
                    reg_exp_t f = new RegExp(r1->value->extend(fc->value));
                    STAT(stats.nextend++);
                    res->children.push_back(f);
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                    return res;
                }
            }
            if(r1->type == Extend && r2->type == Constant) {
                reg_exp_t lc = r1->children.back();
                if(lc->type == Constant) {
                    reg_exp_t l = new RegExp(lc->value->extend(r2->value));
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
                reg_exp_t res = new RegExp(r1->value->extend(r2->value));
                STAT(stats.nextend++);
                return res;
            }
            reg_exp_t res = new RegExp(r1->value->zero());
            res->type = Extend;

            if(r1->type == Extend && r2->type == Extend) {

                reg_exp_t fc1 = r1->children.front();
                reg_exp_t fc2 = r2->children.front();
                if(fc1->type == Constant && fc2->type == Constant) {
                    reg_exp_t fc = new RegExp(fc1->value->extend(fc2->value));
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
                    reg_exp_t fc = new RegExp(fc2->value->extend(r1->value));
                    STAT(stats.nextend++);
                    res->children.push_back(fc);
                    res->children.insert(res->children.end(), ++r2->children.begin(), r2->children.end());
                    return res;
                }
            }
            if(r1->type == Extend && r2->type == Constant) {

                reg_exp_t fc1 = r1->children.front();
                if(fc1->type == Constant) {
                    reg_exp_t fc = new RegExp(fc1->value->extend(r2->value));
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

        int RegExp::out_node_height(set<RegExp *> reg_equations) {
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

        void RegExp::evaluate_iteratively() {
            typedef list<reg_exp_t>::iterator iter_t;
            typedef pair<reg_exp_t, iter_t > stack_el;

            if(last_seen == satProcesses[satProcess].update_count)
                return;

            list<stack_el> stack;
            stack.push_front(stack_el(this, children.begin()));

            while(!stack.empty()) {
                stack_el sel = stack.front();
                stack.pop_front();

                reg_exp_t re = sel.first;
                iter_t cit = sel.second;

                while( cit != re->children.end() && (*cit)->last_seen == satProcesses[satProcess].update_count) {
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
                                           re->last_seen = satProcesses[satProcess].update_count;
                                       } else {
                                           sem_elem_t w = ch->value->star();
                                           STAT(stats.nstar++);
                                           re->last_seen = satProcesses[satProcess].update_count;
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
                                         re->last_seen = satProcesses[satProcess].update_count;
                                         if(changed) {
                                             for(ch = re->children.begin(); ch != re->children.end(); ch++) {
                                                 wnew = wnew->extend( (*ch)->value);
                                                 max = ((*ch)->last_change > max) ? (*ch)->last_change : max;        
                                                 STAT(stats.nextend++);
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
                                                  STAT(stats.ncombine++);
                                              }
                                          }
                                          re->last_seen = satProcesses[satProcess].update_count;
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

            if(last_seen == satProcesses[satProcess].update_count) {
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

            switch(type) {
                case Constant:
                case Updatable:
                    ret = w->extend(value);
                    STAT(stats.nextend++);
                    break;
                case Star: {
                               reg_exp_t ch = children.front();
                               sem_elem_t ans = w->zero(), nans = w;
                               sem_elem_t temp = w;
                               while(!ans->equal(nans)) {
                                   ans = nans;
                                   temp = ch->evaluate(temp);
                                   nans = nans->combine(temp);
                                   STAT(stats.ncombine++);
                               }
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
                                      STAT(stats.ncombine++);
                                  }
                                  ret = temp;
                                  break;
                              }
            }
            eval_map[w] = ret;
            last_seen = satProcesses[satProcess].update_count; last_change = (unsigned)-1;
            return ret;
        }

    // Evaluate in reverse
    sem_elem_t RegExp::evaluateRev(sem_elem_t w) {
        map<sem_elem_t, sem_elem_t,sem_elem_less>::iterator it;
        sem_elem_t ret;

        if(last_seen == satProcesses[satProcess].update_count) {
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

        switch(type) {
            case Constant:
            case Updatable:
                ret = value->extend(w);
                STAT(stats.nextend++);
                break;
            case Star: {
                           reg_exp_t ch = children.front();
                           sem_elem_t ans = w->zero(), nans = w;
                           sem_elem_t temp = w;
                           while(!ans->equal(nans)) {
                               ans = nans;
                               temp = ch->evaluateRev(temp);
                               nans = nans->combine(temp);
                               STAT(stats.ncombine++);
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
                                  STAT(stats.ncombine++);
                              }
                              ret = temp;
                              break;
                          }
        }
        eval_map[w] = ret;
        last_seen = satProcesses[satProcess].update_count; last_change = (unsigned)-1;
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
        if(last_seen == satProcesses[satProcess].update_count && last_change != (unsigned)-1)  // evaluate(w) sets last_change to -1
            return value;

        if(!top_down_eval || !saturation_complete) {
            evaluate();
            return value;
        }
        if(executing_poststar) {
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
        if(last_seen == satProcesses[satProcess].update_count) return;
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
                               STAT(stats.nstar++);

                               if(!value->equal(w)) {
                                   last_change = ch->last_change;
#ifdef DWPDS
                                   sem_elem_t dval = w->diff(value);
                                   delta[last_change] = dval;
#endif
                                   value = w;
                               }
                           }
                           last_seen = satProcesses[satProcess].update_count;
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
                                      STAT(stats.ncombine++);
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
                              last_seen = satProcesses[satProcess].update_count;
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
                                     STAT(stats.nextend++);
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
                             last_seen = satProcesses[satProcess].update_count;
                             break;
                         }
        }
        last_change = (last_change > 1) ? last_change : 1;
        assert(last_seen == satProcesses[satProcess].update_count);
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
            STAT(stats.nstar++);
        } else if(type == Extend) {
            it = children.begin();
            value = value->one();
            for(; it != children.end(); it++) {
                value = value->extend((*it)->value);
                STAT(stats.nextend++);
            }
        } else {
            it = children.begin();
            value = value->zero();
            for(; it != children.end(); it++) {
                value = value->combine((*it)->value);
                STAT(stats.ncombine++);
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

    } // namespace graph
} // namespace wali
