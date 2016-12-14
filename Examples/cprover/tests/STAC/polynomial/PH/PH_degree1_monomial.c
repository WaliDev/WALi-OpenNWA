#include "tick.h"

void work(int n) {
    for(int i = 0; i < n; i++) {
        tick(1);
    }
    __VERIFIER_print_hull(__cost);
}

int main(int argc, char ** argv) {
    init_tick(0);
    int P = (argv[1] != 0) ? atoi(argv[1]) : 0;
    work(P);
    return 0;
}
