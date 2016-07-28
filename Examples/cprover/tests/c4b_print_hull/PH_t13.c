// C4B output: 2|[0,x]|+|[0,y]|

#include "tick.h"

void start(int x, int y)
{
	while (x > 0) {
		x = x - 1;
		if (__VERIFIER_nondet_int()) {
			tick(1);
			y = y + 1;
		}
		else {
			while (y > 0) {
				tick(1);
				y = y - 1;
			}
		}
	}

    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);

	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();

	start(x, y);

	int bnd = 2 * ((x > 0) ? x : 0) + ((y > 0) ? y : 0);
	//assert(__cost <= bnd);
	
	return 0;
}
