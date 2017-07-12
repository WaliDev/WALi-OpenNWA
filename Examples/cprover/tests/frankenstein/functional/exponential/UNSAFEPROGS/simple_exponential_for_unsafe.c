#include "popl18-assert.h"

//x' = 2*x + 1  x_0 = 1
//x' = 2^(n+1) - 1
#include "pow.h"

DECLARE_POW(2);

int x = 1;
void func(int n) {
    for(int i = 0; i<n; i++){
        x = 2*x + 1;
    }
}

int main()
{
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assume(n > 0);
    func(n);
    assert(x  == icra_pow2(n) - 1);
}
