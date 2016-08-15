// C4B output: |[0,n]|

#include "tick.h"

void start(int n)
{
	int x = 0;

	while (x < n) {
		x = x + 1;
		while (x < n) {
			tick(1);
			if (__VERIFIER_nondet_int())
				break;
			x = x + 1;
		}
	}
}

void run(int n)
{
    start(n);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);

	int n = __VERIFIER_nondet_int();
	
	run(n);
	
	int bnd = (n > 0) ? n : 0;
	//assert (__cost <= bnd);
	
	return 0;
}

