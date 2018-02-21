int main (){
  
  int n1, k1;
  int secret1, secret2;

  if (n1<0 || k1 < 0) return 0;
  int n2 = n1;
  int k2 = k1;

  int i1 = 0;
  int time1 = 0;

  int i2 = 0;
  int time2 = 0;

  if (secret1 > 0){
    while (i1 < n1) {
      time1 = time1 + i1;
      i1 = i1 + k1;
    }
    time1 = time1 + 1;
  } else {
    while (i1 < n1) {
      time1 = time1 + i1;
      i1 = i1 + k1;
    }
  }
  
  if (secret2 > 0){
    while (i2 < n2) {
      time2 = time2 + i2;
      i2 = i2 + k2;
    }
    time2 = time2 + 1;
  } else {
    while (i2 < n2) {
      time2 = time2 + i2;
      i2 = i2 + k2;
    }
  }

  __VERIFIER_assert(time1 - time2 <= 1 && time2 - time1 <=1);
  return 0;
}
