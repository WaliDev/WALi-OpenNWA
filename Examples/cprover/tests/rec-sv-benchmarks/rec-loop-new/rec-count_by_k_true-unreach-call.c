#include "assert.h"
int i,k;

void rec() {
    if (i < LARGE_INT*k) {
        i += k;
        rec();
    }
}

int main() {
    k = __VERIFIER_nondet_int();
    __VERIFIER_assume(0 <= k && k <= 10);
    i = 0;
    rec(); //for (i = 0; i < LARGE_INT*k; i += k) ;
    __VERIFIER_assert(i == LARGE_INT*k);
    return 0;
}
