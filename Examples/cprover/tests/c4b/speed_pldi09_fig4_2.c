// C4B output: 1+2|[0,n]|

unsigned int tick = 0;

void start(int n, int m)
{
  int va=n;
  int vb=0;

/* assert(n > 0); */
// assert(m > 0);

  while (va > 0) {
	tick++;
    if (vb < m) { 
      vb=vb+1; 
      va=va-1;
    } else {
      vb=vb-1;
      vb=0;
    }
  }
}

int main() 
{
	int n = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();
	__VERIFIER_assume(m > 0);
	
	start(n, m);
	
	int bnd = 1 + 2 * (n>0?n:0);
	assert(tick <= bnd);
	return 0;
}
