//extern int __VERIFIER_nondet_int();
//#include <stdio.h>

void main() {
    int x, y, result;

    x = 0;
    y = 48;
    result = 0;

    assume(x >= 0);
    while (x + 3 <= y) {
	x = x + 3;
	result = result + 3;
        //printf("%d\n", result);
        //printf("Goal: %d\n", 2 * x);
    }
    assert(result == y);
}

