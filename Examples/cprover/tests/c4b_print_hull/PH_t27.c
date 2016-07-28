// C4B output: 0.01|[n,y]|+11|[n,0]|

#include "tick.h"

void start(int n, int y)
{
	while (n < 0) {
		n++;
		y += 1000;
		while (y >= 100) {
			tick(1);
			y -= 100;
		}
	}
}

void run(int n, int y)
{
    start(n,y);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);

	int n = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();

	run(n, y);
	
	double bnd = 0.01 * ((y > n) ? (y - n) : 0) + 11 * ((n < 0) ? -n : 0);
	//assert(__cost <= bnd);
	
	return 0;
}
