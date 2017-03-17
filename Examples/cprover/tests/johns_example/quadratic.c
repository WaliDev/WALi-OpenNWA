#include "tick.h"

void aux2 (int j, int stopj, int n);

void aux1 (int start, int n);

void aux2(int j, int stopj, int n){
  if (j>stopj){
    aux1(stopj, n);
  } else {
    tick(1);
    aux2 (j+1, stopj, n);
  }
}

void aux1 (int start, int n){
  if (start>n) return;
  else aux2 (0, start+1, n);
}

void quadratic(int n){
  aux1(0, n);
  __VERIFIER_print_hull(__cost);
}

int main () {
  init_tick(0);
  int k = __VERIFIER_nondet_int();
  __VERIFIER_assume(k>=0);
  quadratic(k);
}
