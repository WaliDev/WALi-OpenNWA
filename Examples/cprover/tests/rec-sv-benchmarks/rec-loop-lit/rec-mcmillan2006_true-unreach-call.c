// Source: Ken McMillan: "Lazy Abstraction With Interpolants", CAV 2006

#include "assert.h"

int *x;
int i,n;

void rec1() {
    if (i < n) {
        x[i] = 0;
        i++;
        rec1();
    }
}

void rec2() {
    if (i < n) {
        __VERIFIER_assert(x[i] == 0);
        i++;
        rec2();
    }
}

int main() {
    n = __VERIFIER_nondet_int();
    __VERIFIER_assume(0 <= n && n <= 1000);
    x = malloc(n * sizeof(int));
    i = 0;
    rec1(); //for (int i = 0; i < n; i++) x[i] = 0;
    i = 0;
    rec2(); //for (int i = 0; i < n; i++) __VERIFIER_assert(x[i] == 0);
    return 0;
}
