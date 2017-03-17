//d(i, 0) = i
//d(0, j) = j
//d(i,j) = d(i-1, j-1) if a[i]==b[i]
//d(i,j) = min(d(i-1, j), d(i, j-1), d(i-1, j-1)) + 1

#include <stdlib.h>
#include <string.h>

int** costs;

void initialize_matrix(int a, int b){
    costs = (int**) malloc(a * sizeof(int*));
    int i;
    for (i=0; i<a; i++) {
        costs[i] = (int*) malloc(b * sizeof(int*));
    }
}

int minimum(int x, int y, int z) {
    if (x<y) {
        if (z<x) {
            return z;
        } else {
            return x;
        }
    } else {
        if (y<z) {
            return y;
        } else {
            return z;
        }
    }
}

void edit_distance(char *a, char *b) {
    int i,j;
    for(i=0; i<=strlen(a); i++){
        for(j=0; j<=strlen(b); j++){
            if (j==0) {
                costs[i][j] = i;
            } else if (i==0) {
                costs[i][j] = j;
            } else if (a[i-1]==b[j-1]) {
                costs[i][j] = costs[i-1][j-1];
            } else {
                costs[i][j] = minimum(costs[i-1][j], costs[i][j-1], costs[i-1][j-1]) + 1;
            }
        }
    }
}

int main() {
    char a[10] = "saturday";
    char b[10] = "sunday";
    initialize_matrix(strlen(a) + 1, strlen(b) + 1);
    edit_distance(a, b);
    assert(costs[strlen(a)][strlen(b)] >= 0);
}
