#include "assert.h"
void main () {
    int i,j,x,y;
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assume(n > 0);
    __VERIFIER_assume(n < LARGE_INT);
    // variant 1
    x = 0;
    i = 0;
    while (i <= n) {
	x += i;
	i++;
    }

    // variant 2
    y = 0;
    j = 1;
    while (j <= n) {
	y += j;
	j++;
    }
    __VERIFIER_assert(x == y);
}
