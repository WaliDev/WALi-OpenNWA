// Source: Dirk Beyer, Thomas A. Henzinger, Rupak Majumdar, Andrey
// Rybalchenko: "Path Invariants", PLDI 2007.

#include "assert.h"
int i, n, a, b;

void rec() {
    if (i < n) {
        if (__VERIFIER_nondet_int()) {
            a = a + 1;
            b = b + 2;
        } else {
            a = a + 2;
            b = b + 1;
        }
        i = i + 1;
        rec();
    }
}

int main() {
    i = 0; a = 0; b = 0; n = __VERIFIER_nondet_int();
    __VERIFIER_assume(n >= 0 && n <= LARGE_INT);
    rec();
    __VERIFIER_assert(a + b == 3*n);
    return 0;
}
