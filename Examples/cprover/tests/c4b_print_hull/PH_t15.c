// C4B output: |[0,x]|

#include "tick.h"

void start(int x, int y)
{
	int z;

	while (x > y) {
		tick(1);
		x -= y + 1;
		for (z = y; z > 0; z--)
			/* nothing */;
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
	
	int bnd = (x > 0) ? x : 0;
	//assert(__cost <= bnd);
	
	return 0;
}
