#define a (2)

int main() { 
  int i, n=__VERIFIER_nondet_int(), sn=0;
  __VERIFIER_assume(n < 1000 && n >= -1000);
  for(i=1; i<=n; i++) {
    sn = sn + a;
  }
  __VERIFIER_assert(sn==n*a || sn == 0);
}
