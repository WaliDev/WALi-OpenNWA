#include "tick.h"

// Use this to allow all loops to exit early:
#define COND_MACRO && __VERIFIER_nondet_int()
// When using this macro, we don't find an upper bound

// Use this to force loops to exit according to their original loop condition:
//#define COND_MACRO
// When using this macro, we do find an upper bound, but it assumes termination

void main(int x) {
    init_tick(0);
    int y = 0;
    while(x > 0 COND_MACRO ) {
        y++;
        x--;
        tick(1);
    }
    int z = y;
    while(z > 0 COND_MACRO ) {
        int u = z - 1;
        while(u > 0 COND_MACRO ) {
            u--;
            tick(1);
        }
        z--;
        tick(1);
    }
    __VERIFIER_print_hull(__cost);
}
