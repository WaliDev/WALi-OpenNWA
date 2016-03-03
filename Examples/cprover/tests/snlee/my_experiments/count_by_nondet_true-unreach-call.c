#include "assert.h"
#include "stdio.h"
void main() {
    int i = 0;
    int k = 0;
    while(i < LARGE_INT) {
	int j = __VERIFIER_nondet_int();
       	__VERIFIER_assume(1 <= j && j < LARGE_INT);
	i = i + j;
	k ++;
    }
    //printf("k: %d\n", k);
    __VERIFIER_assert(k <= LARGE_INT);
}
