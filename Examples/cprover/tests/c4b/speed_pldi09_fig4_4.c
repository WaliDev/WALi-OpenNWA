#include "assert.h"

//// C4B output : |[0,n]|

#include "tick.h"

void start(int n, int m)
{
	int i=n;

	//replaced this with the assumption in main
	//assert(0 < m);

	while (i > 0) {
		tick(1);
		if (i < m)
			i=i-1;
		else
			i=i-m;
	}
}

int main() 
{
	init_tick(0);

	int n = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();
	__VERIFIER_assume(m > 0);
		
	start(n, m);
	
	int bnd = n > 0 ? n : 0;
	__VERIFIER_assert(__cost <= bnd);
	return 0;
}
