//t(n) = t(n-2) + n - 1, t(0), t(1) = 1
//t(n) = 1/8(2n^2 + (-1)^n + 7)

#include "tick.h"

void func(int n)
{ 
    if(n == 0 || n == 1)
    {
        tick(1);
        return;
    }
    func(n-2);
    tick(n-1);
}

int main()
{
    init_tick(0);
    func(26);
    assert(__cost == 100);
    return 0;
}
