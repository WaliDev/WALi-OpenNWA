// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

#include "stdio.h"

#define max(A,B) ( (A > B) ? A : B )

int x;
int y;
int z;
int n;

void loop() {
    int t;
    for(t = 0; t <= 10; t++) {
        x = max(z, y);
        y = max(y, z);
        z = max(x, 10 * t + 50);
        printf("M.E.M.: x = %d; y = %d; z = %d;\n",x,y,z);
    }
}

void main(int initial) {
    x = y = z = initial;
    loop();
    __VERIFIER_print_hull(x);
    __VERIFIER_assert(x <= 140 || x == initial);
}
