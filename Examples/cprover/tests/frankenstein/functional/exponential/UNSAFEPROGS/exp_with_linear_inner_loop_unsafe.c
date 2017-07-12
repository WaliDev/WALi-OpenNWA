#include "popl18-assert.h"

//x_(n+1) = 2*x_n + n,  x_0 = 1
//x' = 2^(n+1) - n - 1
#include "pow.h"

DECLARE_POW(2);

int x = 1;
void func(int n) {
    for(int i = 0; i<n; i++){
        int k = 0;
        for (int j = 0; j<i; j++){
          k++;
        }  
        x = 2*x + k;
    }
}

int main()
{
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assume(n > 0);
    func(n);
    assert(x  == 2 * icra_pow2(n) - 1);
}
