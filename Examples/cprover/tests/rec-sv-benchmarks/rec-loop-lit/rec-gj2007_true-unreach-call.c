// Source: Sumit Gulwani, Nebosja Jojic: "Program Verification as
// Probabilistic Inference", POPL 2007.

#include "assert.h"

int x,y;

void rec() {
    if (x < 100) {
        if (x < 50) {
            x = x + 1;
        } else {
            x = x + 1;
            y = y + 1;
        }
        rec();
    }
}

int main() {
    x = 0;
    y = 50;
    rec();
    __VERIFIER_assert(y == 100);
    return 0;
}
