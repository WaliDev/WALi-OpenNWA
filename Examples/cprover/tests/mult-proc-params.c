int i = 0;

int mult(int r,int m) {
  r = r + m;
  i++;
  return r;
}

void main() { 
    int n = __VERIFIER_nondet_int();//n = rand(), m = rand(), r;
	int m = __VERIFIER_nondet_int();
	int r = 0;
    __VERIFIER_assume(n > 0);
	
    while(i < n) {
		r = mult(r,m);
    }
	
    assert(i == n);
    assert(r == n * m);
}
