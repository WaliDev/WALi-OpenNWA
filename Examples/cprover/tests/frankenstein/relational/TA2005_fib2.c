/* Source: Tachio Terauchi and Alex Aiken: Secure Information Flow as a Safety
   Problem, SAS 2005.  Figure 9. */
#include "assert.h"

void main() {
    int f11, f12, n1, l1, x1, i1;
    int f21, f22, n2, l2, x2, i2;
    f11 = __VERIFIER_nondet_int();
    f12 = __VERIFIER_nondet_int();
    f21 = __VERIFIER_nondet_int();
    f22 = __VERIFIER_nondet_int();
    l1 = __VERIFIER_nondet_int();
    l2 = __VERIFIER_nondet_int();
    n1 = __VERIFIER_nondet_int();
    n2 = __VERIFIER_nondet_int();
    i1 = __VERIFIER_nondet_int();
    i2 = __VERIFIER_nondet_int();

    __VERIFIER_assume(f11 == f21);
    __VERIFIER_assume(f12 == f22);
    __VERIFIER_assume(n1 == n2);
    __VERIFIER_assume(l1 == l2);
    __VERIFIER_assume(x1 == x2);
    __VERIFIER_assume(i1 == i2);
    int h1, h2; // high security
    // variant 1
    while(n1 > 0) {
	f11 = f11 + f12;
	f12 = f11 - f12;
	n1 --;
    }
    if (h1) {
	x1 = 1;
    }
    if (!h1) {
	x1 = 1;
    }
    while(i1 < f11) {
	l1 = l1 + x1;
	i1 = i1 + 1;
    }
    // variant 2
    while(n2 > 0) {
	f21 = f21 + f22;
	f22 = f21 - f22;
	n2 --;
    }
    if (h2) {
	x2 = 1;
    }
    if (!h2) {
	x2 = 1;
    }
    while(i2 < f21) {
	l2 = l2 + x2;
	i2 = i2 + 1;
    }
    __VERIFIER_assert(l1 == l2);
}
