int i = 0;
int r = 0;
int m;

void mult() {
  r = r + m;
  i++;
}

void main() { 
    int n = __VERIFIER_nondet_int();//n = rand(), m = rand(), r;
	m = __VERIFIER_nondet_int();
    __VERIFIER_assume(n > 0);

    while(i < n) {
		mult();
    }
    assert(i == n);
    assert(r == n * m);
}
