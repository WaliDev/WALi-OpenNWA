//t(n) = 2 * t(n-2) t(0), t(1) = 1
//t(n) = 2^((n-3)/2) * ((sqrt(2) - 1)(-1)^n + 1 + sqrt(2))

#include "tick.h"

void func(int n)
{ 
    if(n == 0 || n == 1)
    {
        tick(1);
        return;
    }
    func(n-2);
    func(n-2);
}

int main()
{
    init_tick(0);
    func(10);
    //can't evaluate since can't evaluate square roots
    assert(__cost == 32);
    return 0;
}
