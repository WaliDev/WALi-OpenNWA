#include <stdio.h>

int main() {
    int x, yyy;
    int xcubed, xsquared, xfourth;

    x = yyy = 0;
    xsquared = xcubed = xfourth = 0;

    while(x <= 10) {
	yyy = yyy  + 4 * xfourth - 3 * xcubed + 2 * xsquared - x + 1;
	xfourth = xfourth + 4 * xcubed + 6 * xsquared + 4*x + 1;
	xcubed = xcubed + 3 * xsquared + 3 * x + 1;
	xsquared = xsquared + 2*x + 1;
	x = x + 1;
        
    }

    //printf("\n final yyy %d\n", yyy);
    //assert(yyy == 9);
    int result = x * (34 + x * (-45 + x * (70 + x * (-55 + 16 * x))));
    assert (yyy == result);
    //assert(20 * yyy == x * (34 + x * (-45 + x * (70 + x * (-55 + 16 * x)))));

    return 0;
}
