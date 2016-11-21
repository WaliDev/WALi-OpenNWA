//#include "stdio.h"

int main(int argc, char ** argv) {
  int x, y;
  int i, j, k;
    
  y = 0;
  x = __VERIFIER_nondet_int();
  __VERIFIER_assume(x >= 0);

  //x = 3;

  int xSq = x * x;
  
    
  for(i = 0; i < x; i ++) {
    for(j = 0; j < x; j ++) {
      for(k = 0; k < x; k ++) {
	y = y + x;
      }
    }
  }
   
  //printf("y = %d\n", y);  // "y = 81"
  assert(y == x * x * x * x);
    
  return 0;
}
