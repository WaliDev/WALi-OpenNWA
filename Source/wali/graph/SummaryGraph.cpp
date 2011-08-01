#include "wali/Common.hpp"

#include "wali/util/Timer.hpp"

#include "wali/graph/SummaryGraph.hpp"
#include "wali/graph/Graph.hpp"

#include "wali/wfa/Trans.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/TransFunctor.hpp"

#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wpds/fwpds/LazyTrans.hpp"

#include "wali/wpds/GenKeySource.hpp"

namespace wali {
  using wfa::ITrans;
  using wfa::Trans;
  using wfa::State;
  using util::Timer;
  using wpds::fwpds::LazyTrans;
  using wpds::ewpds::ETrans;
  using wpds::ewpds::erule_t;
  using wpds::GenKeySource;

  namespace graph {

    SummaryGraph::SummaryGraph(InterGraphPtr gr, Key ss, set<Key> &pe, wfa::WFA &Agrow, InterGraph::PRINT_OP pop) {
      post_igr = gr;
      pkey = pop;
      init_state = ss;

      procEntries = pe;

      if(procEntries.size() != post_igr->gr_list.size()) {
        cerr << "SWPDS: Error: the number of procedure entries do not match with the number of procedures"
             << "created by FWPDS. Check that post*(Agrow) did not generate new mid-states\n";
        assert(0);
      }

      // Update all weights
      post_igr->update_all_weights();

      // Build a stack -> node map
      int i, n = (int)post_igr->nodes.size();
      for(i=0;i<n;i++) {
        int stk = post_igr->nodes[i].trans.stack;
        if(stk == 0) continue;

        if(stack_graph_map.find(stk) == stack_graph_map.end()) {
          stack_graph_map.insert(stk, i);
        } else {
          // Sanity check: Every stk should be in a unique IntraGraph
          int j = stack_graph_map[stk];
          if(post_igr->nodes[i].gr != post_igr->nodes[j].gr) {
            pkey(cerr << "SWPDS: Warning: Node belongs to multiple procedures: ", stk) << "\n";
            multiple_proc_nodes.insert(stk);

            //TODO: This can be fixed. For now, we assume that this node is a terminal node, i.e.,
            //it cannot reach any node except itself. (This assumption comes because such nodes
            //are typically self loops "error: goto error".) Therefore, if the node has
            //other successors, they may not show up in post* automaton
            //Also, one should not run a prestar query from an automaton that has such a node.
            //assert(0);
          }
        }
      }

      // construct procEntryMap
      set<Key>::iterator sit;
      for(sit = procEntries.begin(); sit != procEntries.end(); sit++) {
        int nno = stk2nodeno(*sit);
        if(nno == -1) {
          cerr << "SWPDS: Error: Procedure entry node reachable. Check argument to SummaryGraph constructor\n";
          assert(0);
        }
        procEntryMap[post_igr->nodes[nno].gr] = *sit;
      }

      // Construct a map: ETrans -> ERule used to create it
      const set<Key> states = Agrow.getStates();
      for(set<Key>::const_iterator csit = states.begin(); csit != states.end(); csit++) {

        // no ETrans can be outgoing from init_state
        if(*csit == init_state) continue;

        State *q = Agrow.getState(*csit);
        State::iterator transit;

        for(transit = q->begin(); transit != q->end(); transit++) {
          ITrans *t = *transit;
          Transition tr(*t);

          // We know that the transition t is a LazyTrans
          LazyTrans *lt = dynamic_cast<LazyTrans *> (t);
          assert(lt != 0);
          ETrans *et = lt->getETrans();
          assert(et != 0);

          eruleMap[tr] = et->getERule();
        }
      }


      // clear visited, and do more preprocessing inside IntraGraphs (evaluate path sequence weights)
      std::list<IntraGraph *>::iterator it;
      for(it = post_igr->gr_list.begin(); it != post_igr->gr_list.end(); it++) {
        IntraGraph *gr = *it;
        gr->setupSummarySolution();
        gr->visited = false;
      }

      // Pop weights
      {
        Timer timer("Pop Weights");
        calculatePopWeights();
      }

    }

    // For each node n, calculate MOP(n,\epsilon).
    // This only requires a bunch of IntraProcedural queries
    void SummaryGraph::calculatePopWeights() {
      Transition tr(init_state, WALI_EPSILON, 0);
      
      list<IntraGraph *>::iterator it;
      for(it = post_igr->gr_list.begin(); it != post_igr->gr_list.end(); it++) {
        IntraGraph *gr = *it;
        
        // Get the eps-transition node (poor IntraGraphs don't know what their nodes are)
        Key pe = procEntryMap[gr];
        int nno = stk2nodeno(pe);
        assert(nno != -1);
        tr.tgt = post_igr->nodes[nno].trans.tgt;

        if(!post_igr->exists(tr)) continue;

        gr->calculatePopWeights(post_igr->nodes[post_igr->nodeno(tr)].intra_nodeno);
      }
    }

    // Return MOP(k,\epsilon)
    sem_elem_t SummaryGraph::popWeight(Key k) {

      // look up cache first
      map<Key, sem_elem_t>::iterator it = popWeightMap.find(k);
      if(it != popWeightMap.end())
        return it->second.get_ptr();

      assert(k != WALI_EPSILON);

      sem_elem_t ret;

      if(!reachable(k)) {
        ret = NULL;
      } else {
        // Find the appropriate node number in the intragraph k belongs
        // to.
        int nno = stk2nodeno(k);
        Transition tr(init_state, k, post_igr->nodes[nno].trans.tgt);
        if(!post_igr->exists(tr)) {
          ret = NULL;
        } else {
          nno = post_igr->nodeno(tr);
          ret = post_igr->nodes[nno].gr->popWeight(post_igr->nodes[nno].intra_nodeno);
        }
      }

      if(ret.get_ptr() == NULL) {
        ret = post_igr->sem->zero();
      }

      popWeightMap[k] = ret;

      return ret;
    }

    // Return the entry node of the procedure containing k
    // Error code: returns WALI_EPSILON if k is unreachable
    Key SummaryGraph::getEntry(Key k) {
      assert(k != WALI_EPSILON);
      int nno = stk2nodeno(k);

      if(nno == -1) return WALI_EPSILON;

      return procEntryMap[post_igr->nodes[nno].gr];
    }

    // return MOP(entry(k), k)
    // Error code: returns NULL if node not reachable
    sem_elem_t SummaryGraph::pushWeight(Key k) {
      assert(k != WALI_EPSILON);

      if(!reachable(k))
        return NULL;

      int nno = stk2nodeno(k);
      Transition tr(init_state, k, post_igr->nodes[nno].trans.tgt);
      assert(post_igr->exists(tr));

      // It is ok to get (init_state, k, _) weight directly from the
      // InterGraph without calling get_weight. This is because only the
      // weight of (mid-state, k, _) is computed on a call to get_weight
      return post_igr->nodes[post_igr->nodeno(tr)].weight;
    }


    // mimics getIntraNodeNumber (see that function for more comments)
    // returns "is the node present in post_igr", i.e., it is reachable
    // from some procedure entry?
    bool SummaryGraph::reachable(Key stk) {
      assert(stk != WALI_EPSILON);
      
      Transition tr(init_state, stk, init_state); // final state is redundant
      
      // Locate the stack first
      int nno = stk2nodeno(tr.stack);
      if(nno == -1) {
        return false;       
      }
      
      // get the correct intended transition
      Transition tt = tr;
      tt.tgt = post_igr->nodes[nno].trans.tgt;
      if(!post_igr->exists(tt)) {
        // Transition does not exist (This may happen when there is a call that never returns
        // and t.stack is the return node for that call)
        return false;
      }
      
      return true;
    }

    // Does the node stk belong to multiple procedures?
    bool SummaryGraph::multiple_proc(Key stk) {
      return (multiple_proc_nodes.find(stk) != multiple_proc_nodes.end());
    }

    SummaryGraph::~SummaryGraph() { }

    // stack node --> post_igr node number
    int SummaryGraph::stk2nodeno(int stk) {
      StackGraphMap::iterator it = stack_graph_map.find(stk);
      if(it == stack_graph_map.end()) { // we should have already seen stk
        //pkey(cout << "SummaryGraph saw this for the first time:", stk) << "\n";
        //assert(0);
        return -1;
      }
      return it->second;
    }

    int SummaryGraph::trans2nodeno(Transition &t) {
      TransMap::iterator it = trans_map.find(t);
      if(it == trans_map.end()) {
        trans_map[t] = nodes.size();
        nodes.push_back(SummaryGraphNode(t));
        return (nodes.size() - 1);
      }
      return it->second;
    }

    int SummaryGraph::getIntraNodeNumber(Transition &tr) {
      assert(tr.stack != (int)WALI_EPSILON);

      // Locate the stack first
      int nno = stk2nodeno(tr.stack);
      if(nno == -1) {
        pkey(cout << "Warning: Unreachable code (", tr.stack) << ")\n";
        return -1;
      }

      // get the correct intended transition
      Transition tt = tr;
      tt.tgt = post_igr->nodes[nno].trans.tgt;
      if(!post_igr->exists(tt)) {
        // Transition does not exist (This may happen when there is a call that never returns
        // and t.stack is the return node for that call)
        pkey(cout << "Warning: Unreachable code (", tr.stack) << ")\n";

        return -1;
      }

      int nno2 = post_igr->nodeno(tt);
      return post_igr->nodes[nno2].intra_nodeno;
    }

    void SummaryGraph::getUpdatedTransitions(std::list<WTransition> &ls) {
      std::set<IntraGraph *>::iterator it;

      for(it = updated_graphs.begin(); it != updated_graphs.end(); it++) {
        IntraGraph *gr = *it;
        gr->solveSummarySolution(ls);
        gr->clearUpdates();
      }

      updated_graphs.clear();
      // TODO: Not clear anything else?
    }

    void SummaryGraph::preGetUpdatedTransitions(list<WTransition> &ls) {
      set<IntraGraph *>::iterator it;
      list<IntraGraph *>::iterator lit;

      for(it = updated_graphs.begin(); it != updated_graphs.end(); it++) {
        IntraGraph *gr = *it;
        gr->preSolveSummarySolution(ls);
        gr->clearUpdates();
      }

      // clear all updates
      updated_graphs.clear();
      changed_graphs.clear();
      for(lit = post_igr->gr_list.begin(); lit != post_igr->gr_list.end(); lit++) {
        IntraGraph *gr = *lit;
        gr->visited = false;
      }
    }

    void SummaryGraph::preAddUpdate(Transition &t, sem_elem_t wt) {

      assert(t.src == init_state);
      assert(t.stack != (int)WALI_EPSILON);

      int nno = getIntraNodeNumber(t);
      if(nno == -1) {
        pkey(cout << "SWPDS: Warning: Unreachable code (", t.stack) << ")\n";
        return;
      }

      IntraGraph *gr = post_igr->nodes[stk2nodeno(t.stack)].gr;
      
      gr->updateWeight(nno, wt);
      if(!gr->visited) {
        gr->visited = true;
        changed_graphs.push_back(gr);
      }
      updated_graphs.insert(gr);
    }

    void SummaryGraph::getMiddleTransitions(std::list<WTransition> &ls) {
      std::list<IntraGraph *>::iterator it;

      // Mark all Intragraphs whose transitions should be there
      while(!changed_graphs.empty()) {
        IntraGraph *gr = changed_graphs.front();
        changed_graphs.pop_front();

        std::set<IntraGraph *>::iterator it2;
        for(it2 = gr->calls.begin(); it2 != gr->calls.end(); it2++) {
          IntraGraph *ch = *it2;
          if(!ch->visited) {
            ch->visited = true;
            changed_graphs.push_back(ch);
          }
        }
      }
      // Get all the appropriate transitions
      int i, n = post_igr->nodes.size();
      for(i=0;i<n;i++) {
        if(post_igr->nodes[i].gr->visited) {
          //IntraGraph::print_trans(nodes[i].trans, cout) << "\n";
          ls.push_back(WTransition(post_igr->nodes[i].trans,post_igr->nodes[i].weight));
        }
      }

      // clear all updates
      for(it = post_igr->gr_list.begin(); it != post_igr->gr_list.end(); it++) {
        IntraGraph *gr = *it;
        gr->visited = false;
      }
      changed_graphs.clear();
    }

    std::ostream &SummaryGraph::printStats(std::ostream &out) {
      out << "******** SummaryGraph Stats ************\n";
      Timer::printTime(out << "t1 = ", stats.t1) << "\n";
      Timer::printTime(out << "t2 = ", stats.t2) << "\n";
      Timer::printTime(out << "t3 = ", stats.t3) << "\n";
      Timer::printTime(out << "t4 = ", stats.t4) << "\n";
      stats.t1 = stats.t2 = stats.t3 = stats.t4 = 0;
      return out;
    }

    // One trick that was quite useful (popRegExpMap):
    // The regexp obtained from IntraGraphs is used only the first time to
    // compute the weight on a eps-transition. After that, an APSP style regexp
    // is used. The intuition is that not all incoming transitions to a state
    // have their weight change in the pop phase. So use their weights once, and
    // after that do APSP style computation of weights on the eps transtions.
    void SummaryGraph::summaryPoststar(wali::wfa::WFA const & ca_in, wali::wfa::WFA& ca_out) {
      int i;
      RegExp::startSatProcess(post_igr->sem);
      RegExp::extendDirectionBackwards(false);

      typedef pair<int, Key> tup;
      std::set<tup> worklist;

      graph::Graph ca_gr;
      std::map<Key, int> state_to_node_map;
      std::map<int, Key> node_to_state_map;

      std::set<Key> states = ca_in.getStates();
      std::set<Key> state_has_eps; // states that have an incoming eps-transition

      std::map<Key, std::set<ITrans *> > state_trans_map;
      std::map<Key, std::set<ITrans* > >::iterator st_map_it;

      // ** For saturation to proceed in pop-phase style (APSP) **
      map<Key, reg_exp_t> popRegExpMap; // state q to pop-regexp for (init, \ee, q)
      set<Key> state_used_once; // we switch from intraq-regexp to pop-regexp after we've used the former once

      Timer *timer1 = new Timer("SWPDS: Setup");

      std::set<Key>::iterator set_it;
      // Build a map: state -> { incoming transition }
      // and a graph representing the WFA (will use for SCC decomposition)
      for(set_it = states.begin(); set_it != states.end(); set_it++) {

        wfa::State const * state = ca_in.getState(*set_it);

        wfa::State::const_iterator tli = state->begin();;

        for( ; tli != state->end(); tli++) {

          ITrans* t = *tli;

          //t->print(cout << "Considering transition: ") << "\n";

          // Ignore eps transitions (assume that eps-closure has been
          // performed on the input)
          if(t->stack() == WALI_EPSILON)
            continue;

          // Ignore unreachable code
          // Note that when there are calls that do not return, the corresponding
          // return transition has a unreachable stack node, but it is important to
          // still consider the transition
          if(t->from() == init_state && !reachable(t->stack())) {
            pkey(cerr << "SWPDS: Warning: Unreachable code(", t->stack()) << ")\n";
            continue;
          }

          // No transition to init state allowed
          assert(t->to() != init_state);

          state_trans_map[t->to()].insert(t);

          if(t->from() != init_state) {

            // non-init state to non-init state transitions carry over
            // unchanged to the output automaton
            ca_out.addTrans(t->copy());
            // Build a graph between states (for SCC, for saturation order)
            ca_gr.addEdge(t->from(), t->to());
          }
        }
      }

      std::set<IntraGraph *> gr_set;
      std::set<IntraGraph *>::iterator gr_it;
      // Iterate over all states
      for(st_map_it = state_trans_map.begin(); st_map_it != state_trans_map.end(); st_map_it++) {
        Key q = st_map_it->first;
        gr_set.clear();
        
        std::set<ITrans*> &trans_set = st_map_it->second;
        
        std::set<ITrans*>::iterator trans_it;
        
        // Create the set of IntraGraphs related to q (gr_set)
        for(trans_it = trans_set.begin(); trans_it != trans_set.end(); trans_it++) {
          
          ITrans *t = *trans_it;
          
          int nno = stk2nodeno(t->stack());
          if(nno == -1)
            continue;
          
          IntraGraph *gr = post_igr->nodes[nno].gr;
          gr_set.insert(gr);
          
          // Keep track of IntraGraphs we're going to explore (for adding MiddleTransitions later)
          if(!gr->visited) {
            gr->visited = true;
            changed_graphs.push_back(gr);
          }
          
        }
        
        // Create nodes using gr_set
        for(gr_it = gr_set.begin(); gr_it != gr_set.end(); gr_it++) {
          IntraGraph *gr = *gr_it;
          // Go through all nodes of gr and make nodes here
          for(i = 1; i < gr->nnodes; i++) {
            Transition tr = gr->nodes[i].trans;
            tr.tgt = q;

            // This creates a node for the transition
            int nno = trans2nodeno(tr);

            nodes[nno].weight = post_igr->sem->zero();
            if(tr.stack == (int)WALI_EPSILON)
              state_has_eps.insert(q);
          }
        }
        
        // Go through all transitions again and pass them onto the
        // appropriate gr (as the starting weights) 
        // and also create updatable node numbers.
        // Also, create a pop-regexp for the eps transition
        list<reg_exp_t> pop_regexp_list;
        for(trans_it = trans_set.begin(); trans_it != trans_set.end(); trans_it++) {
            
          ITrans *t = *trans_it;
          
          int nno = stk2nodeno(t->stack());
          if(nno == -1)
            continue;

          IntraGraph *gr = post_igr->nodes[nno].gr;
          Transition tr(init_state, t->stack(), q);
          int intra_nno = getIntraNodeNumber(tr);

          if(intra_nno == -1)
            continue;
          
          int cno = trans2nodeno(tr);
          if(t->from() == init_state) {
            gr->updateWeight(intra_nno, t->weight());
            nodes[cno].weight = t->weight();
          } else {
            assert(t->stack() != WALI_EPSILON);
            if(nodes[cno].uno == -1) {
              int uno = RegExp::getNextUpdatableNumber();
              reg_exp_t reg = RegExp::updatable(uno, post_igr->sem->zero()); // create the updatable node (increments updatable number)
              reg = RegExp::extend(reg, RegExp::constant(popWeight(t->stack())));
              pop_regexp_list.push_back(reg);

              gr->updateWeight(intra_nno, uno);
              nodes[cno].uno = uno;
            }
          }
        }

        popRegExpMap[q] = RegExp::combine(pop_regexp_list);

        // Go through all the IntraGraphs and get regexp from them
        for(gr_it = gr_set.begin(); gr_it != gr_set.end(); gr_it++) {
          IntraGraph *gr = *gr_it;
          gr->solveRegSummarySolution();
          gr->clearUpdates();

          // Go through all nodes of gr and get their regexp
          for(i = 1; i < gr->nnodes; i++) {
            Transition tr = gr->nodes[i].trans;
            tr.tgt = q;
            int nno = trans2nodeno(tr);
            if(nodes[nno].regexp.is_valid()) { 
              assert(tr.stack == (int)WALI_EPSILON || multiple_proc(tr.stack));
              // Take combine
              nodes[nno].regexp = RegExp::combine(nodes[nno].regexp, gr->nodes[i].regexp);
            } else {
              nodes[nno].regexp = gr->nodes[i].regexp;
            }
            assert(nodes[nno].weight.is_valid());
          }
        }
      }

      // Build the worklist
      int scc = ca_gr.runSCCdecomposition();
      cout << "SWPDS: nSCC: " << scc << "\n";

      for(set_it = states.begin(); set_it != states.end(); set_it++) {
        if(*set_it == init_state)
          continue;
        worklist.insert(tup(ca_gr.getSccNumber(*set_it), *set_it));
      }

      delete timer1;
      //post_igr->sem->printSemiringTime(cout) << "\n";

      Timer *timer = new Timer("SWPDS Saturation");
      // Finally, we're all setup to run saturation
      while(!worklist.empty()) {
        // Get a state q
        tup top = *worklist.begin();
        Key q = top.second;
        worklist.erase(top);
        assert(q != init_state);

        // The corresponding eps-transition is the out-node
        if(state_has_eps.find(q) == state_has_eps.end())
          continue;

        Transition tr(init_state, WALI_EPSILON, q);
        int nno = trans2nodeno(tr);
        assert(nodes[nno].weight.is_valid());

        sem_elem_t weight;
        if(state_used_once.find(q) == state_used_once.end()) {
          state_used_once.insert(q);
          weight = nodes[nno].regexp->get_weight();
        } else {
          weight = nodes[nno].weight->combine(popRegExpMap[q]->get_weight());
        }

        // If weight did not change from last time, continue
        if(nodes[nno].weight->equal(weight)) 
          continue;

        nodes[nno].weight = weight;

        // Go through all outgoing transition of q and do eps-contraction
        State const * state = ca_in.getState(q);
        State::const_iterator tli = state->begin();

        for( ; tli != state->end(); tli++) {

          ITrans* t = *tli;

          //assert(t->stack() != WALI_EPSILON);

          // Ignore eps-transitions in the input and also unreachale code
          if(t->stack() == WALI_EPSILON || !reachable(t->stack())) continue;

          Key qprime = t->to();

          Transition tr2(init_state, t->stack(), qprime);
          int nno2 = trans2nodeno(tr2);
          assert(nodes[nno2].weight.is_valid());
          assert(nodes[nno2].uno != -1);

          // Compute new weight (transition t decides if merge function
          // has to be applied or not)
          TaggedWeight wret(weight, walienum::RETURN);
          sem_elem_t nweight = t->apply_post(wret).getWeight();

          // New weight has been computed, take combine with old one
          nodes[nno2].weight = nodes[nno2].weight->combine(nweight);
          RegExp::update(nodes[nno2].uno, nodes[nno2].weight);

          // qprime goes onto the worklist
          worklist.insert(tup(ca_gr.getSccNumber(qprime), qprime));
        }
      }

      delete timer;
      RegExp::stopSatProcess();
      RegExp::executingPoststar(true);

      Timer *timer2 = new Timer("SWPDS: All Weights");
      // Start adding transitions to ca_out
      // Note: 
      // (1) We have to be careful about the mid-states added to ca_out
      //     because they depend on the generation number of ca_in
      // (2) The type of transition added is Trans/ETrans depending on
      //     the initial state of the transition
      // add...Trans takes care of the above two things

      assert(ca_out.getGeneration() == ca_in.getGeneration() + 1);

      // First, add all the intra-reachable transitions
      for(i=0;i<(int)nodes.size();i++) {
        if(nodes[i].regexp.is_valid()) {
          sem_elem_t wt = nodes[i].regexp->get_weight();
          addIntraTrans(nodes[i].trans, wt, ca_out);
        }
      }

      delete timer2;

      //post_igr->sem->printSemiringTime(cout) << "\n";

      // Add middle transitions
      std::list<WTransition> ls;
      std::list<WTransition>::iterator trans_it;

      getMiddleTransitions(ls);
      for(trans_it = ls.begin(); trans_it != ls.end(); trans_it++) {
        addMiddleTrans(trans_it->first, trans_it->second, ca_out);
      }

      // clean up

      // clear changed graphs
      std::list<IntraGraph *>::iterator gr_lit;
      for(gr_lit = post_igr->gr_list.begin(); gr_lit != post_igr->gr_list.end(); gr_lit++) {
        IntraGraph *gr = *gr_lit;
        gr->visited = false;
      }

      changed_graphs.clear();
      trans_map.clear();
      nodes.clear();
    }

    // Add transitions generated by the (Pop-phase + IntraQ-phase) of SWPDS.
    // All such transitions go to a (non-init) state of the input automaton.
    // They may have init of a mid-state as the source state.
    void SummaryGraph::addIntraTrans(Transition &tr, sem_elem_t wt, wfa::WFA &ca_out) {
      if(!wt.is_valid() || wt->equal(wt->zero())) return;

      // check the source state to see if an ETrans needs to be added
      if((Key)tr.src == init_state) {
        //IntraGraph::print_trans(tr, cout << "Adding Intra Transition: ", pkey) << "\n";
        ca_out.addTrans(tr.src, tr.stack, tr.tgt, wt);
      } else {
        
        // wt is the "weight at call site". To get the weight after
        // call, we would have to consult post_igr->eHandler
        // For this, we need to know the corresponding transition in post_igr
        // (because the target state of tr is one in ca_out)        
        Transition trprime(tr);
        int nno = stk2nodeno(tr.stack);
        assert(nno != -1);
        trprime.tgt = post_igr->nodes[nno].trans.tgt;

        int nret, ncall;
        assert(post_igr->exists(trprime));
        nret = post_igr->nodeno(trprime);
        
        assert(post_igr->eHandler.exists(nret));
        sem_elem_t wtCallRule = post_igr->eHandler.get_dependency(nret, ncall);
        assert(ncall != -1);
        
        // Get the Erule
        assert(eruleMap.find(trprime) != eruleMap.end());
        
        erule_t er = eruleMap[trprime];

        // We need a new mid-state
        Key src = changeStateGeneration(tr.src, ca_out.getGeneration());

        // Add the transition
        ETrans *et = new ETrans(src, tr.stack, tr.tgt,
                                wt, wt->extend(wtCallRule), er);

        //et->print(cout << "Adding Intra Etransition: ") << "\n";
        ca_out.addTrans(et);
      }
    }


    // Add transitions (init / mid-state, *, mid-state). These are the ones
    // with preprocessed weights.
    void SummaryGraph::addMiddleTrans(Transition &tr, sem_elem_t wt, wfa::WFA &ca_out) {
      if(!wt.is_valid() || wt->equal(wt->zero())) return;

      Key src = changeStateGeneration(tr.src, ca_out.getGeneration());
      Key tgt = changeStateGeneration(tr.tgt, ca_out.getGeneration());

      // check the source state to see if an ETrans needs to be added
      if((Key)tr.src == init_state) {
        //IntraGraph::print_trans(Transition(src,tr.stack,tgt), cout << "Adding Middle Transition: ", pkey) << "\n";
        ca_out.addTrans(src, tr.stack, tgt, wt);
      } else {
        
        // wt is the "weight at call site". To get the weight after
        // call, we would have to consult post_igr->eHandler
        int nret, ncall;
        assert(post_igr->exists(tr));
        nret = post_igr->nodeno(tr);
        
        assert(post_igr->eHandler.exists(nret));
        sem_elem_t wtCallRule = post_igr->eHandler.get_dependency(nret, ncall);
        assert(ncall != -1);
        
        // Get the Erule
        assert(eruleMap.find(tr) != eruleMap.end());
        
        erule_t er = eruleMap[tr];

        // Add the transition
        ETrans *et = new ETrans(src, tr.stack, tgt,
                                wt, wt->extend(wtCallRule), er);

        //et->print(cout << "Adding Middle Etransition: ") << "\n";
        ca_out.addTrans(et);
      }
    }


    Key SummaryGraph::changeStateGeneration(Key st, int gen) {
      key_src_t ks = getKeySource(st);
      GenKeySource* gks = dynamic_cast<GenKeySource*> (ks.get_ptr());
      if(gks == 0) return st;

      return getKey( new GenKeySource(gen, gks->getKey()));
    }


  } // namespace graph
} // namespace wali
