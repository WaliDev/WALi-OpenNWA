#include "assert.h"
void main() { 
    int i, n,m,r;
    n = __VERIFIER_nondet_int();
    m = __VERIFIER_nondet_int();
    __VERIFIER_assume(n > 0);
    __VERIFIER_assume(n < LARGE_INT);
    __VERIFIER_assume(m < LARGE_INT);
    i = 0;
    r = 0;
    m = m;
    n = n;
    while(i < n) {
	r = r + m;
	i++;
    }
    __VERIFIER_assert(r == n * m);
}
