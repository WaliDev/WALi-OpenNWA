//t(n) = 2 * t(n-1) + n-1,  t(0) = 1
//t(n) = 2^(n+1)-n-1

#include "tick.h"
#include "pow.h"

DECLARE_POW(2);


void func(int n)
{ 
    if(n == 0)
    {
        tick(1);
        return;
    }
    func(n-1);
    func(n-1);
    tick(n-1);
}

int main()
{
    init_tick(0);
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assum(n > 0);
    func(n);
    assert(__cost == 2*icra_pow2(n) - 1);
    return 0;
}
