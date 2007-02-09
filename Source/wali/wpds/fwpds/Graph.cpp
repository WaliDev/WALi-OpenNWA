#include "wali/wpds/fwpds/Graph.hpp"
#include <cassert>

namespace wali {

    namespace wpds {

        namespace fwpds {

            namespace graph {

                struct FinishedActionFunctor : ActionFunctor {
                    list<int> &finished;

                    FinishedActionFunctor(list<int> &_f) : finished(_f) { }

                    virtual ~FinishedActionFunctor() { }

                    virtual void operator() (int n) {
                        finished.push_front(n);
                    }
                };

                struct AssignSCCActionFunctor : ActionFunctor {
                    Graph &gr;
                    int &scc;

                    AssignSCCActionFunctor(Graph &_gr, int &_scc) : gr(_gr), scc(_scc) { }

                    virtual ~AssignSCCActionFunctor() { }

                    virtual void operator() (int n) {
                        gr.nodes[n].scc = scc;
                    }
                };

                int Graph::create_node(int n) {
                    if(env_to_node.find(n) == env_to_node.end()) {
                        int s = env_to_node.size();
                        env_to_node[n] = s;
                        //node_to_env[s] = n;
                        nodes.resize(s+1);
                        return s;
                    }
                    return env_to_node[n];
                }

                void Graph::addEdge(int s, int t) {
                    s = create_node(s);
                    t = create_node(t);

                    nodes[s].edges[0].insert(t);
                    nodes[t].edges[1].insert(s);
                }

                // Returns the number of SCCs
                int Graph::runSCCdecomposition() {
                    int i,n;
                    n = nodes.size();

                    // initialize
                    for(i=0;i<n;i++) {
                        nodes[i].visited = false;
                        nodes[i].scc = 0;
                        nodes[i].bfs = -1;
                    }

                    // Do a forward DFS and compute the finishing times
                    list<int> finished;
                    FinishedActionFunctor fin(finished);
                    for(i=0;i<n;i++) {
                        if(nodes[i].visited)
                            continue;
                        dfs(i, 0, fin);
                    }
                    assert((int)finished.size() == n); // (would not be true if everything wasn't reachable)

                    // clear visited
                    for(i=0;i<n;i++) {
                        nodes[i].visited = false;
                    }

                    // Now run a backward DFS in order of decreasing finishing time
                    list<int>::iterator it;
                    int scc = 0;
                    AssignSCCActionFunctor ascc(*this, scc);
                    for(it = finished.begin(); it != finished.end(); it++) {
                        i = *it;
                        if(nodes[i].visited)
                            continue;
                        scc++;
                        dfs(i, 1, ascc);
                    }

                    return scc;

                }

                void Graph::dfs(int node, int direction, ActionFunctor &action) {
                    nodes[node].visited = true;
                    set<int>::iterator it;
                    for(it = nodes[node].edges[direction].begin(); it != nodes[node].edges[direction].end(); it++) {
                        if(!nodes[*it].visited) 
                            dfs(*it, direction, action);
                    }
                    action(node);
                }

                int Graph::getNnodes() {
                    return nodes.size();
                }

                int Graph::getSccNumber(int n) {
                    if(env_to_node.find(n) == env_to_node.end())
                        return 0;
                    return nodes[env_to_node[n]].scc;
                }

                int Graph::getBfsNumber(int n) {
                    assert(env_to_node.find(n) != env_to_node.end());
                    return nodes[env_to_node[n]].bfs;
                }

            } // namespace graph

        } // namespace fwpds
    } // namespace wpds
} // namespace wali
