#define a (2)

int main() { 
  int i, n=__VERIFIER_nondet_uint(), sn=0;
  for(i=1; i<=n; i++) {
    if (i<10)
    sn = sn + a;
  }
  __VERIFIER_assert(sn==n*a || sn == 0);
}
