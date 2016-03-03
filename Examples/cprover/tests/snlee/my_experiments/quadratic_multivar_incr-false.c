#include <stdio.h>

int main() {
    int x, yyy, z;

    x = yyy = z = 0;
    //int xsquared = 0;

    while(x <= 10) {
	z = z + x * yyy;
	yyy = yyy + 3 * x;
	x = x + 1;
	//printf("%d: %d\n", x, z);

    }
    //printf("z: %d\n", z);



    int res =x * (2 * x - 1) * (x - 1) * (x - 2);
    assert( 8 * z == res);
    //assert(6 * yyy == x * (x - 1) * (2*x - 1));

    return 0;
}
