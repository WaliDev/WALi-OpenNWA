// C4B output: 50+|[-1,i]|+|[0,k]|

#include "tick.h"

void start(int i, int k)
{
	while (i > 100) {
		tick(1);
		i--;
	}
	i = i + k + 50;
	while (i >= 0) {
		tick(1);
		i--;
	}
}

void run(int i, int k)
{
    start(i,k);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);

	int i = __VERIFIER_nondet_int();
	int k = __VERIFIER_nondet_int();

	run(i, k);
	
	int bnd = 50 + ((i > -1) ? (i + 1) : 0) + ((k > 0) ? k : 0);
	//assert(__cost <= bnd);
	
	return 0;
}
