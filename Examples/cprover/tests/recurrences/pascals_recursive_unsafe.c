//P(i, j) = 1 if i=0
//P(i, j) = 0 if i>j
//P(i,j) = P(i, j-1) + P(i-1, j-1) otherwise
//P(i,j) = j_Choose_i

#include "tick.h"

void triangle(int i, int j) {
    if (i<0 || j<0 || i<j) {
        return;
    }
    if (j==0) {
        tick(1);
        return;
    }
    triangle(i-1, j);
    triangle(i-1, j-1);
}

int main() {
    init_tick(0);
    triangle(9, 4);
    assert(__cost == 100);
    return 0;
}
