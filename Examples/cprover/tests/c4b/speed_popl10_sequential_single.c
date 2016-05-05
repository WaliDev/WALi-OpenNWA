// C4B output: |[0,n]|

int __VERIFIER_nondet_int();
unsigned int tick = 0;

void start(int n)
{
  int x=0;

  while (x<n) {
	tick++;
    if (__VERIFIER_nondet_int()) break;
    x=x+1;
  }
  while (x<n) {
	tick++;
    x=x+1;
  }
}

int main() 
{
	int n = __VERIFIER_nondet_int();
	
	start(n);
	
	int bnd = (n>0)?n:0;
	assert (n <= bnd);
	
	return 0;
}
