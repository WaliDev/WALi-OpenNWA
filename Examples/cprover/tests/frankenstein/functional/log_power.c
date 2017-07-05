#include "assert.h"
int pow2(int n) {
    int i;
    int r = 1;
    for(i = 0; i < n; i++) {
	r *= 2;
    }
    return r;
}

// base 2 logarithm, rounded down
int log2(int n) {
    int j;
    int z = 0;
    for(j = 1; 2*j <= n; j *= 2) {
	z ++;
    }
    return z;
}

void main() {
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assume(n >= 1);
    __VERIFIER_assert(pow2(log2(n)) <= n);
    __VERIFIER_assert(n < pow2(log2(2*n)));
}
