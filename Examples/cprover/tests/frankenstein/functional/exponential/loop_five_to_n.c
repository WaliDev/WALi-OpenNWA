#include "popl18-assert.h"

//x' = 5*x + 3  x_0 = 1
//x' = 1/4(7*5^n - 3)
#include "pow.h"

DECLARE_POW(5);

int x = 1;
void func(int n) {
    while(n>0){      
        n = n - 1;
        x = 5*x + 3;
    }
}

int main()
{
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assume(n > 0);
    func(n);
    assert(x * 4 == 7 * icra_pow5(n) - 3);
}
