// C4B output: |[0,y]|

#include "tick.h"

int x, y;

void produce() {
	while (x > 0) {
		tick(-1);
		x--; y++;
	}
}

void consume() {
	while (y > 0) {
		tick(1);
		y--; x++; 
	}
}

void start(int y, int z) {
	consume(); produce(); consume();
}

void run(int y, int z)
{
    start(y,z);
    __VERIFIER_print_hull(__cost);
}

int main() 
{
	init_tick(0);

	x = __VERIFIER_nondet_int();
	y = __VERIFIER_nondet_int();
	int z = __VERIFIER_nondet_int();

	int bnd = (y > 0) ? y : 0;
	
	run(y, z);
	
	//assert(__cost <= bnd);
	
	return 0;
}
