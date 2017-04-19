// Source: Credited to Anubhav Gupta
// appears in Ranjit Jhala, Ken McMillan: "A Practical and Complete Approach
// to Predicate Refinement", TACAS 2006

#include "assert.h"

int x,y,z;

void rec() {
    if (x != 0) {
        x --;
        y -= 2;
        z ++;
        rec();
    }
}

int main() {
    int i, j;
    i = __VERIFIER_nondet_int();
    j = __VERIFIER_nondet_int();

    __VERIFIER_assume(i >= 0 && i <= LARGE_INT);
    __VERIFIER_assume(j >= 0);
    x = i;
    y = j;
    z = 0;
    rec();
    if (i == j) {
        __VERIFIER_assert(y == -z);
    }
    return 0;
}
