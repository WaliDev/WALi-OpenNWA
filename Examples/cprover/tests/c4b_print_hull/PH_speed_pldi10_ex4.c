// C4B output: 1+2|[0,n]|

#include "tick.h"

void start(int n)
{
	int flag = 1;

	while (flag > 0) {
		flag = 0;
		while (n > 0) {
			tick(1);
			n = n - 1;			 
			flag = 1;
		}
	}
}

int run(int n)
{
    start(n);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);
	
	int n = __VERIFIER_nondet_int();
	run(n);
	
	int bnd = 1 + 2 * (n > 0 ? n : 0);
	//assert(__cost <= bnd);
	return 0;
}
