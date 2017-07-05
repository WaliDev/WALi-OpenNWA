#include "assert.h"

/* Source: Timos Antonopoulos, Paul Gazzillo, Michael Hicks, Eric Koskinen,
   Tachio Terauchi, Shiyi Wei: Decomposition Instead of Self-Composition for
   Proving the Absence of Timing Channels.  PLDI 2017 */
int foo (int high, int low) {
    int tick = 0;
    int i;
    if (high == 0) {
	i = 0;
	while(i < low) {
	    i++;
	    tick++;
	}
    } else {
	i = low;
	while(i > 0) {
	    i--;
	    tick++;
	}
    }
    return tick;
}

void main() {
    int high1 = __VERIFIER_nondet_int();
    int high2 = __VERIFIER_nondet_int();
    int low = __VERIFIER_nondet_int();
    __VERIFIER_assert(foo(high1,low) == foo(high2,low));
}
