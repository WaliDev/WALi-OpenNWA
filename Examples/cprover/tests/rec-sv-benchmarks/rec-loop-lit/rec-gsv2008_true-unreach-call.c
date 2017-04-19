// Source: Sumit Gulwani, Saurabh Srivastava, Ramarathnam Venkatesan: "Program
// Analysis as Constraint Solving", PLDI 2008.

#include "assert.h"
int x,y;

void rec() {
    if (x < 0) {
        x = x + y;
        y++;
        rec();
    }
}

int main() {
    x = -50;
    y = __VERIFIER_nondet_int();
    __VERIFIER_assume(-1000 < y && y < LARGE_INT);
    rec();
    __VERIFIER_assert(y > 0);
    return 0;
}
