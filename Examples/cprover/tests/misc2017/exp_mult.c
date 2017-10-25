#include "pow.h"

DECLARE_POW(2);

void main(int x, int y) {
    __VERIFIER_assume(x >= 1);
    __VERIFIER_assume(y >= 1);
    int a = icra_pow2(x + y);
    int b = icra_pow2(x);
    int c = icra_pow2(y);
    __VERIFIER_assert(a == b * c);
}
