//t(n) = 2t(n/2) + 10n
//t(n) = n + 10*n*log_2_(n), t(1) = 1

#include "tick.h"

void func(int n)
{ 
    if(n == 1)
    {
        tick(1);
        return;
    }
    func(n/2);
    func(n/2);
    tick(10*n);
}

int main()
{
    init_tick(0);
    
    func(256);
    //can't verify since can't evaluate logs
    assert(__cost <= 100);
    return 0;
}
