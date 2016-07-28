// C4B output: |[0,x]|+|[0,y]|

#include "tick.h"

void start(int x, int y)
{
	int t;

	while (x > 0) {
		tick(1);
		x--;
		t=x, x=y, y=t;
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
	
	int bnd = ((x > 0) ? x : 0) + ((y > 0) ? y : 0);
	//assert(__cost <= bnd);
	
	return 0;
}
