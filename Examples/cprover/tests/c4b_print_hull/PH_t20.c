// C4B output: |[x,y]|+|[y,x]|

#include "tick.h"

void start(int x, int y)
{
	while (x < y) {
		x++;
		tick(1);
	}
	while (y < x) {
		y++;
		tick(1);
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
	
	int bnd = (x > y) ? (x - y) : (y - x);
	//assert(__cost <= bnd);
	
	return 0;
}
