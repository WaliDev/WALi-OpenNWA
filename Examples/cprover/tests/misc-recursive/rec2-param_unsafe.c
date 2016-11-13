#include "assert.h"
void loop(int x) {
    if (x < 10) {
	loop(x + 1);
    } else {
	    __VERIFIER_assert(x <= 1); // unsafe
    }
}

void main() {
    loop(0);
}
