int i = 0;
int r = 0;
int m;

void mult() {
  r = r + m;
  i++;
}

void main() { 
    int n;//n = rand(), m = rand(), r;

    assume(n > 0);
    m = m;
    n = n;
    while(i < n) {
	mult();
    }
    assert(i == n);
    assert(r == n * m);
}
