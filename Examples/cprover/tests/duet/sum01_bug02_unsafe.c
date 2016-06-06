#define a (2)

void main() { 
    int i, j=10, n=rand(), sn=0;
    assume(n >= 0);
  for(i=1; i<=n; i++) {
    if (i<j) 
    sn = sn + a;
    j--;
  }
  assert(sn==n*a || sn == 0);
}
