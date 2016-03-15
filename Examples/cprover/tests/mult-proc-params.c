int i = 0;

void mult(int r,int m) {
  r = r + m;
  i++;
}

void main() { 
    int r,m,n;//n = rand(), m = rand(), r;

    assume(n > 0);
    m = m;
    n = n;
    while(i < n) {
	r = mult(r,m);
    }
    assert(i == n);
    assert(r == n * m);
}
