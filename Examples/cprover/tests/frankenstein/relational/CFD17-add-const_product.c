int main () {
  int epsilon = 0;  

  int n1, n2, k1, k2;

  __VERIFIER_assume(n1>=0);
  __VERIFIER_assume(k1>=0);

  __VERIFIER_assume(n1 == n2);
  __VERIFIER_assume(k1 == k2);

  int time1 = 0;
  int i1 = 0;
  int time2 = 0;
  int i2 = 0;

  while (i1 < n1) {
    time1 = time1 + i1;
    i1 = i1 + k1;
    time2 = time2 + i2;
    i2 = i2 + k2;
  }
  
  __VERIFIER_assert((time1-time2) <= epsilon && (time2 - time1) <= epsilon);
}
