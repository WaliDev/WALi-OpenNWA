// C4B output : |[0,m]|+|[0,n]|

unsigned int tick = 0;

void start(int n, int m)
{
  int x=0;
  int y=0;

  while (x<n) {
    tick++;
	if (y<m)
      y=y+1;
    else
      x=x+1;
  }
}

int main() 
{
	int n = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();

	start(n, m);
	
	int bnd = ((n>0)?n:0) + ((m>0)?m:0);
	assert(tick <= bnd);
	
	return 0;
}
