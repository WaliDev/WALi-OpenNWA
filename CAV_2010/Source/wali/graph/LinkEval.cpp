#include "wali/graph/LinkEval.hpp"

namespace wali {

    namespace graph {

        LinkEval::LinkEval(int _n, int *s) {
            n = _n;
            semi = s;
            ancestor = new int[n];
            label = new int[n];
            for(int i=0; i < n; i++) {
                ancestor[i] = -1;
                label[i] = i;
            }
        }

        LinkEval::~LinkEval() {
            delete [] ancestor;
            delete [] label;
        }

        void LinkEval::link(int v, int w) {
            ancestor[w] = v;
        }

        int LinkEval::eval(int v) {
            if(ancestor[v] == -1) {
                return v;
            }
            compress(v);
            return label[v];
        }

        void LinkEval::compress(int v) {
            if(ancestor[ancestor[v]] != -1) {
                compress(ancestor[v]);
                if(semi[label[ancestor[v]]] < semi[label[v]]) {
                    label[v] = label[ancestor[v]];
                }
                ancestor[v] = ancestor[ancestor[v]];
            }
        }
    }
}
