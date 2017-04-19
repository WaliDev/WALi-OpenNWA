#include "assert.h"
int i,j,n,m,k;

void rec2() {
    if (j < m) {
        k ++;
        j++;
        rec2();
    }
}

void rec1() {
    if (i < n) {
        j = 0;
        rec2();
        i++;
        rec1();
    }
}

int main() {
    n = __VERIFIER_nondet_int();
    m = __VERIFIER_nondet_int();
    k = 0;
    i,j;
    __VERIFIER_assume(10 <= n && n <= 10000);
    __VERIFIER_assume(10 <= m && m <= 10000);
    i = 0;
    rec1();
    //for (i = 0; i < n; i++) {
	//for (j = 0; j < m; j++) {
	//    k ++;
	//}
    //}
    __VERIFIER_assert(k >= 100);
    return 0;
}
