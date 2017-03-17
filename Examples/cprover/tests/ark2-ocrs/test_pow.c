#include "pow.h"

DECLARE_POW(2);

int main(int argc, char ** argv) {
    int base = 2;
    int y = __VERIFIER_nondet_int();
    __VERIFIER_assume(y >= 0);

    int i;
    int z = 1;
    for(i = 0; i < y; i++) {
        z = base * z;
    }

    assert(z == icra_pow2(y));

    __VERIFIER_print_hull(z);
}
