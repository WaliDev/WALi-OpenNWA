#include "assert.h"
void loop(int x) {
    if (x < 10) {
	loop(x + 1);
    } else {
	    __VERIFIER_assert(x >= 0); // safe
	    __VERIFIER_assert(x <= 10); // safe
	    __VERIFIER_assert(x == 10); // safe
    }
}

void main() {
    loop(0);
}
