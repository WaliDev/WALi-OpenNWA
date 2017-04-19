#include "assert.h"

int i;

void rec() {
    if (i < LARGE_INT) {

        i+=2;
        rec();
    }
}

int main() {
    i = 0;
    rec(); //for (i = 0; i < LARGE_INT; i += 2) ;
    __VERIFIER_assert(i == LARGE_INT);
    return 0;
}
