// C4B output: Not Available
// SPEED output: mx(n,n-m)

#include "tick.h"

void start(int n, int m, int dir)
{
	int i;

	assume(0 < m);
	assume(m < n);

	i = m;
	
	while (0 < i && i < n) {
		tick(1);
		if (dir == 1)
			i++;
		else
			i--;
	}
}

int run(int n, int m, int dir) {
	start(n, m, dir);
    __VERIFIER_print_hull(__cost);
    __VERIFIER_print_hull(__hwm);
}

int main() 
{
	init_tick(0);
	
	int dir = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();
	int n = __VERIFIER_nondet_int();
	
	run(n, m, dir);
	
	int bnd = (n > ( n - m)) ? n : (n - m);
	//assert (__cost <= bnd);
	return 0;
}
