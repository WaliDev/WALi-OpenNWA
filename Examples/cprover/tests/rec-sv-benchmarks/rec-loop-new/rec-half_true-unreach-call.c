#include "assert.h"
int i,n,k;

void rec() {
    if (i < 2*k) {
        if (i % 2 == 0) {
            n ++;
        }
        i++;
        rec();
    }
}

int main() {
    i = 0;
    n = 0;
    k = __VERIFIER_nondet_int();
    __VERIFIER_assume(k <= LARGE_INT && k >= -LARGE_INT);
    i = 0;
    rec();
    //for(i = 0; i < 2*k; i++) {
    //    if (i % 2 == 0) {
    //        n ++;
    //    }
    //}
    __VERIFIER_assert(k < 0 || n == k);
    return 0;
}
