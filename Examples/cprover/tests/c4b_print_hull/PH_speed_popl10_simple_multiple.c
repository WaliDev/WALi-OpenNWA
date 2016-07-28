// C4B output : |[0,m]|+|[0,n]|

#include "tick.h"

void start(int n, int m)
{
	int x = 0;
	int y = 0;

	while (x < n) {
		tick(1);
		if (y < m)
			y = y + 1;
		else
			x = x + 1;
	}
}

void run(int n, int m)
{
    start(n,m);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);
	
	int n = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();

	run(n, m);
	
	int bnd = ((n > 0) ? n : 0) + ((m > 0) ? m : 0);
	//assert(__cost <= bnd);
	
	return 0;
}
