int main (int argc, char ** argv){
  int n1 = __VERIFIER_nondet_int();
  int n2 = __VERIFIER_nondet_int();
  __VERIFIER_assume(n1 == n2);

  int size = __VERIFIER_nondet_int();
  __VERIFIER_assume(size>n1 && n1 > 2);

  int secret1[size];
  int secret2[size];
  int time1 = 0;
  int time2 = 0;

  int i1 = 0;
  int i2 = 0;

  while ( n1 >=2 ){
    n1 = n1 / 2;
    n2 = n2 / 2;
    if (secret1[i1] > 0) time1++;
    else time1++;
    if (secret2[i2] > 0) time2++;
    else time2++;
    i1++;
    i2++;
  }
  
  __VERIFIER_assert(time1 == time2);
}
