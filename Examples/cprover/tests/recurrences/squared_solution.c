//t(n) = 2t(n/2) + n^2, t(1) = 2
//t(n) = 2n^2

#include "tick.h"

void func(int n)
{ 
    if(n == 1)
    {
        tick(2);
        return;
    }
    func(n/2);
    func(n/2);
    tick(n*n);
}

int main()
{
    init_tick(0);
    
    func(32);
    assert(__cost == 2048);
    return 0;
}
