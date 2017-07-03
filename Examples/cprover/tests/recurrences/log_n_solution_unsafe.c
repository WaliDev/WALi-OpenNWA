//t(n) = t(n/2) + 1
//t(n) = log_2_(2n), t(1) = 1

#include "tick.h"

void func(int n)
{
    tick(1);
    if(n == 1)
    {
        return;
    }
    func(n/2);
}

int main()
{
    init_tick(0);
    
    func(2048);
    //can't do this assert. need a way to verify logs
    assert(__cost == 10);
    return 0;
}
