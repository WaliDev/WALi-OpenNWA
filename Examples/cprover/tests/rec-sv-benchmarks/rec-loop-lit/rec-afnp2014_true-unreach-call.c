// Source: E. De Angelis, F. Fioravanti, J. A. Navas, M. Proietti:
// "Verification of Programs by Combining Iterated Specialization with
// Interpolation", HCVS 2014

#include "assert.h"

int x,y;

void rec() {
    if (y < 1000 && __VERIFIER_nondet_int()) {
        x = x + y;
        y = y + 1;
        rec();
    }
}

int main() {
    x = 1;
    y = 0;
    rec();
    __VERIFIER_assert(x >= y);
    return 0;
}
