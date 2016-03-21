#include <stdio.h>

int x = 0;
int yyy = 0;
int xsquared = 0;

void loop() {
    if (x > 10)
        return;
    yyy = yyy  + xsquared;
    xsquared = xsquared + 2*x + 1;
    x = x + 1;
    loop();
    return;
}

int main() {
    loop();

    //assert(xsquared == x * x);

    //printf("\n final yyy %d\n", yyy);
    assert(6 * yyy == x * (x - 1) * (2*x - 1));

    return 0;
}
