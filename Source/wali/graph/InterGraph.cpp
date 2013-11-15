#include "wali/graph/InterGraph.hpp"
#include "wali/graph/IntraGraph.hpp"
#include "wali/graph/RegExp.hpp"
#include "wali/graph/Functional.hpp"

#include "wali/util/Timer.hpp"

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sstream>
#include <boost/cast.hpp>

// ::wali
#include "wali/SemElemTensor.hpp"

typedef int INTER_GRAPH_INT;

using namespace std;

namespace wali {

    namespace graph {

        UnionFind::UnionFind(int len) {
            n = len;
            count = len;
            arr = new int[n];
            for(int i=0; i < n;i++) {
                arr[i] = i;
            }
        }

        UnionFind::UnionFind(UnionFind& from) : n(from.n), count(from.count){
          arr = new int[n];
          for(int i=0; i<n; ++i)
            arr[i] = from.arr[i];
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
            //TODO: Used ranked union

            // if two sets were merged, the total number of sets has decreaseed
            // by one
            if(ar != br) count--;
            assert(count > 0 && "Count of sets reached zero: There is a bug\n");
        }

        int UnionFind::countSets(){
          return count;
        }

        std::ostream& UnionFind::print(std::ostream& o)
        {
          std::multiset< std::pair< int, int > > shadow;
          int curset = -1;

          o << "[ ";
          for(int i=0; i<n; ++i){
            shadow.insert(std::pair< int, int >(find(i),i));
          }

          for(std::multiset< std::pair< int, int > >::iterator iter = shadow.begin();
              iter != shadow.end();
              ++iter){
            if(iter->first > curset){
              if(curset != -1) 
                o << ">>>>>>>>>" << std::endl;
              curset = iter->first;
            }
            o << iter->second << " ";
          }
          o << "]";
          return o;
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

        sem_elem_tensor_t tensorSetUpFP(sem_elem_tensor_t a, sem_elem_tensor_t b)
        {
          return a->tensor(b.get_ptr());
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

        void ETransHandler::tensorAllWeights()
        {
          if(edgeMap.size() == 0)
            return;
          Dependency dy = edgeMap.begin()->second;
          sem_elem_tensor_t one = boost::polymorphic_downcast<SemElemTensor*>(dy.second->one().get_ptr());
          for(EdgeMap::iterator it = edgeMap.begin(); it != edgeMap.end(); ++it)
            edgeMap[it->first] = Dependency(it->second.first, tensorSetUpFP(one, boost::polymorphic_downcast<SemElemTensor*>(it->second.second.get_ptr())).get_ptr());
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

        InterGraph::InterGraph(wali::sem_elem_t s, bool e, bool pre, bool n)
        {
          sem = s;
          intra_graph_uf = NULL;
          running_ewpds = e;
          running_nwpds = n;
          running_prestar = pre;
          max_scc_computed = 0;
          newtonGr = NULL;
          runningNewton = false;
          dag = new RegExpDag();
          count = 0;
          isOutputAutomatonTensored = false;
        }

        InterGraph::~InterGraph() {
          delete dag;
          std::set<IntraGraph*> deleteGr;
          for(unsigned i = 0; i < nodes.size(); i++) {
            if(nodes[i].gr && intra_graph_uf->find(i) == (int)i) {
              deleteGr.insert(nodes[i].gr);
              nodes[i].gr=NULL;
            }
              nodes[i].sccgr = NULL;
          }
          for(std::set<IntraGraph*>::iterator it = deleteGr.begin(); it != deleteGr.end(); ++it)
            delete *it;

          if(intra_graph_uf) {
            delete intra_graph_uf;
            intra_graph_uf = NULL;
          }
          if(newtonGr){
            delete newtonGr;
            newtonGr = NULL;
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
              if(newtonGr){
                if(op((newtonGr->get_weight(nodes[i].intra_nodeno)).get_ptr())) 
                  return true;
              }else{
                  if(op((nodes[i].gr->get_weight(nodes[i].intra_nodeno)).get_ptr())) 
                    return true;
              }
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
          nodes[s1].out1_hyper_edges.push_back(inter_edges.size() - 1);

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
          nodes[s1].out1_hyper_edges.push_back(inter_edges.size() - 1);
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

        unsigned InterGraph::SCCLight(SCCGraphs& grlist, SCCGraphs& grsorted)
        {
          SCCGraphs::iterator gr_it;
          // reset visited
          for(gr_it = grlist.begin(); gr_it != grlist.end(); gr_it++) {
            (*gr_it)->visited = false;
            (*gr_it)->scc_number = 0;
          }
          SCCGraphs finished;
          std::map<scc_graph_t , SCCGraphs, SCCGraphLE> rev_edges;
          // Do DFS
          SCCGraphs::reverse_iterator gr_rit;
          for(gr_rit = grlist.rbegin(); gr_rit != grlist.rend(); gr_rit++) {
            scc_graph_t gr = *gr_rit;
            if(gr->visited)
              continue;
            dfsLight(gr,finished,rev_edges);
          }

          unsigned scc = 0;

          for(gr_it = grlist.begin(); gr_it != grlist.end(); gr_it++) {
            (*gr_it)->visited = false;
          }

          for(gr_it = finished.begin(); gr_it != finished.end(); gr_it++) {
            scc_graph_t gr = *gr_it;
            if(gr->visited)
              continue;
            scc++;
            typedef pair<scc_graph_t, SCCGraphs::iterator> StackEl;
            std::list<StackEl> stack;
            stack.push_back(StackEl(gr, rev_edges[gr].begin()));
            while(!stack.empty()) {
              StackEl p = stack.front();
              stack.pop_front();
              scc_graph_t v = p.first;
              SCCGraphs::iterator it = p.second;
              v->scc_number = scc;
              if(!v->visited) grsorted.push_back(v);
              v->visited = true;
              while(it != rev_edges[v].end()) {
                scc_graph_t c = *it;
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

        void InterGraph::dfsLight(scc_graph_t gr, SCCGraphs& finished, std::map<scc_graph_t, SCCGraphs, SCCGraphLE >& rev_edges) 
        {
          gr->visited = true;
          for(SCCGraphs::iterator it = gr->nextGraphs.begin();
              it != gr->nextGraphs.end();
              it++){
            scc_graph_t ch = *it;
            rev_edges[ch].push_back(gr);
            if(!ch->visited)
              dfsLight(ch, finished, rev_edges);
          }
                    
          finished.push_front(gr);
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

#if defined(PPP_DBG) && PPP_DBG >= 2
        static std::ostream& pp_weight(std::ostream& o, sem_elem_t weight)
        {
          if(weight == NULL){
            o << "NULL";
            return o;
          }

          stringstream ss;
          weight->print(ss);
          string res = "";
          string ssstr = ss.str();
          for(string::iterator iter = ssstr.begin(); iter != ssstr.end();
              iter++){
            if(*iter == '>'){
              res.push_back(*iter);
              res.append("\\n");
            }else
              res.push_back(*iter);
          }
          o << res;
          return o;
        }
#endif

        /**
         * @brief From the given TDG (The original InterGraph), create linearized TDGs corresponding
         * to each step of Newton. Then, solve the poststar problem by executing steps of the newton's 
         * method till fix-point.
         *
         * @note Only works for poststar
         * @note When the function returns, the result automaton may obtain tensored or non-tensored weights
         * based on some heuristics. You must be prepared to obtain either.
         * @see isOutputAutomatonTensored
         * @see setupInterSolution -- This function evolved originally from that.
         * @see IntraGraph, RegExp, SCCGraph. Uses them heavily.
         *
         * Function description: 
         * A TDG is an InterGraph (yes, me!) that consists of IntraGraphs.
         * The nodes of the TDG (referred to as n/s/c/x/r) are effectively transitions in the output
         * automaton
         * The edges of the TDG (referred to as e) are effectively the Grammar Flow Problem
         * equations (poststar rules, if you will)
         * (1) Find out SCCs in the graph.
         * Consider a graph where each node is an IntraGraph (this is close to the callgraph of the
         * original program).  The fix-point problem will be solved by linearizing each SCC in turn,
         * while walking the list of SCCs in reverse topological order.
         *
         * For each SCC:
         *   // Create one actual IntraGraph for current Intragraph.
         *   // There are two options here:
         *   //   - if |SCC| == 1 (i.e., SCC is just one procedure), mimic non-newton FWPDS
         *   //   - if |SCC| > 1 (i.e., we must coalesce multiple procedures by linearizing), use
         *   //     Newton's linearization
         *   (2) Add nodes to IntraGraph, along with unique source node s.
         *   (3) Compute |SCC|
         *   if |SCC| == 1 then
         *     // Mimic FWPDS
         *     // Add IntraEdges: since there is no recursive call, all edges will be immutable
         *        (4.1.1) For every source node n in InterGraph (roughly, procedure entry), add an
         *        Edge from s to n with weight 1 (call to setSource())
         *        (4.1.2) For every Edge (\delta_1 rule) n1-e->n2, add an edge n1-e->n2 to the
         *        IntraGraph
         *        (4.1.3) For every HyperEdge (\delta_2 rule) c-e-x->r (x is the external source),
         *        add an edge c-e->r with weight w(x). Since the call can't possibly be recursive,
         *        the summary for this procedure must already have been computed in some earlier
         *        SCC. Use that weight.
         *   else   
         *     // There are multiple SCCGraphs in this SCC. We will coalesce them into one
         *     // IntraGraph.
         *     // The basic outline here follows that in (4.1), but the equation system we are
         *     // forming in this case is the linear system. 
         *     // For source nodes and \delta_1 rules, instead of edges with weight w, we create
         *     // edges with weight (w,1^T) where (.,.) is tensor and ^T is transpose
         *     // The case for \delta_2 is split in two -- if the call is made to a function
         *     // belonging to a function with lower SCC number, then this case is handled
         *     // identically to (4.1.3). Instead, if the call is made to a function within this
         *     // SCC, for the call c-e-x->r, we add three edges: c-e1->r with functional(x);
         *     // x-e2->r with functional(c) and s-e3->r with functional(c,x) all mutable. This
         *     // corresponds. A functional is a recipe that is later used to compute the weight
         *     // that must go on the edge (between newton rounds) from the weights on the argument
         *     // nodes. For details of the exact weights and functionals that go on various edges,
         *     // see comments below within the function.
         *     (4.2.1) Add edges for source nodes
         *     (4.2.2) Add edges for \delta_1 rules
         *     (4.2.3.1) Add edges for \delta_2 rules where the call is to a function with lower SCC
         *     (4.2.3.2) Add edges for \delta_2 rules where the call is to a function with from same
         *     SCC.
         *     TODO: Argue that mutable edges are not needed in (4.2.1) and (4.2.2)
         *   (5) Use Tarjan's path listing algorithm to generate regular expressions for the
         *   IntraGraph.
         *   (6) Solve the linearized problem by saturation. 
         *   @see IntraGraph::saturate 
         *   Till fix-point:
         *      -- Evaluate the regular expressions (This corresponds to one Newton's round)
         *      -- update functionals and repeat.
         * (7) Decide whether the output automaton is tensored.
         * Because the weights on IntraGraphs can be both tensored and non-tensored (tensored in
         * 4.2, non-tensored in 4.1), we must decide how the weights on the output automaton are.
         * We do that by counting the number of extra tensor / detensor operations needed in either
         * choice, and then using a heuristic to decide between the two.
         * FIXME: Currently, we override the choice, and always tensor weights. This might be the
         * best choice anyway, because detensor is a lot costlier than tensor in most cases. (Not
         * true with nwa_detensor)
         * (8) Set some more stuff that IntraGraph, FWPDS need after poststar. Mostly copied from
         * setupInterSolution.
         * 
         **/
        void InterGraph::setupNewtonSolution()
        {
          runningNewton = true;         
          // First populate SCCGraph objects

          int n = nodes.size();
          int i;
          unsigned int max_scc_required;
          SCCGraphs scc_gr_list;
          SCCGraphs gr_sorted;
          // (1) The SCC computation.
          // We will use a light-weight graph data structure called SCCGraph to find out the 
          // SCCs in the TDG. Each SCCGraph roughly corresponds to what will later (in the function) become
          // an IntraGraph. But SCCGraph have no extra information beyond the graph node and edge information,
          // just enough to find the SCCs.
          intra_graph_uf = new UnionFind(n);
          {
            vector<GraphEdge>::iterator it;
            vector<HyperEdge>::iterator it2;
            for(it = intra_edges.begin(); it != intra_edges.end(); it++) {
              intra_graph_uf->takeUnion((*it).src,(*it).tgt);
            }
            for(it2 = inter_edges.begin(); it2 != inter_edges.end(); it2++) {
              intra_graph_uf->takeUnion((*it2).src1,(*it2).tgt);
            }

            // Add nodes to SCCGraphs
            for(i = 0; i < n;i++) {
              int j = intra_graph_uf->find(i);
              if(nodes[j].sccgr == NULL){
                nodes[j].sccgr = new SCCGraph();
                scc_gr_list.push_back(nodes[j].sccgr);
              }
              nodes[i].sccgr = nodes[j].sccgr;
              nodes[i].sccgr->nodes.push_back(i);
            }
            // Add edges tp SCCGraphs
            for(it = intra_edges.begin(); it != intra_edges.end(); it++)
              nodes[(*it).src].sccgr->intraEdges.push_back(*it);
            for(it2 = inter_edges.begin(); it2 != inter_edges.end(); it2++)
              nodes[(*it2).tgt].sccgr->interEdges.push_back(*it2);
            // Also populate inter graph maps for the scc
            for (it2 = inter_edges.begin(); it2 != inter_edges.end(); ++it2)
              nodes[(*it2).src2].sccgr->nextGraphs.push_back(nodes[(*it2).tgt].sccgr);


            // Do SCC decomposition 
            max_scc_required = SCCLight(scc_gr_list, gr_sorted);
            STAT(stats.ncomponents = max_scc_required);
          }

          //Setup weights so that everything is tensored
          sem_elem_tensor_t sem_old = boost::polymorphic_downcast<SemElemTensor*>(sem.get_ptr());
          sem = tensorSetUpFP(sem_old, sem_old);
#if defined(PPP_DBG) && PPP_DBG >= 0
          long totCombines=0, totExtends=0, totStars=0;
          unsigned maxNewtonRounds = 0;
          unsigned totNewtonRounds = 0;
#endif


          // For each SCC, solve completely using Newton's method.
          {
            SCCGraphs::iterator gr_it = gr_sorted.begin();
            sem_elem_tensor_t one = boost::polymorphic_downcast<SemElemTensor*>(sem_old->one().get_ptr());
            sem_elem_tensor_t zerot = boost::polymorphic_downcast<SemElemTensor*>((sem->zero()).get_ptr()); //sem is tensored
            sem_elem_tensor_t zero = boost::polymorphic_downcast<SemElemTensor*>((sem_old->zero()).get_ptr());
            for(unsigned scc_n = 1; scc_n <= max_scc_required; scc_n++) {
              ////////////////We will now create the Newton IntraGraph which will store the
              ////////////////actual weights, and from which RegExp will be generated.
              ////////////////This is the TDG for the linearized problem for the current SCC

              SCCGraphs::iterator scc_head = gr_it;
              IntraGraph * graph = new IntraGraph(dag, false, sem); //pre = false
              linear_gr_list.push_back(graph);
              // (2) Add nodes to IntraGraph
              while(gr_it != gr_sorted.end() && (*gr_it)->scc_number == scc_n){
                scc_graph_t gr = *gr_it;
                for(vector<int>::iterator iter = gr->nodes.begin(); iter != gr->nodes.end(); ++iter){
                  int i = *iter;
                  nodes[i].intra_nodeno = graph->makeNode(nodes[i].trans);
                  nodes[i].gr = graph;
                }
                gr_it++;
              }

              //Reset gr_it
              gr_it = scc_head;
              // (3) Determine if this SCC has a recursive call
              bool isRecursive = false;
              while(gr_it != gr_sorted.end() && (*gr_it)->scc_number == scc_n){
                scc_graph_t gr = *gr_it;
                for(vector<HyperEdge>::iterator iter = gr->interEdges.begin(); iter != gr->interEdges.end(); ++iter){
                  if(nodes[iter->src2].gr == graph){
                    isRecursive = true;
                    break;
                  }                    
                }
                if(isRecursive)
                  break;
                gr_it++;
              }

              // Will the current graph have tensored weights?
              // We need to know this for weight queries on the graph after saturation.
              graph->hasTensoredWeights = isRecursive;
              
              //Reset gr_it
              gr_it = scc_head;
              // Now add in the myriad edges.
              if(isRecursive){
                //Do Newton Magic
                dag->startSatProcess(sem); //Creates a new context for RegEx
                while(gr_it != gr_sorted.end() && (*gr_it)->scc_number == scc_n){
                  scc_graph_t gr = *gr_it;

                  // (4.2.1) Source nodes:
                  for(vector<int>::iterator iter = gr->nodes.begin(); iter != gr->nodes.end(); ++iter){
                    int i = *iter;
                    if(is_source_type(nodes[i].type)) {
                      //This is a source node. 
                      //Create an immutable edge with weight:
                      //Post*:  w -> (w,1^T)
                      sem_elem_tensor_t wt = boost::polymorphic_downcast<SemElemTensor*>((nodes[i].weight).get_ptr());
                      sem_elem_tensor_t one = boost::polymorphic_downcast<SemElemTensor*>((wt->one()).get_ptr());
                      wt = tensorSetUpFP(wt,one);
                      graph->setSource(nodes[i].intra_nodeno, wt);
                    }
                    // zero all weights (some are set by InterGraph::setSource() )
                    if(nodes[i].weight.get_ptr() != NULL)
                      nodes[i].weight = zerot;
                  }

                  // (4.2.2) Intra Edges:
                  for(vector<GraphEdge>::iterator iter = gr->intraEdges.begin(); iter != gr->intraEdges.end(); iter++){
                    //This is an edge (src--w-->tgt)
                    //Add an immutable edge src--w'-->tgt)
                    // w' = (w,1^T)
                    sem_elem_tensor_t wt = boost::polymorphic_downcast<SemElemTensor*>((iter->weight).get_ptr());
                    sem_elem_tensor_t one = boost::polymorphic_downcast<SemElemTensor*>((wt->one()).get_ptr());
                    wt = tensorSetUpFP(wt,one);
                    graph->addEdge(nodes[iter->src].intra_nodeno, nodes[iter->tgt].intra_nodeno, wt);
#if 0
                    //Also add a mutable edge (s--f-->tgt) from the source vertex s with weight 0 (tensored) and
                    //functional f = (DetTrans(wt(src)) x w,1^T) (one untensored)
                    sem_elem_tensor_t zerot = boost::polymorphic_downcast<SemElemTensor*>(sem->zero().get_ptr());
                    wt = boost::polymorphic_downcast<SemElemTensor*>((iter->weight).get_ptr());
                    functional_t f = 
                      SemElemFunctional::tensor(
                          SemElemFunctional::extend(
                            SemElemFunctional::detensorTranspose(
                              SemElemFunctional::in(nodes[iter->src].intra_nodeno)),
                            SemElemFunctional::constant(wt)),
                          SemElemFunctional::constant(one));
                    int e = graph->setSource(nodes[iter->tgt].intra_nodeno, zerot, f);
                    // Back references in the node to edges that depend on it.
                    graph->addDependentEdge(e, nodes[iter->src].intra_nodeno);                  
#endif
                  }
                  int trash; 
                  // Inter Edges:
                  for(vector<HyperEdge>::iterator iter = gr->interEdges.begin(); iter != gr->interEdges.end(); ++iter){

                    //Obtain the weight on the call edge
                    assert(eHandler.exists(iter->src1));                    
                    sem_elem_tensor_t wtCallRule =
                      boost::polymorphic_downcast<SemElemTensor*>(eHandler.get_dependency(iter->src1,trash).get_ptr());
                    assert(trash != -1);

                    // src2 is the external source for a hyperedge
                    if(nodes[iter->src2].gr != NULL && nodes[iter->src2].gr != graph){
                      // (4.2.3.1) The entry node belongs to a graph that has a lower scc.
                      // Treat this case as an intraedge


                      //Add an immutable edge src1--w'-->tgt)
                      // If the graph for src2 has tensoredWeights, then
                      // w' = (wtCallRule x DetTrans(wt(src2)), 1^T)
                      // If it has base weights, then
                      // w' = (wtCallRule x wt(src2), 1^T)
		      // ET: Merge Functions?
                      sem_elem_t wtsrc2 = nodes[iter->src2].gr->getWeight(nodes[iter->src2].intra_nodeno);
                      sem_elem_tensor_t wt = boost::polymorphic_downcast<SemElemTensor*>(wtsrc2.get_ptr());
                      if(nodes[iter->src2].gr->hasTensoredWeights)
                        wt = wt->detensorTranspose();
                      wt = boost::polymorphic_downcast<SemElemTensor*>(wtCallRule->extend(wt.get_ptr()).get_ptr());
                      sem_elem_tensor_t one = boost::polymorphic_downcast<SemElemTensor*>((wt->one()).get_ptr());                                     
                      graph->addEdge(nodes[iter->src1].intra_nodeno, nodes[iter->tgt].intra_nodeno,
                          tensorSetUpFP(wt,one));
#if 0
                      //Also add a mutable edge (s--f-->tgt) from the source vertex s with weight 0 (tensored) and
                      //if graph for src2 has tensored weights, then
                      //functional f = (DetTrans(wt(src1)) x (Constant(wtCallRule) x DetTrans(wt(src2))), 1^T) (one untensored)
                      //else
                      //functional f = (DetTrans(wt(src1)) x (Constant(wtCallRule) x DetTrans(wt(src2))), 1^T) (one untensored)
                      sem_elem_tensor_t zerot = boost::polymorphic_downcast<SemElemTensor*>(sem->zero().get_ptr());
                      functional_t f = 
                        SemElemFunctional::tensor(
                            SemElemFunctional::extend(
                              SemElemFunctional::detensorTranspose(
                                SemElemFunctional::in(nodes[iter->src1].intra_nodeno)),
                              SemElemFunctional::constant(wt)),
                            SemElemFunctional::constant(one));
                      int e = graph->setSource(nodes[iter->tgt].intra_nodeno, zerot, f);                    
                      // Back references in the node to edges that depend on it.
                      graph->addDependentEdge(e, nodes[iter->src1].intra_nodeno);
#endif
                    }else{
                      // (4.2.3.2) The entry node does not belong to a graph that has a lower scc.
                      // Note: It must belong to the same scc then
                      assert(nodes[iter->src2].gr == graph);

                      // Add mutable edge src1--f-->tgt with weight 0 (tensored) and
                      // f = (Constant(callWt) x DetTrans(wt(src2)), 1^T)
                      sem_elem_tensor_t zerot = boost::polymorphic_downcast<SemElemTensor*>(sem->zero().get_ptr());
                      functional_t f = 
                        SemElemFunctional::tensor(
                            SemElemFunctional::extend(
                              SemElemFunctional::constant(wtCallRule),
                              SemElemFunctional::detensorTranspose(
                                SemElemFunctional::in(nodes[iter->src2].intra_nodeno))),
                            SemElemFunctional::constant(one));
                      int e = graph->addEdge(nodes[iter->src1].intra_nodeno, nodes[iter->tgt].intra_nodeno, zerot, true, f);
                      // Back references in the node to edges that depend on it.
                      graph->addDependentEdge(e, nodes[iter->src2].intra_nodeno);

                      // Add mutable edge src2--f-->tgt with weight 0 (tensored) and
                      // f = (1, (DetTrans(wt(src1)) x callWt)^T)
                      f = 
                        SemElemFunctional::tensor(
                            SemElemFunctional::constant(one),
                            SemElemFunctional::transpose(
                              SemElemFunctional::extend(
                                SemElemFunctional::detensorTranspose(
                                  SemElemFunctional::in(nodes[iter->src1].intra_nodeno)),
                                SemElemFunctional::constant(wtCallRule))));
                      e = graph->addEdge(nodes[iter->src2].intra_nodeno, nodes[iter->tgt].intra_nodeno, zerot, true, f);
                      // Back references in the node to edges that depend on it.
                      graph->addDependentEdge(e, nodes[iter->src1].intra_nodeno);

                      // Add mutable edge s--f-->tgt from source node s with weight 0(tensored) and
                      // f = (DetTrans(wt(src1)) x callWt x DetTrans(wt(src2)), 1^T)
                      f =
                        SemElemFunctional::tensor(
                            SemElemFunctional::extend(
                              SemElemFunctional::extend(                                
                                SemElemFunctional::detensorTranspose(
                                  SemElemFunctional::in(nodes[iter->src1].intra_nodeno)),
                                SemElemFunctional::constant(wtCallRule)),
                              SemElemFunctional::detensorTranspose(
                                SemElemFunctional::in(nodes[iter->src2].intra_nodeno))),
                            SemElemFunctional::constant(one));
                      e = graph->setSource(nodes[iter->tgt].intra_nodeno, zerot, f); //ET: What does set source do? Why not add edge like in the two other cases?
                      // Back references in the node to edges that depend on it.
                      graph->addDependentEdge(e, nodes[iter->src1].intra_nodeno);
                      graph->addDependentEdge(e, nodes[iter->src2].intra_nodeno);
                    }
                  }

                  gr_it++;
                }              
              }
              else
              {
                //Do Fwpds Magic
                dag->startSatProcess(sem_old); //non tensored
                while(gr_it != gr_sorted.end() && (*gr_it)->scc_number == scc_n){
                  scc_graph_t gr = *gr_it;

                  // (4.1.1)
                  // Source nodes:
                  for(vector<int>::iterator iter = gr->nodes.begin(); iter != gr->nodes.end(); ++iter){
                    int i = *iter;
                    if(is_source_type(nodes[i].type)) {
                      //This is a source node. 
                      //Create an immutable edge with weight w
                      sem_elem_t wt = nodes[i].weight;
                      graph->setSource(nodes[i].intra_nodeno, wt);
                    }
                    // zero all weights (some are set by InterGraph::setSource() )
                    if(nodes[i].weight.get_ptr() != NULL)
                      nodes[i].weight = zero;
                  }

                  // (4.1.2)
                  // Intra Edges:
                  for(vector<GraphEdge>::iterator iter = gr->intraEdges.begin(); iter != gr->intraEdges.end(); iter++){
                    //This is an edge (src--w-->tgt)
                    //Add an immutable edge src--w-->tgt)
                    sem_elem_t wt = iter->weight;
                    graph->addEdge(nodes[iter->src].intra_nodeno, nodes[iter->tgt].intra_nodeno, wt);
                  }
                  int trash;
                  // (4.1.3) 
                  // Inter Edges:
                  for(vector<HyperEdge>::iterator iter = gr->interEdges.begin(); iter != gr->interEdges.end(); ++iter){
                    // Obtain the weight on the call edge
                    assert(eHandler.exists(iter->src1));                    
                    sem_elem_tensor_t wtCallRule =
                      boost::polymorphic_downcast<SemElemTensor*>(eHandler.get_dependency(iter->src1,trash).get_ptr());
                    assert(trash != -1);

                    // src2 is the external source for a hyperedge. This can't be recursive, hence --
                    assert(nodes[iter->src2].gr != NULL && nodes[iter->src2].gr != graph);
                    // Treat this case as an intraedge

                    // Add an immutable edge src1--w'-->tgt)
                    // If the graph for src2 has tensoredWeights, then
                    // w' = wtCallRule x DetTrans(wt(src2))
                    // else
                    // w' = wtCallRule x wt(src2)
                    sem_elem_t wtsrc2 = nodes[iter->src2].gr->getWeight(nodes[iter->src2].intra_nodeno);
                    sem_elem_tensor_t wt = boost::polymorphic_downcast<SemElemTensor*>(wtsrc2.get_ptr());
                    if(nodes[iter->src2].gr->hasTensoredWeights)
                      wt = wt->detensorTranspose();
                    wt = boost::polymorphic_downcast<SemElemTensor*>(wtCallRule->extend(wt.get_ptr()).get_ptr());
                    graph->addEdge(nodes[iter->src1].intra_nodeno, nodes[iter->tgt].intra_nodeno, wt);
                  }
                  gr_it++;
                }              
              }
              // (5)
              // Use Tarjan's path listing algorithm to generate regular expressions for nodes.
              graph->setupIntraSolution();

#if defined(PPP_DBG) && PPP_DBG >= 1
              // We have the intra graph ready at this point. 
                // DEBUGGING
                std::stringstream ss;
                ss << "graph" << scc_n << ".dot";
                std::ofstream foo(ss.str().c_str());
                string dot = graph->toDot();
                foo << "digraph {\n";
                foo << dot;
                foo << "}";
                foo.close();
              cout << "GRAPH " << scc_n << "\n";
#endif 
              // (6) Now solve the linearized problem by saturating.
              unsigned numRounds = 0;
              graph->saturate(numRounds);
#if defined(PPP_DBG) && PPP_DBG >= 0
              maxNewtonRounds = numRounds > maxNewtonRounds ? numRounds : maxNewtonRounds;
              totNewtonRounds += numRounds;
#endif
              // The next SCC will use another sat process phase.
              dag->stopSatProcess();
            }
          }

          // (7)
          // It is time to decide whether the output automaton should have
          // tensored weights or not.
          // Currently, we will decide by counting the number of edges in the
          // output automaton (i.e. the number of IntraGraph nodes that have
          // tensored/non-tensored weights, and then switching at some
          // ciritical ratio (decided emprically)
          int totTensoredNodes=0, totNonTensoredNodes=0;
          for(std::list<IntraGraph*>::iterator gr_it = linear_gr_list.begin(); gr_it != linear_gr_list.end(); ++gr_it){
            IntraGraph * graph = *gr_it;
            if(graph->hasTensoredWeights)
              totTensoredNodes += graph->nnodes;
            else
              totNonTensoredNodes += graph->nnodes;
          }
          if(totTensoredNodes == 0 || ((float)totNonTensoredNodes)/((float)totTensoredNodes) > 0.33)
            isOutputAutomatonTensored = true;
          //XXX:HACK
          isOutputAutomatonTensored = true; //override
          cerr << "Warning: overriding to OutputAutomatonTensored\n";

          if(isOutputAutomatonTensored){
            // Before saying you're done, tensor the weights lying around as call rule weights so that
            // path summary will see tensored weights.
            eHandler.tensorAllWeights();
          }

#if defined(PPP_DBG) && PPP_DBG >= 0
          dag->sanitizeRootsAcrossSatProcesses();
          totCombines = dag->countTotalCombines();
          totExtends = dag->countTotalExtends();
          totStars = dag->countTotalStars();

          cout << "Maximum number of Newton rounds: " << maxNewtonRounds << endl;
          cout << "Total number of Newton rounds: " << totNewtonRounds << endl;
          cout << "Total number of combines: " << totCombines << endl;
          cout << "Total number of Extends: " << totExtends << endl;
          cout << "Total number of Stars: " << totStars << endl;

          dag->printStructureInformation();
#endif
          max_scc_computed = max_scc_required;
          dag->executingPoststar(!running_prestar);

#if defined(PPP_DBG) && PPP_DBG >= 2
            std::stringstream ss;
            ss << "digraph{\n";
            for(i=0; i<n; i++){
              ss << "node" << i << " [label=\"[intergraphno: " << i << "](" <<
                key2str(nodes[i].trans.src) <<
                ", " << key2str(nodes[i].trans.stack) <<
                ", " << key2str(nodes[i].trans.tgt) << ")\\n";
              sem_elem_t wt =nodes[i].gr->get_weight(nodes[i].intra_nodeno); 
              if(wt == NULL)
                pp_weight(ss, wt);
              else{
                sem_elem_tensor_t wtt =
                  boost::polymorphic_downcast<SemElemTensor*>(wt.get_ptr());
                pp_weight(ss, wtt->detensorTranspose());
              }
              ss<< "\"];\n";
            }
            for(vector<GraphEdge>::iterator it = intra_edges.begin(); it != intra_edges.end(); ++it){
              ss << "node" << it->src << " -> node" << it->tgt 
                << " [label=\"";
              pp_weight(ss, it->weight);
              ss  << "\"];\n";
            }
            for(vector<HyperEdge>::iterator it2 = inter_edges.begin(); it2 != inter_edges.end(); ++it2){
              ss << "node" << it2->src1 << " -> node" << it2->tgt 
                << " [label=\"";
              pp_weight(ss, it2->weight);
              ss << "\" color=brown];\n";
              if(it2->mf.get_ptr()){ 
                ss << "node" << it2->src2 << " -> node" << it2->tgt 
                  << " [color=red, label=\"Has Merge Fn\"];\n";
              }else{
                ss << "node" << it2->src2 << " -> node" << it2->tgt 
                  << " [color=red, label=\"No Merge Fn\"];\n";
              }
            }
            ss << "}\n";
            ofstream foo("tdg.dot");
            foo << ss.str();
            foo.close();
#endif 
        }

	double InterGraph::getOutnodeRegExps(map<int,reg_exp_t>& outNodeRegExps, map<int,int>& uMap, map<int,int> & oMap, map<int,std::pair< std::pair<int,int>,int> >& mapBack, vector<int>& eps, bool first)
	{
	  wali::util::GoodTimer * t = new wali::util::GoodTimer("graphTime");
	  //t->start();
	  dag->startSatProcess(sem);
           // First, find the IntraGraphs
          int n = nodes.size();
          int i;
          intra_graph_uf = new UnionFind(n);

          vector<GraphEdge>::iterator it;
          vector<HyperEdge>::iterator it2;
          std::list<IntraGraph *>::iterator gr_it;
          multiset<tup > worklist;


          //These two loops Group together all the nodes in each procedure
          for(it = intra_edges.begin(); it != intra_edges.end(); it++) {
            intra_graph_uf->takeUnion((*it).src,(*it).tgt);
          }
          for(it2 = inter_edges.begin(); it2 != inter_edges.end(); it2++) {
            intra_graph_uf->takeUnion((*it2).src1,(*it2).tgt);
          }
          IntraGraph::SharedMemBuffer * memBuf = NULL;
#ifdef INTRAGRAPH_SHARED_MEMORY
          // Before creating IntraGraphs, create a CommonBuffer, if needed.
          int max_size = 0;
          for(gr_it = gr_list.begin(); gr_it != gr_list.end(); gr_it++) {
            max_size = (max_size > (*gr_it)->getSize()) ? max_size : (*gr_it)->getSize();
          }
          memBuf  = new IntraGraph::SharedMemBuffer(max_size);
#endif

          for(i = 0; i < n;i++) {
            int j = intra_graph_uf->find(i);
            if(nodes[j].gr == NULL) {
              nodes[j].gr = new IntraGraph(dag, running_prestar,sem, memBuf);
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
            int eno = gr->addEdge(nodes[(*it2).src1].intra_nodeno, nodes[(*it2).tgt].intra_nodeno, sem->zero(), true);
            int uno = gr->edges[eno].updatable_no;
	    uMap[uno] = (*it2).src2;
	    IntraGraph *gr2 = nodes[(*it2).src2].gr;
            gr2->setOutNode(nodes[(*it2).src2].intra_nodeno, (*it2).src2);
	    //cout << "Src1: " << (*it2).src1 << "\t Src2: " << (*it2).src2 << "\t Tgt: " << (*it2).tgt << "\n";
          }

          // For SWPDS
          vector<call_edge_t>::iterator it3;
          for(it3 = call_edges.begin(); it3 != call_edges.end(); it3++) {
            IntraGraph *gr1 = nodes[(*it3).first].gr;
            IntraGraph *gr2 = nodes[(*it3).second].gr;
            gr1->addCallEdge(gr2);
          }

	  int index = 1;

          // Setup Worklist
          for(gr_it = gr_list.begin(); gr_it != gr_list.end(); gr_it++) {
            (*gr_it)->setupIntraSolution(false);
	    t->stop();
            for(vector<IntraGraphNode>::const_iterator nit = (*gr_it)->nodes.begin(); nit != (*gr_it)->nodes.end(); ++nit){
	      if(((*nit).node_no != -1) && ((*nit).node_no != 0))
              {
		if(!first)  
		{
                  int label = std::atoi(key2str((*nit).trans.stack).c_str());
                  if(label != 0) {
                    outNodeRegExps[label] = (*nit).regexp;
                  }
                }
		else 
		{
		  int intraNum = (*nit).node_no;
		  int tgt = (*nit).trans.tgt;
		  int stack = (*nit).trans.stack;
		  int src = (*nit).trans.src;
		  mapBack[index] = std::make_pair(std::make_pair(src,tgt),stack);
		  outNodeRegExps[index] = (*nit).regexp;
		  if(stack == 0 && tgt != 0)
		  {
		    eps.push_back(index);
		    if((*nit).type == 3)
		    {
		      list<int>::const_iterator dit = (*gr_it)->out_nodes_intra->begin();
		      for(list<int>::const_iterator cit = (*gr_it)->out_nodes_inter->begin(); cit != (*gr_it)->out_nodes_inter->end(); cit++)
		      {
		        if((*dit) == intraNum)
			{
			  oMap[(*cit)] = index;
			}
			dit++;
		      }
		    }
		  }
		  index++;
		}
	      }
	    }
	    t->start();
	  }
	  t->stop();
	  double totTime = t->total_time();
	  delete t;
	  return totTime;
	}

        // If an argument is passed in then only weights on those transitions will be available
        // I can fix this (i.e., weights for others will be available on demand), but not right now.
        void InterGraph::setupInterSolution(std::list<Transition> *wt_required) {
          dag->startSatProcess(sem);
          // First, find the IntraGraphs
          int n = nodes.size();
          int i;
          unsigned int max_scc_required;
          intra_graph_uf = new UnionFind(n);

          vector<GraphEdge>::iterator it;
          vector<HyperEdge>::iterator it2;
          std::list<IntraGraph *>::iterator gr_it;
          multiset<tup > worklist;


          //These two takeUnions Group together all the nodes in a single procedure
          for(it = intra_edges.begin(); it != intra_edges.end(); it++) {
            intra_graph_uf->takeUnion((*it).src,(*it).tgt);
          }
          for(it2 = inter_edges.begin(); it2 != inter_edges.end(); it2++) {
            intra_graph_uf->takeUnion((*it2).src1,(*it2).tgt);
          }
          IntraGraph::SharedMemBuffer * memBuf = NULL;
#ifdef INTRAGRAPH_SHARED_MEMORY
          // Before creating IntraGraphs, create a CommonBuffer, if needed.
          int max_size = 0;
          for(gr_it = gr_list.begin(); gr_it != gr_list.end(); gr_it++) {
            max_size = (max_size > (*gr_it)->getSize()) ? max_size : (*gr_it)->getSize();
          }
          memBuf  = new IntraGraph::SharedMemBuffer(max_size);
#endif

          for(i = 0; i < n;i++) {
            int j = intra_graph_uf->find(i);
            if(nodes[j].gr == NULL) {
              nodes[j].gr = new IntraGraph(dag, running_prestar,sem, memBuf);
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
#if defined(PPP_DBG) && PPP_DBG >= 0
          vector<reg_exp_t> outNodeRegExps;
#endif
          for(gr_it = gr_list.begin(); gr_it != gr_list.end(); gr_it++) {
            (*gr_it)->setupIntraSolution(false);
#if defined(PPP_DBG) && PPP_DBG >= 0
            for(list<int>::const_iterator cit = (*gr_it)->out_nodes_intra->begin(); cit != (*gr_it)->out_nodes_intra->end(); ++cit)
              outNodeRegExps.push_back((*gr_it)->nodes[*cit].regexp);
#endif
          }

#if defined(PPP_DBG) && PPP_DBG >= 0
          long totNodes = 0, totNotComputed = 0;
          totNodes = dag->countTotalCombines() +
            dag->countTotalExtends() +
            dag->countTotalStars();
          totNotComputed = dag->countExcept(outNodeRegExps); 
          cout << "Total number of combines: " << dag->countTotalCombines() << endl;
          cout << "Total number of Extends: " << dag->countTotalExtends() << endl;
          cout << "Total number of Stars: " << dag->countTotalStars() << endl;
          cout << "#nodes definitely not evaluated during saturation: " << dag->countExcept(outNodeRegExps) << endl;
          cout << "%Nodes never computed > " << 100 * (double)(((double)totNotComputed)/ (double)totNodes) << endl;;
#endif
          // Do SCC decomposition of IntraGraphs
          std::list<IntraGraph *> gr_sorted;
          unsigned components = SCC(gr_list, gr_sorted);
          STAT(stats.ncomponents = components);

          int numSteps = 0;
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
          gr_it = gr_sorted.begin();
          for(unsigned scc_n = 1; scc_n <= max_scc_required; scc_n++) {
            bfsIntra(*gr_it, scc_n);
            setup_worklist(gr_sorted, gr_it, scc_n, worklist);
            numSteps += saturate(worklist,scc_n);
          }
#if defined(PPP_DBG) && PPP_DBG >= 0
          cout << "Total number of steps: " << numSteps << endl;
#endif
        max_scc_computed = max_scc_required;

        //DEBUGGING
#if defined(PPP_DBG) && PPP_DBG >= 1
        {
          stringstream ss;
          ss << "kleene_regexp.dot";
          string filename = ss.str();
          fstream foo;
          foo.open(filename.c_str(), fstream::out);
          const reg_exp_hash_t& roots = dag->getRoots();
          foo << "digraph {\n";
          std::set<long> seen;
          for(reg_exp_hash_t::const_iterator iter = roots.begin();
              iter != roots.end();
              ++iter){
            (iter->second)->toDot(foo, seen, true, true);
          }
          foo << "}\n";
          foo.close();
        }
        int graphnum = 0;
        for(gr_it = gr_list.begin(); gr_it != gr_list.end(); ++gr_it)
        {
          ++graphnum;
          stringstream ss;
          ss << "kleene_graph_" << graphnum << ".dot";
          string filename = ss.str();
          fstream foo;
          foo.open(filename.c_str(), fstream::out);
          foo << "digraph{\n";
          foo << (*gr_it)->toDot();
          foo << "}";
          foo.close();
        }
#endif
        dag->stopSatProcess();
        dag->executingPoststar(!running_prestar);
#ifdef INTRAGRAPH_SHARED_MEMORY
        delete memBuf;
#endif
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
      int changestat = dag->out_node_height(reg_equations);
      total_stats.nhyperedges = inter_edges.size();
      total_stats.nedges = intra_edges.size();
      total_stats.nnodes = nodes.size();

      RegExpStats rst = dag->get_stats();
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
    int InterGraph::saturate(multiset<tup> &worklist, unsigned scc_n) {
      int numSteps = 0;
      sem_elem_t weight;
      std::list<int> *moutnodes;

      while(!worklist.empty()) {
        // Get an outnode whose weight is to be propagated
        multiset<tup>::iterator wit = worklist.begin();
        int onode = (*wit).second;
        worklist.erase(wit);
        //int onode = worklist.front();
        //worklist.pop_front();

        numSteps++;
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
	    cout << "Extending weight ";
	    inter_edges[*beg].weight->print(cout) << " with ";
	    weight->print(cout) << "\n";
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
      //DEBUGGING
      //cout << "Kleene saturation # Steps: " << numSteps << endl;
      return numSteps;

    }

    // Must be called after saturation
    sem_elem_t InterGraph::get_call_weight(Transition t) {
      unsigned orig_size = nodes.size();
      unsigned n = nodeno(t);
      assert(orig_size == nodes.size()); // Transition t must not be a new one

      if(newtonGr)
        return newtonGr->get_weight(nodes[n].intra_nodeno);
      else
        return nodes[n].gr->get_weight(nodes[n].intra_nodeno);
    }

    sem_elem_t InterGraph::get_weight(Transition t){
      unsigned orig_size = nodes.size();
      unsigned n = nodeno(t);
      assert(orig_size == nodes.size()); // Transition t must not be a new one
      return get_weight(n);
    }

    // Changed for Newton Solver.  
    // When the output automaton is tensored (@see comment above setupNewtonSolution), make sure
    // that the returned weight is also tensored.  
    // When the output automaton is non-tensored, make sure that the returned weight is
    // non-tensored. 
    sem_elem_t InterGraph::get_weight(unsigned n) 
    {
      // check eHandler
      if(eHandler.exists(n)) {
        // This must be a return transition
        int nc;
        sem_elem_t wtCallRule = eHandler.get_dependency(n, nc);
        sem_elem_t wt;
        if(nc != -1) {
          wt = nodes[nc].gr->get_weight(nodes[nc].intra_nodeno);
          sem_elem_tensor_t twt = dynamic_cast<SemElemTensor*>(wt.get_ptr());
          if(twt != NULL){
            if(isOutputAutomatonTensored && ! nodes[nc].gr->hasTensoredWeights)
              wt = tensorSetUpFP(twt,boost::polymorphic_downcast<SemElemTensor*>(twt->one().get_ptr()));
            if(! isOutputAutomatonTensored && nodes[nc].gr->hasTensoredWeights)
              wt = twt->detensorTranspose();
          }
        } else {
          // ESource
          wt = wtCallRule->one();
        }
        return wt->extend(wtCallRule.get_ptr());
      }
      sem_elem_t wt = nodes[n].gr->get_weight(nodes[n].intra_nodeno);
      sem_elem_tensor_t twt = dynamic_cast<SemElemTensor*>(wt.get_ptr());
      if(twt != NULL){
        if(isOutputAutomatonTensored && ! nodes[n].gr->hasTensoredWeights)
          wt = tensorSetUpFP(twt,boost::polymorphic_downcast<SemElemTensor*>(twt->one().get_ptr()));
        if(! isOutputAutomatonTensored && nodes[n].gr->hasTensoredWeights)
          wt = twt->detensorTranspose();
      }
      return wt;
    }


        void InterGraph::setupNewtonNoTensorSolution() {
          dag->startSatProcess(sem);
          // First, find the IntraGraphs
          int n = nodes.size();
          int i;
          unsigned int max_scc_required;
          intra_graph_uf = new UnionFind(n);

          vector<GraphEdge>::iterator it;
          vector<HyperEdge>::iterator it2;
          std::list<IntraGraph *>::iterator gr_it;
          multiset<tup > worklist;


          for(it = intra_edges.begin(); it != intra_edges.end(); it++) {
            intra_graph_uf->takeUnion((*it).src,(*it).tgt);
          }
          for(it2 = inter_edges.begin(); it2 != inter_edges.end(); it2++) {
            intra_graph_uf->takeUnion((*it2).src1,(*it2).tgt);
          }
          IntraGraph::SharedMemBuffer * memBuf = NULL;
#ifdef INTRAGRAPH_SHARED_MEMORY
          // Before creating IntraGraphs, create a CommonBuffer, if needed.
          int max_size = 0;
          for(gr_it = gr_list.begin(); gr_it != gr_list.end(); gr_it++) {
            max_size = (max_size > (*gr_it)->getSize()) ? max_size : (*gr_it)->getSize();
          }
          memBuf  = new IntraGraph::SharedMemBuffer(max_size);
#endif

          for(i = 0; i < n;i++) {
            int j = intra_graph_uf->find(i);
            if(nodes[j].gr == NULL) {
              nodes[j].gr = new IntraGraph(dag, running_prestar,sem, memBuf);
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
            // R = C . M(1,x) -- Edge weight M(1,x) to be updated between Newton Rounds
            // Add a mutable intra-edge C-->R with weight M(1,x); the weight will be updated between newton rounds.
            gr->addEdge(nodes[(*it2).src1].intra_nodeno, nodes[(*it2).tgt].intra_nodeno, (*it2).mf->apply_f(sem->one(),sem->zero()), true);
            // R = c . M(1,X) + c . M(1,x), to be updated both within and between Newton Rounds
            // Add edge from the source s-->R weight weight c . M(1,X) + c . M(1,x). The first term will be updated within each newton round, the
            // second term will be updated between newton rounds.
            gr->setSource(nodes[it2->tgt].intra_nodeno, sem->zero(), NULL); //updatable
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
#if defined(PPP_DBG) && PPP_DBG >= 0
          vector<reg_exp_t> outNodeRegExps;
#endif
          for(gr_it = gr_list.begin(); gr_it != gr_list.end(); gr_it++) {
            (*gr_it)->setupIntraSolution(false);
#if defined(PPP_DBG) && PPP_DBG >= 0
            for(list<int>::const_iterator cit = (*gr_it)->out_nodes_intra->begin(); cit != (*gr_it)->out_nodes_intra->end(); ++cit)
              outNodeRegExps.push_back((*gr_it)->nodes[*cit].regexp);
#endif
          }

#if defined(PPP_DBG) && PPP_DBG >= 0
          long totNodes = 0, totNotComputed = 0;
          totNodes = dag->countTotalCombines() +
            dag->countTotalExtends() +
            dag->countTotalStars();
          totNotComputed = dag->countExcept(outNodeRegExps); 
          cout << "Total number of combines: " << dag->countTotalCombines() << endl;
          cout << "Total number of Extends: " << dag->countTotalExtends() << endl;
          cout << "Total number of Stars: " << dag->countTotalStars() << endl;
          cout << "#nodes definitely not evaluated during saturation: " << dag->countExcept(outNodeRegExps) << endl;
          cout << "%Nodes never computed > " << 100 * (double)(((double)totNotComputed)/ (double)totNodes) << endl;;
#endif
          // Do SCC decomposition of IntraGraphs
          std::list<IntraGraph *> gr_sorted;
          unsigned components = SCC(gr_list, gr_sorted);
          STAT(stats.ncomponents = components);

          int numSteps = 0;
          // Saturate
          max_scc_required = components;
          gr_it = gr_sorted.begin();
          for(unsigned scc_n = 1; scc_n <= max_scc_required; scc_n++) {
            bfsIntra(*gr_it, scc_n);
            setup_worklist(gr_sorted, gr_it, scc_n, worklist);
            numSteps += saturateNewtonNoTensor(worklist,scc_n);
          }
#if defined(PPP_DBG) && PPP_DBG >= 0
          cout << "Total number of steps: " << numSteps << endl;
#endif
        max_scc_computed = max_scc_required;

        //DEBUGGING
#if defined(PPP_DBG) && PPP_DBG >= 1
        {
          stringstream ss;
          ss << "kleene_regexp.dot";
          string filename = ss.str();
          fstream foo;
          foo.open(filename.c_str(), fstream::out);
          const reg_exp_hash_t& roots = dag->getRoots();
          foo << "digraph {\n";
          std::set<long> seen;
          for(reg_exp_hash_t::const_iterator iter = roots.begin();
              iter != roots.end();
              ++iter){
            (iter->second)->toDot(foo, seen, true, true);
          }
          foo << "}\n";
          foo.close();
        }
        int graphnum = 0;
        for(gr_it = gr_list.begin(); gr_it != gr_list.end(); ++gr_it)
        {
          ++graphnum;
          stringstream ss;
          ss << "kleene_graph_" << graphnum << ".dot";
          string filename = ss.str();
          fstream foo;
          foo.open(filename.c_str(), fstream::out);
          foo << "digraph{\n";
          foo << (*gr_it)->toDot();
          foo << "}";
          foo.close();
        }
#endif
        dag->stopSatProcess();
        dag->executingPoststar(!running_prestar);
#ifdef INTRAGRAPH_SHARED_MEMORY
        delete memBuf;
#endif
    }

    // New Saturation Procedure -- minimize calls to get_weight
    int InterGraph::saturateNewtonNoTensor(multiset<tup> &worklist, unsigned scc_n) {
      // This function is dumb about how it update weights *between* newton rounds
      // It simply checks all call sites for updated c/x values.
      // This could probably be made better by remembering what c/x values might have changed in the last solve of linear problem. (The inner while loop).
      //
      int numSteps = 0;
      sem_elem_t weight;
      std::list<int> *moutnodes;

      std::tr1::unordered_map<int, sem_elem_t> cwt, xwt;
      vector<HyperEdge>::iterator it2;

      // Initially store the values of c/x to compare against.
      for(it2 = inter_edges.begin(); it2 != inter_edges.end(); it2++) {
        int onode1 = it2->src1;
        int onode = it2->src2;
        cwt[onode1] = nodes[onode1].gr->get_weight(nodes[onode1].intra_nodeno);
        xwt[onode] = nodes[onode].gr->get_weight(nodes[onode].intra_nodeno);
      }

      bool change = false;
      do{
        while(!worklist.empty()) {
          // Get an outnode whose weight is to be propagated
          multiset<tup>::iterator wit = worklist.begin();
          int onode = (*wit).second;
          worklist.erase(wit);
          //int onode = worklist.front();
          //worklist.pop_front();

          numSteps++;
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
            uw = cwt[onode1]->extend(uw);
            // update R = c . M(1,X), here X has changed.
            nodes[inode].gr->updateEdgeWeight(0, nodes[inode].intra_nodeno, uw); //update source edge
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
        // Linear solve done. 
        // Update psuedo-constants.
        change = false;
        std::tr1::unordered_set<int> onodesChanged;
        for(it2 = inter_edges.begin(); it2 != inter_edges.end(); it2++) {
          bool onodeChanged = false;
          int inode = it2->tgt;
          int onode1 = it2->src1;
          int onode = it2->src2;
          sem_elem_t wt;
          sem_elem_t Cwt = nodes[onode1].gr->get_weight(nodes[onode1].intra_nodeno);
          sem_elem_t Xwt = nodes[onode].gr->get_weight(nodes[onode].intra_nodeno);
          if(! Cwt->equal(cwt[onode1])){
            cwt[onode1] = Cwt;
            onodeChanged = true;
          }
          if(! Xwt->equal(xwt[onode])){
            xwt[onode] = Xwt;
            if(running_ewpds && it2->mf.get_ptr()){
              wt = it2->mf->apply_f(Xwt->one(), Xwt);             
            }else{
              wt = it2->weight->extend(Xwt);
            }              
            // Update R = C . M(1,x); here, x has changed from the last newton round. So update the intra-edge we had added.
            nodes[inode].gr->updateEdgeWeight(nodes[onode1].intra_nodeno, nodes[inode].intra_nodeno, wt);
            onodeChanged = true;
          } 
          if(running_ewpds && it2->mf.get_ptr())
            wt = cwt[onode1]->extend(it2->mf->apply_f(cwt[onode1]->one(), xwt[onode]));
          else
            wt = cwt[onode1]->extend(it2->weight->extend(xwt[onode]));
          // Update R = c . M(1,x); here, c and/or x might have changed. Update anyway.
          nodes[inode].gr->updateEdgeWeight(0, nodes[inode].intra_nodeno, wt); 
          if(onodeChanged)
            onodesChanged.insert(onode);
          change |= onodeChanged;
        }
        // Reinitialize worklist.
        for(std::tr1::unordered_set<int>::iterator oit = onodesChanged.begin(); oit != onodesChanged.end(); ++oit) {
          int onode = *oit;
          // Go through all targets again and insert them into the workist without
          // seeing if they actually got modified or not
          std::list<int>::iterator beg = nodes[onode].out_hyper_edges.begin();
          std::list<int>::iterator end = nodes[onode].out_hyper_edges.end();
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
      }while(change);
      //DEBUGGING
      //cout << "Kleene saturation # Steps: " << numSteps << endl;
      return numSteps;
    }

    void InterGraph::update_all_weights() {
      unsigned int i;
      for(i=0;i<nodes.size();i++) {
        sem_elem_t w;
        if(newtonGr)
          w = newtonGr->get_weight(nodes[i].intra_nodeno);
        else
          w = nodes[i].gr->get_weight(nodes[i].intra_nodeno);
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
      IntraGraph *ca = new IntraGraph(dag, true, sem); // running_prestar = true because extend goes backward
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
        if(((int)nodes[i].trans.src) == state &&
           ((int)nodes[i].trans.stack) == stack)
        {
          Transition t1(state,0,0);
          Transition t2(nodes[i].trans.tgt,0,0);
          if(newtonGr)
            ca->addEdge(get_number(intra_node_map,state,ca), get_number(intra_node_map, nodes[i].trans.tgt,ca), 
                correct(newtonGr->get_weight(nodes[i].intra_nodeno).get_ptr()));
          else
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
          if(newtonGr)
            ca->addEdge(t1, t2, correct(newtonGr->get_weight(nodes[i].intra_nodeno).get_ptr()));
          else
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

