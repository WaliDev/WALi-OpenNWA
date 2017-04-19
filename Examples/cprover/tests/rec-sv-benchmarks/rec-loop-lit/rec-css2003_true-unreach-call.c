// Source: Michael Colon, Sriram Sankaranarayanan, Henny Sipma: "Linear
// Invariant Generation using Non-Linear Constraint Solving", CAV 2003.

#include "assert.h"
int i,j,k;

void rec() {
    if (i < LARGE_INT) {
        i = i + 1;
        j = j + k;
        k = k - 1;
        __VERIFIER_assert(1 <= i + k && i + k <= 2 && i >= 1);
        rec();
    }
}

int main() {
    i = 1;
    j = 1;
    k = __VERIFIER_nondet_int();
    __VERIFIER_assume(0 <= k && k <= 1);
    rec();
    return 0;
}
