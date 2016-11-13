#include "assert.h"
int loop(int x) {
    if (x < 10) {
	return loop(x + 1);
    } else {
	return x;
    }
}

void main() {
    int x;
    x = loop(0);
	__VERIFIER_assert(x >= 0); // safe
	__VERIFIER_assert(x <= 10); // safe
	__VERIFIER_assert(x == 10); // safe
}
