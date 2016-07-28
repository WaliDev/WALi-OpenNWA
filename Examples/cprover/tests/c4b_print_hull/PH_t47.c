// C4B output: 1+|[0,n]|

#include "tick.h"

void start(int n)
{
	int flag = 1;

	while (flag > 0) {
		tick(1);
		if (n > 0) {
			n--;
			flag = 1;
		} else
			flag = 0;
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
	
	int bnd = 1 + ((n > 0) ? n : 0);
	//assert (__cost <= bnd);
	
	return 0;
}
