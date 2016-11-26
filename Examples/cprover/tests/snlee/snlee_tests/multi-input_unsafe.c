#include <stdio.h>

int main() {
    int x, yyy, z;

    x = yyy = z = 0;
    //int xsquared = 0;

    while(x <= 10) {
	z = z + yyy * yyy + x * x;
	yyy = yyy + 3*x;
	x = x + 1;
	//printf("%d: %d\n", x, z);
	//printf("Poly %d: %d\n", x, 5 * (x - 1) * x * (2*x - 1) / 3);

    }
    //printf("z: %d\n", z);

    //assert(xsquared == x * x);


    int result = x * (x - 1) * (27 * x * x * x - 108 * x*x + 137 * x - 28); 
    assert(60 * z == result * x);
    //assert(z == 3851);

    return 0;
}
