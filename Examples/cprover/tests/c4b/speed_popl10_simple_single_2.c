// C4B output: |[0,n]|+|[0,m]|

int __VERIFIER_nondet_int();
unsigned int tick = 0;

void start(int n, int m)
{
  int x=0;
  int y=0;

  for (;;) {
    tick++;
    if (x<n) {
      x=x+1; y=y+1;
    }
    else if (y<m) {
      x=x+1; y=y+1;
    }
    else
      break;
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
