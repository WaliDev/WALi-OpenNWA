// C4B output : |[0,n]|

int __VERIFIER_nondet_int();
unsigned int tick = 0;

void start(int n)
{
  int i;
  int j;

  i=0;
  while (i<n) {
    j=i+1;
    while (j<n) {
	  if (__VERIFIER_nondet_int()) {
        tick++;
        j=j-1;
        n=n-1;
      }
      j=j+1;
    }
    i=i+1;
  }
}

int main() 
{
	int n = __VERIFIER_nondet_int();
	start(n);
	
	int bnd = n>0?n:0;
	assert(tick <= bnd);
	return 0;
}
