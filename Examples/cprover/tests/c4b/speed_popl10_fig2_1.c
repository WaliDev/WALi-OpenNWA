// C4B output: |[x,n]|+|[y,m]|

unsigned int tick = 0;

void start(int x, int y, int n, int m)
{
  while (n>x) {
	tick++;
    if (m>y)
      y = y+1;
    else
      x = x+1;
  }
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();
	int n = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();
	
	start(x, y, n, m);
	
	int bnd = ((n>x)?(n-x):0) + ((m>y)?(m-y):0);
	assert(tick <= bnd);
	
	return 0;
}
