#ifndef wali_graph_SUMMARY_GRAPH_H_
#define wali_graph_SUMMARY_GRAPH_H_

#include "wali/HashMap.hpp"

#include "wali/wfa/WFA.hpp"

#include "wali/graph/IntraGraph.hpp"
#include "wali/graph/InterGraph.hpp"

#include "wali/wpds/ewpds/ERule.hpp"

namespace wali {

    namespace graph {

        class SummaryGraphStats {
            public:
                clock_t t1,t2,t3,t4,t5; // for debugging and profiling
                SummaryGraphStats() {
                    t1 = t2 = t3 = t4 = t5 = 0;
                }
        };

        class SummaryGraphNode {
            public:
                Transition trans;
                reg_exp_t regexp;
                int uno; // 0 if not updatable
                sem_elem_t weight;

                SummaryGraphNode() : trans(0,0,0), regexp(NULL), uno(-1),
                                     weight(NULL) { }

                SummaryGraphNode(const SummaryGraphNode &n) : trans(n.trans), regexp(n.regexp),
                                                              uno(n.uno), weight(n.weight) { }

                SummaryGraphNode(Transition &_trans, reg_exp_t &_regexp, int _uno, sem_elem_t _weight) : trans(_trans),
                                                                                                         regexp(_regexp), uno(_uno), weight(_weight) { } 

                SummaryGraphNode(Transition &_trans) : trans(_trans),
                                                       regexp(NULL), uno(-1), weight(NULL) { } 
        };

        class SummaryGraph {
            private:
                typedef wali::HashMap<int, int> StackGraphMap;
                typedef std::map<Transition, int, TransitionCmp> TransMap;

                InterGraphPtr post_igr;
                Key init_state;

                // The set of procedure entry nodes
                set<Key> procEntries;

                // IntraGraph to Key for procedure entry of the procedure
                // represented by the InraGraph
                std::map<IntraGraph *, Key> procEntryMap;
      
                // Map to cache [k -> MOP(k,\epsilon)]
                std::map<Key, sem_elem_t> popWeightMap;

                // Map [Transition -> ERule used to create it]
                std::map<Transition, wpds::ewpds::erule_t, TransitionCmp> eruleMap;

                // Nodes that belong to multiple procedures
                std::set<Key> multiple_proc_nodes;

                StackGraphMap stack_graph_map;
                std::list<IntraGraph *> changed_graphs;
                std::set<IntraGraph *> updated_graphs;

                InterGraph::PRINT_OP pkey;
                SummaryGraphStats stats;

                std::vector<SummaryGraphNode> nodes;
                TransMap trans_map;

            public:
                SummaryGraph(InterGraphPtr gr, 
                             wali::Key ss, 
                             set<Key> &pe, 
                             wfa::WFA &Agrow, 
                             InterGraph::PRINT_OP pop = InterGraph::defaultPrintOp);

                ~SummaryGraph();
                void preAddUpdate(Transition &t, sem_elem_t se);
                void getUpdatedTransitions(std::list<WTransition> &ls);
                void preGetUpdatedTransitions(std::list<WTransition> &ls);
                void getMiddleTransitions(std::list<WTransition> &ls);
                void summaryPoststar(wali::wfa::WFA const & ca_in, wali::wfa::WFA& ca_out);

                sem_elem_t pushWeight(Key k);
                sem_elem_t popWeight(Key k);
                Key getEntry(Key k);
                bool reachable(Key stk);
                bool multiple_proc(Key stk);

                ostream &printStats(ostream &out);

            private:
                int stk2nodeno(int stk);
                int trans2nodeno(Transition &tr);
                int getIntraNodeNumber(Transition &tr);
                void calculatePopWeights();

                void addIntraTrans(Transition &tr, sem_elem_t wt, wfa::WFA &ca_out);
                void addMiddleTrans(Transition &tr, sem_elem_t wt, wfa::WFA &ca_out);
                Key changeStateGeneration(Key st, int gen);

        };

    } // namespace graph

} // namespace wali

#endif // wali_graph_SUMMARY_GRAPH_H_
