//#include<stdio.h>

int main() {
    int x, y, z;
   
    x = 0;
    y = 0;
    z = 2*x;

    while(x <= 10) {
	x = x + 1;
	y = y + x;
	z = 2 * x;
        //printf("x %d\n", x);
        //printf("y %d\n", y);
    }

    assert(y == (x*(x+1)/2));
    return 0;
}
