#include "wali/util/Timer.hpp"

#include "wali/graph/IntraGraph.hpp"
#include "wali/graph/LinkEval.hpp"
#include "wali/graph/GraphCommon.hpp"

#include <iostream>

using namespace std;

namespace wali {

  namespace graph {

    sem_elem_t IntraGraph::se;
#ifdef STATIC_MEMORY
    int *IntraGraph::intraGraphBuffer = 0;
    set<int> *IntraGraph::childrenBuffer = 0;
    reg_exp_t *IntraGraph::regBuffer = 0;
    int IntraGraph::intraGraphBufferSize = 0;
#endif

#define nodeno(a) (a.node_no)

    void IntraGraph::create_node(int a) {
      assert(a < nnodes);
    }

    void IntraGraph::create_node(Transition &t, int a) {
      int n = nodes.size();
      if(n <= a) nodes.resize(2*a);
      int i;
      for(i=nnodes;i<=a;i++) {
        nodes[i].set(i);
        nnodes++;
      }
      nodes[a].trans = t;
    }

    int IntraGraph::edgeno(int s, int t) {
      list<int>::iterator it = nodes[s].outgoing.begin();
      for(; it != nodes[s].outgoing.end(); it++) {
        if(edges[*it].tgt == t) {
          return *it;
        }
      }
      return -1;
    }

    ostream &operator << (ostream &out, const IntraGraphStats &s) {
      out << "Semiring combine : " << s.ncombine << "\n";
      out << "Semiring extend : " << s.nextend << "\n";
      out << "Semiring star : " << s.nstar << "\n";
      out << "Nodes : " << s.nnodes << "\n";
      out << "Edges : " << s.nedges << "\n";
      out << "Updatable : " << s.nupdatable << "\n";
      out << "Cutset : " << s.ncutset << "\n";
      return out;
    }

    ostream &IntraGraph::print_trans(Transition &t, ostream &out, PRINT_OP pop) {
      out << "(";
      pop(out, t.src) << "(" << t.src << "),";
      pop(out, t.stack) << "(" << t.stack << "),";
      pop(out, t.tgt) << "(" << t.tgt << "))";
      return out;
    }

    ostream &IntraGraph::print(ostream &out, PRINT_OP UNUSED_PARAMETER(pop)) {
      int n = nedges;
      int i;
      out << "IntraGraph:\n";
      for(i=0;i<nnodes;i++) {
        if(nodes[i].outgoing.size() == 0 && nodes[i].incoming.size() == 0) {
          //print_trans(nodes[i].trans, out, pop);
          out << i << " ";
          out << "\n";
        } else {
          //print_trans(nodes[i].trans, out, pop);
          out << i << " ";
          if(nodes[i].regexp.get_ptr() != 0) {
            nodes[i].regexp->reevaluate()->print(out);
            //nodes[i].regexp->print(out);
          }
          out << "\n";
        }
      }
      for(i=0; i < n;i++) {
        //Transition src = nodes[edges[i].src].trans;
        //Transition tgt = nodes[edges[i].tgt].trans;
        //print_trans(src, out, pop);
        out << edges[i].src << " ";
        out << "-->";
        out << edges[i].tgt << " ";
        //print_trans(tgt, out, pop);
        //edges[i].weight->print(out);
      }
      return out;
    }

    void IntraGraph::resetUpdatable() {
      vector<int>::iterator it;
      for(it = updatable_edges.begin(); it != updatable_edges.end(); it++) {
        RegExp::update(edges[*it].updatable_no,se->zero());
      }
    }

    IntraGraphStats IntraGraph::get_stats() {
      stats.nnodes = nnodes;
      stats.nedges = nedges;
      stats.nupdatable = 0;
      for(unsigned i = 0; i < (unsigned)nedges; i++) {
        if(edges[i].updatable) {
          stats.nupdatable++;
        }
      }

      set<int> gray;
      set<int> black;
      set<int> cutset;
      dfs(0,gray,black,cutset);
      stats.ncutset = cutset.size();
      return stats;
    }

    // return value: number of SCCs
    int IntraGraph::SCC(vector<IntraGraphNode> &cnodes, int ncnodes,
			vector<IntraGraphEdge> &cedges, int UNUSED_PARAMETER(ncedges)) {
      int n = ncnodes;
      int i;
      // reset visited
      for(i=0;i<n;i++) {  
        cnodes[i].visited = 0;
        cnodes[i].scc_number = 0;
      }

      typedef pair<int, list<int>::iterator> pos_t;
      list<pos_t> stack;
      list<int> ts;
      // DFS(G)
      for( i=0;i<n;i++) {
        if(cnodes[i].visited != 0) continue;
        stack.push_front(pos_t(i, cnodes[i].outgoing.begin()));
        while(!stack.empty()) {
          pos_t p = stack.front();
          stack.pop_front();
          int v = p.first;
          list<int>::iterator it = p.second;
          cnodes[v].visited = 1; // gray
          bool done = true;
          while(it != cnodes[v].outgoing.end()) {
            int c = cedges[*it].tgt;
            if(cnodes[c].visited == 1) { // gray
              it++;
            } else if(cnodes[c].visited == 2) { // black
              it++;
            } else { // white
              stack.push_front(pos_t(v,++it));
              stack.push_front(pos_t(c, cnodes[c].outgoing.begin()));
              done = false;
              break;
            }
          }
          if(done) {
            cnodes[v].visited = 2; // black
            ts.push_front(v);
          }
        }
      }

      for(i=0;i<n;i++) {  
        cnodes[i].visited = 0;
      }

      // Now ts has vertices in decreasing order of finishing time
      // DFG(G^T)
      list<int>::iterator nit;
      int scc_n = 0; // First SCC has number 1

      for(nit = ts.begin(); nit != ts.end(); nit++) {
        i = *nit;
        if(cnodes[i].visited != 0) continue;
        scc_n++;

        stack.push_front(pos_t(i, cnodes[i].incoming.begin()));
        while(!stack.empty()) {
          pos_t p = stack.front();
          stack.pop_front();
          int v = p.first;
          list<int>::iterator it = p.second;
          cnodes[v].visited = 1; // gray
          cnodes[v].scc_number = scc_n;
          bool done = true;
          while(it != cnodes[v].incoming.end()) {
            int c = cedges[*it].src;
            if(cnodes[c].visited == 1) { // gray
              it++;
            } else if(cnodes[c].visited == 2) { // black
              it++;
            } else { // white
              stack.push_front(pos_t(v,++it));
              stack.push_front(pos_t(c, cnodes[c].incoming.begin()));
              done = false;
              break;
            }
          }
          if(done) {
            cnodes[v].visited = 2; // black
          }
        }
      }
      return scc_n;
    }

    // postcond: sets iscutset for all nodes
    void IntraGraph::topSort(vector<IntraGraphNode> &cnodes, int ncnodes,
			     vector<IntraGraphEdge> &cedges, int UNUSED_PARAMETER(ncedges),
                             list<int> &ts, vector<int> &cs, bool no_outgoing, bool no_updatable) {
      int n = ncnodes;
      int i;
      // reset visited
      for(i=0;i<n;i++) {  
        cnodes[i].visited = 0;
        cnodes[i].iscutset = false;
      }
      // First find a cutset
      typedef pair<int, list<int>::iterator> pos_t;
      list<pos_t> stack;
      for( i=0;i<n;i++) {
        if(cnodes[i].visited != 0) continue;
        stack.push_front(pos_t(i, cnodes[i].outgoing.begin()));
        while(!stack.empty()) {
          pos_t p = stack.front();
          stack.pop_front();
          int v = p.first;
          list<int>::iterator it = p.second;
          cnodes[v].visited = 1; // gray
          bool done = true;
          while(it != cnodes[v].outgoing.end()) {
            int c = cedges[*it].tgt;
            if(no_updatable && cedges[*it].updatable) {
              it++;
              continue;
            }
            if(cnodes[c].visited == 1) { // gray
              cnodes[c].iscutset = true;
              it++;
            } else if(cnodes[c].visited == 2) { // black
              it++;
            } else { // white
              stack.push_front(pos_t(v,++it));
              stack.push_front(pos_t(c, cnodes[c].outgoing.begin()));
              done = false;
              break;
            }
          }
          if(done) {
            cnodes[v].visited = 2; // black
          }
        }
      }
      // reset visited
      for( i=0;i<n;i++) {  
        cnodes[i].visited = 0;
        if(cnodes[i].iscutset)
          cs.push_back(i);
      }
      // now do a top-sort -- cutset vertices have no incoming edges / no outgoing edges
      // assert(cnodes[0].iscutset == false);
      for(i = 0; i < n; i++) {
        if(cnodes[i].visited != 0) continue;
        stack.push_front(pos_t(i, cnodes[i].outgoing.begin()));
        while(!stack.empty()) {
          pos_t p = stack.front();
          stack.pop_front();
          int v = p.first;
          list<int>::iterator it = p.second;
          if(no_outgoing && cnodes[v].iscutset) {
            cnodes[v].visited = 2; // black
            continue;
          }
          cnodes[v].visited = 1; // gray
          bool done = true;
          while(it != cnodes[v].outgoing.end()) {
            int c = cedges[*it].tgt;
            if(no_updatable && cedges[*it].updatable) {
              it++;
              continue;
            }
            if(!no_outgoing && cnodes[c].iscutset) {
              it++;
              continue;
            }
            if(cnodes[c].visited != 0) { // gray or black
              assert(cnodes[c].visited == 2); // there cannot be any back edges
              it++;
            } else { // white
              stack.push_front(pos_t(v,++it));
              stack.push_front(pos_t(c, cnodes[c].outgoing.begin()));
              done = false;
              break;
            }
          }
          if(done) {
            cnodes[v].visited = 2; // black
            ts.push_front(v);
          }
        }    
      }
    }

    void IntraGraph::buildCutsetRegExp(list<int> &ts, vector<int> &cs,
                                       vector<IntraGraphNode> &cnodes, int ncnodes,
				       vector<IntraGraphEdge> &cedges, int UNUSED_PARAMETER(ncedges)) {
      int m = cs.size();
      int n = ncnodes;
      assert(n == (int)ts.size());
      int i,j,k,v;

      reg_exp_t **temp = new reg_exp_t *[m+1];
      for(i=0;i<m+1;i++) {
        temp[i] = new reg_exp_t[n];
        for(j=0;j<n;j++) {
          temp[i][j] = RegExp::constant(se->zero());
        }
      }

      // Solve for (s,_)
      temp[m][0] = RegExp::constant(se->one());
      list<int>::iterator it = ts.begin();
      for(;it != ts.end(); it++) {
        v = *it;
        if(cnodes[v].iscutset)
          continue;
        list<int>::iterator beg = cnodes[v].incoming.begin();
        list<int>::iterator end = cnodes[v].incoming.end();
        for(; beg != end; beg++) {
          temp[m][v] = RegExp::combine(temp[m][v], RegExp::extend(temp[m][cedges[*beg].src], cedges[*beg].regexp));
        }
      }
      // Solve for (ci,_)
      vector<int>::iterator it2;
      for(i=0,it2 = cs.begin(); it2 != cs.end(); it2++,i++) {
        list<int>::iterator beg = ts.begin();
        list<int>::iterator end = ts.end();
        while(*beg != *it2) {
          beg++;
        }
        temp[i][*beg] = RegExp::constant(se->one());
        beg++;
        for(; beg != end; beg++) {
          v = *beg;
          if(cnodes[v].iscutset)
            continue;
          list<int>::iterator beg2 = cnodes[v].incoming.begin();
          list<int>::iterator end2 = cnodes[v].incoming.end();
          for(; beg2 != end2; beg2++) {
            temp[i][v] = RegExp::combine(temp[i][v], RegExp::extend(temp[i][cedges[*beg2].src], cedges[*beg2].regexp));
          }
        }
      }

      // Now build the regexp for (ci,cj)
      reg_exp_t **reg = new reg_exp_t* [m];
      for(i = 0; i < m; i++) {
        reg[i] = new reg_exp_t[m];
        for(j = 0; j < m; j++) {
          if(i==j) 
            reg[i][j] = RegExp::constant(se->one());
          else
            reg[i][j] = RegExp::constant(se->zero());
        }
      }
      for(i = 0; i < m; i++) {
        for(j = 0; j < m; j++) {
          int v = cs[j];
          list<int>::iterator beg = cnodes[v].incoming.begin();
          list<int>::iterator end = cnodes[v].incoming.end();
          for(; beg != end; beg++) {
            reg[i][j] = RegExp::combine(reg[i][j], RegExp::extend(temp[i][cedges[*beg].src], cedges[*beg].regexp));
          }
        }
      }
      // now solve APSP on cutsets
      for(k=0;k<m;k++) {
        for(i=0;i<m;i++) {
          for(j=0;j<m;j++) {
            reg[i][j] = RegExp::combine(reg[i][j], RegExp::extend(reg[i][k], RegExp::extend(RegExp::star(reg[k][k]), reg[k][j])));
          }
        }
      }

      for(i = 0; i < m; i++) {
        for(j = 0; j < n; j++) {
          for(k = 0; k < m; k++) {
            temp[i][j] = RegExp::combine(temp[i][j], RegExp::extend(reg[i][k], temp[k][j]));
          }
        }
      }
      // correct temp[m]
      for(i=0;i<m;i++) {
        v = cs[i];
        list<int>::iterator beg = cnodes[v].incoming.begin();
        list<int>::iterator end = cnodes[v].incoming.end();
        for(; beg != end; beg++) {
          temp[m][v] = RegExp::combine(temp[m][v], RegExp::extend(temp[m][cedges[*beg].src], cedges[*beg].regexp));
        }
        //temp[m][v]->print(cout) << "\n";
      }

      // initialize answer
      for(i = 0; i < n; i++) {
        cnodes[i].regexp = temp[m][i]; 
      }
      for(i = 0; i < n; i++) {
        for(j=0;j<m;j++) {
          cnodes[i].regexp = RegExp::combine(cnodes[i].regexp, RegExp::extend(temp[m][cs[j]],temp[j][i]));
        }
      }
      // delete stuff

      for(i = 0; i < m; i++) {
        delete [] reg[i];
        delete [] temp[i];
      }
      delete [] temp[m];
      delete [] temp;
      delete [] reg;
    }

    // Some experimental code. Was not found to be very useful during
    // CAV'06 experimentation.
#include "Experimental.cpp"

    void IntraGraph::dfs(int v, set<int> &gray, set<int> &black, set<int> &cutset) {
      gray.insert(v);
      list<int>::iterator ch = nodes[v].outgoing.begin();
      for(; ch != nodes[v].outgoing.end(); ch++) {
        int c = edges[*ch].tgt;
        set<int>::iterator it = gray.find(c);
        if(it != gray.end()) { // child is gray
          cutset.insert(c);
          continue;
        }
        it = black.find(c);
        if(it != black.end()) { // child is black
          continue;
        }
        dfs(c,gray,black,cutset);
      }
      gray.erase(v);
      black.insert(v);
    }


    // return value: edge not present or is not updatable
    bool IntraGraph::updateEdgeWeight(int s, int t, sem_elem_t se) {
      int eno = edgeno(s,t);
      if(eno == -1) return false;
      if(edges[eno].updatable == false) return false;
      edges[eno].weight = se;
      RegExp::update(edges[eno].updatable_no,se);
      return true;
    }

    void IntraGraph::addEdge(int s, int t, sem_elem_t se, bool updatable) {

      create_node(s);
      create_node(t);

      int eno = edgeno(s,t);

      //    if(eno != -1) {
      //      edges[eno].weight = edges[eno].weight->combine(se);
      //      return;
      //    }

      int uno = 0;
      if(updatable) {
        uno = RegExp::getNextUpdatableNumber();
        // create the updatable reg-exp-node
        RegExp::updatable(uno, se);
      }
      IntraGraphEdge ed(s,t,se,updatable,uno);

      if(eno != -1) { 
        // Edge existed before
        edges[eno].regexp = RegExp::combine(edges[eno].regexp, ed.regexp);
        if(edges[eno].updatable && updatable) {
          cerr << "FWPDS: Warning, parallel updatable edges. Results may not be correct\n";
          print_trans(nodes[edges[eno].src].trans,std::cerr);
          std::cerr << "\n";
          print_trans(nodes[edges[eno].tgt].trans,std::cerr);
          std::cerr << "\n";
        }
        edges[eno].updatable = (edges[eno].updatable | updatable ); 
        if(updatable) {
          edges[eno].updatable_no = uno;
          updatable_edges.push_back(eno);
        }
        return;
      }

      // Create new edge
      if(edges.size() == (unsigned)nedges) {
        edges.resize(2*nedges);
      }
      edges[nedges] = ed; // .set(s,t,se,updatable,uno);
      nedges++;

      int e = nedges - 1;

      nodes[s].outgoing.push_back(e);
      nodes[t].incoming.push_back(e);

      if(updatable) {
        updatable_edges.push_back(e);
      }

    }

    void IntraGraph::setSource(int n, sem_elem_t init_weight) {
      create_node(n);
      nodes[n].type = Source;

      // Create new edge
      if(edges.size() == (unsigned)nedges) {
        edges.resize(2*nedges);
      }
      edges[nedges].set(0,n,init_weight,false);
      nedges++;

      int e = nedges - 1;
      nodes[0].outgoing.push_back(e);
      nodes[n].incoming.push_back(e);
    }

    void IntraGraph::addCallEdge(IntraGraph *next) {
      calls.insert(next);
    }

    void IntraGraph::updateWeight(int node, sem_elem_t wt) {
      if(!(0 < node && node < nnodes)) {
        cout << "Invalid node:" << node << " in an IntraGraph of size " << nnodes << "\n";
        assert(0);
      }
      updates.push_back(update_t(node, wt));
    }

    void IntraGraph::updateWeight(int node, int uno) {
      if(!(0 < node && node < nnodes)) {
        cout << "Invalid node:" << node << " in an IntraGraph of size " << nnodes << "\n";
        assert(0);
      }
      updates.push_back(update_t(node, uno));
    }

    void IntraGraph::clearUpdates() {
      updates.clear();
    }

    // Take updates from "updates" and initialize nodes[].regexp
    // with them
    void IntraGraph::assignUpdates() {
      int i, n = nnodes;
      list<update_t>::iterator beg, end;

      for(i=0;i<n;i++) {
        nodes[i].regexp = NULL;
      }

      beg = updates.begin();
      end = updates.end();
      for(; beg != end; beg++) {
        update_t &it = *beg;

        // updates may repeat the same node (with weight / uno)
        if(it.uno == -1) {
          if(nodes[it.node].regexp.get_ptr() == NULL) {
            nodes[it.node].regexp = RegExp::constant(it.wt);
          } else {
            int uno = nodes[it.node].regexp->updatableNumber();
            RegExp::update(uno, it.wt);
          }
        } else {
          if(nodes[it.node].regexp.get_ptr() == NULL) {
            nodes[it.node].regexp = RegExp::updatable(it.uno, se->zero());
          } else {
            RegExp::update(it.uno, nodes[it.node].regexp->get_weight());
            nodes[it.node].regexp = RegExp::updatable(it.uno, se->zero());
          }
        }
      }

      // Initialize rest of regexps
      for(i=0;i<n;i++) {
        if(nodes[i].regexp.get_ptr() == NULL) {
          nodes[i].regexp = RegExp::constant(se->zero());
        }
      }
    }

    // For each node n, calculate MOP(n,\epsilon). This weight
    // is stored in node_pop_weight. The epsilon transition 
    // is passed in as the parameter eps_trans_nno.
    // The weights are calculated by a single backward query 
    // from eps_trans_nno. The backward query is solved using the
    // path sequence (for paths going forwards).
    void IntraGraph::calculatePopWeights(int eps_trans_nno) {
      int i, n = nnodes;
      RegExp::extendDirectionBackwards(running_prestar);

      assert(n == (int)node_pop_weight.size());
      assert(out_nodes_intra->size() <= 1);

      for(i=0;i<n;i++) {
        node_pop_weight[i] = se->zero();
      }

      int outnode;

      // no eps transition, all weights are zero
      if(eps_trans_nno == -1) {
        assert(out_nodes_intra->size() == 0);
        return;
      }

      // Consistency check between eps_trans and outnode
      if(out_nodes_intra->size() == 1) {
        outnode = *out_nodes_intra->begin();
        assert(outnode == eps_trans_nno);
      } else {
        // need this because some procedures (like main) never 
        // get called and their eps-transition is not an outnode
        outnode = eps_trans_nno; 
      }

      // Initialize for a backward query from outnode
      node_pop_weight[outnode] = se->one();

      // Solve for weights from the path sequence (Go in reverse order)
      for(i=(int)path_sequence.size() - 1; i >= 0; i--) {
        PathSequence &ps = path_sequence[i];

        if(ps.src == ps.tgt) {
          node_pop_weight[ps.src] = ps.regexp->get_weight()->extend(node_pop_weight[ps.tgt]);
        } else {
          node_pop_weight[ps.src] = node_pop_weight[ps.src]->combine(ps.regexp->get_weight()->extend(node_pop_weight[ps.tgt]));
        }
      }
    }

    // Return the pop weight
    // precondition: calculatePopWeights must have already
    // been called.
    sem_elem_t IntraGraph::popWeight(int nno) {
      assert(nno >=0 && nno < nnodes);

      return node_pop_weight[nno];
    }

    // Solve backward query, with initial configurations as "updates"
    void IntraGraph::preSolveSummarySolution(list<WTransition> &change) {
      int i, n = nnodes;

      preSolveRegSummarySolution();

      // The first node is a fake "source" node, so skip it
      for(i=1;i<n;i++) {
        sem_elem_t wt = nodes[i].regexp->get_weight();
        if(!wt->equal(se->zero())) {
          change.push_back(WTransition(nodes[i].trans, wt));
        }
      }
    }

    // Calculate the regexp to solve backward query
    void IntraGraph::preSolveRegSummarySolution() {
      RegExp::extendDirectionBackwards(running_prestar);

      int i, n = nnodes;
      list<update_t>::iterator beg, end;
      set<int>::iterator sbeg, send;
      list<int> stack;

      assert(n == (int)node_weight.size());
      assert(0 != path_sequence.size());

      assignUpdates();

      // Solve for weights from the path sequence (Go in reverse order for pre*)
      for(i=(int)path_sequence.size() - 1; i >= 0; i--) {
        PathSequence &ps = path_sequence[i];

        if(ps.src == ps.tgt) {
          nodes[ps.src].regexp = RegExp::extend(RegExp::constant(ps.regexp->get_weight()), nodes[ps.tgt].regexp);
        } else {
          nodes[ps.src].regexp = RegExp::combine(nodes[ps.src].regexp,RegExp::extend(RegExp::constant(ps.regexp->get_weight()), nodes[ps.tgt].regexp));
        }
      }

    }

    void IntraGraph::solveSummarySolution(list<WTransition> &change) {
      int i, n = nnodes;

      solveRegSummarySolution();

      // Return the updated list of transitions
      for(i=0;i<n;i++) {
        sem_elem_t wt = nodes[i].regexp->get_weight();
        if(!wt->equal(se->zero())) {
          change.push_back(WTransition(nodes[i].trans, wt));
        }
      }

    }

    // Create regexp that solve for weights on all nodes given
    // starting configuration as "updates"
    void IntraGraph::solveRegSummarySolution() {
      RegExp::extendDirectionBackwards(running_prestar);

      int i, n = nnodes;
      
      assert(n == (int)node_weight.size());
      assert(0 != path_sequence.size());
      
      // Initialize: updates --> regexp
      assignUpdates();      
      
      // Solve for regexp from the path sequence
      for(i=0;i<(int)evaluated_path_sequence.size();i++) {
        EvaluatedPathSequence &ps = evaluated_path_sequence[i];
        
        if(ps.src == ps.tgt) {
          nodes[ps.src].regexp = RegExp::extend(nodes[ps.src].regexp, RegExp::constant(ps.value));
        } else {
          nodes[ps.tgt].regexp = RegExp::combine(nodes[ps.tgt].regexp, RegExp::extend(nodes[ps.src].regexp, RegExp::constant(ps.value)));
        }
      }
      
    }

    // Solve for weights of everything in the path sequence
    // Allocate memory for keeping node weights
    void IntraGraph::setupSummarySolution() {
      int i;
      assert(0 != path_sequence.size());

      //path_sequence.clear();
      //computePathSequence(nodes, nnodes, edges, nedges, path_sequence);

      for(i=0; i < (int)path_sequence.size(); i++) {
        PathSequence &ps = path_sequence[i];
        evaluated_path_sequence.push_back(EvaluatedPathSequence(ps.regexp->get_weight(), ps.src, ps.tgt));
      }

      node_weight.resize(nnodes);
      node_pop_weight.resize(nnodes);
    }

    void IntraGraph::setOutNode(int n, int inter_n) {
      create_node(n);
      if(nodes[n].type != OutNode) { // Avoid duplicates
        nodes[n].type = OutNode;
        out_nodes_intra->push_back(n);
        out_nodes_inter->push_back(inter_n);
      }
    }

    sem_elem_t IntraGraph::get_weight(int nno) {

      if(nodes[nno].regexp.get_ptr() == NULL) {
        assert(0);
        //buildCutsetRegExp(topsort_list,cutset_list,nodes,edges); 
        vector<PathSequence> seq;
        domRegExp(nodes,nnodes,edges,nedges,seq);
        buildRegExp(seq);
      }
      FWPDSDBGS({
          cout << "RegExp:";
          nodes[nno].regexp->print(cout) << "\n";
        });
      int nevals ATTR_UNUSED;
      (void) nevals; // This is only sometimes used
      STAT(nevals = nodes[nno].regexp->get_nevals());
      sem_elem_t ret = nodes[nno].regexp->get_weight();
      STAT({
          if(nodes[nno].regexp->get_nevals() != nevals) {
            stats.nget_weight++;
          }
        });
      return ret;
    }


    void IntraGraph::setupIntraSolution(bool UNUSED_PARAMETER(compress_regexp)) {
      RegExp::extendDirectionBackwards(running_prestar);
      vector<IntraGraphNode> cnodes;
      vector<IntraGraphEdge> cedges;
      map<int,int> orig_to_compress;
      list<int> ts;
      vector<int> cs;
      list<int>::iterator nit;

      path_sequence.clear();

      // Note: REGEXP_METHOD other than 0 and 1 may be deprecated
#define REGEXP_METHOD 0

#if REGEXP_METHOD==0

      // SCC followed by Dominator version
      computePathSequence(nodes, nnodes, edges, nedges, path_sequence);
      buildRegExp(path_sequence);

      { // NAK DEBUGGING REGEXP
        //for( int i=0; i < nnodes; i++) {
        //    nodes[i].regexp->print(std::cout<< i << ")   ") << std::endl;
        //}
      }

      //cout << "graph = (" << nnodes << "," << nedges << ")\n";
      //cout << "size = " << path_sequence.size() << "\n";
      //cout << "\n";

      STAT(stats.ndom_sequence = path_sequence.size());
      return;

#elif REGEXP_METHOD==1
      // Dominator
      domRegExp(nodes, nnodes, edges, nedges, path_sequence);
      buildRegExp(path_sequence);

      STAT(stats.ndom_sequence = path_sequence.size());
      return;

#elif REGEXP_METHOD==2

      // Dominator version + regexp compression and huffman-height minimization
      domRegExp(nodes, nnodes, edges, nedges, path_sequence);
      buildRegExp(path_sequence);
      STAT(stats.ndom_sequence = path_sequence.size());
      for(nit = out_nodes_intra->begin(); nit != out_nodes_intra->end(); nit++) {
        int nno = *nit;
        reg_exp_cache_t cache;
        reg_exp_t temp = RegExp::compress(nodes[nno].regexp, cache);
        cache.clear();
        nodes[nno].regexp = RegExp::minimize_height(temp, cache);
        cache.clear();
      }
      return;

#elif REGEXP_METHOD==3

      // Dominators with compression
      topSort(nodes,nnodes, edges, nedges, topsort_list,cutset_list, false, false);
      compressGraph(topsort_list,cutset_list,ts,cs,cnodes,cedges,orig_to_compress);
      domRegExp(cnodes, cnodes.size(), cedges, cedges.size(), path_sequence);
      buildRegExp(path_sequence);
      STAT(stats.ndom_sequence = path_sequence.size());
      for(nit = out_nodes_intra->begin(); nit != out_nodes_intra->end(); nit++) {
        int nno = *nit;
        nodes[nno].regexp = cnodes[orig_to_compress[nno]].regexp;
      }
      return;

#elif REGEXP_METHOD==4

      // Cutset version
      topSort(nodes,edges,topsort_list,cutset_list, false, false);
      buildCutsetRegExp(topsort_list,cutset_list,nodes,nnodes, edges, nedges); return;

#elif REGEXP_METHOD==5

      // Cutset with compression
      compressGraph(topsort_list,cutset_list,ts,cs,cnodes,cedges,orig_to_compress);
      //cout << cs.size() << "\n";
      buildCutsetRegExp(ts,cs,cnodes,cnodes.size(), cedges, cedges.size());

      for(nit = out_nodes_intra->begin(); nit != out_nodes_intra->end(); nit++) {
        int nno = *nit;
        nodes[nno].regexp = cnodes[orig_to_compress[nno]].regexp;
      }
      return;

#elif REGEXP_METHOD==6

      // O(n^3) method
      basicRegExp(compress_regexp);

#endif // REGEXP_METHOD

    }

    sem_elem_t IntraGraph::extend(sem_elem_t w1, sem_elem_t w2) {
      STAT(stats.nextend++);
      if(running_prestar) {
        return w2->extend(w1);
      }
      return w1->extend(w2);
    }

    void IntraGraph::buildRegExp(vector<PathSequence> &seq) {
      int i;
      for(i=0;i<nnodes;i++) {
        nodes[i].regexp = RegExp::constant(se->zero());
      }
      nodes[0].regexp = RegExp::constant(se->one());

      for(i=0;i<(int)seq.size();i++) {
        PathSequence &ps = seq[i];
        if(ps.src == ps.tgt) {
          nodes[ps.src].regexp = RegExp::extend(nodes[ps.src].regexp, ps.regexp);
        } else {
          nodes[ps.tgt].regexp = RegExp::combine(nodes[ps.tgt].regexp, RegExp::extend(nodes[ps.src].regexp, ps.regexp));
        }
      }
    }

    //Use of tree[.] is deprecated
    // e \in tree[v] iff (v = e.tgt and e.src = dom[e.tgt])
    void IntraGraph::domRegExp(vector<IntraGraphNode> &cnodes, int ncnodes, vector<IntraGraphEdge> &cedges, int ncedges, vector<PathSequence> &sequence) {
      int n = ncnodes;
      int i;

      bool buffer_allocated = true;
      int *buffer;
      set<int> *children;
      reg_exp_t *reg;

#ifdef STATIC_MEMORY
      if(n < intraGraphBufferSize) {
        buffer = intraGraphBuffer;
        children = childrenBuffer;
        reg = regBuffer;

        for(i=0;i<n;i++) {
          children[i].clear();
          reg[i] = 0;
        }

        buffer_allocated = false;
      } else {
        buffer = new int[5*n];
        children = new set<int>[n];
        reg = new reg_exp_t[n];
      }
#else
      buffer = new int[5*n];
      children = new set<int>[n];
      reg = new reg_exp_t[n];
#endif

      // children : vertex ->set<vertex>
      // reg : vertex -> R(vertex)

      // does not take much time
      int *dom = computeDominators(cnodes, ncnodes, cedges, ncedges, buffer, children);

      int j,u,v,w;
        
      // dom is buffer..buffer+(n-1)
      int *number = buffer+n; // vertex -> number
      int *vertex = buffer+(2*n); // number -> vertx
      int *tree = buffer + (3*n);   // vertex -> edge

      int *ancestor = buffer + (4*n); // vertex -> vertex

      //vector<PathSequence> sequence;
      setupDomRegExp(cnodes, ncnodes, cedges, ncedges, dom, number, vertex, tree, children);

      // Initialize
      for(i=0;i<n;i++) {
        cnodes[i].regexp = RegExp::constant(se->one());
        ancestor[i] = -1;
      }
      vector<IntraGraphNode> sub_cnodes;
      vector<IntraGraphEdge> sub_cedges;
      // node -> sub_node map
      map<int, int> sub_node_number;
      // sub_node -> node map
      map<int, int> node_number;

      // Loop
      for(i=0;i<n;i++) {
        u = vertex[i];
        sub_cnodes.clear();
        sub_cedges.clear();
        sub_node_number.clear();
        node_number.clear();
        set<int>::iterator beg = children[u].begin();
        set<int>::iterator end = children[u].end();
        for(; beg != end; beg++) {
          v = *beg;
          list<int>::iterator ebeg = cnodes[v].incoming.begin();
          list<int>::iterator eend = cnodes[v].incoming.end();
          for(; ebeg != eend; ebeg++) {
            w = cedges[*ebeg].src;
            //if(*ebeg == tree[v]) continue;
            if(w == dom[v]) continue;
            // head of e-tilde
            while(dom[w] != dom[v]) {
              w = dom[w];
            }
            // make nodes for w and v
            int wt,vt,et;
            if(sub_node_number.find(w) == sub_node_number.end()) {
              wt = sub_node_number.size();
              sub_node_number[w] = wt;
              node_number[wt] = w;
              sub_cnodes.push_back(cnodes[w]);
              sub_cnodes[wt].incoming.clear();
              sub_cnodes[wt].outgoing.clear();
            } else {
              wt = sub_node_number[w];
            }
            if(sub_node_number.find(v) == sub_node_number.end()) {
              vt = sub_node_number.size();
              sub_node_number[v] = vt;
              node_number[vt] = v;
              sub_cnodes.push_back(cnodes[v]);
              sub_cnodes[vt].incoming.clear();
              sub_cnodes[vt].outgoing.clear();
            } else {
              vt = sub_node_number[v];
            }
            sub_cedges.push_back(IntraGraphEdge(wt, vt, se->zero(), false));
            et = sub_cedges.size() - 1;
            sub_cnodes[wt].outgoing.push_back(et);
            sub_cnodes[vt].incoming.push_back(et);
            sub_cedges[et].regexp = eval_and_sequence(cnodes, cedges, *ebeg, ancestor, sequence);
          }
        }
        // Compute a path sequence for sub-graph
        int slength = sequence.size();
        computePathSequence(sub_cnodes, sub_cnodes.size(), sub_cedges, sub_cedges.size(), sequence, true);
        // Solve
        beg = children[u].begin();
        for(; beg != end; beg++) {
          v = *beg;
          reg[v] = RegExp::constant(se->zero());
          list<int>::iterator ebeg = cnodes[v].incoming.begin();
          list<int>::iterator eend = cnodes[v].incoming.end();
          for(; ebeg != eend; ebeg++) {
            if(cedges[*ebeg].src != dom[v]) continue;
            reg[v] = RegExp::combine(reg[v], cedges[*ebeg].regexp);
          }
          //if(tree[v] != -1) {
          //  reg[v] = RegExp::combine(reg[v], cedges[tree[v]].regexp);
          //}
        }
        for(j = slength; j < (int)sequence.size(); j++) {
          PathSequence &ps = sequence[j];
          // correct node numbers
          ps.src = node_number[ps.src]; // nodeno(sub_cnodes[ps.src].trans);
          ps.tgt = node_number[ps.tgt]; // nodeno(sub_cnodes[ps.tgt].trans);

          if(ps.src == ps.tgt) {
            reg[ps.src] = RegExp::extend(reg[ps.src], ps.regexp);
          } else {
            reg[ps.tgt] = RegExp::combine(reg[ps.tgt], RegExp::extend(reg[ps.src], ps.regexp));
          }
        }
        // Update
        beg = children[u].begin();
        for(; beg != end; beg++) {
          v = *beg;
          cnodes[v].regexp = reg[v];
          ancestor[v] = u;
        }
      }
      // Finalize

      if(cnodes[0].incoming.size() != 0) {
        reg_exp_t q = RegExp::constant(se->zero());
        list<int>::iterator beg = cnodes[0].incoming.begin();
        list<int>::iterator end = cnodes[0].incoming.end();

        assert(dom[0] == -1); // node 0 has no tree edges
        for(; beg != end; beg++) {
          //if(*beg == tree[0])
          //  continue;
          q = RegExp::combine(q, eval_and_sequence(cnodes, cedges, *beg, ancestor, sequence));
        }
      
        q = RegExp::star(q);
        sequence.push_back(PathSequence(q, 0, 0));
      }

      for(i = n-2; i >= 0; i--) {
        v = vertex[i];
        sequence.push_back(PathSequence(cnodes[v].regexp,ancestor[v],v));   
      }

      if(buffer_allocated) {
        delete [] buffer;
        delete [] children;
        delete [] reg;
      }
    }

    reg_exp_t IntraGraph::eval_and_sequence(vector<IntraGraphNode> &cnodes, vector<IntraGraphEdge> &cedges,
                                            int e, int *ancestor, vector<PathSequence> &sequence) {
      int h = cedges[e].src;
      int t = cedges[e].tgt;
      reg_exp_t r = cedges[e].regexp;
      if(ancestor[h] != -1) {
        compress_and_sequence(cnodes, cedges, h, ancestor, sequence);
        sequence.push_back(PathSequence(cedges[e].regexp, h, t));
        r = RegExp::extend(cnodes[h].regexp, r);
      }
      return r;
    }

    void IntraGraph::compress_and_sequence(vector<IntraGraphNode> &cnodes, vector<IntraGraphEdge> &cedges,
                                           int h, int *ancestor, vector<PathSequence> &sequence) {
      if(ancestor[ancestor[h]] != -1) {
        compress_and_sequence(cnodes, cedges, ancestor[h], ancestor, sequence);
        sequence.push_back(PathSequence(cnodes[h].regexp, ancestor[h], h));
        cnodes[h].regexp = RegExp::extend(cnodes[ancestor[h]].regexp, cnodes[h].regexp);
        ancestor[h] = ancestor[ancestor[h]];
      }
    }

    void IntraGraph::computePathSequence(vector<IntraGraphNode> &cnodes, int ncnodes, vector<IntraGraphEdge> &cedges, 
                                         int ncedges, vector<PathSequence> &sequence, bool use_cutset) {
      if(ncedges == 0) 
        return;
          
      int maxs = SCC(cnodes,ncnodes,cedges,ncedges);
      int n = (int)ncnodes;
      int m = (int)ncedges;
      int i;
      //static int count = 0;

      if(use_cutset && m < n) { // possibly acyclic
        computePathSequenceCutset(cnodes,ncnodes,cedges,ncedges,sequence);
        return;
      }

      vector<IntraGraphNode> *scc_nodes = new vector<IntraGraphNode>[maxs+1];
      vector<IntraGraphEdge> *scc_edges = new vector<IntraGraphEdge>[maxs+1];
      vector<int> *connecting_edges = new vector<int>[maxs+1];

      map<int, int> *inv_node_map = new map<int,int>[maxs+1];
      map<int, int> node_map;

      // Build a graph for each SCC
      for(i=0;i<n;i++) {
        int s = cnodes[i].scc_number;
        assert(s > 0 && s <=maxs);
        scc_nodes[s].push_back(cnodes[i]);

        int last = scc_nodes[s].size();
        scc_nodes[s][last-1].incoming.clear();
        scc_nodes[s][last-1].outgoing.clear();

        node_map[i] = last-1;
        inv_node_map[s][last-1] = i;
      }
      for(i=0;i<m;i++) {
        //cedges[i].regexp = RegExp::updatable(i,se->zero());
        int s1 = cnodes[cedges[i].src].scc_number;
        int s2 = cnodes[cedges[i].tgt].scc_number;
        if(s1 != s2) {
          //assert(s1< s2);
          connecting_edges[s1].push_back(i);
        } else {
          int ns = node_map[cedges[i].src], nt = node_map[cedges[i].tgt];
          scc_edges[s1].push_back(IntraGraphEdge(ns,nt,se->zero(),false));
          int last = scc_edges[s1].size();
          scc_edges[s1][last-1].regexp = cedges[i].regexp;
          scc_nodes[s1][ns].outgoing.push_back(last-1);
          scc_nodes[s1][nt].incoming.push_back(last-1);
        }
      }

      unsigned int j;

      /* Sanity check
         int a=0,b=0;
         for(i=1;i<=maxs;i++) {
         cout << scc_nodes[i].size() << "\n";
         for(j=0;j<scc_edges[i].size();j++) {
         cout << "(" << inv_node_map[i][scc_edges[i][j].src] << "," << inv_node_map[i][scc_edges[i][j].tgt] << ") ";
         }
         cout << "\n";
         for(j=0;j<connecting_edges[i].size();j++) {
         cout << "(" << cedges[connecting_edges[i][j]].src << "," << cedges[connecting_edges[i][j]].tgt << ") ";
         }
         cout << "\n";
         a += scc_nodes[i].size();
         b += scc_edges[i].size();
         b += connecting_edges[i].size();
         }
         if(!(a == n && b == m)) {
         cout << a << " " << n << "\n";
         cout << b << " " << m << "\n";
         assert(0);
         }
         cin >> a;
      */

      // Now build the complete path sequence

      for(i=1;i<=maxs;i++) {
        vector<PathSequence> seq;
        if(use_cutset)
          computePathSequenceCutset(scc_nodes[i],scc_nodes[i].size(),scc_edges[i],scc_edges[i].size(),seq);
        else
          domRegExp(scc_nodes[i], scc_nodes[i].size(), scc_edges[i], scc_edges[i].size(), seq);

        for(j=0;j<seq.size();j++) {
          sequence.push_back(PathSequence(seq[j].regexp, inv_node_map[i][seq[j].src], inv_node_map[i][seq[j].tgt]));
          //seq[j].regexp->print(cout) << "\n";
        }
        // Now put in the connecting edges
        for(j=0;j<connecting_edges[i].size();j++) {
          int ed = connecting_edges[i][j];
          sequence.push_back(PathSequence(cedges[ed].regexp, cedges[ed].src, cedges[ed].tgt));
        }
      }

      delete [] scc_nodes;
      delete [] scc_edges;
      delete [] connecting_edges;
      delete [] inv_node_map;

      //WIN(stats.path_seq_time  += clock() - start);
    }

    void IntraGraph::computePathSequenceCutset(vector<IntraGraphNode> &cnodes, int ncnodes, vector<IntraGraphEdge> &cedges, int ncedges, vector<PathSequence> &sequence) {
      int n = (int)ncnodes;
      if(ncedges==0) return;

      list<int> ts; vector<int> cs;
      topSort(cnodes,ncnodes,cedges,ncedges,ts,cs,false,false);
      int m = (int)cs.size();

      list<int>::iterator beg,end;

      STAT({
          stats.ndom_components ++;
          stats.ndom_componentsize += n;
          stats.ndom_componentcutset += m;
        });
      int i,j,k,u,v,w;
      reg_exp_t **reg;
      //int seqlen = sequence.size();
      // Put in top-sorted acyclic edges into sequence
      vector<int>::iterator it;
      beg = ts.begin();
      end = ts.end();
      for(; beg != end; beg++) {
        v = *beg;
        list<int>::iterator beg2 = cnodes[v].outgoing.begin();
        list<int>::iterator end2 = cnodes[v].outgoing.end();
        for(; beg2 != end2; beg2++) {
          u = cedges[*beg2].tgt;
          if(!cnodes[u].iscutset)
            sequence.push_back(PathSequence(cedges[*beg2].regexp, cedges[*beg2].src, cedges[*beg2].tgt));
        }
      }
      if(m==0) return;

      reg = new reg_exp_t *[m];
      for(i=0;i<m;i++) {
        reg[i] = new reg_exp_t[n];
        for(j=0;j<n;j++) {
          reg[i][j] = RegExp::constant(se->zero());
        }
      }

      // Solve for (ci,_)
      for(i=0,it = cs.begin(); it != cs.end(); it++,i++) {
        list<int>::iterator beg = ts.begin();
        list<int>::iterator end = ts.end();
        while(*beg != *it) {
          beg++;
        }
        reg[i][*beg] = RegExp::constant(se->one());
        beg++;
        for(; beg != end; beg++) {
          v = *beg;
          if(cnodes[v].iscutset)
            continue;
          list<int>::iterator beg2 = cnodes[v].incoming.begin();
          list<int>::iterator end2 = cnodes[v].incoming.end();
          for(; beg2 != end2; beg2++) {
            reg[i][v] = RegExp::combine(reg[i][v], RegExp::extend(reg[i][cedges[*beg2].src], cedges[*beg2].regexp));
          }
        }
      }
      // Put in incoming edges of ci
      for(i = 0; i < m; i++) {
        for(j = 0; j < m; j++) {
          v = cs[j];
          list<int>::iterator beg = cnodes[v].incoming.begin();
          list<int>::iterator end = cnodes[v].incoming.end();
          for(; beg != end; beg++) {
            u = cedges[*beg].src;
            if(!cnodes[u].iscutset) {
              sequence.push_back(PathSequence(cedges[*beg].regexp, cedges[*beg].src, cedges[*beg].tgt));
              reg[i][v] = RegExp::combine(reg[i][v], RegExp::extend(reg[i][cedges[*beg].src], cedges[*beg].regexp));

            } else {
              if(u==cs[i])
                reg[i][v] = RegExp::combine(reg[i][v], cedges[*beg].regexp);
            }
          }
        }
      }
      // Construct a path sequence for (ci,cj)
      for(i = 0; i < m; i++) {
        v = cs[i];
        reg[i][v] = RegExp::star(reg[i][v]);
        for(j = i+1; j < m; j++) {
          u = cs[j];
          if(reg[j][v]->isZero()) continue;
          reg[j][v] = RegExp::extend(reg[j][v],reg[i][v]);
          for(k = i+1; k < m; k++) {
            w = cs[k];
            if(reg[i][w]->isZero()) continue;
            reg[j][w] = RegExp::combine(reg[j][w],RegExp::extend(reg[j][v],reg[i][w]));
          }
        }
      }
      // Add to sequence
      for(i = 0; i < m; i++) {
        u = cs[i];
        for(j = i; j < m; j++) {
          w = cs[j];
          if(w == u && reg[i][w]->isOne()) continue;
          if(reg[i][w]->isZero()) continue;
          sequence.push_back(PathSequence(reg[i][w],u,w));
        }
      }
      for(i = m-1; i >= 0; i--) {
        u = cs[i];
        for(j = i-1; j >= 0; j--) {
          w = cs[j];
          if(reg[i][w]->isZero()) continue;
          sequence.push_back(PathSequence(reg[i][w],u,w));
        }
      }
      // Put in the acyclic edges again
      beg = ts.begin();
      end = ts.end();
      for(; beg != end; beg++) {
        v = *beg;
        list<int>::iterator beg2 = cnodes[v].outgoing.begin();
        list<int>::iterator end2 = cnodes[v].outgoing.end();
        for(; beg2 != end2; beg2++) {
          u = cedges[*beg2].tgt;
          if(!cnodes[u].iscutset)
            sequence.push_back(PathSequence(cedges[*beg2].regexp, cedges[*beg2].src, cedges[*beg2].tgt));
        }
      }
      // Delete
      for(i=0;i<m;i++)
        delete [] reg[i];
      delete [] reg;

    }

    // All time spent here is in the first for-loop, and it isn't much
    void IntraGraph::setupDomRegExp(vector<IntraGraphNode> &cnodes ATTR_UNUSED, int ncnodes,
				    vector<IntraGraphEdge> &cedges, int ncedges,
                                    int *dom, int *number, int *vertex, int *tree, set<int> *children) {
      (void) cnodes;

      int n = ncnodes;
      int m = ncedges;
      int i,count;

      for(i=0;i<n;i++) {
        tree[i] = -1;
        if(dom[i] != -1)
          children[dom[i]].insert(i);
      }
    
      for(i=0;i<m;i++) {
        if(dom[cedges[i].tgt] == cedges[i].src) {
          tree[cedges[i].tgt] = i;
        }
      }
      count = n-1;

      numberNodes(0, number, vertex, children, count);
    }

    void IntraGraph::numberNodes(int v, int *number, int *vertex, set<int> *children, int &count) {
      number[v] = count;
      vertex[count] = v;
      count--;
      set<int>::iterator beg = children[v].begin();
      set<int>::iterator end = children[v].end();
      for(; beg != end; beg++) {
        numberNodes(*beg,number,vertex,children,count);
      }
    }


    // buffer must be of size atleast 4*ncnodes
    int *IntraGraph::computeDominators(vector<IntraGraphNode> &cnodes, int ncnodes,
				       vector<IntraGraphEdge> &cedges, int ncedges ATTR_UNUSED,
				       int *buffer, set<int> *bucket_buffer) {
      (void) ncedges;

      int n = ncnodes;
      int i,u,v,w,count;

      int *dom = buffer;
      int *parent = buffer+n;
      int *semi = buffer+(2*n);
      int *vertex = buffer+(3*n);
      set<int> *bucket = bucket_buffer;

      LinkEval le(n,semi);

      // Step 1
      for(i=0;i<n;i++) {
        semi[i] = -1;
      }
      count = 0;

      dfsDominators(cnodes,cedges,0,parent,semi,vertex,count);

      assert(count == n);

      // Step 2 and 3
      for(i = n-1; i >= 1; i--) {
        w = vertex[i];
        list<int>::iterator beg = cnodes[w].incoming.begin();
        list<int>::iterator end = cnodes[w].incoming.end();
        for(; beg != end; beg++) {
          v = cedges[*beg].src;
          u = le.eval(v);
          if(semi[u] < semi[w]) {
            semi[w] = semi[u];
          }
        }
        bucket[vertex[semi[w]]].insert(w);
        le.link(parent[w],w);
        set<int>::iterator bbeg = bucket[parent[w]].begin();
        set<int>::iterator bend = bucket[parent[w]].end();
        for(; bbeg != bend; bbeg++) {
          v = *bbeg;
          u = le.eval(v);
          dom[v] = (semi[u] < semi[v]) ? u : parent[w];
        }
        bucket[parent[w]].clear();
      }
        
      // Step 4
      for(i=1;i<n;i++) {
        w = vertex[i];
        if(dom[w] != vertex[semi[w]]) {
          dom[w] = dom[dom[w]];
        }
      }
      dom[0] = -1;

      return dom;
    }

    void IntraGraph::dfsDominators(vector<IntraGraphNode> &cnodes, vector<IntraGraphEdge> &cedges, 
                                   int v, int *parent, int *semi, int *vertex, int &n) {
      semi[v] = n;
      vertex[n] = v;
      n++;
      list<int>::iterator beg = cnodes[v].outgoing.begin();
      list<int>::iterator end = cnodes[v].outgoing.end();
      for(; beg != end; beg++) {
        int w = cedges[*beg].tgt;
        if(semi[w] == -1) {
          parent[w] = v;
          dfsDominators(cnodes,cedges,w,parent,semi,vertex,n);
        }
      }
    }

  } // namespace graph
} // namespace wali
