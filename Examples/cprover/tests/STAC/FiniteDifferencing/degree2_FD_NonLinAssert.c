//#include "stdio.h"

int main(int argc, char ** argv) {
  int x, y;
  int i, j;
  int xx, xi, ii, ij, xj, yy, yi, yj, jj, xy;

  xx = __VERIFIER_nondet_int();
  xi = __VERIFIER_nondet_int();
  ii = __VERIFIER_nondet_int();
  ij = __VERIFIER_nondet_int();
  xj = __VERIFIER_nondet_int();
  yy = __VERIFIER_nondet_int();
  yi = __VERIFIER_nondet_int();
  yj = __VERIFIER_nondet_int();
  jj = __VERIFIER_nondet_int();
  xy = __VERIFIER_nondet_int();


//xx = xx;
//xi = xi;
//ii = ii;
//ij = ij;
//xj = xj;
  // yy = 0;
  // yi = 0;
  // yj = 0;
//jj = jj;
  // xy = 0;
  y = 0;                         //


  xx = __VERIFIER_nondet_int();   //
   // xi = __VERIFIER_nondet_int();
//ii = ii;
//ij = ij;
   // xj = __VERIFIER_nondet_int();
//yy = yy;
//yi = yi;
//yj = yj;
//jj = jj;
  x = __VERIFIER_nondet_int();     //
  __VERIFIER_assume(x >= 0);       //
  // xy = x * y;

  
//xx = xx;
  xi = 0;                           //
  // ii = 0;
  // ij = 0;
//xj = xj;
//yy = yy;
  // yi = 0;
//yj = yj;
//jj = jj;
//xy = xy;
  i = 0;                           //
  while(i != x) {                  //
//  xx = xx;
//  xi = xi;                       //
//  ii = ii;
    // ij = 0;
    xj = 0;                        //
//  yy = yy;
//  yi = yi;
    // yj = 0;
    // jj = 0;
//  xy = xy;
    j = 0;                        //
    while(j != x) {               //
//    xx = xx;
//    xi = xi;
//    ii = ii;
//    ij = ij;
//    xj = xj;
      // yy = yy + 2*y + 1;
      // yi = yi + i;
      // yj = yj + j;
//    jj = jj;
      // xy = xy + x;
      y = y + 1;                  //

//    xx = xx;
//    xi = xi;
//    ii = ii;
      // ij = ij + i;
      xj = xj + x;                 //
//    yy = yy;
//    yi = yi;
      // yj = yj + y;
      // jj = jj + 2*j + 1;
//    xy = xy;
      j++;                          //
    }
    __VERIFIER_assume(xj == xx);    //
    // __VERIFIER_assume(xj == jj);

//  xx = xx;
    xi = xi + x;                    // 
    // ii = ii + 2*i + 1;
    // ij = ij + j;
//  xj = xj;
//  yy = yy;
    // yi = yi + y;
//  yj = yj;
//  jj = jj;
//  xy = xy;
    i++;                             //
  }
  __VERIFIER_assume(xi == xx);       //
  // __VERIFIER_assume(xi == ii);
   
  assert(y == x*x);                   //
    
  return 0;
}
