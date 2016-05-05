// C4B output: |[x,n]|+|[z,n]|

unsigned int tick = 0;

void start(int x, int z, int n)
{
  while (x<n) {
	tick++;
    if (z>x)
      x=x+1;
    else
      z=z+1;
  }
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	int z = __VERIFIER_nondet_int();
	int n = __VERIFIER_nondet_int();
	
	start(x, z, n);
	
	int bnd = ((n>x)?(n-x):0) + ((n>z)?(n-z):0);
	assert(tick<=bnd);
	return 0;
}
