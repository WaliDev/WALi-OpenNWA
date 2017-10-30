// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

#include "stdio.h"

#define max(A,B) ( (A > B) ? A : B )

int x;
int y;
int z;
int n;

void loop() {
    for(int t = 0; t <= 10; t++) {
        x = max(x, y);
        y = max(y, z);
        z = max(z, 10 * t + 50);
        printf("x = %d; y = %d; z = %d;\n",x,y,z);
    }
}

void main() {
    x = y = z = 0;
    loop();
    __VERIFIER_print_hull(x);
    __VERIFIER_assert(x <= 150);
}
