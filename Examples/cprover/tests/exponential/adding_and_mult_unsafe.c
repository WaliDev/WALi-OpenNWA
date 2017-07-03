void main() {
  int i, j, k;
  int n;
  __VERIFIER_assume(n >= 1);

  j = 1;
  k = 1;

  for (i=0; i < n; i++){
    j = 2*j;
    k = k+k;
  }
  __VERIFIER_assert(j != k);
}
