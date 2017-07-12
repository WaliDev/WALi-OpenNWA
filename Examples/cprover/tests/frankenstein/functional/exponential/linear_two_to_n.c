#include "popl18-assert.h"

//t(n) = 2 * t(n-1) t(0) = 1
//t(n) = 2^n

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
}

int main()
{
    init_tick(0);
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assume(n > 0);
    func(n);
    //func(n);
    assert(__cost == icra_pow2(n));
    //assert(__cost == 1<<n)
    return 0;
}
