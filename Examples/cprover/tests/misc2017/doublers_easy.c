#include "pow.h"

//DECLARE_POW(2);

void main(int n) {
    int x = 1;
    int y = 1;
    for(int k = 0; k < n; k ++) {
        __VERIFIER_assume(x > 0);
        int v = 1;
        for(int p = 0; p < k; p++) {
            v = v + v;
        }
        x = x + v;
    }
    for(int k = 0; k < n; k ++) {
        __VERIFIER_assume(y > 0);
        int v = 1;
        for(int p = 0; p < k; p++) {
            v = v + v;
        }
        y = y + v;
    }
    __VERIFIER_assert(x == y);
}
