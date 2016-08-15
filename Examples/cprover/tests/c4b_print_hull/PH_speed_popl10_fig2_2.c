// C4B output: |[x,n]|+|[z,n]|

#include "tick.h"

void start(int x, int z, int n)
{
	while (x < n) {
		tick(1);
		if (z > x)
			x = x + 1;
		else
			z = z + 1;
	}
}

void run(int x, int z, int n)
{
    start(x,z,n);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);

	int x = __VERIFIER_nondet_int();
	int z = __VERIFIER_nondet_int();
	int n = __VERIFIER_nondet_int();
	
	run(x, z, n);
	
	int bnd = ((n > x) ? (n - x) : 0) + ((n > z) ? (n - z) : 0);
	//assert(__cost <= bnd);
	return 0;
}
