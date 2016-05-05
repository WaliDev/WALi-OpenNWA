#include <stdlib.h>

float * calloc_ut_noindex(int numPoints) {
    int size;
    float * arr;

    float * p;
    
    size = (numPoints * (numPoints+1)) / 2;
    arr = malloc(sizeof(float) * size);
    
    for(p = arr; p < &arr[size]; p++) {
        *p = __VERIFIER_nondet_float();
    }
    return arr;
}

// getElementUT(arr, numPoints, row, column)
//   returns the address of the element at (row,column) in an array that
//   represents an upper-triangular matrix having numPoints columns.
//
// This function is needed because (I think) Duet does not store information
// about the contents of arrays, so it cannot understand the invariant of the
// index array.  With this function, we bypass the index array entirely.
//
//float * getElementUT(float * arr, int numPoints, int row, int column) {
//    return arr + (row * (2 * numPoints - row - 1)) / 2 + column;
//}
//#define getElementUT(arr,numPoints,row,column)   \
//     ((arr) + ((row) * (2 * (numPoints) - (row) - 1)) / 2 + (column))
#define getElementUT(arr,numPoints,row,column)   \
     ((arr) + (numPoints)*(row) + (column) - ((row) * ((row) + 1)) / 2)
//#define assertUT(numPoints,row,column) \
//    assert(((row) * (2 * (numPoints) - (row) - 1)) / 2 + (column) \
//    < \
//    ((numPoints)*((numPoints)+1)/2))
#define assertUT(numPoints,row,column) \
    assert( (2*(numPoints)*(row) + 2*(column) - ((row) * ((row) + 1))  \
    < \
    ((numPoints)*((numPoints)+1)) ) && \
     0 <= (row) && (row) <= (column) && (column) < (numPoints) \
            )
//#define assertUT(numPoints,row,column) \
//    assert( ( ((row) * (2 * (numPoints) - (row) - 1)) + 2*(column) \
//    < \
//    ((numPoints)*((numPoints)+1)) ) && \
//     0 <= (row) && (row) <= (column) && (column) < (numPoints) \
//            )

void multiply(float * m1, float * m2, float * ans, int numPoints) {
    int r, c, k;
    float temp;

    __VERIFIER_assume(numPoints > 0);
    for(r = 0; r < numPoints; r++) {
        for(c = r; c < numPoints; c++) {
            // Note: as of 2016-05-04, we are not able to prove these assertUT statements.
            temp = 0.0;
            for(k = r; k <= c; k++) {
                assertUT(numPoints,r,k);
                assertUT(numPoints,k,c);
                //Think: temp += m1[r][k] * m2[k][c];
                temp += (*getElementUT(m1,numPoints,r,k)) * (*getElementUT(m2,numPoints,k,c));
            }
            assertUT(numPoints,r,c);
            //Think: ans[r][c] = temp;
            *getElementUT(ans,numPoints,r,c) = temp;
        }
    }

}

int main(int argc, char ** argv) {
    int numPoints = 5;
    //int numPoints = __VERIFIER_nondet_int();
    __VERIFIER_assume(numPoints > 0);

    float * m1, * m2, * ans;

    m1  = calloc_ut_noindex(numPoints);
    m2  = calloc_ut_noindex(numPoints);
    ans = calloc_ut_noindex(numPoints);

    multiply(m1,m2,ans,numPoints);
    
    return 0;
}

