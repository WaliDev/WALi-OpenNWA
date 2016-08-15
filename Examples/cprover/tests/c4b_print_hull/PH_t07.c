// C4B output: 1+3|[0,x]|+|[0,y]|

#include "tick.h"

void fish() { return; }

void start(int x, int y)
{
	while (x > 0) {
		tick(1);
		x = x - 1;
		y = y + 2;
	}
	while (y > 0) {
		tick(1);
		y = y - 1;
	}
	fish();
	while (y > 0) {
		tick(1);
		y = y - 1;
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

	run(x, y);
	
	int bnd = 1 + 3 * ((x > 0) ? x : 0) + ((y > 0) ? y : 0);
	//assert(__cost <= bnd);
	
	return 0;
}
