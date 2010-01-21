#ifndef wali_graph__LINK_EVAL_H_
#define wali_graph__LINK_EVAL_H_

namespace wali {
    namespace graph {
        class LinkEval {
            private:
                int n;
                int *ancestor;
                int *label;
                int *semi;
            public:
                LinkEval(int _n, int *s);
                ~LinkEval();
                void link(int,int);
                int eval(int);
            private:
                void compress(int);
        };
    }
}

#endif // wali_graph__LINK_EVAL_H_
