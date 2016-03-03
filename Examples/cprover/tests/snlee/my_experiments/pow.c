//#include <stdio.h>
//#include <stdlib.h>


extern int __VERIFIER_nondet_int();


int tick;

int power(int x, int y) {
    int ret = 1;
    int i;
    
    tick = 0;

    for(i = 0; i < y; i++) {
	ret = ret * x;
	tick++;
    }
    
    return ret;
}

int main() {
    int x, y, z;

    //srand(time(NULL));

    x = __VERIFIER_nondet_int();//rand() % 20;
    y = __VERIFIER_nondet_int();//rand() % 20;

    //printf("x: %d y: %d", x, y);


    assume(y >=0);

    //tick = 0;
    z = power(x,y);

    //printf(" tick: %d\n", tick);

    assert(tick == y);

    return 0;
}
