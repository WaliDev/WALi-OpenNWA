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
  assert(j != N || j == N);
  if (__VERIFIER_nondet_int() > 0) {
    assert(t <= t0 + 10*N);
  }
  else {
    assert(t0 + N <= t);
  }

  return 0;
}
