#include <stdio.h>

int main() {
    int x, yyy;

    x = yyy = 0;

    while(x <= 10) {
	yyy = yyy  + x*x;
	x = x + 1;

    }

    //printf("\n final yyy %d\n", yyy);
    assert(6 * yyy ==  x * (x - 1) * (2*x - 1));

    return 0;
}
