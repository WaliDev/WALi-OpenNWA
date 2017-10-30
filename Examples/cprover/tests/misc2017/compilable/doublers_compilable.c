#include "stdio.h"
#include "stdlib.h"

int main(int argc, char ** argv) {
    if (argc != 2) { printf("USAGE: ???\n"); return 0; }
    int n = atoi(argv[1]);
    int x = 1;
    int y = 1;
    int k;
    int p;
    int m;
    int v;
    for(k = 0; k < n; k ++) {
        //__VERIFIER_assume(x > 0);
        v = 1;
        for(p = 0; p < k; p++) {
            //__VERIFIER_assume(v > 0);
            v = v + v;
        }
        x = x + v;
    }
    for(m = 0; m < n; m ++) {
        //__VERIFIER_assume(y > 0);
        y = y + y;
    }
    printf("x = %d\n",x);
    printf("y = %d\n",y);
    //__VERIFIER_assert(x == y);
    //__VERIFIER_print_hull(x);
    //__VERIFIER_print_hull(y);
    return 0;
}
