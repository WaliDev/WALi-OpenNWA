//#include "stdio.h"

int main(int argc, char ** argv) {
  int x, y;
  int i, j;

  y = 0;                         //


  x = __VERIFIER_nondet_int();     //
  __VERIFIER_assume(x >= 0);       //

  
  i = 0;                           //
  while(i != x) {                  //
    j = 0;                        //
    while(j != x) {               //
      y = y + 1;                  //

      j++;                          //
    }
    i++;                             //
  }
   
  assert(y == x*x);                   //
    
  return 0;
}
