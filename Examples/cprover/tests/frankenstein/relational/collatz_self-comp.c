int main () {
  int N1, size;
  if (N1 < 0) return 0;
  if (size <= N1) return 0;

  int N2 = N1;
  int secret1[size];
  int secret2[size];

  int e1 = 0;
  int o1 = 0;
  int i1;
  for (i1=0; i1 < N1; i1++){
    if(secret1[i1] % 2 == 0) e1++;
    else o1++;
  }
  int n1 = e1 + o1;
  int t1;
  for(t1=0; n1 != 1; t1++){
    if(n1%2 == 0) n1 = n1/2;
    else n1 = 3*n1+1;
  }

  int e2 = 0;
  int o2 = 0;
  int i2;
  for (i2=0; i2 < N2; i2++){
    if(secret2[i2] % 2 == 0) e2++;
    else o2++;
  }
  int n2 = e2 + o2;
  int t2;
  for(t2=0; n2 != 1; t2++){
    if(n2%2 == 0) n2 = n2/2;
    else n2 = 3*n2+1;
  }

  __VERIFIER_assert(t1 == t2);
  return 0;
}
