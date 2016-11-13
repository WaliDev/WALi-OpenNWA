#include "assert.h"
int n;
int m;
void mult(int r, int i) {
    if (i < n) {
	mult(r + m, i + 1);
    } else {
	__VERIFIER_assert(i == n);
	__VERIFIER_assert(r == n * m);
    }
}
void main() { 
    __VERIFIER_assume(n > 0);
    mult(0, 0);
}
