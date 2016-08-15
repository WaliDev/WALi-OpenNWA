// C4B output: |[0,n]|

#include "tick.h"

void start(int n)
{
	while (n > 0) {
		n = n - 1;
		while (n > 0) {
			tick(1);
			if (__VERIFIER_nondet_int()) 
				break;
			n = n - 1;
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
	//assert(__cost <= bnd);
	return 0;
}
