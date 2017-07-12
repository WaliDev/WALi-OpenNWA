#include "popl18-assert.h"

//x' = 2*x + 1  x_0 = 1
//x' = 2^(n+1) - 1
#include "pow.h"

DECLARE_POW(2);

int func(int n){
    if (n<=0){
        return 1;
    }
    return 2*func1(n-1) + n-1;
}

int main()
{
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assume(n > 0);
    int x = func(n);
    assert(x  == 2 * icra_pow2(n) - 1);
}
