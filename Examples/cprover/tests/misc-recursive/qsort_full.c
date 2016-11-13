#include "assert.h"
// adapted from C4B example "qsort.c"
#define RANDOM_INCLUSIVE(L, H) __t = __VERIFIER_nondet_int(); \
                           __VERIFIER_assume(__t >= L && __t <= H)
#define SWAP_ELEMENTS(X,Y) __VERIFIER_assert(X >= low && X < hi && Y >= low && Y < hi); \
                           __t=a[X]; a[X]=a[Y]; a[Y]=__t 
#define ASSERT_IN_BOUNDS(X) __VERIFIER_assert(X >= low && X < hi)
int length;

// Sort the subregion of a at index positions [low,hi)
void quicksort(int *a, int low, int hi) {
    int __t, pivot, pivotIndex, i, j;

    __VERIFIER_assert(0 <= low && hi <= length);

    if (hi - low < 2) return;

    pivotIndex = RANDOM_INCLUSIVE(low, hi-1);
    
    SWAP_ELEMENTS(hi-1, pivotIndex);
   
    ASSERT_IN_BOUNDS(hi-1);
    pivot = a[hi-1];
    
    i = low-1;
    for(j = low; j < hi-1; j++) {
        ASSERT_IN_BOUNDS(j);
        if (a[j] <= pivot) {
            i = i+1;
            SWAP_ELEMENTS(i, j);
        }
    }

    SWAP_ELEMENTS(i+1,hi-1);
    
    quicksort(a, low, i+1);
    quicksort(a, i+2, hi);
}

int main(int argc)
{
    int * a;
    int i;

    length = argc;
    __VERIFIER_assume(length > 0);

    a = malloc(length * sizeof(int));

    for (i = 0; i < length; i++)
        a[i] = __VERIFIER_nondet_int();
    quicksort(a, 0, length);
    return 0;
}
