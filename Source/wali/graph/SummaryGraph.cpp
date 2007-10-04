#include "wali/Common.hpp"

#include "wali/util/Timer.hpp"

#include "wali/graph/SummaryGraph.hpp"
#include "wali/graph/Graph.hpp"

#include "wali/wfa/Trans.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/TransFunctor.hpp"

namespace wali {
    using wfa::Trans;
    using wfa::State;
    using util::Timer;

    namespace graph {

        SummaryGraph::SummaryGraph(InterGraph *gr, Key ss, InterGraph::PRINT_OP pop) {
            igr = gr;
            pkey = pop;
            init_state = ss;

            // Update all weights
            igr->update_all_weights();
            // Build a stack -> node map
            int i, n = (int)igr->nodes.size();
            for(i=0;i<n;i++) {
                int stk = igr->nodes[i].trans.stack;
                if(stk == 0) continue;
                if(stack_graph_map.find(stk) == stack_graph_map.end()) {
                    stack_graph_map.insert(stk, i);
                } else {
                    // Sanity check: Every stk should be in a unique IntraGraph
                    int j = stack_graph_map[stk];
                    if(igr->nodes[i].gr != igr->nodes[j].gr) {
                        pkey(stk, cout << "Node belongs to multiple procedures:") << "\n";
                        assert(0);
                    }
                }
            }
            // clear visited, and do more preprocessing
            std::list<IntraGraph *>::iterator it;
            for(it = igr->gr_list.begin(); it != igr->gr_list.end(); it++) {
                IntraGraph *gr = *it;
                gr->setupSummarySolution();
                gr->visited = false;
            }

        }

        SummaryGraph::~SummaryGraph() {
            if(igr != NULL)
                delete igr;
        }

        int SummaryGraph::stk2nodeno(int stk) {
            StackGraphMap::iterator it = stack_graph_map.find(stk);
            if(it == stack_graph_map.end()) { // we should have already seen stk
                //pkey(stk, cout << "SummaryGraph saw this for the first time:") << "\n";
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

        void SummaryGraph::addUpdate(Transition &t, SemElem *se) {

            sem_elem_t wt(se);
            int nno = getIntraNodeNumber(t);
            if(nno == -1)
                return;

            IntraGraph *gr = igr->nodes[stk2nodeno(t.stack)].gr;

            gr->updateWeight(nno, wt);
            if(!gr->visited) {
                gr->visited = true;
                changed_graphs.push_back(gr);
            }
            updated_graphs.insert(gr);
        }

        int SummaryGraph::getIntraNodeNumber(Transition &tr) {
            assert(tr.stack != (int)WALI_EPSILON);

            // Locate the stack first
            int nno = stk2nodeno(tr.stack);
            if(nno == -1) {
                pkey(tr.stack, cout << "Warning: Unreachable code (") << ")\n";
                return -1;
            }

            // get the correct intended transition
            Transition tt = tr;
            tt.tgt = igr->nodes[nno].trans.tgt;
            if(!igr->exists(tt)) {
                // Transition does not exist (This may happen when there is a call that never returns
                // and t.stack is the return node for that call)
                pkey(tr.stack, cout << "Warning: Unreachable code (") << ")\n";

                return -1;
            }

            int nno2 = igr->nodeno(tt);
            return igr->nodes[nno2].intra_nodeno;
        }

        void SummaryGraph::getUpdatedTransitions(std::list<WTransition> &ls) {
            std::set<IntraGraph *>::iterator it;

            for(it = updated_graphs.begin(); it != updated_graphs.end(); it++) {
                clock_t start = clock();

                IntraGraph *gr = *it;
                gr->solveSummarySolution(ls);
                gr->clearUpdates();

                stats.t4 += (clock() - start);

                STAT( //IntraGraphStats st = gr->get_stats();
                        //stats.t1 += st.t1;
                        //stats.t2 += st.t2;
                        //stats.t3 += st.t3;
                    );
            }

            updated_graphs.clear();
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
            int i, n = igr->nodes.size();
            for(i=0;i<n;i++) {
                if(igr->nodes[i].gr->visited) {
                    //IntraGraph::print_trans(nodes[i].trans, cout) << "\n";
                    ls.push_back(WTransition(igr->nodes[i].trans,igr->nodes[i].weight));
                }
            }

            // clear all updates
            for(it = igr->gr_list.begin(); it != igr->gr_list.end(); it++) {
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

        //template< typename T > struct CountTrans : TransActionFunctor<T> {
        struct CountTrans : public wfa::ConstTransFunctor {

            int cnt;

            CountTrans() {
                cnt = 0;
            }
            virtual ~CountTrans() { }

            virtual  void operator()(const Trans* t) {
                cnt++;
            }

            int count() {
                return cnt;
            }

        };

        void SummaryGraph::summaryPoststar(wali::wfa::WFA& ca_in, wali::wfa::WFA& ca_out) {
            int i;
            RegExp::init(igr->sem);

            typedef pair<int, Key> tup;
            std::set<tup> worklist;

            graph::Graph ca_gr;
            std::map<Key, int> state_to_node_map;
            std::map<int, Key> node_to_state_map;

            std::set<Key> states = ca_in.getStates();
            std::set<Key> state_has_eps; // states that have an incoming eps-transition

            std::map<Key, std::set<Trans *> > state_trans_map;

            std::map<Key, std::set<Trans* > >::iterator st_map_it;

            Timer *timer1 = new Timer("SWPDS: Setup");

            std::set<Key>::iterator set_it;
            // Build a map: state -> { incoming transition }
            // and a graph representing the WFA (will use for SCC decomposition)
            for(set_it = states.begin(); set_it != states.end(); set_it++) {

                wfa::State* state = ca_in.getState(*set_it);

                wfa::State::iterator tli = state->begin();;

                for( ; tli != state->end(); tli++) {

                    Trans* t = *tli;
                    if(t->stack() == WALI_EPSILON)
                        continue;
                    assert(t->to_state() != init_state);
                    state_trans_map[t->to_state()].insert(t);//.get_ptr());
                    if(t->from_state() != init_state) {
                        //ca_out.add(t->from_state(), t->stack(), t->to_state(), t->semiring_element().get_ptr());
                        ca_out.addTrans(t->from_state(), t->stack(), t->to_state(), t->weight());//.get_ptr());
                        ca_gr.addEdge(t->from_state(), t->to_state());
                    }
                }
            }

            //CountTrans<SemElem> cnt;
            //ca.for_each(cnt);
            //assert(trans_cnt == cnt.count());

			std::set<IntraGraph *> gr_set;
			std::set<IntraGraph *>::iterator gr_it;
            // Iterate over all states
            for(st_map_it = state_trans_map.begin(); st_map_it != state_trans_map.end(); st_map_it++) 
            {
                Key q = st_map_it->first;
                gr_set.clear();

                std::set<Trans*> &trans_set = st_map_it->second;

                std::set<Trans*>::iterator trans_it;

                // Create the set of IntraGraphs related to q
                for(trans_it = trans_set.begin(); trans_it != trans_set.end(); trans_it++) 
                {

                    Trans *t = *trans_it;

                    int nno = stk2nodeno(t->stack());
                    if(nno == -1) {
                        pkey(t->stack(), cout << "Warning: Unreachable code (") << ")\n";
                        continue;
                    }

                    IntraGraph *gr = igr->nodes[nno].gr;
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
                        int nno = trans2nodeno(tr);
                        nodes[nno].weight = igr->sem->zero();
                        if(tr.stack == (int)WALI_EPSILON)
                            state_has_eps.insert(q);
                    }
                }

                // Go through all transitions again and pass them onto the
                // appropriate gr and create updatable node numbers
                for(trans_it = trans_set.begin(); trans_it != trans_set.end(); trans_it++) 
                {

                    Trans *t = *trans_it;

                    int nno = stk2nodeno(t->stack());
                    if(nno == -1) {
                        continue;
                    }
                    IntraGraph *gr = igr->nodes[nno].gr;
                    Transition tr(init_state, t->stack(), q);
                    int intra_nno = getIntraNodeNumber(tr);
                    if(intra_nno == -1)
                        continue;

                    int cno = trans2nodeno(tr);
                    if(t->from_state() == init_state) {
                        gr->addInitTransition(intra_nno, t->weight().get_ptr());
                        nodes[cno].weight = t->weight();
                    } else {
                        assert(t->stack() != WALI_EPSILON);
                        if(nodes[cno].uno == -1) {
                            int uno = RegExp::getNextUpdatableNumber();
                            RegExp::updatable(uno, igr->sem->zero()); // create the updatable node (increments updatable number)

                            gr->addInitTransition(intra_nno, uno);
                            nodes[cno].uno = uno;
                        }
                    }
                }
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
                        if(nodes[nno].regexp.get_ptr() != NULL) { 
                            assert(tr.stack == (int)WALI_EPSILON);
                            // Take combine
                            nodes[nno].regexp = RegExp::combine(nodes[nno].regexp, gr->nodes[i].regexp);
                        } else {
                            nodes[nno].regexp = gr->nodes[i].regexp;
                        }
                        assert(nodes[nno].weight.get_ptr() != NULL);
                    }
                }
            }
            // Build the worklist
            int scc = ca_gr.runSCCdecomposition();
            cout << "SWPDS: nSCC: " << scc << "\n";
			for(std::set<wali::Key>::iterator set_it = states.begin(); set_it != states.end(); set_it++) {
                if(*set_it == init_state)
                    continue;
                worklist.insert(tup(ca_gr.getSccNumber(*set_it), *set_it));
            }

            delete timer1;
            // NAK-TODO
            //igr->sem->printSemiringTime(cout) << "\n";

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
                assert(nodes[nno].weight.get_ptr() != NULL);

                // If weight did not change from last time, continue
                sem_elem_t weight = nodes[nno].regexp->get_weight();
                if(nodes[nno].weight->equal(weight)) 
                    continue;

                //assert(weight->equal(weight->combine(nodes[nno].weight)));
                nodes[nno].weight = weight;

                // Go through all outgoing transition of q and do eps-contraction
                State* state = ca_in.getState(q);

                State::iterator tli = state->begin();

                for( ; tli != state->end(); tli++) {

                    Trans* t = *tli;

                    assert(t->stack() != WALI_EPSILON);
                    assert(t->to_state() != init_state);
                    Key qprime = t->to_state();

                    Transition tr2(init_state, t->stack(), qprime);
                    int nno2 = trans2nodeno(tr2);
                    assert(nodes[nno2].weight.get_ptr() != NULL);
                    assert(nodes[nno2].uno != -1);

                    sem_elem_t nweight = t->weight()->extend(weight.get_ptr());
                    nodes[nno2].weight = nodes[nno2].weight->combine(nweight);
                    RegExp::update(nodes[nno2].uno, nodes[nno2].weight);

                    // qprime goes onto the worklist
                    worklist.insert(tup(ca_gr.getSccNumber(qprime), qprime));
                }
            }
            delete timer;
            RegExp::saturationComplete();
            RegExp::executingPoststar(true);

            // TODO - NAK
            //igr->sem->printSemiringTime(cout) << "\n";

            Timer *timer2 = new Timer("SWPDS: All Weights");

            // Start adding transitions to ca_out
            for(i=0;i<(int)nodes.size();i++) {
                if(nodes[i].regexp.get_ptr() != NULL) {
                    sem_elem_t wt = nodes[i].regexp->get_weight();
                    if(!wt->equal(wt->zero())) {
                        //ca_out.add(nodes[i].trans.src, nodes[i].trans.stack, nodes[i].trans.tgt, wt.get_ptr());
                        ca_out.addTrans(nodes[i].trans.src, nodes[i].trans.stack, nodes[i].trans.tgt, wt);//.get_ptr());
                    }
                }
            }

            delete timer2;

            // TODO -NAK
            //igr->sem->printSemiringTime(cout) << "\n";

            // Add middle transitions
            std::list<WTransition> ls;
            std::list<WTransition>::iterator trans_it;

            getMiddleTransitions(ls);
            for(trans_it = ls.begin(); trans_it != ls.end(); trans_it++) {
                //ca_out.add(trans_it->first.src, trans_it->first.stack, trans_it->first.tgt, trans_it->second.get_ptr());
                ca_out.addTrans(
                        trans_it->first.src, 
                        trans_it->first.stack, 
                        trans_it->first.tgt, trans_it->second.get_ptr());
            }

            // clean up

            // clear changed graphs
            std::list<IntraGraph *>::iterator gr_lit;
            for(gr_lit = igr->gr_list.begin(); gr_lit != igr->gr_list.end(); gr_lit++) {
                IntraGraph *gr = *gr_lit;
                gr->visited = false;
            }
            changed_graphs.clear();

            RegExp::reset();
            trans_map.clear();
            nodes.clear();
            }


            } // namespace graph
        } // namespace wali
