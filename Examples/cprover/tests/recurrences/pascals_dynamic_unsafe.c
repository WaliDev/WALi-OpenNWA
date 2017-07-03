//P(i, j) = 1 if i=0
//P(i, j) = 0 if i>j
//P(i,j) = P(i, j-1) + P(i-1, j-1) otherwise
//P(i,j) = j_Choose_i

#include <stdlib.h>
#include "tick.h"


int** triangle;

void initialize_triangle(int n){
    triangle = (int**) malloc(n * sizeof(int*));
    int i, j;
    for (i=0; i<n; i++) {
        triangle[i] = (int*) malloc(n * sizeof(int*));
        for (j=0; j<n; j++) {
            triangle[i][j] = -1;
        }
    }
}

int P(int i, int j) {
    if (i<0 || j<0 || i<j) {
        triangle[i][j] = 0;
        return 0;
    }
    if (j==0) {
        tick(1);
        __cost++;
        triangle[i][j] = 1;
        return 1;
    }
    if (triangle[i][j] != -1) {
        tick(triangle[i][j]);
        return triangle[i][j];
    }
    int ans = P(i-1, j) + P(i-1, j-1);
    triangle[i][j] = ans;
    return ans;
}

int main() {
    init_tick(0);
    initialize_triangle(10);
    P(9,4);
    assert(__cost == 100);
    return 0;
}
