// C4B output: 1+2|[0,n]|

unsigned int tick = 0;

void start(int n)
{
  int flag=1;

  while (flag>0) {
    flag=0;
    while (n>0) {
	  tick++;
      n=n-1;       
      flag=1;
    }
  }
}

int main() 
{
	int n = __VERIFIER_nondet_int();
	start(n);
	
	int bnd = 1 + 2 * (n>0?n:0);
	assert(tick <= bnd);
	return 0;
}
