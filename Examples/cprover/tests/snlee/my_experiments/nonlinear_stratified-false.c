#include <stdio.h>

int main() {
    int x, yyy, z;

    x = yyy = z = 0;
    //int xsquared = 0;

    while(x <= 10) {
	z = z + yyy * yyy;
	yyy = yyy + x * x;
	x = x + 1;
	//printf("%d: %d\n", x, z);

    }
    printf("z: %d\n", z);

    //assert(xsquared == x * x);


    assert(z == 154870);
    //assert(6 * yyy == x * (x - 1) * (2*x - 1));

    return 0;
}
