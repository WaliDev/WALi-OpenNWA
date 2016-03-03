#include <stdio.h>

int main() {
    int x, y;
    int xcubed, xsquared;

    x = y = 0;
    xsquared = xcubed = 0;

    while(x <= 10) {
	y = y  + xcubed;
	xcubed = xcubed + 3 * xsquared + 3 * x + 1;
	xsquared = xsquared + 2*x + 1;
	x = x + 1;

    }

    //printf("\n final y %d\n", y);
    assert(4 * y == x * x * (x - 1) * (x - 1));

    return 0;
}
