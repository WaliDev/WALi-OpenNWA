// C4B output: 1+2|[0,n]|

#include "tick.h"

void start(int n, int m)
{
	int va = n;
	int vb = 0;

/* assert(n > 0); */
// assert(m > 0);

	while (va > 0) {
		tick(1);
		if (vb < m) { 
			vb = vb + 1; 
			va = va - 1;
		} else {
			vb = vb - 1;
			vb = 0;
		}
	}
}

void run(int n, int m)
{
    start(n,m);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);
	int n = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();
	__VERIFIER_assume(m > 0);
	
	run(n, m);
	
	int bnd = 1 + 2 * (n > 0 ? n : 0);
	assert(__cost <= bnd);
	return 0;
}
