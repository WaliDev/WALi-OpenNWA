#include "assert.h"

int fib(int n, int high) {
    int f1 = 1;
    int f2 = 0;
    int temp = 0;
    if (high) {
        while(n > 0) { f1 = f1 + f2; f2 = f1 - f2; n --; }
    } else {
        while(n > 0) { temp = f2; f2 = f1; f1 = f2 + temp; n --; }
    }
    return f1;
}

void main(int argc, char ** argv) {
    int n1, n2, l1, l2, high1, high2;
    n1 = __VERIFIER_nondet_int();
    n2 = __VERIFIER_nondet_int();
    high1 = __VERIFIER_nondet_int();
    high2 = __VERIFIER_nondet_int();
    __VERIFIER_assume(n1 == n2); // Should be sufficient to make the assertion PASS
    //__VERIFIER_assume(n1 == n2 + 5); // Should but sufficient to make the assertion FAIL
    l1 = fib(n1, high1);
    l2 = fib(n2, high2);
    assert(l1 == l2);
    return 0;
}

