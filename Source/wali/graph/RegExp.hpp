#ifndef wali_graph__REGEXP_H_
#define wali_graph__REGEXP_H_

#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <map>

#include "wali/SemElem.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/HashMap.hpp"

#include "wali/graph/GraphCommon.hpp"

#ifdef _WIN32
#include <time.h>
#endif

using namespace std;

/* RegExp usage:
 *
 * (1) The calls to RegExp::startSatProcess() and RegExp::stopSatProcess() must alternate.
 * (2) RegExp::startSatProcess() must be called once before RegExps can be used.
 * (3) The period between startSatProcess() and stopSatProcess() is available to carry out a saturation process
 * (4) RegExp::stopSatProcess() resets the set of updatable nodes and fixes old updatable nodes to act as
 *     constants forever. Old RegExps can still be reevaluated if they were not evaluated before
 *     stopSatProcess was called, but once evaluated there would be no need to evaluate them again.
 * (5) RegExp created in different startSatProcess()-stopSatProcess periods can be mixed together with the understanding
 *     that RegExps are always used as constant and are never mutated (its hopefully enforced by the compiler ---
 *     keep the constructors private)
 * (6) To achieve the above, we use the static value currentSatProcess (an unsigned int) and non-static
 *     value satProcess (also an unsigned int)
 * (7) One can also change the semiring being used by calling startSatProcess() but then it should not be
 *     mixed with old RegExps.
 */

namespace wali {

    namespace graph {

        enum reg_exp_type {Constant, Updatable, Extend, Combine, Star};

        class RegExp;
        typedef ref_ptr<RegExp> reg_exp_t;
        typedef long unsigned int node_no_t;

        struct cmp_reg_exp {
            bool operator() (reg_exp_t r1, reg_exp_t r2) const {
                return (r1.get_ptr() < r2.get_ptr());
            }
        };

        struct reg_exp_key_t {
            reg_exp_type type;
            reg_exp_t c1;
            reg_exp_t c2;
            reg_exp_key_t() {
                c1 = 0;
                c2 = 0;
                type = Constant;
            }
            reg_exp_key_t(reg_exp_type t, reg_exp_t _c1, reg_exp_t _c2 = 0) {
                type = t;
                c1 = _c1;
                c2 = _c2;
            }
            bool operator() (reg_exp_key_t r1, reg_exp_key_t r2) const {
                return (r1.type == r2.type && r1.c1.get_ptr() == r2.c1.get_ptr() &&
                        r1.c2.get_ptr() == r2.c2.get_ptr());
            }
        };

        struct hash_reg_exp_key {
            size_t operator() (const reg_exp_key_t &k) const {
                return ((size_t)k.type ^ (size_t)k.c1.get_ptr() ^ (size_t)k.c2.get_ptr());
            }
        };

        struct hash_sem_elem {
            size_t operator() (const sem_elem_t &se) const {
                return (size_t)se.get_ptr();
            }
        };

        struct sem_elem_equal {
            bool operator() (sem_elem_t s1, sem_elem_t s2) const {
                return (s1.get_ptr() == s2.get_ptr());
                //return (s1->equal(s2));
            }
        };

        struct sem_elem_less {
            bool operator() (sem_elem_t s1, sem_elem_t s2) const {
                return (s1.get_ptr() < s2.get_ptr());
                //return (s1->equal(s2));
            }
        };

        typedef pair<long int, long int> out_node_height_t;
        typedef map<long int, out_node_height_t> out_node_stat_t;

        struct RegExpStats {
            int nstar;
            int nextend;
            int ncombine;
            int hashmap_hits;
            int hashmap_misses;
            long int height;
            long int out_nodes;
            long int lnd;
            RegExpStats() {
                nstar = ncombine = nextend = 0;
                hashmap_hits = 0;
                hashmap_misses = 0;
                height = lnd = out_nodes = 0;
            }
        };

        ostream &operator << (ostream &out, const RegExpStats &s);

        typedef map<reg_exp_t, reg_exp_t, cmp_reg_exp> reg_exp_cache_t;

        typedef map<unsigned int, sem_elem_t> delta_map_t;


        typedef wali::HashMap<reg_exp_key_t, reg_exp_t, hash_reg_exp_key, reg_exp_key_t> reg_exp_hash_t;
        typedef wali::HashMap<sem_elem_t, reg_exp_t, hash_sem_elem, sem_elem_equal> const_reg_exp_hash_t;

        class RegExpSatProcess {
          public:
          unsigned int update_count;
          RegExpSatProcess() : update_count(1) { }
        };


        class RegExp {
            public:
                unsigned int count; // for reference counting
            private:
                reg_exp_type type;
                sem_elem_t value;
#ifdef DWPDS
                delta_map_t delta;
#endif
                node_no_t updatable_node_no;
                list<reg_exp_t> children;
                unsigned int last_change;
                unsigned int last_seen;

                set<long int> outnodes; // set of out-nodes contained in this RegExp
                out_node_stat_t outnode_height;
                int samechange,differentchange,lastchange;

                map<sem_elem_t, sem_elem_t, sem_elem_less> eval_map; // value under certain contexts
                static bool saturation_complete;
                static bool executing_poststar;
                static bool initialized;
                static bool top_down_eval;
                int nevals; // for gathering stats: no of times eval was called on this regexp

                static vector<reg_exp_t> updatable_nodes;

                static vector<RegExpSatProcess> satProcesses;
                static long unsigned int currentSatProcess;
                long unsigned int satProcess;

                static bool extend_backwards;
                static reg_exp_hash_t reg_exp_hash;
                static const_reg_exp_hash_t const_reg_exp_hash;
                static RegExpStats stats;
                static reg_exp_t reg_exp_zero, reg_exp_one;

                // For debugging
                bool uptodate;

                RegExp(node_no_t nno, sem_elem_t se) {
                    type = Updatable;
                    value = se;
                    updatable_node_no = nno;
                    count = 0;
                    last_change = 1;
                    last_seen = 1;
                    uptodate = false;
                    nevals = 0;
                    samechange = differentchange = 0;
                    lastchange=-1;
                    satProcess = currentSatProcess;
                }
                RegExp(reg_exp_type t, const reg_exp_t r1, const reg_exp_t r2 = 0) {
                    count = 0;
                    nevals = 0;
                    if(t == Extend || t == Combine) {
                        type = t;
                        assert(r1.get_ptr() != 0 && r2.get_ptr() != 0);
                        value = r1->value->zero();
                        children.push_back(r1);
                        children.push_back(r2);
                    } else if(t == Star) {
                        type = Star;
                        assert(r1.get_ptr() != 0);
                        value = r1->value->zero();
                        children.push_back(r1);
                    } else {
                        assert(0);
                    }
                    last_change = 0;
                    last_seen = 0;
                    uptodate = false;
                    samechange = differentchange = 0;
                    lastchange=-1;
                    satProcess = currentSatProcess;
                }
                RegExp(sem_elem_t se) {
                    type = Constant;
                    value = se;
                    updatable_node_no = 0; // default value
                    count = 0;
                    last_change = 1;
                    last_seen = 0;
                    uptodate = false;
                    nevals = 0;
                    samechange = differentchange = 0;
                    lastchange=-1;
                    satProcess = currentSatProcess;
                }

            public:

                static void extendDirectionBackwards(bool b) {
                    extend_backwards = b;
                }

                static void saturationComplete() {
                    saturation_complete = true;
                }

                static void executingPoststar(bool f) {
                    executing_poststar = f;
                }

                static void topDownEval(bool f) {
                    top_down_eval = f;
                }

                ostream &print(ostream &out);
                static reg_exp_t star(reg_exp_t r);
                static reg_exp_t extend(reg_exp_t r1, reg_exp_t r2);
                static reg_exp_t combine(reg_exp_t r1, reg_exp_t r2);
                static reg_exp_t combine(list<reg_exp_t> &ls);
                static reg_exp_t constant(sem_elem_t se);
                static void startSatProcess(const sem_elem_t se);
                static void stopSatProcess();

                static reg_exp_t updatable(node_no_t nno, sem_elem_t se);
                static reg_exp_t compress(reg_exp_t r, reg_exp_cache_t &cache);
                static reg_exp_t minimize_height(reg_exp_t r, reg_exp_cache_t &cache);
                static size_t getNextUpdatableNumber() {
                    return updatable_nodes.size();
                }
                static void update(node_no_t nno, sem_elem_t se);
                int updatableNumber();
#ifdef DWPDS
                sem_elem_t get_delta(unsigned int ls);
#endif
                static ostream &print_stats(ostream & out) {
                    out << stats;
                    return out;
                }
                static int out_node_height(set<RegExp *> reg_equations);
                sem_elem_t get_weight();

                int get_nevals() {
                    return nevals;
                }

                static RegExpStats get_stats() {
                    return stats;
                }

                bool isZero() {
                    return (type == Constant && value->equal(value->zero()));
                }

                bool isOne() {
                    return (type == Constant && value->equal(value->one()));
                }

                bool isCyclic();
                sem_elem_t reevaluate();

            private:

                void evaluate();
                void evaluate_iteratively();
                sem_elem_t evaluate(sem_elem_t w);
                sem_elem_t evaluateRev(sem_elem_t w);
                sem_elem_t reevaluateIter();
                static reg_exp_t compressExtend(reg_exp_t r1, reg_exp_t r2);
                static reg_exp_t compressCombine(reg_exp_t r1, reg_exp_t r2);

                bool dfs(set<RegExp *> &, set<RegExp *> &);
                int calculate_height(set<RegExp *> &visited, out_node_stat_t &stat_map);
        };

    } // namespace graph
} // namespace wali

#endif // wali_graph__REGEXP_H_
