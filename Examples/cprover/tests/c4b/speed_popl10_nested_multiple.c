// C4B output: |[x,n]|+|[y,m]|

int __VERIFIER_nondet_int();
unsigned int tick = 0;

void start(int x, int n, int y, int m)
{
  while (x<n) {
    while (y<m) {
      if (__VERIFIER_nondet_int())
        break;
      y=y+1;
      tick++;
    }
    x=x+1;
  }
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();
	int n = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();

	start(x, n, y, m);
	
	int bnd = ((n>x)?(n-x):0) + ((m>y)?(m-y):0);
	assert(tick <= bnd);
	
	return 0;
}
