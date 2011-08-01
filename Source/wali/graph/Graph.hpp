#ifndef wali_graph__MY_GRAPH_H_
#define wali_graph__MY_GRAPH_H_

#include <set>
#include <vector>
#include <list>
#include <map>
using namespace std;

namespace wali {

    namespace graph {

        struct ActionFunctor {
            ActionFunctor() { }
            virtual ~ActionFunctor() { }
            virtual void operator() (long int n) = 0;
        };

        class Node {
            public:
                set<int> edges[2]; // 0 - forward, 1 - backward
                bool visited;
                int scc;
                int bfs;
                Node() {
                    visited = false;
                    scc = 0;
                    bfs = 0;
                }
                Node(const Node &n) : visited(n.visited), scc(n.scc), bfs(n.bfs) { 
                    edges[0] = n.edges[0];
                    edges[1] = n.edges[1];
                }
        };

        struct AssignSCCActionFunctor;

        class Graph {
            friend struct AssignSCCActionFunctor;

            vector<Node> nodes;
            map<long int, long int> env_to_node;
            //map<int, int> node_to_env;

            public:
            void addEdge(int s, int t);
            int runSCCdecomposition();

            size_t getNnodes();
            int getSccNumber(int n);
            int getBfsNumber(int n);

            private:
            inline long int create_node(int n);
            void dfs(long node, int direction, ActionFunctor &action);
        };

    } // namespace graph

} // namespace wali

#endif  // wali_graph__MY_GRAPH_H_
