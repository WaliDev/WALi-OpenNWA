//#include <stdio.h>
int t0, t, j, N;

int main() {

  N = __VERIFIER_nondet_int();
  __VERIFIER_assume(N >= 0);

  t0 = __VERIFIER_nondet_int();
  t = t0;

  for(j = 0; j < N; j++) {
    if (__VERIFIER_nondet_int() > 0) {
      t = t + 1;
    }
    else {
      t = t + 10;
    }
  }

  // Check several independent assertions
  if (__VERIFIER_nondet_int() > 0) {
    assert(j == N);
  }
  else if (__VERIFIER_nondet_int() > 0) {
    assert(t <= t0 + 10*N);
  }
  else {
    assert(t0 + N <= t);
  }

  return 0;
}

// We ran the following modified version of this program
//   through C4B's web interface on 2016-04-21 and we
//   got the answer shown below.  Note that we only get
//   an upper bound on the tick count.
//
// -------------------------------------------------------
// //#include <stdio.h>
// #pragma tick
// int t, j;
// 
// int main(int N, int t0, int choose) {
// 
//   assert(N >= 0);
// 
//   t = t0;
// 
//   for(j = 0; j < N; j++) {
//     if (choose > 0) {
//       t = t + 1;
//       tick(1);
//     }
//     else {
//       t = t + 10;
//       tick(10);
//     }
//   }
// 
//   return 0;
// }
// 
// -------------------------------------------------------
//
// Analysis using the tick metric:
// 10.00 |[0, N]|

