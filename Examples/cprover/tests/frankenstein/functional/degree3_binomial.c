#include "assert.h"
void main() {
    int n = __VERIFIER_nondet_int();
    int result = 0;
    __VERIFIER_assume(n >= 0);
    __VERIFIER_assume(LARGE_INT > n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < i; j++) {
            for(int k = 0; k < j; k++) {
                result++;
            }
        }
    }
    __VERIFIER_assert(6*result == ((n - 2) * (n - 1) * n));
}
