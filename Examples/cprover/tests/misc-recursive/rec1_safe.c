#include "assert.h"
int x;
void loop() {
    if (x < 10) {
	x = x + 1;
	loop();
    }
}

void main() {
    x = 0;
    loop();
	__VERIFIER_assert(x >= 0); // safe
	__VERIFIER_assert(x <= 10); // safe
	__VERIFIER_assert(x == 10); // safe
}
