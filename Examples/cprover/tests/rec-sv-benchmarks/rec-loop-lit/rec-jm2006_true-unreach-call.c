// Source: Credited to Anubhav Gupta
// appears in Ranjit Jhala, Ken McMillan: "A Practical and Complete Approach
// to Predicate Refinement", TACAS 2006

#include "assert.h"

int x,y;

void rec() {
    if (x != 0) {
        x--;
        y--;
        rec();
    }
}

int main() {
    int i, j;
    i = __VERIFIER_nondet_int();
    j = __VERIFIER_nondet_int();
    __VERIFIER_assume(i >= 0 && j >= 0);
    x = i;
    y = j;
    rec();

    if (i == j) {
        __VERIFIER_assert(y == 0);
    }
    return 0;
}
