#ifndef wali_graph__INTRA_GRAPH_H
#define wali_graph__INTRA_GRAPH_H

#include "wali/SemElem.hpp"

#include "wali/graph/RegExp.hpp"
#include "wali/graph/GraphCommon.hpp"

#include <map>

namespace wali {

    namespace graph {

        enum node_type {None, SuperSource, Source, OutNode};

        struct IntraGraphStats {
            int nstar;
            int ncombine;
            int nextend;
            int nnodes;
            int nedges;
            int nupdatable;
            int ncutset;
            int nget_weight;
            int ndom_sequence;
            int ndom_componentsize;
            int ndom_components;
            int ndom_componentcutset;
            clock_t t1,t2,t3,t4,t5; // Timers for debugging (used on a need-to-use basis)

            IntraGraphStats() {
                nstar = nextend = ncombine = 0;
                nnodes = 0;
                nupdatable = 0;
                ncutset = 0;
                nedges = 0;
                ndom_sequence = ndom_componentsize = ndom_components = 0;
                ndom_componentcutset = 0;
                nget_weight = 0;
                t1 = t2 = t3 = t4 = t5 = 0;
            }
        };
        ostream &operator << (ostream &out, const IntraGraphStats &s);

        class IntraGraphEdge {
            public:
                int src, tgt;
                sem_elem_t weight;
                bool updatable;
                int updatable_no;
                reg_exp_t regexp;
                IntraGraphEdge(int s, int t, sem_elem_t w, bool u, int uno = 0) : src(s), tgt(t), weight(w), updatable(u), updatable_no(uno) {
                    if(updatable) 
                        regexp = RegExp::updatable(uno, weight);
                    else
                        regexp = RegExp::constant(weight);
                }
                void set(int s, int t, sem_elem_t w, bool u, int uno = 0) {
                    src = s;
                    tgt = t;
                    weight = w;
                    updatable = u;
                    updatable_no = uno;
                    if(updatable) 
                        regexp = RegExp::updatable(uno, weight);
                    else
                        regexp = RegExp::constant(weight);
                }
                IntraGraphEdge() : src(-1), tgt(-1) { } // creates a fake edge
                IntraGraphEdge(const IntraGraphEdge &e) {
                    src = e.src;
                    tgt = e.tgt;
                    weight = e.weight;
                    updatable = e.updatable;
                    updatable_no = e.updatable_no;
                    regexp = e.regexp;
                }
        };

        struct PathSequence {
            reg_exp_t regexp;
            int src, tgt;
            PathSequence(reg_exp_t r, int s, int t) : regexp(r), src(s), tgt(t) {}
        };

        struct EvaluatedPathSequence {
            sem_elem_t value;
            int src, tgt;
            EvaluatedPathSequence(sem_elem_t v, int s, int t) : value(v), src(s), tgt(t) {}
        };

        class IntraGraphNode {
            public:
                Transition trans;
                int node_no; // Node number in the IntraGraph node array (-1 if not in the array)
                node_type type;
                list<int> outgoing;
                list<int> incoming;
                reg_exp_t regexp;

                bool iscutset;
                int visited;
                int scc_number;

                IntraGraphNode() : trans(0,0,0), node_no(-1), type(None) { } // creates a fake node
                IntraGraphNode(int nno, node_type ty = None) : trans(0,0,0), node_no(nno), type(ty), iscutset(false), visited(0), scc_number(0) {}
                IntraGraphNode(const IntraGraphNode &n) : trans(n.trans), node_no(n.node_no), type(n.type), outgoing(n.outgoing), incoming(n.incoming), 
                regexp(n.regexp), iscutset(n.iscutset), visited(n.visited), scc_number(n.scc_number) {
                }
                void set(int nno, node_type ty = None) {
                    trans = Transition(0,0,0);
                    node_no = nno;
                    type = ty;
                    iscutset = false;
                    visited = 0;
                    scc_number = 0;
                }
        };

        class InterGraph;

        //typedef wpds::HashMap<Transition, int, TransitionHash, TransitionEq> transition_map_t;
        typedef map<Transition, int, TransitionCmp> transition_map_t;


        class update_t {
        public:
          int node;
          sem_elem_t wt;
          int uno;
          update_t(int _node, int _uno) : node(_node), wt(NULL), uno(_uno) { }
          update_t(int _node, sem_elem_t _wt) : node(_node), wt(_wt), uno(-1) { }
          update_t(const update_t &it) : node(it.node), wt(it.wt), uno(it.uno) { }
        };

        class IntraGraph {
            friend class InterGraph;
            friend class SummaryGraph;
            private:
            typedef ostream & (*PRINT_OP)(ostream &, int);

            vector<IntraGraphNode> nodes;
            vector<IntraGraphEdge> edges;
            int nnodes; // Keep these counts because the vectors might be bigger than whats required
            int nedges;
            list<int> *out_nodes_intra; // out nodes numbered as in this IntraGraph
            list<int> *out_nodes_inter; // out nodes numbered as in the InterGraph
            //transition_map_t node_number;
            bool running_prestar;

            set<IntraGraph *> calls;
            vector<PathSequence> path_sequence;
            vector<EvaluatedPathSequence> evaluated_path_sequence;
            list<update_t> updates;
            vector<sem_elem_t> node_weight; // To avoid memory allocation on the critical path
            vector<sem_elem_t> node_pop_weight; // To avoid memory allocation on the critical path
            sem_elem_t **apsp;

            list<int> topsort_list;
            vector<int> cutset_list;

            bool visited;
            unsigned int scc_number;
            unsigned int bfs_number;

            vector<int> updatable_edges;
            IntraGraphStats stats;
            static sem_elem_t se;
#ifdef STATIC_MEMORY
            static int * intraGraphBuffer;
            static set<int> *childrenBuffer;
            static reg_exp_t *regBuffer;
            static int intraGraphBufferSize;
#endif
            static ostream &defaultPrintOp(ostream &out, int a) {
                out << a;
                return out;
            }
            public:
            IntraGraph(bool pre, sem_elem_t _se) :nodes(50), edges(50) {
                visited = false;
                scc_number = 0;
                bfs_number = 0;
                nodes[0].set(0, SuperSource);
                nnodes = 1;
                nedges = 0;
                out_nodes_intra = new list<int>;
                out_nodes_inter = new list<int>;
                running_prestar = pre;
                se = _se;
                apsp = NULL;
            }

            ~IntraGraph() {
                delete out_nodes_intra;
                delete out_nodes_inter;
                if(apsp != NULL) {
                    int i;
                    for(i=0;i<nnodes;i++)
                        delete [] apsp[i];
                    delete [] apsp;
                    apsp = NULL;
                }
            }
            list<int> *getOutTransitions() {
                return out_nodes_inter;
            }
            IntraGraphStats get_stats();
            int getSize() {
                return nnodes;
            }
#ifdef STATIC_MEMORY
            static void addStaticBuffer(int s) {
                intraGraphBuffer = new int[5*s];
                childrenBuffer = new set<int>[s];
                regBuffer = new reg_exp_t[s];
                intraGraphBufferSize = s;
            }
            static void clearStaticBuffer() {
                delete [] intraGraphBuffer;
                delete [] childrenBuffer;
                delete [] regBuffer;

                intraGraphBuffer = 0;
                childrenBuffer = 0;
                regBuffer = 0;

                intraGraphBufferSize = 0;
            }
#endif
            int makeNode(Transition t) {
                create_node(t, nnodes);
                return (nnodes-1); // nnodes gets incremented by create_node
            }
            int makeNode() {
                Transition t(0,0,0);
                return makeNode(t);
            }
            void addEdge(int src, int tgt, sem_elem_t se, bool updatable = false);
            void setSource(int t, sem_elem_t se);
            void setOutNode(int t, int inter_t);
            void addCallEdge(IntraGraph *next);
            void updateWeight(int node, sem_elem_t wt);
            void updateWeight(int node, int uno);
            void assignUpdates();
            void clearUpdates();

            bool updateEdgeWeight(int src, int tgt, sem_elem_t se);
            void setupIntraSolution(bool compress_regexp = false);
            sem_elem_t get_weight(int outnode);
            ostream &print(ostream &out, PRINT_OP pop = defaultPrintOp);
            static ostream &print_trans(Transition &t, ostream &out, PRINT_OP pop = defaultPrintOp);
            void resetUpdatable();

            void solveSummarySolution(list<WTransition> &change);
            void preSolveSummarySolution(list<WTransition> &change);
            void setupSummarySolution();

            private:
            //int nodeno(Transition &t);
            void create_node(Transition &t, int n);
            void create_node(int n);
            int edgeno(int s, int t);
            sem_elem_t extend(sem_elem_t w1, sem_elem_t w2);
            void dfs(int v, set<int> &, set<int> &, set<int> &);
            void topSort(vector<IntraGraphNode> &cnodes, int, vector<IntraGraphEdge> &cedges, int,
                    list<int> &ts, vector<int> &cs, bool no_outgoing, bool no_updatable);
            int SCC(vector<IntraGraphNode> &cnodes, int, vector<IntraGraphEdge> &cedges, int);
            void buildCutsetRegExp(list<int> &ts, vector<int> &cs, vector<IntraGraphNode> &cnodes, int, vector<IntraGraphEdge> &cedges, int);
            void compressGraph(list<int> &ts, vector<int> &cs, list<int> &nts, vector<int> &ncs,
                    vector<IntraGraphNode> &cnodes, vector<IntraGraphEdge> &cedges,
                    map<int,int> &orig_to_compress);
            void compressGraphAggressive(list<int> &ts, vector<int> &cs, list<int> &nts, vector<int> &ncs,
                    vector<IntraGraphNode> &cnodes, vector<IntraGraphEdge> &cedges,
                    map<int,int> &orig_to_compress);
            void basicRegExp(bool compress_regexp);
            int *computeDominators(vector<IntraGraphNode> &cnodes, int, vector<IntraGraphEdge> &cedges, int, int *buffer, set<int> *bucket_buffer);
            void dfsDominators(vector<IntraGraphNode> &cnodes, vector<IntraGraphEdge> &cedges, int v, int *parent, int *semi, int *vertex, int &n);
            void setupDomRegExp(vector<IntraGraphNode> &cnodes, int, vector<IntraGraphEdge> &cedges, int, 
                    int *dom, int *number, int *vertex, int *tree, set<int> *children);
            void numberNodes(int v, int *number, int *vertex, set<int> *children, int &count);
            void compress_and_sequence(vector<IntraGraphNode> &cnodes, vector<IntraGraphEdge> &cedges,
                    int h, int *ancestor, vector<PathSequence> &sequence);
            reg_exp_t eval_and_sequence(vector<IntraGraphNode> &cnodes, vector<IntraGraphEdge> &cedges,
                    int e, int *ancestor, vector<PathSequence> &sequence);
            void domRegExp(vector<IntraGraphNode> &cnodes, int, vector<IntraGraphEdge> &cedges, int, vector<PathSequence> &seq);
            void buildRegExp(vector<PathSequence> &seq);
            void computePathSequence(vector<IntraGraphNode> &cnodes, int, vector<IntraGraphEdge> &cedges, int, vector<PathSequence> &sequence, bool use_cutset = false);
            void computePathSequenceCutset(vector<IntraGraphNode> &cnodes, int, vector<IntraGraphEdge> &cedges, int, vector<PathSequence> &sequence);

            sem_elem_t popWeight(int nno);
            void calculatePopWeights(int eps_nno);

            void solveRegSummarySolution();
            void preSolveRegSummarySolution();

        };

    } // namespace graph

} // namespace wali

#endif // wali_graph__INTRA_GRAPH_H
