#include "assert.h"

/* Source: Timos Antonopoulos, Paul Gazzillo, Michael Hicks, Eric Koskinen,
   Tachio Terauchi, Shiyi Wei: Decomposition Instead of Self-Composition for
   Proving the Absence of Timing Channels.  PLDI 2017 */
int bar (int high, int low) {
    int tick = 0;
    int i;
    if (low > 0) {
	i = 0;
	while (i < low) {
	    i++;
	    tick++;
	}
	while (i > 0) {
	    i--;
	    tick++;
	}
    } else {
	if (high == 0) {
	    i = 5;
	} else {
	    i = 0;
	    i++;
	}
    }
    return tick;
}

void main() {
    int high1 = __VERIFIER_nondet_int();
    int high2 = __VERIFIER_nondet_int();
    int low = __VERIFIER_nondet_int();
    __VERIFIER_assert(bar(high1,low) == bar(high2,low));
}
