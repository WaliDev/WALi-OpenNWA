// C4B output: |[x,n]|+|[y,m]|

#include "tick.h"

void start(int x, int y, int n, int m)
{
	while (n > x) {
		tick(1);
		if (m > y) 
			y = y + 1;
		else
			x = x + 1;
	}
}

void run(int x, int y, int n, int m)
{
    start(x,y,n,m);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);

	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();
	int n = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();
	
	run(x, y, n, m);
	
	int bnd = ((n > x) ? (n - x) : 0) + ((m > y) ? (m - y) : 0);
	//assert(__cost <= bnd);
	
	return 0;
}
