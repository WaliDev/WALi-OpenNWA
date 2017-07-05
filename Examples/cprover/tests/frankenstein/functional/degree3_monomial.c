#include "assert.h"
void main() {
    int n = __VERIFIER_nondet_int();
    int result = 0;
    __VERIFIER_assume(n >= 0);
    __VERIFIER_assume(LARGE_INT > n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < n; k++) {
                result++;
            }
        }
    }
    __VERIFIER_assert(result == n * n * n);
}
