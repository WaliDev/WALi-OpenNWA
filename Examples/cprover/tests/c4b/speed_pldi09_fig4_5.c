// C4B output: Not Available
// SPEED output: mx(n,n-m)

unsigned int tick = 0;

void start(int n, int m, int dir)
{
  int i;

  // assert(0 < m);
  // assert(m < n);

  i = m;

  while (0 < i && i < n) {
    tick++;
    if (dir == 1)
      i++;
    else
      i--;
  }
}

int main() 
{
	int dir = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();
	__VERIFIER_assume(m > 0);
	int n = __VERIFIER_nondet_int();
	__VERIFIER_assume(n > m);
	
	start(n, m, dir);
	
	int bnd = (n>(n-m)) ? n : (n-m);
	assert (tick <= bnd);
	return 0;
}
