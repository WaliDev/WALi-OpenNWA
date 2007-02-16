#ifndef wali_graph_SUMMARY_GRAPH_H_
#define wali_graph_SUMMARY_GRAPH_H_

#include "wali/HashMap.hpp"

#include "wali/wfa/WFA.hpp"

#include "wali/graph/IntraGraph.hpp"
#include "wali/graph/InterGraph.hpp"

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

                InterGraph *igr;
                //wpds_key_t init_state;
                Key init_state;
                StackGraphMap stack_graph_map;
                std::list<IntraGraph *> changed_graphs;
                std::set<IntraGraph *> updated_graphs;

                InterGraph::PRINT_OP pkey;
                SummaryGraphStats stats;

                std::vector<SummaryGraphNode> nodes;
                TransMap trans_map;

            public:
                SummaryGraph(InterGraph *gr, wali::Key ss, InterGraph::PRINT_OP pop = InterGraph::defaultPrintOp);
                ~SummaryGraph();
                void addUpdate(Transition &t, SemElem *se);
                void getUpdatedTransitions(std::list<WTransition> &ls);
                void getMiddleTransitions(std::list<WTransition> &ls);
                void summaryPoststar(wali::wfa::WFA& ca_in, wali::wfa::WFA& ca_out);

                ostream &printStats(ostream &out);

            private:
                int stk2nodeno(int stk);
                int trans2nodeno(Transition &tr);
                int getIntraNodeNumber(Transition &tr);
        };

    } // namespace graph

} // namespace wali

#endif // wali_graph_SUMMARY_GRAPH_H_
