// C4B output: 0.01|[n,y]|+11|[n,0]|

unsigned int tick = 0;

void start(int n, int y)
{
  while (n<0) {
    n++;
    y+=1000;
    while (y>=100) {
      tick++;
	  y-=100;
	}
  }
}

int main() 
{
	int n = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();

	start(n, y);
	
	double bnd = 0.01 * ((y>n)?(y-n):0) + 11 * ((n<0)?-n:0);
	assert(tick <= bnd);
	
	return 0;
}
