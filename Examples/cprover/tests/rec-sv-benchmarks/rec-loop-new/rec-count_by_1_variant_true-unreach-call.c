#include "assert.h"

int i;

void rec() {
    if (i != LARGE_INT) {
        __VERIFIER_assert(i <= LARGE_INT);
        i++;
        rec();
    }
}

int main() {
    i = 0;
    rec(); //for (i = 0; i != LARGE_INT; i++) {
	//__VERIFIER_assert(i <= LARGE_INT);
    //}
    return 0;
}
