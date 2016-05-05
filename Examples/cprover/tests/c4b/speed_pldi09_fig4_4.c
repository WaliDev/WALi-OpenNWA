//// C4B output : |[0,n]|

unsigned int tick = 0;

void start(int n, int m)
{
  int i=n;

//replaced this with the assumption in main
//assert(0 < m);

  while (i > 0) {
	tick++;
    if (i < m)
      i=i-1;
    else
      i=i-m;
  }
}

int main() 
{
	int n = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();
	__VERIFIER_assume(m > 0);
		
	start(n, m);
	
	int bnd = n>0?n:0;
	assert(tick <= bnd);
	return 0;
}
