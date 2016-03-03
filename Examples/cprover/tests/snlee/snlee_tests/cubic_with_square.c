#include <stdio.h>

int main() {
    int x, yyy;

    x = yyy = 0;
    int xsquared = 0;

    while(x <= 10) {
	yyy = yyy  + xsquared;
	xsquared = xsquared + 2*x + 1;
	x = x + 1;

    }

    //assert(xsquared == x * x);

    //printf("\n final yyy %d\n", yyy);
    assert(6 * yyy == x * (x - 1) * (2*x - 1));

    return 0;
}
