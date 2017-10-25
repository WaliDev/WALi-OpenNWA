#include "pow.h"

void main(int n) {
    int x = 1;
    int y = 1;
    for(int k = 0; k < n; k ++) {
        __VERIFIER_assume(x > 0);
        int v = 1;
        for(int p = 0; p < k; p++) {
            __VERIFIER_assume(v > 0);
            v = v + v;
        }
        x = x + v;
    }
    for(int m = 0; m < n; m ++) {
        __VERIFIER_assume(y > 0);
        y = y + y;
    }
    __VERIFIER_assert(x == y);
    __VERIFIER_print_hull(x);
    __VERIFIER_print_hull(y);
}
