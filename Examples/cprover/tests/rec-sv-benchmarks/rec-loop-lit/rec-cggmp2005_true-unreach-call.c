// Source: A. Costan, S. Gaubert, E. Goubault, M. Martel, S. Putot: "A Policy
// Iteration Algorithm for Computing Fixed Points in Static Analysis of
// Programs", CAV 2005

#include "assert.h"
int i,j;

void rec() {
    if (j >= i) {
        i = i + 2;
        j = -1 + j;
        rec();
    }
}

int main() {
    i = 1;
    j = 10;
    rec();
    __VERIFIER_assert(j == 6);
    return 0;
}
