#include "assert.h"
void main() {
    int n = __VERIFIER_nondet_int();
    int m = __VERIFIER_nondet_int();
    int k = 0;
    int i,j;
    __VERIFIER_assume(10 <= n);
    __VERIFIER_assume(n <= 10000);
    __VERIFIER_assume(10 <= m);
    __VERIFIER_assume(m <= 10000);
    for (i = 0; i < n; i++) {
	for (j = 0; j < m; j++) {
	    k ++;
	}
    }
    __VERIFIER_assert(k >= 100);
}
