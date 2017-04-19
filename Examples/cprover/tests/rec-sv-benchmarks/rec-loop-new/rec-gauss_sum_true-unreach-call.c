#include "assert.h"
int n, sum, i;

void rec() {
    if (i <= n) {
        sum = sum + i;
        i++;
        rec();
    }
}

int main() {
    n = __VERIFIER_nondet_int();
    __VERIFIER_assume(1 <= n && n <= 1000);
    sum = 0;
    i = 1;
    rec();
    //for(i = 1; i <= n; i++) {
    //    sum = sum + i;
    //}
    __VERIFIER_assert(2*sum == n*(n+1));
    return 0;
}
