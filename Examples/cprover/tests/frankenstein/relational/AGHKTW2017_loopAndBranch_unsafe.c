/* Source: Timos Antonopoulos, Paul Gazzillo, Michael Hicks, Eric Koskinen,
   Tachio Terauchi, Shiyi Wei: Decomposition Instead of Self-Composition for
   Proving the Absence of Timing Channels.  PLDI 2017 */


/* This is actually the original from the above paper. They mark the program
   safe, but we believe it is unsafe. */

#include "assert.h"

int loopAndbranch_safe(int high, int low) {
    int i = high;
    int tick = 0;
    if (low < 0) {
	while (i > 0) {
	    tick++;
	    i--;
	}
    } else {
	low = low + 10; // low is above 0
	if (low >= 10) {
	    int j = high;
	    while (j > 0) {
		j--;
		tick++;
	    }
	} else {
	    if (high < 0) {
		int k = high;
		while (k>0) {
		    k--;
		    tick++;
		}
	    }
	}
    }
    return tick; //tick == high
}

void main() {
    int high1 = __VERIFIER_nondet_int();
    int high2 = __VERIFIER_nondet_int();
    int low = __VERIFIER_nondet_int();
    __VERIFIER_assert(loopAndbranch_safe(high1,low) == loopAndbranch_safe(high2,low));
}
