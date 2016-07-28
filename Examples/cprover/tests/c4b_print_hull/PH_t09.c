// C4B output: |[0, x]|

#include "tick.h"

void start(int x) {
	int i,j;
	i = 1, j = 0;
	while (j < x) {
		tick(1);
		j++;
		if (i >= 4) {
			i = 1;
		} 
		else
			i++;
	}
}

void run(int x) 
{
    start(x);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);

	int x = __VERIFIER_nondet_int();
	
	run(x);
	
	int bnd = (x > 0) ? x : 0;
	//assert (__cost <= bnd);
	return 0;
}
