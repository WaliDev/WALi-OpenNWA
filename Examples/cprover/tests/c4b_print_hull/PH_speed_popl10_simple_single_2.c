// C4B output: |[0,n]|+|[0,m]|

#include "tick.h"

void start(int n, int m)
{
	int x = 0;
	int y = 0;

	for (;;) {
		if (x < n) {
			x = x + 1; 
			y = y + 1;
		}
		else if (y < m) {
			x = x + 1; 
			y = y + 1;
		}
		else
			break;
		tick(1);
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
