//t(n) = 5*t(n-1) + 3, t(0) = 1
//t(n) = 1/4(7*5^n - 3)

#include "tick.h"
#include "pow.h"

DECLARE_POW(5);

void func(int n)
{ 
    if(n == 0)
    {
        tick(1);
        return;
    }
    func(n-1);
    func(n-1);
    func(n-1);
    func(n-1);
    func(n-1);
    tick(3);
}

int main()
{
    init_tick(0);
    int n = __VERIFIER_nondet_int();
    func(n);
    assert(__cost * 4 == 7 * icra_pow5(n));
}
