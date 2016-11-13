#include "assert.h"
int x;

void p(int v) {
  if (v != 1) {
    if ((v/2)*2 == v) { /* v is even */

      x = x + 1;
      p(v/2);
      x = x - 1;
    }
    else { /* v is odd */

      x = x - 1;
      p(3*v + 1);
      x = x + 1;

    }
  }
}

int main() {
  int v = __VERIFIER_nondet_int();
  __VERIFIER_assume(v > 0);
  int n = __VERIFIER_nondet_int();
  __VERIFIER_assume(n > 0);
  
  
  x = n;
  p(v);
  
  __VERIFIER_assert(x==n);
}
