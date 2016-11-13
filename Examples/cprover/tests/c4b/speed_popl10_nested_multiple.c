#include "assert.h"

// C4B output: |[x,n]|+|[y,m]|

#include "tick.h"

void start(int x, int n, int y, int m)
{
	while (x < n) {
		while (y < m) {
			if (__VERIFIER_nondet_int())
				break;
			y = y + 1;
			tick(1);
		}
		x = x + 1;
	}
}

int main() 
{
	init_tick(0);

	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();
	int n = __VERIFIER_nondet_int();
	int m = __VERIFIER_nondet_int();

	start(x, n, y, m);
	
	int bnd = ((n > x) ? (n - x) : 0) + ((m > y) ? (m - y) : 0);
	__VERIFIER_assert(__cost <= bnd);
	
	return 0;
}
