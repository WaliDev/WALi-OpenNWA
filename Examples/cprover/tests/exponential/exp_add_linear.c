//x_(n+1) = 2*x_n + n,  x_0 = 1
//x' = 2^(n+1) - n - 1
#include "pow.h"

DECLARE_POW(2);

int x = 1;
void func(int n) {
    //x = 1; adding this line causes a Z3 invalid rational error
    int k = 0;
    for(int i = 0; i<n; i++){
        x = 2*x + k;
	k++;
    }
    __VERIFIER_print_hull(x);
}

int main()
{
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assume(n > 0);
    func(n);
    assert(x  == 2 * icra_pow2(n) - n - 1);
}
