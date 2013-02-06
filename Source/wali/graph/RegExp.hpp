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
            void reset() {
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

        class RegExpDag;

        class RegExp {
            public:
              friend class RegExpDag; 
            public:
                unsigned int count; // for reference counting
            private:
                /**
                 * @author Prathmesh Prabhu
                 * Ideally, I would like to not store dag in RegExp.
                 * but RegExp accesses static variables formerly in RegExp, now in RegExpDag very often. 
                 * We will indirect all such accesses through dag.
                 **/
                RegExpDag * dag;
                reg_exp_type type;
                sem_elem_t value;
#ifdef DWPDS
                delta_map_t delta;
#endif
                node_no_t updatable_node_no;
                list<reg_exp_t> children;
#if defined(PUSH_EVAL)
                /*
                   In push based evaluation of the RegExp graph, an updatable regular expression
                   sets the dirty bit for all its ancestors. During a real evaluation, only the
                   dirty bit is checked to determine whether reevaluation is needed. All
                   non-terminal RegExp nodes are created with the dirty bit set.
                */
                bool dirty;
                /* When set, points to the ancestors in the RegExp dag. */
                list<reg_exp_t> parents;
#endif
                unsigned int last_change;
                unsigned int last_seen;

                set<long int> outnodes; // set of out-nodes contained in this RegExp
                out_node_stat_t outnode_height;
                int samechange,differentchange,lastchange;

                map<sem_elem_t, sem_elem_t, sem_elem_less> eval_map; // value under certain contexts
                int nevals; // for gathering stats: no of times eval was called on this regexp

                long unsigned int satProcess;

                // For debugging
                bool uptodate;
                std::vector<unsigned int> updates;
                std::vector<unsigned int> evaluations;

                RegExp(long unsigned int currentSatProcess, RegExpDag * d, node_no_t nno, sem_elem_t se) {
                    type = Updatable;
                    value = se;
                    updatable_node_no = nno;
                    count = 0;
#if defined(PUSH_EVAL)
                    dirty = true;
#endif
                    last_change = 1;
                    last_seen = 1;
                    uptodate = false;
                    nevals = 0;
                    samechange = differentchange = 0;
                    lastchange=-1;
                    satProcess = currentSatProcess;
                    dag = d;
                }
                RegExp(long unsigned int currentSatProcess, RegExpDag * d, reg_exp_type t, reg_exp_t r1, reg_exp_t r2 = 0) {
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
#if defined(PUSH_EVAL)
                    dirty = true;
                    r1->parents.push_back(static_cast<RegExp*>(this));
                    if(!(r2 == NULL))
                      r2->parents.push_back(static_cast<RegExp*>(this));
#endif
                    last_change = 0;
                    last_seen = 0;
                    uptodate = false;
                    samechange = differentchange = 0;
                    lastchange=-1;
                    satProcess = currentSatProcess;
                    dag = d;
                }
                RegExp(long unsigned int currentSatProcess, RegExpDag * d, sem_elem_t se) {
                    type = Constant;
                    value = se;
                    updatable_node_no = 0; // default value
                    count = 0;
#if defined(PUSH_EVAL)
                    dirty = false; //Will always remains o                    
#endif
                    last_change = 1;
                    last_seen = 0;
                    uptodate = false;
                    nevals = 0;
                    samechange = differentchange = 0;
                    lastchange=-1;
                    satProcess = currentSatProcess;
                    dag = d;
                }

            public:


                ostream &print(ostream &out);
                long toDot(ostream &out, std::set<long>& seen, bool printUpdates = false, bool isRoot = false);
                int updatableNumber();
#ifdef DWPDS
                sem_elem_t get_delta(unsigned int ls);
#endif
                sem_elem_t get_weight();

                int get_nevals() {
                    return nevals;
                }

                bool isZero() {
                    return (type == Constant && value->equal(value->zero()));
                }

                bool isOne() {
                    return (type == Constant && value->equal(value->one()));
                }

                bool isCyclic();
                sem_elem_t reevaluate();

                /**
                 * @author Prathmesh Prabhu
                 * Obtain the hash map that stores the roots in the current
                 * sat process
                 **/

            private:

#if defined(PUSH_EVAL)
                void setDirty();
#endif
                void evaluate();
                void evaluate_iteratively();
                sem_elem_t evaluate(sem_elem_t w);
                sem_elem_t evaluateRev(sem_elem_t w);
                sem_elem_t reevaluateIter();

                bool dfs(set<RegExp *> &, set<RegExp *> &);
                int calculate_height(set<RegExp *> &visited, out_node_stat_t &stat_map);
        };

        /**
         * @class RegExpDag
         * This is a context to build a RegExp dag
         * Different RegExpDag objects should not share RegExp nodes, I don't know what will happen.
         * An object of RegExpDag
         * -- Provides functions to actually create RegExp nodes. (Its own constructor is private)
         * -- Provides a bunch of functions to collect information
         * -- Stores and manipulates dag level information for RegExp
         * -- encapsulates what was earlier a bunch of static data members of RegExp.
         *
         * @author Prathmesh Prabhu
         **/        
        class RegExpDag
        {
          /**
           * This class is not properly decoupled from RegExp.
           * All it has succeeded in doing is getting rid of static variables.
           **/
          friend class RegExp;
          public:
            RegExpDag();
            ~RegExpDag();
          public:
            // ///////////////////////////////////////////////////////
            // RegExp dag diagnositcs.
            // Whenever this information is available, it is available across
            // calls to startSatProcess / stopSatProcess.
            
            // Although we try our best, the set of roots accross sat processes
            // may not be correct.  Some roots might have become non-roots. It
            // is a good idea to sanitize the set before collecting diagnostic
            // information. This can be called any number of times.
            void sanitizeRootsAcrossSatProcesses();

            /**
             * Count the total number of active nodes in the RegExp graph
             **/
            long countTotalNodes();
            long countTotalCombines();
            long countTotalExtends();
            long countTotalStars();
            long countTotalLeaves();            
            long getHeight();
            long countSpline();
            long countFrontier();

            /**
             * Counts the total cumulative nodes except those reachable from
             * the list of nodes given as exclude
             **/
            long countExcept(std::vector<reg_exp_t>& exceptions);

            /**
             * Print information about the structure of the RegExp dag:
             *   Spline: is a path from a root to an updatable node.
             *   Frontier: is the set of children of nodes on a Spline that are
             *   not themselves on a spline.
             *
             * - #spline
             * - #frontier
             * - #totalNodes
             * - fraction spline/totalNodes
             * - fraction frontier/totalNodes
             * - fraction (totalNodes - (spline U frontier U leaves)) / totalNodes
             *
             * Additionally print:
             * - Height of the dag.
             * - fraction (height) / log_2(totalNodes)  
             **/
             void printStructureInformation();

             // Add a reg_exp_t to the set of regular expressions labelling some
             // IntraGraphEdge. Used only for diagnostic purposes.
             void markAsEdgeLabel(reg_exp_t r);
               
               
          private:
            /**
              * Helper functions for diagnostics.
              **/
            long countFrontier(reg_exp_t const e);
            void markSpline();
            bool markSpline(reg_exp_t const e);
            long getHeight(reg_exp_t const e);
            long countTotalLeaves(reg_exp_t const e);
            long countTotalCombines(reg_exp_t const e);
            long countTotalExtends(reg_exp_t const e);
            long countTotalStars(reg_exp_t const e);
            void excludeFromCountReachable(reg_exp_t const e);
            long countTotalNodes(reg_exp_t const e);
            void removeDagFromRoots(reg_exp_t const e);

            /**
             * Functions related to those copied from RegExp (used to be static).
             **/
            reg_exp_t _minimize_height(reg_exp_t r, reg_exp_cache_t &cache);
            reg_exp_t compressExtend(reg_exp_t r1, reg_exp_t r2);
            reg_exp_t compressCombine(reg_exp_t r1, reg_exp_t r2);
            reg_exp_t _compress(reg_exp_t r, reg_exp_cache_t &cache);

            /**
             * Functions copied from RegExp (used to be static)
             **/
          public:
            reg_exp_t star(reg_exp_t r);
            reg_exp_t extend(reg_exp_t r1, reg_exp_t r2);
            reg_exp_t combine(std::list<reg_exp_t>& rlist);
            reg_exp_t combine(reg_exp_t r1, reg_exp_t r2);
            reg_exp_t constant(sem_elem_t se);
            void startSatProcess(const sem_elem_t se);
            void stopSatProcess();

            reg_exp_t updatable(node_no_t nno, sem_elem_t se);
            reg_exp_t compress(reg_exp_t r, reg_exp_cache_t &cache);
            reg_exp_t minimize_height(reg_exp_t r, reg_exp_cache_t &cache);
            size_t getNextUpdatableNumber() {
              return updatable_nodes.size();
            }
            void update(node_no_t nno, sem_elem_t se);
            void update(std::vector<node_no_t> nnos, std::vector<sem_elem_t> ses);

            int out_node_height(set<RegExp *> reg_equations);
            void evaluateRoots();
            const reg_exp_hash_t& getRoots();


            ostream &print_stats(ostream & out) {
              out << stats;
              return out;
            }

            void extendDirectionBackwards(bool b) {
              extend_backwards = b;
            }

            void saturationComplete() {
              saturation_complete = true;
            }

            void executingPoststar(bool f) {
              executing_poststar = f;
            }

            void topDownEval(bool f) {
              top_down_eval = f;
            }

            RegExpStats get_stats() {
              return stats;
            }

          private:
            reg_exp_hash_t visited;
            reg_exp_hash_t spline;
            wali::HashMap<reg_exp_key_t, long, hash_reg_exp_key, reg_exp_key_t> height;

            /**
             * Data copied from RegExp (used to be static)
             **/
          private:
            bool saturation_complete;
            bool executing_poststar;
            bool initialized;
            bool top_down_eval;

            vector<reg_exp_t> updatable_nodes;

            vector<RegExpSatProcess> satProcesses;
            long unsigned int currentSatProcess;

            bool extend_backwards;
            reg_exp_hash_t reg_exp_hash;
            const_reg_exp_hash_t const_reg_exp_hash;
            // Remember what Regular Experessions are the root of the tree
            // in the current Saturation phase. This is reset between saturation
            // phases.
            reg_exp_hash_t rootsInSatProcess;
            // Also remember the set of all roots created
            // This is used for diagnostic purposes
            reg_exp_hash_t rootsAccrossSatProcesses;
            // Set of reg_exp nodes that label some IntraGraphEdge
            reg_exp_hash_t edgeLabels;

            RegExpStats stats;
            reg_exp_t reg_exp_zero, reg_exp_one;
        };

    } // namespace graph
} // namespace wali

#endif // wali_graph__REGEXP_H_
