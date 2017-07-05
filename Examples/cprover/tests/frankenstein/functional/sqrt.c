// https://github.com/Stevendeo/Pilat/blob/master/test/sqrt.c
#include "assert.h"
int sqrt(int n){
  
   int a,su,t;

    a=0;
    su=1;
    t=1;

    while ( su <= n ) {
        a=a+1;
        t=t+2;
        su=su+t;
    }
    
    return a;
    
}

void main(){
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assume(n >= 1);
    __VERIFIER_assume(LARGE_INT > n);
    int n_sqrt = sqrt(n);
    __VERIFIER_assert(n_sqrt * n_sqrt <= n);
    __VERIFIER_assert(n <= (n_sqrt + 1) * (n_sqrt + 1));
}
