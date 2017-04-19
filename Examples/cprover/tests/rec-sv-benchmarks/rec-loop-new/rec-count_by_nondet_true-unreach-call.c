#include "assert.h"
int i, k;

void rec() {
    if (i < LARGE_INT) {
        int j = __VERIFIER_nondet_int();
        __VERIFIER_assume(1 <= j && j < LARGE_INT);
        i = i + j;
        k ++;
        rec();
    }
}

int main() {
    i = 0;
    k = 0;
    rec();
    __VERIFIER_assert(k <= LARGE_INT);
    return 0;
}
