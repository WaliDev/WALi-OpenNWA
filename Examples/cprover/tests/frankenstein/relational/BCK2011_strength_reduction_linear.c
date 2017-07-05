/* Prove soudness of an induction variable strength reduction transformation
   via self composition.

   Source: Gilles Barthe, Juan Manuel Crespo, and Cesar Kunz: Relational
     verification using product programs,
 */

#include "assert.h"
#define B 2
#define C 3
int nondet();

void main() {
    int i1,j1,x1,i2,j2,x2,N;
    N = nondet();
    x1 = nondet();
    x2 = nondet();
    __VERIFIER_assume(N < LARGE_INT);
    __VERIFIER_assume(x1 == x2);

    // before strength reduction
    i1 = 0;
    while (i1 < N) {
	j1 = i1*B + C;
	x1 += j1;
	i1++;
    }

    // after strength reduction
    i2 = 0;
    j2 = C;
    while (i2 < N) {
	x2 += j2;
	j2 += B;
	i2++;
    }
    __VERIFIER_assert(x1 == x2);
}
