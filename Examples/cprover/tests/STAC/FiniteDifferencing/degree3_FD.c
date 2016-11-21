//#include "stdio.h"

int main(int argc, char ** argv) {
  int x, y;
  int i, j;
  int xxx, xxi, xxj, xj, xx;

  xxx = __VERIFIER_nondet_int();
  xxi = __VERIFIER_nondet_int();
  //  xxj = __VERIFIER_nondet_int();
  xj  = __VERIFIER_nondet_int();

  y = 0;

  xxx = __VERIFIER_nondet_int();
  __VERIFIER_assume(xxx >= 0);
  xx = __VERIFIER_nondet_int();
  __VERIFIER_assume(xx >= 0);
  x = __VERIFIER_nondet_int();
  __VERIFIER_assume(x >= 0);

  
  xxi = 0;
  i = 0;
  while(i != x) {
    //    xxj = 0;
    xj = 0;
    j = 0;
    while(j != x) {
      y = y + x;

      // xxj = xxj + xx;
      xj = xj + x;
      j++;
    }
    __VERIFIER_assume(xj == xx);
    //    __VERIFIER_assume(xxj == xxx);

    xxi = xxi + xx;
    i++;
  }
  __VERIFIER_assume(xxi == xxx);
   
  assert(y == xxx);
  //  assert(y == x*x*x);
    
  return 0;
}
