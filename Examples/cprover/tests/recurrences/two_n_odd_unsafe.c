//t(n) = t(n-2) + 1, t(0), t(1) = 1
//t(n) = 1/4(2n + (-1)^n + 3)

#include "tick.h"

void func(int n)
{ 
    tick(1);
    if(n == 0 || n == 1)
    {
        return;
    }
    func(n-2);
}

int main()
{
    init_tick(0);
    func(51);
    assert(__cost == 20);
    return 0;
}
