//#include <stdio.h>

int main() {
    int x, y,i,j;

    x = 20;
    y = i = j = 0;


    for(; i < x; i++) {
	int y_old = y;

	j = 0;
	for (; j < x; j++) {
	    y++;
	}
	assert(y == (y_old + x));
    }

    //printf("\n final y %d\n", y);
    assert(y == x*x);

    return 0;
}
