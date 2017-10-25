#include "pow.h"

DECLARE_POW(2);

void main(int n) {
    __VERIFIER_assume(n >= 0);
    int x = 1;
    int y = 1;
    for(int k = 0; k < n; k ++) {
        __VERIFIER_assume(x > 0);
        x = x + icra_pow2(k);
    }
    for(int k = 0; k < n; k ++) {
        __VERIFIER_assume(y > 0);
        y = y + icra_pow2(k);
    }
    __VERIFIER_assert(x == y);
    __VERIFIER_print_hull(x);
    __VERIFIER_print_hull(y);
}
