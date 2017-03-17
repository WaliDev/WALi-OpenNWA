//t(m,n) = 2t(m/2, n/2) + mn, t(1, n) = n t(m,1) = m
//t(m,n) = 2mn - m if m>=n
//t(m,n) = 2mn - n if m<n

#include "tick.h"

void func(int m, int n)
{ 
    if(m == 1)
    {
        tick(n);
        return;
    }
    if(n == 1) 
    {
        tick(m);
        return;
    }
    func(m/2, n/2);
    func(m/2, n/2);
    tick(m*n);
}

int main()
{
    init_tick(0);
    func(32, 16);
    assert(__cost == 992);
    return 0;
}
