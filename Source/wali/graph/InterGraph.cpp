#include "wali/graph/InterGraph.hpp"
#include "wali/graph/IntraGraph.hpp"
#include "wali/graph/RegExp.hpp"

#include "wali/util/Timer.hpp"

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <iomanip>

typedef int INTER_GRAPH_INT;

using namespace std;

namespace wali {

    namespace graph {

        UnionFind::UnionFind(int len) {
            n = len;
            arr = new int[n];
            for(int i=0; i < n;i++) {
                arr[i] = i;
            }
        }

        UnionFind::~UnionFind() {
            delete [] arr;
            arr = NULL;
            n = 0;
        }

        void UnionFind::reset() {
            for(int i=0; i < n;i++) {
                arr[i] = i;
            }
        }

        int UnionFind::find(int a) {
            int r = a,p;
            while(arr[r] != r) {
                r = arr[r];
            }
            // path compression
            while(arr[a] != a) {
                p = arr[a];
                arr[a] = r;
                a = p;
            }
            return r;
        }

        void UnionFind::takeUnion(int a, int b) {
            int ar = find(a);
            int br = find(b);
            arr[ar] = br; // FIXME: Randomize this
        }

        inter_node_t promote_type(inter_node_t t1, inter_node_t t2) {
            if(t1 == InterNone)
                return t2;
            if(t2 == InterNone)
                return t1;
            if(t1 == t2)
                return t1;
            return InterSourceOutNode;
        }

        bool is_source_type(inter_node_t t1) {
            return (t1 == InterSource || t1 == InterSourceOutNode);
        }

        void ETransHandler::addEdge(int call, int ret, sem_elem_t wtCallRule) {
          EdgeMap::iterator it = edgeMap.find(ret);
          if(it != edgeMap.end()) {
            Dependency &d = it->second;
            d.second = d.second->combine(wtCallRule);
          } else {
            edgeMap[ret] = Dependency(call, wtCallRule);
          }
        }

        bool ETransHandler::exists(int ret) {
          EdgeMap::iterator it = edgeMap.find(ret);
          if(it != edgeMap.end()) {
            return true;
          }
          return false;
        }


        // return NULL if no match found
        sem_elem_t ETransHandler::get_dependency(int ret, int &call) {
          EdgeMap::iterator it = edgeMap.find(ret);
          if(it != edgeMap.end()) {
            Dependency &d = it->second;
            call = d.first;
            return d.second;
          }
          return sem_elem_t(0);
        }

        InterGraph::~InterGraph() {
            for(unsigned i = 0; i < nodes.size(); i++) {
                if(nodes[i].gr && intra_graph_uf->find(i) == (int)i) {
                    delete nodes[i].gr;
                }
            }
            if(intra_graph_uf) {
                delete intra_graph_uf;
            }
        }

        int InterGraph::nodeno(Transition &t) {
            TransMap::iterator it = node_number.find(t);
            if(it == node_number.end()) {
                node_number[t] = static_cast<INTER_GRAPH_INT>(nodes.size());
                nodes.push_back(GraphNode(t));
                return (static_cast<INTER_GRAPH_INT>(nodes.size()) - 1);
            }
            return it->second;
        }

        bool InterGraph::exists(Transition &t) {
            TransMap::iterator it = node_number.find(t);
            if(it == node_number.end()) {
                return false;
            }
            return true;
        }

        int InterGraph::intra_edgeno(Transition &src, Transition &tgt) {
            int s = nodeno(src);
            int t = nodeno(tgt);
            std::list<int>::iterator it = nodes[s].outgoing.begin();
            for(; it != nodes[s].outgoing.end(); it++) {
                if(intra_edges[*it].tgt == t) {
                    return *it;
                }
            }
            return -1;
        }

        int InterGraph::inter_edgeno(Transition &src1, Transition &src2, Transition &tgt) {
            int s1 = nodeno(src1);
                int s2 = nodeno(src2);
                int t = nodeno(tgt);
                std::list<int>::iterator it = nodes[s2].out_hyper_edges.begin();
                for(; it != nodes[s2].out_hyper_edges.end(); it++) {
                    if(inter_edges[*it].tgt == t && inter_edges[*it].src1 == s1) {
                        return *it;
                    }
                }
                return -1;
            }

            bool InterGraph::exists(int state, int stack, WT_CHECK op) {
                size_t i;
                for(i=0;i<nodes.size();i++) {
                    if(static_cast<INTER_GRAPH_INT>(nodes[i].trans.src) == state
                       && static_cast<INTER_GRAPH_INT>(nodes[i].trans.stack) == stack)
                    {
                        if(op((nodes[i].gr->get_weight(nodes[i].intra_nodeno)).get_ptr())) 
                            return true;
                    }
                }
                return false;
            }

            std::ostream &InterGraph::print(std::ostream &out, PRINT_OP pop) {
                unsigned i;
                if(CONSTANT_CONDITION(1))//intra_graph_uf == NULL) 
                {
                    out << "Source Transitions:\n";
                    for(i=0;i<nodes.size();i++) {
                        if(is_source_type(nodes[i].type)) {
                            IntraGraph::print_trans(nodes[i].trans, out, pop);
                            nodes[i].weight->print(out);
                            out << "\n";
                        }
                    }
                    out << "IntraEdges:\n";
                    for(i=0;i<intra_edges.size();i++) {
                        Transition src = nodes[intra_edges[i].src].trans;
                        Transition tgt = nodes[intra_edges[i].tgt].trans;
                        IntraGraph::print_trans(src, out, pop);
                        out << "-->";
                        IntraGraph::print_trans(tgt, out, pop);
                        intra_edges[i].weight->print(out);
                        out << "\n";
                    }
                } else {
                    for(i = 0; i < nodes.size(); i++) {
                        if(nodes[i].gr && intra_graph_uf->find(i) == (int)i) {
                            nodes[i].gr->print(out,pop);
                            out << "\n";
                        }
                    }
                }
                out << "HyperEdges:\n";
                for(i = 0; i < inter_edges.size(); i++) {
                    Transition src1 = nodes[inter_edges[i].src1].trans;
                    Transition src2 = nodes[inter_edges[i].src2].trans;
                    Transition tgt = nodes[inter_edges[i].tgt].trans;
                    IntraGraph::print_trans(src1, out, pop);
                    out << ",";
                    IntraGraph::print_trans(src2, out, pop);
                    out << "-->";
                    IntraGraph::print_trans(tgt, out, pop);
                    if(inter_edges[i].weight.get_ptr()) {
                        inter_edges[i].weight->print(out);
                    } else {
                        inter_edges[i].mf->print(out);
                    }
                    out << "\n";
                }
                return out;
            }

            void InterGraph::addEdge(Transition src, Transition tgt, wali::sem_elem_t se) {
                int eno = intra_edgeno(src,tgt);
                if(eno != -1) { // edge already present
                    intra_edges[eno].weight = intra_edges[eno].weight->combine(se);
                    return;
                }
                int s = nodeno(src);
                int t = nodeno(tgt);
                GraphEdge ed(s,t,se);
                intra_edges.push_back(ed);
                int e = intra_edges.size() - 1;
                nodes[s].outgoing.push_back(e);
                nodes[t].incoming.push_back(e);
            }

            void InterGraph::addCallRetEdge(Transition src, Transition tgt, wali::sem_elem_t se) {
              addEdge(src, tgt, se->one());
              int s = nodeno(src);
              int t = nodeno(tgt);
              eHandler.addEdge(s, t, se);
            }

            void InterGraph::addEdge(Transition src1, Transition src2, Transition tgt, wali::sem_elem_t se) {
                int eno = inter_edgeno(src1,src2,tgt);
                if(eno != -1) { // edge already present
                    inter_edges[eno].weight = inter_edges[eno].weight->combine(se);
                    return;
                }
                int s1 = nodeno(src1);
                int s2 = nodeno(src2);
                int t = nodeno(tgt);
                HyperEdge ed(s1,s2,t,se);
                nodes[s2].type = promote_type(nodes[s2].type, InterOutNode);
                inter_edges.push_back(ed);
                nodes[s2].out_hyper_edges.push_back(inter_edges.size() - 1);
            }

            void InterGraph::addEdge(Transition src1, Transition src2, Transition tgt, merge_fn_t mf) {
                assert(running_ewpds);
                int eno = inter_edgeno(src1,src2,tgt);
                if(eno != -1 && mf == inter_edges[eno].mf) { // edge already present
                    return;
                }
                int s1 = nodeno(src1);
                int s2 = nodeno(src2);
                int t = nodeno(tgt);
                HyperEdge ed(s1,s2,t,mf);
                nodes[s2].type = promote_type(nodes[s2].type, InterOutNode);
                inter_edges.push_back(ed);
                nodes[s2].out_hyper_edges.push_back(inter_edges.size() - 1);
            }

            void InterGraph::addCallEdge(Transition src1, Transition src2) {
                call_edges.push_back(call_edge_t(nodeno(src1),nodeno(src2)));
            }

            void InterGraph::setSource(Transition t, wali::sem_elem_t se) {
                int n = nodeno(t);
                nodes[n].type = promote_type(nodes[n].type, InterSource);
                nodes[n].weight = se;
            }

            void InterGraph::setESource(Transition t, wali::sem_elem_t wtAtCall, wali::sem_elem_t wtAfterCall) {
                // setSource
                int n = nodeno(t);
                nodes[n].type = promote_type(nodes[n].type, InterSource);
                nodes[n].weight = wtAtCall;
                // Extra dependency
                eHandler.addEdge(-1, n, wtAfterCall);
            }

            unsigned InterGraph::SCC(list<IntraGraph *> &grlist, std::list<IntraGraph *> &grsorted) {
                std::list<IntraGraph *>::iterator gr_it;
                // reset visited
                for(gr_it = grlist.begin(); gr_it != grlist.end(); gr_it++) {
                    (*gr_it)->visited = false;
                    (*gr_it)->scc_number = 0;
                    (*gr_it)->bfs_number = (unsigned)(-1);
                }
                std::list<IntraGraph *> finished;
                std::map<IntraGraph *, std::list<IntraGraph *> > rev_edges;
                // Do DFS
                std::list<IntraGraph *>::reverse_iterator gr_rit;
                for(gr_rit = grlist.rbegin(); gr_rit != grlist.rend(); gr_rit++) {
                    IntraGraph *gr = *gr_rit;
                    if(gr->visited)
                        continue;
                    dfsIntraForward(gr,finished,rev_edges);
                }

                unsigned scc = 0;

                for(gr_it = grlist.begin(); gr_it != grlist.end(); gr_it++) {
                    (*gr_it)->visited = false;
                }

                for(gr_it = finished.begin(); gr_it != finished.end(); gr_it++) {
                    IntraGraph *gr = *gr_it;
                    if(gr->visited)
                        continue;
                    scc++;
                    typedef pair<IntraGraph *, std::list<IntraGraph *>::iterator> StackEl;
                    std::list<StackEl> stack;
                    stack.push_back(StackEl(gr, rev_edges[gr].begin()));
                    while(!stack.empty()) {
                        StackEl p = stack.front();
                        stack.pop_front();
                        IntraGraph *v = p.first;
                        std::list<IntraGraph *>::iterator it = p.second;
                        v->scc_number = scc;
                        if(!v->visited) grsorted.push_back(v);
                        v->visited = true;
                        while(it != rev_edges[v].end()) {
                            IntraGraph *c = *it;
                            if(c->visited) {
                                it++;
                            } else { 
                                stack.push_front(StackEl(v,++it));
                                stack.push_front(StackEl(c, rev_edges[c].begin()));
                                break;
                            }
                        }
                    }
                }
                // reset visited
                for(gr_it = grlist.begin(); gr_it != grlist.end(); gr_it++) {
                    (*gr_it)->visited = false;
                }
                return scc;
            }

            void InterGraph::dfsIntraForward(IntraGraph *gr, std::list<IntraGraph *> &finished, std::map<IntraGraph *, std::list<IntraGraph *> > &rev_edges) {
                gr->visited = true;
                std::list<int> *outnodes = gr->getOutTransitions();
                std::list<int>::iterator it;
                for(it = outnodes->begin(); it != outnodes->end(); it++) {
                    int n = *it;
                    std::list<int>::iterator beg = nodes[n].out_hyper_edges.begin();
                    std::list<int>::iterator end = nodes[n].out_hyper_edges.end();
                    for(; beg != end; beg++) {
                        IntraGraph *ch = nodes[inter_edges[*beg].tgt].gr;
                        rev_edges[ch].push_back(gr);
                        if(!ch->visited)
                            dfsIntraForward(ch, finished,rev_edges);
                    }
                }
                finished.push_front(gr);
            }

            // BFS of a SCC
            void InterGraph::bfsIntra(IntraGraph *start, unsigned int scc_number) {
                std::list <IntraGraph *> workset;
                workset.push_back(start);
                IntraGraph *gr;
                start->bfs_number = 0;
                while(!workset.empty()) {
                    gr = workset.front();
                    workset.pop_front();
                    if(gr->visited) continue;

                    gr->visited = true;

                    std::list<int> *outnodes = gr->getOutTransitions();
                    std::list<int>::iterator it;
                    for(it = outnodes->begin(); it != outnodes->end(); it++) {
                        int n = *it;
                        std::list<int>::iterator beg = nodes[n].out_hyper_edges.begin();
                        std::list<int>::iterator end = nodes[n].out_hyper_edges.end();
                        for(; beg != end; beg++) {
                            IntraGraph *ch = nodes[inter_edges[*beg].tgt].gr;
                            if(ch->scc_number != scc_number) continue;
                            if(!ch->visited) {
                                ch->bfs_number = (ch->bfs_number > (gr->bfs_number) + 1) ? (gr->bfs_number+1) : ch->bfs_number;
                                workset.push_back(ch);
                            }
                        }
                    }
                }
            }

            // BFS of a SCC
            void InterGraph::resetSCCedges(IntraGraph *gr, unsigned int scc_number) {
                std::list<int> *outnodes = gr->getOutTransitions();
                std::list<int>::iterator it;
                for(it = outnodes->begin(); it != outnodes->end(); it++) {
                    int n = *it;
                    std::list<int>::iterator beg = nodes[n].out_hyper_edges.begin();
                    std::list<int>::iterator end = nodes[n].out_hyper_edges.end();
                    for(; beg != end; beg++) {
                        int inode = inter_edges[*beg].tgt;
                        int onode1 = inter_edges[*beg].src1;
                        IntraGraph *ch = nodes[inode].gr;
                        if(ch->scc_number != scc_number) continue;
                        ch->updateEdgeWeight(nodes[onode1].intra_nodeno, nodes[inode].intra_nodeno, sem->zero());
                    }
                }
            }

            void InterGraph::setup_worklist(list<IntraGraph *> &gr_sorted, std::list<IntraGraph *>::iterator &gr_it, unsigned int scc_n,
                    multiset<tup> &worklist) {
                worklist.clear();
                while(gr_it != gr_sorted.end() && (*gr_it)->scc_number == scc_n ) {
                    std::list<int>::iterator tbeg = (*gr_it)->getOutTransitions()->begin();
                    std::list<int>::iterator tend = (*gr_it)->getOutTransitions()->end();
                    while(tbeg != tend) {
                        int onode = *tbeg;
                        worklist.insert(tup((*gr_it)->bfs_number,onode));
                        //nodes[onode].weight = nodes[onode].gr->get_weight(nodes[onode].trans);
                        tbeg++;
                    }
                    gr_it++;
                }
            }                       

            int myrand(int l, int u) {
                int a = rand();
                return l+int((u-l+1)*(a/(RAND_MAX + 1.0))); 
            }

            // If an argument is passed in then only weights on those transitions will be available
            // I can fix this (i.e., weights for others will be available on demand), but not right now.
            void InterGraph::setupInterSolution(std::list<Transition> *wt_required) {
                RegExp::startSatProcess(sem);

                //util::Timer *timer = new util::Timer("FWPDS Find Graphs");

                // First, find the IntraGraphs
                int n = nodes.size();
                int i;
                unsigned int max_scc_required;
                intra_graph_uf = new UnionFind(n);

                vector<GraphEdge>::iterator it;
                vector<HyperEdge>::iterator it2;

                for(it = intra_edges.begin(); it != intra_edges.end(); it++) {
                    intra_graph_uf->takeUnion((*it).src,(*it).tgt);
                }


                for(it2 = inter_edges.begin(); it2 != inter_edges.end(); it2++) {
                    intra_graph_uf->takeUnion((*it2).src1,(*it2).tgt);
                }

                std::list<IntraGraph *>::iterator gr_it;

                for(i = 0; i < n;i++) {
                    int j = intra_graph_uf->find(i);
                    if(nodes[j].gr == NULL) {
                        nodes[j].gr = new IntraGraph(running_prestar,sem);
                        gr_list.push_back(nodes[j].gr);
                    }
                    nodes[i].gr = nodes[j].gr;

                    nodes[i].intra_nodeno = nodes[i].gr->makeNode(nodes[i].trans);

                    if(is_source_type(nodes[i].type)) {
                        nodes[i].gr->setSource(nodes[i].intra_nodeno, nodes[i].weight);
                    }
                    // zero all weights (some are set by setSource() )
                    if(nodes[i].weight.get_ptr() != NULL)
                        nodes[i].weight = nodes[i].weight->zero();

                }

                // Now fill up the IntraGraphs
                for(it = intra_edges.begin(); it != intra_edges.end(); it++) {
                    int s = (*it).src;
                    int t = (*it).tgt;
                    nodes[s].gr->addEdge(nodes[s].intra_nodeno, nodes[t].intra_nodeno, (*it).weight);
                }

                for(it2 = inter_edges.begin(); it2 != inter_edges.end(); it2++) {
                    IntraGraph *gr = nodes[(*it2).tgt].gr;
                    gr->addEdge(nodes[(*it2).src1].intra_nodeno, nodes[(*it2).tgt].intra_nodeno, sem->zero(), true);

                    IntraGraph *gr2 = nodes[(*it2).src2].gr;
                    gr2->setOutNode(nodes[(*it2).src2].intra_nodeno, (*it2).src2);
                }

                // For SWPDS
                vector<call_edge_t>::iterator it3;
                for(it3 = call_edges.begin(); it3 != call_edges.end(); it3++) {
                    IntraGraph *gr1 = nodes[(*it3).first].gr;
                    IntraGraph *gr2 = nodes[(*it3).second].gr;
                    gr1->addCallEdge(gr2);
                }

                // Setup Worklist
                multiset<tup > worklist;

#ifdef STATIC_MEMORY
                int max_size = 0;
                for(gr_it = gr_list.begin(); gr_it != gr_list.end(); gr_it++) {
                    max_size = (max_size > (*gr_it)->getSize()) ? max_size : (*gr_it)->getSize();
                }
                IntraGraph::addStaticBuffer(max_size);
#endif

                for(gr_it = gr_list.begin(); gr_it != gr_list.end(); gr_it++) {
                    (*gr_it)->setupIntraSolution(false);
                }
                //cout << "Intra setup done.\n";

                // Do SCC decomposition of IntraGraphs
                std::list<IntraGraph *> gr_sorted;
                unsigned components = SCC(gr_list, gr_sorted);
                STAT(stats.ncomponents = components);

                //delete timer;
                //timer = new util::Timer("FWPDS Saturation");

                // Saturate
                if(wt_required == NULL) {
                    max_scc_required = components;
                } else {
                    max_scc_required = 0;
                    std::list<Transition>::iterator trans_it;
                    for(trans_it = wt_required->begin(); trans_it != wt_required->end(); trans_it++) {
                        int nno = nodeno(*trans_it);
                        max_scc_required = (max_scc_required >= nodes[nno].gr->scc_number) ? max_scc_required : nodes[nno].gr->scc_number;
                    }
                }
                //cout << "Saturation started\n";
                gr_it = gr_sorted.begin();
                for(unsigned scc_n = 1; scc_n <= max_scc_required; scc_n++) {
                    //cout << ".";
                    bfsIntra(*gr_it, scc_n);
                    setup_worklist(gr_sorted, gr_it, scc_n, worklist);
                    saturate(worklist,scc_n);
                }
                max_scc_computed = max_scc_required;
                RegExp::stopSatProcess();
                RegExp::executingPoststar(!running_prestar);

#ifdef STATIC_MEMORY
                IntraGraph::clearStaticBuffer();
#endif
                //delete timer;
            }

            std::ostream &InterGraph::print_stats(std::ostream &out) {
                InterGraphStats total_stats = stats;
                int n = nodes.size();
                int i;      
                set<RegExp *> reg_equations;
                int ned = 0;
                for(i = 0; i < n; i++) {
                    if(intra_graph_uf->find(i) == i) {
                        std::list<int>::iterator tbeg = nodes[i].gr->getOutTransitions()->begin();
                        std::list<int>::iterator tend = nodes[i].gr->getOutTransitions()->end();
                        for(; tbeg != tend; tbeg++) {
                            int onode = nodes[i].intra_nodeno;
                            reg_equations.insert(nodes[i].gr->nodes[onode].regexp.get_ptr());
                        }
                        total_stats.ngraphs ++;
                        IntraGraphStats st = nodes[i].gr->get_stats(); 
                        total_stats.ncombine += st.ncombine;
                        total_stats.nextend += st.nextend;
                        total_stats.nstar += st.nstar;
                        total_stats.nupdatable += st.nupdatable * st.nupdatable;
                        total_stats.ncutset += st.ncutset * st.ncutset * st.ncutset;
                        total_stats.nget_weight += st.nget_weight;
                        total_stats.ndom_sequence += st.ndom_sequence;
                        total_stats.ndom_components += st.ndom_components;
                        total_stats.ndom_componentsize += (st.ndom_componentsize * st.ndom_componentsize);
                        total_stats.ndom_componentcutset += st.ndom_componentcutset;
                        ned += st.nedges;
                    }
                }
                int changestat = RegExp::out_node_height(reg_equations);
                total_stats.nhyperedges = inter_edges.size();
                total_stats.nedges = intra_edges.size();
                total_stats.nnodes = nodes.size();

                RegExpStats rst = RegExp::get_stats();
                total_stats.ncombine += rst.ncombine;
                total_stats.nextend += rst.nextend;
                total_stats.nstar += rst.nstar;
                total_stats.ngraphs = (total_stats.ngraphs == 0) ? 1 : total_stats.ngraphs;
                rst.out_nodes = (rst.out_nodes == 0) ? 1 : rst.out_nodes;
                out << "----------------------------------\n";
                out << "          FWPDS Stats             \n";
                out << "----------------------------------\n";
                out << "InterGraph nodes : " << total_stats.nnodes << "\n";
                out << "InterGraph edges : " << total_stats.nedges << "\n";
                out << "InterGraph hyperedges : " << total_stats.nhyperedges << "\n";
                out << "InterGraph iterations : " << total_stats.niter << "\n";
                out << "InterGraph get_weight : " << total_stats.nget_weight << "\n";
                out << "IntraGraphs : " << total_stats.ngraphs << "\n";
                out << "IntraGraph SCC : " << total_stats.ncomponents << "\n";
                out << "IntraGraph SCC Computed: " << max_scc_computed << "\n";
                out << "Avg. IntraGraph nodes : " << (total_stats.nnodes / total_stats.ngraphs) << "\n";
                out << "Avg. IntraGraph edges : " << (ned / total_stats.ngraphs) << "\n";
                out << "Avg. IntraGraph cutset : " << (pow(total_stats.ncutset/total_stats.ngraphs,0.33)) << "\n";
                out << "Avg. IntraGraph updatable : " << (int)(pow((double)total_stats.nupdatable/total_stats.ngraphs,0.5)) << "\n";
                out << "Avg. IntraGraph dom-sequence length : " << (total_stats.ndom_sequence / total_stats.ngraphs) << "\n";
                out << "Avg. IntraGraph dom-component size : " << (int)pow((double)total_stats.ndom_componentsize / (total_stats.ndom_components+1),0.5) << "\n";
                out << "Avg. IntraGraph dom-component cuset : " << setprecision(2) << (double)total_stats.ndom_componentcutset / (total_stats.ndom_components+1) << "\n";
                out << "Semiring Combine : " << total_stats.ncombine << "\n";
                out << "Semiring Extend : " << total_stats.nextend << "\n";
                out << "Semiring Star : " << total_stats.nstar << "\n";
                out << "RegExp HashMap hits : " << rst.hashmap_hits << "\n";
                out << "RegExp HashMap misses : " << rst.hashmap_misses << "\n";
                out << "OutNode Height : " << setprecision(4) << (rst.height / rst.out_nodes) << "\n"; 
                out << "OutNode Loop ND : " << setprecision(4) << (rst.lnd / rst.out_nodes) << "\n";
                out << "Change Stat : " << changestat << "\n";
                out << "\n";
                return out;
            }

            // New Saturation Procedure -- minimize calls to get_weight
            void InterGraph::saturate(multiset<tup> &worklist, unsigned scc_n) {
                sem_elem_t weight;
                std::list<int> *moutnodes;

                while(!worklist.empty()) {
                    // Get an outnode whose weight is to be propagated
                    multiset<tup>::iterator wit = worklist.begin();
                    int onode = (*wit).second;
                    worklist.erase(wit);
                    //int onode = worklist.front();
                    //worklist.pop_front();

                    weight = nodes[onode].gr->get_weight(nodes[onode].intra_nodeno);
                    if(nodes[onode].weight.get_ptr() != NULL && nodes[onode].weight->equal(weight))
                        continue;
                    nodes[onode].weight = weight;

                    STAT(stats.niter++);

                    FWPDSDBGS(
                            cout << "Popped ";
                            IntraGraph::print_trans(nodes[onode].trans,cout) << "with weight ";
                            weight->print(cout) << "\n";
                            );

                    // Go through all its targets and modify their weights
                    std::list<int>::iterator beg = nodes[onode].out_hyper_edges.begin();
                    std::list<int>::iterator end = nodes[onode].out_hyper_edges.end();
                    for(; beg != end; beg++) {
                        int inode = inter_edges[*beg].tgt;
                        int onode1 = inter_edges[*beg].src1;
                        sem_elem_t uw;
                        if(running_ewpds && inter_edges[*beg].mf.get_ptr()) {
                            uw = inter_edges[*beg].mf->apply_f(sem->one(), weight);
                            FWPDSDBGS(
                                    cout << "Apply merge function ";
                                    inter_edges[*beg].mf->print(cout) << " to ";
                                    weight->print(cout) << "\n";
                                    uw->print(cout << "Got ") << "\n";
                                    );
                        } else {
                            uw = inter_edges[*beg].weight->extend(weight);
                        }
                        STAT(stats.nextend++);
                        nodes[inode].gr->updateEdgeWeight(nodes[onode1].intra_nodeno, nodes[inode].intra_nodeno, uw);
                    }
                    // Go through all targets again and insert them into the workist without
                    // seeing if they actually got modified or not
                    beg = nodes[onode].out_hyper_edges.begin();
                    for(; beg != end; beg++) {
                        int inode = inter_edges[*beg].tgt;
                        IntraGraph *gr = nodes[inode].gr;
                        if(gr->scc_number != scc_n) {
                            assert(gr->scc_number > scc_n);
                            continue;
                        }
                        moutnodes = gr->getOutTransitions();
                        std::list<int>::iterator mbeg = moutnodes->begin();
                        std::list<int>::iterator mend = moutnodes->end();
                        for(; mbeg != mend; mbeg++) {
                            int mnode = (*mbeg);
                            worklist.insert(tup(gr->bfs_number, mnode));
                        }
                    }
                }
            }

            // Must be called after saturation
            sem_elem_t InterGraph::get_call_weight(Transition t) {
                unsigned orig_size = nodes.size();
                unsigned n = nodeno(t);
                assert(orig_size == nodes.size()); // Transition t must not be a new one

                return nodes[n].gr->get_weight(nodes[n].intra_nodeno);
            }

            sem_elem_t InterGraph::get_weight(Transition t) {
                unsigned orig_size = nodes.size();
                unsigned n = nodeno(t);
                assert(orig_size == nodes.size()); // Transition t must not be a new one

                // check eHandler
                if(eHandler.exists(n)) {
                  // This must be a return transition
                  int nc;
                  sem_elem_t wtCallRule = eHandler.get_dependency(n, nc);
                  sem_elem_t wt;
                  if(nc != -1) {
                    wt = nodes[nc].gr->get_weight(nodes[nc].intra_nodeno);
                  } else {
                    // ESource
                    wt = sem->one();
                  }
                  return wt->extend(wtCallRule);
                }

                return nodes[n].gr->get_weight(nodes[n].intra_nodeno);
            }

            void InterGraph::update_all_weights() {
                unsigned int i;
                for(i=0;i<nodes.size();i++) {
                    sem_elem_t w = nodes[i].gr->get_weight(nodes[i].intra_nodeno);
                    nodes[i].weight = w;
                }
            }

            inline int get_number(map<int,int> &intra_node_map, int src, IntraGraph *ca) {
                std::map<int,int>::iterator it = intra_node_map.find(src);
                if(it != intra_node_map.end()) {
                    return it->second;
                }
                int s = ca->makeNode();
                intra_node_map[src] = s;
                return s;
            }

            bool InterGraph::path_summary(int state, int stack, int accept, WT_CORRECT correct, WT_CHECK op) {
                // Build a hashmap: transition.src -> transitions
                typedef wali::HashMap<int, std::list<int> > trans_map_t;
                std::map<int, int> intra_node_map; // transition.src -> intra node number

                trans_map_t trans_map;
                set<int> states_visited;
                std::list<int> worklist;
                unsigned int i;
                IntraGraph *ca = new IntraGraph(true, sem); // running_prestar = true because extend goes backward
                //Transition initial_st(state, 0, 0);

                ca->setSource(get_number(intra_node_map,state,ca), sem->one());
                for(i=0;i<nodes.size();i++) {
                    trans_map_t::iterator it = trans_map.find(nodes[i].trans.src);
                    if(it == trans_map.end()) {
                        std::list<int> temp;
                        temp.push_back(i);
                        trans_map.insert(nodes[i].trans.src, temp);
                    } else {
                        it->second.push_back(i);
                    }
                    // add initial (state, stack, _) transitions
                    if(nodes[i].trans.src == state && nodes[i].trans.stack == stack) {
                        Transition t1(state,0,0);
                        Transition t2(nodes[i].trans.tgt,0,0);
                        ca->addEdge(get_number(intra_node_map,state,ca), get_number(intra_node_map, nodes[i].trans.tgt,ca), 
                                correct(nodes[i].gr->get_weight(nodes[i].intra_nodeno).get_ptr()));
                        worklist.push_back(nodes[i].trans.tgt);
                    }
                }
                states_visited.insert(state);
                while(!worklist.empty()) {
                    int st = worklist.front();
                    worklist.pop_front();
                    if(states_visited.find(st) != states_visited.end()) 
                        continue;
                    states_visited.insert(st);
                    trans_map_t::iterator trans_it = trans_map.find(st);
                    if(trans_it == trans_map.end())
                        continue;
                    std::list<int> &trans = trans_it->second;
                    std::list<int>::iterator it;
                    for(it = trans.begin(); it != trans.end(); it++) {
                        i = *it;
                        int t1 = get_number(intra_node_map,nodes[i].trans.src,ca);
                        int t2 = get_number(intra_node_map,nodes[i].trans.tgt,ca);
                        ca->addEdge(t1, t2, correct(nodes[i].gr->get_weight(nodes[i].intra_nodeno).get_ptr()));
                        if(states_visited.find(nodes[i].trans.tgt) == states_visited.end()) {
                            worklist.push_back(nodes[i].trans.tgt);
                        }
                    }
                }
                int final_st = get_number(intra_node_map,accept, ca);
                ca->setOutNode(final_st, 1); // second argument is not required
                ca->setupIntraSolution(false);
                bool r = op(ca->get_weight(final_st).get_ptr());
                delete ca;
                return r;
            }


    } // namespace graph

} // namespace wali

