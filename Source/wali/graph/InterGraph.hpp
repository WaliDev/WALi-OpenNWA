#ifndef wali_graph_INTER_GRAPH_GUARD
#define wali_graph_INTER_GRAPH_GUARD 1

#include "wali/Countable.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/MergeFn.hpp"

#include "wali/graph/GraphCommon.hpp"

#include <list>
#include <vector>
#include <iostream>
#include <map>
#include <set>

namespace wali {

    namespace graph {

        class IntraGraph;
        class InterGraph;

        class UnionFind {
            friend class InterGraph;
            int *arr;
            int n;
            public:
            UnionFind(int len);
            ~UnionFind();
            void reset();
            int find(int a);
            void takeUnion(int a, int b);
        };


        struct InterGraphStats {
            int nnodes;
            int nedges;
            int nhyperedges;
            int ncombine;
            int nextend;
            int nstar;
            int ngraphs;
            int nupdatable;
            int ncutset;
            int nget_weight;
            int niter;
            int ncomponents;
            int ndom_sequence;
            int ndom_componentsize;
            int ndom_components;
            int ndom_componentcutset;

            InterGraphStats() {
                nnodes = nedges = nhyperedges = 0;
                ncombine = nextend = nstar = 0;
                ngraphs = nupdatable = ncutset = 0;
                niter = 0;
                ncomponents = 0;
                ndom_sequence = 0;
                ndom_componentsize = 0;
                ndom_components = 0;
                ndom_componentcutset = 0;
                nget_weight = 0;
                //WIN(intra_saturation = inter_saturation = setup_time = 0);
                //WIN(t1 = t2 = t3 = 0);
            }
        };

       /* For call site to (mid-state) return site transition */
       class ETransHandler {

         friend class InterGraph;
         friend class SummaryGraph;
         
       private:
         typedef std::pair<int, sem_elem_t> Dependency; 
         typedef std::map<int, Dependency> EdgeMap;
         
         ETransHandler() {}
         bool exists(int ret);
         void addEdge(int call, int ret, sem_elem_t wtCallRule);
         sem_elem_t get_dependency(int ret, int &call);

       private:
         EdgeMap edgeMap;

       };

        class HyperEdge {
            public:
                int src1, src2, tgt;
                sem_elem_t weight;
                wali::merge_fn_t mf;
                HyperEdge(int s1, int s2, int t, sem_elem_t se) : src1(s1), src2(s2), tgt(t), weight(se), mf(0) {}
                HyperEdge(int s1, int s2, int t, wali::merge_fn_t f) : src1(s1), src2(s2), tgt(t), weight(0), mf(f) {}
                HyperEdge(const HyperEdge &h) : src1(h.src1), src2(h.src2), tgt(h.tgt), weight(h.weight), mf(h.mf) {}
        };

        class GraphEdge {
            public:
                int src, tgt;
                sem_elem_t weight;
                GraphEdge(int s, int t, sem_elem_t w) : src(s), tgt(t), weight(w) {}
                GraphEdge(const GraphEdge &e) {
                    src = e.src;
                    tgt = e.tgt;
                    weight = e.weight;
                }
        };

        enum inter_node_t {InterNone = 0, InterSource = 1, InterOutNode = 2, InterSourceOutNode = 3};

        class GraphNode {
            public:
                Transition trans;
                int intra_nodeno;
                inter_node_t type;
                sem_elem_t weight;
                IntraGraph *gr;
                std::list<int> outgoing;
                std::list<int> incoming;
                std::list<int> out_hyper_edges;
                bool visited;
                GraphNode(Transition tr, inter_node_t ty = InterNone) : trans(tr), intra_nodeno(-1), type(ty), weight(0), gr(NULL) {}
                GraphNode(const GraphNode &g) : trans(g.trans), intra_nodeno(g.intra_nodeno), type(g.type), weight(g.weight), 
                gr(g.gr), outgoing(g.outgoing), incoming(g.incoming),
                out_hyper_edges(g.out_hyper_edges), visited(g.visited) {}
        };

      class InterGraph : public Countable {
            public:
                typedef std::ostream & (*PRINT_OP)(std::ostream &, int);

            private:
                friend class SummaryGraph;

                typedef std::map<Transition, int, TransitionCmp> TransMap;
                typedef bool (*WT_CHECK)(SemElem *);
                typedef SemElem *(*WT_CORRECT)(SemElem *);
                typedef std::pair<int,int> tup;
                typedef std::pair<int, int> call_edge_t;

                std::vector<GraphNode> nodes;
                std::vector<GraphEdge> intra_edges;
                std::vector<HyperEdge> inter_edges;
                std::vector<call_edge_t> call_edges;
                int max_scc_computed;

                UnionFind *intra_graph_uf;
                std::list<IntraGraph *> gr_list;

                ETransHandler eHandler;
                //map<int,IntraGraph*> intra_graph_map;

                TransMap node_number;
                sem_elem_t sem; 
                bool running_ewpds;
                bool running_prestar;
                InterGraphStats stats;

                static std::ostream &defaultPrintOp(std::ostream &out, int a) {
                    out << a;
                    return out;
                }

            public:
                InterGraph(wali::sem_elem_t s, bool e, bool pre) {
                    sem = s;
                    intra_graph_uf = NULL;
                    running_ewpds = e;
                    running_prestar = pre;
                    max_scc_computed = 0;
                    count = 0;
                }
                ~InterGraph();
                void addEdge(Transition src, Transition tgt, wali::sem_elem_t se);
                void addEdge(Transition src1, Transition src2, Transition tgt, wali::sem_elem_t se);
                void addEdge(Transition src1, Transition src2, Transition tgt, wali::merge_fn_t mf);
                void addCallRetEdge(Transition src, Transition tgt, wali::sem_elem_t se);

                void addCallEdge(Transition src1, Transition src2);

                void setSource(Transition t, wali::sem_elem_t se);
                void setESource(Transition t, wali::sem_elem_t wtAtCall, wali::sem_elem_t wtAfterCall);

                void setupInterSolution(std::list<Transition> *wt_required = NULL);

                sem_elem_t get_weight(Transition t);
                sem_elem_t get_call_weight(Transition t);

                void update_all_weights();

                bool exists(int state, int stack, WT_CHECK op);

                std::ostream &print(std::ostream &out, PRINT_OP pop = defaultPrintOp);

                std::ostream &print_stats(std::ostream &out); 

                bool path_summary(int state, int stack, int accept, WT_CORRECT correct, WT_CHECK op);

                bool exists(Transition &t);

                int nGraphs() {
                    return (int)gr_list.size();
                }
            private:
                int nodeno(Transition &t);

                int intra_edgeno(Transition &src, Transition &tgt);

                int inter_edgeno(Transition &src1, Transition &src2, Transition &tgt);

                void dfsIntraForward(IntraGraph *gr, 
                        std::list<IntraGraph *> &finished, 
                        std::map<IntraGraph *, 
                        std::list<IntraGraph *> > &rev_edges);

                void bfsIntra(IntraGraph *start, unsigned int scc_number);

                unsigned SCC(std::list<IntraGraph *> &grlist, std::list<IntraGraph *> &grsorted);

                void saturate(std::multiset<tup> &worklist, unsigned scc_n);

                void setup_worklist(std::list<IntraGraph *> &gr_sorted, 
                        std::list<IntraGraph *>::iterator &gr_it, 
                        unsigned int scc_n,
                        std::multiset<tup> &worklist);

                void resetSCCedges(IntraGraph *gr, unsigned int scc_number);
        };

        typedef ref_ptr<InterGraph> InterGraphPtr;
    } // namespace graph

} // namespace wali


#endif // wali_graph_INTER_GRAPH_GUARD 

