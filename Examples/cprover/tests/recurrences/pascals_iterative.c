//P(i, j) = 1 if i=0
//P(i, j) = 0 if i>j
//P(i,j) = P(i, j-1) + P(i-1, j-1) otherwise
//P(i,j) = j_C_i

#include <stdlib.h>

int** triangle;

void initialize_matrix(int n){
    triangle = (int**) malloc(n * sizeof(int*));
    int i;
    for (i=0; i<n; i++) {
        triangle[i] = (int*) malloc(n * sizeof(int*));
    }
}

void populate_triangle(int n) {
    int i,j;
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            if (j==0) {
                triangle[i][j] = 1;
            } else if (i<j) {
                triangle[i][j] = 0;
            } else {
                triangle[i][j] = triangle[i-1][j] + triangle[i-1][j-1];
            }
        }
    }
}

int main() {
    int size = 10;
    initialize_matrix(size);
    populate_triangle(size);
    assert(triangle[9][4] == 126);
}
