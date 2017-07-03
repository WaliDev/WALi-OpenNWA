#include "tick.h"
#include "log.h"

DECLARE_LOG(2)

void halving(int n) {
    while(n >= 2) {
        n = n / 2;
        tick(1);
    }
    __VERIFIER_print_hull(__cost);
}

int main(int argc, char ** argv) {
    init_tick(0);

    int N = __VERIFIER_nondet_int();
    //__VERIFIER_assume(N > 0);

    halving(N);

    int lg_N = icra_log2(N); // does this assert exists k . N == 2^k ?

    __VERIFIER_assert(__cost <= lg_N);
    return 0;
}
