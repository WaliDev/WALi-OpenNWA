// Source: Denis Gopan, Thomas Reps: "Lookahead Widening", CAV 2006.

#include "assert.h"

int x,y;

void rec() {
    if (1) {
        if (x < 50) {
            y++;
        } else {
            y--;
        }
        if (y < 0) return;
        x++;
        rec();
    }
}

int main() {
    x = 0;
    y = 0;
    rec();
    __VERIFIER_assert(x == 100);
    return 0;
}
