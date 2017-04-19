// Source: Thomas A. Henzinger, Thibaud Hottelier, Laura Kovacs: "Valigator:
// A verification Tool with Bound and Invariant Generation", LPAR 2008

#include "assert.h"
int res, cnt;

void rec() {
    if (cnt > 0) {
        cnt = cnt - 1;
        res = res + 1;
        rec();
    }
}

int main() {
    int a = __VERIFIER_nondet_int();
    int b = __VERIFIER_nondet_int();
    __VERIFIER_assume(a <= 1000000);
    __VERIFIER_assume(0 <= b && b <= 1000000);
    res = a;
    cnt = b;
    rec();
    __VERIFIER_assert(res == a + b);
    return 0;
}
