// C4B output: 101|[0,x]|

#include "tick.h"

void start(int x, int y)
{
	int z;

	// assert(y >= 0);

	while (x > y) {
		x -= y + 1;
		z = 100 + 2 * y;
		while (z > 0) {
			tick(1);
			z--;
		}
	}
}

void run(int x, int y)
{
    start(x,y);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);

	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();
	__VERIFIER_assume(y >= 0);

	run(x, y);
	
	int bnd = 101 * ((x > 0) ? x : 0);
	//assert(__cost <= bnd);
	
	return 0;
}
