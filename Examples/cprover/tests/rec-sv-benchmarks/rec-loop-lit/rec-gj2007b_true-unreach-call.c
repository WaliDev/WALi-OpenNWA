// Source: Sumit Gulwani, Nebosja Jojic: "Program Verification as
// Probabilistic Inference", POPL 2007.

#include "assert.h"

int x,m,n;

void rec() {
    if (x < n) {
        if(__VERIFIER_nondet_int()) {
            m = x;
        }
        x = x + 1;
        rec();
    }
}

int main() {
    x = 0;
    m = 0;
    n = __VERIFIER_nondet_int();
    rec();
    __VERIFIER_assert((m >= 0 || n <= 0));
    __VERIFIER_assert((m < n || n <= 0));
    return 0;
}
