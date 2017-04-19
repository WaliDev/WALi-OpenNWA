// Source: A. Costan, S. Gaubert, E. Goubault, M. Martel, S. Putot: "A Policy
// Iteration Algorithm for Computing Fixed Points in Static Analysis of
// Programs", CAV 2005

#include "assert.h"

int i,j,k;

void rec2() {
    if (j < 20) {
        j = i + j;
        rec2();
    }
}

void rec3() {
    if (k <= 3) {
        k = k + 1;
        rec3();
    }
}

void rec1() {
    if (i <= 100) {
        i = i + 1;
        rec2();
        k = 4;
        rec3();
        rec1();
    }
}

int main() {
    i = 0;
    k = 9;
    j = -100;
    rec1();
    __VERIFIER_assert(k == 4);
    return 0;
}
