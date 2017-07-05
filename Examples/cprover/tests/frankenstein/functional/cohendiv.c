/* E. Cohen, Programming in the 1990s: an introduction to the calculation of programs. Springer-Verlag, 1990 */
#include "assert.h"
int cohendiv(int x, int y){
 int q = 0; // quotient
 int r = x; // remainder
 __VERIFIER_assume(x >= 0);
 __VERIFIER_assume(y > 0);
 while (r >= y) {
     int a = 1;
     int b = y;
     while (r >= 2 * b) {
	 a = 2 * a;
	 b = 2 * b;
     }
     r = r - b;
     q = q + a;
 }
 __VERIFIER_assert(x == q*y + r);
 __VERIFIER_assert(0 <= r && r < y);
 return q;
}
void main() {
    cohendiv(__VERIFIER_nondet_int(), __VERIFIER_nondet_int());
}
